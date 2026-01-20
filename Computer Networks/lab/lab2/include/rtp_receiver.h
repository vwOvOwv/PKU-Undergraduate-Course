#include <cstdint>
#include <cstdio>
#include <string.h>
#include <netinet/in.h> 
#include "util.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include "rtp.h"
#include <sys/epoll.h>
#include <vector>
#include <cerrno>
#include <algorithm>
#include <cstddef>
#include <unistd.h>
#include <deque>
#include <chrono>  

using namespace std;

#define MAXLINE 128  /**< Maximum length for various string parameters */

/**
 * @brief Structure to hold command-line parameters for the sender.
 *
 * This structure stores the receiver's listening port, file path to send,
 * window size for the sliding window protocol, and the mode of operation
 * (e.g., Go-Back-N or Selective Repeat).
 */
struct Param {
    char listen_port[MAXLINE]; /**< Receiver's listening port */
    char file_path[MAXLINE];   /**< Path to the file to be sent */
    int window_size;           /**< Size of the sliding window */
    int mode;                  /**< Mode of operation (0 for GBN, 1 for SR) */
};

/**
 * @brief Structure to wrap RTP packets with their corresponding index.
 *
 * This structure is used to manage packets within the sending queue,
 * allowing easy tracking and acknowledgment of individual packets.
 */
struct WrappedPacket {
    rtp_packet_t pkt; /**< RTP packet containing header and payload */
    int index;        /**< Index of the packet in the sending queue */
};

/**
 * @brief Parses command-line arguments into the Param structure.
 *
 * This function extracts the receiver's listening port, file path,
 * window size, and mode of operation from the command-line arguments
 * and populates the provided Param structure.
 *
 * @param params Pointer to the Param structure to populate.
 * @param args   Array of command-line arguments.
 */
void parse(struct Param *params, char **args);

/**
 * @brief Receives data from the sender.
 *
 * This function uses the global socket file descriptor to receive data
 * from the sender. It logs the number of bytes received for debugging purposes.
 *
 * @param buf Pointer to the buffer where received data will be stored.
 * @param n   Number of bytes to receive.
 * @return ssize_t Number of bytes received on success, or -1 on error.
 */
ssize_t from_sender(void *buf, size_t n);

/**
 * @brief Sends data to the sender.
 *
 * This function uses the global socket file descriptor to send data
 * to the sender's address. It logs the number of bytes sent for debugging purposes.
 *
 * @param buf Pointer to the buffer containing data to send.
 * @param n   Number of bytes to send.
 * @return ssize_t Number of bytes sent on success, or -1 on error.
 */
ssize_t to_sender(void *buf, size_t n);