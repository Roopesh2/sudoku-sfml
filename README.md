## The Sudoku Game
For now, its just a simple sudoku game with minimal graphics, however I do plan to make it multiplayer/collaborative.

# Running on Linux
### Install dependencies
```bash
sudo apt update &&
sudo apt install \
	libxrandr-dev \
	libxcursor-dev \
	libudev-dev \
	libfreetype-dev \
	libflac-dev \
	libvorbis-dev \
	libgl1-mesa-dev \
	libegl1-mesa-dev \
	libfreetype-dev
```
### Building
```bash
cmake -B build
cmake --build build
```
### Run the binary
from project directory:
```
./build/bin/main
```
