#include <esp_system.h>
#include <esp_timer.h>
#include <tinyusb.h>
#include <tinyusb_default_config.h>

#include <joybus/joybus.h>
#include <joybus/backend/esp32.h>

#include "config.h"
#include "gcca.h"
#include "usb_descriptors.h"

// Joybus data-line GPIO for each channel (0-3)
// Configure via `idf.py menuconfig` or sdkconfig.defaults
static const int JOYBUS_GPIOS[GCCA_JOYBUS_CHANNELS] = {
  CONFIG_GCCA_JOYBUS_GPIO_CH0,
  CONFIG_GCCA_JOYBUS_GPIO_CH1,
  CONFIG_GCCA_JOYBUS_GPIO_CH2,
  CONFIG_GCCA_JOYBUS_GPIO_CH3,
};

// Joybus backend instances
static struct joybus_esp32 esp32_buses[GCCA_JOYBUS_CHANNELS];

// esp_timer trampoline
static void poll_timer_cb(void *arg)
{
  gcca_poll_and_report();
}

void app_main(void)
{
  // Bring up TinyUSB (esp_tinyusb) with the WUP-028 descriptors
  tinyusb_config_t tusb_cfg             = TINYUSB_DEFAULT_CONFIG();
  tusb_cfg.descriptor.device            = &gcca_device_desc;
  tusb_cfg.descriptor.full_speed_config = gcca_configuration_desc;
  tusb_cfg.descriptor.string            = gcca_string_desc;
  tusb_cfg.descriptor.string_count      = gcca_string_desc_count;
  ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

  // Create the platform buses and hand them to the adapter
  struct gcca_config cfg = {.reboot = esp_restart};
  for (int i = 0; i < GCCA_JOYBUS_CHANNELS; i++) {
    struct joybus_esp32_config c = joybus_esp32_config_default(JOYBUS_GPIOS[i]);
    c.rmt_tx_ch                  = i;
    c.rmt_rx_ch                  = i;

    joybus_esp32_init(&esp32_buses[i], c);
    cfg.buses[i] = JOYBUS(&esp32_buses[i]);
  }
  gcca_init(&cfg);

  // Drive gcca_poll_and_report() at 1kHz
  const esp_timer_create_args_t poll_args = {.callback = poll_timer_cb, .name = "gcca_poll"};
  esp_timer_handle_t poll_timer;
  ESP_ERROR_CHECK(esp_timer_create(&poll_args, &poll_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(poll_timer, GCCA_POLL_INTERVAL_MS * 1000));
}
