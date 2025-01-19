
constexpr unsigned long millisToSecRatio = 1000;

unsigned long tmpTime = 0;
void Timer() {
	wdt_reset();
	if (tmpTime > millis() / millisToSecRatio)
		tmpTime = 0;
	else { 
        centralTime.AddTime(1);
		tmpTime += 1;
		sensorsTimer += 1;
	}
}