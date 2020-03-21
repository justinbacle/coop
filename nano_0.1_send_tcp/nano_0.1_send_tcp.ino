#include <UIPEthernet.h>

EthernetServer server = EthernetServer(1000);
EthernetClient client;

void setup() {
  Serial.begin(9600);
  uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  IPAddress myIP(192, 168, 1, 111);
  Ethernet.begin(mac, myIP);
}

void loop() {
  if (client.connect(IPAddress(192, 168, 1, 100), 40000))
  {
    client.println(F("Ttest:23c"));
    int size;
    while ((size = client.available()) > 0)
    {
      uint8_t* msg = (uint8_t*)malloc(size + 1);
      memset(msg, 0, size + 1);
      size = client.read(msg, size);
      free(msg);
    }
    //disconnect client
    client.flush();
    client.stop();
  }
}
