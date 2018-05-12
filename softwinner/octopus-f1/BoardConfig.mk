# BoardConfig.mk
#
# Product-specific compile-time definitions.
#

include device/softwinner/octopus-common/BoardConfigCommon.mk

# image related
TARGET_NO_BOOTLOADER := true
TARGET_NO_RECOVERY := false
TARGET_NO_KERNEL := false

# Enable dex-preoptimization to speed up first boot sequence
WITH_DEXPREOPT := false
#DONT_DEXPREOPT_PREBUILTS := true


INSTALLED_KERNEL_TARGET := kernel
BOARD_KERNEL_CMDLINE := 
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_FLASH_BLOCK_SIZE := 4096
# BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1610612736
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1073741824

# recovery stuff
#TARGET_RECOVERY_PIXEL_FORMAT := "BGRA_8888"
TARGET_RECOVERY_UI_LIB := librecovery_ui_octopus_f1
SW_BOARD_TOUCH_RECOVERY := true
SW_BOARD_RECOVERY_ROTATION := 0

#sensor parameter
#SW_BOARD_USES_SENSORS_TYPE := lsm9ds0

TARGET_USE_BOOSTUP_OPZ := false

BOARD_WIFI_VENDOR := realtek
ifeq ($(BOARD_WIFI_VENDOR), realtek)
    WPA_SUPPLICANT_VERSION := VER_0_8_X
    BOARD_WPA_SUPPLICANT_DRIVER := NL80211
    BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_rtl
    BOARD_HOSTAPD_DRIVER        := NL80211
    BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_rtl

#    BOARD_USR_WIFI := rtl8723bs
#    BOARD_WLAN_DEVICE := rtl8723bs
    BOARD_USR_WIFI := rtl8188eu
    BOARD_WLAN_DEVICE := rtl8188eu

endif

ifeq ($(BOARD_USR_WIFI), rtl8723bs)
  BOARD_HAVE_BLUETOOTH := true
  BOARD_HAVE_BLUETOOTH_RTK := true
  BLUETOOTH_HCI_USE_RTK_H5 := true
  BOARD_HAVE_BLUETOOTH_NAME := rtl8723bs
  BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/softwinner/octopus-f1/bluetooth

PRODUCT_COPY_FILES += \
    hardware/realtek/wlan/config/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
    hardware/realtek/wlan/config/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf \
    hardware/realtek/bluetooth/rtl8723bs/firmware/rtl8723b_fw:system/etc/firmware/rtl8723b_fw \
    hardware/realtek/bluetooth/rtl8723bs/firmware/rtl8723b_config:system/etc/firmware/rtl8723b_config

  PRODUCT_PROPERTY_OVERRIDES += ro.product.8723b_bt.used=true
endif

ifeq ($(BOARD_USR_WIFI), rtl8188eu)
  BOARD_HAVE_BLUETOOTH := false
endif

ifeq ($(BOARD_HAVE_BLUETOOTH), true)
  PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    device/softwinner/octopus-f1/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf
endif

