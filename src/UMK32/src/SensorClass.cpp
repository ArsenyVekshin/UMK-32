#include "SensorClass.h"

Sensor::Sensor(byte num, KTime *timer) : sensor(timer)
{
	this->timer = timer;
	SetDefault(num, timer);
}

float Sensor::readCurrSensor(OneWire &wire, const byte _addr[8]) {
	byte present = 0;
	byte type_s;
	byte data[9];

	// the first ROM byte indicates which chip
	switch (_addr[0]) {
		case 0x10:	// old DS1820
			type_s = 1;
			break;
		case 0x28:	// DS1820
			type_s = 0;
			break;
		case 0x22:	// DS18B22
			type_s = 0;
			break;
		default:
			Serial.println(F("Device is not a DS18x20 family device."));
			return SENS_ERR_VALUE;
	}

	wire.select(_addr);

	wire.write(0x44, 1);	// start conversion, with parasite power on at the end
	// delay(SENS_READ_DELAY);

	present = wire.reset();
	wire.select(_addr);
	wire.write(0xBE); // Read Scratchpad

	for (byte i = 0; i < 12; i++) {
		data[i] = wire.read();
	}

	int16_t raw = (data[1] << 8) | data[0];
	if (type_s) {
		raw = raw << 3; // 9 bit resolution default
		if (data[7] == 0x10) {
			raw = (raw & 0xFFF0) + 12 - data[6];
		}
	}
	else {
		const byte cfg = (data[4] & 0x60);

		if (cfg == 0x00) raw = raw & ~7;
		else if (cfg == 0x20) raw = raw & ~3;
		else if (cfg == 0x40) raw = raw & ~1;
	}
	return static_cast<float>(raw) / 16.0;
}

boolean Sensor::IsNull() const
{
	if (sensor.addr == nullptr) return true;
	const byte tmp[8] { NONE };
	return IsIdent(tmp);
}

boolean Sensor::IsIdent(const byte arr2[8]) const
{
	for (byte i = 0; i < 8; i++)
		if (sensor.addr[i] != arr2[i])
			return false;
	return true;
}

boolean Sensor::IsVirtual() const
{
	if (IsNull()) return false;

	for (byte i = 1; i < 8; i++)
		if (sensor.addr[0] != sensor.addr[i]) return false;

	return true;
}

//проверяет коректность нового считывания, true = все ок
boolean Sensor::CheckNewVal(float newT) const
{
	if (newT == 85 || newT == 0 || sensor.temp / newT > 6) return false;
	return true;
}

void Sensor::SetAddr(const byte arr[8])
{
	for (byte i = 0; i < 8; i++) sensor.addr[i] = arr[i];
}

void Sensor::SetName(String name)
{
	sensor.name = name;
}

void Sensor::SetNum(byte num)
{
	sensor.num = num;
}

void Sensor::SetStatus(byte status)
{
	sensor.status = status;
}

void Sensor::SetStatus(String status)
{
	if (status == "on") sensor.status = 2;
	else if (status == "off") sensor.status = 1;
	else sensor.status = 0;
}

void Sensor::SetTemp(float temp)
{
	sensor.temp = temp;
}

void Sensor::SetMinTemp(int minTemp)
{
	sensor.minTemp = minTemp;
}

void Sensor::SetMaxTemp(int maxTemp)
{
	sensor.maxTemp = maxTemp;
}

void Sensor::SetWarnTime(KTime warnTime)
{
	sensor.warnTime = warnTime;
}

void Sensor::SetDefault(byte num, KTime *timer)
{
	const byte tmp[8] { NONE };
	SetAddr(tmp); // addr
	SetName("sens" + String(num)); //name
	SetNum(num);
	SetStatus(2); //status "on"
	SetMinTemp(10); //max temp
	SetMaxTemp(60); //min temp
	SetWarnTime(KTime(timer)); //warnTime
}

byte *Sensor::GetAddr()
{
	return sensor.addr;
}

void Sensor::CheckWarn()
{
	if (IsNull()) return;
	if ((sensor.temp <= static_cast<float>(sensor.minTemp)
		 || sensor.temp >= static_cast<float>(sensor.maxTemp)
		 || sensor.status == 0
		 || sensor.status >= 3)
		&& sensor.status != 1)
	{
		if (!sensor.warnTime.Check())
			sensor.warnTime.Set();
	}
	else if (sensor.warnTime.Check())
		sensor.warnTime.Zero();

	if (sensor.status >= 43) SetDefault(sensor.num, nullptr);
}

