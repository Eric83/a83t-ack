$(call inherit-product, build/target/product/full_base.mk)
$(call inherit-product, device/softwinner/octopus-common/octopus-common.mk)
$(call inherit-product-if-exists, device/softwinner/octopus-f1/modules/modules.mk)

DEVICE_PACKAGE_OVERLAYS := device/softwinner/octopus-f1/overlay \
                           $(DEVICE_PACKAGE_OVERLAYS)

# PRODUCT_PACKAGES += gatord

PRODUCT_PACKAGES += \
    ESFileExplorer \
    VideoPlayer \
    tinyplay \
    tinycap \
    tinymix \
    tinypcminfo

PRODUCT_COPY_FILES += \
    device/softwinner/octopus-f1/kernel:kernel \
    device/softwinner/octopus-f1/fstab.sun8i:root/fstab.sun8i \
    device/softwinner/octopus-f1/init.sun8i.rc:root/init.sun8i.rc \
    device/softwinner/octopus-f1/init.recovery.sun8i.rc:root/init.recovery.sun8i.rc \
    device/softwinner/octopus-f1/ueventd.sun8i.rc:root/ueventd.sun8i.rc \
    device/softwinner/octopus-f1/recovery.fstab:recovery.fstab \
    device/softwinner/octopus-f1/modules/modules/nand.ko:root/nand.ko
#    device/softwinner/octopus-f1/modules/modules/sunxi_tr.ko:root/sunxi_tr.ko \
#    device/softwinner/octopus-f1/modules/modules/disp.ko:root/disp.ko \
#    device/softwinner/octopus-f1/modules/modules/sw-device.ko:obj/sw-device.ko \
#    device/softwinner/octopus-f1/modules/modules/gt9xxnew_ts.ko:obj/gt9xxnew_ts.ko \

PRODUCT_COPY_FILES += \
    device/softwinner/octopus-f1/configs/camera.cfg:system/etc/camera.cfg \
    device/softwinner/octopus-f1/configs/cfg-Gallery2.xml:system/etc/cfg-Gallery2.xml \
    device/softwinner/octopus-f1/configs/gsensor.cfg:system/usr/gsensor.cfg \
    device/softwinner/octopus-f1/configs/media_profiles.xml:system/etc/media_profiles.xml \
    device/softwinner/octopus-f1/configs/sunxi-keyboard.kl:system/usr/keylayout/sunxi-keyboard.kl \
    device/softwinner/octopus-f1/configs/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    device/softwinner/octopus-f1/configs/gpio-keys-polled.kl:system/usr/keylayout/gpio-keys-polled.kl \
    device/softwinner/octopus-f1/configs/sunxi-ir.kl:system/usr/keylayout/sunxi-ir.kl \
    device/softwinner/octopus-f1/configs/tp.idc:system/usr/idc/tp.idc \
    device/softwinner/octopus-f1/configs/tp.idc:system/usr/idc/ft5x_ts.idc \
    device/softwinner/octopus-f1/configs/tp.idc:system/usr/idc/gt9xxnew_ts.idc \
    device/softwinner/octopus-f1/configs/tp.idc:system/usr/idc/ctp_icn85xx.idc

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml   \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml

# Low mem(memory <= 512M) device should not copy android.software.managed_users.xml
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.software.managed_users.xml:system/etc/permissions/android.software.managed_users.xml

PRODUCT_COPY_FILES += \
	device/softwinner/octopus-f1/media/bootanimation.zip:system/media/bootanimation.zip \

#camera config for camera detector
PRODUCT_COPY_FILES += \
        device/softwinner/octopus-f1/hawkview/sensor_list_cfg.ini:system/etc/hawkview/sensor_list_cfg.ini

# Radio Packages and Configuration Flie
$(call inherit-product, device/softwinner/common/rild/radio_common.mk)
#$(call inherit-product, device/softwinner/common/ril_modem/huawei/mu509/huawei_mu509.mk)
#$(call inherit-product, device/softwinner/common/ril_modem/Oviphone/em55/oviphone_em55.mk)

# camera config for isp
#PRODUCT_COPY_FILES += \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/isp_3a_param.ini:system/etc/hawkview/ov8858_4lane/isp_3a_param.ini \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/isp_iso_param.ini:system/etc/hawkview/ov8858_4lane/isp_iso_param.ini \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/isp_test_param.ini:system/etc/hawkview/ov8858_4lane/isp_test_param.ini \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/isp_tuning_param.ini:system/etc/hawkview/ov8858_4lane/isp_tuning_param.ini \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/bin/gamma_tbl.bin:system/etc/hawkview/ov8858_4lane/bin/gamma_tbl.bin \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/bin/hdr_tbl.bin:system/etc/hawkview/ov8858_4lane/bin/hdr_tbl.bin \
#    device/softwinner/octopus-common/hawkview/8M/ov8858_4lane/bin/lsc_tbl.bin:system/etc/hawkview/ov8858_4lane/bin/lsc_tbl.bin

