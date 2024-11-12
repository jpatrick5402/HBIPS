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

    bool run_in_foreground;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t * pd;

    if (strcmp(argv[1], "-f") == 0) {
        run_in_foreground = true;
    } else if (strcmp(argv[1], "-b") == 0) {
        run_in_foreground = false;
    } else if (strcmp(argv[1], "-p") == 0) {
        print_network_devices();
        printf("\n");
        return 0;
    } else {
        printf("usage: sudo %s [option]\n", argv[0]);
        printf("options: \n");
        printf("-h : display this help screen\n");
        printf("-f : run this application in foreground\n");
        printf("-b : run this application in the background\n");
        printf("-p : print network devices\n");
        return 0;
    }

    // initialize any processes before starting to loop through traffic
    unsigned int temp_null = 0;
    pcap_init(temp_null, errbuf);

    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t * interfaces, * temp;
    if (pcap_findalldevs(&interfaces,error) == -1) {
        printf("Error in pcap findall devs\n");
        return -1;
    }

    printf("Starting Host-Based IPS\n");


    // open device for capture
    pd = pcap_open_live(interfaces->name, 65536, 1, 1000, errbuf);
    if (pd == NULL) {
        fprintf(stderr, "Cant open %s\n", interfaces->name);
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

        if ((i % 16 == 0 && i != 0) || i == h->len - 1)
            printf("\n");
    }
    printf("\n\n\n");

    return;
}
