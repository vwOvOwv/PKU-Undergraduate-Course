#include "rtp.h"         
#include "util.h"  
#include <cstdint> 
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>
#include <algorithm> 
#include <cerrno>
#include <unistd.h>

using namespace std;

/**
 * @brief Maximum length for various string parameters.
 */
#define MAXLINE 128

/**
 * @brief Structure to hold command-line parameters for the sender.
 *
 * This structure stores the receiver's IP address, receiver's port,
 * file path to send, window size for the sliding window protocol,
 * and the mode of operation (e.g., Go-Back-N or Selective Repeat).
 */
struct Param {
    char receiver_ip[MAXLINE];    /**< Receiver's IP address */
    char receiver_port[MAXLINE];  /**< Receiver's port number */
    char file_path[MAXLINE];      /**< Path to the file to be sent */
    int window_size;              /**< Size of the sliding window */
    int mode;                     /**< Mode of operation (0 for GBN, 1 for SR) */
};

// ======================================
// ============ Function Declarations ====
// ======================================

/**
 * @brief Parses command-line arguments into the Param structure.
 *
 * This function extracts the receiver's IP address, port number, file path,
 * window size, and mode of operation from the command-line arguments
 * and populates the provided Param structure.
 *
 * @param params Pointer to the Param structure to populate.
 * @param args   Array of command-line arguments.
 */
void parse(struct Param *params, char **args);

/**
 * @brief Receives data from the receiver.
 *
 * This function uses the global socket file descriptor to receive data
 * from the receiver. It logs the number of bytes received for debugging purposes.
 *
 * @param buf Pointer to the buffer where received data will be stored.
 * @param n   Number of bytes to receive.
 * @return ssize_t Number of bytes received on success, or -1 on error.
 */
ssize_t from_receiver(void *buf, size_t n);

/**
 * @brief Sends data to the receiver.
 *
 * This function uses the global socket file descriptor to send data
 * to the receiver's address. It logs the number of bytes sent for debugging purposes.
 *
 * @param buf Pointer to the buffer containing data to send.
 * @param n   Number of bytes to send.
 * @return ssize_t Number of bytes sent on success, or -1 on error.
 */
ssize_t to_receiver(void *buf, size_t n);