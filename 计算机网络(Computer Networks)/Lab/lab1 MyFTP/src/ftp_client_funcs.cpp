#include "../include/ftp_client.hh"

int client_fd = 0;
extern int connect_flag;
char ip[MAXLINE], port[MAXLINE];
char server_dir[MAXLINE];

void myftp_open(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 3) {
        cout << "Usage: open <ip> <port>" << endl;
        return;
    }
    
    /* Copy the IP and port arguments to global variables */
    strncpy(ip, tok->argv[1], strlen(tok->argv[1]) + 1);
    strncpy(port, tok->argv[2], strlen(tok->argv[2]) + 1);
    cout << "Connecting to " << ip << ":" << port << endl;

    /* Open a connection to the specified IP and port */
    if ((client_fd = open_clientfd(ip, port)) == -1) {
        cout << "Connection rejected" << endl;
        return;
    }

    /* Prepare and send a connection request message to the server */
    myFTPMessage OPEN_CONN_REQUEST;
    packing(&OPEN_CONN_REQUEST, 0xA1, -1, 12); // Message type 0xA1 for connection request
    safe_send(client_fd, &OPEN_CONN_REQUEST, 12, 0);

    /* Receive the connection reply from the server */
    myFTPMessage OPEN_CONN_REPLY;
    safe_recv(client_fd, &OPEN_CONN_REPLY, 12, 0);

    /* Check if the reply message has the correct protocol and type */
    if (!same_protocol(&OPEN_CONN_REPLY) || OPEN_CONN_REPLY.m_type != 0xA2) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Update connection status based on server reply */
    if (OPEN_CONN_REPLY.m_status == 1) {
        cout << "Connection accepted" << endl;
        connect_flag = 1; // Set connection flag to indicate successful connection
    } else {
        cout << "Connection rejected" << endl;
    }
}


void myftp_quit(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 1) {
        cout << "Usage: quit" << endl;
        return;
    }

    /* If connected to the server, proceed to close the connection */
    if (connect_flag) {
        /* Prepare and send a quit request message to the server */
        myFTPMessage QUIT_REQUEST;
        packing(&QUIT_REQUEST, 0xAD, -1, 12); // Message type 0xAD for quit request
        safe_send(client_fd, &QUIT_REQUEST, 12, 0);

        /* Receive quit confirmation reply from the server */
        myFTPMessage QUIT_REPLY;
        safe_recv(client_fd, &QUIT_REPLY, 12, 0);

        /* Verify the received message */
        if (!same_protocol(&QUIT_REPLY) || QUIT_REPLY.m_type != 0xAE) {
            cerr << "Error: wrong message from server" << endl;
            return;
        }

        /* Close the client socket and reset the connection flag */
        close(client_fd);
        connect_flag = 0;
    }
    else {
        /* If not connected to the server, exit the client program */
        exit(0);
    }
}


void myftp_ls(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 1) {
        cout << "Usage: ls" << endl;
        return;
    }
    
    /* Ensure client is connected to the server */
    if (!connect_flag) {
        cout << "Please connect to server first" << endl;
        return;
    }

    /* Prepare and send an 'ls' request message to the server */
    myFTPMessage LIST_REQUEST;
    packing(&LIST_REQUEST, 0xA3, -1, 12); // Message type 0xA3 for 'ls' request
    safe_send(client_fd, &LIST_REQUEST, 12, 0);

    /* Receive 'ls' response from the server */
    myFTPMessage LIST_REPLY;
    safe_recv(client_fd, &LIST_REPLY, 12, 0);

    /* Verify the received message */
    if (!same_protocol(&LIST_REPLY) || LIST_REPLY.m_type != 0xA4) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Calculate the length of the file list data and receive it */
    size_t file_names_length = size_t(ntohl(LIST_REPLY.m_length)) - 12;
    char buf[MAXLINE] = {0};
    safe_recv(client_fd, buf, file_names_length, 0);

    /* Output the received file list */
    cout << buf;
}


