#include <Arduino.h>
#include "serial_config.h"
#include "settings.h"

// ============================================================
// SERIAL CONFIGURATION
// ============================================================

static String inputBuffer;

// Process a completed command.
static void processCommand(String command) {

  command.trim();

  if (command == "help") {
    Serial.println("Proto Terminal Configuration");
    Serial.println("Commands:");
    Serial.println("  help");
    Serial.println("  status");
    Serial.println("  server <url>");
    Serial.println("  key <value>");
    return;
  }

  if (command == "status") {
    Serial.print("Server: ");
    Serial.println(getApiServerUrl());

    Serial.print("API Key: ");
    Serial.println(getApiKey().length() > 0
      ? "[CONFIGURED]"
      : "[NOT CONFIGURED]");
    return;
  }

  if (command.startsWith("server ")) {
    String url = command.substring(7);
    url.trim();

    if (url.length() == 0 || url.length() >= 128 ||
        !url.startsWith("http://")) {
      Serial.println("Invalid server URL.");
      return;
    }

    saveApiConfiguration(url, getApiKey());
    Serial.println("Server saved.");
    return;
  }

  if (command.startsWith("key ")) {
    String key = command.substring(4);
    key.trim();

    if (key.length() == 0 || key.length() >= 96) {
      Serial.println("Invalid API key.");
      return;
    }

    saveApiConfiguration(getApiServerUrl(), key);
    Serial.println("API key saved.");
    return;
  }

  Serial.println("Unknown command. Type help.");
}


// ============================================================
// INITIALIZATION
// ============================================================

void initSerialConfig() {
  Serial.begin(115200);
  inputBuffer.reserve(160);

  Serial.println();
  Serial.println("Proto Terminal Serial Configuration");
  Serial.println("Type help for available commands.");
}


// ============================================================
// COMMAND LOOP
// ============================================================

void updateSerialConfig() {

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\r') {
      continue;
    }

    if (c == '\n') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
      continue;
    }

    if (inputBuffer.length() < 160) {
      inputBuffer += c;
    } else {
      inputBuffer = "";
      Serial.println("Command too long.");
    }
  }
}