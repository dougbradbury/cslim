#include <SPI.h>

#include <Client.h>
#include <Ethernet.h>
#include <Server.h>
#include <Udp.h>
#include "TcpComLink.h"

extern "C" {
  #include "Slim.h"
}

Slim * slim;
Server server(22);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,1, 177 };
byte gateway[] = { 192,168,1, 1 };
byte subnet[] = { 255, 255, 255, 0 };

void setup()
{
  slim = Slim_Create();

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  Serial.begin(9600);
  Serial.write("Slim running\n");

}


void loop_ethernet()
{
  Client client = server.available();
  if (client) {
    Serial.write("New Connection\n");
    TcpComLink * comLink = TcpComLink_Create(&client);
    Slim_HandleConnection(slim, (void*)comLink, &TcpComLink_send, &TcpComLink_recv);
    TcpComLink_Destroy(comLink);
  }
}

#include "SerialComLink.h"
void loop_serial()
{
  Slim_HandleConnection(slim, (void*)0, &SerialComLink_send, &SerialComLink_recv);
}

void loop()
{
  loop_ethernet();
}

