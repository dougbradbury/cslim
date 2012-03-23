#pragma once

#include <boost/function.hpp>

#include <stdio.h>
#include <winsock2.h>

///////////////////////////////////////////////////////////////////////////////
//
//  SocketServer is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

namespace Slim
{
  class SocketServer
  {
  public:
    SocketServer();
    ~SocketServer();

    typedef boost::function<int (SOCKET)>  ConnectionHandler;
    void RegisterHandler(ConnectionHandler const& handler);
    void Run(int portToListenTo);

  private:
    void StartWSA();
    void BindToSocket();
    void Shutdown();
    void CreateSocket();
    void WaitForConnection();

    ConnectionHandler m_handler;
    SOCKET m_socket;
    int m_portToListenTo;	
  };
}
