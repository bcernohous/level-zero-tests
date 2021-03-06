/*
 *
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_harness/test_harness.hpp"
#include "utils/utils.hpp"
#include "utils/utils.hpp"
#include "gtest/gtest.h"
#include <level_zero/ze_api.h>
#include <thread>
#include <chrono>

namespace level_zero_tests {

ze_module_handle_t create_module(ze_device_handle_t device,
                                 const std::string filename) {

  return (
      create_module(device, filename, ZE_MODULE_FORMAT_IL_SPIRV, "", nullptr));
}

ze_module_handle_t create_module(ze_device_handle_t device,
                                 const std::string filename,
                                 const ze_module_format_t format,
                                 const char *build_flags,
                                 ze_module_build_log_handle_t *p_build_log) {

  ze_module_desc_t module_description = {};
  ze_module_handle_t module;
  ze_module_constants_t module_constants = {};
  const std::vector<uint8_t> binary_file =
      level_zero_tests::load_binary_file(filename);

  EXPECT_TRUE((format == ZE_MODULE_FORMAT_IL_SPIRV) ||
              (format == ZE_MODULE_FORMAT_NATIVE));
  module_description.version = ZE_MODULE_DESC_VERSION_CURRENT;
  module_description.format = format;
  module_description.inputSize = static_cast<uint32_t>(binary_file.size());
  module_description.pInputModule = binary_file.data();
  module_description.pBuildFlags = build_flags;
  module_description.pConstants = &module_constants;

  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleCreate(device, &module_description, &module, p_build_log));

  return module;
}

size_t get_build_log_size(const ze_module_build_log_handle_t build_log) {
  size_t build_log_size = 0;
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleBuildLogGetString(build_log, &build_log_size, nullptr));
  EXPECT_GT(build_log_size, 0);
  return build_log_size;
}

std::string get_build_log_string(const ze_module_build_log_handle_t build_log) {
  size_t build_log_size = 0;
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleBuildLogGetString(build_log, &build_log_size, nullptr));

  EXPECT_GT(build_log_size, 0);

  std::vector<char> build_log_c_string(build_log_size);
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleBuildLogGetString(build_log, &build_log_size,
                                      build_log_c_string.data()));
  return std::string(build_log_c_string.begin(), build_log_c_string.end());
}

size_t get_native_binary_size(const ze_module_handle_t module) {
  size_t native_binary_size = 0;
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleGetNativeBinary(module, &native_binary_size, nullptr));
  EXPECT_GT(native_binary_size, 0);
  return native_binary_size;
}

void save_native_binary_file(const ze_module_handle_t module,
                             const std::string filename) {
  size_t native_binary_size = 0;
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleGetNativeBinary(module, &native_binary_size, nullptr));
  EXPECT_GT(native_binary_size, 0);

  std::vector<uint8_t> native_binary(native_binary_size);
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeModuleGetNativeBinary(module, &native_binary_size,
                                    native_binary.data()));
  level_zero_tests::save_binary_file(native_binary, filename);
}

void destroy_build_log(const ze_module_build_log_handle_t build_log) {
  EXPECT_EQ(ZE_RESULT_SUCCESS, zeModuleBuildLogDestroy(build_log));
}

void set_argument_value(ze_kernel_handle_t hFunction, uint32_t argIndex,
                        size_t argSize, const void *pArgValue) {
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelSetArgumentValue(hFunction, argIndex, argSize, pArgValue));
}

void set_group_size(ze_kernel_handle_t hFunction, uint32_t groupSizeX,
                    uint32_t groupSizeY, uint32_t groupSizeZ) {
  EXPECT_EQ(ZE_RESULT_SUCCESS, zeKernelSetGroupSize(hFunction, groupSizeX,
                                                    groupSizeY, groupSizeZ));
}

void suggest_group_size(ze_kernel_handle_t hFunction, uint32_t globalSizeX,
                        uint32_t globalSizeY, uint32_t globalSizeZ,
                        uint32_t &groupSizeX, uint32_t &groupSizeY,
                        uint32_t &groupSizeZ) {
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelSuggestGroupSize(hFunction, globalSizeX, globalSizeY,
                                     globalSizeZ, &groupSizeX, &groupSizeY,
                                     &groupSizeZ));
}

void destroy_module(ze_module_handle_t module) {
  EXPECT_EQ(ZE_RESULT_SUCCESS, zeModuleDestroy(module));
}

ze_kernel_handle_t create_function(ze_module_handle_t module,
                                   std::string func_name) {
  return create_function(module, ZE_KERNEL_FLAG_NONE, func_name);
}

ze_kernel_handle_t create_function(ze_module_handle_t module,
                                   ze_kernel_flag_t flag,
                                   std::string func_name) {
  ze_kernel_handle_t kernel;
  ze_kernel_desc_t kernel_description = {};
  EXPECT_TRUE((flag == ZE_KERNEL_FLAG_NONE) ||
              (flag == ZE_KERNEL_FLAG_FORCE_RESIDENCY));
  kernel_description.version = ZE_KERNEL_DESC_VERSION_CURRENT;
  kernel_description.flags = flag;
  kernel_description.pKernelName = func_name.c_str();

  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelCreate(module, &kernel_description, &kernel));
  return kernel;
}

void destroy_function(ze_kernel_handle_t kernel) {

  EXPECT_EQ(ZE_RESULT_SUCCESS, zeKernelDestroy(kernel));
}

// Currently limited to creating function with 1d group and single argument.
// Expand as needed.
void create_and_execute_function(ze_device_handle_t device,
                                 ze_module_handle_t module,
                                 std::string func_name, int group_size,
                                 void *arg) {
  std::vector<FunctionArg> args;
  FunctionArg func_arg{sizeof(arg), &arg};
  args.push_back(func_arg);
  create_and_execute_function(device, module, func_name, group_size, args);
}

void create_and_execute_function(ze_device_handle_t device,
                                 ze_module_handle_t module,
                                 std::string func_name, int group_size,
                                 const std::vector<FunctionArg> &args) {

  ze_kernel_handle_t function = create_function(module, func_name);
  ze_command_list_handle_t cmdlist = create_command_list(device);
  ze_command_queue_handle_t cmdq = create_command_queue(device);
  uint32_t group_size_x = group_size;
  uint32_t group_size_y = 1;
  uint32_t group_size_z = 1;
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelSuggestGroupSize(function, group_size, 1, 1, &group_size_x,
                                     &group_size_y, &group_size_z));

  EXPECT_EQ(
      ZE_RESULT_SUCCESS,
      zeKernelSetGroupSize(function, group_size_x, group_size_y, group_size_z));

  int i = 0;
  for (auto arg : args) {
    EXPECT_EQ(
        ZE_RESULT_SUCCESS,
        zeKernelSetArgumentValue(function, i++, arg.arg_size, arg.arg_value));
  }

  ze_group_count_t thread_group_dimensions;
  thread_group_dimensions.groupCountX = 1;
  thread_group_dimensions.groupCountY = 1;
  thread_group_dimensions.groupCountZ = 1;

  EXPECT_EQ(ZE_RESULT_SUCCESS, zeCommandListAppendLaunchKernel(
                                   cmdlist, function, &thread_group_dimensions,
                                   nullptr, 0, nullptr));

  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeCommandListAppendBarrier(cmdlist, nullptr, 0, nullptr));
  EXPECT_EQ(ZE_RESULT_SUCCESS, zeCommandListClose(cmdlist));

  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeCommandQueueExecuteCommandLists(cmdq, 1, &cmdlist, nullptr));

  EXPECT_EQ(ZE_RESULT_SUCCESS, zeCommandQueueSynchronize(cmdq, UINT32_MAX));

  destroy_function(function);
  EXPECT_EQ(ZE_RESULT_SUCCESS, zeCommandQueueDestroy(cmdq));
  EXPECT_EQ(ZE_RESULT_SUCCESS, zeCommandListDestroy(cmdlist));
}

uint32_t get_kernel_source_attribute_size(ze_kernel_handle_t hKernel) {
  uint32_t size = 0;
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelGetAttribute(hKernel, ZE_KERNEL_ATTR_SOURCE_ATTRIBUTE,
                                 &size, nullptr));
  return size;
}

void get_kernel_attribute(ze_kernel_handle_t hKernel,
                          ze_kernel_attribute_t attr, uint32_t *size,
                          void *pValue) {
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelGetAttribute(hKernel, attr, size, pValue));
}

void set_kernel_attribute(ze_kernel_handle_t hKernel,
                          ze_kernel_attribute_t attr, uint32_t size,
                          void *pValue) {
  EXPECT_NE(attr, ZE_KERNEL_ATTR_SOURCE_ATTRIBUTE);
  EXPECT_EQ(ZE_RESULT_SUCCESS,
            zeKernelSetAttribute(hKernel, attr, size, pValue));
}

} // namespace level_zero_tests
