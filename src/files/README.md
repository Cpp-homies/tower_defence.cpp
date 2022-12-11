## Instruction for creating waves in waves.txt

Format for one enemy type(spaces between)(ends with semicolon):
[number of enemies to spawn] [the corresponding number of an enemy to spawn] [delay between spawns (ms)];

Enemies are numbered as follows:
	*Compiler Errors:
		-syntax_error = 1
		-exception = 2
	*Memory Errors:
		-invalid_read = 3
		-ivalid_write = 4
		-x_bytes_lost = 5
		-mismatched = 6
	*Runtime Errors:
		-stack_overflow = 7


Wave definition are separated with a newline.

The game has 20 levels, all must be defined.

---

## Instructions for creating maps in map.tdmap

The first two numbers represent the dimensions of the map. Then, on a new line, a matrix of characters representing the tiles of the map are separated by either newlines or spaces.

Character legend:
- 1: Normal square, on which towers can be built.
- 2: Path tile, on which comment towers can be built. (These are only intermediate paths, that is, neither start nor end points of the path.)
- A: Starting point for enemies, on which no towers of any kind can be built.
- B: Ending point for enemies, on which no towers of any kind can be built.

For example:
11 11
1 1 1 1 1 1 1 1 1 1 1
1 2 2 2 2 2 2 2 2 2 1
1 2 1 1 1 1 1 1 1 2 1
1 2 1 1 1 1 1 1 1 2 1
1 2 1 1 1 1 1 1 1 2 1
1 2 1 1 2 2 2 2 2 2 1
1 2 2 2 2 1 1 1 1 2 1
1 1 1 1 2 1 1 1 1 2 1
1 A 2 2 2 1 1 1 1 2 1
1 1 1 1 1 1 1 1 1 B 1
1 1 1 1 1 1 1 1 1 1 1

Disclaimer: The game is only confirmed to work with 11x11 maps.