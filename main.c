#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/loramac.h"
#include "semtech_loramac.h"
#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"
#include "hts221.h"
#include "hts221_params.h"
#include "board.h"

static semtech_loramac_t loramac;
static sx127x_t sx127x;
static hts221_t hts221;

static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static void initialize(void) {
    if (hts221_init(&hts221, &hts221_params[0]) != HTS221_OK) {
        puts("Sensor initialization failed");
        return;
    }

    if (hts221_power_on(&hts221) != HTS221_OK) {
        puts("Sensor initialization power on failed");
        return;
    }

    if (hts221_set_rate(&hts221, hts221.p.rate) != HTS221_OK) {
        puts("Sensor continuous mode setup failed");
        return;
    }

    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;

    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    puts("Initialization succeeded");
}

static void sender(void) {
    while (1) {
        char message[200];

        uint16_t humidity = 0;
        int16_t temperature = 0;

        if (hts221_read_humidity(&hts221, &humidity) != HTS221_OK) {
            puts("Failed to read humidity!");
        }
        if (hts221_read_temperature(&hts221, &temperature) != HTS221_OK) {
            puts("Failed to read temperature!");
        }

        sprintf(message, "{\"id\": \"1\", \"temperature\": \"%u.%u\", \"humidity\": \"%u.%u\"}",
                (temperature / 10), (temperature % 10), (humidity / 10), (humidity % 10));

        printf("Sending data: %s\n", message);

        uint8_t ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));

        if (ret != SEMTECH_LORAMAC_TX_DONE) {
            printf("Cannot send message '%s', ret code: %d\n", message, ret);
        }

        
        xtimer_sleep(300);
    }
}

int main(void) {
    initialize();

    puts("Starting join procedure");

    semtech_loramac_set_dr(&loramac, 5);

    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }

    puts("Join procedure succeeded");

    sender();

    return 0;
}
