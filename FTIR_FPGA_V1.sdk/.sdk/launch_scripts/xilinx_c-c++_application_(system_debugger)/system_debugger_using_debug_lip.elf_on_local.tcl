connect -url tcp:127.0.0.1:3121
source F:/Awork_space/LF398_Debug/Xilinx/ftir_fpga_design/FTIR_FPGA_V1/FTIR_FPGA_V1.sdk/design_1_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS1 210512180081"} -index 0
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-HS1 210512180081" && level==0} -index 1
fpga -file F:/Awork_space/LF398_Debug/Xilinx/ftir_fpga_design/FTIR_FPGA_V1/FTIR_FPGA_V1.sdk/design_1_wrapper_hw_platform_0/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS1 210512180081"} -index 0
loadhw -hw F:/Awork_space/LF398_Debug/Xilinx/ftir_fpga_design/FTIR_FPGA_V1/FTIR_FPGA_V1.sdk/design_1_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-HS1 210512180081"} -index 0
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS1 210512180081"} -index 0
dow F:/Awork_space/LF398_Debug/Xilinx/ftir_fpga_design/FTIR_FPGA_V1/FTIR_FPGA_V1.sdk/lip/Debug/lip.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-HS1 210512180081"} -index 0
con
