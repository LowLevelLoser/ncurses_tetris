# ncurses\_tetris

A simple **Tetris clone** written in C using the [ncurses](https://invisible-island.net/ncurses/) library that feels better than other terminal Tetris games I've tried.


## ⌨️ Controls

| Key       | Action                |
| --------- | --------------------- |
| **↑**     | Rotate piece          |
| **← / →** | Move left / right     |
| **↓**     | Soft drop (move down) |
| **Space** | Hard drop             |
| **C**     | Hold / switch piece   |
| **Esc**   | Pause                 |
| **R**     | Restart               |


## Dependencies

You need:

* A C compiler (GCC or Clang)
* The `ncurses` development headers

### Install on different distros

**Debian / Ubuntu:**

```bash
sudo apt install gcc make libncurses5-dev libncursesw5-dev
```

**Arch Linux:**

```bash
sudo pacman -S base-devel ncurses
```

**Fedora:**

```bash
sudo dnf install gcc make ncurses-devel
```

**Void Linux:**

```bash
sudo xbps-install -S base-devel ncurses
```

---

## Building

Clone the repo and run:

Clone and build:
```bash
git clone https://github.com/LowLevelLoser/ncurses_tetris
cd ncurses_tetris
make
```

This builds the executable `tetris`.

---

## Installing

To install globally (into `/usr/local/bin/tetris`):

```bash
sudo make install
```

To uninstall:

```bash
sudo make uninstall
```

You can change the prefix if you don’t want to install into `/usr/local`:

```bash
make PREFIX=$HOME/.local install
```

This will place `tetris` in `~/.local/bin`.

---

## Running

If installed system-wide:

```bash
tetris
```

If running directly from the repo:

```bash
./tetris
```

