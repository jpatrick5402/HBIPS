make:
	gcc src/main.c -lpcap -o NIDS
	./NIDS -h
