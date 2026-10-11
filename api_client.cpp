
#include "api_client.h"
#include "settings.h"

#include <rpcWiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


// ============================================================
// PROTORECON API CONNECTION
// ============================================================

static bool beginProtoReconRequest(
  HTTPClient &http,
  const String &endpoint
) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[API] WiFi is not connected.");
    return false;
  }

  if (!hasApiConfiguration()) {
    Serial.println("[API] Server URL or API key not configured.");
    return false;
  }

  String serverUrl = getApiServerUrl();
  String apiKey = getApiKey();

  serverUrl.trim();

  if (!serverUrl.startsWith("http://")) {
    Serial.println("[API] Invalid server URL.");
    return false;
  }

  // Remove trailing slashes.
  while (serverUrl.endsWith("/")) {
    serverUrl.remove(serverUrl.length() - 1);
  }

  String fullUrl = serverUrl + endpoint;

  if (!http.begin(fullUrl)) {
    Serial.println("[API] Failed to initialize HTTP.");
    return false;
  }

  // Authenticate with ProtoRecon FastAPI.
  http.addHeader("X-API-Key", apiKey);

  // Connection and response timeouts.
  http.setConnectTimeout(3000);
  http.setTimeout(5000);

  return true;
}


// ============================================================
// HTTP ERROR HANDLING
// ============================================================

static bool checkProtoReconResponse(
  HTTPClient &http,
  int httpCode
) {
  if (httpCode <= 0) {
    Serial.print("[API] Connection failed: ");
    Serial.println(http.errorToString(httpCode));
    return false;
  }

  Serial.print("[API] HTTP status: ");
  Serial.println(httpCode);

  if (httpCode == 200) {
    return true;
  }

  if (httpCode == 401) {
    Serial.println("[API] Authentication failed. Check API key.");
  }
  else if (httpCode == 403) {
    Serial.println("[API] Access forbidden.");
  }
  else if (httpCode == 404) {
    Serial.println("[API] Endpoint not found.");
  }
  else if (httpCode == 503) {
    Serial.println("[API] Server unavailable or authentication not configured.");
  }
  else {
    Serial.println("[API] Unexpected server response.");
  }

  return false;
}


// ============================================================
// GET PROTORECON STATUS
// ============================================================

bool getProtoReconStatus(ProtoReconStatus &status) {
  status.online = false;
  status.workspace = "";
  status.target = "";
  status.workspaceCount = 0;
  status.targetCount = 0;

  HTTPClient http;

  if (!beginProtoReconRequest(http, "/api/status")) {
    return false;
  }

  Serial.println("[API] GET /api/status");

  int httpCode = http.GET();

  if (!checkProtoReconResponse(http, httpCode)) {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<2048> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("[API] Status JSON parse failed: ");
    Serial.println(error.c_str());
    return false;
  }

  status.online = true;
  status.workspace = String(doc["workspace"] | "default");

  if (doc["target"].isNull()) {
    status.target = "None";
  } else {
    status.target = String(doc["target"] | "");
  }

  JsonArray workspaces = doc["workspaces"].as<JsonArray>();
  status.workspaceCount = workspaces.size();

  JsonArray targets = doc["targets"].as<JsonArray>();
  status.targetCount = targets.size();

  Serial.println("[API] Status parsed successfully.");

  return true;
}


// ============================================================
// GET PROTORECON WORKSPACES
// ============================================================

bool getProtoReconWorkspaces(
  ProtoReconWorkspaceList &workspaces
) {
  workspaces.online = false;
  workspaces.active = "";
  workspaces.count = 0;

  for (int i = 0; i < MAX_PROTO_WORKSPACES; i++) {
    workspaces.items[i] = "";
  }

  HTTPClient http;

  if (!beginProtoReconRequest(http, "/api/workspaces")) {
    return false;
  }

  Serial.println("[API] GET /api/workspaces");

  int httpCode = http.GET();

  if (!checkProtoReconResponse(http, httpCode)) {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<3072> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("[API] Workspace JSON parse failed: ");
    Serial.println(error.c_str());
    return false;
  }

  workspaces.online = true;
  workspaces.active = String(doc["active"] | "default");

  JsonArray list = doc["workspaces"].as<JsonArray>();

  for (JsonVariant item : list) {
    if (workspaces.count >= MAX_PROTO_WORKSPACES) {
      break;
    }

    workspaces.items[workspaces.count] =
      String(item.as<const char*>());

    workspaces.count++;
  }

  Serial.print("[API] Workspaces loaded: ");
  Serial.println(workspaces.count);

  Serial.print("[API] Active workspace: ");
  Serial.println(workspaces.active);

  return true;
}


// ============================================================
// SET ACTIVE WORKSPACE
// ============================================================

bool setProtoReconWorkspace(
  const String &workspaceName
) {
  HTTPClient http;

  if (!beginProtoReconRequest(
        http,
        "/api/workspaces/use"
      )) {
    return false;
  }

  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<512> requestDoc;
  requestDoc["workspace"] = workspaceName;

  String requestBody;
  serializeJson(requestDoc, requestBody);

  Serial.print("[API] Switching workspace to: ");
  Serial.println(workspaceName);

  int httpCode = http.POST(requestBody);

  if (!checkProtoReconResponse(http, httpCode)) {
    http.end();
    return false;
  }

  http.end();

  Serial.println("[API] Workspace switched successfully.");

  return true;
}


// ============================================================
// GET PROTORECON TARGETS
// ============================================================

bool getProtoReconTargets(
  ProtoReconTargetList &targets
) {
  targets.online = false;
  targets.active = "";
  targets.count = 0;

  for (int i = 0; i < MAX_PROTO_TARGETS; i++) {
    targets.items[i] = "";
  }

  HTTPClient http;

  if (!beginProtoReconRequest(http, "/api/targets")) {
    return false;
  }

  Serial.println("[API] GET /api/targets");

  int httpCode = http.GET();

  if (!checkProtoReconResponse(http, httpCode)) {
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  StaticJsonDocument<4096> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("[API] Target JSON parse failed: ");
    Serial.println(error.c_str());
    return false;
  }

  targets.online = true;

  if (doc["active"].isNull()) {
    targets.active = "";
  } else {
    targets.active = String(doc["active"] | "");
  }

  JsonArray list = doc["targets"].as<JsonArray>();

  for (JsonVariant item : list) {
    if (targets.count >= MAX_PROTO_TARGETS) {
      break;
    }

    targets.items[targets.count] =
      String(item.as<const char*>());

    targets.count++;
  }

  Serial.print("[API] Targets loaded: ");
  Serial.println(targets.count);

  Serial.print("[API] Active target: ");

  if (targets.active.length() > 0) {
    Serial.println(targets.active);
  } else {
    Serial.println("None");
  }

  return true;
}


// ============================================================
// SET ACTIVE TARGET
// ============================================================

bool setProtoReconTarget(
  const String &targetName
) {
  HTTPClient http;

  if (!beginProtoReconRequest(
        http,
        "/api/targets/use"
      )) {
    return false;
  }

  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<512> requestDoc;
  requestDoc["target"] = targetName;

  String requestBody;
  serializeJson(requestDoc, requestBody);

  Serial.print("[API] Aiming at target: ");
  Serial.println(targetName);

  int httpCode = http.POST(requestBody);

  if (!checkProtoReconResponse(http, httpCode)) {
    http.end();
    return false;
  }

  http.end();

  Serial.println("[API] Target switched successfully.");

  return true;
}
