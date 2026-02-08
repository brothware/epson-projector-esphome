# ESC/VP21 Protocol Reference

This component implements the Epson ESC/VP21 serial protocol.

## Protocol Basics

- Commands end with carriage return (`\r`)
- Responses end with `\r:` (colon is the prompt)
- Query commands use `?` suffix (e.g., `PWR?`)
- Error response is `ERR\r:`

## Commands

| Command | Query | Description |
|---------|-------|-------------|
| `PWR ON` / `PWR OFF` | `PWR?` | Power control |
| `SOURCE xx` | `SOURCE?` | Input source (xx = source code) |
| `MUTE ON` / `MUTE OFF` | `MUTE?` | A/V mute |
| `VOL xx` | `VOL?` | Volume (0-255 raw) |
| `BRIGHT xx` | `BRIGHT?` | Brightness (0-255 raw) |
| `CONTRAST xx` | `CONTRAST?` | Contrast (0-255 raw) |
| `SHARP xx` | `SHARP?` | Sharpness (0-255 raw) |
| `DENSITY xx` | `DENSITY?` | Color saturation (0-255 raw) |
| `TINT xx` | `TINT?` | Color tint (0-255 raw) |
| `CTEMP xx` | `CTEMP?` | Color temperature (0-255 raw) |
| `CMODE xx` | `CMODE?` | Color mode (xx = mode code) |
| `ASPECT xx` | `ASPECT?` | Aspect ratio (xx = ratio code) |
| `LUMINANCE xx` | `LUMINANCE?` | Lamp brightness mode |
| `GAMMA xx` | `GAMMA?` | Gamma preset |
| `VKEYSTONE xx` | `VKEYSTONE?` | Vertical keystone (0-255 raw) |
| `HKEYSTONE xx` | `HKEYSTONE?` | Horizontal keystone (0-255 raw) |
| `HREVERSE ON/OFF` | `HREVERSE?` | Horizontal image flip |
| `VREVERSE ON/OFF` | `VREVERSE?` | Vertical image flip |
| `FREEZE ON/OFF` | `FREEZE?` | Freeze frame |
| - | `LAMP?` | Lamp hours |
| - | `ERR?` | Error code |
| - | `SNO?` | Serial number |

## Power States

| Value | State |
|-------|-------|
| 00 | Standby |
| 01 | On |
| 02 | Warmup |
| 03 | Cooldown |

## Source Codes

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

## Color Mode Codes

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

## Aspect Ratio Codes

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

## Luminance Codes

| Code | Mode |
|------|------|
| 00 | High |
| 01 | Low |
| 02 | Medium |

## Gamma Codes

| Code | Preset |
|------|--------|
| 20 | 2.0 |
| 21 | 2.1 |
| 22 | 2.2 |
| 23 | 2.3 |
| 24 | 2.4 |
| F0 | Custom |

## Value Scaling

The component automatically scales between protocol values (0-255) and user-friendly ranges:

| Parameter | UI Range | Protocol Range |
|-----------|----------|----------------|
| Brightness | 0-100 | 0-255 |
| Contrast | 0-100 | 0-255 |
| Volume | 0-20 | 0-255 |
| Sharpness | 0-20 | 0-255 |
| Density | 0-100 | 0-255 |
| Tint | 0-100 | 0-255 |
| Color Temp | 0-13 | 0-255 |
| V Keystone | 0-60 | 0-255 |
| H Keystone | 0-60 | 0-255 |

## Example Communication

Power on:
```
TX: PWR ON\r
RX: \r:
```

Query power state:
```
TX: PWR?\r
RX: PWR=01\r:
```

Set brightness to 50%:
```
TX: BRIGHT 128\r
RX: \r:
```

Error response:
```
TX: INVALID\r
RX: ERR\r:
```
