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

#include "SlimConnectionHandler.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include <assert.h>
#include <vector>

namespace Slim
{
  ConnectionHandler::ConnectionHandler(ComSendFunc const& sendFunction, ComRecvFunc const& recvFunction)
    : m_sendFunc(sendFunction)
    , m_recvFunc(recvFunction)
    , m_slimHandler(0)
  {
  }

  ConnectionHandler::~ConnectionHandler()
  {
  }

  void ConnectionHandler::RegisterSlimMessageHandler(SlimMessageHandler const& handler)
  {
    m_slimHandler = handler;
  }

  int ConnectionHandler::ReadSize()
  {
    char size[7];
    memset(size, 0, 7);
    int size_i = -1;
    if (m_recvFunc(size, 6) == 6)
    {
      char colon;
      if ((m_recvFunc(&colon, 1)) == 1 && colon == ':')
      {
        size_i = boost::lexical_cast<int>(size);
      }
    }
    return size_i;
  }


  int ConnectionHandler::Run()
  {
    assert(m_slimHandler);

    if (m_sendFunc("Slim -- V0.0\n", 13) == -1)
    {
      return -1;
    }

    while (true)
    {
      int size_i = ReadSize();
      if (size_i > 0)
      {
        std::string message(size_i, 0);
        int numbytes = m_recvFunc(&message[0], size_i);
        if (numbytes != size_i)
        {
          printf("did not receive right number of bytes.  %d expected but received %d\n", size_i, numbytes);
          break;
        }

        if (message == "bye")
        {
          break;
        }

        // execute and get response
        std::string response_message = m_slimHandler(message.c_str());
        int response_length = response_message.size();
        int send_result = m_sendFunc((boost::format("%1$06d:%2%") % response_length % response_message).str().c_str(), response_length + 7);
        if (send_result != (response_length + 7))
        {
          printf("Failure to send all data");
          break;
        }
      }
    }

    fflush(stdout);	   
    return 0;
  }
}