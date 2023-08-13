/**
 * @file cydonia.c
 * @author 0xca7 (m1etz@redacted.com)
 * @brief cydonia c file
 * @version 0.1
 * @date 2022-12-28
 */

/*****************************************************************************/

/*****************************************************************************
 * INCLUDES
 */
#include "cydonia.h"


/*****************************************************************************
 * GLOBALS 
 */


/*****************************************************************************
 * MACROS
 */
#define CHECK_ASCII(c) ( (c >= 0x20 && c <= 0x7e) ? c : '.' )


/*****************************************************************************
 * DATASTRUCTURES
 */


/*****************************************************************************
 * PRIVATE FUNCTION PROTOTYPES 
 */

/**
 * @brief internal hexdump function
 * @param p_buf the buffer to hexdump
 * @param siz the size of the buffer in bytes
 * @param fp the filepointer to write to (stdout / file)
 * @return 0 on success, -1 on failure
 */
static int
_cydonia_hexdump(const uint8_t *p_buf, size_t siz, FILE **fp);


/*****************************************************************************
 * PRIVATE FUNCTIONS
 */

/*** Inline Functions ***/

/**
 * @brief convenience function to check if a pointer is NULL. if this happens
 *        I want to see a message on the screen.
 * @param p the pointer to check
 * @return 0 if not null, 1 if null
 */
static inline int
CHECK_NULL(const void *p) 
{
    if(!p) fprintf(stderr, "[cydonia] NULL pointer encountered\n");
    return p == NULL;
}


/**
 * @brief print an error message
 * @param p_msg the message to print
 * @param p_fctn optional parameter - if this is not NULL the strerror will
 *               be printed. p_fctn should contain the failed function.
 *               example: cydonia_print_error("failed", "access");
 *
 */
static inline void
cydonia_print_error(const char *p_msg, const char *p_fctn)
{
    if(p_msg) fprintf(stderr, "[cydonia error] %s\n", p_msg);
    if(p_fctn) fprintf(stderr, "[cydonia error] %s: %s\n", p_fctn, 
        strerror(errno));
}


static int
_cydonia_hexdump(const uint8_t *p_buf, size_t siz, FILE **fp)
{
    if( CHECK_NULL(p_buf) || CHECK_NULL(fp) || CHECK_NULL(*fp) )
    {
        return -1;
    }

    size_t i = 0;
    /* get the number of lines */
    size_t lines = siz / 16;
    /* is there a rest? */
    size_t rest = siz % 16;

    for(i = 0; i < lines*16; i += 16)
    {
        fprintf(*fp, "%08lx %02x %02x %02x %02x %02x %02x %02x %02x %02x \
%02x %02x %02x %02x %02x %02x %02x", 
        i,
        p_buf[i+0], p_buf[i+1], p_buf[i+2], p_buf[i+3], 
        p_buf[i+4], p_buf[i+5], p_buf[i+6], p_buf[i+7], 
        p_buf[i+8], p_buf[i+9], p_buf[i+10], p_buf[i+11], 
        p_buf[i+12], p_buf[i+13], p_buf[i+14], p_buf[i+15]);

        fprintf(*fp, " | ");

        fprintf(*fp, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
            CHECK_ASCII(p_buf[i+0]), CHECK_ASCII(p_buf[i+1]), 
            CHECK_ASCII(p_buf[i+2]), CHECK_ASCII(p_buf[i+3]), 
            CHECK_ASCII(p_buf[i+4]), CHECK_ASCII(p_buf[i+5]), 
            CHECK_ASCII(p_buf[i+6]), CHECK_ASCII(p_buf[i+7]), 
            CHECK_ASCII(p_buf[i+8]), CHECK_ASCII(p_buf[i+9]), 
            CHECK_ASCII(p_buf[i+10]), CHECK_ASCII(p_buf[i+11]), 
            CHECK_ASCII(p_buf[i+12]), CHECK_ASCII(p_buf[i+13]), 
            CHECK_ASCII(p_buf[i+14]), CHECK_ASCII(p_buf[i+15])
        );
    }

    if(rest)
    {
        fprintf(*fp, "%08lx ", lines*16);
        for(i = 0; i < 16; i++)
        {
            if(i <= rest)
            {
                fprintf(*fp, "%02x ", p_buf[lines * 16 + i]);
            }   
            else
            {
                fprintf(*fp, "   ");
            }
        }

        fprintf(*fp, "| ");
        for(i = 0; i < rest; i++)
        {
            fprintf(*fp, "%c", CHECK_ASCII(p_buf[lines * 16 + i]));
        }
        fprintf(*fp, "\n");
    } /* if rest */

    return 0;
}


