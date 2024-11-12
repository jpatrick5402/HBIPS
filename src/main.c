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
#include "process_packet.h"

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
