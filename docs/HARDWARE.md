# Hardware Setup

Connect your ESP32/ESP8266 to the projector's RS-232 port.

## Important

Projectors use RS-232 voltage levels (±12V). **You must use a level shifter** such as MAX3232 between the ESP and the projector. Connecting directly will damage your ESP.

## Basic Wiring

```
ESP32          RS-232 (DB-9)
─────          ─────────────
TX  ──────────── RX (Pin 2)
RX  ──────────── TX (Pin 3)
GND ──────────── GND (Pin 5)
```

## Wiring with MAX3232

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

## MAX3232 Module Pinout

Most MAX3232 modules have clearly labeled pins:

| Module Pin | Connect To |
|------------|------------|
| VCC | ESP 3.3V |
| GND | ESP GND + DB-9 Pin 5 |
| TX | ESP TX pin |
| RX | ESP RX pin |
| T1OUT | DB-9 Pin 2 |
| R1IN | DB-9 Pin 3 |

## RS-232 DB-9 Pinout

| Pin | Signal | Description |
|-----|--------|-------------|
| 2 | RX | Receive Data (from ESP) |
| 3 | TX | Transmit Data (to ESP) |
| 5 | GND | Ground |
| 7 | RTS | Request to Send (not used) |
| 8 | CTS | Clear to Send (not used) |

Only pins 2, 3, and 5 are needed for this component.

## UART Settings

```yaml
uart:
  tx_pin: GPIO17    # Adjust for your board
  rx_pin: GPIO16    # Adjust for your board
  baud_rate: 9600   # Required for ESC/VP21
```

The ESC/VP21 protocol requires:
- Baud rate: 9600
- Data bits: 8
- Stop bits: 1
- Parity: None

## Common ESP32 UART Pins

| Board | TX | RX |
|-------|----|----|
| ESP32 DevKit | GPIO17 | GPIO16 |
| ESP32-S2 | GPIO17 | GPIO18 |
| ESP32-S3 | GPIO17 | GPIO18 |
| ESP32-C3 | GPIO21 | GPIO20 |

## Troubleshooting

### No Communication

1. **Swap TX/RX** - Most common issue. TX on ESP connects to RX on projector
2. **Check baud rate** - Must be exactly 9600
3. **Verify level shifter** - Measure voltage on T1OUT, should swing ±12V
4. **Check cable** - Some DB-9 cables are null modem (swap TX/RX internally)

### Intermittent Communication

1. **Check connections** - Loose wires cause dropped bytes
2. **Add capacitors** - MAX3232 requires 0.1µF caps (usually on module)
3. **Shorten cable** - RS-232 works best under 15m
4. **Check ground** - Ensure common ground between ESP and MAX3232

### Projector Not Responding

1. **Enable RS-232** - Some projectors have RS-232 disabled in menu
2. **Check standby** - Some projectors don't respond in deep standby
3. **Try generic model** - Your projector may use different commands
