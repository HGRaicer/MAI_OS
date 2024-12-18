all:
	g++ -std=c++17 -o server server.cpp -lzmq
	g++ -std=c++17 -o node node.cpp -lzmq

clear:
	rm -f node server