#pragma once

#include <joybus/joybus.h>

#include "config.h"

// Platform-provided configuration handed to core at startup
struct gcca_config {
  // Initialized Joybus host handles, one per channel
  struct joybus *buses[GCCA_JOYBUS_CHANNELS];

  // Called when the USB host requests a device reset (RPT_RESET report)
  void (*reboot)(void);
};

// Initialize the adapter
void gcca_init(const struct gcca_config *config);

// Run a poll/report cycle
void gcca_poll_and_report(void);
