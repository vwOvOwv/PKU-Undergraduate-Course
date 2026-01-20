#ifndef __RTP_H
#define __RTP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// =======================================
// ========== Macro Definitions ==========
// =======================================

/**
 * @brief Maximum number of epoll events.
 */
#define MAX_EVENT 100

/**
 * @brief Maximum number of retransmission attempts.
 */
#define MAX_RETRANS 50

/**
 * @brief Maximum payload size for RTP packets.
 *
 * The payload size is set to 1461 bytes to accommodate typical MTU sizes
 * when accounting for IP and UDP headers.
 */
#define PAYLOAD_MAX 1461

// ========================================
// ============ Enum Definitions ==========
// ========================================

/**
 * @brief Flags used in the RTP header to indicate packet type.
 */
typedef enum RtpHeaderFlag {
    RTP_SYN = 0b0001, /**< Synchronization packet to initiate connection */
    RTP_ACK = 0b0010, /**< Acknowledgment packet to confirm receipt */
    RTP_FIN = 0b0100, /**< Finish packet to terminate connection */
} rtp_header_flag_t;

// ======================================
// ======== Structure Definitions =========
// ======================================

/**
 * @brief RTP Header Structure.
 *
 * This structure represents the header of an RTP packet. It contains essential
 * fields required for managing packet sequencing, payload length, integrity,
 * and control flags.
 */
typedef struct __attribute__((__packed__)) RtpHeader {
    uint32_t seq_num;   /**< Sequence number of the packet */
    uint16_t length;    /**< Length of the payload in bytes; 0 for SYN, ACK, and FIN packets */
    uint32_t checksum;  /**< 32-bit CRC checksum for data integrity verification */
    uint8_t flags;      /**< Flags indicating the type of RTP packet (e.g., SYN, ACK) */
} rtp_header_t;

/**
 * @brief RTP Packet Structure.
 *
 * This structure represents an RTP packet, consisting of an RTP header and
 * an optional payload. The payload size is defined by `PAYLOAD_MAX`.
 */
typedef struct __attribute__((__packed__)) RtpPacket {
    rtp_header_t header;           /**< RTP header containing control information */
    char payload[PAYLOAD_MAX];     /**< Payload data; size defined by `PAYLOAD_MAX` */
} rtp_packet_t;

#ifdef __cplusplus
}
#endif

#endif // __RTP_H