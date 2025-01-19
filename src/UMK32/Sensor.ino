#include <OneWire.h>
#include "EEPROM.h"
#include <SPI.h>
#include "configuration/Configuration.h"
#include <KTime.h>
#include "src/SensorClass.h"
#include "src/DeltaClass.h"

#define parts_num_sens ((calcActiveSensors()/PRINT_PART_SIZE) + (calcActiveSensors() % PRINT_PART_SIZE > 0 ? 1 : 0))
#define parts_num_deltas ((calcActiveDeltas()/PRINT_PART_SIZE) + (calcActiveDeltas() % PRINT_PART_SIZE > 0 ? 1 : 0))

#pragma pack(push,1)
unsigned int sensorsTimer = 0; // таймер секунд до повторного чтения

const int sensor_pins[SENS_PINS_NUM] = {32, 33};

Sensor sensors[SENSORS_MAX];
Delta deltas[DELTAS_MAX];
#pragma pack(pop)

#pragma region Датчики
int findSensorByAddr(byte addr[8]) {
	for (byte i = 0; i < SENSORS_MAX; i++) {
		if (sensors[i].IsIdent(addr)) return i;
	}
	return -1; 
}

int findNullSensor() {
	for (byte i = 0; i < SENSORS_MAX; i++) {
		if (sensors[i].IsNull()) return i;
	}
	return -1; 
}

void readSensors() {
	// устанавливаем все датчки в статус "сломан"
	for (byte i=0; i<SENS_PINS_NUM; i++) {
		if (sensors[i].GetStatus() == SENS_STATUS_OK) 
			sensors[i].SetStatus(SENS_STATUS_ERROR);
	}

	Serial.print("Sensors reading:");

	for (byte i=0; i<SENS_PINS_NUM; i++) {
		OneWire wire(sensor_pins[i]);
		//wdt_reset();

		for (byte j=0; j<SENSORS_MAX; j++) {\
			Serial.print(".");
			//wdt_reset();
			Timer();
			byte addr[8];

			if (!wire.search(addr)) {
				wire.reset_search();
				break;
			}

			//wdt_reset();
			float celsius = Sensor::readCurrSensor(wire, addr);
			if(celsius == SENS_ERR_VALUE) continue;

			int sensor_id = findSensorByAddr(addr);
			if (sensor_id == -1) {
				sensor_id = findNullSensor();
				if(sensor_id == -1) continue;
				sensors[sensor_id].SetDefault(sensor_id + 1, &centralTime);
				sensors[sensor_id].SetAddr(addr);
				sensors[sensor_id].SetTemp((celsius != 85.00) ? celsius : 0);
				sensors[sensor_id].SetStatus(SENS_STATUS_OK);
			}
			else {
				if (sensors[sensor_id].GetStatus() != SENS_STATUS_OFF) {
					sensors[sensor_id].SetStatus(SENS_STATUS_OK);
					}
				if (sensors[sensor_id].CheckNewVal(celsius)) {
					sensors[sensor_id].SetTemp(celsius);
					}
				
			}
		}
	}
	Serial.println("Done");
	//wdt_reset();
}

void updateWarnTimes() {
	//wdt_reset();
	for (byte i = 0; i < SENSORS_MAX; i++) sensors[i].CheckWarn();
	for (byte i = 0; i < DELTAS_MAX; i++) deltas[i].CheckWarn();
}

int calcActiveSensors() {
	//wdt_reset();
	int out = 0;
	for (byte i = 0; i < SENSORS_MAX; i++) {
		if (sensors[i].IsNull()) out++;
	}
	return out;
}

String getSensorsJSON(byte part = 0)
{
	//wdt_reset();
	String JSON = "[";
	part = part % parts_num_sens;
	bool isFirst = true;
	for (uint8_t i = part * PRINT_PART_SIZE ; i < (SENSORS_MAX < ((part + 1) * PRINT_PART_SIZE) ? SENSORS_MAX : ((part + 1) * PRINT_PART_SIZE)); i++)
	{
		// Проверяем на наличие датчика
		if (sensors[i].IsNull()) continue;

		// Не ставим запятую вначале
		if (isFirst) isFirst = false;
		else JSON += ",";

		// Добавляем датчик в JSON
		JSON += sensors[i].ToJSON(i + 1);
		//wdt_reset();
	}
	JSON += "]";
	return JSON;
}

#pragma endregion
#pragma region Дельты
int findNullDelta() {
	for (byte i = 0; i < DELTAS_MAX; i++) {
		if (deltas[i].IsNull()) return i;
	}
	return -1; 
}

void createDelta(int sens1, int sens2) {
	//wdt_reset();
	if(sens1 >= SENSORS_MAX || sensors[sens1].IsNull()) return;
	if(sens2 >= SENSORS_MAX || sensors[sens2].IsNull()) return;

	int delta_id = findNullDelta();
	if (delta_id == -1) return;

	//wdt_reset();
	deltas[delta_id].SetDefaults(delta_id + 1, &centralTime);
	deltas[delta_id].SetSens1(&sensors[sens1]);
	deltas[delta_id].SetSens2(&sensors[sens2]);

	SaveConfigs();
}

