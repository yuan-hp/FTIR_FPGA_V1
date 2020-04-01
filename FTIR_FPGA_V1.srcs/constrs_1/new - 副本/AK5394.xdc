set_property IOSTANDARD LVCMOS33 [get_ports FPGA_LOGIC_LF398]
set_property PACKAGE_PIN V12 [get_ports FPGA_LOGIC_LF398]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_SCLK]
set_property PACKAGE_PIN Y6 [get_ports FPGA_AD_SCLK]
create_clock -period 162.760 -name FPGA_AD_SCLK -waveform {0.000 81.380} [get_ports FPGA_AD_SCLK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_LRCK]
set_property PACKAGE_PIN Y8 [get_ports FPGA_AD_LRCK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_SDATA]
set_property PACKAGE_PIN Y8 [get_ports FPGA_AD_LRCK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_MCLK]
set_property PACKAGE_PIN M16 [get_ports FPGA_AD_MCLK]
set_property SLEW FAST [get_ports FPGA_AD_MCLK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_ZCAL]
set_property PACKAGE_PIN V7 [get_ports FPGA_AD_ZCAL]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_AD_RSTn]
set_property PACKAGE_PIN W5 [get_ports FPGA_AD_RSTn]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_SPI0_SCLK]
set_property PACKAGE_PIN AB19 [get_ports FPGA_SPI0_SCLK]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_SPI0_SDO]
set_property PACKAGE_PIN AB20 [get_ports FPGA_SPI0_SDO]

set_property IOSTANDARD LVCMOS33 [get_ports FPGA_SPI0_CSn0]
set_property PACKAGE_PIN W12 [get_ports FPGA_SPI0_CSn0]
