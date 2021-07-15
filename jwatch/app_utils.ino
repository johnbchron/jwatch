

String refresh_printer_status() {
  esp_log("printer status requested:");
  
  if (!WiFi.isConnected()) {
    return "Not connected to WiFi";
  }

  if (WiFi.SSID() != "ORBI64") {
    return "Not connected to ORBI64 (" + WiFi.SSID() + ")";
  }

  esp_log("  connected to wifi");

  WiFiClient client;
  const int port = 80;
  const char * host = PRINTER_HOST;
  
  if (!client.connect(host, port)) {
    client.stop();
    return "Connection to \"" + String(host) + "\" failed";
  }

  esp_log("  client connected to host");

  String url = "/api/job";
  String request = "GET " + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "X-Api-Key: " + String(PRINTER_API_KEY) +
                   "\r\nConnection: close\r\n\r\n";
  client.print(request);

  esp_log("  request made");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > CONNECTION_TIMEOUT) {
      client.stop();
      return "Connection timed out";
    }
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    client.stop();
    return String(F("Unexpected response: ")) + status;
  }
  
  esp_log("  http status recieved");

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    client.stop();
    return F("Invalid response");
  }

  esp_log("  headers recieved");

  DynamicJsonDocument doc(1024);
  esp_log("  json document allocated");
  DeserializationError error = deserializeJson(doc, client);
  
  esp_log("  json client started");

  if (error) {
    client.stop();
    String error_string = "deserializing error: " + String(error.f_str());
    return error_string;
  }
  
  esp_log("  json deserialized");
  
  String printer_status = "";

  const char * printer_state = doc["state"];
  const char * file_name = doc["job"]["file"]["name"];
  float completion = doc["progress"]["completion"];
  int print_time_left = doc["progress"]["printTimeLeft"];
  
  esp_log("  document parsed");

  if (String(printer_state).compareTo("Printing") == 0) {
    printer_status = String("Printing: \"") + file_name + "\"\n" + String(completion, 1) + "% complete";
    if (print_time_left > 0) {
      if (print_time_left >= 60) {
        if (print_time_left >= 3600) {
          printer_status += "\nEstimated " + String((int)floor(print_time_left / 3600.0)) + "h " + String((int)floor((print_time_left % 3600) / 60.0)) + "m " + String(print_time_left % 60) + "s left";
        } else {
          printer_status += "\nEstimated " + String((int)floor(print_time_left / 60.0)) + "m " + String(print_time_left % 60) + "s left";
        }
      } else {
        printer_status += "\nEstimated " + String(print_time_left) + "s left";
      }
    }
  } else if (String(printer_state).compareTo("Operational") == 0) {
    printer_status = String("Printer is idle.\nLast printed file: \"") + file_name + "\"";
  } else {
    printer_status = String("Printer is in state \"") + printer_state + String('\"');
  }
  
  esp_log("  state generated");

  client.stop();

  esp_log("  client closed");

  return printer_status;
}

void refresh_printer_status(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    String printer_status = refresh_printer_status();
    Serial.print("printer status is \""); Serial.print(printer_status); Serial.println('\"');
//    esp_log("printer status is \"" + printer_status + "\"");    
    lv_label_set_text(printer_status_label, printer_status.c_str());
  }
}

String parse_printer_state_json(DynamicJsonDocument root){

  const char* printer_state_text = root["state"];
//  double printer_temperature_bed = root["temperature"]["bed"]["actual"];
//  double printer_temperature_bed_target = root["temperature"]["bed"]["target"];
//  double printer_temperature_tool0 = root["temperature"]["tool0"]["actual"];
//  double printer_temperature_tool0_target = root["temperature"]["tool0"]["target"];

//  boolean is_printing = root["state"]["flags"]["printing"];
//  boolean is_ready = root["state"]["flags"]["ready"];
//  boolean is_paused = root["state"]["flags"]["paused"];
//  boolean is_operational = root["state"]["flags"]["operational"];
//  boolean has_error = root["state"]["flags"]["error"];
  
//  Serial.print("printer_state_text: ");
//  Serial.println(printer_state_text);
//  
//  Serial.print("printer_temperature_bed: ");
//  Serial.println(printer_temperature_bed);
//  Serial.print("printer_temperature_bed_target: ");
//  Serial.println(printer_temperature_bed_target);
//  Serial.print("printer_temperature_tool0: ");
//  Serial.println(printer_temperature_tool0);
//  Serial.print("printer_temperature_tool0_target: ");
//  Serial.println(printer_temperature_tool0_target);
//
//  String status_text = "";
//  if(is_ready){
//    status_text = "PRINTER READY";
//  }
//  if(is_printing){
//    status_text = "PRINTING";
//  }
//  if(is_paused){
//    status_text = "PAUSED";
//  }
//  if(has_error){
//    status_text = "ERROR";
//  }

//  esp_log("status_text: " + status_text);
  return printer_state_text;
}
