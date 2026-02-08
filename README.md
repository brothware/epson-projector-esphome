# Epson Projector ESPHome Component

ESPHome external component for controlling Epson projectors via the ESC/VP21 serial protocol.

## Features

- Power on/off control with warmup/cooldown state tracking
- Input source selection (model-specific sources)
- A/V mute control
- Volume control (0-20)
- Lamp hours monitoring
- Error code monitoring
- Picture settings (brightness 0-100, contrast 0-100)
- Color mode selection (Dynamic, Cinema, Natural, etc.)
- Aspect ratio selection (Normal, 4:3, 16:9, Auto, etc.)
- Model-based configuration with automatic feature detection
- Smart polling: only configured entities trigger projector queries

## Installation

Add to your ESPHome configuration:

```yaml
external_components:
  - source: github://brothware/epson-projector-esphome@main
    components:
      - epson_projector
```

## Configuration

### Hub Configuration

```yaml
uart:
  id: projector_uart
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 9600

epson_projector:
  id: projector
  uart_id: projector_uart
  model: "eh-tw7400"      # See supported models below
  update_interval: 5s     # Polling interval
```

### Complete Example

```yaml
external_components:
  - source: github://brothware/epson-projector-esphome@main
    components:
      - epson_projector

uart:
  id: projector_uart
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 9600

epson_projector:
  id: projector
  uart_id: projector_uart
  model: "eh-tw7400"
  update_interval: 5s

switch:
  - platform: epson_projector
    projector_id: projector
    power:
      name: "Projector Power"
    mute:
      name: "Projector Mute"

binary_sensor:
  - platform: epson_projector
    projector_id: projector
    power_state:
      name: "Projector Power State"
    mute_state:
      name: "Projector Mute State"

sensor:
  - platform: epson_projector
    projector_id: projector
    lamp_hours:
      name: "Lamp Hours"
    error_code:
      name: "Error Code"

select:
  - platform: epson_projector
    projector_id: projector
    source:
      name: "Input Source"
    color_mode:
      name: "Color Mode"
    aspect_ratio:
      name: "Aspect Ratio"

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

### Platform Reference

#### Switch

| Entity | Description |
|--------|-------------|
| `power` | Power on/off control |
| `mute` | A/V mute control |

#### Binary Sensor

| Entity | Description |
|--------|-------------|
| `power_state` | True when projector is on |
| `mute_state` | True when A/V is muted |

#### Sensor

| Entity | Description |
|--------|-------------|
| `lamp_hours` | Lamp usage in hours |
| `error_code` | Current error code (0 = no error) |

#### Select

| Entity | Description |
|--------|-------------|
| `source` | Input source selection (options from model config) |
| `color_mode` | Color mode selection (Dynamic, Cinema, etc.) |
| `aspect_ratio` | Aspect ratio selection (Normal, 4:3, 16:9, etc.) |

#### Number

| Entity | Range | Description |
|--------|-------|-------------|
| `brightness` | 0-100 | Picture brightness |
| `contrast` | 0-100 | Picture contrast |
| `volume` | 0-20 | Audio volume |

## Supported Models

The component includes configurations for many Epson projector models. Specify the model ID in lowercase with hyphen (e.g., `eh-tw7400`).

### Home Cinema Series

| Series | Models |
|--------|--------|
| EH-TW Entry | TW490, TW570, TW610, TW650, TW700, TW710, TW740, TW750 |
| EH-TW5xxx | TW5000-TW5825 (various models) |
| EH-TW6xxx | TW6000-TW6800 (3D support) |
| EH-TW7xxx | TW7000, TW7100, TW7200, TW7300, TW7400 |
| EH-TW8xxx | TW8000-TW8400 (3D support) |
| EH-TW9xxx | TW9000-TW9400 (flagship, 3D support) |
| EH-LS Laser | LS300, LS500, LS650, LS800, LS9000-LS12000 |

### Business Series

| Series | Models |
|--------|--------|
| EB-S (SVGA) | S04, S05, S06, S31, S41 |
| EB-X (XGA) | X05, X06, X31, X41, X49, X50, X51 |
| EB-W (WXGA) | W05, W06, W31, W41, W42, W49-W52 |
| EB-U (WUXGA) | U32, U42, U50 |
| EB-E (Entry) | E01, E10, E20 |
| EB-FH (Full HD) | FH06, FH52 |
| EB-17xx/18xx | 1780W, 1781W, 1785W, 1795F |
| EB-2xxx | 2040-2265U (various models) |
| EB-9xx | 970, 972, 980W, 982W, 990U, 992F |
| EB-L Laser | L200F, L200W, L210W, L255F, L260F, L265F |

### Generic Fallback

Use `model: "generic"` for any ESC/VP21 compatible projector not in the list. This enables all common features and input sources.

## Hardware Connection

Connect your ESP32/ESP8266 to the projector's RS-232 port:

```
ESP32          RS-232 (DB-9)
─────          ─────────────
TX  ──────────── RX (Pin 2)
RX  ──────────── TX (Pin 3)
GND ──────────── GND (Pin 5)
```

**Important**: Projectors use RS-232 voltage levels (±12V). You need a level shifter such as MAX3232 between the ESP and the projector.

### Typical Wiring with MAX3232

```
ESP32        MAX3232        RS-232 Connector
─────        ───────        ────────────────
3.3V ─────── VCC
GND  ─────── GND ────────── Pin 5 (GND)
TX   ─────── T1IN
             T1OUT ──────── Pin 2 (RX)
