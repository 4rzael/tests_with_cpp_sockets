//
// server.cpp for  in /home/agor_m/github/tests_with_cpp_sockets/src/examples
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Mon May  2 18:08:14 2016 Maxime Agor
// Last update Tue May  3 17:00:13 2016 Maxime Agor
//

#include "INETSocket.hpp"
#include "InputSocket.hh"
#include "StandardSerializer.hh"
#include <string>
#include <iostream>
#include <cstring>

int	main()
{
  int o;
  Socket::InputSocket<Socket::INETSocket, int> s;

  s.startServer("5000");
  s.waitConnection();
  while (!s.messageAvailable());
  s >> o;
  std::cout << o << std::endl;
  s.endServer();
  return 0;
}
