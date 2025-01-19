#include "EEPROM.h"

class minis
{
public:
	#pragma pack(push,1)
	char text[100];
	int adr;
	#pragma pack(pop)

	minis(int set_adr);

	void set();
	void read();

	String toStr();
	void reset(String Data);
};

minis::minis(int set_adr)
{
	//wdt_reset();
	adr = set_adr;
}

String minis::toStr()
{
	//wdt_reset();
	return text;
}

void minis::set()
{
	//wdt_reset();
	EEPROM.put(adr, text);
}

void minis::reset(String Data)
{
	//wdt_reset();
	for (unsigned int i = 0; i < 100; i++)
	{
		text[i] = NULL;
		if (i < Data.length()) text[i] = Data[i];
	}
	set();
}


void minis::read()
{
	//wdt_reset();
	EEPROM.get(adr, text);
}

#pragma pack(push,1)
struct
{
	byte IP[4];
	byte MASK[4];
	byte GATE[4];
	byte FLAG;
	int TimeSET;
	int errTimeout;
} Memory;

minis PW(EEPROM_PASSWORD_ADDR);
minis Name(EEPROM_DEVICENAME_ADDR);
#pragma pack(pop)

void setIP()
{
	//wdt_reset();
	EEPROM.write(EEPROM_IP_ADDR, Memory.IP[0]);
	EEPROM.write(EEPROM_IP_ADDR + 1, Memory.IP[1]);
	EEPROM.write(EEPROM_IP_ADDR + 2, Memory.IP[2]);
	EEPROM.write(EEPROM_IP_ADDR + 3, Memory.IP[3]);
	EEPROM.commit();
}

void setMASK()
{
	//wdt_reset();
	EEPROM.write(EEPROM_MASK_ADDR, Memory.MASK[0]);
	EEPROM.write(EEPROM_MASK_ADDR + 1, Memory.MASK[1]);
	EEPROM.write(EEPROM_MASK_ADDR + 2, Memory.MASK[2]);
	EEPROM.write(EEPROM_MASK_ADDR + 3, Memory.MASK[3]);
	EEPROM.commit();
}

void setGATE()
{
	//wdt_reset();
	EEPROM.write(EEPROM_GATE_ADDR, Memory.GATE[0]);
	EEPROM.write(EEPROM_GATE_ADDR + 1, Memory.GATE[1]);
	EEPROM.write(EEPROM_GATE_ADDR + 2, Memory.GATE[2]);
	EEPROM.write(EEPROM_GATE_ADDR + 3, Memory.GATE[3]);
	EEPROM.commit();
}

void setTS()
{
	//wdt_reset();
	EEPROM.put(EEPROM_TIMESET_ADDR, Memory.TimeSET);
}

void setOther()
{
	//wdt_reset();
	EEPROM.put(EEPROM_ERRTIMEOUT_ADDR, Memory.errTimeout);
}



void readIP()
{
	//wdt_reset();
	Memory.IP[0] = EEPROM.read(EEPROM_IP_ADDR);
	Memory.IP[1] = EEPROM.read(EEPROM_IP_ADDR + 1);
	Memory.IP[2] = EEPROM.read(EEPROM_IP_ADDR + 2);
	Memory.IP[3] = EEPROM.read(EEPROM_IP_ADDR + 3);
}

void readMASK()
{
	//wdt_reset();
	Memory.MASK[0] = EEPROM.read(EEPROM_MASK_ADDR);
	Memory.MASK[1] = EEPROM.read(EEPROM_MASK_ADDR + 1);
	Memory.MASK[2] = EEPROM.read(EEPROM_MASK_ADDR + 2);
	Memory.MASK[3] = EEPROM.read(EEPROM_MASK_ADDR + 3);
}

void readGATE()
{
	//wdt_reset();
	Memory.GATE[0] = EEPROM.read(EEPROM_GATE_ADDR);
	Memory.GATE[1] = EEPROM.read(EEPROM_GATE_ADDR + 1);
	Memory.GATE[2] = EEPROM.read(EEPROM_GATE_ADDR + 2);
	Memory.GATE[3] = EEPROM.read(EEPROM_GATE_ADDR + 3);
}

void readTS()
{
	//wdt_reset();
	EEPROM.get(EEPROM_TIMESET_ADDR, Memory.TimeSET);
}

void readOther()
{
	//wdt_reset();
	EEPROM.get(EEPROM_ERRTIMEOUT_ADDR, Memory.errTimeout);
}

void resetMemory()
{
	//wdt_reset();
	Memory.IP[0] = NET_DEFAULT_IP_1;
	Memory.IP[1] = NET_DEFAULT_IP_2;
	Memory.IP[2] = NET_DEFAULT_IP_3;
	Memory.IP[3] = NET_DEFAULT_IP_4;

	Memory.MASK[0] = NET_DEFAULT_MASK_1;
	Memory.MASK[1] = NET_DEFAULT_MASK_2;
	Memory.MASK[2] = NET_DEFAULT_MASK_3;
	Memory.MASK[3] = NET_DEFAULT_MASK_4;

	Memory.GATE[0] = NET_DEFAULT_GATEWAY_1;
	Memory.GATE[1] = NET_DEFAULT_GATEWAY_2;
	Memory.GATE[2] = NET_DEFAULT_GATEWAY_3;
	Memory.GATE[3] = NET_DEFAULT_GATEWAY_4;

	PW.reset(DEVICE_PASS);
	Memory.TimeSET = -1;
	Memory.errTimeout = 15;

	Name.reset(DEVICE_NAME);

	EEPROM.write(0, 0);

	setIP();
	setMASK();
	setGATE();
	setTS();
	setOther();
}

bool isMemoryRaw(){
	//wdt_reset();
	Memory.FLAG = EEPROM.read(0);
	return Memory.FLAG != 0;
}

void readMemory()
{
	//wdt_reset();
	if (isMemoryRaw()) resetMemory();

	readIP();
	readMASK();
	readGATE();
	readTS();
	readOther();

	Name.read();
	PW.read();
}


