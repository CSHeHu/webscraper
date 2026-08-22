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

## Configuration

Feeds come from `/etc/newscraper/config.json` when installed, or `config/config.json`
next to the binary when running from `build/`.

It is a JSON array of providers. `name` shows in the Provider menu, `url` is the feed,
and the eight `*Begin`/`*End` keys are the literal substrings each field is sliced out
with — copy an existing entry and adjust. All ten keys are required.

## License

MIT. See [LICENSE](LICENSE).