#sensor    
PRODUCT_COPY_FILES += \
	device/softwinner/octopus-f1/sensor.sh:system/bin/sensor.sh
  
# gps
$(call inherit-product, device/softwinner/octopus-f1/gps/gps.mk)

# usb
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp,adb \
    ro.adb.secure=0 \
    ro.sys.mutedrm=true \
    rw.logger=0

PRODUCT_PROPERTY_OVERRIDES += \
    ro.product.firmware=PEITE-A83T-1.0

PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapsize=512m \
    dalvik.vm.heapstartsize=8m \
    dalvik.vm.heapgrowthlimit=192m \
    dalvik.vm.heaptargetutilization=0.75 \
    dalvik.vm.heapminfree=2m \
    dalvik.vm.heapmaxfree=8m \
    ro.zygote.disable_gl_preload=true

PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.showhdmisettings=7 \
    ro.property.tabletUI=false \
    sys.fullscreen_mode=4

# function
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.bootfast=true
#    ro.dmic.used=true

PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.timezone=Asia/Shanghai \
    persist.sys.country=CN \
    persist.sys.language=zh

# # NFC packages
# PRODUCT_PACKAGES += \
#     libnfc-nci \
#     libnfc_nci_jni \
#     nfc_nci.pn54x.default \
#     NfcNci \
#     Tag \
#     com.android.nfc_extras
# #    libnfc \
# #    libnfc_jni \
# #    Nfc \
# 
# PRODUCT_PROPERTY_OVERRIDES += nfc.nxp_log_level_global=0
# 
# PRODUCT_COPY_FILES += \
#     $(LOCAL_PATH)/nfc/nfcee_access.xml:system/etc/nfcee_access.xml \
#     frameworks/native/data/etc/com.nxp.mifare.xml:system/etc/permissions/com.nxp.mifare.xml \
#     frameworks/native/data/etc/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
#     frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml \
#     frameworks/native/data/etc/android.hardware.nfc.hce.xml:system/etc/permissions/android.hardware.nfc.hce.xml \
#     device/softwinner/octopus-f1/nfc/conf/libnfc-brcm.conf:system/etc/libnfc-brcm.conf \
#     device/softwinner/octopus-f1/nfc/conf/libnfc-nxp-PN547C2_example.conf:system/etc/libnfc-nxp.conf \
#     device/softwinner/octopus-f1/nfc/firmware/libpn547_fw_32bit.so:system/vendor/firmware/libpn547_fw.so

# PRODUCT_PACKAGES += Bluetooth

PRODUCT_PROPERTY_OVERRIDES += \
    ro.debuggable=1 \
    ro.sf.lcd_density=160 \
    ro.sf.rotation=180 \
    sys.def_dhcp_timeout=180 \
    sys.def_screen_off_timeout=2147483647 \
    sys.def_screen_brightness=255 \
    sys.def_hdmi_output_mode=10 \
    sys.def_hdmi_audio=0 \
    sys.def_hdmi_rotation=0 \
    sys.def_disable_rotate=1 \
    sys.def_force_immersive_mode=4 \
    sys.def_bright_system=1 \
    sys.def_ethernet_on=1 \
    sys.def_wifi_on=1 \
    sys.def_bluetooth_on=0 \
    sys.def_ntp_server=ntp.sjtu.edu.cn \
    sys.def_ntp_timeout=20000 \
    sys.def_volume_music=15 \
    sys.def_volume_ring=7 \
    sys.def_volume_system=7 \
    sys.def_volume_voicecall=5 \
    sys.def_volume_alarm=7 \
    sys.def_volume_notification=7 \
    sys.def_volume_bluetoothsoc=15
    
    
$(call inherit-product, device/softwinner/octopus-f1/CedarX/CedarX.mk)
$(call inherit-product, device/softwinner/octopus-f1/EETI_touch/eGTouch.mk)

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := tablet

PRODUCT_BRAND := Allwinner
PRODUCT_NAME := octopus_f1
PRODUCT_DEVICE := octopus-f1
PRODUCT_MODEL := PEITE-A83T
PRODUCT_MANUFACTURER := Allwinner
