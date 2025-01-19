#include "DeltaClass.h"
#include <Arduino.h>

Delta::Delta(byte num, KTime *timer) : delta(timer)
{
	this->timer = timer;
	SetDefaults(num, timer);
}

void Delta::SetName(String &name)
{
	delta.name = name;
}

void Delta::SetSens1(Sensor *sens1)
{
	delta.sens1 = sens1;
}

void Delta::SetSens2(Sensor *sens2)
{
	delta.sens2 = sens2;
}

void Delta::SetMinTemp(int minTemp)
{
	delta.minTemp = minTemp;
}

void Delta::SetMaxTemp(int maxTemp)
{
	delta.maxTemp = maxTemp;
}

void Delta::SetWarnTime(KTime warnTime)
{
	delta.warnTime = warnTime;
}

void Delta::SetDefaults(byte num, KTime *timer)
{
	SetName("delta" + String(num)); // name
	SetSens1(nullptr);	// sens number 1
	SetSens2(nullptr);	// sens number 2
	SetMinTemp(10);	// minTemp
	SetMaxTemp(60);	// maxTemp
	SetWarnTime(KTime(timer)); // warnTime
}

boolean Delta::IsNull() const
{
	if (!delta.sens1 || !delta.sens2 || delta.name == "") return true;
	return false;
}

String Delta::GetName() const
{
	return delta.name;
}

Sensor *Delta::GetSens1()
{
	return delta.sens1;
}

Sensor *Delta::GetSens2()
{
	return delta.sens2;
}

int Delta::GetMinTemp() const
{
	return delta.minTemp;
}

int Delta::GetMaxTemp() const
{
	return delta.maxTemp;
}

KTime *Delta::GetWarnTime()
{
	return &delta.warnTime;
}

float Delta::GetValue() const
{
	if (IsNull()) return 0;

	if (delta.sens1->GetStatus() != 2 ||
		delta.sens2->GetStatus() != 2) return 0;

	const float t1 = delta.sens1->GetTemp();
	const float t2 = delta.sens2->GetTemp();

	return t1 - t2;
}

int Delta::GetStructSize()
{
	return sizeof(byte) * 2 +
		sizeof(int) * 2 +
		delta_name_length;
}

String Delta::ToJSON(byte id) const
{
	// Начинаем новую дельту
	String JSON = "{";

	// Порядковый номер
	JSON += "\"id\":\"" + String(id) + "\",";

	// Имя
	JSON += "\"n\":\"" +
		delta.name + "\",";

	// num1
	JSON += "\"s1\":\"" +
		String(delta.sens1->GetNum()) + "\",";

	// num2
	JSON += "\"s2\":\"" +
		String(delta.sens2->GetNum()) + "\",";

	// Минимум
	JSON += "\"min\":\"" +
		String(delta.minTemp) + "\",";

	// Максимум
	JSON += "\"max\":\"" +
		String(delta.maxTemp) + "\",";

	// Температура
	JSON += "\"t\":\"" +
		String(GetValue()) + "\",";

	// Время ошибки
	JSON += "\"e\":\"" +
		(delta.warnTime.Check() && !IsNull() ? delta.warnTime.ToString() : "Нет ошибки") + "\"}";
	return JSON;
}

String Delta::ToString(byte id) const
{
	String result = String(id) + ". " +
		delta.name + "  " +
		String(delta.sens1->GetNum()) + " - " +
		String(delta.sens2->GetNum()) + " = " +
		String(GetValue()) + " | min: " +
		String(delta.minTemp) + " max: " +
		String(delta.maxTemp);
	if (GetValue() < delta.minTemp
		|| GetValue() > delta.maxTemp)
		result += " -WARNING";
	else result += " -OK";
	return result;
}

void Delta::SwapSens(Sensor *sens1, Sensor *sens2)
{
	if (delta.sens1 == sens1) delta.sens1 = sens2;
	else if (delta.sens1 == sens2) delta.sens1 = sens1;

	if (delta.sens2 == sens1) delta.sens2 = sens2;
	else if (delta.sens2 == sens2) delta.sens2 = sens1;
}

void Delta::CheckWarn()
{
	if (IsNull()) return;
	if (GetValue() <= static_cast<float>(delta.minTemp)
			 || GetValue() >= static_cast<float>(delta.maxTemp)
			 || delta.sens1->GetStatus() == 0
			 || delta.sens2->GetStatus() == 0)
	{
		if (!delta.warnTime.Check())
			delta.warnTime.Set();
	}
	else if (delta.warnTime.Check())
		delta.warnTime.Zero();
}

void Delta::FixName()
{
	if (delta.name.length() == 0) return;
	for (byte i = delta.name.length() - 1; i > 0; i--)
	{
		if (static_cast<char>(delta.name[i]) <= 32 || static_cast<char>(delta.name[i]) > 125)
			delta.name = delta.name.substring(0, i);
	}
	if (static_cast<char>(delta.name[0]) <= 32 || static_cast<char>(delta.name[0]) > 125) delta.name = "";
}

void Delta::FixMinMax()
{
	if (delta.minTemp > delta.maxTemp)
	{
		const int tmp = delta.maxTemp;
		delta.maxTemp = delta.minTemp;
		delta.minTemp = tmp;
	}
}

void Delta::Save(int startPoint) const
{
	char _name[delta_name_length];
	const char *tmp1 = delta.name.c_str();
	for (byte i = 0; i < delta_name_length; i++)
	{
		if (i < delta.name.length())
			_name[i] = tmp1[i];
		else _name[i] = ' ';
	}
	EEPROM.put(startPoint, _name);
	startPoint += delta_name_length;

	byte tmp2 = delta.sens1 ? delta.sens1->GetNum() : 0;
	EEPROM.put(startPoint, tmp2);
	startPoint += 1;

	tmp2 = delta.sens2 ? delta.sens2->GetNum() : 0;
	EEPROM.put(startPoint, tmp2);
	startPoint += 1;

	EEPROM.put(startPoint, delta.minTemp);
	startPoint += 2;

	EEPROM.put(startPoint, delta.maxTemp);
	startPoint += 2;
}

void Delta::Load(int startPoint, byte *num)
{
	char _dName[delta_name_length];
	EEPROM.get(startPoint, _dName);
	delta.name = String(_dName);
	delta.name.trim();
	startPoint += delta_name_length; // sizeof(delta.name)

	EEPROM.get(startPoint, num[0]);
	startPoint += 1; 

	EEPROM.get(startPoint, num[1]);
	startPoint += 1;

	EEPROM.get(startPoint, delta.minTemp);
	startPoint += 2;

	EEPROM.get(startPoint, delta.maxTemp);
	startPoint += 2;
}
