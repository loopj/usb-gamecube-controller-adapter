#include "tusb.h"
#include "hardware/watchdog.h"
#include "pico/stdlib.h"

#include <joybus/joybus.h>
#include <joybus/backend/rp2xxx.h>

#include "config.h"
#include "gcca.h"
#include "usb_descriptors.h"

// Joybus data GPIO for each channel (0-3)
#ifndef GCCA_JOYBUS_GPIO_CH0
#define GCCA_JOYBUS_GPIO_CH0 1
#endif
#ifndef GCCA_JOYBUS_GPIO_CH1
#define GCCA_JOYBUS_GPIO_CH1 2
#endif
#ifndef GCCA_JOYBUS_GPIO_CH2
#define GCCA_JOYBUS_GPIO_CH2 3
#endif
#ifndef GCCA_JOYBUS_GPIO_CH3
#define GCCA_JOYBUS_GPIO_CH3 4
#endif

static const uint JOYBUS_GPIOS[GCCA_JOYBUS_CHANNELS] = {
  GCCA_JOYBUS_GPIO_CH0,
  GCCA_JOYBUS_GPIO_CH1,
  GCCA_JOYBUS_GPIO_CH2,
  GCCA_JOYBUS_GPIO_CH3,
};

// Joybus backend instances
static struct joybus_rp2xxx buses[GCCA_JOYBUS_CHANNELS];

// Poll timer
static struct repeating_timer poll_timer;

// Repeating-timer trampoline: the Pico SDK expects a bool-returning callback
static bool poll_timer_cb(struct repeating_timer *timer)
{
  gcca_poll_and_report();
  return true;
}

// Reboot the device (host RPT_RESET request)
static void reboot(void)
{
  watchdog_reboot(0, 0, 0);
  while (1)
    ;
}

// TinyUSB device descriptor callback
uint8_t const *tud_descriptor_device_cb(void)
{
  return (uint8_t const *)&gcca_device_desc;
}

// TinyUSB configuration descriptor callback
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
  return gcca_configuration_desc;
}

// TinyUSB string descriptor callback
const uint16_t *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  return gcca_string_desc_utf16(index);
}

int main(void)
{
  // Bring up TinyUSB
  tusb_init();

  // Create the platform buses and hand them to the adapter
  struct gcca_config cfg = {.reboot = reboot};
  for (int i = 0; i < GCCA_JOYBUS_CHANNELS; i++) {
    joybus_rp2xxx_init(&buses[i], joybus_rp2xxx_config_default(JOYBUS_GPIOS[i]));
    cfg.buses[i] = JOYBUS(&buses[i]);
  }
  gcca_init(&cfg);

  // Drive gcca_poll_and_report() at 1kHz
  add_repeating_timer_ms(GCCA_POLL_INTERVAL_MS, poll_timer_cb, NULL, &poll_timer);

  // Service USB forever
  while (true) {
    tud_task();
  }

  return 0;
}
