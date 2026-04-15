# Mouse Jiggler Xtra

A Flipper Zero USB mouse jiggler with selectable jiggle modes.

## Modes

| Mode | Movement | Description |
|------|----------|-------------|
| Simple | 2px x, 2px y | Subtle, nearly invisible jiggle |
| Short | 20px x, 20px y | Small diagonal movement |
| Short Horizontal | 50px x | Horizontal only |
| Short Vertical | 50px y | Vertical only |
| Long | 200px x, 80px y | Larger diagonal movement |
| Long Horizontal | 200px x | Wide horizontal sweep |
| Long Vertical | 200px y | Tall vertical sweep |
| DVD | 3px x, 2px y @ 30fps | Bouncing DVD logo in a 720x480 virtual box |
| Chaos Lite | random (-38 to 38) @ 6/sec | Moderate random movement |
| Chaos | random (-127 to 127) @ 20/sec | Full-range random movement |

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
