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
#include <math.h>

static semtech_loramac_t loramac;
static sx127x_t sx127x;
static hts221_t hts221;

static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = {0x00, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00};
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = {0x00, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00};
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = {0x00, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00};

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
        int status;
        uint16_t humidity;
        int16_t temperature;

        // Generate virtual data
        static double time = 0.0;
        double PI = 3.1415926;
        double daily_variation_amplitude = 5.0;
        double temperature_variation_day = daily_variation_amplitude * sin(2.0 * PI * time / (24.0 * 60.0));
        double temperature_variation_night = daily_variation_amplitude * sin(2.0 * PI * (time + 12.0 * 60.0) / (24.0 * 60.0));
        double temperature_noise = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        temperature_noise *= 2.0;

        static int16_t base_temperature_day = 210;
        static int16_t base_temperature_night = 150;
        int16_t base_temperature = (time < 12.0 * 60.0) ? base_temperature_day : base_temperature_night;
        temperature = base_temperature + (int16_t)((time < 12.0 * 60.0) ? temperature_variation_day : temperature_variation_night) + (int16_t)temperature_noise;

        double humidity_variation_amplitude = 300.0;
        double humidity_variation_day = humidity_variation_amplitude * sin(2.0 * PI * time / (24.0 * 60.0));
        double humidity_variation_night = humidity_variation_amplitude * sin(2.0 * PI * (time + 12.0 * 60.0) / (24.0 * 60.0));
        double humidity_noise = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        humidity_noise *= 2.0;

        static uint16_t base_humidity_day = 500;
        static uint16_t base_humidity_night = 900;
        uint16_t base_humidity = (time < 12.0 * 60.0) ? base_humidity_day : base_humidity_night;
        humidity = base_humidity + (uint16_t)((time < 12.0 * 60.0) ? humidity_variation_day : humidity_variation_night) + (uint16_t)humidity_noise;

        status = ((temperature > 250) || (humidity > 750)) ? 1 : 0;

        sprintf(message, "{\"id\": \"1\", \"temperature\": \"%u.%u\", \"humidity\": \"%u.%u\", \"status\": \"%d\"}",
                (temperature / 10), (temperature % 10), (humidity / 10), (humidity % 10), status);

        printf("Sending data: %s\n", message);

        uint8_t ret = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));

        if (ret != SEMTECH_LORAMAC_TX_DONE) {
            printf("Cannot send message, ret code: %d\n", ret);
        }

        xtimer_sleep(30);

        time += (24.0 * 60.0) / (5 * 60); 
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
