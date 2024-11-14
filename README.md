# Host-Based IPS
_Host-Based Intrusion Prevention System_

## Goal
Design, code, and test a custom-made Intrusion Prevention System

## Language
C++

## Installation
### Linux
pre-requisites
- gcc


setup:
```
git clone https://github.com/jpatrick5402/HBIPS.git
cd HBIPS/
make
cp HBIPS /usr/local/bin/
```


If you want this to run on startup, run this command
`echo "HBIPS -b" >> ~/.bashrc`


## Usage
```
HBIPS [option]
options:
-h : display this help screen
-p : print network devices
-r : run the application
```

**Note: You are free to modify the configuration.txt file to suit your needs, however if you add a '=' char into the file, it will register it as a new config and likely crash

## Resources
- https://github.com/the-tcpdump-group/libpcap
- https://github.com/nmap/npcap
- https://npcap.com/guide/npcap-tutorial.html
- https://npcap.com
- https://github.com/fffaraz/awesome-cpp#networking
- http://recursos.aldabaknocking.com/libpcapHakin9LuisMartinGarcia.pdf
- https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
- https://www.devdungeon.com/content/using-libpcap-c
- https://embeddedguruji.blogspot.com/2014/01/pcapfindalldevs-example.html
- http://yuba.stanford.edu/~casado/pcap/section3.html
- https://github.com/the-tcpdump-group/tcpdump/blob/3b11c29db440d55f7720bef9f7faff562a7e1f39/tcpdump.c#L2582
- https://elf11.github.io/2017/01/22/libpcap-in-C.html
