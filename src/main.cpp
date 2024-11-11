#include <iostream>
#include <iomanip>
#include <string.h>
#include <pcap/pcap.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#include "print_network_devices.h"
#include "get_configs.h"

using namespace std;

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

    if (strcmp(argv[1], "-f") == 0) {
        run_in_foreground = true;
    } else if (strcmp(argv[1], "-b") == 0) {
        run_in_foreground = false;
    } else if (strcmp(argv[1], "-p") == 0) {
        print_network_devices();
        cout << '\n';
        exit(0);
    } else {
        cout << "usage: " << argv[0] << " [option]" << '\n';
        cout << "options: " << '\n';
        cout << setw(10) << "-h" << ": display this help screen" << '\n';
        cout << setw(10) << "-f" << ": run this application in forground" << '\n';
        cout << setw(10) << "-b" << ": run this application in background" << '\n';
        cout << setw(10) << "-p" << ": print network devices" << '\n';
        exit(0);
    }


    unordered_map<string, int> configs = get_configs(); // choose configuration items
    cout << '\n';

    // initialize any processes before starting to loop through traffic

    cout << "Starting IPS" << '\n';
    while (true) {
        // capture packet
        // determine if packet is malicious
        // store packet in a file/log
        // run through log file to check for repeated attack attempt
        // log results
    }

    return 0;
}
