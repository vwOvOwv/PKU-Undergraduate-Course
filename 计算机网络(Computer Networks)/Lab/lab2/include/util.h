#ifndef UTIL_H
#define UTIL_H

#include "rtp.h"
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

// =======================================
// ========== Function Declarations ======
// =======================================

/**
 * @brief Packs the RTP header with the given sequence number, payload length, flags, and computes the checksum.
 *
 * This function sets the fields of the RTP header, computes the CRC32 checksum over the entire packet
 * (header and payload), and assigns the checksum value to the header's checksum field.
 *
 * @param header Pointer to the RTP header structure to be packed.
 * @param seq_num Sequence number for the RTP packet.
 * @param length Length of the payload in bytes.
 * @param flags Flags indicating the type of RTP packet (e.g., SYN, ACK).
 */
void pack_header(rtp_header_t *header, uint32_t seq_num, uint16_t length, uint8_t flags);

/**
 * @brief Unpacks and logs the RTP header information.
 *
 * This function is primarily used for debugging purposes to verify the contents of the RTP header
 * after receiving a packet.
 *
 * @param header Pointer to the RTP header structure to be unpacked.
 */
void unpack_header(rtp_header_t *header);

/**
 * @brief Validates the RTP packet by checking payload length and checksum.
 *
 * This function ensures that the received packet has the correct payload length as specified in the header
 * and that the checksum matches the computed CRC32 value. It temporarily resets the checksum field to zero
 * before computing the checksum to ensure accurate validation.
 *
 * @param header Pointer to the RTP header structure.
 * @param n_received Number of bytes received for the packet.
 * @return true If the packet is valid.
 * @return false If the packet is invalid.
 */
bool validate(rtp_header_t *header, ssize_t n_received);

/**
 * @brief Computes the CRC32 checksum for the given packet data.
 *
 * Before computing the checksum, ensure that the "checksum" field of the packet is set to 0.
 * The checksum is calculated over the entire packet, including the header and payload.
 *
 * @param pkt Pointer to the packet data.
 * @param n_bytes Total number of bytes in the packet (including header).
 * @return uint32_t The computed CRC32 checksum.
 */
uint32_t compute_checksum(const void *pkt, size_t n_bytes);

/**
 * @brief Increments the sequence number with wrapping.
 *
 * This function adds a specified value to the sequence number and wraps it around at \(2^{30}\)
 * to prevent overflow. It supports both positive and negative increments.
 *
 * @param seq_num Current sequence number.
 * @param x Value to increment (can be negative for decrement).
 * @return uint32_t The incremented sequence number.
 */
uint32_t increment(uint32_t seq_num, int x);

// ======================================
// ================ Logging Macros =======
// ======================================

/**
 * @brief Logs informational messages to standard output with green color.
 *
 * Use this macro to display help messages or general informational logs.
 *
 * @param ... Variable arguments similar to printf.
 */
#define LOG_MSG(...)                                                    \
    do {                                                                \
        fprintf(stdout, "\033[40;32m[ INFO     ] \033[0m" __VA_ARGS__); \
        fflush(stdout);                                                 \
    } while (0)

/**
 * @brief Logs debug messages to standard error with yellow color.
 *
 * This macro is only active when the `LDEBUG` macro is defined.
 * It is intended for debugging purposes and can be enabled or disabled
 * through compilation flags (e.g., in CMakeLists.txt).
 *
 * @param ... Variable arguments similar to printf.
 */
#ifdef LDEBUG
#define LOG_DEBUG(...)                                                  \
    do {                                                                \
        fprintf(stderr, "\033[40;33m[ DEBUG    ] \033[0m" __VA_ARGS__); \
        fflush(stderr);                                                 \
    } while (0)
#else
#define LOG_DEBUG(...)
#endif

/**
 * @brief Logs fatal error messages to standard error with red color and terminates the program.
 *
 * Use this macro when an unrecoverable error occurs. It logs the error message and exits the program.
 *
 * @param ... Variable arguments similar to printf.
 */
#define LOG_FATAL(...)                                                  \
    do {                                                                \
        fprintf(stderr, "\033[40;31m[ FATAL    ] \033[0m" __VA_ARGS__); \
        fflush(stderr);                                                 \
        exit(1);                                                        \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // UTIL_H