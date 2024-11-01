#include <cstdint>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <pthread.h>
#include <filesystem>
#include <fstream>
#include <unordered_map>

using namespace std;

/* Constants */
#define MAXARGS 128
#define MAXLINE 2048
#define MAGIC_NUMBER_LENGTH 6
#define LISTENQ 32

/* Protocol magic number for MyFTP communication */
const char protocol[6] = {'\xc1', '\xa1', '\x10', 'f', 't', 'p'};

/* Structure defining the MyFTP message format */
struct myFTPMessage {
    char m_protocol[MAGIC_NUMBER_LENGTH]; // Protocol magic number (6 bytes)
    uint8_t m_type;                       // Message type (1 byte)
    uint8_t m_status;                     // Message status (1 byte)
    uint32_t m_length;                    // Message length (4 bytes, Big endian)
} __attribute__ ((packed));               // Packed to avoid padding

/* Function declarations for handling MyFTP commands */
void myftp_cd(int connect_fd, uint32_t m_length);       // Change directory command
void myftp_get(int connect_fd, uint32_t m_length);      // Get file from server
void myftp_ls(int connect_fd);                          // List directory contents
void myftp_open(int connect_fd);                        // Open connection to client
void myftp_put(int connect_fd, uint32_t m_length);      // Put file to server
void myftp_quit(int connect_fd);                        // Quit connection with client
void myftp_sha256(int connect_fd, uint32_t m_length);   // Compute SHA-256 checksum

/* Server utility function declarations */
int open_listenfd(char *port);                          // Open listening socket on specified port
void doit(int connect_fd);                              // Process client request
ssize_t safe_send(int fd, const void *buf, size_t n, int flags); // Send data safely
ssize_t safe_recv(int fd, void *buf, size_t n, int flags);       // Receive data safely
void packing(myFTPMessage *msg, uint8_t type, uint8_t status, uint32_t length); // Pack FTP message

/* Thread and mutex utility function declarations */
void *thread(void *vargp);                              // Thread function to handle client connections
void mutex_init();           

/* Helper function to execute shell commands and capture output */
string execute_cmd(const char *command);