void Sensor::FixName()
{
	if (sensor.name.length() == 0) return;
	for (byte i = sensor.name.length() - 1; i > 0; i--)
	{
		if (static_cast<char>(sensor.name[i]) <= 32 || static_cast<char>(sensor.name[i]) > 125)
			sensor.name = sensor.name.substring(0, i);
	}
	if (static_cast<char>(sensor.name[0]) <= 32 || static_cast<char>(sensor.name[0]) > 125) sensor.name = "";
}

void Sensor::FixMinMax()
{
	if (sensor.minTemp > sensor.maxTemp)
	{
		const int tmp = sensor.maxTemp;
		sensor.maxTemp = sensor.minTemp;
		sensor.minTemp = tmp;
	}
}

void Sensor::Save(int startPoint) const
{
	EEPROM.put(startPoint, sensor.status);
	startPoint += sizeof(byte);

	EEPROM.put(startPoint, sensor.minTemp);
	startPoint += sizeof(int);

	EEPROM.put(startPoint, sensor.maxTemp);
	startPoint += sizeof(int);

	EEPROM.put(startPoint, sensor.addr);
	startPoint += 8 * sizeof(byte);

	char _name[sens_name_length] = { ' ' };
	for (byte i = 0; i < sensor.name.length(); i++)
		_name[i] = sensor.name[i];
	EEPROM.put(startPoint, _name);
}

void Sensor::Load(int startPoint)
{
	EEPROM.get(startPoint, sensor.status);
	startPoint += sizeof(byte);

	EEPROM.get(startPoint, sensor.minTemp);
	startPoint += sizeof(int);

	EEPROM.get(startPoint, sensor.maxTemp);
	startPoint += sizeof(int);

	EEPROM.get(startPoint, sensor.addr);
	startPoint += 8 * sizeof(byte);

	char _name[sens_name_length];
	EEPROM.get(startPoint, _name);
	sensor.name = String(_name);
	sensor.name.trim();
}

String Sensor::GetTextAddr() const
{
	String out = "";
	if (IsVirtual())
	{
		out += "virtual";
		out += String(sensor.addr[0]);
	}
	else
	{
		for (byte i = 0; i < 8; i++)
		{
			out += String(sensor.addr[i], HEX);
		}
	}
	return out;
}

String Sensor::GetName() const
{
	return sensor.name;
}

byte Sensor::GetNum() const
{
	return sensor.num;
}

byte Sensor::GetStatus() const
{
	return sensor.status;
}

String Sensor::GetStatusCode() const
{
	if (sensor.status == 2) return "on";
	if (sensor.status == 1) return "off";
	return "X";
}

float Sensor::GetTemp() const
{
	return sensor.temp;
}

int Sensor::GetMinTemp() const
{
	return sensor.minTemp;
}

int Sensor::GetMaxTemp() const
{
	return sensor.maxTemp;
}

String Sensor::ToJSON(byte id) const
{
	String JSON = "{";

	// порядковый номер
	JSON += "\"id\":\"" + String(id) + "\",";

	// адрес
	JSON += "\"a\":\"" +
		GetTextAddr() + "\",";

	// имя
	JSON += "\"n\":\"" +
		sensor.name + "\",";

	// статус
	JSON += "\"s\":\"" +
		GetStatusCode() + "\",";

	// минимум
	JSON += "\"min\":\"" +
		String(sensor.minTemp) + "\",";

	// максимум
	JSON += "\"max\":\"" +
		String(sensor.maxTemp) + "\",";

	// температура
	if (sensor.status == 2)
		JSON += "\"t\":\"" +
		String(sensor.temp) + "\",";
	else JSON += "\"t\":\"- - -\",";

	// Время ошибки
	JSON += "\"e\":\"" +
		(sensor.warnTime.Check() ? sensor.warnTime.ToString() : "Нет ошибки") + "\"}";

	return JSON;
}

KTime *Sensor::GetWarnTime()
{
	return &sensor.warnTime;
}

int Sensor::GetStructSize()
{
	return 1 + //sensor.status
		2 + //sensor.minTemp
		2 + //sensor.maxTemp
		8 + //sensor.addr
		sens_name_length; //sensor.name
}


