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
    // loop through program catpuring packets and reporting issues

    cout << "Starting IPS" << '\n';
    while (true) {

        // capture packets
        // determine if packets are malicious
        // log results
    }

    return 0;
}
