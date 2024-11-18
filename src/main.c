#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
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

struct hash_table IP_hash_table;
char ** IP_addr_table;
int IP_addr_table_size = 0;

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

    // required declarations
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t * pd;
    pcap_if_t * interfaces;
    IP_hash_table.m = 1000000;
    IP_hash_table.table = (long int *)malloc(sizeof(long int) * IP_hash_table.m);
    init_0_ht(&IP_hash_table);
    IP_addr_table = (char **)malloc(16 * 1000);
    for (int i = 0; i < 1000; i++) {
        IP_addr_table[i] = (char *)malloc(16);
    }

    // parse flags for commands
    if (argc != 2) {
        print_help();
        return -1;
    } else if (strcmp(argv[1], "-r") == 0) {
        printf("Starting Host-Based IPS\n");
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

    // print out the pertainant information about this packet
    printf("Unix Time stamp (%d)\n", h->ts);
    printf("IP (");
    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
        printf("%c", sourceIP[i]);
    }
    printf(":%d)->(", sourcePort);
    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
        printf("%c", destIP[i]);
    }
    printf(":%d)\n", destPort);

    // print out the type of packet
    printf("TYPE (");
    if (ipHeader->ip_p) {
        if (ipHeader->ip_p == IPPROTO_ICMP) {
            printf("ICMP");
        } else if (ipHeader->ip_p == IPPROTO_UDP) {
            printf("UDP");
        } else if (ipHeader->ip_p == IPPROTO_TCP) {
            printf("TCP");
        } else {
            printf("Unknown");
        }
    }
    printf(")\n");

    int place = get_place(&IP_hash_table, sourceIP);
    if (IP_hash_table.table[place] == 0 && place != 1) {
        for (int i = 0; sourceIP[i] != '\0'; i++) {
            IP_addr_table[IP_addr_table_size][i] = sourceIP[i];
        }
        IP_addr_table_size++;
    }

    // insert the IP address into the hash_table
    insert_ht(&IP_hash_table, sourceIP);

    // print out raw data of packet
    for (int i = 0; i < h->len; i++) {
        if (isprint(bytes[i]))
            printf("%c ", bytes[i]);
        else
            printf(". ");
    }
    printf("\n");

    // print out IP address and count of that address
    printf("%15s Count\n", "Address");
    for (int i = 0; i < IP_addr_table_size; i++) {
        place = get_place(&IP_hash_table, IP_addr_table[i]);
        printf("%15s %d\n", IP_addr_table[i], IP_hash_table.table[place]);
    }

    printf("\n");

    // write output to output.csv
    FILE * fptr;
    fptr = fopen("output.csv", "w");
    for (int i = 0; i < IP_addr_table_size; i++) {
        place = get_place(&IP_hash_table, IP_addr_table[i]);
        fprintf(fptr, "%s,%d\n", IP_addr_table[i], IP_hash_table.table[place]);
    }
    fclose(fptr);

    return;
}
