#include <cstddef>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <filesystem>
#include <fstream>

using namespace std;

/* Constants */
#define MAXARGS 128                    // Maximum number of arguments
#define MAXLINE 2048                   // Maximum length of a command line
#define MAGIC_NUMBER_LENGTH 6          // Length of the protocol magic number
#define ST_NORMAL   0                  // State: next token is an argument
#define ST_INFILE   1                  // State: next token is an input file
#define ST_OUTFILE  2                  // State: next token is an output file

/* Protocol magic number for MyFTP communication */
const char protocol[MAGIC_NUMBER_LENGTH] = {'\xc1', '\xa1', '\x10', 'f', 't', 'p'};

/* Structure to store parsed command line tokens */
struct cmdline_tokens {
    char *infile;                    // Input file name
    char *outfile;                   // Output file name
    int argc;                        // Number of arguments
    char *argv[MAXARGS];             // Array to hold arguments list
    enum builtins_t {                // Enum for built-in command types
        BUILTIN_open,
        BUILTIN_ls,
        BUILTIN_cd,
        BUILTIN_get,
        BUILTIN_put,
        BUILTIN_sha256,
        BUILTIN_quit,
        BUILTIN_NONE
    } builtins;                      // Type of built-in command
};

/* Structure to define the MyFTP message format */
struct myFTPMessage{
    char m_protocol[MAGIC_NUMBER_LENGTH]; // Protocol magic number (6 bytes)
    uint8_t m_type;                       // Message type (1 byte)
    uint8_t m_status;                     // Message status (1 byte)
    uint32_t m_length;                    // Message length (4 bytes, Big Endian)
} __attribute__ ((packed));               // Packed to avoid padding

/* Function Prototypes */
int parseline(const char *command_line, struct cmdline_tokens *tok); // Parses the command line
void eval(const string &command_line);                               // Evaluates the command line

/* Command function prototypes */
void myftp_cd(const cmdline_tokens *tok);                            // Change directory command
void myftp_get(const cmdline_tokens *tok);                           // Get file from server
void myftp_ls(const cmdline_tokens *tok);                            // List directory contents
void myftp_open(const cmdline_tokens *tok);                          // Open connection to server
void myftp_put(const cmdline_tokens *tok);                           // Put file to server
void myftp_quit(const cmdline_tokens *tok);                          // Quit FTP client
void myftp_sha256(const cmdline_tokens *tok);                        // Compute SHA-256 checksum

/* Network utility function prototypes */
int open_clientfd(char *ip, char *port);                             // Opens client file descriptor
ssize_t safe_send(int fd, const void *buf, size_t n, int flags);     // Safely send data
ssize_t safe_recv(int fd, void *buf, size_t n, int flags);           // Safely receive data

/* FTP message utility function prototypes */
void packing(myFTPMessage *msg, uint8_t type, uint8_t status, uint32_t length); // Pack message
bool same_protocol(myFTPMessage *msg);                                           // Check protocol
