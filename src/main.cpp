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

    print_network_devices();
    cout << '\n';
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
