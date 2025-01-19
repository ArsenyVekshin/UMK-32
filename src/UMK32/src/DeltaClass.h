#pragma once
#include <Arduino.h>
#include "SensorClass.h"
#include <KTime.h>
#include "EEPROM.h"

// максимальная длинна имени дельты  <255
constexpr byte delta_name_length = 10;

class Delta
{
public:
	Delta(byte num = 1, KTime *timer = nullptr);

	void SetName(String &name);
	void SetSens1(Sensor *sens1);
	void SetSens2(Sensor *sens2);
	void SetMinTemp(int minTemp);
	void SetMaxTemp(int maxTemp);
	void SetWarnTime(KTime warnTime);
	void SetDefaults(byte num, KTime *timer);

	boolean IsNull() const;

	String GetName() const;
	Sensor *GetSens1();
	Sensor *GetSens2();
	int GetMinTemp() const;
	int GetMaxTemp() const;
	KTime *GetWarnTime();
	float GetValue() const;
	static int GetStructSize();
	String ToJSON(byte id = 0) const;
	String ToString(byte id = 0) const;

	void SwapSens(Sensor *sens1, Sensor *sens2);
	void CheckWarn();
	void FixName();
	void FixMinMax();
	void Save(int startPoint) const;
	void Load(int startPoint, byte *num);

private:
	#pragma pack(push,1)
	KTime *timer;

	struct data
	{
		data(KTime *timer) : warnTime(timer) {}

		String name = "delta";
		Sensor *sens1 = nullptr, *sens2 = nullptr;
		int minTemp = 0, maxTemp = 0;
		KTime warnTime;
	} delta;
	#pragma pack(pop)
};