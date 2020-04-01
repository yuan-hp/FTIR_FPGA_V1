

set_property IOSTANDARD LVCMOS33 [get_ports pwm_0]
set_property PACKAGE_PIN V19 [get_ports pwm_0]


set_property IOSTANDARD LVCMOS33 [get_ports FPGA_LASER_PULSE]
set_property PACKAGE_PIN AA9 [get_ports FPGA_LASER_PULSE]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_LOGIC_LF398]
set_property PACKAGE_PIN T21 [get_ports FPGA_LOGIC_LF398]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_SCLK]
set_property PACKAGE_PIN Y6 [get_ports FPGA_AD_SCLK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_LRCK]
set_property PACKAGE_PIN V12 [get_ports FPGA_AD_LRCK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_SDATA]
set_property PACKAGE_PIN Y5 [get_ports FPGA_AD_SDATA]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_MCLK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_ZCAL]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_RSTn]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_SPI0_SCLK]
set_property PACKAGE_PIN W5 [get_ports FPGA_SPI0_SCLK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_SPI0_SDO]
set_property PACKAGE_PIN AB20 [get_ports FPGA_SPI0_SDO]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_SPI0_CSn0]
set_property PACKAGE_PIN W12 [get_ports FPGA_SPI0_CSn0]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_UART0_RX]
set_property PACKAGE_PIN AB7 [get_ports FPGA_UART0_RX]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_UART0_TX]
set_property PACKAGE_PIN AB6 [get_ports FPGA_UART0_TX]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_DJ_Dir_IN]
set_property PACKAGE_PIN AA8 [get_ports FPGA_DJ_Dir_IN]

set_property BITSTREAM.CONFIG.UNUSEDPIN Pullnone [current_design]


create_clock -period 162.760 -name FPGA_AD_SCLK -waveform {0.000 81.380} [get_ports FPGA_AD_SCLK]

set_property IOSTANDARD LVCMOS33 [get_ports LED2]
set_property IOSTANDARD LVCMOS33 [get_ports LED4]
set_property IOSTANDARD LVCMOS33 [get_ports LED3]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_FSYNC]
set_property PACKAGE_PIN V18 [get_ports LED2]
set_property PACKAGE_PIN U17 [get_ports LED3]
set_property PACKAGE_PIN V17 [get_ports LED4]

set_property PACKAGE_PIN U21 [get_ports FPGA_DongJ_PWM_C0]
set_property PACKAGE_PIN U20 [get_ports FPGA_DongJ_PWM_C1]
set_property PACKAGE_PIN V20 [get_ports FPGA_PWM_C2]
set_property IOSTANDARD LVCMOS33 [get_ports FPGA_DongJ_PWM_C0]
set_property IOSTANDARD LVCMOS33 [get_ports FPGA_DongJ_PWM_C1]
set_property IOSTANDARD LVCMOS33 [get_ports FPGA_PWM_C2]
