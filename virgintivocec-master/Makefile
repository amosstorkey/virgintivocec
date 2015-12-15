CC=g++
CFLAGS=-Wall

all: main.o
	g++ -o virgintivocec main.o -ldl

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm main.o virgintivocec

install: all
	cp virgintivocec /usr/bin/
	cp virgintivocec.init /etc/init.d/virgintivocec
	chmod ug+x /etc/init.d/virgintivocec

uninstall:
	rm /usr/bin/virgintivocec
	rm /etc/init.d/virgintivocec
