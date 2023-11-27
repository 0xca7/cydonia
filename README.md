# Cydonia

![imagenotfound](doc/logo_small.png "Cydonia")

This little library contains tools I often need when C programming. It's not for everyone, just a collection of utility functions in one header and one .c file for personal use.

Utilities currently integrated in Cydonia:

- [x] output numbers in binary
- [x] hexdump to screen and file
- [x] read and write files
- [x] TCP Server/Client
- [x] UDP Server/Client

open:
- [ ] conversion from hex buffer to hex string and vice versa

Testing
- [x] unit tests for functions
- [x] valgrind testing for allocations

## Usage

Just copy `cydonia/cydonia.c` and `cydonia/cydonia.h` and use them in your project. Documentation is included in the `cydonia/cydonia.h` header file.

Example Project:

```
Makefile
main.c
cydonia.c
cydonia.h
```

You can use this Makefile (`./examples/Makefile`):

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

## Networking

For networking, check out the functions 

```c
test_tcp_server_thread
test_tcp_client_thread
test_udp_server_thread
test_udp_client_thread
```

in `./tests/cydonia_tests/cydonia_ut.cpp`.

You have the option to implement your own handlers using

```c
/*  @brief implement a function to handle a new client for a server 
          or as a connection handler for a client. 
    @note expects a socket as an argument and a sockaddr_in (UDP) 
          expected to return -1 on error, and 0 on success
*/
typedef int (cydonia_net_handler_fnctn_t)(int, struct sockaddr_in*);
```

here are some examples (taken from the unit tests).

```c
/**
 * @brief the handler for a connected client 
 */
static int 
net_tcp_client_handler(int sockfd, struct sockaddr_in *p_addr)
{
    ssize_t bread = -1;
    uint8_t buf[32] = { 0x00 };
    printf("[+] client_handler started\n");

    bread = recv(sockfd, &buf[0], 32, 0);
    if(-1 == bread)
    {
        fprintf(stderr, "[!] recv: %s", strerror(errno));
        return -1;    
    }

    printf("[+] received buffer:\n");
    cydonia_hexdump(buf, 32);

    return 0;
}

/**
 * @brief the handler for a server
 */
static int 
net_tcp_handler(int sockfd, struct sockaddr_in *p_addr)
{
    ssize_t bwrite = -1;
    uint8_t buf[32] = { 0x00 };

    for(int i = 0; i < 32; i++)
    {
        buf[i] = (uint8_t)(i % 256);
    }
    printf("[+] connection handler started\n");

    bwrite = send(sockfd, &buf[0], 32, 0);
    if(-1 == bwrite)
    {
        fprintf(stderr, "[!] send: %s", strerror(errno));
        return -1;    
    }

    printf("[+] sent buffer:\n");
    cydonia_hexdump(buf, 32);

    return 0;
}

/**
 * @brief the handler for a connected client 
 */
static int 
net_udp_client_handler(int sockfd, struct sockaddr_in *p_addr)
{
    ssize_t bread = -1;
    uint8_t buf[32] = { 0x00 };
    socklen_t socklen = sizeof(struct sockaddr_in);
    printf("[+] client_handler started\n");

    for(int i = 0; i < 32; i++)
    {
        buf[i] = (uint8_t)(i % 256);
    }

    bread = sendto(sockfd, &buf[0], 32, 0, (struct sockaddr*)p_addr, 
        socklen);
    if(-1 == bread)
    {
        fprintf(stderr, "[!] sendto: %s", strerror(errno));
        return -1;    
    }

    printf("[+] received buffer:\n");
    cydonia_hexdump(buf, 32);

    return 0;
}

/**
 * @brief the handler for a server
 */
static int 
net_udp_handler(int sockfd, struct sockaddr_in *p_addr)
{
    ssize_t bwrite = -1;
    uint8_t buf[32] = { 0x00 };

    socklen_t socklen = sizeof(struct sockaddr_in);

    printf("[+] connection handler started\n");

    bwrite = recvfrom(sockfd, &buf[0], 32, 0, (struct sockaddr*)p_addr, 
        &socklen);
    if(-1 == bwrite)
    {
        fprintf(stderr, "[!] sendto: %s", strerror(errno));
        return -1;    
    }

    printf("[+] sent buffer:\n");
    cydonia_hexdump(buf, 32);

    return 0;
}
```


## Testing

For testing do you need `libcpputest` and `cmake` (I use ninja as a generator).

Compile and Run Tests:

```
mkdir build && cd build
cmake .. -GNinja -DTARGET_GROUP=test
ninja -v
ctest -VV
```

### 0xca7
