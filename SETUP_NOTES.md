# Proto Terminal local setup

1. Copy `wifi_config.example.h` to `wifi_config.h` in the same sketch folder.
2. Edit `wifi_config.h`: set `WIFI_SSID` and `WIFI_PASSWORD`. The blank template compiles, but Wi-Fi connection needs real credentials.
3. Open `Proto_Terminal.ino` in Arduino IDE, install the Wio Terminal board support and required libraries, then compile/upload.
4. Use Serial Monitor at 115200 baud with newline ending and run `server http://YOUR_SERVER_IP:8000` and `key YOUR_WIO_API_KEY`; then `status`. Those settings persist in device flash and are not stored in any repository file.
5. Configure the corresponding `PROTORECON_WIO_API_KEY` in the ProtoRecon host's private `.env`.

**Security:** The Wio client currently uses HTTP, so the key is not encrypted on the Wi-Fi leg. Use a secured trusted network; the travel router's Tailscale connection encrypts the routed leg only. Serial Monitor input may echo credentials.
