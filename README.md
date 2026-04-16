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
| DVD | 4px x, 3px y @ 30fps | DVD logo style drift/bounce in a 720x480 area of the screen |
| Chaos Lite | random (-38 to 38) @ 6/sec | Moderate random movement |
| Chaos | random (-127 to 127) @ 20/sec | Full-range random movement |

## Demo

### Simple Modes
<img src="https://raw.githubusercontent.com/trwtsn/flipper-mouse-jiggler-xtra/refs/heads/master/mouse-jiggler-xtra/resources/gif/Demo_Simple_Modes.gif" width="200" alt="Simple Modes">

### DVD Mode
<img src="https://raw.githubusercontent.com/trwtsn/flipper-mouse-jiggler-xtra/refs/heads/master/mouse-jiggler-xtra/resources/gif/Demo_DVD.gif" width="200" alt="DVD Mode">

### Chaos Lite
<img src="https://raw.githubusercontent.com/trwtsn/flipper-mouse-jiggler-xtra/refs/heads/master/mouse-jiggler-xtra/resources/gif/Demo_Chaos_Lite.gif" width="200" alt="Chaos Lite">


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
