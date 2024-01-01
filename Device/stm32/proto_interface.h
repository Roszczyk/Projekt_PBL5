#ifndef __PROTO_INTERFACE_H__
#define __PROTO_INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>


enum UplinkMessageField {
    WATER_LEVEL = 1,
    PH = 2,
};

enum DownlinkMessageField {
    TIME = 1,
};

struct UplinkMessage {
    uint32_t water_level;
    uint32_t ph;
};

struct DownlinkMessage {
    uint32_t time;
};



/**
 * @brief Encode uplink message to byte array
 * 
 * @param uplink Uplink message struct
 * @param output_len Output length of encoded message
 * @return uint8_t* encoded message
 */
uint8_t* encode_uplink(struct UplinkMessage *uplink, uint8_t *output_len);


/**
 * @brief Decode downlink message from byte array
 * 
 * @param message byte array
 * @param len length of byte array
 * @return struct DownlinkMessage decoded message
 */
struct DownlinkMessage decode_downlink(uint8_t *message, uint8_t len);

#endif