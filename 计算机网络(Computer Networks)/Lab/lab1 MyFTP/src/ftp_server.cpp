#include "../include/ftp_server.hh"
#include <cstddef>

using namespace std;

pthread_t tid; // Thread identifier

int main(int argc, char **argv) {
    int listen_fd, *connect_fd_p;
    socklen_t client_len;
    struct sockaddr_storage client_addr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    /* Check for correct command-line arguments */
    if (argc != 3) {
        cout << "usage: " << argv[0] << " <ip> <port>" << endl;
        exit(0);
    }

    /* Open listening socket on specified port */
    listen_fd = open_listenfd(argv[2]);

    /* Initialize mutexes for thread safety */
    mutex_init();

    /* Main server loop to accept client connections */
    while (1) {
        client_len = sizeof(struct sockaddr_storage);

        /* Allocate memory for the connected client socket descriptor */
        connect_fd_p = (int *)malloc(sizeof(int));

        /* Accept client connection */
        *connect_fd_p = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);

        /* Retrieve and print client information */
        getnameinfo((struct sockaddr *) &client_addr, client_len, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        cout << "Connected to " << client_hostname << ", " << client_port << endl;

        /* Create a new thread to handle the client connection */
        pthread_create(&tid, NULL, thread, connect_fd_p);
    }
    exit(0);
}

/* Thread function to handle each client connection */
void *thread(void *vargp) {
    int connect_fd = (*(int *)vargp); // Retrieve the connected client socket descriptor
    pthread_detach(pthread_self());   // Detach thread to reclaim resources on exit
    free(vargp);                      // Free allocated memory for the socket descriptor

    /* Process client requests */
    doit(connect_fd);

    /* Close client connection and exit thread */
    close(connect_fd);
    pthread_exit(0);
}
