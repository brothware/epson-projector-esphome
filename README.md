# Epson Projector ESPHome Component

[![CI](https://github.com/brothware/epson-projector-esphome/actions/workflows/ci.yml/badge.svg)](https://github.com/brothware/epson-projector-esphome/actions/workflows/ci.yml)
[![Tests](https://github.com/brothware/epson-projector-esphome/actions/workflows/tests.yml/badge.svg)](https://github.com/brothware/epson-projector-esphome/actions/workflows/tests.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESPHome](https://img.shields.io/badge/ESPHome-2024.1+-blue.svg)](https://esphome.io/)
[![GitHub release](https://img.shields.io/github/v/release/brothware/epson-projector-esphome)](https://github.com/brothware/epson-projector-esphome/releases)

ESPHome external component for controlling Epson projectors via the ESC/VP21 serial protocol.

## Features

- Power control with warmup/cooldown state tracking
- Input source, color mode, aspect ratio selection
- Picture adjustments (brightness, contrast, sharpness, saturation, tint)
- Keystone correction (vertical/horizontal)
- Volume control and A/V mute
- Lamp hours and error monitoring
- Model-based configuration with 50+ supported models
- Smart polling - only queries configured entities

## Quick Start

```yaml
external_components:
  - source: github://brothware/epson-projector-esphome@v1.0.0
    components:
      - epson_projector

uart:
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 9600

epson_projector:
  id: projector
  model: "eh-tw7400"

switch:
  - platform: epson_projector
    projector_id: projector
    power:
      name: "Projector Power"

sensor:
  - platform: epson_projector
    projector_id: projector
    lamp_hours:
      name: "Lamp Hours"
```

## Hardware

Connect ESP32 to projector's RS-232 port via MAX3232 level shifter:

```
ESP32 ── MAX3232 ── RS-232 (DB-9)
 TX   ──  T1IN/T1OUT  ──  Pin 2 (RX)
 RX   ──  R1OUT/R1IN  ──  Pin 3 (TX)
 GND  ──────────────────  Pin 5 (GND)
```

> **Note**: RS-232 uses ±12V levels. A level shifter (MAX3232) is required.

## Documentation

| Document | Description |
|----------|-------------|
| [Configuration Guide](docs/CONFIGURATION.md) | Full YAML reference and examples |
| [Supported Models](docs/MODELS.md) | List of supported projector models |
| [Hardware Setup](docs/HARDWARE.md) | Wiring diagrams and hardware details |
| [Protocol Reference](docs/PROTOCOL.md) | ESC/VP21 commands and codes |
| [Development](docs/DEVELOPMENT.md) | Contributing and testing |

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No response | Check TX/RX wiring, verify 9600 baud, check level shifter |
| State not updating | Check ESPHome logs, verify entity configuration |
| Wrong sources | Verify model ID or use `model: "generic"` |

## License

MIT License - see [LICENSE](LICENSE)