void myftp_cd(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 2) {
        cout << "Usage: cd <DIR>" << endl;
        return;
    }

    /* Ensure client is connected to the server */
    if (!connect_flag) {
        cout << "Please connect to server first" << endl;
        return;
    }

    /* Prepare the directory name and send 'cd' request to the server */
    char dir_name[MAXLINE] = {0};
    strncpy(dir_name, tok->argv[1], strlen(tok->argv[1]) + 1);
    myFTPMessage CHANGE_DIR_REQUEST;
    packing(&CHANGE_DIR_REQUEST, 0xA5, -1, 12 + strlen(dir_name) + 1); // Message type 0xA5 for 'cd' request
    safe_send(client_fd, &CHANGE_DIR_REQUEST, 12, 0);
    safe_send(client_fd, dir_name, strlen(dir_name) + 1, 0);

    /* Receive the 'cd' reply from the server */
    myFTPMessage CHANGE_DIR_REPLY;
    safe_recv(client_fd, &CHANGE_DIR_REPLY, 12, 0);

    /* Verify the received message */
    if (!same_protocol(&CHANGE_DIR_REPLY) || CHANGE_DIR_REPLY.m_type != 0xA6) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Check server's response status for directory change success */
    if (CHANGE_DIR_REPLY.m_status == 0)
        cerr << tok->argv[1] << ": No such file or directory on server" << endl;
    else {
        /* Update the current directory path on the client side */
        strncpy(server_dir, tok->argv[1], strlen(tok->argv[1]) + 1);
        cout << "Change server's current directory to " << server_dir << endl;
    }
}


void myftp_get(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 2) {
        cout << "Usage: get <filename>" << endl;
        return;
    }

    /* Ensure client is connected to the server */
    if (!connect_flag) {
        cout << "Please connect to server first" << endl;
        return;
    }

    /* Prepare the file name and send 'get' request to the server */
    char file_name[MAXLINE] = {0};
    strncpy(file_name, tok->argv[1], strlen(tok->argv[1]) + 1);
    myFTPMessage GET_REQUEST;
    packing(&GET_REQUEST, 0xA7, -1, 12 + strlen(file_name) + 1); // Message type 0xA7 for 'get' request
    safe_send(client_fd, &GET_REQUEST, 12, 0);
    safe_send(client_fd, file_name, strlen(file_name) + 1, 0);

    /* Receive 'get' reply from the server */
    myFTPMessage GET_REPLY;
    safe_recv(client_fd, &GET_REPLY, 12, 0);

    /* Verify the received message */
    if (!same_protocol(&GET_REPLY) || GET_REPLY.m_type != 0xA8) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Check server's response status for file existence */
    if (GET_REPLY.m_status == 0) {
        cerr << tok->argv[1] << ": No such file or directory on server" << endl;
        return;
    } else {
        cout << "Downloading file from server..." << endl;
    }

    /* Receive the actual file data from the server */
    myFTPMessage FILE_DATA;
    safe_recv(client_fd, &FILE_DATA, 12, 0);

    /* Verify the received data message */
    if (!same_protocol(&FILE_DATA) || FILE_DATA.m_type != 0xFF) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Calculate the file size from the server's message and prepare to save it locally */
    size_t file_length = size_t(ntohl(FILE_DATA.m_length)) - 12;
    ofstream outfile(file_name, ios::binary | ios::trunc);
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file for writing" << endl;
        return;
    }

    /* Receive the file data in chunks and write to the output file */
    size_t byte_recv = 0;
    char file_buf[MAXLINE];
    while (byte_recv < file_length) {
        size_t chunk_size = min(file_length - byte_recv, sizeof(file_buf));
        ssize_t n = safe_recv(client_fd, file_buf, chunk_size, 0);
        
        /* Check for errors during file reception */
        if (n < 0) {
            cerr << "Error: Failed to receive file" << endl;
            outfile.close();
            return;
        }

        /* Write received data to the output file */
        outfile.write(file_buf, n);
        byte_recv += n;
    }
    
    /* Close the output file and confirm download completion */
    outfile.close();
    cout << "Download complete: " << file_name << endl;
}

