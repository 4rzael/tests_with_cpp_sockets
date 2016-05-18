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
	std::cout << "read possible : " << fd << " " << size << " bytes" << std::endl;
}


int main()
{
	Socket::Server s;

	s.OnConnect(&onco);
	s.OnDisconnect(&ondisco);
	s.OnReadPossible(&onread);
	s.OnWritePossible(&onwrite);

	s.start(5000);

	while (1)
		sleep(60);
}