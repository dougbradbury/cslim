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