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

#pragma once

#include <boost/function.hpp>

#include <string>

///////////////////////////////////////////////////////////////////////////////
//
//  SlimConnectionHandler is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

namespace Slim
{
  class ConnectionHandler
  {
  public:
    typedef boost::function<int (char * msg, int length)>  ComRecvFunc;
    typedef boost::function<int (char const* msg, int length)>  ComSendFunc;
    ConnectionHandler(ComSendFunc const& sendFunction, ComRecvFunc const& recvFunction);
    ~ConnectionHandler();

    int Run();

    typedef boost::function<std::string (char const*)>  SlimMessageHandler;
    void RegisterSlimMessageHandler(SlimMessageHandler const& handler);

  private:
    int ReadSize();

    ComSendFunc m_sendFunc;
    ComRecvFunc m_recvFunc;
    SlimMessageHandler m_slimHandler;
  };
}