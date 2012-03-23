// Fitnesse Slim plugin port for VC++ 2010
// 
// Copyright (C) 2012  Chih C. Von
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "TcpComLink.h"

namespace Slim
{
  TcpComLink::TcpComLink(SOCKET socket)
    : m_socket(socket)
  {
  }

  TcpComLink::~TcpComLink()
  {
  }

  int TcpComLink::Send(char const* msg, int length)
  {
    int total = 0;        // how many bytes we've sent
    int bytesleft = length; // how many we have left to send
    int n;

    while (total < length) 
    {
      n = send(m_socket, msg+total, bytesleft, 0);
      if (n == -1) 
      { 
        break; 
      }

      total += n;
      bytesleft -= n;
    }

    return total;
  } 

  int TcpComLink::Receive(char * buffer, int length)
  {
    //No MSG_WAITALL flag in winsock2 ????
    int bytesReceived = 0;
    while (bytesReceived < length)
    {
      bytesReceived += recv(m_socket, buffer + bytesReceived, length - bytesReceived, 0);
    }

    return bytesReceived;
  }
}