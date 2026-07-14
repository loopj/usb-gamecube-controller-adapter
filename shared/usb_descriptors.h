#pragma once

#include <stdint.h>

#include <tusb.h>

// WUP-028 USB descriptors
extern const tusb_desc_device_t gcca_device_desc;
extern const uint8_t gcca_configuration_desc[];
extern const char *gcca_string_desc[];
extern const int gcca_string_desc_count;

// Pack the UTF-16 USB string descriptor for the given index from gcca_string_desc[]
const uint16_t *gcca_string_desc_utf16(uint8_t index);
