//
// INETSocket.cpp for  in /home/agor_m/github/tests_with_cpp_sockets/src
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:32:10 2016 Maxime Agor
// Last update Tue May  3 17:03:56 2016 Maxime Agor
//

#include "INETSocket.hpp"

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <string>
#include <iostream>
#include <sstream>
#include <arpa/inet.h>

namespace Socket
{
  INETSocket::INETSocket()
    : _isServer(false), _fd(-1), _client(-1), _address(std::string(""))
  {
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      throw SocketCreateError(std::string("error in socket creation : ") + std::string(strerror(errno)));
  }

  /* SERVER API */
  void	INETSocket::startServer(std::string const &address, int maxClients)
  {
    struct sockaddr_in	addr;
    std::stringstream	ss;
    int			port;

    try
      {
	ss << address;
	ss >> port;
      }
    catch (...)
      {
	throw SocketConnectError("error in server creation : bad port");
      }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(_fd, (sockaddr *)(&addr), sizeof(addr)) == -1)
      throw SocketConnectError("server : " + std::string(strerror(errno)));
    if (listen(_fd, maxClients) == -1)
      throw SocketConnectError("server : " + std::string(strerror(errno)));

    _isServer = true;
    _address = address;
  }

  int  INETSocket::waitConnection() //! blocking
  {
    int	fd;

    if (!_isServer)
      throw SocketConnectError("cannot wait for a connection : not passive");
    if ((fd = accept(_fd, NULL, NULL)) == -1)
      throw SocketConnectError("server : " + std::string(strerror(errno)));
    if (_client > 0)
      closeConnection();
    _client = fd;
    return fd;
  }

  void  INETSocket::closeConnection()
  {
    if (_client > 0)
      close(_client);
    _client = -1;
  }

  void  INETSocket::endServer()
  {
    if (_isServer)
      {
	if (_client > 0)
	  close(_client);
	_client = -1;
	close(_fd);
	_isServer = false;
	_fd = -1;
      }
  }

  /* CLIENT API */
  void  INETSocket::connect(std::string const &address)
  {
    struct sockaddr_in addr;
    std::stringstream	ss;
    std::string		ip;
    int			port;

    try
      {
	ss << address;
	std::getline(ss, ip, ':');
	ss >> port;
      }
    catch (...)
      {
	throw SocketConnectError("client : bad address/port");
      }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (::connect(_fd, (sockaddr *)(&addr), sizeof(addr)) == -1)
      throw SocketConnectError("client : " + std::string(strerror(errno)));

    _isServer = false;
  }

  void  INETSocket::disconnect()
  {
    close(_fd);
    _fd = -1;
  }

  /* I/O API */

  int	INETSocket::bytesAvailables() const
  {
    size_t tmp = 0;

    if (ioctl(_isServer ? _client : _fd, FIONREAD, &tmp) == -1)
      {
	throw SocketIOError(std::string(strerror(errno)));
      }
    return (tmp);
  }

  int   INETSocket::read(void *buffer, std::size_t size) const
  {
    int	tmp;

    if ((tmp = ::recv(_isServer ? _client : _fd, buffer, size, 0)) == -1)
      {
	throw SocketIOError(std::string(strerror(errno)));
      }
    return tmp;
  }

  int   INETSocket::write(void const *buffer, std::size_t size) const
  {
    int	tmp;

    if ((tmp = ::send(_isServer ? _client : _fd, buffer, size, 0)) == -1)
      {
	throw SocketIOError(std::string(strerror(errno)));
      }
    return tmp;
  }

}
