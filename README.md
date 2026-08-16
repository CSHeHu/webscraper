# Newscraper

Qt desktop app for reading news headlines from RSS feeds.

![Screenshot](docs/screenshot.png)

## Requirements

- Qt 6.8.1
- CMake 3.25+
- A C++23 compiler

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
./newscraper
```

## Install on Arch Linux

update PKGBUILD to match the latest version of newscraper

```bash
makepkg -si
```

The installed app reads its feed providers from `/etc/newscraper/config.json`.

## License

MIT. See [LICENSE](LICENSE).

