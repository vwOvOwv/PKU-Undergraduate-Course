/*****************************************************************************
 *                                McPAT
 *                      SOFTWARE LICENSE AGREEMENT
 *            Copyright 2012 Hewlett-Packard Development Company, L.P.
 *                          All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.”
 *
 ***************************************************************************/

#include "XML_Parse.h"
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void ParseXML::parse(char *filepath) {
  // Initialize all structures
  ParseXML::initialize();
}
void ParseXML::initialize()  // Initialize all
{
  // All number_of_* at the level of 'system' 03/21/2009
  sys.number_of_cores = 1;
  sys.number_of_L1Directories = 1;
  sys.number_of_L2Directories = 1;
  sys.number_of_L2s = 1;
  sys.Private_L2 = false;
  sys.number_of_L3s = 1;
  sys.number_of_NoCs = 1;
  // All params at the level of 'system'
  // strcpy(sys.homogeneous_cores,"default");
  sys.core_tech_node = 1;
  sys.target_core_clockrate = 1;
  sys.target_chip_area = 1;
  sys.temperature = 360;
  sys.number_cache_levels = 1;
  sys.homogeneous_cores = 1;
  sys.homogeneous_L1Directories = 1;
  sys.homogeneous_L2Directories = 1;
  sys.homogeneous_L2s = 1;
  sys.homogeneous_L3s = 1;
  sys.homogeneous_NoCs = 1;
  sys.homogeneous_ccs = 1;

  sys.Max_area_deviation = 1;
  sys.Max_power_deviation = 1;
  sys.device_type = 1;
  sys.longer_channel_device = true;
  sys.power_gating = false;
  sys.Embedded = false;
  sys.opt_dynamic_power = false;
  sys.opt_lakage_power = false;
  sys.opt_clockrate = true;
  sys.opt_area = false;
  sys.interconnect_projection_type = 1;
  sys.vdd = 0;
  sys.power_gating_vcc = -1;
  int i, j;
  for (i = 0; i <= 63; i++) {
    sys.core[i].vdd = 0;
    sys.core[i].power_gating_vcc = -1;
    sys.core[i].clock_rate = 1;
    sys.core[i].opt_local = true;
    sys.core[i].x86 = false;
    sys.core[i].machine_bits = 1;
    sys.core[i].virtual_address_width = 1;
    sys.core[i].physical_address_width = 1;
    sys.core[i].opcode_width = 1;
    sys.core[i].micro_opcode_width = 1;
    // strcpy(sys.core[i].machine_type,"default");
    sys.core[i].internal_datapath_width = 1;
    sys.core[i].number_hardware_threads = 1;
    sys.core[i].fetch_width = 1;
    sys.core[i].number_instruction_fetch_ports = 1;
    sys.core[i].decode_width = 1;
    sys.core[i].issue_width = 1;
    sys.core[i].peak_issue_width = 1;
    sys.core[i].commit_width = 1;
    for (j = 0; j < 20; j++)
      sys.core[i].pipelines_per_core[j] = 1;
    for (j = 0; j < 20; j++)
      sys.core[i].pipeline_depth[j] = 1;
    strcpy(sys.core[i].FPU, "default");
    strcpy(sys.core[i].divider_multiplier, "default");
    sys.core[i].ALU_per_core = 1;
    sys.core[i].FPU_per_core = 1.0;
    sys.core[i].MUL_per_core = 1;
    sys.core[i].instruction_buffer_size = 1;
    sys.core[i].decoded_stream_buffer_size = 1;
    // strcpy(sys.core[i].instruction_window_scheme,"default");
    sys.core[i].instruction_window_size = 1;
    sys.core[i].ROB_size = 1;
    sys.core[i].archi_Regs_IRF_size = 1;
    sys.core[i].archi_Regs_FRF_size = 1;
    sys.core[i].phy_Regs_IRF_size = 1;
    sys.core[i].phy_Regs_FRF_size = 1;
    // strcpy(sys.core[i].rename_scheme,"default");
    sys.core[i].checkpoint_depth = 1;
    sys.core[i].register_windows_size = 1;
    strcpy(sys.core[i].LSU_order, "default");
    sys.core[i].store_buffer_size = 1;
    sys.core[i].load_buffer_size = 1;
    sys.core[i].memory_ports = 1;
    strcpy(sys.core[i].Dcache_dual_pump, "default");
    sys.core[i].RAS_size = 1;
    // all stats at the level of system.core(0-n)
    sys.core[i].total_instructions = 1;
    sys.core[i].int_instructions = 1;
    sys.core[i].fp_instructions = 1;
    sys.core[i].branch_instructions = 1;
    sys.core[i].branch_mispredictions = 1;
    sys.core[i].committed_instructions = 1;
    sys.core[i].load_instructions = 1;
    sys.core[i].store_instructions = 1;
    sys.core[i].total_cycles = 1;
    sys.core[i].idle_cycles = 1;
    sys.core[i].busy_cycles = 1;
    sys.core[i].instruction_buffer_reads = 1;
    sys.core[i].instruction_buffer_write = 1;
    sys.core[i].ROB_reads = 1;
    sys.core[i].ROB_writes = 1;
    sys.core[i].rename_accesses = 1;
    sys.core[i].inst_window_reads = 1;
    sys.core[i].inst_window_writes = 1;
    sys.core[i].inst_window_wakeup_accesses = 1;
    sys.core[i].inst_window_selections = 1;
    sys.core[i].archi_int_regfile_reads = 1;
    sys.core[i].archi_float_regfile_reads = 1;
    sys.core[i].phy_int_regfile_reads = 1;
    sys.core[i].phy_float_regfile_reads = 1;
    sys.core[i].windowed_reg_accesses = 1;
    sys.core[i].windowed_reg_transports = 1;
    sys.core[i].function_calls = 1;
    sys.core[i].ialu_accesses = 1;
    sys.core[i].fpu_accesses = 1;
    sys.core[i].mul_accesses = 1;
    sys.core[i].cdb_alu_accesses = 1;
    sys.core[i].cdb_mul_accesses = 1;
    sys.core[i].cdb_fpu_accesses = 1;
    sys.core[i].load_buffer_reads = 1;
    sys.core[i].load_buffer_writes = 1;
    sys.core[i].load_buffer_cams = 1;
    sys.core[i].store_buffer_reads = 1;
    sys.core[i].store_buffer_writes = 1;
    sys.core[i].store_buffer_cams = 1;
    sys.core[i].store_buffer_forwards = 1;
    sys.core[i].main_memory_access = 1;
    sys.core[i].main_memory_read = 1;
    sys.core[i].main_memory_write = 1;
    sys.core[i].IFU_duty_cycle = 1;
    sys.core[i].BR_duty_cycle = 1;
    sys.core[i].LSU_duty_cycle = 1;
    sys.core[i].MemManU_I_duty_cycle = 1;
    sys.core[i].MemManU_D_duty_cycle = 1;
    sys.core[i].ALU_duty_cycle = 1;
    sys.core[i].MUL_duty_cycle = 1;
    sys.core[i].FPU_duty_cycle = 1;
    sys.core[i].ALU_cdb_duty_cycle = 1;
    sys.core[i].MUL_cdb_duty_cycle = 1;
    sys.core[i].FPU_cdb_duty_cycle = 1;
    // system.core?.predictor
    sys.core[i].predictor.prediction_width = 1;
    strcpy(sys.core[i].predictor.prediction_scheme, "default");
    sys.core[i].predictor.predictor_size = 1;
    sys.core[i].predictor.predictor_entries = 1;
    sys.core[i].predictor.local_predictor_entries = 1;
    for (j = 0; j < 20; j++)
      sys.core[i].predictor.local_predictor_size[j] = 1;
    sys.core[i].predictor.global_predictor_entries = 1;
    sys.core[i].predictor.global_predictor_bits = 1;
    sys.core[i].predictor.chooser_predictor_entries = 1;
    sys.core[i].predictor.chooser_predictor_bits = 1;
    sys.core[i].predictor.predictor_accesses = 1;
    // system.core?.itlb
    sys.core[i].itlb.number_entries = 1;
    sys.core[i].itlb.total_hits = 1;
    sys.core[i].itlb.total_accesses = 1;
    sys.core[i].itlb.total_misses = 1;
    // system.core?.icache
    for (j = 0; j < 20; j++)
      sys.core[i].icache.icache_config[j] = 1;
    // strcpy(sys.core[i].icache.buffer_sizes,"default");
    sys.core[i].icache.total_accesses = 1;
    sys.core[i].icache.read_accesses = 1;
    sys.core[i].icache.read_misses = 1;
    sys.core[i].icache.replacements = 1;
    sys.core[i].icache.read_hits = 1;
    sys.core[i].icache.total_hits = 1;
    sys.core[i].icache.total_misses = 1;
    sys.core[i].icache.miss_buffer_access = 1;
    sys.core[i].icache.fill_buffer_accesses = 1;
    sys.core[i].icache.prefetch_buffer_accesses = 1;
    sys.core[i].icache.prefetch_buffer_writes = 1;
    sys.core[i].icache.prefetch_buffer_reads = 1;
    sys.core[i].icache.prefetch_buffer_hits = 1;
    // system.core?.dtlb
    sys.core[i].dtlb.number_entries = 1;
    sys.core[i].dtlb.total_accesses = 1;
    sys.core[i].dtlb.read_accesses = 1;
    sys.core[i].dtlb.write_accesses = 1;
    sys.core[i].dtlb.write_hits = 1;
    sys.core[i].dtlb.read_hits = 1;
    sys.core[i].dtlb.read_misses = 1;
    sys.core[i].dtlb.write_misses = 1;
    sys.core[i].dtlb.total_hits = 1;
    sys.core[i].dtlb.total_misses = 1;
    // system.core?.dcache
    for (j = 0; j < 20; j++)
      sys.core[i].dcache.dcache_config[j] = 1;
    // strcpy(sys.core[i].dcache.buffer_sizes,"default");
    sys.core[i].dcache.total_accesses = 1;
    sys.core[i].dcache.read_accesses = 1;
    sys.core[i].dcache.write_accesses = 1;
    sys.core[i].dcache.total_hits = 1;
    sys.core[i].dcache.total_misses = 1;
    sys.core[i].dcache.read_hits = 1;
    sys.core[i].dcache.write_hits = 1;
    sys.core[i].dcache.read_misses = 1;
    sys.core[i].dcache.write_misses = 1;
    sys.core[i].dcache.replacements = 1;
    sys.core[i].dcache.write_backs = 1;
    sys.core[i].dcache.miss_buffer_access = 1;
    sys.core[i].dcache.fill_buffer_accesses = 1;
    sys.core[i].dcache.prefetch_buffer_accesses = 1;
    sys.core[i].dcache.prefetch_buffer_writes = 1;
    sys.core[i].dcache.prefetch_buffer_reads = 1;
    sys.core[i].dcache.prefetch_buffer_hits = 1;
    sys.core[i].dcache.wbb_writes = 1;
    sys.core[i].dcache.wbb_reads = 1;
    // system.core?.BTB
    for (j = 0; j < 20; j++)
      sys.core[i].BTB.BTB_config[j] = 1;
    sys.core[i].BTB.total_accesses = 1;
    sys.core[i].BTB.read_accesses = 1;
    sys.core[i].BTB.write_accesses = 1;
    sys.core[i].BTB.total_hits = 1;
    sys.core[i].BTB.total_misses = 1;
    sys.core[i].BTB.read_hits = 1;
    sys.core[i].BTB.write_hits = 1;
    sys.core[i].BTB.read_misses = 1;
    sys.core[i].BTB.write_misses = 1;
    sys.core[i].BTB.replacements = 1;
  }

  // system_L1directory
  for (i = 0; i <= 63; i++) {
    for (j = 0; j < 20; j++)
      sys.L1Directory[i].Dir_config[j] = 1;
    for (j = 0; j < 20; j++)
      sys.L1Directory[i].buffer_sizes[j] = 1;
    sys.L1Directory[i].clockrate = 1;
    sys.L1Directory[i].ports[19] = 1;
    sys.L1Directory[i].device_type = 1;
    sys.L1Directory[i].vdd = 0;
    sys.L1Directory[i].power_gating_vcc = -1;
    strcpy(sys.L1Directory[i].threeD_stack, "default");
    sys.L1Directory[i].total_accesses = 1;
    sys.L1Directory[i].read_accesses = 1;
    sys.L1Directory[i].write_accesses = 1;
    sys.L1Directory[i].duty_cycle = 1;
  }
  // system_L2directory
  for (i = 0; i <= 63; i++) {
    for (j = 0; j < 20; j++)
      sys.L2Directory[i].Dir_config[j] = 1;
    for (j = 0; j < 20; j++)
      sys.L2Directory[i].buffer_sizes[j] = 1;
    sys.L2Directory[i].clockrate = 1;
    sys.L2Directory[i].ports[19] = 1;
    sys.L2Directory[i].device_type = 1;
    sys.L2Directory[i].vdd = 0;
    sys.L2Directory[i].power_gating_vcc = -1;
    strcpy(sys.L2Directory[i].threeD_stack, "default");
    sys.L2Directory[i].total_accesses = 1;
    sys.L2Directory[i].read_accesses = 1;
    sys.L2Directory[i].write_accesses = 1;
    sys.L2Directory[i].duty_cycle = 1;
  }
  for (i = 0; i <= 63; i++) {
    // system_L2
    for (j = 0; j < 20; j++)
      sys.L2[i].L2_config[j] = 1;
    sys.L2[i].clockrate = 1;
    for (j = 0; j < 20; j++)
      sys.L2[i].ports[j] = 1;
    sys.L2[i].device_type = 1;
    sys.L2[i].vdd = 0;
    sys.L2[i].power_gating_vcc = -1;
    strcpy(sys.L2[i].threeD_stack, "default");
    for (j = 0; j < 20; j++)
      sys.L2[i].buffer_sizes[j] = 1;
    sys.L2[i].total_accesses = 1;
    sys.L2[i].read_accesses = 1;
    sys.L2[i].write_accesses = 1;
    sys.L2[i].total_hits = 1;
    sys.L2[i].total_misses = 1;
    sys.L2[i].read_hits = 1;
    sys.L2[i].write_hits = 1;
    sys.L2[i].read_misses = 1;
    sys.L2[i].write_misses = 1;
    sys.L2[i].replacements = 1;
    sys.L2[i].write_backs = 1;
    sys.L2[i].miss_buffer_accesses = 1;
    sys.L2[i].fill_buffer_accesses = 1;
    sys.L2[i].prefetch_buffer_accesses = 1;
    sys.L2[i].prefetch_buffer_writes = 1;
    sys.L2[i].prefetch_buffer_reads = 1;
    sys.L2[i].prefetch_buffer_hits = 1;
    sys.L2[i].wbb_writes = 1;
    sys.L2[i].wbb_reads = 1;
    sys.L2[i].duty_cycle = 1;
    sys.L2[i].merged_dir = false;
    sys.L2[i].homenode_read_accesses = 1;
    sys.L2[i].homenode_write_accesses = 1;
    sys.L2[i].homenode_read_hits = 1;
    sys.L2[i].homenode_write_hits = 1;
    sys.L2[i].homenode_read_misses = 1;
    sys.L2[i].homenode_write_misses = 1;
    sys.L2[i].dir_duty_cycle = 1;
  }
  for (i = 0; i <= 63; i++) {
    // system_L3
    for (j = 0; j < 20; j++)
      sys.L3[i].L3_config[j] = 1;
    sys.L3[i].clockrate = 1;
    for (j = 0; j < 20; j++)
      sys.L3[i].ports[j] = 1;
    sys.L3[i].device_type = 1;
    sys.L3[i].vdd = 0;
    sys.L2[i].power_gating_vcc = -1;
    strcpy(sys.L3[i].threeD_stack, "default");
    for (j = 0; j < 20; j++)
      sys.L3[i].buffer_sizes[j] = 1;
    sys.L3[i].total_accesses = 1;
    sys.L3[i].read_accesses = 1;
    sys.L3[i].write_accesses = 1;
    sys.L3[i].total_hits = 1;
    sys.L3[i].total_misses = 1;
    sys.L3[i].read_hits = 1;
    sys.L3[i].write_hits = 1;
    sys.L3[i].read_misses = 1;
    sys.L3[i].write_misses = 1;
    sys.L3[i].replacements = 1;
    sys.L3[i].write_backs = 1;
    sys.L3[i].miss_buffer_accesses = 1;
    sys.L3[i].fill_buffer_accesses = 1;
    sys.L3[i].prefetch_buffer_accesses = 1;
    sys.L3[i].prefetch_buffer_writes = 1;
    sys.L3[i].prefetch_buffer_reads = 1;
    sys.L3[i].prefetch_buffer_hits = 1;
    sys.L3[i].wbb_writes = 1;
    sys.L3[i].wbb_reads = 1;
    sys.L3[i].duty_cycle = 1;
    sys.L3[i].merged_dir = false;
    sys.L3[i].homenode_read_accesses = 1;
    sys.L3[i].homenode_write_accesses = 1;
    sys.L3[i].homenode_read_hits = 1;
    sys.L3[i].homenode_write_hits = 1;
    sys.L3[i].homenode_read_misses = 1;
    sys.L3[i].homenode_write_misses = 1;
    sys.L3[i].dir_duty_cycle = 1;
  }
  // system_NoC
  for (i = 0; i <= 63; i++) {
    sys.NoC[i].clockrate = 1;
    sys.NoC[i].type = true;
    sys.NoC[i].chip_coverage = 1;
    sys.NoC[i].vdd = 0;
    sys.NoC[i].power_gating_vcc = -1;
    sys.NoC[i].has_global_link = true;
    strcpy(sys.NoC[i].topology, "default");
    sys.NoC[i].horizontal_nodes = 1;
    sys.NoC[i].vertical_nodes = 1;
    sys.NoC[i].input_ports = 1;
    sys.NoC[i].output_ports = 1;
    sys.NoC[i].virtual_channel_per_port = 1;
    sys.NoC[i].flit_bits = 1;
    sys.NoC[i].input_buffer_entries_per_vc = 1;
    sys.NoC[i].total_accesses = 1;
    sys.NoC[i].duty_cycle = 1;
    sys.NoC[i].route_over_perc = 0.5;
    for (j = 0; j < 20; j++)
      sys.NoC[i].ports_of_input_buffer[j] = 1;
    sys.NoC[i].number_of_crossbars = 1;
    strcpy(sys.NoC[i].crossbar_type, "default");
    strcpy(sys.NoC[i].crosspoint_type, "default");
    // system.NoC?.xbar0;
    sys.NoC[i].xbar0.number_of_inputs_of_crossbars = 1;
    sys.NoC[i].xbar0.number_of_outputs_of_crossbars = 1;
    sys.NoC[i].xbar0.flit_bits = 1;
    sys.NoC[i].xbar0.input_buffer_entries_per_port = 1;
    sys.NoC[i].xbar0.ports_of_input_buffer[19] = 1;
    sys.NoC[i].xbar0.crossbar_accesses = 1;
  }
  // system_mem
  sys.mem.mem_tech_node = 1;
  sys.mem.device_clock = 1;
  sys.mem.capacity_per_channel = 1;
  sys.mem.number_ranks = 1;
  sys.mem.peak_transfer_rate = 1;
  sys.mem.num_banks_of_DRAM_chip = 1;
  sys.mem.Block_width_of_DRAM_chip = 1;
  sys.mem.output_width_of_DRAM_chip = 1;
  sys.mem.page_size_of_DRAM_chip = 1;
  sys.mem.burstlength_of_DRAM_chip = 1;
  sys.mem.internal_prefetch_of_DRAM_chip = 1;
  sys.mem.memory_accesses = 1;
  sys.mem.memory_reads = 1;
  sys.mem.memory_writes = 1;
  // system_mc
  sys.mc.mc_clock = 1;
  sys.mc.number_mcs = 1;
  sys.mc.peak_transfer_rate = 1;
  sys.mc.memory_channels_per_mc = 1;
  sys.mc.number_ranks = 1;
  sys.mc.req_window_size_per_channel = 1;
  sys.mc.IO_buffer_size_per_channel = 1;
  sys.mc.databus_width = 1;
  sys.mc.addressbus_width = 1;
  sys.mc.memory_accesses = 1;
  sys.mc.memory_reads = 1;
  sys.mc.memory_writes = 1;
  sys.mc.LVDS = true;
  sys.mc.type = 1;
  sys.mc.vdd = 0;
  sys.mc.power_gating_vcc = -1;
  // system_niu
  sys.niu.clockrate = 1;
  sys.niu.number_units = 1;
  sys.niu.type = 1;
  sys.niu.vdd = 0;
  sys.niu.power_gating_vcc = -1;
  sys.niu.duty_cycle = 1;
  sys.niu.total_load_perc = 1;
  // system_pcie
  sys.pcie.clockrate = 1;
  sys.pcie.number_units = 1;
  sys.pcie.num_channels = 1;
  sys.pcie.type = 1;
  sys.pcie.vdd = 0;
  sys.pcie.power_gating_vcc = -1;
  sys.pcie.withPHY = false;
  sys.pcie.duty_cycle = 1;
  sys.pcie.total_load_perc = 1;
  // system_flash_controller
  sys.flashc.mc_clock = 1;
  sys.flashc.number_mcs = 1;
  sys.flashc.vdd = 0;
  sys.flashc.power_gating_vcc = -1;
  sys.flashc.peak_transfer_rate = 1;
  sys.flashc.memory_channels_per_mc = 1;
  sys.flashc.number_ranks = 1;
  sys.flashc.req_window_size_per_channel = 1;
  sys.flashc.IO_buffer_size_per_channel = 1;
  sys.flashc.databus_width = 1;
  sys.flashc.addressbus_width = 1;
  sys.flashc.memory_accesses = 1;
  sys.flashc.memory_reads = 1;
  sys.flashc.memory_writes = 1;
  sys.flashc.LVDS = true;
  sys.flashc.withPHY = false;
  sys.flashc.type = 1;
  sys.flashc.duty_cycle = 1;
  sys.flashc.total_load_perc = 1;
}
