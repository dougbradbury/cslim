#include "CppUTest/TestHarness.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern "C"
{
  #include "SlimConnectionHandler.h"
  #include "CppUTest/TestHarness_c.h"
  
  struct MockComLink {
    char lastSendMsg[32];
    int lastSendIndex;
    char const * recvStream;
    char const * recvPtr;
  };
  int mock_send_func(void * voidSelf, char * msg, int length)
  {
    MockComLink * self = (MockComLink*)voidSelf;
    strncpy(self->lastSendMsg + self->lastSendIndex, msg, length);
    self->lastSendIndex += length;
    return length;
  }
  int mock_recv_func(void * voidSelf, char * buffer, int length)
  {
    MockComLink * self = (MockComLink*)voidSelf;
    strncpy(buffer, self->recvPtr, length);
    self->recvPtr += length;
    return length;
  }

  char * slimResponse;
  char sentSlimMessage[32];
  void * sentMsgHandler;
  char * mock_handle_slim_message(void* self, char * message)
  {
    strcpy(sentSlimMessage, message);
    sentMsgHandler = self;
    return slimResponse;
  }
  
}

TEST_GROUP(SlimConnectionHandler)
{
    SlimConnectionHandler* slimConnectionHandler;
    MockComLink comLink;
    void* mockMessageHandler;
    void setup()
    {
      slimConnectionHandler = SlimConnectionHandler_Create(&mock_send_func, &mock_recv_func, (void*)&comLink);
      memset(comLink.lastSendMsg, 0, 32);    
      comLink.lastSendIndex = 0;
      mockMessageHandler = (void*)0x123456;
      SlimConnectionHandler_RegisterSlimMessageHandler(slimConnectionHandler, mockMessageHandler, &mock_handle_slim_message);
    }
    
    void teardown()
    {
       SlimConnectionHandler_Destroy(slimConnectionHandler);
    }
};

TEST(SlimConnectionHandler, ShouldSendVersion)
{  
  comLink.recvStream = "000003:bye";
  comLink.recvPtr = comLink.recvStream;

  SlimConnectionHandler_Run(slimConnectionHandler);
  
  STRCMP_EQUAL("Slim -- V0.0\n", comLink.lastSendMsg);
}

TEST(SlimConnectionHandler, ShouldReadMessageAndCallSlimHandler)
{
  comLink.recvStream = "000006:abcdef000003:bye";
  comLink.recvPtr = comLink.recvStream;
  
  slimResponse = (char*)cpputest_malloc(8);
  strcpy(slimResponse, "ghijklm");
  
  SlimConnectionHandler_Run(slimConnectionHandler);
  
  STRCMP_EQUAL("Slim -- V0.0\n000007:ghijklm", comLink.lastSendMsg);
  STRCMP_EQUAL("abcdef", sentSlimMessage);
  CHECK_EQUAL(mockMessageHandler, sentMsgHandler);
}
