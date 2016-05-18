//
// INETSocket.hpp for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:47 2016 Maxime Agor
// Last update Mon May  2 18:10:15 2016 Maxime Agor
//

#ifndef INETSOCKET_HPP_
# define INETSOCKET_HPP_

# include "ASocket.hpp"

#ifdef _WIN32
# include <winsock2.h>
# define ioctl ioctlsocket

#elif __linux__
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <cstring>
# include <unistd.h>
# include <arpa/inet.h>
typedef int SOCKET;
typedef sockaddr_in SOCKADDR_IN;
typedef sockaddr SOCKADDR;
typedef in_addr IN_ADDR;
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1
# define closesocket(s) close(s)

#else
# error OS not supported
#endif



namespace Socket
{
  class INETSocket : public ASocket
  {
  public:
    INETSocket();

    virtual ~INETSocket() {};
    
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
    bool	    	_isServer;
    SOCKET    		_fd;
    int	  		    _client;
    std::string 	_address;
  };
};

#endif
