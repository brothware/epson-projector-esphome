# Configuration Guide

Full configuration reference for the Epson Projector ESPHome component.

## Hub Configuration

```yaml
uart:
  id: projector_uart
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 9600

epson_projector:
  id: projector
  uart_id: projector_uart
  model: "eh-tw7400"      # See docs/MODELS.md
  update_interval: 5s     # Polling interval
```

## Complete Example

```yaml
external_components:
  - source: github://brothware/epson-projector-esphome@v1.0.0
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
    h_reverse:
      name: "Horizontal Reverse"
    v_reverse:
      name: "Vertical Reverse"
    freeze:
      name: "Freeze"

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

text_sensor:
  - platform: epson_projector
    projector_id: projector
    serial_number:
      name: "Serial Number"

select:
  - platform: epson_projector
    projector_id: projector
    source:
      name: "Input Source"
    color_mode:
      name: "Color Mode"
    aspect_ratio:
      name: "Aspect Ratio"
    luminance:
      name: "Luminance"
    gamma:
      name: "Gamma"

number:
  - platform: epson_projector
    projector_id: projector
    brightness:
      name: "Brightness"
    contrast:
      name: "Contrast"
    volume:
      name: "Volume"
    sharpness:
      name: "Sharpness"
    density:
      name: "Color Saturation"
    tint:
      name: "Tint"
    color_temperature:
      name: "Color Temperature"
    v_keystone:
      name: "Vertical Keystone"
    h_keystone:
      name: "Horizontal Keystone"
```

## Platform Reference

### Switch

| Entity | Description |
|--------|-------------|
| `power` | Power on/off control |
| `mute` | A/V mute control |
| `h_reverse` | Horizontal image flip |
| `v_reverse` | Vertical image flip |
| `freeze` | Freeze frame |

### Binary Sensor

| Entity | Description |
|--------|-------------|
| `power_state` | True when projector is on |
| `mute_state` | True when A/V is muted |

### Sensor

| Entity | Description |
|--------|-------------|
| `lamp_hours` | Lamp usage in hours |
| `error_code` | Current error code (0 = no error) |

### Text Sensor

| Entity | Description |
|--------|-------------|
| `serial_number` | Projector serial number |

### Select

| Entity | Description |
|--------|-------------|
| `source` | Input source selection (options from model config) |
| `color_mode` | Color mode selection (Dynamic, Cinema, etc.) |
| `aspect_ratio` | Aspect ratio selection (Normal, 4:3, 16:9, etc.) |
| `luminance` | Lamp brightness mode (High, Low, Medium) |
| `gamma` | Gamma preset (2.0, 2.1, 2.2, 2.3, 2.4, Custom) |

### Number

| Entity | Range | Description |
|--------|-------|-------------|
| `brightness` | 0-100 | Picture brightness |
| `contrast` | 0-100 | Picture contrast |
| `volume` | 0-20 | Audio volume |
| `sharpness` | 0-20 | Image sharpness |
| `density` | 0-100 | Color saturation |
| `tint` | 0-100 | Color tint/hue |
| `color_temperature` | 0-13 | Color temperature (warm to cool) |
| `v_keystone` | 0-60 | Vertical keystone correction |
| `h_keystone` | 0-60 | Horizontal keystone correction |

## Smart Polling

The component only polls properties that have configured entities:

- If you only configure a power switch, only power state is queried
- Adding a lamp_hours sensor will also query lamp hours
- This reduces unnecessary serial traffic to the projector

Queries only run when the projector is powered on (except for power state itself).
