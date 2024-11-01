#include "../include/ftp_client.hh"

void eval(const string &cmdline) {
    cmdline_tokens tok;

    /* Parse the command line and populate the tok structure */
    if (parseline(cmdline.c_str(), &tok) == -1) {
        cerr << "Error: invalid command" << endl;
        return;
    }

    /* If no command was found, return immediately */
    if (tok.argv[0] == NULL)
        return;

    /* Execute the appropriate command based on the parsed command type */
    switch (tok.builtins) {
        case cmdline_tokens::BUILTIN_cd: 
            myftp_cd(&tok); break;
        case cmdline_tokens::BUILTIN_get: 
            myftp_get(&tok); break;
        case cmdline_tokens::BUILTIN_ls: 
            myftp_ls(&tok); break;
        case cmdline_tokens::BUILTIN_open: 
            myftp_open(&tok); break;
        case cmdline_tokens::BUILTIN_put: 
            myftp_put(&tok); break;
        case cmdline_tokens::BUILTIN_quit: 
            myftp_quit(&tok); break;
        case cmdline_tokens::BUILTIN_sha256: 
            myftp_sha256(&tok); break;
        default: 
            cerr << "Unknown command: " << tok.argv[0] << endl; // Handle unknown commands
    }
    return;
}

int parseline(const char *cmdline, struct cmdline_tokens *tok) {
    static char array[MAXLINE]; 
    const char delims[10] = " \t\r\n";
    char *buf = array;
    char *next;                 
    char *endbuf;
    int parsing_state;

    if (cmdline == NULL) {
        cerr << "Error: command line is NULL" << endl;
        return -1;
    }

    strncpy(buf, cmdline, MAXLINE);
    endbuf = buf + strlen(buf);
    tok->infile = NULL;
    tok->outfile = NULL;
    parsing_state = ST_NORMAL;
    tok->argc = 0;

    while (buf < endbuf) {
        buf += strspn(buf, delims);
        if (buf >= endbuf)
            break;
        
        /* I/O redirection */
        if (*buf == '<') {
            if (tok->infile) {
                cerr << "Error: Ambiguous I/O redirection" << endl;
                return -1;
            }
            parsing_state |= ST_INFILE;
            buf++;
            continue;
        }
        if (*buf == '>') {
            if (tok->outfile) {
                cerr << "Error: Ambiguous I/O redirection" << endl;
                return -1;
            }
            parsing_state |= ST_OUTFILE;
            buf ++;
            continue;
        }

        if (*buf == '\'' || *buf == '\"') {
            /* Detect quoted tokens */
            buf++;
            next = strchr(buf, *(buf - 1)); // Find the closing quote
        } else {
            /* Find next delimiter */
            next = buf + strcspn (buf, delims);
        }
        
        if (next == NULL) {
            /* Returned by strchr(); this means that the closing
               quote was not found. */
            cerr << "Error: unmatched" << *(buf - 1) <<endl;
            return -1;
        }

        /* Terminate the token */
        *next = '\0';

        /* Record the token as either the next argument or the I/O file */
        switch (parsing_state) {
        case ST_NORMAL:
            tok->argv[tok->argc++] = buf;
            break;
        case ST_INFILE:
            tok->infile = buf;
            break;
        case ST_OUTFILE:
            tok->outfile = buf;
            break;
        default:
            cerr << "Error: Ambiguous I/O redirection" << endl;
            return -1;
        }
        parsing_state = ST_NORMAL;

        /* Check if argv is full */
        if (tok->argc >= MAXARGS - 1)
            break;

        buf = next + 1;
    }

    tok->argv[tok->argc] = NULL;  // End argv with NULL

    /* Check command type */ 
    if (tok->argc == 0)
        return 1; 
    if (strcmp(tok->argv[0], "cd") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_cd;
    else if (strcmp(tok->argv[0], "get") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_get;
    else if (strcmp(tok->argv[0], "ls") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_ls;
    else if (strcmp(tok->argv[0], "open") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_open;
    else if (strcmp(tok->argv[0], "put") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_put;
    else if (strcmp(tok->argv[0], "quit") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_quit;
    else if (strcmp(tok->argv[0], "sha256") == 0)
        tok->builtins = cmdline_tokens::BUILTIN_sha256;
    else tok->builtins = cmdline_tokens::BUILTIN_NONE;

    return 0;
}

int open_clientfd(char *hostname, char *port) {
    int client_fd;
    struct addrinfo hints, *listp, *p;

    /* Initialize the hints structure for getaddrinfo */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; // Use TCP sockets
    hints.ai_flags = AI_NUMERICSERV; // Service name is numeric (e.g., port number)
    hints.ai_flags |= AI_ADDRCONFIG; // Use IPv4 or IPv6 based on system configuration

    /* Get a list of potential addresses */
    if (getaddrinfo(hostname, port, &hints, &listp) != 0)
        return -1;

    /* Iterate through the list and try to connect */
    for (p = listp; p; p = p->ai_next) {
        /* Attempt to create a socket */
        if ((client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue; // If socket creation fails, try the next address
        
        /* Attempt to connect to the server */
        if (connect(client_fd, p->ai_addr, p->ai_addrlen) != -1)
            break; // Connection successful

        /* If connection fails, close the socket and try the next address */
        close(client_fd);
    }

    /* Free the address list allocated by getaddrinfo */
    freeaddrinfo(listp);

    /* Check if connection was successful */
    if (!p)
        return -1; // All connections failed
    else
        return client_fd; // Return the connected socket file descriptor
}

ssize_t safe_send(int fd, const void *buf, size_t n, int flags) {
    ssize_t ret = 0;

    /* Loop to ensure all data is sent */
    while (ret < n) {
        /* Attempt to send remaining data */
        ssize_t b = send(fd, (const char *)buf + ret, n - ret, flags);
        
        /* Check if the connection was closed */
        if (b == 0) {
            cout << "Connection closed" << endl;
            return -1;
        }
        
        /* Check if there was a sending error */
        if (b < 0) {
            cout << "Send error" << endl;
            return -1;
        }
        
        /* Accumulate the number of bytes successfully sent */
        ret += b;
    }
    
    return ret;
}

ssize_t safe_recv(int fd, void *buf, size_t n, int flags) {
    ssize_t ret = 0;

    /* Loop to ensure all requested data is received */
    while (ret < n) {
        /* Attempt to receive remaining data */
        ssize_t b = recv(fd, (char *)buf + ret, n - ret, flags);

        /* Check if the connection was closed */
        if (b == 0) {
            cout << "Connection closed" << endl;
            return -1;
        }

        /* Check if there was a receive error */
        if (b < 0) {
            cout << "Recv error" << endl;
            return -1;
        }

        /* Accumulate the number of bytes successfully received */
        ret += b;
    }

    return ret;
}


void packing(myFTPMessage *msg, uint8_t type, uint8_t status, uint32_t length){
    memcpy(msg->m_protocol, protocol, sizeof(protocol));
    msg->m_type = type;
    msg->m_status = status;
    /* Set the message length in network byte order (big-endian) */
    msg->m_length = htonl(length);
}

bool same_protocol(myFTPMessage *msg){
    return !memcmp(protocol, msg->m_protocol, MAGIC_NUMBER_LENGTH);
}