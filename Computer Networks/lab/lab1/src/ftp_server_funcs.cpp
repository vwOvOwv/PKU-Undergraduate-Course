#include "../include/ftp_server.hh"
#include <pthread.h>

unordered_map<int, filesystem::path> client_paths;  // Stores the current working directory for each client
extern pthread_mutex_t client_paths_mutex;  // Mutex to protect access to the client_paths map

void myftp_open(int connect_fd){
    myFTPMessage OPEN_CONN_REPLY;

    /* Lock mutex to safely update client_paths for this client */
    pthread_mutex_lock(&client_paths_mutex);
    client_paths[connect_fd] = filesystem::current_path();  // Set initial directory to server's current path
    pthread_mutex_unlock(&client_paths_mutex);

    /* Pack and send an OPEN_CONN_REPLY message to confirm connection */
    packing(&OPEN_CONN_REPLY, 0xA2, 1, 12); // Message type 0xA2 indicates a connection reply with status 1 (success)
    safe_send(connect_fd, &OPEN_CONN_REPLY, 12, 0);
}

void myftp_quit(int connect_fd) {
    myFTPMessage QUIT_REPLY;

    /* Pack and send a QUIT_REPLY message to confirm disconnection */
    packing(&QUIT_REPLY, 0xAE, -1, 12); // Message type 0xAE indicates a quit reply with status -1 (disconnection)
    safe_send(connect_fd, &QUIT_REPLY, 12, 0);

    /* Lock mutex to safely remove the client's entry from client_paths */
    pthread_mutex_lock(&client_paths_mutex);
    client_paths.erase(connect_fd); // Remove client's working directory entry
    pthread_mutex_unlock(&client_paths_mutex);
}

void myftp_ls(int connect_fd) {
    /* Construct the ls command based on the client's current directory */
    string command = "ls ";
    command += client_paths[connect_fd].string(); // Add the client's current path to the command

    /* Execute the ls command and capture the output */
    string result = execute_cmd(command.c_str());

    /* Pack and send the LIST_REPLY message containing the directory listing */
    myFTPMessage LIST_REPLY;
    packing(&LIST_REPLY, 0xA4, -1, 12 + strlen(result.c_str()) + 1); // Message type 0xA4 for directory listing
    safe_send(connect_fd, &LIST_REPLY, 12, 0);
    
    /* Send the result to the client */
    safe_send(connect_fd, result.c_str(), strlen(result.c_str()) + 1, 0);
}

void myftp_cd(int connect_fd, uint32_t m_length) {
    char dir_name[MAXLINE];

    /* Receive directory name from the client */
    safe_recv(connect_fd, dir_name, size_t(ntohl(m_length) - 12), 0);

    /* Get the current path for the client and construct the target path */
    filesystem::path current_path = client_paths[connect_fd];
    filesystem::path target_path = current_path / dir_name;

    myFTPMessage CHANGE_DIR_REPLY;

    /* Check if the target path exists and is a directory */
    if (filesystem::exists(target_path) && filesystem::is_directory(target_path)) {
        /* Resolve the target path to its canonical form */
        current_path = filesystem::canonical(target_path); 

        /* Update the client's current directory in a thread-safe manner */
        pthread_mutex_lock(&client_paths_mutex);
        client_paths[connect_fd] = current_path;
        pthread_mutex_unlock(&client_paths_mutex);

        /* Pack and send a successful change directory reply */
        packing(&CHANGE_DIR_REPLY, 0xA6, 1, 12); // Status 1 indicates success
    } else {
        /* Pack and send a failure reply if the directory doesn't exist or isn't a directory */
        packing(&CHANGE_DIR_REPLY, 0xA6, 0, 12); // Status 0 indicates failure
    }

    /* Send the change directory reply to the client */
    safe_send(connect_fd, &CHANGE_DIR_REPLY, 12, 0);
}

