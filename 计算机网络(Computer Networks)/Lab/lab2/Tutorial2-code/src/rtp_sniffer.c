#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* default snap length (maximum bytes per packet to capture) */
#define SNAP_LEN 1518

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

uint8_t SIZE_ETHERNET;

/* Ethernet header */
struct sniff_ethernet {
        u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
        u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
        u_short ether_type;                     /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
        u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
        u_char  ip_tos;                 /* type of service */
        u_short ip_len;                 /* total length */
        u_short ip_id;                  /* identification */
        u_short ip_off;                 /* fragment offset field */
        #define IP_RF 0x8000            /* reserved fragment flag */
        #define IP_DF 0x4000            /* don't fragment flag */
        #define IP_MF 0x2000            /* more fragments flag */
        #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
        u_char  ip_ttl;                 /* time to live */
        u_char  ip_p;                   /* protocol */
        u_short ip_sum;                 /* checksum */
        struct  in_addr ip_src,ip_dst;  /* source and dest address */
};
#define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)                (((ip)->ip_vhl) >> 4)

/* UDP header */
struct sniff_udp {
    u_short uh_sport;               /* source port */
    u_short uh_dport;              /* destination port */
	u_short uh_len;			/* length of udp header and payload */
	u_short uh_sum;			/* checksum */
};

/* RTP header */

typedef enum RtpHeaderFlag {
    RTP_SYN = 0b0001,
    RTP_ACK = 0b0010,
    RTP_FIN = 0b0100,
} rtp_header_flag_t;

struct __attribute__((__packed__)) sniff_rtp {
    uint32_t seq_num;   // Sequence number
    uint16_t length;    // Length of data; 0 for ACK, START and END packets
    uint32_t checksum;  // 32-bit CRC
    uint8_t flags;      // 0: START; 1: END; 2: DATA; 3: ACK
};


/*
 * print help text
 */
void print_app_usage(void) {
	printf("Usage: ./rtp_sniffer interface\n");
	printf("\n");
	printf("Argument:\n");
	printf("    interface    Listen on <interface> for packets.\n");
	printf("\n");
}


/*
 * dissect/print packet
 */
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

	static int count = 1;                   /* packet counter */

	/* declare pointers to packet headers */
	const struct sniff_ethernet *ethernet;  /* The ethernet header */
	const struct sniff_ip *ip;              /* The IP header */
	const struct sniff_udp *udp;            /* The UDP header */
	const struct sniff_rtp *rtp;		/* The RTP header */
	const char *payload;                    /* Packet payload */

	int size_ip;

	printf("\nPacket number %d:\n", count++);

	/* define/compute ip header offset */
	ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
	size_ip = IP_HL(ip)*4;
	if (size_ip < 20) {
		printf("   * Invalid IP header length: %u bytes\n", size_ip);
		return;
	}

	/* print source and destination IP addresses */
	printf("       From: %s\n", inet_ntoa(ip->ip_src));
	printf("         To: %s\n", inet_ntoa(ip->ip_dst));

	/* extract udp header */
	udp = (struct sniff_udp*)(packet + SIZE_ETHERNET + size_ip);
	printf("   Src port: %d\n", ntohs(udp->uh_sport));
	printf("   Dst port: %d\n", ntohs(udp->uh_dport));

	/* extract rtp header */
	rtp = (struct sniff_rtp*)(packet + SIZE_ETHERNET + size_ip + sizeof(struct sniff_udp));
	if (rtp->flags == RTP_SYN) {
		printf("   RTP type: SYN\n");
		printf(" RTP SeqNum: %d\n", rtp->seq_num);
	} else if (rtp->flags == RTP_ACK) {
		printf("   RTP type: ACK.\n");
		printf(" RTP SeqNum: %d\n", rtp->seq_num);
	} else if (rtp->flags == (RTP_ACK | RTP_SYN)) {
		printf("   RTP type: SYNACK.\n");
		printf(" RTP ACKSeq: %d\n", rtp->seq_num);
	} else if (rtp->flags == RTP_FIN) {
		printf("   RTP type: FIN.\n");
		printf(" RTP SeqNum: %d\n", rtp->seq_num);
	} else if (rtp->flags == (RTP_FIN | RTP_ACK)) {
		printf("   RTP type: FINACK.\n");
		printf(" RTP SeqNum: %d\n", rtp->seq_num);
	} else if (rtp->flags == 0) {
		printf("   RTP type: DATA.\n");
		printf(" RTP SeqNum: %d\n", rtp->seq_num);
	} else {
		fprintf(stderr, "   RTP type: [ERROR] unrecognized type.\n");
		exit(EXIT_FAILURE);
	}
	printf(" RTP Length: %d.\n", rtp->length);
}

int main(int argc, char **argv)
{

	char *dev = NULL;			/* capture device name */
	char errbuf[PCAP_ERRBUF_SIZE];		/* error buffer */
	pcap_t *handle;				/* packet capture handle */
	char filter_exp[] = "port 29876";	/* filter expression */
	struct bpf_program fp;			/* compiled filter program (expression) */
	int num_packets = 256;			/* number of packets to capture */

	/* check for capture device name on command-line */
	if (argc != 2) {
		fprintf(stderr, "error: unrecognized command-line options\n\n");
		print_app_usage();
		exit(EXIT_FAILURE);
	}
	dev = argv[1];

	/* print capture info */
	printf("Device: %s\n", dev);
	printf("Number of packets: %d\n", num_packets);
	printf("Filter expression: %s\n", filter_exp);

	/* open capture device */
	handle = pcap_open_live(dev, SNAP_LEN, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		exit(EXIT_FAILURE);
	}

	if (pcap_datalink(handle) == DLT_EN10MB) {
    		fprintf(stdout, "Link type is Ethernet.\n");
		// ethernet headers are always exactly 14 bytes
		SIZE_ETHERNET = 14;
	} else if (pcap_datalink(handle) == DLT_NULL) {
		fprintf(stdout, "Link type is DLT_NULL (loopback), device doesn't provide Ethernet headers.\n");
		// BSD loopback encapsulation; the link layer header is a 4-byte field, in host byte order, 
		// containing a value of 2 for IPv4 packets
		// see: https://www.tcpdump.org/linktypes.html
		SIZE_ETHERNET = 4;
	}

	/* compile the filter expression */
	if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n",
		    filter_exp, pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}

	/* apply the compiled filter */
	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n",
		    filter_exp, pcap_geterr(handle));
		exit(EXIT_FAILURE);
	}

	/* now we can set our callback function */
	pcap_loop(handle, num_packets, got_packet, NULL);

	/* cleanup */
	pcap_freecode(&fp);
	pcap_close(handle);

	printf("\nCapture complete.\n");

	return 0;
}
