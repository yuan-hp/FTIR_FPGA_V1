set_property IOSTANDARD LVCMOS33 [get_ports FPGA_TAS_PWM_A]
set_property PACKAGE_PIN U10 [get_ports FPGA_TAS_PWM_A]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_TAS_PWM_B]
set_property PACKAGE_PIN U6 [get_ports FPGA_TAS_PWM_B]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_TAS_RESETn]
set_property PACKAGE_PIN T6 [get_ports FPGA_TAS_RESETn]

set_property IOSTANDARD LVCMOS33 [get_ports {FPGA_PWM_0[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {FPGA_PWM_0[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {FPGA_PWM_0[0]}]
set_property PACKAGE_PIN R20 [get_ports {FPGA_PWM_0[0]}]

set_property IOSTANDARD LVCMOS33 [get_ports {FPGA_PWM_DIR_0[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {FPGA_PWM_DIR_0[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {FPGA_PWM_DIR_0[0]}]
set_property PACKAGE_PIN R21 [get_ports {FPGA_PWM_DIR_0[0]}]


set_property IOSTANDARD LVCMOS33 [get_ports FPGA_LASER_PULSE]
set_property PACKAGE_PIN AB1 [get_ports FPGA_LASER_PULSE]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_Switch]
set_property PACKAGE_PIN AB2 [get_ports FPGA_Switch]

set_property BITSTREAM.CONFIG.UNUSEDPIN Pullnone [current_design]

