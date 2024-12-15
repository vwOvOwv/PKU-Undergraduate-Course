#include "../include/rtp_receiver.h"

using namespace std;

// =============================================
// ======== Global Variable Definitions ========
// =============================================
vector<char> write_buffer;
const size_t BUFFER_SIZE = 4 * 1024;

rtp_packet_t msg_recv;
rtp_packet_t msg_send;
uint32_t cur_seq_num;
uint32_t next_seq_num;
ssize_t n_received;
int socket_fd;
struct sockaddr_in sender_addr;
socklen_t sender_addr_len = sizeof(sender_addr);    // IMPORTANT
vector<WrappedPacket> packets;

// Comparator for sorting packets based on index
bool sort_packets(WrappedPacket &a, WrappedPacket &b){
    return a.index < b.index;
}

// =========================================
// ============= Main Function =============
// =========================================

int main(int argc, char **argv) {
    /* ========================================= */
    /* ==== Argument Validation and Parsing ==== */
    /* ========================================= */

    /* --- Stage 1: Validate Command-Line Arguments --- */
    if (argc != 5) {
        LOG_FATAL("Invalid arguments. Usage: ./receiver [listen_port] [file_path] [window_size] [mode]\n");
    }

    /* --- Stage 2: Parse Input Arguments --- */
    struct Param params;
    parse(&params, argv);

    /* ================================== */
    /* ===== Receiver Initialization ==== */
    /* ================================== */

    /* --- Stage 1: Initialize Receiver Socket --- */
    // Create a UDP socket for communication
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        LOG_FATAL("Failed to create UDP socket.\n");
    }

    /* --- Stage 2: Configure Receiver Address --- */
    memset(&sender_addr, 0, sizeof(sender_addr));
    sender_addr.sin_family = AF_INET;
    sender_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sender_addr.sin_port = htons(atoi(params.listen_port));
    if (bind(socket_fd, (struct sockaddr *)&sender_addr, sizeof(sender_addr)) < 0) {
        close(socket_fd);
        LOG_FATAL("Failed to bind socket to address: %s:%s\n", params.listen_port, strerror(errno));
    }

    /* --- Stage 3: Setup Epoll for Event Monitoring --- */
    // Create an epoll instance for monitoring socket events
    int epfd = epoll_create(128);
    if (epfd < 0) {
        close(socket_fd);
        LOG_FATAL("Failed to create epoll instance.\n");
    }

    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = socket_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socket_fd, &evt) < 0) {
        close(socket_fd);
        close(epfd);
        LOG_FATAL("Failed to add socket to epoll instance.\n");
    }

    struct epoll_event events[MAX_EVENT];
    int retrans = 0;

    LOG_DEBUG("Timer set to 5 seconds for receiving SYN.\n");

    /* =================================== */
    /* ===== Connection Establishment ==== */
    /* =================================== */

    /* --- Stage 1: Wait for SYN from Sender --- */
    // Wait for SYN packet within 5 seconds, else timeout
    while (true) {
        int nevents = epoll_wait(epfd, events, MAX_EVENT, 5000);
        if (nevents == 0) {
            close(socket_fd);
            close(epfd);
            LOG_FATAL("Timeout: Did not receive SYN within 5 seconds.\n");
        }
        else {
            // Receive incoming packet, use recvfrom to initialize sender address
            n_received = recvfrom(socket_fd, &msg_recv.header, sizeof(rtp_header_t), 0, 
                                  (struct sockaddr *)&sender_addr, &sender_addr_len);
            // unpack_header(&msg_recv.header);

            // Validate received SYN packet
            if (validate(&msg_recv.header, n_received) && msg_recv.header.flags == RTP_SYN) {
                cur_seq_num = increment(msg_recv.header.seq_num, 1);
                LOG_MSG("Received SYN from sender.\n");
                break; // Proceed to send SYNACK
            }
        }
    }

    /* --- Stage 2: Send SYNACK to Sender --- */
    // Send SYNACK packet to acknowledge connection initiation
    pack_header(&msg_send.header, cur_seq_num, 0, RTP_SYN | RTP_ACK);
    to_sender(&msg_send.header, sizeof(rtp_header_t));
    LOG_MSG("Sent SYNACK to sender.\n");

    /* --- Stage 3: Wait for ACK from Sender --- */
    // Wait for ACK packet with a timeout of 100 milliseconds, retransmit SYNACK if necessary
    retrans = 0;
    while (true) {
        int nevents = epoll_wait(epfd, events, MAX_EVENT, 100); // 100 ms timeout
        if (nevents > 0) {
            // Receive incoming packet
            n_received = from_sender(&msg_recv.header, sizeof(rtp_header_t));
            // unpack_header(&msg_recv.header);

            // Validate received ACK packet
            if (validate(&msg_recv.header, n_received) && 
                msg_recv.header.flags == RTP_ACK && 
                msg_recv.header.seq_num == cur_seq_num) {
                
                LOG_MSG("Received ACK from sender.\n");
                break; // Connection established
            }
        }
        else if (retrans < MAX_RETRANS) {
            // Resend SYNACK packet if maximum retransmissions not reached
            retrans++;
            pack_header(&msg_send.header, cur_seq_num, 0, RTP_SYN | RTP_ACK);
            to_sender(&msg_send.header, sizeof(rtp_header_t));
            LOG_MSG("Resent SYNACK to sender (Attempt %d).\n", retrans);
        }
        else {
            // Exceeded maximum retransmissions without receiving ACK
            close(socket_fd);
            close(epfd);
            LOG_FATAL("Failed to receive ACK after %d attempts.\n", retrans);
        }
    }

    LOG_MSG("Connection Established with Sender.\n");
    next_seq_num = cur_seq_num;

    /* ================================= */
    /* ===== Packet Reception Phase ==== */
    /* ================================= */

    /* --- Stage 1: Open File for Writing --- */
    LOG_DEBUG("Opening file for writing: %s\n", params.file_path);
    FILE *fp = fopen(params.file_path, "w");
    if (fp == NULL) {
        close(socket_fd);
        close(epfd);
        LOG_FATAL("Unable to open file for writing: %s\n", params.file_path);
    }

    /* --- Stage 2: Receive Packets Based on Mode --- */
    if (params.mode == 0) {   // GBN (Go-Back-N)
        /* --- Stage 2.1: Go-Back-N Packet Reception --- */
        int num_packets = 0;
        while (true) {
            int nevents = epoll_wait(epfd, events, MAX_EVENT, 5000); // 5 seconds timeout
            if (nevents == 0) {
                // Timeout reached, no more packets expected
                break;
            }
            else {
                // Receive incoming packet
                n_received = from_sender(&msg_recv, sizeof(rtp_packet_t));
                // unpack_header(&msg_recv.header);

                // Validate received packet
                if (validate(&msg_recv.header, n_received)) {
                    if (msg_recv.header.flags == 0) { // Data packet
                        if (msg_recv.header.seq_num == next_seq_num) {
                            // Write payload to file
                            write_buffer.insert(write_buffer.end(), msg_recv.payload, msg_recv.payload + msg_recv.header.length);
                            // fwrite(&msg_recv.payload, sizeof(char), msg_recv.header.length, fp);
                            num_packets++;
                            LOG_DEBUG("Received packet %d (Seq: %u).\n", num_packets, next_seq_num);
                            next_seq_num = increment(next_seq_num, 1);
                            if (write_buffer.size() >= BUFFER_SIZE) {
                                fwrite(write_buffer.data(), sizeof(char), write_buffer.size(), fp);
                                write_buffer.clear();
                            }
                        }
                        // Send ACK for the expected sequence number
                        pack_header(&msg_send.header, next_seq_num, 0, RTP_ACK);
                        to_sender(&msg_send.header, sizeof(rtp_header_t));
                    }
                    else if (msg_recv.header.flags == RTP_FIN) { // FIN packet
                        if (msg_recv.header.seq_num == next_seq_num) {
                            LOG_MSG("Received FIN from sender.\n");
                            // Send FINACK to acknowledge termination
                            pack_header(&msg_send.header, next_seq_num, 0, RTP_FIN | RTP_ACK);
                            to_sender(&msg_send.header, sizeof(rtp_header_t));
                            LOG_MSG("Sent FINACK to sender.\n");
                            break; // Termination successful
                        }
                    }
                }
            }
        }
        if (!write_buffer.empty()) 
            fwrite(write_buffer.data(), sizeof(char), write_buffer.size(), fp);
        LOG_DEBUG("Total packets received: %d\n", num_packets);
    }
    else {   // SR (Selective Repeat)
        /* --- Stage 2.2: Selective Repeat Packet Reception --- */
        int recv_base = 0;
        int num_packets = 0;
        int window_size = params.window_size;
        uint32_t start_seq_num = next_seq_num;

        // Initialize deque to track received packets within the window
        deque<bool> received(window_size, false); 
        while (true) {
            int nevents = epoll_wait(epfd, events, MAX_EVENT, 5000); // 5 seconds timeout
            if (nevents == 0) {
                // Timeout reached, no more packets expected
                break;
            }
            else {
                // Receive incoming packet
                n_received = from_sender(&msg_recv, sizeof(rtp_packet_t));
                // unpack_header(&msg_recv.header);

                // Validate received packet
                if (validate(&msg_recv.header, n_received)) {
                    if (msg_recv.header.flags == 0) { // Data packet
                        // Calculate the index relative to the receive base
                        int index = increment(msg_recv.header.seq_num, -(int)start_seq_num);
                        
                        // Check if the packet is within the acceptable window range
                        if (index < recv_base|| index >= recv_base + window_size){
                            if(index >= recv_base - window_size){
                                // Send ACK for the received packet to update sender's window
                                pack_header(&msg_send.header, msg_recv.header.seq_num, 0, RTP_ACK);
                                to_sender(&msg_send.header, sizeof(rtp_header_t));
                            }
                            continue;
                        }
                        if (!received[index - recv_base]) {
                            received[index - recv_base] = true;
                            num_packets++;
                            WrappedPacket tmp;
                            tmp.index = index;
                            tmp.pkt = msg_recv;
                            packets.push_back(tmp);

                            // Slide the window forward if the base packet is received
                            if (index == recv_base) {
                                while (received.front()) {
                                    received.pop_front();
                                    received.push_back(false);
                                    recv_base++;
                                }
                            }
                        }
                        // Send ACK for the received packet
                        pack_header(&msg_send.header, msg_recv.header.seq_num, 0, RTP_ACK);
                        to_sender(&msg_send.header, sizeof(rtp_header_t));
                    }
                    else if (msg_recv.header.flags == RTP_FIN) { // FIN packet
                        // Validate FIN sequence number
                        if (msg_recv.header.seq_num == increment(start_seq_num, num_packets)) {
                            LOG_MSG("Received FIN from sender.\n");
                            // Send FINACK to acknowledge termination
                            pack_header(&msg_send.header, increment(start_seq_num, num_packets), 
                                        0, RTP_FIN | RTP_ACK);
                            to_sender(&msg_send.header, sizeof(rtp_header_t));
                            LOG_MSG("Sent FINACK to sender.\n");
                            break; // Termination successful
                        }
                    }
                }
            }
        }

        // Sort received packets based on their index
        sort(packets.begin(), packets.end(), sort_packets);
        LOG_DEBUG("Total packets received: %lu\n", packets.size());
        LOG_DEBUG("Recorded packets: %d\n", num_packets);

        // Write sorted packets' payloads to the file
        for (int i = 0; i < num_packets; i++) {
            LOG_DEBUG("Writing packet %d (Index: %d) to file.\n", i + 1, packets[i].index);
            fwrite(&packets[i].pkt.payload, sizeof(char), packets[i].pkt.header.length, fp);
        }
    }

    /* ================================== */
    /* ======== Termination Phase ======= */
    /* ================================== */

    LOG_DEBUG("Receiver: Exiting...\n");

    /* --- Stage 1: Cleanup Resources --- */
    close(epfd);
    close(socket_fd);
    fclose(fp);
    return 0;
}