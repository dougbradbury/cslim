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