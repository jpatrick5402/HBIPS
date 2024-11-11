#include <iostream>
#include <iomanip>
#include <string.h>
#include <pcap/pcap.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#include "print_network_devices.h"
#include "get_configs.h"

#define MAXBYTES2CAPTURE 2048

using namespace std;

void process_packet(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet);

int main (int argc, char *argv[]) {
    cout << R""""(
 /$$   /$$ /$$$$$$$  /$$$$$$ /$$$$$$$   /$$$$$$
| $$  | $$| $$__  $$|_  $$_/| $$__  $$ /$$__  $$
| $$  | $$| $$  \ $$  | $$  | $$  \ $$| $$  \__/
| $$$$$$$$| $$$$$$$   | $$  | $$$$$$$/|  $$$$$$
| $$__  $$| $$__  $$  | $$  | $$____/  \____  $$
| $$  | $$| $$  \ $$  | $$  | $$       /$$  \ $$
| $$  | $$| $$$$$$$/ /$$$$$$| $$      |  $$$$$$/
|__/  |__/|_______/ |______/|__/       \______/
)"""" << '\n';

    bool run_in_foreground;
    char errbuf[PCAP_ERRBUF_SIZE];
    int i = 0, count = 0;
    pcap_t *descr = NULL;

    if (strcmp(argv[1], "-f") == 0) {
        run_in_foreground = true;
    } else if (strcmp(argv[1], "-b") == 0) {
        run_in_foreground = false;
    } else if (strcmp(argv[1], "-p") == 0) {
        print_network_devices();
        cout << '\n';
        return 0;
    } else {
        cout << "usage: " << argv[0] << " [option]" << '\n';
        cout << "options: " << '\n';
        cout << setw(10) << "-h" << ": display this help screen" << '\n';
        cout << setw(10) << "-f" << ": run this application in forground" << '\n';
        cout << setw(10) << "-b" << ": run this application in background" << '\n';
        cout << setw(10) << "-p" << ": print network devices" << '\n';
        return 0;
    }


    unordered_map<string, int> configs = get_configs(); // choose configuration items
    cout << '\n';

    // initialize any processes before starting to loop through traffic
    unsigned int temp_null = 0;
    pcap_init(temp_null, errbuf);

    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t * interfaces, * temp;
    if (pcap_findalldevs(&interfaces,error) == -1) {
        printf("Error in pcap findall devs\n");
        return -1;
    }

    cout << "Starting Host-Based IPS" << '\n';

    // open device for capture
    descr = pcap_open_live(interfaces->name, MAXBYTES2CAPTURE, 1, 512, errbuf);
    cout << descr << '\n';
    cout << interfaces->name << '\n';
    // loop and run process_packet for every packet
    pcap_loop(descr, -1, process_packet, (u_char *)&count);

    return 0;
}

void process_packet(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet) {

    int i = 0, *counter = (int *)arg;
    printf("Packet Count: %d\n", ++(*counter));

    for (i = 0; i < pkthdr->len; i++) {
        if (isprint(packet[i]))
            printf("%c ", packet[i]);
        else
            printf(". ");

        if ((i % 16 == 0 && i != 0) || i == pkthdr->len - 1)
            printf("\n");
    }

    return;
}
