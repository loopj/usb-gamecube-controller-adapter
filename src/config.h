#pragma once

// Number of Joybus channels
#define GCCA_JOYBUS_CHANNELS        4

// Enables rumble support
#define GCCA_RUMBLE_POWER_AVAILABLE 1

// Poll the Joybus every 1ms (1kHz)
#define GCCA_POLL_INTERVAL_MS       1

// Tell USB hosts that they should poll every 1ms (1kHz)
// NOTE: This differs from the OEM adapter's poll interval of 8ms
#define GCCA_USB_ENDPOINT_INTERVAL  1
