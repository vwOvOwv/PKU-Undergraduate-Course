# Copyright (C) 2017 CAMELab
#
# This file is part of SimpleSSD.
#
# SimpleSSD is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SimpleSSD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.

# Set version and project name
cmake_minimum_required(VERSION 3.10)
project(simplessd)

# Add options for debug build
option(DEBUG_BUILD "Build SimpleSSD in debug mode." OFF)

# Set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set cache entry for external library include path
set(DRAMPOWER_SOURCE_DIR "." CACHE PATH "Source directory to DRAMPower library")

# Check DRAMPOWER_SOURCE_DIR is valid
if (NOT EXISTS "${DRAMPOWER_SOURCE_DIR}/libdrampower/LibDRAMPower.h")
  message(FATAL_ERROR "Failed to find DRAMPower library. \
Check you specified DRAMPower path by -DDRAMPOWER_SOURCE_DIR.")
endif ()

# Check mcpat is valid
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/lib/mcpat/mcpat.h")
  message(FATAL_ERROR "Failed to find McPAT library. \
Check you initialized submodules.")
endif ()

# Add subproject
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/mcpat)

# Set include directories
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/lib/mcpat
  ${PROJECT_SOURCE_DIR}/lib/mcpat/cacti
  ${PROJECT_SOURCE_DIR}/lib/inih
  ${DRAMPOWER_SOURCE_DIR}
)

# Platform specific settings
if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_compile_options(/wd4819)  # Surpress unicode warning
  add_compile_options(/wd4244)  # Surpress conversion warning
  add_compile_options(/wd4267)  # Surpress conversion warning
  add_compile_options(/wd4305)  # Surpress truncation warning
  add_compile_options(/wd4800)  # Surpress int -> boolean waring
else ()
  set(CMAKE_CXX_FLAGS "-g -rdynamic -Wall -Wextra -Werror ${CMAKE_CXX_FLAGS}")

  if (DEBUG_BUILD)
    set(CMAKE_CXX_FLAGS "-O0 ${CMAKE_CXX_FLAGS}")
  else ()
    set(CMAKE_CXX_FLAGS "-O2 ${CMAKE_CXX_FLAGS}")
  endif ()

  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.0")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")
    endif ()
  endif ()
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify source files
set(SRC_CPU
  cpu/config.cc
  cpu/cpu.cc
)
set(SRC_DRAM
  dram/abstract_dram.cc
  dram/config.cc
  dram/simple.cc
)
set(SRC_FTL_COMMON
  ftl/common/block.cc
)
set(SRC_FTL
  ftl/config.cc
  ftl/ftl.cc
  ftl/page_mapping.cc
  ftl/fast.cc
)
set(SRC_HIL_NVME
  hil/nvme/abstract_subsystem.cc
  hil/nvme/config.cc
  hil/nvme/controller.cc
  hil/nvme/dma.cc
  hil/nvme/namespace.cc
  hil/nvme/ocssd.cc
  hil/nvme/queue.cc
  hil/nvme/subsystem.cc
)
set(SRC_HIL_SATA
  hil/sata/config.cc
  hil/sata/device.cc
  hil/sata/hba.cc
)
set(SRC_HIL_UFS
  hil/ufs/config.cc
  hil/ufs/def.cc
  hil/ufs/device.cc
  hil/ufs/host.cc
)
set(SRC_HIL
  hil/hil.cc
)
set(SRC_ICL
  icl/abstract_cache.cc
  icl/config.cc
  icl/generic_cache.cc
  icl/icl.cc
)
set(SRC_LIB_INIH
  lib/inih/ini.c
)
set(SRC_PAL_OLD
  pal/old/Latency.cc
  pal/old/LatencyMLC.cc
  pal/old/LatencySLC.cc
  pal/old/LatencyTLC.cc
  pal/old/PAL2.cc
  pal/old/PAL2_TimeSlot.cc
  pal/old/PALStatistics.cc
)
set(SRC_PAL
  pal/config.cc
  pal/pal.cc
  pal/pal_old.cc
)
set(SRC_SIM
  sim/config_reader.cc
  sim/cpu.cc
  sim/log.cc
  sim/simulator.cc
  sim/state.cc
)
set(SRC_UTIL
  util/bitset.cc
  util/def.cc
  util/disk.cc
  util/fifo.cc
  util/interface.cc
  util/simplessd.cc
)

# Source group for MSVC
SOURCE_GROUP("Source Files\\cpu" FILES ${SRC_CPU})
SOURCE_GROUP("Source Files\\dram" FILES ${SRC_DRAM})
SOURCE_GROUP("Source Files\\ftl" FILES ${SRC_FTL})
SOURCE_GROUP("Source Files\\ftl\\common" FILES ${SRC_FTL_COMMON})
SOURCE_GROUP("Source Files\\hil" FILES ${SRC_HIL})
SOURCE_GROUP("Source Files\\hil\\nvme" FILES ${SRC_HIL_NVME})
SOURCE_GROUP("Source Files\\hil\\sata" FILES ${SRC_HIL_SATA})
SOURCE_GROUP("Source Files\\hil\\ufs" FILES ${SRC_HIL_UFS})
SOURCE_GROUP("Source Files\\icl" FILES ${SRC_ICL})
SOURCE_GROUP("Source Files\\lib\\inih" FILES ${SRC_LIB_INIH})
SOURCE_GROUP("Source Files\\pal" FILES ${SRC_PAL})
SOURCE_GROUP("Source Files\\pal\\old" FILES ${SRC_PAL_OLD})
SOURCE_GROUP("Source Files\\sim" FILES ${SRC_SIM})
SOURCE_GROUP("Source Files\\util" FILES ${SRC_UTIL})

# Define library
add_library(simplessd STATIC
  ${SRC_CPU}
  ${SRC_DRAM}
  ${SRC_FTL}
  ${SRC_FTL_COMMON}
  ${SRC_HIL}
  ${SRC_HIL_NVME}
  ${SRC_HIL_SATA}
  ${SRC_HIL_UFS}
  ${SRC_ICL}
  ${SRC_LIB_INIH}
  ${SRC_PAL}
  ${SRC_PAL_OLD}
  ${SRC_SIM}
  ${SRC_UTIL}
)
target_link_libraries(simplessd mcpat)
