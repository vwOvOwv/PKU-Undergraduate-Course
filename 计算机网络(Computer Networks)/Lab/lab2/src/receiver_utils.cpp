#include "../include/rtp_receiver.h"

// ================================================
// ======== External Variable Declarations ========
// ================================================

extern int socket_fd;                   // Socket file descriptor
extern struct sockaddr_in sender_addr;  // Sender's address structure
extern socklen_t sender_addr_len;       // Length of sender's address

// ============================================
// ============= Helper Functions =============
// ============================================

/**
 * @brief Parses command-line arguments into the Param structure.
 *
 * @param params Pointer to the Param structure to populate.
 * @param args   Array of command-line arguments.
 */
void parse(struct Param *params, char **args) {
    /* --- Stage 1: Validate and Copy Listen Port --- */
    strncpy(params->listen_port, args[1], sizeof(params->listen_port) - 1);
    params->listen_port[sizeof(params->listen_port) - 1] = '\0'; // Ensure null-termination

    /* --- Stage 2: Validate and Copy File Path --- */
    strncpy(params->file_path, args[2], sizeof(params->file_path) - 1);
    params->file_path[sizeof(params->file_path) - 1] = '\0'; // Ensure null-termination

    /* --- Stage 3: Convert Window Size and Mode --- */
    params->window_size = atoi(args[3]);
    params->mode = atoi(args[4]);

    /* --- Stage 4: Log Parsed Parameters --- */
    LOG_DEBUG("Parsed arguments: listen_port=%s, file_path=%s, window_size=%d, mode=%d\n",
              params->listen_port, params->file_path, params->window_size, params->mode);
}

/**
 * @brief Receives data from the sender.
 *
 * @param buf Pointer to the buffer where received data will be stored.
 * @param n   Number of bytes to receive.
 * @return ssize_t Number of bytes received, or -1 on error.
 */
ssize_t from_sender(void *buf, size_t n) {
    // Receive data from the sender without storing the sender's address
    ssize_t ret = recvfrom(socket_fd, buf, n, 0, 
                           NULL, NULL);
    
    if (ret < 0) {
        LOG_FATAL("Error receiving data from sender: %s\n", strerror(errno));
    }

    LOG_DEBUG("Received %ld bytes from sender.\n", ret);
    return ret;
}

/**
 * @brief Sends data to the sender.
 *
 * @param buf Pointer to the buffer containing data to send.
 * @param n   Number of bytes to send.
 * @return ssize_t Number of bytes sent, or -1 on error.
 */
ssize_t to_sender(void *buf, size_t n) {
    // Send data to the sender using the sender's address
    ssize_t ret = sendto(socket_fd, buf, n, 0, 
                         (struct sockaddr *)&sender_addr, 
                         sender_addr_len);
    
    if (ret < 0) {
        LOG_FATAL("Error sending data to sender: %s\n", strerror(errno));
    }

    LOG_DEBUG("Sent %ld bytes to sender.\n", ret);
    return ret;
}