void myftp_put(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 2) {
        cout << "Usage: put <filename>" << endl;
        return;
    }

    /* Ensure client is connected to the server */
    if (!connect_flag) {
        cout << "Please connect to server first" << endl;
        return;
    }

    /* Prepare the file name and open it for reading */
    char file_name[MAXLINE] = {0};
    strncpy(file_name, tok->argv[1], strlen(tok->argv[1]) + 1);
    ifstream infile(file_name, ios::binary);
    
    /* Check if file exists and can be opened */
    if (!infile.good()) {
        cout << tok->argv[1] << ": No such file or directory on client" << endl;
        return;
    }
    if (!infile.is_open()) {
        cout << "Error: Could not open file for reading." << endl;
        return;
    }

    /* Send 'put' request to the server with the file name */
    myFTPMessage PUT_REQUEST;
    packing(&PUT_REQUEST, 0xA9, -1, 12 + strlen(file_name) + 1); // Message type 0xA9 for 'put' request
    safe_send(client_fd, &PUT_REQUEST, 12, 0);
    safe_send(client_fd, file_name, strlen(file_name) + 1, 0);

    /* Receive 'put' reply from the server */
    myFTPMessage PUT_REPLY;
    safe_recv(client_fd, &PUT_REPLY, 12, 0);

    /* Verify the received message */
    if (!same_protocol(&PUT_REPLY) || PUT_REPLY.m_type != 0xAA) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Send file data header with the file size to the server */
    myFTPMessage FILE_DATA;
    unsigned file_length = filesystem::file_size(string(file_name));
    packing(&FILE_DATA, 0xFF, -1, 12 + file_length); // Message type 0xFF for file data
    safe_send(client_fd, &FILE_DATA, 12, 0);

    /* Begin file upload */
    cout << "Uploading file to server..." << endl;
    size_t byte_send = 0;
    char file_buf[MAXLINE];

    /* Read and send the file in chunks */
    while (byte_send < file_length) {
        size_t chunk_size = min(file_length - byte_send, sizeof(file_buf));
        infile.read(file_buf, chunk_size);
        
        /* Send the chunk to the server */
        ssize_t n = safe_send(client_fd, file_buf, chunk_size, 0);
        
        /* Check for errors during file transmission */
        if (n < 0) {
            cerr << "Error: Failed to send file data." << endl;
            infile.close();
            return;
        }
        
        byte_send += n;
    }

    /* Close the file and confirm upload completion */
    infile.close();
    cout << "Upload complete: " << file_name << endl;
}

void myftp_sha256(const cmdline_tokens *tok) {
    /* Check the command */
    if (tok->argc != 2) {
        cout << "Usage: sha256 <filename>" << endl;
        return;
    }

    /* Ensure client is connected to the server */
    if (!connect_flag) {
        cout << "Please connect to server first" << endl;
        return;
    }

    /* Prepare the file name and send SHA-256 hash request to the server */
    char file_name[MAXLINE] = {0};
    strncpy(file_name, tok->argv[1], strlen(tok->argv[1]) + 1);

    myFTPMessage SHA_REQUEST;
    packing(&SHA_REQUEST, 0xAB, -1, 12 + strlen(file_name) + 1); // Message type 0xAB for SHA-256 request
    safe_send(client_fd, &SHA_REQUEST, 12, 0);
    safe_send(client_fd, &file_name, strlen(file_name) + 1, 0);

    /* Receive SHA-256 reply from the server */
    myFTPMessage SHA_REPLY;
    safe_recv(client_fd, &SHA_REPLY, 12, 0);

    /* Verify the received message */
    if (!same_protocol(&SHA_REPLY) || SHA_REPLY.m_type != 0xAC) {
        cerr << "Error: wrong message from server" << endl;
        return;
    }

    /* Check server's response status for file existence */
    if (SHA_REPLY.m_status == 0) {
        cout << tok->argv[1] << ": No such file or directory on server" << endl;
        return;
    }

    /* Receive the SHA-256 hash data from the server */
    myFTPMessage FILE_DATA;
    safe_recv(client_fd, &FILE_DATA, 12, 0);
    size_t sum_length = size_t(ntohl(FILE_DATA.m_length)) - 12;
    
    /* Receive and display the hash data in chunks */
    size_t byte_recv = 0;
    char file_buf[MAXLINE] = {0};
    while (byte_recv < sum_length) {
        size_t chunk_size = min(sum_length - byte_recv, sizeof(file_buf));
        ssize_t n = safe_recv(client_fd, file_buf, chunk_size, 0);
        
        // Check for errors during data reception
        if (n < 0) {
            cerr << "Error: Failed to receive file data" << endl;
            return;
        }

        // Output the received SHA-256 hash data
        cout << file_buf;
        byte_recv += n;
    }
}
