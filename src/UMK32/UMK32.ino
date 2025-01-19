#include <esp_task_wdt.h>
#include <SPI.h>
#include <KTime.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "configuration/Configuration.h"

#define wdt_reset() esp_task_wdt_reset()

#pragma pack(push,1)
byte mac[] = DEVICE_MAC;

IPAddress ip(NET_DEFAULT_IP_1, NET_DEFAULT_IP_2, NET_DEFAULT_IP_3, NET_DEFAULT_IP_4);
IPAddress mask(NET_DEFAULT_MASK_1, NET_DEFAULT_MASK_2, NET_DEFAULT_MASK_3, NET_DEFAULT_MASK_4);
IPAddress gate(NET_DEFAULT_GATEWAY_1, NET_DEFAULT_GATEWAY_2, NET_DEFAULT_GATEWAY_3, NET_DEFAULT_GATEWAY_4);

EthernetServer Telnet(23);
EthernetServer Web(80);

String command = "";
String WebData = "";

EthernetClient client;
EthernetClient clientTelnet;

boolean login = false;

KTime centralTime = KTime();
#pragma pack(pop)