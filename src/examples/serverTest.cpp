#include "Server.hpp"
#include <iostream>

void onco(Socket::Server &s, int fd)
{
	std::cout << "client connected : " << fd << std::endl;
}
void ondisco(Socket::Server &s, int fd)
{
	std::cout << "client disconnected : " << fd << std::endl;
}
void onwrite(Socket::Server &s, int fd)
{
	std::cout << "write possible : " << fd << std::endl;
}
void onread(Socket::Server &s, int fd, int size)
{
	char buffer[size + 1];

	memset(buffer, 0, size + 1);
	s.read(fd, buffer, size);
	std::cout << "read possible : " << fd << " " << size << " bytes : " << buffer << std::endl;
}

void onstart(Socket::Server &s, int port)
{
	std::cout << "Server started on port " << port << std::endl;
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	Socket::Server s;

	s.OnStart(&onstart);
	s.OnConnect(&onco);
	s.OnDisconnect(&ondisco);
	s.OnReadPossible(&onread);
	s.OnWritePossible(&onwrite);

	s.start(5000);

	while (1) {
#ifdef _WIN32
        Sleep(60000);
#else
		sleep(60);
#endif
	}

#ifdef _WIN32
    WSACleanup();
#endif
}