void myftp_get(int connect_fd, uint32_t m_length) {
    char file_name[MAXLINE];

    /* Receive the requested file name from the client */
    safe_recv(connect_fd, file_name, size_t(ntohl(m_length) - 12), 0);

    /* Get the current directory for the client and build the target file path */
    filesystem::path current_path = client_paths[connect_fd];
    filesystem::path target_path = current_path / file_name;

    myFTPMessage GET_REPLY;

    /* Check if the file exists and is a regular file */
    if (filesystem::exists(target_path) && filesystem::is_regular_file(target_path)) {
        /* Send a positive GET_REPLY to indicate file availability */
        packing(&GET_REPLY, 0xA8, 1, 12); // Status 1 indicates the file exists
        safe_send(connect_fd, &GET_REPLY, 12, 0);

        /* Pack and send the FILE_DATA message with the file length */
        myFTPMessage FILE_DATA;
        unsigned file_length = filesystem::file_size(target_path);
        packing(&FILE_DATA, 0xFF, -1, 12 + file_length); // Status -1 indicates file data
        safe_send(connect_fd, &FILE_DATA, 12, 0);

        /* Open the file and send it in chunks */
        ifstream infile(target_path, ios::binary);
        size_t byte_send = 0;
        char file_buf[MAXLINE];

        /* Loop to send the file data in chunks until complete */
        while (byte_send < file_length) {
            size_t chunk_size = min(file_length - byte_send, sizeof(file_buf));
            infile.read(file_buf, chunk_size);
            ssize_t n = safe_send(connect_fd, file_buf, chunk_size, 0);

            /* Check for errors during transmission */
            if (n < 0) {
                infile.close();
                return;
            }
            byte_send += n;
        }

        /* Close the file after sending is complete */
        infile.close();
    } else {
        /* Send a negative GET_REPLY if the file doesn't exist or isn't a regular file */
        packing(&GET_REPLY, 0xA8, 0, 12); // Status 0 indicates failure
        safe_send(connect_fd, &GET_REPLY, 12, 0);
    }
}

void myftp_put(int connect_fd, uint32_t m_length) {
    char file_name[MAXLINE];

    /* Receive the file name from the client */
    safe_recv(connect_fd, file_name, size_t(ntohl(m_length) - 12), 0);

    /* Get the client's current path and construct the target file path */
    filesystem::path current_path = client_paths[connect_fd];
    filesystem::path target_path = current_path / file_name;

    /* Send a PUT_REPLY to confirm that the server is ready to receive the file */
    myFTPMessage PUT_REPLY;
    packing(&PUT_REPLY, 0xAA, -1, 12); // Message type 0xAA for PUT reply
    safe_send(connect_fd, &PUT_REPLY, 12, 0);

    /* Receive FILE_DATA message to get the file length */
    myFTPMessage FILE_DATA;
    safe_recv(connect_fd, &FILE_DATA, 12, 0);
    size_t file_length = size_t(ntohl(FILE_DATA.m_length)) - 12;

    /* Open the target file for writing in binary mode */
    ofstream outfile(target_path, ios::binary | ios::trunc);
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file for writing" << endl;
        return;
    }

    /* Receive and write file data in chunks */
    size_t byte_recv = 0;
    char file_buf[MAXLINE];
    while (byte_recv < file_length) {
        size_t chunk_size = min(file_length - byte_recv, sizeof(file_buf));
        ssize_t n = safe_recv(connect_fd, file_buf, chunk_size, 0);

        /* Check for errors during reception */
        if (n < 0) {
            cerr << "Error: Failed to receive file data" << endl;
            outfile.close();
            return;
        }

        /* Write received data to the file */
        outfile.write(file_buf, n);
        byte_recv += n;
    }

    /* Close the file after receiving all data */
    outfile.close();
}

void myftp_sha256(int connect_fd, uint32_t m_length) {
    char file_name[MAXLINE];

    /* Receive the file name from the client */
    safe_recv(connect_fd, file_name, size_t(ntohl(m_length) - 12), 0);

    myFTPMessage SHA_REPLY;

    /* Check if the file exists */
    if (filesystem::exists(file_name)) {
        /* Pack and send a positive SHA_REPLY indicating file exists */
        packing(&SHA_REPLY, 0xAC, 1, 12); // Status 1 indicates file exists
        safe_send(connect_fd, &SHA_REPLY, 12, 0);

        /* Calculate the SHA-256 checksum using system command */
        myFTPMessage FILE_DATA;
        string command = "sha256sum " + string(file_name);
        string result = execute_cmd(command.c_str());

        /* Pack and send FILE_DATA message with the SHA-256 checksum result */
        packing(&FILE_DATA, 0xFF, -1, 12 + strlen(result.c_str()) + 1); // Status -1 indicates file data
        safe_send(connect_fd, &FILE_DATA, 12, 0);
        safe_send(connect_fd, result.c_str(), strlen(result.c_str()) + 1, 0);
    } else {
        /* Pack and send a negative SHA_REPLY if file does not exist */
        packing(&SHA_REPLY, 0xAC, 0, 12); // Status 0 indicates file does not exist
        safe_send(connect_fd, &SHA_REPLY, 12, 0);
    }
}