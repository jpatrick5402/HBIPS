make:
	gcc src/main.c -lpcap -o HIDS
	./HIDS -h
