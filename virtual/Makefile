# Name of your application
APPLICATION = lorawan-virtual

# Use the ST B-L072Z-LRWAN1 board by default:
BOARD ?= b-l072z-lrwan1

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
# DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
LORA_REGION = EU868

# Include the Semtech-loramac package
USEPKG += semtech-loramac

USEMODULE += $(DRIVER)
USEMODULE += xtimer
USEMODULE += hts221
USEMODULE += sx1276  # for a SX1276 radio device


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

include $(RIOTBASE)/Makefile.includeroot
