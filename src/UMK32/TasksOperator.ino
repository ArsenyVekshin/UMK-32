
bool validateSensorId(String arg) {
    return (arg.toInt() <= SENSORS_MAX && arg.toInt() > 0);
}

bool validateDeltaId(String arg) {
    return (arg.toInt() <= DELTAS_MAX && arg.toInt() > 0);
}

bool isSensorExist(String arg) {
    return validateSensorId(arg) && !sensors[arg.toInt()].IsNull();
}

bool validateStatus(String arg) {
    return arg == "on" || arg=="off";
}

void doOperation(String cmd = "", String arg1 = "",
                 String arg2="", String arg3="") {
    
    if (String(cmd) == F("sensor_del")) {
        if(validateSensorId(arg1)) {
            sensors[arg1.toInt() - 1].SetDefault(arg1.toInt(), &centralTime);
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("sensor_del_all")) {
        if(validateSensorId(arg1)) {
            Reset_configs("sens");
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("sensor_name")) {
        if(isSensorExist(arg1) && arg2!="") {
            if(arg2.length() > NAMES_MAX_LEN) arg2.substring(0, NAMES_MAX_LEN);
            sensors[arg1.toInt() - 1].SetName(arg2);
		    sensors[arg1.toInt() - 1].FixName();
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("sensor_status")) {
        if(isSensorExist(arg1) && validateStatus(arg2)) {
            sensors[arg1.toInt() - 1].SetStatus(arg2);
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("sensor_min")) {
        if(isSensorExist(arg1)) {
            sensors[arg1.toInt() - 1].SetMinTemp(arg2.toInt());
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("sensor_max")) {
        if(isSensorExist(arg1)) {
            sensors[arg1.toInt() - 1].SetMaxTemp(arg2.toInt());
            SaveConfigs();
        }
        return;
    }

    if (String(cmd) == F("delta_del")) {
        if(validateDeltaId(arg1)) {
            deltas[arg1.toInt() - 1].SetDefault(arg1.toInt(), &centralTime);
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("delta_new")) {
        if(validateSensorId(arg1) && validateSensorId(arg2)) {
            createDelta(arg1.toInt(), arg2.toInt());
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("delta_name")) {
        if(validateDeltaId(arg1) && arg2!="") {
            if(arg2.length() > NAMES_MAX_LEN) arg2.substring(0, NAMES_MAX_LEN);
            deltas[arg1.toInt() - 1].SetName(arg2);
		    deltas[arg1.toInt() - 1].FixName();
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("delta_min")) {
        if(validateDeltaId(arg1)) {
            deltas[arg1.toInt() - 1].SetMinTemp(arg2.toInt());
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("delta_max")) {
        if(validateDeltaId(arg1)) {
            deltas[arg1.toInt() - 1].SetMaxTemp(arg2.toInt());
            SaveConfigs();
        }
        return;
    }
    if (String(cmd) == F("delta_redact")) {
		if(validateDeltaId(arg1) && validateSensorId(arg2) && validateSensorId(arg3)) {
            deltas[arg1.toInt() - 1].SetSens1(&sensors[arg2.toInt() - 1]);
            deltas[arg1.toInt() - 1].SetSens2(&sensors[arg3.toInt() - 1]);
            SaveConfigs();
        }
        return;
	}
    if (String(cmd) == F("delta_s1")) {
		if(validateDeltaId(arg1) && validateSensorId(arg2)) {
            deltas[arg1.toInt() - 1].SetSens1(&sensors[arg2.toInt() - 1]);
            SaveConfigs();
        }
        return;
	}
    if (String(cmd) == F("delta_s2")) {
		if(validateDeltaId(arg1) && validateSensorId(arg2)) {
            deltas[arg1.toInt() - 1].SetSens2(&sensors[arg2.toInt() - 1]);
            SaveConfigs();
        }
        return;
	}

    if (String(cmd) == F("get_temp")) {
		readSensors();
        updateWarnTimes();
        return;
	}

}