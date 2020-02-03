.PHONY build:
build:
	@g++ -c -fpic -std=c++14 -I ./include ./src/OcapReplaySaver2.cpp ./src/easylogging++.cc; \
	g++ OcapReplaySaver2.o easylogging++.o -o OcapReplaySaver2.so;
