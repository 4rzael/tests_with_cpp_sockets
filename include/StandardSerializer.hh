//
// StandardSerializer.hh for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:21 2016 Maxime Agor
// Last update Mon May  2 18:10:01 2016 Maxime Agor
//

#ifndef STANDARD_SERIALIZER_HH_
# define STANDARD_SERIALIZER_HH_

#include "ASocket.hpp"

namespace Socket
{
  template <class T>
  class StandardSerializer
  {
  public:
    StandardSerializer(ASocket *socket)
      : _sock(socket)
    {
    }
    
    virtual ~StandardSerializer()
    {
    }
    
    virtual T const &serialize(T const &toSerialize) const
    {
      _sock->write(&toSerialize, sizeof(T));
      return (toSerialize);
    }
    
    virtual T &deserialize(T &toFill) const
    {
      if (!messageAvailable())
	return (toFill);
      _sock->read(&toFill, sizeof(T));
      return (toFill);
    }

    virtual bool messageAvailable() const
    {
      return (_sock->bytesAvailables() >= sizeof(T));
    }
    
  protected:
    ASocket *_sock;
  };
};

#endif
