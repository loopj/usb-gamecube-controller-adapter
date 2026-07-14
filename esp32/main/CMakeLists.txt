set(GCCA_SHARED ${CMAKE_CURRENT_LIST_DIR}/../../shared)

idf_component_register(
    SRCS
        "main.c"
        "${GCCA_SHARED}/gcca.c"
        "${GCCA_SHARED}/usb_descriptors.c"
    INCLUDE_DIRS "." "${GCCA_SHARED}"
    REQUIRES libjoybus esp_tinyusb)
