#include "stdafx.h"
#include <SlimConnectionHandler.h>

#include <boost/bind.hpp>

#include <algorithm>

using namespace Slim;

namespace
{
  struct MockComLink 
  {
    int Send(char const* msg, int length);
    int Receive(char * buffer, int length);

    std::string lastSendMsg;
    char const* recvStream;
    char const* recvPtr;
  };

  inline
  int MockComLink::Send(char const* msg, int length)
  {
    lastSendMsg = std::string(msg, length);
    return length;
  }

  inline
  int MockComLink::Receive(char * buffer, int length)
  {
    std::copy(recvPtr, recvPtr + length, buffer);
    recvPtr += length;
    return length;
  }

  std::string slimResponse;
  std::string sentSlimMessage;
  std::string mock_handle_slim_message(char const* message)
  {
    sentSlimMessage = message;
    return slimResponse;
  }
}

struct InitSlimConnectionHandlerTest
{
  InitSlimConnectionHandlerTest()
    : slimConnectionHandler(
        boost::bind(&MockComLink::Send, &comLink, _1, _2),
        boost::bind(&MockComLink::Receive, &comLink, _1, _2))
  {
    slimConnectionHandler.RegisterSlimMessageHandler(mock_handle_slim_message);
  }

  ~InitSlimConnectionHandlerTest()
  {
  }

  MockComLink comLink;
  ConnectionHandler slimConnectionHandler;
};

BOOST_FIXTURE_TEST_SUITE(SlimConnectionHandlerTestSuite, InitSlimConnectionHandlerTest)

BOOST_AUTO_TEST_CASE(ShouldSendVersion)
{	
  comLink.recvStream = "000003:bye";
  comLink.recvPtr = comLink.recvStream;

  slimConnectionHandler.Run();

  BOOST_CHECK_EQUAL("Slim -- V0.0\n", comLink.lastSendMsg);
}

BOOST_AUTO_TEST_CASE(ShouldReadMessageAndCallSlimHandler)
{
  comLink.recvStream = "000006:abcdef000003:bye";
  comLink.recvPtr = comLink.recvStream;

  slimResponse = "ghijklm";
  slimConnectionHandler.Run();

  BOOST_CHECK_EQUAL("000007:ghijklm", comLink.lastSendMsg);
  BOOST_CHECK_EQUAL("abcdef", sentSlimMessage);
}

BOOST_AUTO_TEST_SUITE_END()