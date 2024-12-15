#include "../include/rtp_sender.h"

// ================================================
// ======== External Variable Declarations ========
// ================================================

extern int socket_fd;                   // Socket file descriptor
extern struct sockaddr_in receiver_addr; // Receiver's address structure

// ============================================
// ============= Helper Functions =============
// ============================================

/* ======================================== */
/* ==== Argument Validation and Parsing === */
/* ======================================== */

/**
 * @brief Parses command-line arguments into the Param structure.
 *
 * @param params Pointer to the Param structure to populate.
 * @param args   Array of command-line arguments.
 */
void parse(struct Param *params, char **args){
    // Safely copy receiver IP address
    strncpy(params->receiver_ip, args[1], sizeof(params->receiver_ip) - 1);
    params->receiver_ip[sizeof(params->receiver_ip) - 1] = '\0'; // Ensure null-termination

    // Safely copy receiver port
    strncpy(params->receiver_port, args[2], sizeof(params->receiver_port) - 1);
    params->receiver_port[sizeof(params->receiver_port) - 1] = '\0'; // Ensure null-termination

    // Safely copy file path
    strncpy(params->file_path, args[3], sizeof(params->file_path) - 1);
    params->file_path[sizeof(params->file_path) - 1] = '\0'; // Ensure null-termination

    // Convert window size and mode from string to integer
    params->window_size = atoi(args[4]);
    params->mode = atoi(args[5]);

    LOG_DEBUG("Parsed arguments: receiver_ip=%s, receiver_port=%s, file_path=%s, window_size=%d, mode=%d\n",
              params->receiver_ip, params->receiver_port, params->file_path, params->window_size, params->mode);
}

/* ======================================== */
/* ========= Data Transmission ============ */
/* ======================================== */

/**
 * @brief Receives data from the receiver.
 *
 * @param buf Pointer to the buffer where received data will be stored.
 * @param n   Number of bytes to receive.
 * @return ssize_t Number of bytes received, or -1 on error.
 */
ssize_t from_receiver(void *buf, size_t n){
    ssize_t ret = recvfrom(socket_fd, buf, n, 0, 
                           NULL, NULL);
    if(ret < 0){
        LOG_FATAL("Error receiving data from receiver: %s\n", strerror(errno));
    }
    LOG_DEBUG("Received %ld bytes from receiver.\n", ret);
    return ret;
}

/**
 * @brief Sends data to the receiver.
 *
 * @param buf Pointer to the buffer containing data to send.
 * @param n   Number of bytes to send.
 * @return ssize_t Number of bytes sent, or -1 on error.
 */
ssize_t to_receiver(void *buf, size_t n){
    ssize_t ret = sendto(socket_fd, buf, n, 0, 
                         (struct sockaddr *)&receiver_addr, 
                         sizeof(receiver_addr));
    if(ret < 0){
        LOG_FATAL("Error sending data to receiver: %s\n", strerror(errno));
    }
    LOG_DEBUG("Sent %ld bytes to receiver.\n", ret);
    return ret;
}