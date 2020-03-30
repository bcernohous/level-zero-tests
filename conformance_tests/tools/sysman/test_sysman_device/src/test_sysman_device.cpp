/*
 *
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include "logging/logging.hpp"
#include "utils/utils.hpp"
#include "test_harness/test_harness.hpp"

namespace lzt = level_zero_tests;

#include <level_zero/ze_api.h>
#include <level_zero/zet_api.h>

namespace {

uint32_t get_prop_length(int8_t prop[ZET_STRING_PROPERTY_SIZE]) {
  uint32_t length = 0;
  for (int i = 0; i < ZET_STRING_PROPERTY_SIZE; i++) {
    if (prop[i] == '\0') {
      break;
    } else {
      length += 1;
    }
  }

  return length;
}

class SysmanDeviceTest : public lzt::SysmanCtsClass {};

TEST_F(
    SysmanDeviceTest,
    GivenValidDeviceWhenResettingSysmanDeviceThenSysmanDeviceResetIsSucceded) {
  for (auto device : devices) {
    lzt::sysman_device_reset(device);
  }
}

TEST_F(
    SysmanDeviceTest,
    GivenValidDeviceWhenResettingSysmanDeviceNnumberOfTimesThenSysmanDeviceResetAlwaysSucceded) {
  int number_iterations = 10;
  for (int i = 0; i < number_iterations; i++) {
    for (auto device : devices) {
      lzt::sysman_device_reset(device);
    }
  }
}

TEST_F(
    SysmanDeviceTest,
    GivenValidDeviceWhenRetrievingSysmanDevicePropertiesThenValidPropertiesAreReturned) {
  for (auto device : devices) {
    auto properties = lzt::get_sysman_device_properties(device);

    EXPECT_EQ(ZE_DEVICE_TYPE_GPU, properties.core.type);
    if (properties.core.isSubdevice == true) {
      EXPECT_LT(properties.core.subdeviceId, UINT32_MAX);
    } else {
      EXPECT_EQ(0, properties.core.subdeviceId);
    }
    EXPECT_LE(get_prop_length(properties.serialNumber),
              ZET_STRING_PROPERTY_SIZE);
    EXPECT_LE(get_prop_length(properties.boardNumber),
              ZET_STRING_PROPERTY_SIZE);
    EXPECT_LE(get_prop_length(properties.brandName), ZET_STRING_PROPERTY_SIZE);
    EXPECT_LE(get_prop_length(properties.modelName), ZET_STRING_PROPERTY_SIZE);
    EXPECT_LE(get_prop_length(properties.vendorName), ZET_STRING_PROPERTY_SIZE);
    EXPECT_LE(get_prop_length(properties.driverVersion),
              ZET_STRING_PROPERTY_SIZE);
  }
}

TEST_F(
    SysmanDeviceTest,
    GivenValidDeviceWhenRetrievingSysmanDevicePropertiesThenExpectSamePropertiesReturnedTwice) {
  for (auto device : devices) {
    auto propertiesInitial = lzt::get_sysman_device_properties(device);
    auto propertiesLater = lzt::get_sysman_device_properties(device);
    EXPECT_EQ(propertiesInitial.core.type, propertiesLater.core.type);
    EXPECT_EQ(propertiesInitial.core.isSubdevice,
              propertiesLater.core.isSubdevice);
    if (propertiesInitial.core.isSubdevice == true &&
        propertiesLater.core.isSubdevice == true) {
      EXPECT_EQ(propertiesInitial.core.subdeviceId,
                propertiesLater.core.subdeviceId);
    }
    EXPECT_TRUE(0 ==
                std::memcmp(propertiesInitial.serialNumber,
                            propertiesLater.serialNumber,
                            get_prop_length(propertiesInitial.serialNumber)));
    EXPECT_TRUE(0 ==
                std::memcmp(propertiesInitial.boardNumber,
                            propertiesLater.boardNumber,
                            get_prop_length(propertiesInitial.boardNumber)));

    EXPECT_TRUE(0 == std::memcmp(propertiesInitial.brandName,
                                 propertiesLater.brandName,
                                 get_prop_length(propertiesInitial.brandName)));

    EXPECT_TRUE(0 == std::memcmp(propertiesInitial.modelName,
                                 propertiesLater.modelName,
                                 get_prop_length(propertiesInitial.modelName)));

    EXPECT_TRUE(0 ==
                std::memcmp(propertiesInitial.vendorName,
                            propertiesLater.vendorName,
                            get_prop_length(propertiesInitial.vendorName)));

    EXPECT_TRUE(0 ==
                std::memcmp(propertiesInitial.driverVersion,
                            propertiesLater.driverVersion,
                            get_prop_length(propertiesInitial.driverVersion)));
  }
}
} // namespace