RX   ─────── R1OUT
             R1IN ───────── Pin 3 (TX)
```

## Protocol Reference

This component implements the Epson ESC/VP21 protocol.

### Commands

| Command | Query | Description |
|---------|-------|-------------|
| `PWR ON` / `PWR OFF` | `PWR?` | Power control |
| `SOURCE xx` | `SOURCE?` | Input source (xx = source code) |
| `MUTE ON` / `MUTE OFF` | `MUTE?` | A/V mute |
| `VOL xx` | `VOL?` | Volume (0-20) |
| `BRIGHT xx` | `BRIGHT?` | Brightness (0-255 raw) |
| `CONTRAST xx` | `CONTRAST?` | Contrast (0-255 raw) |
| `CMODE xx` | `CMODE?` | Color mode (xx = mode code) |
| `ASPECT xx` | `ASPECT?` | Aspect ratio (xx = ratio code) |
| - | `LAMP?` | Lamp hours |
| - | `ERR?` | Error code |

### Power States

| Value | State |
|-------|-------|
| 00 | Standby |
| 01 | On |
| 02 | Warmup |
| 03 | Cooldown |

### Source Codes

| Code | Source |
|------|--------|
| 10 | Computer1 |
| 20 | Computer2 |
| 30 | HDMI1 |
| A0 | HDMI2 |
| 14 | Component |
| 41 | Video |
| 42 | S-Video |
| 52 | USB |
| 53 | LAN |

### Color Mode Codes

| Code | Mode |
|------|------|
| 01 | sRGB |
| 02 | Text |
| 04 | Presentation |
| 05 | Cinema |
| 06 | Dynamic |
| 07 | Natural |
| 08 | Sports |
| 0C | Bright Cinema |
| 10 | Custom |
| 11 | Blackboard |
| 14 | Photo |
| 15 | 3D Dynamic |
| 17 | 3D Cinema |

### Aspect Ratio Codes

| Code | Ratio |
|------|-------|
| 00 | Normal |
| 10 | 4:3 |
| 20 | 16:9 |
| 30 | Auto |
| 40 | Full |
| 50 | Zoom |
| 60 | Native |
| 70 | Wide |
| 80 | Anamorphic |

## Smart Polling

The component only polls properties that have configured entities. For example:
- If you only configure a power switch, only power state is queried
- Adding a lamp_hours sensor will also query lamp hours
- This reduces unnecessary serial traffic to the projector

Queries only run when the projector is powered on (except for power state itself).

## Development

### Setup

```bash
# Clone the repository
git clone https://github.com/brothware/epson-projector-esphome.git
cd epson-projector-esphome

# Install pre-commit hooks
./scripts/setup-hooks.sh
```

### Running Tests

```bash
# Run all tests
./scripts/run-tests.sh

# Run Python tests only
pytest tests/

# Run C++ tests only
cd tests/cpp && mkdir -p build && cd build
cmake .. && make && ctest
```

### Linting

```bash
pre-commit run --all-files
```

### Building Test Configuration

```bash
esphome compile tests/esphome/test_config.yaml
```

## Troubleshooting

### No response from projector

1. Check wiring - TX/RX might be swapped
2. Verify baud rate is 9600
3. Ensure level shifter is working (check voltages)
4. Try the `generic` model if your specific model isn't listed

### Commands work but state not updating

1. Check ESPHome logs for response parsing
2. Verify the projector is responding to queries
3. Ensure entities are properly configured (they register callbacks on setup)

### Wrong input sources shown

1. Verify the model ID matches your projector
2. Check `models.py` for your model's source configuration
3. Use `generic` model for full source list

## License

MIT License - see [LICENSE](LICENSE)
