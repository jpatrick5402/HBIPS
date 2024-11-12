#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pcap/pcap.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <ctype.h>

#include "print_network_devices.h"

#define MAXBYTES2CAPTURE 2048

void process_packet(u_char *user, const struct pcap_pkthdr* h, const u_char * bytes);
void print_help();

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
    unsigned int temp_null = 0;
    pcap_if_t * interfaces, * temp;

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

    // initialize any processes before starting to loop through traffic
    pcap_init(temp_null, errbuf);

    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        printf("Error in pcap findall devs\n");
        return -1;
    }

    printf("Starting Host-Based IPS\n");

    // open device for capture
    pd = pcap_open_live(interfaces->name, 65536, 1, 1000, errbuf);
    if (pd == NULL) {
        fprintf(stderr, "Cant open %s\nMaybe sudo/run as administrator is needed\n", interfaces->name);
        return -1;
    }
    
    // loop and run process_packet for every packet
    pcap_loop(pd, 0, process_packet, NULL);

    pcap_freealldevs(interfaces);

    return 0;
}

void process_packet(u_char *user, const struct pcap_pkthdr* h, const u_char * bytes) {
    
    for (int i = 0; i < h->len; i++) {
        if (isprint(bytes[i]))
            printf("%c ", bytes[i]);
        else
            printf(". ");
    }
    printf("\n\n\n");

    return;
}

void print_help() {
    printf("usage: sudo HBIPS [option]\n");
    printf("options: \n");
    printf("-h : display this help screen\n");
    printf("-f : run this application in foreground\n");
    printf("-b : run this application in the background\n");
    printf("-p : print network devices\n");
}
