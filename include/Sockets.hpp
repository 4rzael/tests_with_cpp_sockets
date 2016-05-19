//
// SocketS.hpp for  in /home/agor_m/github/tests_with_cpp_sockets/include
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 15:20:51 2016 Maxime Agor
// Last update Tue May  3 16:31:14 2016 Maxime Agor
//

#ifndef SOCKETS_HPP_
# define SOCKETS_HPP_

# include <cstddef>
# include <string>
# include <vector>

#ifdef _WIN32
# include <winsock2.h>
# define ioctl ioctlsocket

#else
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
typedef protoent PROTOENT;
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1
# define closesocket(s) close(s)
#endif

#include "SocketErrors.hpp"

#endif
