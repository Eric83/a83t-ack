# a83t-ack
1. linux-3.4 -- Copy linux-3.4 to lichee and instead of linux-3.4.
2. sys_config.fex -- Copy sys_config.fex to lichee/tools/pack/chips/sun8iw6p1/configs/f1/sys_config.fex
3. softwinner -- Copy softwinner instead of android/device/softwinner.

Then:
	Compile kernel by
	./build.sh config
	./build


	update camera.cfg
	rebuild android
	or adb push camera.cfg system/etc/

