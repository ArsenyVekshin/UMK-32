
constexpr unsigned int WEB_DATA_LIMIT = 250;

void WebServer()
{
	//wdt_reset();
	client = Web.available();
	//FIXME Добавь защиту!!!

	if (client)
	{
		while (client.connected())
		{
			if (client.available())
			{

				WebData = "";
				while (client.available())
				{
					const int newChar = client.read();
					if (newChar > 0) WebData += static_cast<char>(newChar);
					if (WebData.length() > WEB_DATA_LIMIT && WEB_DATA_LIMIT > 0) break;
				}

				String getRequest =
					WebData.substring(WebData.indexOf(F("GET ")) + 4);
				getRequest =
					getRequest.substring(0, getRequest.indexOf(" "));

				client.flush();

				//if (getRequest.startsWith(F("/favicon.ico")))
				//	ImageLOGO(); else
				if (getRequest.startsWith(F("/files")))
				{
					if (getRequest.startsWith(F("/files/sens.json")))
						SnmpSensorsJSON();
					else if (getRequest.startsWith(
						F("/files/deltas.json"))) SnmpDeltasJSON();
					else if (getRequest.startsWith(
						F("/files/log.json"))) LogJSON();
				}
				else
				{
					HTTP();

					if (getRequest.startsWith(F("/favicon")))
						client.println(F("ok"));
					else if (getRequest.startsWith(F("/login")))
						LoginJS();
					else if (getRequest.startsWith(F("/checkpass=")))
						CheckPassJS();
					else if (getRequest.startsWith(F("/main")))
					{
						if (login) MainJS();
						else toLoginJS();
					}
					else if (getRequest.startsWith("/pass=" +
												   PW.toStr() + "/sensor")) SensorJS();
					// else if (getRequest.startsWith("/pass=" + PW.toStr() + "/command"))
					// 	CommandJS();
					else if (getRequest.startsWith("/pass=" +
												   PW.toStr() + "/net")) NetJS();
					else if (getRequest.startsWith("/pass=" +
												   PW.toStr() + "/fall")) FallJS();
					else if (getRequest.startsWith("/pass=" +
												   PW.toStr() + "/reset")) ResetJS();
					else toLoginJS();
				}
				break;
			}
		}
	}
	WebData = "";
	client.stop();
}

String CutString(String SData, String Begin, int x)
{
	//wdt_reset();
	SData = SData.substring(SData.indexOf(Begin), SData.length());
	SData = SData.substring(SData.indexOf("=") + x,
							SData.indexOf("&"));

	for (unsigned int i = 0; i < SData.length(); i++)
	{
		if (SData[i] == '+') SData[i] = ' ';
	}
	return SData;
}

void HTTP()
{
	//wdt_reset();
	client.println(F("HTTP/1.1 200 OK"));
	client.println(F("Content-Type: text/html"));
	client.println(F("Connection: close"));
	client.println("");
}

void CheckPassJS()
{
	//wdt_reset();
	const String getPW = CutString(WebData, F("/checkpass="), 1);
	if (getPW == PW.toStr())
	{
		client.println(F("ok"));
		login = true;
	}
	else
	{
		client.println(F("error"));
		//login = false;
	}
}

void SensorJS()
{
	//wdt_reset();
	const int spart = CutString(WebData, F("spart="), 1).toInt();
	const int dpart = CutString(WebData, F("dpart="), 1).toInt();
	client.print("WT=Time: " + centralTime.ToString());
	client.print(F("&sensors="));
	client.print(getSensorsJSON(spart));
	client.print(F("&deltas="));
	client.print(getDeltasJSON(dpart));
	client.print("&spart=" + String(spart + 1 > parts_num_sens ? 1 : spart + 1));
	client.print("&dpart=" + String(dpart + 1 > parts_num_deltas ? 1 : dpart + 1));
	client.print("&psize=" + String(PRINT_PART_SIZE) + "&");
}

void CommandJS()
{
	//wdt_reset();

	// DEBUG_PRINTLN(CutString(WebData, F("command="), 1) + " " +
	// 			  CutString(WebData, F("value1="), 1) + " " +
	// 			  CutString(WebData, F("value2="), 1) + " " +
	// 			  CutString(WebData, F("value3="), 1), 7);



	doOperation(CutString(WebData, F("command="), 1),
				CutString(WebData, F("value1="), 1),
				CutString(WebData, F("value2="), 1),
				CutString(WebData, F("value3="), 1));
}

void toLoginJS()
{
	//wdt_reset();
	client.print(F(
		"<!DOCTYPE html>"
		"<html>"
			"<script>"));
				client.println("location.href = 'http://" + IPmem() + "/login'");
			client.println(F(
			"</script>"
		"</html>"));
}

