//
// OutputSocket.hh for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:34 2016 Maxime Agor
// Last update Mon May  2 18:11:53 2016 Maxime Agor
//

#ifndef OUTPUTSOCKET_HH_
# define OUTPUTSOCKET_HH_

#include "ASocket.hpp"
#include "StandardSerializer.hh"

namespace Socket
{
  template <class SocketType, class SerialType, class Serializer = StandardSerializer<SerialType> >
  class OutputSocket
  {
  public:
    OutputSocket()
      : _serializer(&_sock)
    {
    }

    virtual void	startServer(std::string const &address)
    {
      _sock.startServer(address);
    }
    virtual int	waitConnection()
    {
      return _sock.waitConnection();
    }
    virtual void	closeConnection()
    {
      _sock.closeConnection();
    }
    virtual void	endServer()
    {
      _sock.endServer();
    }

    virtual void	connect(std::string const &address)
    {
      _sock.connect(address);
    }
    virtual void	disconnect()
    {
      _sock.disconnect();
    }

    SerialType const	&operator<<(SerialType const &toSend) const
    {
      return _serializer.serialize(toSend);
    }

  protected:
    Serializer _serializer;
    SocketType _sock;
  };
};

#endif
