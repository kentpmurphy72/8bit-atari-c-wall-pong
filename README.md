# Wall Pong — Atari 800XL

A one-player paddle-and-wall game for the Atari 800XL. Keep the ball alive with
your paddle on the left. The right side is a solid wall; there is no opponent.

## Play

- Plug a joystick into **port 1**; move up/down to position the paddle.
- Press and release **fire** to serve. You have **three lives**.
- Each hit on the right wall earns one point (up to 999).
- Hitting different parts of the paddle changes the rebound angle. The ball
  speeds up gradually after paddle returns.
- After a miss, press fire to serve again. After game over, press fire to reset,
  then release and press again to serve.
- **Esc** exits to the loader/DOS.

The three digits at the top show points; the digit at the right shows lives.
The small center indicator lights green while waiting to serve, disappears
while playing, and turns orange at game over.

## Build

Requires `make`, a C compiler for native tests, and **cc65 2.19** for the Atari
binary. With Git and a C build toolchain installed:

```sh
make bootstrap     # downloads/builds pinned cc65 into ignored .tools/
make               # build/wall-pong.xex
make test          # native game logic tests
make test-6502     # same tests compiled for and executed on a simulated 6502
```

You can also use an existing cc65 installation: `make CL65=/path/to/cl65`.
Bootstrap verifies the expected source revision and keeps its build log in
`.tools/cc65-build.log`. No ROMs or proprietary assets are downloaded.

## Run on an Atari

Load `build/wall-pong.xex` as an Atari binary executable through your preferred
DOS loader or storage device (for example, SIO2SD or FujiNet). Disable BASIC
(hold **Option** during boot). This is an XEX, not a cartridge ROM or boot disk.

In Atari800 or Altirra, select an **800XL with 64 KB**, disable BASIC, map a
joystick to port 1, and load the XEX with the emulator's executable loader.
Supply your own legally obtained OS ROM where the emulator requires one.
Both PAL and NTSC use one update per vertical blank; NTSC plays somewhat faster.

## Implementation and validation

C compiled to native 6502 code, with a custom ANTIC mode D display (160×96,
four colors), OS vertical-blank timing, and POKEY bounce/miss sounds. The bitmap
is aligned to a 4 KB boundary and the display list to a 1 KB boundary to respect
ANTIC address wrapping. No external graphics or sound assets are required.

`src/game.c` contains platform-independent fixed-point physics. Tests cover
serving, top/bottom and right walls, scoring, paddle angles, misses, three-life
game over, restart button edges, paddle limits, and a 10,000-frame rally.
The Atari XEX builds, and those tests pass both natively and under sim65.
Full Atari display, audio, joystick input, and hardware timing still require
validation on an Atari emulator or real 800XL; sim65 only tests the game logic.
