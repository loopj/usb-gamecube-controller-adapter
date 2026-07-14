set(GCCA_CORE ${CMAKE_CURRENT_LIST_DIR}/../../../src)

idf_component_register(
    SRCS
        "main.c"
        "${GCCA_CORE}/gcca.c"
        "${GCCA_CORE}/usb_descriptors.c"
    INCLUDE_DIRS "." "${GCCA_CORE}"
    REQUIRES libjoybus esp_tinyusb)
