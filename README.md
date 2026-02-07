# Epson Projector ESPHome Component

ESPHome external component for controlling Epson projectors via the ESC/VP21 serial protocol.

## Features

- Power on/off control
- Input source selection
- A/V mute control
- Lamp hours monitoring
- Picture settings (brightness, contrast, color mode)
- Geometry settings (keystone correction)
- Model-based configuration with automatic feature detection

## Installation

Add to your ESPHome configuration:

```yaml
external_components:
  - source: github://brothware/epson-projector-esphome@main
    components:
      - epson_projector
```

## Configuration

### Basic Setup

```yaml
uart:
  id: projector_uart
  tx_pin: GPIO3
  rx_pin: GPIO4
  baud_rate: 9600

epson_projector:
  id: projector
  uart_id: projector_uart
  model: "EH-TW7400"
  update_interval: 5s
```

### Available Platforms

#### Switch

```yaml
switch:
  - platform: epson_projector
    projector_id: projector
    power:
      name: "Projector Power"
    mute:
      name: "Projector Mute"
```

#### Binary Sensor

```yaml
binary_sensor:
  - platform: epson_projector
    projector_id: projector
    power_state:
      name: "Projector Power State"
```

#### Sensor

```yaml
sensor:
  - platform: epson_projector
    projector_id: projector
    lamp_hours:
      name: "Lamp Hours"
    error_code:
      name: "Error Code"
```

#### Select

```yaml
select:
  - platform: epson_projector
    projector_id: projector
    source:
      name: "Input Source"
    color_mode:
      name: "Color Mode"
```

#### Number

```yaml
number:
  - platform: epson_projector
    projector_id: projector
    brightness:
      name: "Brightness"
    contrast:
      name: "Contrast"
    volume:
      name: "Volume"
```

## Supported Models

- EH-TW7400 (Home Cinema)
- EB-U42 (Business)
- Generic (fallback for any ESC/VP21 compatible projector)

See `components/epson_projector/models.py` for the full list.

## Hardware Connection

Connect your ESP32/ESP8266 to the projector's RS-232 port:

| ESP Pin | RS-232 Signal | Description |
|---------|---------------|-------------|
| TX      | RX (Pin 2)    | Data to projector |
| RX      | TX (Pin 3)    | Data from projector |
| GND     | GND (Pin 5)   | Ground |

**Note**: You may need an RS-232 to TTL level shifter (e.g., MAX3232) as projectors use ±12V RS-232 levels.

## Protocol Reference

This component implements the Epson ESC/VP21 protocol. Key commands:

| Command | Query | Description |
|---------|-------|-------------|
| `PWR ON/OFF` | `PWR?` | Power control |
| `SOURCE xx` | `SOURCE?` | Input source |
| `MUTE ON/OFF` | `MUTE?` | A/V mute |
| `VOL xx` | `VOL?` | Volume |
| - | `LAMP?` | Lamp hours |
| - | `ERR?` | Error code |

## Development

### Setup

```bash
./scripts/setup-hooks.sh
```

### Running Tests

```bash
./scripts/run-tests.sh
```

### Linting

```bash
pre-commit run --all-files
```

## License

MIT License - see [LICENSE](LICENSE)
