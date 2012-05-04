/* 
 * Copyright © 2012 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Benjamin Segovia <benjamin.segovia@intel.com>
 */

#include "utest_helper.hpp"

static void compiler_if_else(void)
{
  const size_t n = 17;

  // Setup kernel and buffers
  OCL_CREATE_KERNEL("compiler_if_else");
  buf_data[0] = (uint32_t*) malloc(sizeof(uint32_t) * n);
  for (uint32_t i = 0; i < n; ++i) ((uint32_t*)buf_data[0])[i] = 2;
  OCL_CREATE_BUFFER(buf[0], CL_MEM_COPY_HOST_PTR, n * sizeof(uint32_t), buf_data[0]);
  OCL_CREATE_BUFFER(buf[1], 0, n * sizeof(uint32_t), NULL);
  free(buf_data[0]);
  buf_data[0] = NULL;

  // Run the kernel
  OCL_SET_ARG(0, sizeof(cl_mem), &buf[0]);
  OCL_SET_ARG(1, sizeof(cl_mem), &buf[1]);
  globals[0] = 16;
  locals[0] = 16;
  OCL_NDRANGE(1);

  // First control flow
  OCL_MAP_BUFFER(0);
  OCL_MAP_BUFFER(1);
  for (uint32_t i = 0; i < 16; ++i) {
    OCL_ASSERT(((int32_t*)buf_data[1])[i] == 2);
    OCL_ASSERT(((int32_t*)buf_data[0])[i] == 1);
  }

  // Second control flow
  for (uint32_t i = 0; i < n; ++i) ((int32_t*)buf_data[0])[i] = -1;
  OCL_UNMAP_BUFFER(0);
  OCL_UNMAP_BUFFER(1);
  OCL_NDRANGE(1);
  OCL_MAP_BUFFER(0);
  OCL_MAP_BUFFER(1);
  for (uint32_t i = 0; i < 16; ++i) {
    OCL_ASSERT(((uint32_t*)buf_data[1])[i] == -2);
    OCL_ASSERT(((uint32_t*)buf_data[0])[i] == 2);
  }

  // Third control flow
  for (uint32_t i = 0; i < 4; ++i) ((int32_t*)buf_data[0])[i] = 2;
  for (uint32_t i = 4; i < n; ++i) ((int32_t*)buf_data[0])[i] = -1;
  OCL_UNMAP_BUFFER(0);
  OCL_UNMAP_BUFFER(1);
  OCL_NDRANGE(1);
  OCL_MAP_BUFFER(0);
  OCL_MAP_BUFFER(1);
  for (uint32_t i = 0; i < 3; ++i) {
    OCL_ASSERT(((int32_t*)buf_data[1])[i] == 2);
    OCL_ASSERT(((int32_t*)buf_data[0])[i] == 1);
  }
  OCL_ASSERT(((int32_t*)buf_data[1])[3] == -1);
  OCL_ASSERT(((int32_t*)buf_data[0])[3] == 1);
  for (uint32_t i = 4; i < 16; ++i) {
    OCL_ASSERT(((int32_t*)buf_data[1])[i] == -2);
    OCL_ASSERT(((int32_t*)buf_data[0])[i] == 2);
  }
}

MAKE_UTEST_FROM_FUNCTION(compiler_if_else);

