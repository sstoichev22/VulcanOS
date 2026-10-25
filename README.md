# VulcanOS

A tiny operating system for the wide Intel 8080, written in C and assembly: bootstrap, a console, keyboard and mouse drivers, graphics, and a heap. It runs on the display-capable machine in [Vulcan IDE](../vulcanide).

## Run

Open `os/build.run` in the IDE and press Run (or run it from the MCP server). The `vos` preset boots to a text console with a blinking cursor.

## Layout

- `os/` - boot, kernel, drivers, memory manager, `build.run` manifest
- `stdlib/` - the C standard library headers and sources that [c8080](../c8080) and the IDE bundle
- `docs/architecture.md` - how the pieces fit together
