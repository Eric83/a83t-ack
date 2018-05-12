#
# Copyright (C) 2012 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This file is executed by build/envsetup.sh, and can use anything
# defined in envsetup.sh.
#
# In particular, you can add lunch options with the add_lunch_combo
# function: add_lunch_combo generic-eng

export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64
export PATH=$JAVA_HOME/bin:$PATH
export CLASSPATH=.:$JAVA_HOME/lib

function get_device_dir()
{
	DEVICE=$(gettop)/device/softwinner/$(get_build_var TARGET_DEVICE)
}

function cdevice()
{
	get_device_dir
	cd $DEVICE
}

function cout()
{
	cd $OUT
}

function get_lichee_out_dir()
{
    LICHEE_DIR=$ANDROID_BUILD_TOP/../lichee

    TARGET_BOARD_PLATFORM=$(get_build_var TARGET_BOARD_PLATFORM)

    if [ "$TARGET_BOARD_PLATFORM" == "kylin" ]; then
        LINUXOUT_DIR=$LICHEE_DIR/out/sun9iw1p1/android/common
    fi
    if [ "$TARGET_BOARD_PLATFORM" == "astar" ]; then
        LINUXOUT_DIR=$LICHEE_DIR/out/sun8iw5p1/android/common
    fi
    if [ "$TARGET_BOARD_PLATFORM" == "octopus" ]; then
        LINUXOUT_DIR=$LICHEE_DIR/out/sun8iw6p1/android/common
    fi
    if [ "$TARGET_BOARD_PLATFORM" == "tulip" ]; then
        LINUXOUT_DIR=$LICHEE_DIR/out/sun50iw1p1/android/common
    fi

    LINUXOUT_MODULE_DIR=$LINUXOUT_DIR/lib/modules/*/*
}

function extract-bsp()
{
	CURDIR=$PWD

	get_lichee_out_dir
	get_device_dir

	cd $DEVICE

	#extract kernel
	if [ -f kernel ] ; then
		rm kernel
	fi
	cp $LINUXOUT_DIR/bImage kernel
	echo "$DEVICE/bImage copied!"

	#extract linux modules
	if [ -d modules ] ; then
		rm -rf modules
	fi
	mkdir -p modules/modules
	cp -rf $LINUXOUT_MODULE_DIR modules/modules
	echo "$DEVICE/modules copied!"
	chmod 0755 modules/modules/*

# create modules.mk
(cat << EOF) > ./modules/modules.mk 
# modules.mk generate by extract-files.sh, do not edit it.
PRODUCT_COPY_FILES += \\
	\$(call find-copy-subdir-files,*,\$(LOCAL_PATH)/modules,system/vendor/modules)
EOF

	cd $CURDIR
}

function pack()
{
	T=$(gettop)
	get_device_dir
	export ANDROID_IMAGE_OUT=$OUT
	export PACKAGE=$T/../lichee/tools/pack

	sh $DEVICE/package.sh $*
}

function get_uboot()
{
    pack $@
    echo "-------------------------------------"
    if [ ! -e $OUT/boot-resource ];then
        mkdir $OUT/boot-resource
    fi
    rm -rf $OUT/boot-resource/*
    cp -vf $PACKAGE/out/boot-resource.fex $OUT
    cp -rf $PACKAGE/out/boot-resource/* $OUT/boot-resource/
    echo "\"$PACKAGE/out/boot-resource/* -> $OUT/boot-resource/\""
    cp -vf $PACKAGE/out/env.fex $OUT
    cp -vf $PACKAGE/out/boot0_nand.fex $OUT
    cp -vf $PACKAGE/out/boot0_sdcard.fex $OUT
    cp -vf $PACKAGE/out/u-boot.fex $OUT
}

function pack4dist()
{
	DATE=`date +%Y%m%d`
	keys_dir="./vendor/security"
	target_files="$OUT/obj/PACKAGING/target_files_intermediates/$TARGET_PRODUCT-target_files-$DATE.zip"
	signed_target_files="$OUT/signed_target_files.zip"
	target_images="$OUT/target_images.zip"

	get_uboot $@

	make target-files-package

	if [ -d $keys_dir ] ; then
		./build/tools/releasetools/sign_target_files_apks \
			-d $keys_dir $target_files $signed_target_files
		./build/tools/releasetools/img_from_target_files \
			$signed_target_files $target_images
	else
		./build/tools/releasetools/img_from_target_files \
			$target_files $target_images
	fi

	unzip -o $target_images -d $OUT
	rm $target_images
	pack $@
}
