//
// UNIXSocket.cpp for  in /home/agor_m/epitech/cpp_plazza/encapsulation/socket
//
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
//
// Started on  Tue Apr 12 14:49:10 2016 Maxime Agor
// Last update Mon May  2 18:12:18 2016 Maxime Agor
//

#include "UNIXSocket.hpp"

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <iostream>

namespace Socket
{
  UNIXSocket::UNIXSocket()
    : _isServer(false), _fd(-1), _client(-1), _address(std::string(""))
  {
    if ((_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
      throw SocketCreateError();
  }

  /* SERVER API */
  void	UNIXSocket::startServer(std::string const &address, int maxClients)
  {
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, address.c_str(), sizeof(addr.sun_path) - 1);

    unlink(address.c_str());

    if (bind(_fd, (sockaddr *)(&addr), sizeof(addr)) == -1)
      throw SocketConnectError();
    if (listen(_fd, maxClients) == -1)
      throw SocketConnectError();

    _isServer = true;
    _address = address;
  }

  int  UNIXSocket::waitConnection() //! blocking
  {
    int	fd;

    if (!_isServer)
      throw SocketConnectError();
    if ((fd = accept(_fd, NULL, NULL)) == -1)
      throw SocketConnectError();
    if (_client > 0)
      closeConnection();
    _client = fd;
    return fd;
  }

  void  UNIXSocket::closeConnection()
  {
    if (_client > 0)
      close(_client);
    _client = -1;
  }

  void  UNIXSocket::endServer()
  {
    if (_isServer)
      {
	if (_client > 0)
	  close(_client);
	_client = -1;
	close(_fd);
	_isServer = false;
	_fd = -1;
	remove(_address.c_str());
      }
  }

  /* CLIENT API */
  void  UNIXSocket::connect(std::string const &address)
  {
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, address.c_str(), sizeof(addr.sun_path) - 1);

    if (::connect(_fd, (sockaddr *)(&addr), sizeof(addr.sun_path) - 1) == -1)
      throw SocketConnectError();

    _isServer = false;
  }

  void  UNIXSocket::disconnect()
  {
    close(_fd);
    _fd = -1;
  }

  /* I/O API */

  int	UNIXSocket::bytesAvailables() const
  {
    size_t tmp = 0;

    if (ioctl(_isServer ? _client : _fd, FIONREAD, &tmp) == -1)
      {
	perror("error in bytesAvailables :");
	throw SocketIOError();
      }
    return (tmp);
  }

  int   UNIXSocket::read(void *buffer, std::size_t size) const
  {
    int	tmp;

    if ((tmp = ::recv(_isServer ? _client : _fd, buffer, size, 0)) == -1)
      {
	perror("error in read :");
	throw SocketIOError();
      }
    return tmp;
  }

  int   UNIXSocket::write(void const *buffer, std::size_t size) const
  {
    int	tmp;

    if ((tmp = ::send(_isServer ? _client : _fd, buffer, size, 0)) == -1)
      {
	perror("error in write :");
	throw SocketIOError();
      }
    return tmp;
  }

}
