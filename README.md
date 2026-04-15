# Mouse Jiggler Xtra

A Flipper Zero USB mouse jiggler with selectable jiggle modes.

## Modes

| Mode | Movement | Description |
|------|----------|-------------|
| Simple | 2px x, 2px y | Subtle, nearly invisible jiggle |
| Short | 10px x, 3px y | Small diagonal movement |
| Short Horizontal | 10px x | Horizontal only |
| Short Vertical | 3px y | Vertical only |
| Long | 20px x, 5px y | Larger diagonal movement |
| Long Horizontal | 20px x | Wide horizontal sweep |
| Long Vertical | 5px y | Tall vertical sweep |
| Chaos | random, random | Full-range random movement at 20x speed |

## Building

Install [ufbt](https://github.com/flipperdevices/flipperzero-ufbt):

```bash
pipx install ufbt
```

Build the `.fap`:

```bash
cd mouse-jiggler-xtra
ufbt
```

The built file will be at `mouse-jiggler-xtra/dist/mouse_jiggler_xtra.fap`.

## Installing

With your Flipper connected via USB:

```bash
cd mouse-jiggler-xtra
ufbt launch
```

Or manually copy `mouse_jiggler_xtra.fap` to `apps/USB/` on the Flipper's SD card.

The app appears under **Apps -> USB -> Mouse Jiggler Xtra**.