/*****************************************************************************
 * PUBLIC FUNCTIONS
 */

void
cydonia_print_version(void)
{
    printf("cydonia v%d.%d.%d\n", 
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

void *
cydonia_read_file(const char *p_path, size_t *p_fsize)
{
    void *p = NULL;
    FILE *fp = NULL;
    long fsize = -1;
    size_t bread = 0;

    if( CHECK_NULL(p_path) || CHECK_NULL(p_fsize) )
    {
        goto LABEL_FAILURE;
    }

    if(access(p_path, R_OK))
    {
        cydonia_print_error(NULL, "access");
        goto LABEL_FAILURE;
    }
    
    fp = fopen(p_path, "rb");
    if(!fp)
    {
        cydonia_print_error("error opening file", 
            "fopen");
        goto LABEL_FAILURE;
    }

    if(-1 == fseek(fp, 0L, SEEK_END))
    {
        cydonia_print_error(NULL, 
            "fseek");
        goto LABEL_FAILURE;
    }
    
    fsize = ftell(fp);
    if(-1 == fsize)
    {
        cydonia_print_error(NULL, 
            "ftell");
        goto LABEL_FAILURE;
    }

    rewind(fp);

    p = malloc(fsize);
    if(!p)
    {
        cydonia_print_error("malloc for file failed", 
            "malloc");
        goto LABEL_FAILURE;
    }

    bread = fread(p, 1, fsize, fp);
    if(bread != (size_t)fsize)
    {
        cydonia_print_error(NULL, 
            "fread");
        goto LABEL_FAILURE;
    }

    *p_fsize = (size_t)fsize;
    
LABEL_FAILURE:

    if(fp)
    {
        fclose(fp);
    }

    return p;
}


int
cydonia_write_file(const char *p_path, const void *p_buf, size_t siz)
{
    int ret = -1;
    FILE *fp = NULL;
    size_t bwrite = 0;

    if( CHECK_NULL(p_path) || CHECK_NULL(p_buf) )
    {
        goto LABEL_FAILURE;
    }

    fp = fopen(p_path, "wb");
    if(!fp)
    {
        cydonia_print_error("error opening file", 
            "fopen");
        goto LABEL_FAILURE;
    }

    bwrite = fwrite(p_buf, 1, siz, fp);
    if(bwrite != siz)
    {
        cydonia_print_error(NULL, 
            "fwrite");
        goto LABEL_FAILURE;
    }

    ret = 0;

LABEL_FAILURE:
    if(fp)
    {
        fclose(fp);
    }
    return ret;
}


void
cydonia_hexdump(const uint8_t *p_buf, size_t siz)
{
    int ret = _cydonia_hexdump(p_buf, siz, &stdout);
    (void)ret;
}


int
cydonia_hexdump_to_file(const char *p_path, const uint8_t *p_buf, size_t siz) 
{
    int ret = -1;
    FILE *fp = NULL;

    fp = fopen(p_path, "wb");
    if(!fp)
    {
        cydonia_print_error("error opening dump file",
            "fopen");
        goto LABEL_FAILURE;
    }

    ret = _cydonia_hexdump(p_buf, siz, &fp);

LABEL_FAILURE:

    if(fp)
    {
        fclose(fp);
    }

    return ret;
}

void
cydonia_print_binary(uint64_t number, e_BITS_t bits)
{
    int i = 0;

    printf("b");
    for(i = bits-1; i >= 0; i--)
    {
        printf("%ld", (number >> i) & 1);
    }
    printf("\n");
}

void
cydonia_print_binary_verbose(uint64_t number, e_BITS_t bits)
{
    int i = 0;

    for(i = bits-1; i >= 0; i--)
    {
        printf("%ld  ", (number >> i) & 1);
    }

    printf("\n");

    for(i = bits-1; i >= 0; i--)
    {
        printf("---");
    }

    printf("\n");

    for(i = bits-1; i >= 0; i--)
    {
        printf("%d ", i);
        if(i <= 9)
        {
            printf(" ");
        }
    }

    printf("\n\n");
}

int
cydonia_tcp_server(const char *ip, uint16_t port, 
    cydonia_tcp_handler_t *p_handler)
{
    int ret = -1;
    int sock = -1;
    int clientsock = -1;
    struct sockaddr_in addr = {0};
    struct sockaddr_in peer = {0};
    socklen_t addrlen = 0;
    const int enable = 1;

    if(CHECK_NULL((const void*)p_handler))
    {
        goto TCPSERVER_FAILURE;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sock)
    {
        cydonia_print_error("(tcp server) error creating socket", "socket");
        goto TCPSERVER_FAILURE;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(ip)
    {
        addr.sin_addr.s_addr = inet_addr(ip);
        if(-1 == addr.sin_addr.s_addr)
        {
            cydonia_print_error("(tcp_server) ip address invalid", "inet_addr");
            goto TCPSERVER_FAILURE;
        }
    } 
    else 
    {
        addr.sin_addr.s_addr = INADDR_ANY;
    }

    /* get rid of "address already in use" */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        cydonia_print_error("(tcp_server) error setsockopt", "setsockopt");
        goto TCPSERVER_FAILURE;
    }

    ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(-1 == ret)
    {
        cydonia_print_error("(tcp_server) error binding socket", "bind");
        goto TCPSERVER_FAILURE;
    }

    /* TODO: backlog as macro? */
    ret = listen(sock, 10);
    if(-1 == ret)
    {
        cydonia_print_error("(tcp_server) error listen", "listen");
        goto TCPSERVER_FAILURE;
    }

    clientsock = accept(sock, (struct sockaddr*)&peer, &addrlen);
    if(-1 == clientsock)
    {
        cydonia_print_error("(tcp_server) error accept", "accept");
        goto TCPSERVER_FAILURE;
    }

    ret = p_handler(clientsock);

TCPSERVER_FAILURE:
    close(sock); /* TODO: handle error? */
    return ret;
}

extern int
cydonia_tcp_client(const char *ip, uint16_t port, 
    cydonia_tcp_handler_t *p_handler)
{
    int ret = -1;
    int sock = -1;
    struct sockaddr_in addr = {0};

    if(CHECK_NULL((const void*)p_handler))
    {
        goto TCPLCIENT_FAILURE;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sock)
    {
        cydonia_print_error("(tcp_client) error creating socket", "socket");
        goto TCPLCIENT_FAILURE;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if(ip)
    {
        addr.sin_addr.s_addr = inet_addr(ip);
        if(-1 == addr.sin_addr.s_addr)
        {
            cydonia_print_error("(tcp_client) ip address invalid", "inet_addr");
            goto TCPLCIENT_FAILURE;
        }
    } 

    ret = connect(sock, (const struct sockaddr*)&addr, 
        sizeof(struct sockaddr_in));
    if(-1 == ret) 
    {
        cydonia_print_error("(tcp_client) connect failed", "connect");
        goto TCPLCIENT_FAILURE;
    }

    ret = p_handler(sock);

TCPLCIENT_FAILURE:
    close(sock); /* TODO: handle error? */
    return ret;

}

/*****************************************************************************/





