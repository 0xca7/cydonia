# Cydonia

![imagenotfound](doc/logo_small.png "Cydonia")

This little library contains tools I often need when C programming. It's not for everyone, just a collection of utility functions in one header and one .c file for personal use.

Utilities currently integrated in Cydonia:

- [x] output numbers in binary
- [x] hexdump to screen and file
- [x] read and write files
- [ ] TCP/UDP Server/Client

## Usage

Just copy `cydonia/cydonia.c` and `cydonia/cydonia.h` and use them in your project. Documentation is included in the `cydonia/cydonia.h` header file.

Example Project:

```
Makefile
main.c
cydonia.c
cydonia.h
```

You can use this Makefile:

```
src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = #-lGL -lglut -lpng -lz -lm
CFLAGS = -Wall -pedantic -g

cydonia_example: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) cydonia_example
```

... and a `main.c`

```c
#include <stdio.h>
#include "cydonia.h"

int
main(int argc, char *argv[])
{
    cydonia_print_version();
    return 0;
}
```


## Testing

For testing do you need `libcpputest` and `cmake` (I use ninja as a generator).

Compile and Run Tests:

```
mkdir build && cd build
cmake -GNinja ../CMakeLists.txt
# or cmake ../CMakeLists.txt && make
ninja -v
ctest -VV
```

### 0xca7
