#include "../include/ftp_server.hh"
pthread_mutex_t client_paths_mutex; // Mutex for protecting client_paths access

/* Initialize the mutex */
void mutex_init() {
    pthread_mutex_init(&client_paths_mutex, NULL);
}

/* Open a listening socket on the specified port */
int open_listenfd(char *port) {
    struct addrinfo hints, *listp, *p;
    int listen_fd, optval = 1;

    /* Set up the hints structure for getaddrinfo */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;   // TCP socket
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;

    /* Get list of potential server addresses */
    if (getaddrinfo(NULL, port, &hints, &listp) != 0) {
        return -1;
    }

    /* Try each address and bind the first we can */
    for (p = listp; p; p = p->ai_next) {
        /* Create socket */
        if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;

        /* Set socket options to reuse address */
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        /* Bind socket to the address */
        if (bind(listen_fd, p->ai_addr, p->ai_addrlen) != -1)
            break;

        /* Close socket on failure and try next address */
        close(listen_fd);
    }

    freeaddrinfo(listp);
    if (!p)
        return -1;

    /* Start listening on the socket */
    if (listen(listen_fd, LISTENQ)) {
        close(listen_fd);
        return -1;
    }

    return listen_fd;
}

/* Safely send data over a socket, ensuring all bytes are transmitted */
ssize_t safe_send(int fd, const void *buf, size_t n, int flags) {
    ssize_t ret = 0;
    while (ret < n) {
        ssize_t b = send(fd, (const char *)buf + ret, n - ret, flags);
        if (b == 0) {
            cout << "Connection closed" << endl;
            return -1;
        }
        if (b < 0) {
            cout << "Send error" << endl;
            return -1;
        }
        ret += b;
    }
    return ret;
}

/* Safely receive data over a socket, ensuring all bytes are received */
ssize_t safe_recv(int fd, void *buf, size_t n, int flags) {
    ssize_t ret = 0;
    while (ret < n) {
        ssize_t b = recv(fd, (char *)buf + ret, n - ret, flags);
        if (b == 0) {
            cout << "Connection closed" << endl;
            return -1;
        }
        if (b < 0) {
            cout << "Recv error" << endl;
            return -1;
        }
        ret += b;
    }
    return ret;
}

/* Pack data into a myFTPMessage structure */
void packing(myFTPMessage *msg, uint8_t type, uint8_t status, uint32_t length) {
    memcpy(msg->m_protocol, protocol, sizeof(protocol));
    msg->m_type = type;                                  
    msg->m_status = status;          
    msg->m_length = htonl(length);  // Set message length in network byte order
}

/* Handle client requests */
void doit(int connect_fd) {
    myFTPMessage MSG;

    while (1) {
        /* Receive a message from the client */
        safe_recv(connect_fd, &MSG, sizeof(myFTPMessage), 0);

        /* Process message based on its type */
        switch (MSG.m_type) {
            case 0xA1: // Open connection
                myftp_open(connect_fd);
                break;
            case 0xA3: // List directory
                myftp_ls(connect_fd);
                break;
            case 0xA5: // Change directory
                myftp_cd(connect_fd, MSG.m_length);
                break;
            case 0xA7: // Get file
                myftp_get(connect_fd, MSG.m_length);
                break;
            case 0xA9: // Put file
                myftp_put(connect_fd, MSG.m_length);
                break;
            case 0xAB: // Compute SHA-256
                myftp_sha256(connect_fd, MSG.m_length);
                break;
            case 0xAD: // Quit connection
                myftp_quit(connect_fd);
                return;
        }
    }
}

/* Execute a shell command and return its output as a string */
string execute_cmd(const char *command) {
    char buf[MAXLINE] = {0};
    string result = "";
    FILE *pipe = popen(command, "r"); // Open a pipe to the shell command
    if (!pipe)
        throw runtime_error("popen() failed!");

    try {
        while (fgets(buf, sizeof(buf), pipe) != nullptr) {
            result += buf; // Append each line of output to the result
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe); // Close the pipe
    return result;
}