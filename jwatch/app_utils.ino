

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

  const char * printer_state = doc["status"];
  const char * file_name = doc["job"]["file"]["name"];
  float completion = doc["progress"]["completion"];
  
  esp_log("  document parsed");

  esp_log(String("printer status is ") + printer_state);

  if (printer_state == "Printing") {
    printer_status = String("Printing: \"") + file_name + "\"\n" + String(completion, 1) + "% complete";
  } else if (printer_state == "Operational") {
    printer_status = String("Printer is idle.\nLast printed file: \"") + file_name + "\"";
  } else {
    printer_status = String("Printer in state \"") + printer_state;
  }
  
  esp_log("  state generated");

  client.stop();

  esp_log("  client closed");

  return printer_status;
}

void refresh_printer_status(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    lv_label_set_text(printer_status_label, refresh_printer_status().c_str());
  }
}
