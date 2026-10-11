# Proto Terminal — NightGlass Beta

**A pocket-sized companion interface for [ProtoRecon](https://github.com/ProtoV0id/protorecon), built for the Seeed Studio Wio Terminal.**

Proto Terminal connects to a self-hosted ProtoRecon FastAPI server over Wi-Fi. It provides an on-device interface for viewing the current ProtoRecon session, browsing workspaces and targets, and changing the active workspace or target. The OSINT tools themselves execute on the computer running ProtoRecon, **not** on the Wio Terminal.

> **Beta notice:** This is pre-release firmware. Some menu items are placeholders, features may change, and compatibility has not been validated on every board package or operating system. Use it on networks and systems you control.

## Features in this beta

- **ProtoRecon Status:** check API connectivity, active workspace, active target, and counts.
- **Workspaces:** list available workspaces and choose an active workspace.
- **Targets:** list available targets and choose an active target.
- **Authenticated API access:** sends the `X-API-Key` header on ProtoRecon requests.
- **Persistent API settings:** save the API server URL and authentication key in device flash via USB Serial.
- **Wi-Fi menu:** connect to the configured SSID, scan nearby networks, disconnect, and display network information.
- **Two display themes:** Default and Night City; theme preference is stored in flash.
- **Modular firmware:** separate source files for API, settings, Wi-Fi, UI screens, and themes.

**Not yet implemented:** on-device OSINT module execution; a graphical list of optional OSINT API key statuses; adding new targets/workspaces from the device; the on-screen Settings → API editor. The main menu also contains placeholder categories for future expansion.

## Architecture

```text
Wio Terminal
  |  Wi-Fi, HTTP, X-API-Key
  v
Trusted Wi-Fi / local router
  |  LAN routing, or an optional Tailscale-enabled gateway
  v
ProtoRecon FastAPI (port 8000)
  |
  +-- Workspaces and targets
  +-- Status and other API endpoints
  +-- Python OSINT tools (executed on the ProtoRecon host)
```

Proto Terminal **does not** run a Tailscale client. For remote access, a compatible travel router may connect to a Tailscale network and route Wio traffic to a remote ProtoRecon host. No Cloudflare Tunnel or public port forwarding is required for that arrangement.

## Requirements

### Hardware

- [Seeed Studio Wio Terminal](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/)
- USB-C **data** cable for programming and configuration
- Wi-Fi network the Wio can join (the current firmware does **not** handle captive-portal sign-ins)
- Computer or server running a compatible ProtoRecon release
- Optional: GL.iNet or another router with working Tailscale client/routing for remote operation

### Software

- Arduino IDE 2.x (or another Arduino IDE version compatible with the Wio Terminal)
- **Seeed SAMD Boards** board package, selecting **Seeed Wio Terminal**
- The Arduino libraries/headers used by the source:
  - `TFT_eSPI`
  - `FlashStorage`
  - `ArduinoJson` (the firmware uses `StaticJsonDocument`, the ArduinoJson 6 API)
  - `rpcWiFi` and `HTTPClient` supplied by the compatible Wio Terminal board/core stack
- [ProtoRecon](https://github.com/ProtoV0id/ProtoRecon) installed and running on the host

Install the Wio board support using Seeed Studio's [official getting-started guide](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/). Install the library dependencies through **Arduino IDE → Library Manager** as appropriate for your board package. If your toolchain reports missing headers, confirm the **Wio Terminal board** is selected before installing unrelated libraries with similar names.

> Library version compatibility can vary. This beta's uploaded firmware source has not been rebuilt in this README-writing pass. ArduinoJson 7 may require source changes because the firmware uses ArduinoJson 6-style declarations.

## Installation

### 1. Download and open the firmware

Clone or download this repository. All `.ino`, `.cpp`, and `.h` files belong together in the **same Arduino sketch directory**. Keep the sketch directory named `Proto_Terminal` so Arduino can recognize `Proto_Terminal.ino` correctly.

```bash
# Example: after cloning a repository to a folder with a different name
mv prototerminal Proto_Terminal
```

Only rename the folder if needed; don't rename source files.

### 2. Create your private Wi-Fi configuration

The repository ships a safe template named `wifi_config.example.h`. Make your own local copy:

```bash
cp wifi_config.example.h wifi_config.h
```

Edit **`wifi_config.h`**:

```cpp
#pragma once

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
```

`wifi_config.h` is excluded by `.gitignore`; the example file remains public. The firmware reads this configuration to connect when you select Wi-Fi → Connect. A network scan does not automatically supply credentials for a different SSID.

### 3. Select the board and upload

1. Open `Proto_Terminal.ino` in Arduino IDE.
2. Select **Tools → Board → Seeed SAMD Boards → Seeed Wio Terminal**.
3. Select the Wio's USB serial port in **Tools → Port**.
4. Verify/compile the sketch, then **Upload**.

If upload fails with `SAM-BA operation failed`, close Serial Monitor, manually enter Wio bootloader mode by quickly double-sliding and releasing its power switch, choose the bootloader port, and retry. Seeed documents the bootloader procedure in its getting-started guide.

### 4. Configure the ProtoRecon host

On the **computer running ProtoRecon**, create its private configuration file according to the [ProtoRecon README](https://github.com/ProtoV0id/ProtoRecon). Configure an independently generated device authentication secret under:

```dotenv
PROTORECON_WIO_API_KEY=YOUR_GENERATED_SECRET
```

To generate a secret:

```bash
python -c "import secrets; print(secrets.token_urlsafe(32))"
```

Start FastAPI on an interface reachable from the Wio's network. For example, to listen **only** on a host's existing Tailscale address:

```bash
python -m uvicorn protorecon.api.server:app --host YOUR_TAILSCALE_IP --port 8000
```

For localhost-only development, use `--host 127.0.0.1`. If you use `--host 0.0.0.0`, Uvicorn listens on all host interfaces; restrict access with the host firewall and do **not** forward port 8000 to the public internet.

The host's API **base URL** looks like `http://YOUR_SERVER_IP:8000`. Do **not** append `/api/status` when configuring the Wio; it appends endpoint paths itself.

### 5. Provision the Wio through USB Serial

Connect the Wio to your computer. Open **Tools → Serial Monitor** and configure:

- **Baud rate:** `115200`
- **Line ending:** `Newline`

Enter these commands one at a time:

```text
help
server http://YOUR_SERVER_IP:8000
key YOUR_GENERATED_SECRET
status
```

Use the **same** secret stored in `PROTORECON_WIO_API_KEY` on the ProtoRecon host. Sample output:

```text
Server: http://YOUR_SERVER_IP:8000
API Key: [CONFIGURED]
```

The Serial commands implemented by this beta are:

| Command | Purpose |
| --- | --- |
| `help` | Show supported commands |
| `status` | Display stored server URL and whether a key is present |
| `server http://HOST:8000` | Save the ProtoRecon API base URL |
| `key SECRET` | Save the device authentication key |

Server URLs must begin with `http://` in this beta. The Serial interface does not implement an HTTPS client or a graphical key-entry flow. **`[CONFIGURED]` means a nonempty key is stored; it does not prove authentication succeeded.** The key and URL persist across normal power cycles through `FlashStorage`. Persistence across *all* firmware reflashes is not guaranteed.

> **Credentials warning:** The key is stored as plaintext in device flash and sent in an HTTP header. Arduino Serial Monitor may echo the key command. Provision from a trusted computer, avoid showing logs containing your secret, use a protected local Wi-Fi network, and rotate the secret if exposed. Tailscale encryption protects the routed link between Tailscale nodes, not necessarily the Wio-to-router Wi-Fi hop.

## Using Proto Terminal

### Controls

| Control | Function |
| --- | --- |
| 5-way joystick **Up / Down** | Navigate list selections |
| 5-way joystick **Press** | Select / execute |
| Top **A** button | Select / execute |
| 5-way joystick **Left** | Back |
| Top **B** button | Back |
| Top **C** button | Return to main menu |

Button labels refer to the `WIO_KEY_A`, `WIO_KEY_B`, and `WIO_KEY_C` mappings used by the firmware; on-screen prompts may say EXEC, BACK, or HOME.

### ProtoRecon

Open **ProtoRecon** from the main menu:

- **Status:** fetch the current workspace, active target, and workspace/target counts. Select/execute again to refresh.
- **Workspace:** view existing workspaces and select one to make active on the server.
- **Target:** view existing targets and select one to aim at on the server.

The Wio changes **server-side session state**, so selections may affect the active ProtoRecon CLI session. Current firmware does not create targets or run OSINT modules from its menus.

### Network and settings

- **Settings → WiFi:** connect to the compiled-in SSID, scan for networks, or disconnect.
- **Settings → Theme:** choose **Default** or **Night City**. The choice is remembered across power cycles.
- **Settings → API:** placeholder in this beta. Use USB Serial for server/key setup.
- **About:** firmware identification and NightGlass beta information.

Other main-menu categories are included for future functionality and may open placeholder screens.

## Optional remote access with a Tailscale travel router

Remote access has been tested using a **GL.iNet travel router running Tailscale**, with the Wio attached to the router's Wi-Fi and ProtoRecon running on a Tailscale-connected home machine.

1. Install and authorize Tailscale on the **travel router** and on the **ProtoRecon host**.
2. Configure the travel router to route LAN-client traffic to tailnet destinations; verify its forwarding, firewall, and Tailscale settings allow that traffic.
3. Connect the Wio to the travel router's Wi-Fi via `wifi_config.h` and the Wi-Fi menu.
4. Configure the Wio's `server` command with the ProtoRecon host's reachable **Tailscale IP** and port, e.g., `http://YOUR_TAILSCALE_IP:8000`.
5. Ensure Uvicorn and the host firewall permit access on the desired tailnet interface.
6. Navigate to **ProtoRecon → Status** and confirm it loads.

A **MagicDNS** name may work if the router's DNS handling makes it resolvable for Wio clients. Using the Tailscale IP avoids relying on that DNS arrangement. **No Tailscale software runs on the Wio itself**, and there is no Cloudflare Tunnel component in this project.

## Troubleshooting

| Symptom | What to check |
| --- | --- |
| `wifi_config.h: No such file or directory` | Copy `wifi_config.example.h` to `wifi_config.h` in the sketch directory |
| Build fails on `rpcWiFi.h`, `HTTPClient.h`, or `TFT_eSPI.h` | Correct Wio Terminal board/core and compatible display/network libraries |
| Build fails with ArduinoJson symbols | Install compatible ArduinoJson 6 library version |
| `SAM-BA operation failed` | Close Serial Monitor; enter bootloader mode; reselect the USB port; use a data-capable USB cable |
| `Server URL or API key not configured` | Enter `server ...` and `key ...` in Serial Monitor, then run `status` |
| Wio shows offline / connection failure | Connect Wi-Fi; check server address, port, routing, host firewall, and whether FastAPI is running |
| HTTP `401` | Device key is missing or does not match `PROTORECON_WIO_API_KEY` on the server |
| HTTP `503` | The API authentication configuration may be missing on the host |
| Settings persist after unplugging but not after reflash | Re-provision through Serial; firmware uploads can modify flash layout or storage |
| Remote router setup doesn't connect | Verify travel-router Tailscale membership, routing to the tailnet, DNS, and firewall permissions |
| Public Wi-Fi login page appears | Captive portal authentication isn't supported in this firmware; use a network/router already authorized to connect |

For connection diagnostics, use Serial Monitor at `115200` and watch messages prefixed with `[API]`.

## Project layout

```text
Proto_Terminal/
├── Proto_Terminal.ino        # Setup and main loop
├── api_client.cpp/.h         # Authenticated FastAPI requests
├── serial_config.cpp/.h      # USB Serial provisioning commands
├── settings.cpp/.h           # Persistent theme + API settings
├── themes.cpp/.h             # Color themes
├── wifi_manager.cpp/.h       # Wi-Fi connection logic
├── wifi_config.example.h     # Public Wi-Fi credentials template
├── wifi_config.h             # Local credentials (ignored by Git)
├── ui_core.cpp               # Input processing and screen routing
├── ui_main.cpp/.h            # Main menu
├── ui_protorecon.cpp/.h      # ProtoRecon submenu
├── ui_proto_status.cpp/.h    # Status screen
├── ui_proto_workspace.cpp/.h # Workspaces screen
├── ui_proto_target.cpp/.h    # Targets screen
├── ui_settings.cpp/.h        # Settings and themes
├── ui_wifi.cpp/.h            # Wi-Fi screens
├── ui_shared.cpp/.h          # Shared drawing/layout helpers
├── ui_state.cpp/.h           # UI state
├── ui_about.cpp/.h           # About screen
└── README.md
```

The remaining `ui_*` and support headers in the repository are also part of the sketch. Do not copy just the `.ino` file or move the `.cpp` files to unrelated directories.

## Development roadmap

Planned improvements include a graphical status view for optional OSINT API keys, dynamic module discovery and execution, creating targets/workspaces on the device, improved device credential provisioning, and UI/memory optimization. These are **planned**, not implemented in this beta.

## Security and responsible use

This is a companion controller for a security research tool. Run ProtoRecon only against targets you are authorized to investigate, and comply with applicable laws and service rules. The device API key is an access credential, not a substitute for encrypted transport, network segmentation, or a host firewall. Never commit `wifi_config.h`, actual `.env` files, or populated API secrets.

## Feedback and contributions

Bug reports and feature requests are welcome through GitHub Issues. When filing an issue, include the Wio board/core version, Arduino IDE version, installed library versions, the ProtoRecon release version, and relevant Serial Monitor output **with all keys, IPs, hostnames, and personal identifiers redacted**.

**Project:** Proto Terminal  
**Release:** NightGlass Beta  
**Companion:** [ProtoRecon](https://github.com/ProtoV0id/protorecon)
