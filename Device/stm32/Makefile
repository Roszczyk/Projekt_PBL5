# name of your application
APPLICATION = JIN_lorawan

# Use the ST B-L072Z-LRWAN1 board by default:
BOARD ?= b-l072z-lrwan1

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/pi/RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Change this to abp to enable Activation By Personnalization mode
#ACTIVATION_MODE ?= otaa

#ifeq (otaa,$(ACTIVATION_MODE))
  DEVEUI ?= 70B3D57ED0063437
  APPEUI ?= 0101010101010101
  APPKEY ?= 2B7E151628AED2A6ABF7158809CF4F3C
#else ifeq (abp,$(ACTIVATION_MODE))
#  DEVADDR ?= 260B4CC4
#  NWKSKEY ?= 861F2658D649EEF6C693722DF9FB3567
#  APPSKEY ?= BB262A2919AFCCDCABF01A3212B5F417
#  RX2_FREQ ?= 869525000
#  RX2_DR ?= 3
#else
#  $(error Unsupported activation mode '$(ACTIVATION_MODE)')
#endif

# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
LORA_REGION ?= EU868

# Send a message every 20s after joining the network
#SEND_PERIOD_S ?= 20

# Pass these enviroment variables to docker
#DOCKER_ENV_VARS += DEVEUI
#DOCKER_ENV_VARS += APPEUI
#DOCKER_ENV_VARS += APPKEY

# Include the Semtech-loramac package
USEPKG += semtech-loramac
USEMODULE += auto_init_loramac

USEMODULE += $(DRIVER)
USEMODULE += fmt
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += ps
USEMODULE += srf04
FEATURES_REQUIRED += periph_gpio_irq 
USEMODULE += semtech_loramac_rx
#FEATURES_OPTIONAL += periph_rtc

# Uncomment the following line to enable Loramac stack state persistence on EEPROM.
# Make sure the EEPROM is erased before enabling this and when flashing a board
# with an EEPROM the first time. If the board already contains a previous Loramac state
# in its EEPROM that is not corresponding to your LoRaWAN application settings,
# joining a network will fail.
# FEATURES_OPTIONAL += periph_eeprom

#CFLAGS += -DSEND_PERIOD_S=$(SEND_PERIOD_S)
#ifeq (otaa,$(ACTIVATION_MODE))
#  CFLAGS += -DUSE_OTAA
#else ifeq (abp,$(ACTIVATION_MODE))
#  CFLAGS += -DUSE_ABP
#endif

# Default IotLab Config to run the test
#ifneq (,$(filter iotlab%,$(MAKECMDGOALS)))
#  IOTLAB_NODES ?= 1
#  IOTLAB_TYPE  ?= st-lrwan1:sx1276
#  IOTLAB_SITE  ?= saclay
#  include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab
#endif

#ifneq (,$(filter test,$(MAKECMDGOALS)))
#  DEFAULT_MODULE += test_utils_interactive_sync
#endif

CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
    
include $(RIOTBASE)/Makefile.include

#ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
#  ifeq (otaa,$(ACTIVATION_MODE))
    # OTAA compile time configuration keys
#    CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
#    CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
#    CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
#  else ifeq (abp,$(ACTIVATION_MODE))
#    CFLAGS += -DCONFIG_LORAMAC_DEV_ADDR_DEFAULT=\"$(DEVADDR)\"
#    CFLAGS += -DCONFIG_LORAMAC_APP_SKEY_DEFAULT=\"$(APPSKEY)\"
#    CFLAGS += -DCONFIG_LORAMAC_NWK_SKEY_DEFAULT=\"$(NWKSKEY)\"
#    CFLAGS += -DCONFIG_LORAMAC_DEFAULT_RX2_FREQ=$(RX2_FREQ)
#    CFLAGS += -DCONFIG_LORAMAC_DEFAULT_RX2_DR=$(RX2_DR)
#  endif
#endif
