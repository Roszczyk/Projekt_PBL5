#ifndef __WATER_LORAWAN_H__
#define __WATER_LORAWAN_H__

#include "net/loramac.h"
#include "semtech_loramac.h"
#include "fmt.h"

#include "proto_interface.h"

#define ENABLE_DEBUG (1)
#if ENABLE_DEBUG
#include "debug.h"
#endif

#define MAX_SLEEP_TIME 86400

// opis TODO + typ

/**
 * @brief Init LoRaWAN settings (join should be called after)
 *
 * @return 0 if success
 */
int init_lorawan(void);

/**
 * @brief Join LoRaWAN (init should be called before)
 *
 * @return 0 if joined, 1 if error
 */
int join_lorawan(void);

/**
 * @brief Check if joined to LoRaWAN
 *
 * @return 0 if not joined, 1 if joined
 */
uint8_t if_lorawan_joined(void);

/**
 * @brief Send lorawan message
 *
 * @param message ptr to array of uint8_t values to send
 * @param len length of message array
 * @return int
 */
int send_lorawan_message(uint8_t *message, size_t len);

// void *receive_lorawan_message_thr(void *arg);

#endif