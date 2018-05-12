# 3G Modem Configuration Flie
PRODUCT_COPY_FILES += \
	device/softwinner/fiber-common/rild/ip-down:system/etc/ppp/ip-down \
	device/softwinner/fiber-common/rild/ip-up:system/etc/ppp/ip-up \
	device/softwinner/fiber-common/rild/call-pppd:system/etc/ppp/call-pppd \
	device/softwinner/fiber-common/rild/apns-conf_sdk.xml:system/etc/apns-conf.xml \
	device/softwinner/wing-k8x4/ril_modem/yuga/cwm600/libsoftwinner-ril-yuga-cwm600.so:system/lib/libsoftwinner-ril-yuga-cwm600.so

PRODUCT_COPY_FILES += \
	device/softwinner/wing-k8x4/ril_modem/yuga/cwm600/init.yuga_cwm600.rc:root/init.sun7i.3g.rc
