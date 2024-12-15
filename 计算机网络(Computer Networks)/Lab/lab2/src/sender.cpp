#include "../include/rtp_sender.h"

using namespace std;

// =============================================
// ======== Global Variable Definitions ========
// =============================================
struct Param params;
int socket_fd;
struct sockaddr_in receiver_addr;
int epfd;
struct epoll_event evt;
struct epoll_event events[MAX_EVENT];
rtp_packet_t msg_recv;
rtp_packet_t msg_send;
uint32_t cur_seq_num;
uint32_t start_seq_num;
uint32_t final_seq_num;
int retrans;
ssize_t n_received;
vector<rtp_packet_t> packets;

// =========================================
// ============= Main Function =============
// =========================================

int main(int argc, char **argv) {
    /* ======================================== */
    /* ==== Argument Validation and Parsing === */
    /* ======================================== */

    /* --- Stage 1: Validate Command-Line Arguments --- */
    if (argc != 6) {
        LOG_FATAL("Invalid arguments. Usage: ./sender [receiver_ip]"
        " [receiver_port] [file_path] [window_size] [mode]\n");
    }

    /* --- Stage 2: Parse Input Arguments --- */
    parse(&params, argv);


    /* =============================== */
    /* ===== Sender Initialization ==== */
    /* =============================== */

    /* --- Stage 1: Initialize Sender Socket --- */
    // Create a UDP socket for communication
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOG_FATAL("Failed to create UDP socket.\n");
    }

    /* --- Stage 2: Configure Receiver Address --- */
    memset(&receiver_addr, 0, sizeof(struct sockaddr_in));
    receiver_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, params.receiver_ip, &receiver_addr.sin_addr) <= 0) {
        close(socket_fd);
        LOG_FATAL("Invalid receiver IP address: %s\n", params.receiver_ip);
    }
    receiver_addr.sin_port = htons(atoi(params.receiver_port));

    /* --- Stage 3: Setup Epoll for Event Monitoring --- */
    // Create an epoll instance for monitoring socket events
    if ((epfd = epoll_create(128)) < 0) {
        close(socket_fd);
        LOG_FATAL("Failed to create epoll instance.\n");
    }

    // Configure epoll event for the socket
    evt.events = EPOLLIN;
    evt.data.fd = socket_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &evt) < 0) {
        close(socket_fd);
        close(epfd);
        LOG_FATAL("Failed to add socket to epoll instance.\n");
    }

    /* --- Stage 4: Initialize Sequence Numbers --- */
    // Initialize current sequence number with a random value
    cur_seq_num = (uint32_t)(random() % (1 << 30));

    /* --- Stage 5: Prepare Packets for Sending --- */
    // Open the file to be sent in read mode
    FILE *fp = fopen(params.file_path, "r");
    if (!fp) {
        close(socket_fd);
        close(epfd);
        LOG_FATAL("Unable to open file: %s\n", params.file_path);
    }

    // Initialize sequence numbers for packet headers
    start_seq_num = increment(cur_seq_num, 1);
    final_seq_num = start_seq_num;

    // Read file content and create packets
    while (!feof(fp)) {
        rtp_packet_t tmp_pkt;

        // Clear the payload buffer
        memset(&tmp_pkt.payload, 0, PAYLOAD_MAX);

        // Read data from file into packet payload
        size_t n_reads = fread(&tmp_pkt.payload, sizeof(char), PAYLOAD_MAX, fp);
        if (n_reads < 0) {
            close(socket_fd);
            close(epfd);
            fclose(fp);
            LOG_FATAL("Error reading from file: %s\n", params.file_path);
        }

        // Pack the RTP header with sequence number and payload length
        pack_header(&tmp_pkt.header, final_seq_num, n_reads, 0);

        // Increment the sequence number for the next packet
        final_seq_num = increment(final_seq_num, 1);

        // Add the packet to the sending queue
        packets.push_back(tmp_pkt);
        
    }

    // Close the file after reading
    if (fclose(fp) != 0) {
        close(socket_fd);
        close(epfd);
        LOG_FATAL("Error closing file: %s\n", params.file_path);
    }

    /* =============================== */
    /* ===== Establish Connection ==== */
    /* =============================== */

    /* --- Stage 1: Send SYN to Receiver --- */
    // Initialize and send SYN packet to initiate connection
    pack_header(&msg_send.header, cur_seq_num, 0, RTP_SYN);
    to_receiver(&msg_send.header, sizeof(rtp_header_t));
    LOG_MSG("Sent SYN to receiver.\n");

    /* --- Stage 2: Wait for SYNACK from Receiver --- */
    retrans = 0;
    while (true) {
        // Wait for events with a timeout of 100 milliseconds
        int nevents = epoll_wait(epfd, events, MAX_EVENT, 100);
        
        if (nevents > 0) {
            // Receive and process incoming packet
            n_received = from_receiver(&msg_recv.header, sizeof(rtp_header_t));
            unpack_header(&msg_recv.header);
            
            // Validate received packet for SYNACK
            if (validate(&msg_recv.header, n_received) &&
                msg_recv.header.flags == (RTP_SYN | RTP_ACK) &&
                msg_recv.header.seq_num == increment(cur_seq_num, 1)) {
                
                cur_seq_num = increment(cur_seq_num, 1);
                LOG_MSG("Received SYNACK from receiver.\n");
                break;
            }
        }
        else if (retrans < MAX_RETRANS) {
            // Resend SYN packet if maximum retransmissions not reached
            retrans++;
            pack_header(&msg_send.header, cur_seq_num, 0, RTP_SYN);
            to_receiver(&msg_send.header, sizeof(rtp_header_t));
            LOG_MSG("Resent SYN to receiver (Attempt %d).\n", retrans);
        }
        else {
            // Exceeded maximum retransmissions without receiving SYNACK
            close(socket_fd);
            close(epfd);
            LOG_FATAL("Failed to receive SYNACK after %d attempts.\n", retrans);
        }
    }

    /* --- Stage 3: Send ACK to Receiver --- */
    // Send ACK packet to confirm connection establishment
    pack_header(&msg_send.header, cur_seq_num, 0, RTP_ACK);
    to_receiver(&msg_send.header, sizeof(rtp_header_t));
    LOG_MSG("Sent ACK to receiver.\n");

    /* --- Stage 4: Confirm ACK Receipt --- */
    retrans = 0;
    while (true) {
        // Wait for events with a timeout of 2000 milliseconds (2 seconds)
        int nevents = epoll_wait(epfd, events, MAX_EVENT, 2000);
        
        if (nevents == 0) {
            // Timeout reached, assume ACK has been received
            break;
        }
        else {
            // Receive and process incoming packet
            n_received = from_receiver(&msg_recv.header, sizeof(rtp_header_t));
            unpack_header(&msg_recv.header);
            
            // Validate received packet for SYNACK (if applicable)
            if (validate(&msg_recv.header, n_received) &&
                msg_recv.header.flags == (RTP_SYN | RTP_ACK) &&
                msg_recv.header.seq_num == cur_seq_num) {
                
                LOG_MSG("Received unexpected SYNACK. Resending ACK (Attempt %d).\n", retrans + 1);
                
                if (retrans < MAX_RETRANS) {
                    // Resend ACK packet
                    pack_header(&msg_send.header, cur_seq_num, 0, RTP_ACK);
                    to_receiver(&msg_send.header, sizeof(rtp_header_t));
                    retrans++;
                }
                else {
                    // Exceeded maximum retransmissions without proper acknowledgment
                    close(socket_fd);
                    close(epfd);
                    LOG_FATAL("Failed to confirm ACK receipt after %d attempts.\n", retrans);
                }
            }
        }
    }

    LOG_MSG("Connection Established Successfully.\n");

    /* =================================== */
    /* ==== Packet Transmission Phase ==== */
    /* =================================== */

    /* --- Stage 1: Initialize Transmission Variables --- */
    int num_packets = packets.size();
    int send_base = 0;
    int next_packet = 0;
    int window_size = params.window_size;
    retrans = 0;
    // LOG_MSG("num_packets = %d\n", num_packets);

    if (params.mode == 0) {  // GBN (Go-Back-N)
        /* --- Stage 2-1: Go-Back-N Transmission --- */
        while (next_packet != num_packets) {
            // Send packets within the window
            for (; next_packet < min(send_base + window_size, num_packets); next_packet++) {
                to_receiver(&packets[next_packet], 
                            sizeof(rtp_header_t) + packets[next_packet].header.length);
            }

            int last_unconfirmed = send_base;
            int ACK_received = 0;

            // Wait for ACKs
            while (true) {
                int nevents = epoll_wait(epfd, events, MAX_EVENT, 100);
                if (nevents == 0){
                    retrans++;
                    if(retrans > MAX_RETRANS){
                        close(socket_fd);
                        close(epfd);
                        LOG_FATAL("Failed to receive ACK after %d attempts.\n", retrans - 1);
                    }
                    break;
                }

                retrans = 0;
                n_received = from_receiver(&msg_recv.header, sizeof(rtp_header_t));
                unpack_header(&msg_recv.header);

                // Validate received ACK
                if (validate(&msg_recv.header, n_received) && msg_recv.header.flags == RTP_ACK) {
                    // Calculate the index based on sequence number
                    int index = increment(msg_recv.header.seq_num, -(1 + (int)start_seq_num));
                    if (index >= send_base && index < next_packet) {
                        last_unconfirmed = index + 1;
                        ACK_received++;
                        if (ACK_received == window_size)
                            break;
                    }
                }
            }

            // Slide the window
            send_base = last_unconfirmed;
            next_packet = send_base;
            LOG_DEBUG("send_base = %d\n", send_base);
        }
    }
    else {   // SR (Selective Repeat)
        /* --- Stage 2-2: Selective Repeat Transmission --- */
        vector<bool> confirmed(num_packets, false);
        while (next_packet != num_packets) {
            int last_unconfirmed = send_base;
            int ACK_received = 0;

            // Send unconfirmed packets within the window
            for (; next_packet < min(send_base + window_size, num_packets); next_packet++) {
                if (!confirmed[next_packet]) {
                    to_receiver(&packets[next_packet], 
                                sizeof(rtp_header_t) + packets[next_packet].header.length);
                }
                else
                    ACK_received++;
            }

            // Wait for ACKs
            while (true) {
                int nevents = epoll_wait(epfd, events, MAX_EVENT, 100);
                if (nevents == 0){
                    retrans++;
                    if(retrans > MAX_RETRANS){
                        close(socket_fd);
                        close(epfd);
                        LOG_FATAL("Failed to receive ACK after %d attempts.\n", retrans - 1);
                    }
                    break;
                }
                
                retrans = 0;
                n_received = from_receiver(&msg_recv.header, sizeof(rtp_header_t));
                unpack_header(&msg_recv.header);

                // Validate received ACK
                if (validate(&msg_recv.header, n_received) && msg_recv.header.flags == RTP_ACK) {
                    // Calculate the index based on sequence number
                    int index = increment(msg_recv.header.seq_num, -(int)start_seq_num);
                    if (index >= send_base && index < next_packet && !confirmed[index]) {
                        confirmed[index] = true;
                        if (last_unconfirmed == index) {
                            while (index < num_packets && confirmed[index])
                                index++;
                            last_unconfirmed = index;
                        }
                        ACK_received++;
                        if (ACK_received == window_size)
                            break;
                    }
                }
            }

            // Slide the window
            send_base = last_unconfirmed;
            next_packet = send_base;
            LOG_DEBUG("send_base = %d\n", send_base);
        }
    }

    /* ================================== */
    /* ======== Termination Phase ======= */
    /* ================================== */

    /* --- Stage 1: Send FIN to Receiver --- */
    // Send FIN packet to terminate connection
    pack_header(&msg_send.header, final_seq_num, 0, RTP_FIN);
    to_receiver(&msg_send.header, sizeof(rtp_header_t));
    LOG_MSG("Sent FIN to receiver.\n");

    /* --- Stage 2: Wait for FINACK from Receiver --- */
    retrans = 0;
    while (true) {
        // Wait for events with a timeout of 100 milliseconds
        int nevents = epoll_wait(epfd, events, MAX_EVENT, 100);
        if (nevents > 0) {
            // Receive and process incoming packet
            n_received = from_receiver(&msg_recv.header, sizeof(rtp_header_t));
            unpack_header(&msg_recv.header);
            
            // Validate received packet for FINACK
            if (validate(&msg_recv.header, n_received) &&
                msg_recv.header.flags == (RTP_FIN | RTP_ACK) &&
                msg_recv.header.seq_num == final_seq_num) {
                
                LOG_MSG("Received FINACK from receiver.\n");
                break; // Termination successful
            }
        }
        else if (retrans < MAX_RETRANS) {
            // Resend FIN packet if maximum retransmissions not reached
            retrans++;
            pack_header(&msg_send.header, final_seq_num, 0, RTP_FIN);
            to_receiver(&msg_send.header, sizeof(rtp_header_t));
            LOG_MSG("Resent FIN to receiver (Attempt %d).\n", retrans);
        }
        else {
            // Exceeded maximum retransmissions without receiving FINACK
            close(socket_fd);
            close(epfd);
            LOG_FATAL("Failed to receive FINACK after %d attempts.\n", retrans);
        }
    }

    LOG_DEBUG("Sender: Exiting...\n");

    /* --- Stage 3: Cleanup Resources --- */
    close(epfd);
    close(socket_fd);
    // Note: The file has already been closed after packet preparation.
    return 0;
}