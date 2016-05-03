//
// main.cpp for  in /home/agor_m/epitech/cpp_plazza/encapsulation/socket
// 
// Made by Maxime Agor
// Login   <agor_m@epitech.net>
// 
// Started on  Tue Apr 12 15:35:49 2016 Maxime Agor
// Last update Mon May  2 18:11:19 2016 Maxime Agor
//

#include "INETSocket.hpp"
#include "OutputSocket.hh"
#include "StandardSerializer.hh"
#include <string>
#include <iostream>
#include <string.h>

int	main()
{
  int toSend = 42;
  Socket::OutputSocket<Socket::INETSocket, int>	s;


  s.connect("127.0.0.1:5000");
  s << toSend;
  s.disconnect();
  return 0;
}
