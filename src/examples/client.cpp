//
// main.cpp for  in /home/agor_m/epitech/cpp_plazza/encapsulation/socket
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Tue Apr 12 15:35:49 2016 Maxime Agor
// Last update Mon May  2 18:11:19 2016 Maxime Agor
//

#include <string>
#include <iostream>
#include <string.h>

#include "OutputSocket.hh"
#include "StandardSerializer.hh"
#include "INETSocket.hpp"

int	main()
{

#ifdef _WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa))
    return 1;
#endif

  int toSend = 42;
  Socket::OutputSocket<Socket::INETSocket, int>	s;


  s.connect("127.0.0.1:5000");
  s << toSend;
  s.disconnect();

#ifdef _WIN32
  WSACleanup();
#endif

  return 0;
}
