//
// UNIXSocket.hpp for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:47 2016 Maxime Agor
// Last update Mon May  2 18:10:40 2016 Maxime Agor
//

#ifndef UNIXSOCKET_HPP_
# define UNIXSOCKET_HPP_

# include "ASocket.hpp"

namespace Socket
{
  class UNIXSocket : public ASocket
  {
  public:
    UNIXSocket();

    virtual ~UNIXSocket() {};
    
    /* SERVER API */
    virtual void	startServer(std::string const &address, int maxClients = 1);
    virtual int		waitConnection(); //! blocking
    virtual void	closeConnection();
    virtual void	endServer();
    
    /* CLIENT API */
    virtual void	connect(std::string const &address);
    virtual void	disconnect();
    
    /* I/O API */
    virtual int		bytesAvailables() const;
    virtual int		read(void *buffer, std::size_t size) const; // blocking
    virtual int		write(void const *buffer, std::size_t size) const;
    
  protected:
    bool		_isServer;
    int			_fd;
    int			_client;
    std::string 	_address;
  };
};

#endif
