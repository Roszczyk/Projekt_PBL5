#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

#include "cayenne_lpp.h"

#define PAM_ERR (1)
#define PAM_OK (0)

#define ERR(format, ...)                                       \
    do                                                         \
    {                                                          \
        fprintf(stderr, "Error: " format "\n", ##__VA_ARGS__); \
        exit(EXIT_FAILURE);                                    \
    } while (0)

void print_buffer(cayenne_lpp_t *lpp);
void print_hex(uint8_t *buffer, uint8_t len);

#endif /* COMMON_H */