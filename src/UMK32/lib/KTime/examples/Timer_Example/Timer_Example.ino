#include <KTime.h>

KTime timer = KTime();

void setup()
{
	Serial.begin(9600);
	Serial.println("Testing KTime lib:");
}

void loop()
{
	Serial.println(timer.ToString());
	delay(1000);
	timer.AddTime(1, 0, 0, 0);
}