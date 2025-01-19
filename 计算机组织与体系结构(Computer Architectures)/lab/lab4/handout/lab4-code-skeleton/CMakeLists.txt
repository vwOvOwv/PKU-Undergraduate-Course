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
project(simplessd-standalone)

# Add options for debug build
option(DEBUG_BUILD "Build SimpleSSD-Standalone in debug mode." OFF)

# Set DRAMPower path
set(DRAMPOWER_SOURCE_DIR
  "${PROJECT_SOURCE_DIR}/lib/drampower/src"
  CACHE PATH
  ""
  FORCE
)

# Check SimpleSSD is valid
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/simplessd/CMakeLists.txt")
  message(FATAL_ERROR "Failed to find SimpleSSD. \
Check you initialized submodules.")
endif ()

# Add subproject
add_subdirectory(${PROJECT_SOURCE_DIR}/simplessd
  ${CMAKE_BINARY_DIR}/simplessd
)

# Set include directories
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/simplessd
  ${DRAMPOWER_SOURCE_DIR}
)

# Platform specific settings
if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_compile_options(/wd4819)  # Surpress unicode warning
  add_compile_options(/wd4305)  # Surpress truncation warning
else ()
  if (DEBUG_BUILD)
    set(CMAKE_CXX_FLAGS
      "-O0 -g -rdynamic -pthread -Wall -Wextra -Werror ${CMAKE_CXX_FLAGS}")
  else ()
    set(CMAKE_CXX_FLAGS
      "-O2 -rdynamic -pthread -Wall -Wextra -Werror ${CMAKE_CXX_FLAGS}")
  endif ()

  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
      message(FATAL_ERROR "GNU g++ < 4.9 does not support <regex>.\
Please update your compiler.")
    endif ()
  endif ()
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify source files
set(SRC_BIL
  bil/entry.cc
  bil/noop_scheduler.cc
)
set(SRC_IGL_REQUEST
  igl/request/request_config.cc
  igl/request/request_generator.cc
)
set(SRC_IGL_TRACE
  igl/trace/trace_config.cc
  igl/trace/trace_replayer.cc
)
set(SRC_LIB_DRAMPOWER
  lib/drampower/src/CAHelpers.cc
  lib/drampower/src/CmdHandlers.cc
  lib/drampower/src/CmdScheduler.cc
  lib/drampower/src/CommandAnalysis.cc
  lib/drampower/src/MemArchitectureSpec.cc
  lib/drampower/src/MemBankWiseParams.cc
  lib/drampower/src/MemCommand.cc
  lib/drampower/src/MemoryPowerModel.cc
  lib/drampower/src/MemorySpecification.cc
  lib/drampower/src/MemPowerSpec.cc
  lib/drampower/src/MemTimingSpec.cc
  lib/drampower/src/Parameter.cc
  lib/drampower/src/Parametrisable.cc
  lib/drampower/src/TraceParser.cc
  lib/drampower/src/libdrampower/LibDRAMPower.cc
)
set(SRC_SIL_NONE
  sil/none/none.cc
)
set(SRC_SIL_NVME
  sil/nvme/nvme.cc
  sil/nvme/prp.cc
  sil/nvme/queue.cc
)
set(SRC_SIM
  sim/cfg_reader.cc
  sim/engine.cc
  sim/global_config.cc
  sim/main.cc
  sim/signal.cc
)
set(SRC_UTIL
  util/convert.cc
  util/print.cc
  util/stopwatch.cc
)

# Source group for MSVC
SOURCE_GROUP("Source Files\\bil" FILES ${SRC_BIL})
SOURCE_GROUP("Source Files\\igl\\request" FILES ${SRC_IGL_REQUEST})
SOURCE_GROUP("Source Files\\igl\\trace" FILES ${SRC_IGL_TRACE})
SOURCE_GROUP("Source Files\\lib\\drampower" FILES ${SRC_LIB_DRAMPOWER})
SOURCE_GROUP("Source Files\\sil\\none" FILES ${SRC_SIL_NONE})
SOURCE_GROUP("Source Files\\sil\\nvme" FILES ${SRC_SIL_NVME})
SOURCE_GROUP("Source Files\\sim" FILES ${SRC_SIM})
SOURCE_GROUP("Source Files\\util" FILES ${SRC_UTIL})

# Define executable
add_executable(simplessd-standalone
  ${SRC_BIL}
  ${SRC_IGL_REQUEST}
  ${SRC_IGL_TRACE}
  ${SRC_LIB_DRAMPOWER}
  ${SRC_SIL_NONE}
  ${SRC_SIL_NVME}
  ${SRC_SIM}
  ${SRC_UTIL}
)
target_link_libraries(simplessd-standalone simplessd)
