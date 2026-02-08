# Supported Models

The component includes configurations for many Epson projector models. Specify the model ID in lowercase with hyphen (e.g., `eh-tw7400`).

## Home Cinema Series

### EH-TW Entry Level

| Model | Features |
|-------|----------|
| EH-TW490, TW570, TW610, TW650 | Basic home cinema |
| EH-TW700, TW710, TW740, TW750 | Entry-level 1080p |

### EH-TW5xxx Series

| Model | Features |
|-------|----------|
| EH-TW5000, TW5100, TW5200, TW5210 | Full HD |
| EH-TW5300, TW5350, TW5400 | 3D support |
| EH-TW5600, TW5650, TW5700, TW5705 | HDR support |
| EH-TW5800, TW5820, TW5825 | Latest generation |

### EH-TW6xxx Series

| Model | Features |
|-------|----------|
| EH-TW6000, TW6100 | 3D projectors |
| EH-TW6600, TW6700, TW6800 | Premium 3D |

### EH-TW7xxx Series

| Model | Features |
|-------|----------|
| EH-TW7000, TW7100 | 4K PRO-UHD |
| EH-TW7200, TW7300 | 4K enhancement |
| EH-TW7400 | Premium 4K PRO-UHD |

### EH-TW8xxx Series

| Model | Features |
|-------|----------|
| EH-TW8000, TW8100 | 3D flagship |
| EH-TW8200, TW8300 | 4K enhancement |
| EH-TW8400 | Premium 4K |

### EH-TW9xxx Series (Flagship)

| Model | Features |
|-------|----------|
| EH-TW9000, TW9100 | Top-tier 3D |
| EH-TW9200, TW9300 | 4K enhancement |
| EH-TW9400 | Ultimate home cinema |

### EH-LS Laser Series

| Model | Features |
|-------|----------|
| EH-LS300 | Ultra-short throw |
| EH-LS500, LS650 | 4K laser |
| EH-LS800 | Gaming laser |
| EH-LS9000, LS10000, LS10500, LS11000, LS12000 | Reference-class laser |

## Business Series

### EB-S (SVGA)

S04, S05, S06, S31, S41

### EB-X (XGA)

X05, X06, X31, X41, X49, X50, X51

### EB-W (WXGA)

W05, W06, W31, W41, W42, W49, W50, W51, W52

### EB-U (WUXGA)

U32, U42, U50

### EB-E (Entry)

E01, E10, E20

### EB-FH (Full HD)

FH06, FH52

### EB-17xx/18xx (Portable)

1780W, 1781W, 1785W, 1795F

### EB-2xxx (Conference)

2040, 2055, 2065, 2140W, 2155W, 2165W, 2245U, 2247U, 2250U, 2255U, 2265U

### EB-9xx (Small Venue)

970, 972, 980W, 982W, 990U, 992F

### EB-L Laser (Business Laser)

L200F, L200W, L210W, L255F, L260F, L265F

## Generic Fallback

Use `model: "generic"` for any ESC/VP21 compatible projector not in the list. This enables all common features and input sources.

```yaml
epson_projector:
  id: projector
  model: "generic"
```

## Adding New Models

Model definitions are in `components/epson_projector/models.py`. Each model specifies:

- Available input sources
- Supported color modes
- Aspect ratio options
- Feature flags (3D, lens shift, etc.)

Contributions welcome via pull request.
