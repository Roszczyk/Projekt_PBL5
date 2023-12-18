#include "common.h"

void print_buffer(cayenne_lpp_t *lpp)
{
    for (uint8_t i = 0; i < lpp->cursor; ++i)
    {
        printf("%02X ", lpp->buffer[i]);
    }
    puts("");
}

void print_hex(uint8_t *buffer, uint8_t len)
{
    for (uint8_t i = 0; i < len; ++i)
    {
        printf("%02X ", buffer[i]);
    }
    puts("");
}