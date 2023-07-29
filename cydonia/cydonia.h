/**
 * @file cydonia.h
 * @author 0xca7 (m1etz@redacted.com)
 * @brief cydonia header file
 * @version 0.1
 * @date 2023-07-14
 */

/*****************************************************************************/

#ifndef CYDONIA_H
#define CYDONIA_H

/*****************************************************************************
 * INCLUDES
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/*****************************************************************************
 * MACROS
 */

/* @brief major version number */
#define VERSION_MAJOR   0
/* @brief minor version number */
#define VERSION_MINOR   1
/* @brief patch number */
#define VERSION_PATCH   0

/*****************************************************************************
 * DATASTRUCTURES
 */
typedef enum 
{
    CYDONIA_BITS8  = 8,
    CYDONIA_BITS16 = 16,
    CYDONIA_BITS32 = 32,
    CYDONIA_BITS64 = 64,
}
e_BITS_t;

/*****************************************************************************
 * GLOBALS 
 */

/*****************************************************************************
 * PUBLIC FUNCTIONS
 */

/*** GENERAL ***/

/**
 * @brief print cydonia version
 * @param void
 * @return void
 */
extern void
cydonia_print_version(void);

/*** FILE HANDLING ***/

/**
 * @brief read a BINARY file
 * @param[in] p_path the path of the file
 * @param[inout] p_fsize the filesize
 * @return a buffer with file contents, NULL on failure
 */
extern void *
cydonia_read_file(const char *p_path, size_t *p_fsize);

/**
 * @brief write a BINARY file
 * @param[in] p_path the path of the file
 * @param[in] p_buf buffer containing the data to be written
 * @param[in] siz the size of the buffer in bytes 
 * @return 0 on success, -1 on error
 */
extern int
cydonia_write_file(const char *p_path, const void *p_buf, size_t siz);

/*** HEXDUMP ***/


/**
 * @brief hexdump a buffer of `size` bytes, 16 bytes wide
 * @param p_buf the buffer to dump
 * @param siz the size of the buffer
 * @return void 
 */
extern void
cydonia_hexdump(const uint8_t *p_buf, size_t siz);

/**
 * @brief hexdump a buffer of `size` bytes, 16 bytes wide
 * @param p_path the path to write to
 * @param p_buf the buffer to dump
 * @param siz the size of the buffer
 * @return void 
 */
extern int
cydonia_hexdump_to_file(const char *p_path, const uint8_t *p_buf, size_t siz);

/**
 * @brief print a number as binary
 * @param number the number to print - cast to uint64_t
 * @param bits the number of bits @e_BITS_t
 * @return void
 */
extern void
cydonia_print_binary(uint64_t number, e_BITS_t bits);

/**
 * @brief print a number as binary, with bit labels
 * @param number the number to print - cast to uint64_t
 * @param bits the number of bits @e_BITS_t
 * @return void
 */
extern void
cydonia_print_binary_verbose(uint64_t number, e_BITS_t bits);

/*****************************************************************************/

#endif
