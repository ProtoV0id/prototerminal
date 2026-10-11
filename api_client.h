#pragma once

#include <Arduino.h>

static const int MAX_PROTO_WORKSPACES = 16;
static const int MAX_PROTO_TARGETS = 32;

struct ProtoReconStatus {
  bool online;
  String workspace;
  String target;
  int workspaceCount;
  int targetCount;
};

struct ProtoReconWorkspaceList {
  bool online;
  String active;
  String items[MAX_PROTO_WORKSPACES];
  int count;
};

struct ProtoReconTargetList {
  bool online;
  String active;
  String items[MAX_PROTO_TARGETS];
  int count;
};

bool getProtoReconStatus(ProtoReconStatus &status);
bool getProtoReconWorkspaces(ProtoReconWorkspaceList &workspaces);
bool setProtoReconWorkspace(const String &workspaceName);
bool getProtoReconTargets(ProtoReconTargetList &targets);
bool setProtoReconTarget(const String &targetName);
