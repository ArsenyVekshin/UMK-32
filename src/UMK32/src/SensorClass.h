#pragma once
#include <Arduino.h>
#include <KTime.h>
#include "EEPROM.h"
#include <OneWire.h>
#include "configuration/Configuration.h"


// максимальная длинна имени датчика
constexpr byte sens_name_length = 10;

class Sensor
{
public:
	Sensor(byte num = 1, KTime *timer = nullptr);

	boolean IsNull() const;

	boolean IsIdent(const byte arr2[8]) const;

	boolean IsVirtual() const;

	boolean CheckNewVal(float newT = 0) const;

	void SetAddr(const byte arr[8]);

	void SetName(String name);

	void SetNum(byte num);

	void SetStatus(byte status);

	void SetStatus(String status);

	void SetTemp(float temp);

	void SetMinTemp(int minTemp);

	void SetMaxTemp(int maxTemp);

	void SetWarnTime(KTime warnTime);

	void SetDefault(byte num, KTime *timer);

	byte *GetAddr();

	String GetTextAddr() const;

	/// <summary>
	/// Sensor's name help's to identify sensor by human.
	/// </summary>
	/// <returns>Returns sensor's name.</returns>
	String GetName() const;

	/// <summary>
	/// Sensor's number represents number in parents array.
	/// </summary>
	/// <returns>Returns sensor's number.</returns>
	byte GetNum() const;

	/// <summary>
	/// Sensor's status X(broken/unpluged) - 0, off - 1, on - 2
	/// </summary>
	/// <returns>byte(0), byte(1) or byte(2)</returns>
	byte GetStatus() const;
	String GetStatusCode() const;
	float GetTemp() const;
	int GetMinTemp() const;
	int GetMaxTemp() const;
	String ToJSON(byte id = 0) const;
	String ToString(byte id = 0) const;
	KTime *GetWarnTime();
	static int GetStructSize();
	static float readCurrSensor(OneWire &wire, const byte _addr[8]);

	void CheckWarn();
	void FixName();
	void FixMinMax();
	void Save(int startPoint) const;
	void Load(int startPoint);

private:
	#pragma pack(push,1)
	static constexpr byte NONE { '0' };
	KTime *timer;

	struct data
	{
		data(KTime *timer) :
			num(0), status(0),
			minTemp(0), maxTemp(0),
			warnTime(timer), temp(0) {}

		byte num;
		byte status; //2 - on, 1 - off, 0 - NONE, >3 - ошибка считывания N-3
		int minTemp, maxTemp;
		KTime warnTime;
		float temp;
		byte addr[8]{ NONE };
		String name;
	} sensor;
	#pragma pack(pop)
};