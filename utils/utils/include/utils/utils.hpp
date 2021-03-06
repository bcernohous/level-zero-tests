/*
 *
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef level_zero_tests_UTILS_HPP
#define level_zero_tests_UTILS_HPP

#include <vector>
#include <string>

#include <level_zero/ze_api.h>
#include <level_zero/zet_api.h>

namespace level_zero_tests {

ze_device_handle_t get_default_device(ze_driver_handle_t driver);
ze_driver_handle_t get_default_driver();
std::vector<ze_device_handle_t> get_devices(ze_driver_handle_t driver);
std::vector<ze_driver_handle_t> get_all_driver_handles();

uint32_t get_device_count(ze_driver_handle_t driver);
uint32_t get_driver_handle_count();
uint32_t get_sub_device_count(ze_device_handle_t device);

std::string to_string(const ze_api_version_t version);
std::string to_string(const ze_result_t result);
std::string to_string(const ze_bool_t ze_bool);
std::string to_string(const ze_command_queue_desc_version_t version);
std::string to_string(const ze_command_queue_flag_t flags);
std::string to_string(const ze_command_queue_mode_t mode);
std::string to_string(const ze_command_queue_priority_t priority);
std::string to_string(const ze_image_format_layout_t layout);
std::string to_string(const ze_image_format_type_t type);
std::string to_string(const ze_image_format_swizzle_t swizzle);
std::string to_string(const ze_image_flag_t flag);
std::string to_string(const ze_image_type_t type);
std::string to_string(const ze_image_desc_version_t version);
std::string to_string(const ze_fp_capabilities_t capabilities);
std::string to_string(const ze_native_kernel_uuid_t uuid);
ze_image_format_layout_t to_layout(const std::string layout);
ze_image_format_type_t to_format_type(const std::string format_type);
ze_image_flag_t to_flag(const std::string flag);
ze_image_type_t to_image_type(const std::string type);

void print_driver_version();
void print_driver_overview(const ze_driver_handle_t driver);
void print_driver_overview(const std::vector<ze_driver_handle_t> driver);
void print_platform_overview(const std::string context);
void print_platform_overview();

std::vector<uint8_t> load_binary_file(const std::string &file_path);
void save_binary_file(const std::vector<uint8_t> &data,
                      const std::string &file_path);

template <typename T> int size_in_bytes(const std::vector<T> &v) {
  return static_cast<int>(sizeof(T) * v.size());
}

} // namespace level_zero_tests

std::ostream &operator<<(std::ostream &os, const ze_api_version_t &x);
std::ostream &operator<<(std::ostream &os, const ze_result_t &x);
std::ostream &operator<<(std::ostream &os, const ze_bool_t &x);
std::ostream &operator<<(std::ostream &os,
                         const ze_command_queue_desc_version_t &x);
std::ostream &operator<<(std::ostream &os, const ze_command_queue_flag_t &x);
std::ostream &operator<<(std::ostream &os, const ze_command_queue_mode_t &x);
std::ostream &operator<<(std::ostream &os,
                         const ze_command_queue_priority_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_desc_version_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_format_layout_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_format_type_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_format_swizzle_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_flag_t &x);
std::ostream &operator<<(std::ostream &os, const ze_image_type_t &x);
std::ostream &operator<<(std::ostream &os, const ze_fp_capabilities_t &x);
std::ostream &operator<<(std::ostream &os, const ze_native_kernel_uuid_t &x);
bool operator==(const ze_device_uuid_t &id_a, const ze_device_uuid_t &id_b);
bool operator!=(const ze_device_uuid_t &id_a, const ze_device_uuid_t &id_b);

#endif