void ResetJS()
{
	//wdt_reset();
	client.println(F("ok"));
	delay(1000);
}

void FallJS()
{
	//wdt_reset();
	resetMemory();

	client.println("ip=" + IPmem() + "&");
	client.println("mask=" + MASKmem() + "&");
	client.println("gate=" + GATEmem() + "&");
	client.println("newpass=" + PW.toStr() + "&");
	client.println("name=" + Name.toStr() + "&");
}

void NetJS()
{
	//wdt_reset();
	StringToIP(CutString(WebData, F("ip="), 0));
	if (ResultIP[0] != 0)
	{
		Memory.IP[0] = ResultIP[0];
		Memory.IP[1] = ResultIP[1];
		Memory.IP[2] = ResultIP[2];
		Memory.IP[3] = ResultIP[3];
		setIP();
	}

	StringToIP(CutString(WebData, F("mask="), 0));
	if (ResultIP[0] != 0)
	{
		Memory.MASK[0] = ResultIP[0];
		Memory.MASK[1] = ResultIP[1];
		Memory.MASK[2] = ResultIP[2];
		Memory.MASK[3] = ResultIP[3];
		setMASK();
	}

	StringToIP(CutString(WebData, F("gate="), 0));
	if (ResultIP[0] != 0)
	{
		Memory.GATE[0] = ResultIP[0];
		Memory.GATE[1] = ResultIP[1];
		Memory.GATE[2] = ResultIP[2];
		Memory.GATE[3] = ResultIP[3];
		setGATE();
	}

	Memory.errTimeout = CutString(WebData, F("timeout="), 1).toInt();
	setOther();

	PW.reset(CutString(WebData, F("newpass="), 1));
	Name.reset(CutString(WebData, F("name="), 1));

	client.print("ip=" + IPmem() + "&");
	client.print("mask=" + MASKmem() + "&");
	client.print("gate=" + GATEmem() + "&");
	client.print("newpass=" + PW.toStr() + "&");
	client.print("name=" + Name.toStr() + "&");
	client.print("timeout=" + String(TIMEOUTmem()) + "&");
}

void SnmpSensorsJSON()
{
	//wdt_reset();
	client.println(F("HTTP/1.1 200 OK"));
	client.println(F("Content-Type: application/json"));
	client.println(F("Connection: close"));
	client.println("");
	client.print("{\"data\":[");
	bool flag = false;
	for (byte i = 0; i < SENSORS_MAX; i++)
	{
		//wdt_reset();
		if (sensors[i].IsNull()) continue;

		if (!flag) flag = true;
		else client.print(",");

		client.print("{");
		client.print("\"{#SENSNUM}\":\"" + String(sensors[i].GetNum()));
		client.print("\",\"{#SENSNAME}\":\"" + sensors[i].GetName());
		client.print("\",\"{#DEVICE}\":\"" + String(Name.text));
		client.print("\"}");
	}
	client.print("]}");
}

void SnmpDeltasJSON()
{
	//wdt_reset();
	client.println(F("HTTP/1.1 200 OK"));
	client.println(F("Content-Type: application/json"));
	client.println(F("Connection: close"));
	client.println("");
	client.print("{\"data\":[");
	bool flag = false;
	for (byte i = 0; i < DELTAS_MAX; i++)
	{
		//wdt_reset();
		if (deltas[i].IsNull()) continue;

		if (!flag) flag = true;
		else client.print(",");

		client.print("{");
		client.print("\"{#DELTAID}\":\"" + String(i + 1));
		client.print("\",\"{#DELTANAME}\":\"" + deltas[i].GetName());
		client.print("\",\"{#DEVICE}\":\"" + String(Name.text));
		client.print("\"}");
	}
	client.print("]}");
}

void LogJSON()
{
	//wdt_reset();
	client.println(F("HTTP/1.1 200 OK"));
	client.println(F("Content-Type: application/json"));
	client.println(F("Connection: close"));
	client.println("");
	client.print("{\"data\":[");

	client.print("{");
	client.print("\"{#DEVICE}\":\"" + String(Name.text));

	client.print("\",\"{#MAC}\":\"" + String(mac[0], HEX) + "-" + String(mac[1], HEX) + "-" + String(mac[2], HEX) + "-" + String(mac[3], HEX) + "-" + String(mac[4], HEX) + "-" + String(mac[5], HEX));
	client.print("\",\"{#IP}\":\"" + String(IPmem()));
	client.print("\",\"{#MASK}\":\"" + String(MASKmem()));
	client.print("\",\"{#GATE}\":\"" + String(GATEmem()));

	client.print("\",\"{#ERRORS}\":\"" + String(F("NONE"))); //point for diagnostic-lib printing
	client.print("\"}");
	
	client.print("]}");
}
