#include <errno.h>
#include <algorithm>
#include <sys/select.h>

#include <iostream>

#include "Server.hpp"

namespace Socket
{

	Server::Server()
		{
			_stateThread = std::thread();
			_OnConnect = NULL;
			_OnDisconnect = NULL;
			_OnReadPossible = NULL;
			_OnWritePossible = NULL;
			_isRunning = false;
			_timeout.tv_sec = 10;
			_timeout.tv_usec = 0;
		}

	Server::~Server()
		{
		}

	void  Server::start(int port, size_t maxClients)
	{
		SOCKADDR_IN	addr;

		// port checking
		if (port <= 0 || port > 0xFFFF)
			throw SocketCreateError(std::string("server : bad port"));
		_port = port;
		_maxClients = maxClients;

		// create socket
		if ((_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1)
		  throw SocketCreateError(std::string("server : ") +
		  		std::string(strerror(errno)));

		// bind
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		if (bind(_fd, (sockaddr *)(&addr), sizeof(addr)) == -1)
		  throw SocketConnectError("server : " + std::string(strerror(errno)));
		if (listen(_fd, _maxClients) == -1)
		  throw SocketConnectError("server : " + std::string(strerror(errno)));

		// create select() sets
		FD_ZERO(&_fd_set);
		_max_fd = _fd;
		FD_SET(_fd, &_fd_set);

		// run the event loop
		_isRunning = true;
		if (!_stateThread.joinable())
			_stateThread = std::thread(&Server::stateChecker, this);
	}

	void  Server::stop()
	{
		_isRunning = false;
		if (_stateThread.joinable())
			_stateThread.join();
	}

	void  Server::setTimeout(float t)
	{
		_timeout.tv_sec = t / 1000;
		_timeout.tv_usec = (int)(t * 1000) % (1000 * 1000);
	}

	float Server::getTimeout() const
	{
		return (_timeout.tv_sec * 1000) + ((float)(_timeout.tv_usec) / 1000);
	}

	size_t  Server::getMaxClients() const { return _maxClients; }

	void  Server::OnConnect(std::function < void (Socket::Server &, int) > const &callback)
	{
		_OnConnect = callback;
	}

	void  Server::OnDisconnect(std::function < void (Socket::Server &, int) > const &callback)
	{
		_OnDisconnect = callback;
	}

	void  Server::OnReadPossible(std::function < void (Socket::Server &, int, size_t) > const &callback)
	{
		_OnReadPossible = callback;
	}

	void  Server::OnWritePossible(std::function < void (Socket::Server &, int) > const &callback)
	{
		_OnWritePossible = callback;
	}


	// TODO : re-set _max_fd
	void  Server::disconnect(int fd)
	{
		auto found = std::find(_clients.begin(), _clients.end(), fd);

		if (found != std::end(_clients))
		{
			_clients.erase(found);
			closesocket(fd);
			FD_CLR(fd, &_fd_set);

			auto max = (std::max_element(_clients.begin(), _clients.end()));
			if (max != _clients.end())
				_max_fd = *max;
			else
				_max_fd = _fd;

			if (_OnDisconnect)
				_OnDisconnect(*this, fd);
		}
	}

	int  Server::read(int fd, void *buffer, size_t size)
	{
		int tmp;

		if ((tmp = ::recv(fd, (char *)buffer, size, 0)) == -1)
		{
			throw SocketIOError(std::string(strerror(errno)));
		}
		if (size != 0 && tmp == 0)
			disconnect(fd);
		return tmp;
	}

	int  Server::write(int fd, void const *buffer, size_t size)
	{
		int tmp;

		if ((tmp = ::send(fd, (char *)buffer, size, 0)) == -1)
		{
			if (errno == ECONNRESET)
				disconnect(fd);
			else
				throw SocketIOError(std::string(strerror(errno)));
		}
		return tmp;
	}

	size_t Server::bytesAvailables(int fd) const
	{
#ifdef _WIN32
	    u_long bytes = 0;
#else
	    size_t bytes = 0;
#endif	

	    if (::ioctl(fd, FIONREAD, &bytes) == -1)
	    {
	    	throw SocketIOError(std::string(strerror(errno)));
	    }
	    return bytes;
	}

	void Server::stateChecker()
	{
		int fd;
		int nb_fd;
		fd_set read_set, write_set;

		// event loop
		while (_isRunning)
		{
			memcpy(&read_set, &_fd_set, sizeof(_fd_set));
			memcpy(&write_set, &_fd_set, sizeof(_fd_set));

			// wait for an event
			nb_fd = select(_max_fd + 1, &read_set, &write_set, NULL, &_timeout);
			if (nb_fd < 0 && errno != EINTR)
				throw SocketIOError(std::string(strerror(errno)));

			if (FD_ISSET(_fd, &read_set))
			{
				// connection event
				while ((fd = ::accept4(_fd, NULL, NULL, SOCK_NONBLOCK)) != -1)
				{
					_clients.push_back(fd);
					FD_SET(fd, &_fd_set);
					_max_fd = (fd > _max_fd) ? fd : _max_fd;
					if (_OnConnect)
						_OnConnect(*this, fd);
					/*
					else
						std::cout << "no callback set for OnConnect" << std::endl;
						*/
				}
				if (errno != EAGAIN && errno != EWOULDBLOCK)
					throw SocketConnectError(std::string(strerror(errno)));
			}

			for (auto it = _clients.begin(); it != _clients.end(); ++it)
			{
				// read event
				if (FD_ISSET(*it, &read_set))
				{
					if (*it == _fd);
					else if (_OnReadPossible)
						_OnReadPossible(*this, *it, bytesAvailables(*it));
					/*
					else
						std::cout << "no callback set for OnReadPossible" << std::endl;
						*/
				}
				// write event
				if (FD_ISSET(*it, &write_set))
				{
					if (*it == _fd);
					else if (_OnWritePossible)
						_OnWritePossible(*this, *it);
					/*
					else
						std::cout << "no callback set for OnWritePossible" << std::endl;
						*/
				}
			}
		}
	}
};
