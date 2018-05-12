# Copyright (C) 2008 The Android Open Source Project
# Copyright (C) 2012 Broadcom Corporation
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

LOCAL_PATH := $(call my-dir)

##########################
#include $(CLEAR_VARS)
#LOCAL_MODULE := Sample
#LOCAL_MODULE_TAGS := optional
#LOCAL_CERTIFICATE := PRESIGNED
#LOCAL_MODULE_PATH := $(TARGET_OUT)/preinstall
#LOCAL_MODULE_PATH := $(TARGET_OUT)/precopy
#LOCAL_MODULE_CLASS := APPS
#LOCAL_SRC_FILES := Sample.apk
#LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
#include $(BUILD_PREBUILT)

########################################
include $(CLEAR_VARS)
LOCAL_MODULE := DragonFire
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_PATH := $(TARGET_OUT)/precopy
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := DragonFire.apk
include $(BUILD_PREBUILT)

########################################
include $(CLEAR_VARS)
LOCAL_MODULE := DragonPhone
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_PATH := $(TARGET_OUT)/precopy
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := DragonPhone.apk
include $(BUILD_PREBUILT)

########################################
include $(CLEAR_VARS)
LOCAL_MODULE := DragonAging
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_PATH := $(TARGET_OUT)/precopy
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := DragonAging.apk
include $(BUILD_PREBUILT)

################################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := ESFileExplorer
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_MODULE_PATH := $(TARGET_OUT)/preinstall
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_SRC_FILES := ES_File_Explorer_3.2.5.1.apk
include $(BUILD_PREBUILT)
