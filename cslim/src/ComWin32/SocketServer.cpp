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

#include "SocketServer.h"

namespace Slim
{
  SocketServer::SocketServer()
    : m_handler(0)
    , m_socket()
    , m_portToListenTo(-1)
  {
  }

  SocketServer::~SocketServer()
  {
    Shutdown();
  }

  void SocketServer::RegisterHandler(ConnectionHandler const& handler)
  {
    m_handler = handler;
  }

  void SocketServer::Run(int portToListenTo)
  {
    m_portToListenTo = portToListenTo;

    StartWSA();
    CreateSocket();
    BindToSocket();
    WaitForConnection();	
    Shutdown();
  }

  void SocketServer::StartWSA()
  {
    WSADATA WsaDat;
    if (!WSAStartup(MAKEWORD(2,2), &WsaDat))
    {
      printf("WSA Initialization failed!");
    }
  }

  void SocketServer::BindToSocket()
  {
    SOCKADDR_IN serverInf;
    serverInf.sin_family = AF_INET;
    serverInf.sin_addr.s_addr = INADDR_ANY;
    serverInf.sin_port = htons(m_portToListenTo);

    if (bind(m_socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
    {
      printf("Unable to bind socket!");
    }
  }

  void SocketServer::Shutdown()
  {
    shutdown(m_socket, SD_SEND);
    closesocket(m_socket);
    WSACleanup();
  }

  void SocketServer::CreateSocket()
  {
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
      printf("Socket creation failed.");
    }
  }

  void SocketServer::WaitForConnection()
  {
    listen(m_socket, 1);

    SOCKET TempSock = SOCKET_ERROR;
    while(TempSock == SOCKET_ERROR)
    {
      TempSock = accept(m_socket, NULL, NULL);
    }

    m_handler(TempSock);
    closesocket(TempSock);
  }
}