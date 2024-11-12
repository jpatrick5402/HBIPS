#include <string.h>

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


    printf("TS (%d)\nPort (%d)->(%d)\nIP (", h->ts, sourcePort, destPort);
    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
        printf("%c", sourceIP[i]);
    }
    printf(") -> (");
    for (int i = 0; i < INET_ADDRSTRLEN; i++) {
        printf("%c", destIP[i]);
    }
    printf("):\n");

    for (int i = 0; i < h->len; i++) {
        if (isprint(bytes[i]))
            printf("%c ", bytes[i]);
        else
            printf(". ");
    }

    printf("\n\n\n");
    return;
}
