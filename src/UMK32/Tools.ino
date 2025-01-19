#pragma once

void sys_exit() {
    Serial.println("Rebooting... Internal code error");
    while(1) {delay(100);};
}


int ResultIP[] = {0, 0, 0, 0};

void StringToIP(String Data)
{
  Data = Data + '.';
  int L = Data.length();

  int Car = Data.indexOf('=');
  int preCar = Data.indexOf('=');

  for (int i = 0;i <= 3;i++)
  {
  Car = Car + Data.substring(Car+1, L).indexOf('.') + 1;
  ResultIP[i] = Data.substring(preCar+1, Car).toInt();
  preCar = Car;
  }
}

void(*reset)(void) = 0;


String IPstring() {
	//wdt_reset();
	String out = "";
	out.concat(ip[0]);
	out.concat(".");
	out.concat(ip[1]);
	out.concat(".");
	out.concat(ip[2]);
	out.concat(".");
	out.concat(ip[3]);
	return out;
}

String IPmem() {
	//wdt_reset();
	String out = "";
	out.concat(Memory.IP[0]);
	out.concat(".");
	out.concat(Memory.IP[1]);
	out.concat(".");
	out.concat(Memory.IP[2]);
	out.concat(".");
	out.concat(Memory.IP[3]);
	return out;
}

String MASKmem() {
	//wdt_reset();
	String out = "";
	out.concat(Memory.MASK[0]);
	out.concat(".");
	out.concat(Memory.MASK[1]);
	out.concat(".");
	out.concat(Memory.MASK[2]);
	out.concat(".");
	out.concat(Memory.MASK[3]);
	return out;
}

String GATEmem() {
	//wdt_reset();
	String out = "";
	out.concat(Memory.GATE[0]);
	out.concat(".");
	out.concat(Memory.GATE[1]);
	out.concat(".");
	out.concat(Memory.GATE[2]);
	out.concat(".");
	out.concat(Memory.GATE[3]);
	return out;
}

int TIMEOUTmem() {
	//wdt_reset();
	return Memory.errTimeout;
}

String PWmem() {
	//wdt_reset();
	String out = "";
	//out.concat(Memory.PW);
	return out;
}

String TimeString(unsigned long T) {
	//wdt_reset();
	if (T != (unsigned int)-1) {
		String out = "";

		const unsigned long S = T;
		const unsigned long M = S / 60;
		const unsigned long H = M / 60;
		const unsigned long D = H / 24;
		const unsigned long Y = D / 365;

		out.concat(Y);
		out.concat(F(" year "));

		out.concat(D);
		out.concat(F(" day "));

		out.concat(H % 24);
		out.concat(F(":"));
		out.concat(M % 60);
		out.concat(F(":"));
		out.concat(S % 60);

		return out;
	}
	else {
		return F("not found");
	}
}

int inRange(int min, int Val, int max) {
	//wdt_reset();
	int res = 0;
	if ((Val > min) && (Val < max)) res = 1;
	return res;
}
