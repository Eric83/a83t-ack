# 3G Data Card Packages
PRODUCT_PACKAGES += \
	u3gmonitor \
        chat \
        pppd \
        rild \
        Mms \
	Dialer \
	Stk

# 3G Modem Configuration Flie
PRODUCT_COPY_FILES += \
	device/softwinner/common/rild/ip-down:system/etc/ppp/ip-down \
	device/softwinner/common/rild/ip-up:system/etc/ppp/ip-up \
	device/softwinner/common/rild/3g_dongle.cfg:system/etc/3g_dongle.cfg \
	device/softwinner/common/rild/usb_modeswitch:system/bin/usb_modeswitch \
	device/softwinner/common/rild/call-pppd:system/xbin/call-pppd \
	device/softwinner/common/rild/usb_modeswitch.sh:system/xbin/usb_modeswitch.sh \
	device/softwinner/common/rild/apns-conf_sdk.xml:system/etc/apns-conf.xml \
	device/softwinner/common/rild/lib/libsoftwinner-ril-5.0.so:system/lib/libsoftwinner-ril-5.0.so\
        device/softwinner/common/rild/lib64/libsoftwinner-ril-5.0.so:system/lib64/libsoftwinner-ril-5.0.so \
	device/softwinner/octopus-f1/ril_modem/huawei/mu509/libsoftwinner-ril-huawei-mu509.so:system/lib/libsoftwinner-ril-huawei-mu509.so

# 3G Data Card usb modeswitch File
PRODUCT_COPY_FILES += \
        $(call find-copy-subdir-files,*,device/softwinner/common/rild/usb_modeswitch.d,system/etc/usb_modeswitch.d)


# Radio parameter
PRODUCT_PROPERTY_OVERRIDES += \
        rild.libargs=-d/dev/ttyUSB2 \
        rild.libpath=libsoftwinner-ril-5.0.so 

# Radio parameter
#PRODUCT_PROPERTY_OVERRIDES += \
        rild.libargs=-d/dev/ttyUSB2 \
        rild.libpath=libsoftwinner-ril-huawei-mu509.so \
	ro.ril.ecclist=110,119,120,112,114,911 \
	audio.without.earpiece=1 \
        ro.sw.embeded.telephony=true \
	ro.sw.audio.codec_plan_name=PLAN_ONE \
	ro.sw.audio.bp._device_name=mu509 \
	rw.talkingstandby.enabled=1
