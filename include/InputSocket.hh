//
// InputSocket.hh for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:28 2016 Maxime Agor
// Last update Mon May  2 18:11:48 2016 Maxime Agor
//

#ifndef INPUTSOCKET_HH_
# define INPUTSOCKET_HH_

#include "ASocket.hpp"
#include "StandardSerializer.hh"

namespace Socket
{
  template <class SocketType, class SerialType, class Serializer = StandardSerializer<SerialType> >
  class InputSocket
  {
  public:
    InputSocket()
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
    
    virtual bool messageAvailable()
    {
      return _serializer.messageAvailable();
    }

    SerialType	&operator>>(SerialType &toFill)
    {
      return _serializer.deserialize(toFill);
    }
    
  protected:
    SocketType	_sock;
    Serializer	_serializer;
  };
};

#endif
  