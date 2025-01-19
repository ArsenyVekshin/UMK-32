void loop(void) {

    sensor_loop();
    wdt_reset();

    WebServer();
	wdt_reset();
	Timer();
}