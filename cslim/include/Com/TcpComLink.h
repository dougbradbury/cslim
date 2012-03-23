#pragma once

#include <winsock2.h>

///////////////////////////////////////////////////////////////////////////////
//
//  TcpComLink is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

namespace Slim
{
  class TcpComLink
  {
  public:
    TcpComLink(SOCKET socket);
    ~TcpComLink();

    int Send(char const* msg, int length);
    int Receive(char* buffer, int length);

  private:
    SOCKET m_socket;
  };
}