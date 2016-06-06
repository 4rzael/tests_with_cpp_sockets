#include "Client.hpp"
#include <iostream>

void ondisco(Socket::Client &s)
{
	std::cout << "client disconnected : " << std::endl;
}
void onwrite(Socket::Client &s)
{
	std::cout << "write possible : " << std::endl;
}
void onread(Socket::Client &s, int size)
{
	char buffer[size + 1];

	memset(buffer, 0, size + 1);
	s.read(buffer, size);
	std::cout << "read possible : " << size << " bytes : " << buffer << std::endl;
}

void onstart(Socket::Client &s, std::string const &addr, int port)
{
  std::cout << "Client started : " << addr << ", on port " << port << std::endl;
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	Socket::Client s;

	s.OnStart(&onstart);
	s.OnDisconnect(&ondisco);
	s.OnReadPossible(&onread);
	s.OnWritePossible(&onwrite);

	s.start("127.0.0.1", 5000);

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