int calcActiveDeltas() {
	//wdt_reset();
	int out = 0;
	for (byte i = 0; i < DELTAS_MAX; i++) {
		if (deltas[i].IsNull()) out++;
	}
	return out;
}

String getDeltasJSON(byte part = 0)
{
	//wdt_reset();
	String JSON = "[";
	part = part % parts_num_deltas;
	bool isFirst = true;
	for (int i = part * PRINT_PART_SIZE; i < (DELTAS_MAX < ((part + 1) * PRINT_PART_SIZE) ? DELTAS_MAX : ((part + 1) * PRINT_PART_SIZE)); i++)
	{
		// Проверяем на наличие дельты
		if (deltas[i].IsNull()) continue;

		// Не ставим запятую вначале
		if (isFirst) isFirst = false;
		else JSON += ",";

		//Добавляем дельту в JSON
		JSON += deltas[i].ToJSON(i + 1);
		//wdt_reset();
	}
	JSON += "]";
	return JSON;
}

#pragma endregion

#pragma region EEPROM
void SaveConfigs() {
	//wdt_reset();
	unsigned int addr = EEPROM_SENSORS_START_ADDR;
	unsigned int structSize = Sensor::GetStructSize();
	for (byte i = 0; i < SENSORS_MAX; i++) {
		addr = EEPROM_SENSORS_START_ADDR + structSize * i;
		sensors[i].Save(addr);
	}
	//wdt_reset();

	addr = EEPROM_DELTAS_START_ADDR;
	structSize = Delta::GetStructSize();
	for (byte i = 0; i < DELTAS_MAX; i++) {
		addr = EEPROM_SENSORS_START_ADDR + structSize * i;
		deltas[i].Save(addr);
	}
	//wdt_reset();
}

void LoadConfigs() {
	//wdt_reset();
	unsigned int addr = EEPROM_SENSORS_START_ADDR;
	unsigned int structSize = Sensor::GetStructSize();
	for (byte i = 0; i < SENSORS_MAX; i++) {
		addr = EEPROM_SENSORS_START_ADDR + structSize * i;
		sensors[i].Load(addr);
	}

	//wdt_reset();
	addr = EEPROM_DELTAS_START_ADDR;
	structSize = Delta::GetStructSize();
	for (byte i = 0; i < DELTAS_MAX; i++) {
		addr = EEPROM_DELTAS_START_ADDR + structSize * i;
		byte num[2];
		deltas[i].Load(addr, num);
		deltas[i].SetSens1(num[0] ? &sensors[num[0]-1] : nullptr);
		deltas[i].SetSens2(num[1] ? &sensors[num[1]-1] : nullptr);
	}
	//wdt_reset();
}

void Reset_configs(String type = "all") {
	//wdt_reset();
	if (type == F("deltas") || type == F("all")) {
		for (byte i = 0; i < DELTAS_MAX; i++) {
			deltas[i].SetDefaults(i + 1, &centralTime);
		}
	}
	//wdt_reset();
	if (type == F("sens") || type == F("all")) {
		for (byte i = 0; i < SENSORS_MAX; i++) {
			sensors[i].SetDefault(i + 1, &centralTime);
		}
	}
	SaveConfigs();
}

// Для исправления возможных ошибок при импорте из памяти
void fixNamesAtributes() {
	for (byte i = 0; i < SENSORS_MAX; i++) {
		sensors[i].FixName();
	}
	//wdt_reset();
	for (byte i = 0; i < DELTAS_MAX; i++) {
		deltas[i].FixName();
	}
	//wdt_reset();
}

void fixMinMaxAtributes() {
	//wdt_reset();
	for (byte i = 0; i < SENSORS_MAX; i++) sensors[i].FixMinMax();
	for (byte i = 0; i < DELTAS_MAX; i++) deltas[i].FixMinMax();
}

#pragma endregion

void sensor_setup()
{
	for (byte i = 0; i < SENSORS_MAX; i++)
		sensors[i] = Sensor(i + 1, &centralTime);
	for (byte i = 0; i < DELTAS_MAX; i++) 
		deltas[i] = Delta(i + 1, &centralTime);
	//wdt_reset();

	LoadConfigs();
	fixNamesAtributes();
	fixMinMaxAtributes();
	//wdt_reset();
}

void sensor_loop()
{
	if (sensorsTimer >= SENS_READ_PERIOD) {
		sensorsTimer = 0;
		readSensors(); //Считываем все подключенные датчики
		//wdt_reset();

		for (byte i = 0; i < SENSORS_MAX; i++) {
			if(sensors[i].IsNull()) continue;
			Serial.print(sensors[i].GetNum());
			Serial.print("\t");
			Serial.print(sensors[i].GetTextAddr());
			Serial.print("\t");
			Serial.print(sensors[i].GetStatusCode());
			Serial.print("\t");
			Serial.println(sensors[i].GetTemp());
		}
	}
	//wdt_reset();
}
