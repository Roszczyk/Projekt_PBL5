#include "proto_interface.h"

uint8_t* encode_uplink(struct UplinkMessage *uplink, uint8_t *output_len)
{
    uint8_t len = 0;
    uint8_t *buffer = malloc(20);

    buffer[len] = WATER_LEVEL << 3;
    len++;

    uint32_t water_level = uplink->water_level;
    do
    {
        buffer[len] = water_level % 128;
        water_level >>= 7;

        if (water_level > 0)
        {
            buffer[len] |= 0x80;
        }

        len++;
    } while (water_level > 0);

    buffer[len] = PH << 3;
    len++;

    uint32_t ph = uplink->ph;
    do
    {
        buffer[len] = ph % 128;
        ph >>= 7;

        if (ph > 0)
        {
            buffer[len] |= 0x80;
        }

        len++;
    } while (ph > 0);

    *output_len = len;

    return buffer;
}


struct DownlinkMessage decode_downlink(uint8_t *message, uint8_t len)
{
    struct DownlinkMessage downlink = {0};

    uint8_t i = 0;
    while (i < len)
    {
        uint8_t field = message[i] >> 3;
        i++;

        uint32_t value = 0;
        uint8_t shift = 0;
        while (message[i] & 0x80)
        {
            value |= (message[i] & 0x7F) << shift;
            shift += 7;
            i++;
        }
        value |= message[i] << shift;
        i++;

        switch (field)
        {
        case TIME:
            downlink.time = value;
            break;
        default:
            break;
        }
    }

    return downlink;
}