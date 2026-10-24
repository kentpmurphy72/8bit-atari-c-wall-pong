# Wall Pong — C / Atari 800XL

A one-player paddle-and-wall game for the Atari 800XL, programmed in **C** and
compiled to native 6502 machine code using **cc65**. Keep the ball alive with
your paddle on the left. The right side is a solid wall; there is no opponent.

This repository contains the C implementation. The
[Atari BASIC version](https://github.com/kentpmurphy72/8bit-atari-basic-wall-pong)
is maintained in a separate repository.

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

## Download the build tools

The game is built on a modern computer, then the resulting `.xex` file is
transferred to an Atari or loaded into an emulator.

| Tool | Download / official information | Purpose |
| --- | --- | --- |
| **cc65 2.19** | [cc65 website](https://cc65.github.io/) · [2.19 source release](https://github.com/cc65/cc65/releases/tag/V2.19) | Includes `cl65`, the C compiler/assembler/linker that produces the Atari executable, and `sim65` for the 6502 tests. |
| **Git** | [Git downloads](https://git-scm.com/downloads) | Clones this repository and downloads the pinned cc65 source during bootstrap. |
| **GNU Make** | [GNU Make](https://www.gnu.org/software/make/) | Runs the build commands in the Makefile. Install through your operating system's tools below. |
| **Host C compiler** | [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/) | Builds cc65 from source and runs the native game tests. Install through your operating system's tools below. |

You do **not** need to download cc65 separately if you use `make bootstrap`:
it downloads and builds the pinned 2.19 source automatically. Bootstrap also
requires a POSIX shell; Linux, macOS, and Windows with WSL provide one.

### Windows (recommended: WSL with Ubuntu)

Install [Windows Subsystem for Linux (WSL)](https://learn.microsoft.com/en-us/windows/wsl/install)
with Ubuntu, then open an **Ubuntu terminal** and run:

```sh
sudo apt update
sudo apt install git build-essential
```

Run the clone and build commands below inside that Ubuntu terminal. A native
Windows cc65 installation is another option, but this repository's bootstrap
script and Makefile are intended for a POSIX shell; these instructions use WSL.

### macOS

Install Apple's [Xcode Command Line Tools](https://developer.apple.com/xcode/resources/)
(which supply Git, Make, and Clang) by running:

```sh
xcode-select --install
```

Complete the installer, then open a terminal and use the build commands below.

### Debian / Ubuntu Linux

Install Git, GNU Make, and GCC through the distribution's package manager:

```sh
sudo apt update
sudo apt install git build-essential
```

Other Linux distributions provide equivalent Git, Make, and C compiler packages.

## Build

From a terminal with the tools above installed, download the game source:

```sh
git clone https://github.com/kentpmurphy72/8bit-atari-c-wall-pong.git
cd 8bit-atari-c-wall-pong
```

If you already cloned the repository, simply change into its directory instead.

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

For emulator downloads, see [Atari800](https://atari800.github.io/) or
[Altirra (Windows)](https://www.virtualdub.org/altirra.html). An emulator is
optional for building, but lets you play without physical Atari hardware.

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
