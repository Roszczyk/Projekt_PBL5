#include "water_lorawan.h"
#define LORAMAC_RECV_MSG_QUEUE (4U)

extern semtech_loramac_t loramac;
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

// static msg_t _loramac_recv_queue[LORAMAC_RECV_MSG_QUEUE];

int init_lorawan(void)
{
    /* Convert identifiers and keys strings to byte arrays */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    DEBUG("key %s\n\r", CONFIG_LORAMAC_APP_KEY_DEFAULT);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    return 0;
}

int join_lorawan(void)
{
    /* Join the network if not already joined */
    if (!semtech_loramac_is_mac_joined(&loramac))
    {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        DEBUG("Starting join procedure\n\r");

        // TODO obsłużyć błędy
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
        {
            DEBUG("Join procedure failed\n\r");
            return 1;
        }
        else
        {
            DEBUG("Join procedure succeded\n\r");
            return 0;
        }
    }
    else
    {
        DEBUG("Joined already!\n\r");
        return 0;
    }
}

uint8_t if_lorawan_joined(void)
{
    return (uint8_t)semtech_loramac_is_mac_joined(&loramac);
}

int send_lorawan_message(uint8_t *message, size_t len)
{
    uint8_t ret = semtech_loramac_send(&loramac, message, len);

    if (ret != SEMTECH_LORAMAC_TX_DONE) // TODO inne przypadki
    {
        DEBUG("Cannot send message '%s', ret code: %d\n", message, ret);
    }

    return ret;
}

// /**
//  * @brief   LoRaMAC return status
//  */
// enum {
//     SEMTECH_LORAMAC_JOIN_SUCCEEDED,             /**< Join procedure succeeded */
//     SEMTECH_LORAMAC_JOIN_FAILED,                /**< Join procedure failed */
//     SEMTECH_LORAMAC_NOT_JOINED,                 /**< MAC is not joined */
//     SEMTECH_LORAMAC_ALREADY_JOINED,             /**< MAC is already joined */
//     SEMTECH_LORAMAC_TX_OK,                      /**< Transmission is in progress */
//     SEMTECH_LORAMAC_TX_SCHEDULE,                /**< TX needs reschedule */
//     SEMTECH_LORAMAC_TX_DONE,                    /**< Transmission completed */
//     SEMTECH_LORAMAC_TX_CNF_FAILED,              /**< Confirmable transmission failed */
//     SEMTECH_LORAMAC_TX_ERROR,                   /**< Error in TX (invalid param, unknown service) */
//     SEMTECH_LORAMAC_RX_DATA,                    /**< Data received */
//     SEMTECH_LORAMAC_RX_LINK_CHECK,              /**< Link check info received */
//     SEMTECH_LORAMAC_RX_CONFIRMED,               /**< Confirmed ACK received */
//     SEMTECH_LORAMAC_BUSY,                       /**< Internal MAC is busy */
//     SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED        /**< Restricted access to channels */
// };

// void *receive_lorawan_message_thr(void *arg)
// {
//     msg_init_queue(_loramac_recv_queue, LORAMAC_RECV_MSG_QUEUE);

//     uint32_t *measure_delay_s_ptr = (uint32_t *)arg;
//     (void) measure_delay_s_ptr;

//     while (1) {
//         /* blocks until something is received */
//         switch (semtech_loramac_recv(&loramac)) {
//             case SEMTECH_LORAMAC_RX_DATA:
//                 loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
//                 printf("Data received: %s, port: %d, len %d\n\r",
//                 (char *)loramac.rx_data.payload, loramac.rx_data.port, loramac.rx_data.payload_len);

//                 if(loramac.rx_data.payload_len < 2)
//                 {
//                     printf("Wrong message length\n\r");
//                     break;
//                 }

//                 printf("Message in hex: ");

//                 for(int i = 0; i < loramac.rx_data.payload_len; i++)
//                 {
//                     printf(" %02x", loramac.rx_data.payload[i]);
//                 }

//                 printf("\n\r");

//                 struct DownlinkMessage downlink_message = decode_downlink(loramac.rx_data.payload, loramac.rx_data.payload_len);

//                 if(downlink_message.time < 10 || downlink_message.time > MAX_SLEEP_TIME)
//                 {
//                     printf("Wrong delay value\n");
//                     break;
//                 }

//                 *measure_delay_s_ptr = downlink_message.time;

//                 printf("New delay: %lu s\n\r", *measure_delay_s_ptr);

//                 break;

//             case SEMTECH_LORAMAC_RX_LINK_CHECK:
//                 printf("Link check information:\n"
//                    "  - Demodulation margin: %d\n"
//                    "  - Number of gateways: %d\n",
//                    loramac.link_chk.demod_margin,
//                    loramac.link_chk.nb_gateways);
//                 break;

//             case SEMTECH_LORAMAC_RX_CONFIRMED:
//                 puts("Received ACK from network");
//                 break;

//             case SEMTECH_LORAMAC_TX_SCHEDULE:
//                 puts("The Network Server has pending data");
//                 break;

//             default:
//                 break;
//         }
//     }
//     return NULL;
// }
