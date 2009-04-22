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
		char * recvStream;
		char * recvPtr;
	};
	int mock_send_func(void * voidSelf, char * msg, int length)
	{
		MockComLink * self = (MockComLink*)voidSelf;
		strncpy(self->lastSendMsg, msg, length);
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
	char * mock_handle_slim_message(char * message)
	{
		strcpy(sentSlimMessage, message);
		return slimResponse;
	}
	
}

TEST_GROUP(SlimConnectionHandler)
{
    SlimConnectionHandler* slimConnectionHandler;
	MockComLink comLink;
    void setup()
    {
		slimConnectionHandler = SlimConnectionHandler_Create(&mock_send_func, &mock_recv_func, (void*)&comLink);
		memset(comLink.lastSendMsg, 0, 32);
		
		SlimConnectionHandler_registerSlimMessageHandler(slimConnectionHandler, &mock_handle_slim_message);
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

	SlimConnectionHandler_run(slimConnectionHandler);
	
	STRCMP_EQUAL("Slim -- V0.0\n", comLink.lastSendMsg);
}

TEST(SlimConnectionHandler, ShouldReadMessageAndCallSlimHandler)
{
	comLink.recvStream = "000006:abcdef000003:bye";
	comLink.recvPtr = comLink.recvStream;
	
	slimResponse = cpputest_malloc(8);
	strcpy(slimResponse, "ghijklm");
	
	SlimConnectionHandler_run(slimConnectionHandler);
	
	STRCMP_EQUAL("000007:ghijklm", comLink.lastSendMsg);
	STRCMP_EQUAL("abcdef", sentSlimMessage);
}
