#include "configuration/Configuration.h"

void setNetConstants(){
    ip[0] = Memory.IP[0];
	ip[1] = Memory.IP[1];
	ip[2] = Memory.IP[2];
	ip[3] = Memory.IP[3];

	mask[0] = Memory.MASK[0];
	mask[1] = Memory.MASK[1];
	mask[2] = Memory.MASK[2];
	mask[3] = Memory.MASK[3];

	gate[0] = Memory.GATE[0];
	gate[1] = Memory.GATE[1];
	gate[2] = Memory.GATE[2];
	gate[3] = Memory.GATE[3];
}

void ethernetShieldCheck() {
    Serial.print("Ethernet check: ");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        Serial.println("Ethernet shield was not found!");
        sys_exit();
    }

    if (Ethernet.linkStatus() == LinkOFF) {
        Serial.println("Ethernet cable is not connected.");
    }
    else {
        Serial.println(F("OK"));
    }
}

void setup(void) {
    // esp_task_wdt_config_t wdt_config = {
    //     .timeout_ms = WDT_TIMEOUT * 1000,                 // Convertin ms
    //     .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,  // Bitmask of all cores
    //     .trigger_panic = true                             // Enable panic to restart ESP32
    // };

    // esp_err_t ESP32_ERROR = esp_task_wdt_init(&wdt_config); 
    // esp_task_wdt_add(NULL);
	Serial.begin(115200);
	Serial.println("Starting");

    if(isMemoryRaw()) {
        Serial.print(F("EEPROM status: RAW. \n\tMemory markup:"));
        resetMemory();
        Serial.println(F("OK"));
    }

    Serial.print(F("EEPROM settings read: "));
    resetMemory();
    //readMemory();
    Serial.println(F("OK"));

    //setNetConstants();
    //wdt_reset();

	Ethernet.init(ETH_PIN_CS);
	Ethernet.begin(mac, ip, mask, gate);
    ethernetShieldCheck();

    Telnet.begin();
	Web.begin();

	client.setConnectionTimeout(5000);
	clientTelnet.setConnectionTimeout(5000);

	//wdt_reset();
    
    Serial.println(F("Net settings:"));
    Serial.println("\tip:\t" + Ethernet.localIP().toString());
    Serial.println("\tmask:\t" + Ethernet.subnetMask().toString());
    Serial.println("\tgate:\t" + Ethernet.gatewayIP().toString());


    Serial.print(F("Sensors settings read: "));
    sensor_setup();
    Serial.println(F("OK"));

}