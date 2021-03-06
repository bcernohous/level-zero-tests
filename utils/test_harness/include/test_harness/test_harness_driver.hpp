/*
 *
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef level_zero_tests_ZE_TEST_HARNESS_DRIVER_HPP
#define level_zero_tests_ZE_TEST_HARNESS_DRIVER_HPP

#include <level_zero/ze_api.h>

namespace level_zero_tests {

void ze_init();
void ze_init(ze_init_flag_t init_flag);

uint32_t get_driver_version(ze_driver_handle_t driver);
ze_api_version_t get_api_version(ze_driver_handle_t driver);
ze_driver_ipc_properties_t get_ipc_properties(ze_driver_handle_t driver);

}; // namespace level_zero_tests

#endif