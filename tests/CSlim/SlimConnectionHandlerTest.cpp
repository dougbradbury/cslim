#include "CppUTest/TestHarness.h"
#include "SlimList.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

extern "C"
{
  #include "SlimConnectionHandler.h"
  #include "CppUTest/TestHarness_c.h"
  
  struct MockComLink {
    char lastSendMsg[32];
    int lastSendIndex;
    char const * recvStream;
    char const * recvPtr;
    SlimList* sendReturnCodes;
  };
  int mock_send_func(void * voidSelf, char * msg, int length)
  {
    MockComLink * self = (MockComLink*)voidSelf;
    strncpy(self->lastSendMsg + self->lastSendIndex, msg, length);
    self->lastSendIndex += length;

	int result = length;

    if (SlimList_GetLength(self->sendReturnCodes) > 0)
    {
    	char* resultAsAString = SlimList_GetStringAt(self->sendReturnCodes, 0);
    	result = atoi(resultAsAString);

    	SlimList_PopHead(self->sendReturnCodes);
    }

    return result;
  }

  int mock_recv_func(void * voidSelf, char * buffer, int length)
  {
    MockComLink * self = (MockComLink*)voidSelf;
    assert(self->recvPtr != NULL);

    int result = length;
    strncpy(buffer, self->recvPtr, length);
    if (strlen(self->recvPtr) < result)
      result = strlen(self->recvPtr);

    self->recvPtr += result;

    if (result == 0) result = -1;

    return result;
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
  
  void AddSendResult(MockComLink * self, int result)
  {
	  char string[22];
	  sprintf(string, "%d", result);
	  SlimList_AddString(self->sendReturnCodes, string);
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

      comLink.sendReturnCodes = SlimList_Create();
      SlimConnectionHandler_RegisterSlimMessageHandler(slimConnectionHandler, mockMessageHandler, &mock_handle_slim_message);
    }
    
    void teardown()
    {
       SlimConnectionHandler_Destroy(slimConnectionHandler);
       SlimList_Destroy(comLink.sendReturnCodes);
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

TEST(SlimConnectionHandler, CanMockSendResultsAsPartOfTest)
{
	int expected = 1;
	AddSendResult(&comLink, expected);
	char message[] = "";

	int actual = mock_send_func(&comLink, message, 0);
	CHECK_EQUAL(expected, actual);
}

TEST(SlimConnectionHandler, HandlesSendErrorWithoutMemoryLeak)
{
	char message[] = "";
	comLink.recvStream = message;
	comLink.recvPtr = comLink.recvStream;

	AddSendResult(&comLink, -1);
	SlimConnectionHandler_Run(slimConnectionHandler);
}

