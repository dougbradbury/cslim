#include "CppUTest/TestHarness.h"

extern "C"
{
	#include "TcpComLink.h"
}

TEST_GROUP(TcpComLink)
{
    TcpComLink* tcpComLink;

    void setup()
    {
      tcpComLink = TcpComLink_Create(0);
    }
    
    void teardown()
    {
       TcpComLink_Destroy(tcpComLink);
    }
};

TEST(TcpComLink, Create)
{
}

