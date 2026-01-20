#include "../include/util.h"

// ======================================
// =========== Helper Functions =========
// ======================================

/**
 * @brief Computes the CRC32 value for a single byte.
 *
 * @param r The input byte as a 32-bit unsigned integer.
 * @return uint32_t The computed CRC32 value for the byte.
 */
static uint32_t crc32_for_byte(uint32_t r) {
    for (int j = 0; j < 8; ++j) {
        r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ (r >> 1);
    }
    return r ^ (uint32_t)0xFF000000L;
}

/**
 * @brief Computes the CRC32 checksum for a block of data.
 *
 * This function uses a static lookup table to compute the CRC32 value.
 * The table is initialized on the first call.
 *
 * @param data     Pointer to the data block.
 * @param n_bytes  Number of bytes in the data block.
 * @param crc      Pointer to the CRC32 value to be updated.
 */
static void crc32(const void* data, size_t n_bytes, uint32_t* crc) {
    static uint32_t table[0x100] = {0};
    static bool table_initialized = false;

    // Initialize CRC32 table on first use
    if (!table_initialized) {
        for (size_t i = 0; i < 0x100; ++i) {
            table[i] = crc32_for_byte(i);
        }
        table_initialized = true;
    }

    for (size_t i = 0; i < n_bytes; ++i) {
        *crc = table[(*crc ^ ((uint8_t*)data)[i]) & 0xFF] ^ (*crc >> 8);
    }
}

/**
 * @brief Computes the CRC32 checksum for a packet.
 *
 * Before computing the checksum, ensure that the "checksum" field
 * of the packet is set to 0.
 *
 * @param pkt      Pointer to the packet data.
 * @param n_bytes  Total number of bytes in the packet (including header).
 * @return uint32_t The computed CRC32 checksum.
 */
uint32_t compute_checksum(const void* pkt, size_t n_bytes) {
    uint32_t crc = 0;
    crc32(pkt, n_bytes, &crc);
    return crc;
}

/**
 * @brief Packs the RTP header with sequence number, length, flags, and checksum.
 *
 * This function sets the fields of the RTP header, computes the checksum,
 * and logs the packed header details.
 *
 * @param header   Pointer to the RTP header structure.
 * @param seq_num  Sequence number for the packet.
 * @param length   Length of the payload in bytes.
 * @param flags    Flags indicating packet type (e.g., SYN, ACK).
 */
void pack_header(rtp_header_t *header, uint32_t seq_num, uint16_t length, uint8_t flags) {
    header->seq_num = seq_num;
    header->length = length;
    header->flags = flags;
    header->checksum = 0;  // Initialize checksum to 0 before computation

    // Compute checksum over the entire packet (header + payload)
    uint32_t sum = compute_checksum(header, length + sizeof(rtp_header_t));
    header->checksum = sum;

    // Log the packed header details
    LOG_DEBUG("Pack Header:\n"
              "  Sequence Number: %u\n"
              "  Length: %u\n"
              "  Flags: %d\n"
              "  Checksum: %u\n", 
              seq_num, length, flags, sum);
}

/**
 * @brief Unpacks the RTP header and logs its contents.
 *
 * This function is primarily used for debugging purposes to verify
 * the contents of the RTP header after receiving a packet.
 *
 * @param header Pointer to the RTP header structure.
 */
void unpack_header(rtp_header_t *header) {
    // Log the unpacked header details
    LOG_DEBUG("Unpack Header:\n"
              "  Sequence Number: %u\n"
              "  Length: %u\n"
              "  Flags: %d\n"
              "  Checksum: %u\n", 
              header->seq_num, header->length, 
              header->flags, header->checksum);
}

/**
 * @brief Validates the RTP packet by checking payload length and checksum.
 *
 * @param header      Pointer to the RTP header structure.
 * @param n_received  Number of bytes received for the packet.
 * @return true       If the packet is valid.
 * @return false      If the packet is invalid.
 */
bool validate(rtp_header_t *header, ssize_t n_received) {
    // Check if the received payload length matches the header's length
    if ((uint16_t)n_received - (uint16_t)sizeof(rtp_header_t) != header->length) {
        LOG_DEBUG("Validation Failed: Incorrect payload length.\n");
        return false;
    }

    // Store the original checksum and set it to 0 for computation
    uint32_t original_sum = header->checksum;
    header->checksum = 0;

    // Compute the checksum over the entire packet
    uint32_t compute_sum = compute_checksum(header, header->length + sizeof(rtp_header_t));

    // Restore the original checksum
    header->checksum = original_sum;

    // Compare the computed checksum with the original
    if (original_sum != compute_sum) {
        LOG_DEBUG("Validation Failed: Checksum mismatch (Computed: %u, Expected: %u).\n", compute_sum, original_sum);
        return false;
    }

    return true;
}

/**
 * @brief Increments the sequence number with wrapping.
 *
 * @param seq_num  Current sequence number.
 * @param x        Value to increment (can be negative for decrement).
 * @return uint32_t The incremented sequence number.
 */
uint32_t increment(uint32_t seq_num, int x) {
    return (seq_num + x) % (1 << 30);
}