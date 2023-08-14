#include <CppUTest/TestHarness.h>
#include "CppUTest/TestHarness_c.h"
#include <CppUTestExt/MockSupport.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

extern "C"
{
    #include "../../cydonia/cydonia.c"
}

TEST_GROUP(Cydonia)
{
    void setup() {
    }

    void teardown() {
    }

};


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

TEST(Cydonia, cydonia_read_write_file) {
    
    const char *buffer = "test";

    void *p = NULL;
    size_t size = 0;
    p = cydonia_read_file("/tmp/thisdoesnotexist", &size);
    CHECK(p == NULL);

    CHECK(!cydonia_write_file("/tmp/cydonia_testfile", (const void*)buffer, 4));

    p = cydonia_read_file("/tmp/cydonia_testfile", &size);
    CHECK(p != NULL);
    CHECK(!memcmp(p, "test", 4));

    free(p);
}

TEST(Cydonia, cydonia_hexdump) {
    
    usleep(1000);

    const uint8_t bytes0[] = {
        0xcf,0xee,0x46,0x70,0x81,0x4b,0x1c,0x44,
        0xb7,0x2f,0x3c,0x02,0x70,0xc5,0x86,0xae,
        0xdc,0xff,0xd1,0x67,0xef,0x49,0x68,0xe7,
        0xe0,0x1c,0x5e,0xfc,0x01,0x6d,0x67,0x48,
        0x7e,0x6a,0x88,0x76,0x4a,0x9e,0x51,0x05,
        0xc1,0x7f,0x49,0x95,0xad,0x94,0x89,0x87,
        0x00,0x35,0xaf,0xd9,0x57,0x5f,0x25,0x38,
        0xc1,0xcf,0x06,0xbb,0x78,0x46,0xd2,0xbd,
        0x40,0x69
    };

    const uint8_t bytes1[] = {
        0xcf,0xee,0x46
    };

    const uint8_t bytes2[] = {
        0xcf,0xee,0x46,0x70,0x81,0x4b,0x1c,0x44,
        0xb7,0x2f,0x3c,0x02,0x70,0xc5,0x86,0xae
    };

    cydonia_hexdump(bytes0, sizeof(bytes0));
    cydonia_hexdump(bytes1, sizeof(bytes1));
    cydonia_hexdump(bytes2, sizeof(bytes2));

    cydonia_hexdump(NULL, sizeof(bytes2));
    cydonia_hexdump_to_file("/tmp/testfile", bytes0, sizeof(bytes0));

}

TEST(Cydonia, cydonia_printbinary) {

    uint8_t  num0  = 0x55;
    uint16_t num1 = 0x55aa;
    uint32_t num2 = 0xdeadbeef;
    uint64_t num3 = 0xcafebabebaadf00d;
    cydonia_print_binary(num0, CYDONIA_BITS8);
    cydonia_print_binary(num1, CYDONIA_BITS16);
    cydonia_print_binary(num2, CYDONIA_BITS32);
    cydonia_print_binary(num3, CYDONIA_BITS64);

    cydonia_print_binary_verbose(num0, CYDONIA_BITS8);
    cydonia_print_binary_verbose(num1, CYDONIA_BITS16);
    cydonia_print_binary_verbose(num2, CYDONIA_BITS32);
    cydonia_print_binary_verbose(num3, CYDONIA_BITS64);

}

static void*
test_tcp_server_thread(void *args)
{
    printf("[+] TEST TCP SERVER\n");
    cydonia_net_handler_fnctn_t *p_handler = NULL;
    p_handler = net_tcp_client_handler;
    CHECK( cydonia_net_server(CYDONIA_NET_TYPE_TCP, "127.0.0.1", 
        7777, p_handler) == 0 );
    return NULL;
}

static void*
test_tcp_client_thread(void *args)
{
    printf("[+] TEST TCP CLIENT\n");
    sleep(1); /* server must be able to start listening */
    cydonia_net_handler_fnctn_t *p_handler = NULL;
    p_handler = net_tcp_handler;
    CHECK( cydonia_net_client(CYDONIA_NET_TYPE_TCP, "127.0.0.1", 
        7777, p_handler) == 0 );
    return NULL;
}

static void*
test_udp_server_thread(void *args)
{
    printf("[+] TEST UDP SERVER\n");
    cydonia_net_handler_fnctn_t *p_handler = NULL;
    p_handler = net_udp_handler;
    CHECK( cydonia_net_server(CYDONIA_NET_TYPE_UDP, "127.0.0.1", 
        8888, p_handler) == 0 );
    return NULL;
}
static void*
test_udp_client_thread(void *args)
{
    printf("[+] TEST UDP CLIENT\n");
    sleep(1); /* server must be able to start listening */
    cydonia_net_handler_fnctn_t *p_handler = NULL;
    p_handler = net_udp_client_handler;
    CHECK( cydonia_net_client(CYDONIA_NET_TYPE_UDP, "127.0.0.1", 
        8888, p_handler) == 0 );
    return NULL;
}

TEST(Cydonia, cydona_net) {

    int ret = -1;
    pthread_t server_thread = 0;
    pthread_t client_thread = 0;

    cydonia_net_handler_fnctn_t *p_handler = NULL;
    p_handler = net_tcp_client_handler;

    CHECK( cydonia_net_server(CYDONIA_NET_TYPE_TCP, "", 7777, NULL) == -1 );
    CHECK( cydonia_net_server(CYDONIA_NET_TYPE_UDP, "", 7777, p_handler) == -1 );

    CHECK( cydonia_net_client(CYDONIA_NET_TYPE_TCP, "", 7777, NULL) == -1 );
    CHECK( cydonia_net_client(CYDONIA_NET_TYPE_UDP, "", 7777, p_handler) == -1 );

    ret = pthread_create(&server_thread, NULL, test_tcp_server_thread, NULL);
    ret = pthread_create(&client_thread, NULL, test_tcp_client_thread, NULL);

    CHECK( pthread_join(server_thread, NULL) == 0 );
    CHECK( pthread_join(client_thread, NULL) == 0 );

    ret = pthread_create(&server_thread, NULL, test_udp_server_thread, NULL);
    ret = pthread_create(&client_thread, NULL, test_udp_client_thread, NULL);

    CHECK( pthread_join(server_thread, NULL) == 0 );
    CHECK( pthread_join(client_thread, NULL) == 0 );

}