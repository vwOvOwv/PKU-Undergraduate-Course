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

# [ Filename, Regex of function name, NS code, FCT code ]
# NAMESPACE code
#  0: FTL
#  1: FTL::PageMapping
#  2: ICL
#  3: ICL::GenericCache
#  4: HIL
#  5: HIL::NVMe::Controller
#  6: HIL::NVMe::PRPList
#  7: HIL::NVMe::SGL
#  8: HIL::NVMe::Subsystem
#  9: HIL::NVMe::Namespace
# 10: HIL::NVMe::OpenChannelSSD
# 11: HIL::UFS::Device
# 12: HIL::SATA::Device
# FUNCTION code
#   0: read
#   1: write
#   2: flush
#   3: trim
#   4: format
#   5: readInternal
#   6: writeInternal
#   7: eraseInternal
#   8: trimInternal
#   9: selectVictimBlock
#  10: doGarbageCollection
#  11: createCQ
#  12: createSQ
#  13: collectSQ
#  14: handleRequest
#  15: work
#  16: completion
#  17: getPRPListFromPRP
#  18: parseSGLSegment
#  19: submitCommand
#  20: convertUnit
#  21: formatNVM
#  22: datasetManagement
#  23: vectorChunkRead
#  24: vectorChunkWrite
#  25: vectorChunkReset
#  26: physicalPageRead
#  27: physicalPageWrite
#  28: physicalBlockErase
#  29: processQueryCommand
#  30: processCommand
#  31: prdtRead
#  32: prdtWrite
#  33: readDMA
#  34: readNCQ
#  35: readDMASetup
#  36: readDMADone
#  37: writeDMA
#  38: writeNCQ
#  39: writeDMASetup
#  40: writeDMADone
function = [
    ["ftl/ftl.cc", "read", 0, 0],
    ["ftl/ftl.cc", "write", 0, 1],
    ["ftl/ftl.cc", "trim", 0, 3],
    ["ftl/ftl.cc", "format", 0, 4],
    ["ftl/page_mapping.cc", "read", 1, 0],
    ["ftl/page_mapping.cc", "write", 1, 1],
    ["ftl/page_mapping.cc", "trim", 1, 3],
    ["ftl/page_mapping.cc", "format", 1, 4],
    ["ftl/page_mapping.cc", "selectVictimBlock", 1, 9],
    ["ftl/page_mapping.cc", "doGarbageCollection", 1, 10],
    ["ftl/page_mapping.cc", "readInternal", 1, 5],
    ["ftl/page_mapping.cc", "writeInternal", 1, 6],
    ["ftl/page_mapping.cc", "trimInternal", 1, 8],
    ["ftl/page_mapping.cc", "eraseInternal", 1, 7],
    ["icl/icl.cc", "read", 2, 0],
    ["icl/icl.cc", "write", 2, 1],
    ["icl/icl.cc", "flush", 2, 2],
    ["icl/icl.cc", "trim", 2, 3],
    ["icl/icl.cc", "format", 2, 4],
    ["icl/generic_cache.cc", "read", 3, 0],
    ["icl/generic_cache.cc", "write", 3, 1],
    ["icl/generic_cache.cc", "flush", 3, 2],
    ["icl/generic_cache.cc", "trim", 3, 3],
    ["icl/generic_cache.cc", "format", 3, 4],
    ["hil/hil.cc", "read", 4, 0],
    ["hil/hil.cc", "write", 4, 1],
    ["hil/hil.cc", "flush", 4, 2],
    ["hil/nvme/controller.cc", "handleRequest", 5, 14],
    ["hil/nvme/controller.cc", "collectSQ", 5, 13],
    ["hil/nvme/controller.cc", "completion", 5, 16],
    ["hil/nvme/controller.cc", "work", 5, 15],
    ["hil/nvme/controller.cc", "createCQ", 5, 11],
    ["hil/nvme/controller.cc", "createSQ", 5, 12],
    ["hil/nvme/dma.cc", "getPRPListFromPRP", 6, 17],
    ["hil/nvme/dma.cc", "read", 6, 0],
    ["hil/nvme/dma.cc", "write", 6, 1],
    ["hil/nvme/dma.cc", "parseSGLSegment", 7, 18],
    ["hil/nvme/dma.cc", "read", 7, 0],
    ["hil/nvme/dma.cc", "write", 7, 1],
    ["hil/nvme/subsystem.cc", "submitCommand", 8, 19],
    ["hil/nvme/subsystem.cc", "convertUnit", 8, 20],
    ["hil/nvme/subsystem.cc", "formatNVM", 8, 21],
    ["hil/nvme/namespace.cc", "submitCommand", 9, 19],
    ["hil/nvme/namespace.cc", "read", 9, 0],
    ["hil/nvme/namespace.cc", "write", 9, 1],
    ["hil/nvme/namespace.cc", "flush", 9, 2],
    ["hil/nvme/namespace.cc", "datasetManagement", 9, 22],
    ["hil/nvme/ocssd.cc", "submitCommand", 10, 19],
    ["hil/nvme/ocssd.cc", "read", 10, 0],
    ["hil/nvme/ocssd.cc", "write", 10, 1],
    ["hil/nvme/ocssd.cc", "datasetManagement", 10, 22],
    ["hil/nvme/ocssd.cc", "readInternal", 10, 5],
    ["hil/nvme/ocssd.cc", "writeInternal", 10, 6],
    ["hil/nvme/ocssd.cc", "eraseInternal", 10, 7],
    ["hil/nvme/ocssd.cc", "convertUnit", 10, 20],
    ["hil/nvme/ocssd.cc", "vectorChunkRead", 10, 23],
    ["hil/nvme/ocssd.cc", "vectorChunkWrite", 10, 24],
    ["hil/nvme/ocssd.cc", "vectorChunkReset", 10, 25],
    ["hil/nvme/ocssd.cc", "physicalPageRead", 10, 26],
    ["hil/nvme/ocssd.cc", "physicalPageWrite", 10, 27],
    ["hil/nvme/ocssd.cc", "physicalBlockErase", 10, 28],
    ["hil/ufs/device.cc", "processQueryCommand", 11, 29],
    ["hil/ufs/device.cc", "processCommand", 11, 30],
    ["hil/ufs/device.cc", "prdtRead", 11, 31],
    ["hil/ufs/device.cc", "prdtWrite", 11, 32],
    ["hil/ufs/device.cc", "read", 11, 0],
    ["hil/ufs/device.cc", "write", 11, 1],
    ["hil/ufs/device.cc", "flush", 11, 2],
    ["hil/sata/device.cc", "submitCommand", 12, 19],
    ["hil/sata/device.cc", "prdtRead", 12, 31],
    ["hil/sata/device.cc", "prdtWrite", 12, 32],
    ["hil/sata/device.cc", "read", 12, 0],
    ["hil/sata/device.cc", "write", 12, 1],
    ["hil/sata/device.cc", "flush", 12, 2],
    ["hil/sata/device.cc", "readDMA", 12, 33],
    ["hil/sata/device.cc", "readNCQ", 12, 34],
    ["hil/sata/device.cc", "_readDMASetup", 12, 35],
    ["hil/sata/device.cc", "_readDMADone", 12, 36],
    ["hil/sata/device.cc", "writeDMA", 12, 37],
    ["hil/sata/device.cc", "writeNCQ", 12, 38],
    ["hil/sata/device.cc", "_writeDMASetup", 12, 39],
    ["hil/sata/device.cc", "_writeDMADone", 12, 40],
]
