#include <stdio.h>
#include <pcap.h>

int print_network_devices() {
    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t * interfaces, * temp;
    int i = 0;
    if (pcap_findalldevs(&interfaces,error) == -1) {
        printf("Error in pcap findall devs\n");
        return -1;
    }

    printf("The interfaces present on the system are:\n");
    for(temp = interfaces; temp ; temp = temp->next) {
        printf("%d  :  %s\n", i++, temp->name);
    }

    return 0;
}
