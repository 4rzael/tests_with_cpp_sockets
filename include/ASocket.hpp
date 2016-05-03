//
// ASocket.hpp for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:51 2016 Maxime Agor
// Last update Tue May  3 16:31:14 2016 Maxime Agor
//

#ifndef ASOCKET_HPP_
# define ASOCKET_HPP_

# include <cstddef>
# include <string>
# include <list>

#include "SocketErrors.hpp"

namespace Socket
{
  class ASocket
  {
  public:
    virtual ~ASocket() {};
    
    /* SERVER API */

    // throws SocketConnectError
    virtual void	startServer(std::string const &address, int maxClients = 1) = 0;
    virtual int		waitConnection() = 0; //! blocking, returns the client fd
    virtual void	closeConnection() = 0;
    virtual void	endServer() = 0;

    /* CLIENT API */

    // throws SocketConnectError
    virtual void	connect(std::string const &address) = 0;
    virtual void	disconnect() = 0;

    /* I/O API */

    virtual int		bytesAvailables() const = 0;
    // throws SocketIOError
    virtual int		read(void *buffer, std::size_t size) const = 0;
    // throws SocketIOError
    virtual int		write(void const *buffer, std::size_t size) const = 0;

  protected:
    bool		_isServer;
    int			_fd;
    int			_client;
    std::string		_address;
  };
};

#endif
