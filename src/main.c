#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pcap/pcap.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include "print_network_devices.h"
#include "print_help.h"
#include "hash_table.h"

void process_packet(u_char *user, const struct pcap_pkthdr* h, const u_char * bytes);

int main (int argc, char *argv[]) {
    printf(R""""(
 /$$   /$$ /$$$$$$$  /$$$$$$ /$$$$$$$   /$$$$$$
| $$  | $$| $$__  $$|_  $$_/| $$__  $$ /$$__  $$
| $$  | $$| $$  \ $$  | $$  | $$  \ $$| $$  \__/
| $$$$$$$$| $$$$$$$   | $$  | $$$$$$$/|  $$$$$$
| $$__  $$| $$__  $$  | $$  | $$____/  \____  $$
| $$  | $$| $$  \ $$  | $$  | $$       /$$  \ $$
| $$  | $$| $$$$$$$/ /$$$$$$| $$      |  $$$$$$/
|__/  |__/|_______/ |______/|__/       \______/

)"""");

    // configs
    bool run_in_foreground;

    // required declarations
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t * pd;
    pcap_if_t * interfaces;

    // parse flags for commands
    if (argc < 2) {
        print_help();
        return -1;
    } else if (strcmp(argv[1], "-f") == 0) {
        run_in_foreground = true;
    } else if (strcmp(argv[1], "-b") == 0) {
        run_in_foreground = false;
    } else if (strcmp(argv[1], "-p") == 0) {
        print_network_devices();
        printf("\n");
        return 0;
    } else {
        print_help();
        return 0;
    }

    // initialize any processes before running pcap_loop()
    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        printf("Error in pcap findall devs\n");
        return -1;
    }

    printf("Starting Host-Based IPS\n");

    // open device for capture
    pd = pcap_open_live(interfaces->name, 65536, 1, 1000, errbuf);
    if (pd == NULL) {
        fprintf(stderr, "Cant open %s\nEnsure you are running as sudo/admin\n", interfaces->name);
        return -1;
    }

    // loop and run process_packet() for every packet
    // see process_packet.h for code that runs
    pcap_loop(pd, 0, process_packet, NULL);

    // release the network device so there are no issues
    pcap_freealldevs(interfaces);

    return 0;
}


void process_packet(u_char *user, const struct pcap_pkthdr* h, const u_char * bytes) {

    const struct ether_header* ethernetHeader;
    const struct ip* ipHeader;
    const struct tcphdr* tcpHeader;
    const struct udphdr* udpHeader;
    char sourceIP[INET_ADDRSTRLEN] = "";
    char destIP[INET_ADDRSTRLEN] = "";
    u_int sourcePort = 0, destPort = 0;
    u_char *data;
    int dataLength = 0;
    int i;

    ethernetHeader = (struct ether_header*)bytes;
    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {

        ipHeader = (struct ip*)(bytes + sizeof(struct ether_header));
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIP, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIP, INET_ADDRSTRLEN);

        if (ipHeader->ip_p == IPPROTO_TCP) {
            tcpHeader = (struct tcphdr*)(bytes + sizeof(struct ether_header) + sizeof(struct ip));
            sourcePort = ntohs(tcpHeader->source);
            destPort = ntohs(tcpHeader->dest);
            data = (u_char*)(bytes + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
            dataLength = h->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

        } else if (ipHeader->ip_p == IPPROTO_UDP) {
            udpHeader = (struct udphdr*)(bytes + sizeof(struct ether_header) + sizeof(struct ip));
            sourcePort = ntohs(udpHeader->source);
            destPort = ntohs(udpHeader->dest);
        } else if (ipHeader->ip_p == IPPROTO_ICMP) {
        }
    }

    printf("TS (%d)\nPort (%d)->(%d)\nIP (", h->ts, sourcePort, destPort);
    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
        printf("%c", sourceIP[i]);
    }
    printf(") -> (");
    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
        printf("%c", destIP[i]);
    }
    printf("):\n");

    for (int i = 0; i < h->len; i++) {
        if (isprint(bytes[i]))
            printf("%c ", bytes[i]);
        else
            printf(". ");
    }

    printf("\n\n\n");
    return;
}
