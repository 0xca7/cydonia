#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "../../cydonia/cydonia.h"

int
main(int argc, char *argv[])
{
    system("dd if=/dev/urandom of=/tmp/testfile bs=1 count=256");

    void *p = NULL;
    size_t fsize = 0;

    p = cydonia_read_file("/tmp/testfile", &fsize);
    assert(p != NULL);

    printf("[+] read a file of size %ld to %p\n", fsize, p);

    free(p);
    system("rm /tmp/testfile");

    return 0;
}