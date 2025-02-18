/*
 * Copyright (c) 2018 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "filesystem.h"

#include <pb.h>

namespace particle {

int openFile(lfs_file_t* file, const char* path, unsigned flags = LFS_O_RDWR);
int dumpFile(const char* path);

int decodeMessageFromFile(lfs_file_t* file, const pb_msgdesc_t* desc, void* msg);
int encodeMessageToFile(lfs_file_t* file, const pb_msgdesc_t* desc, const void* msg);

} // particle
