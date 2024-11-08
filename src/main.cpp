#include "print_network_devices.h"
#include "display_configuration_menu.h"

using namespace std;

int main (int argc, char *argv[]) {
    print_network_devices();

    unordered_map<string, int> configs = display_configuration_menu(); // choose configuration items

    // loop through program catpuring packets and reporting issues

    return 0;
}
