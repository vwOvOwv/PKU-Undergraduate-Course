import m5
from m5.objects import *
import os
import argparse

cpu_types = {
    "simple": TimingSimpleCPU,
    "minor": MinorCPU,
    "o3": O3CPU,
}

mem_types = {
    "DDR3": DDR3_1600_8x8,
    "DDR4": DDR4_2400_8x8,
    "DDR5": DDR5_8400_4x8,
}

# Set up clock
# def init_system(system):
def init_system(system, args):
    # system.clk_domain = SrcClockDomain(clock='4GHz', voltage_domain=VoltageDomain())
    system.clk_domain = SrcClockDomain(clock=args.clock, voltage_domain=VoltageDomain())

    # Create the memory and the memory controller
    system.mem_mode = 'timing'  # timing mode
    system.mem_ranges = [AddrRange ('2GB')] # a single memory range of size 2GB 
    system.mem_ctrl = MemCtrl() # memory controller 
    # system.mem_ctrl.dram = DDR4_2400_8x8()  # memory device
    system.mem_ctrl.dram = mem_types[args.mem]()
    system.mem_ctrl.dram.range = root.system.mem_ranges[0]  # assign the memory device with the memory range

    # Create a CPU
    # system.cpu = TimingSimpleCPU()
    system.cpu = cpu_types[args.cpu]()
    system.cpu.max_insts_any_thread = 5e+8

    # Create a system bus
    system.membus = SystemXBar()

    # Describe the interconnect logic
    system.cpu.icache_port = system.membus.cpu_side_ports
    system.cpu.dcache_port = system.membus.cpu_side_ports
    system.mem_ctrl.port = system.membus.mem_side_ports
    system.cpu.createInterruptController()
    system.system_port = system.membus.cpu_side_ports
    # we use ARM ISA and ignore following requirements
    # system.cpu.interrupts[0].pio = system.membus.mem_side_ports
    # system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
    # system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

# Create a process
# def init_process(root):
def init_process(root, args):
    # exe_path = 'tests/labexe/hello'
    exe_path = args.commands_to_run[0]
    root.system.workload = SEWorkload.init_compatible(exe_path)
    process = Process()
    process.executable = exe_path
    process.cwd = os.getcwd()
    # process.cmd = [exe_path]
    process.cmd = args.commands_to_run
    root.system.cpu.workload = process
    root.system.cpu.createThreads() 

if __name__ == '__m5_main__':
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "commands_to_run",
        nargs="*",
        help="Command(s) to run",
    )
    parser.add_argument(
        "--cpu",
        type=str,
        choices=list(cpu_types.keys()),
        default="simple",
        help="CPU model to use",
    )
    parser.add_argument(
        "--mem",
        type=str,
        choices=list(mem_types.keys()),
        default="DDR4",
        help="type of memory to use",
    )
    parser.add_argument(
        "--clock",
        type=str,
        default="4GHz",
    )
    args = parser.parse_args()

    root = Root()
    root.full_system = False
    root.system = System()

    init_system(root.system, args)
    init_process(root, args)
    m5.instantiate()
    exit_event = m5.simulate()
    print(f'{exit_event.getCause()} ({exit_event.getCode()}) @ {m5.curTick()}')
