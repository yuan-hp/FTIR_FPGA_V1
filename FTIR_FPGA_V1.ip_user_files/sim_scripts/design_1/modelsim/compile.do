vlib modelsim_lib/work
vlib modelsim_lib/msim

vlib modelsim_lib/msim/xilinx_vip
vlib modelsim_lib/msim/xil_defaultlib
vlib modelsim_lib/msim/xpm
vlib modelsim_lib/msim/axi_infrastructure_v1_1_0
vlib modelsim_lib/msim/axi_vip_v1_1_4
vlib modelsim_lib/msim/processing_system7_vip_v1_0_6
vlib modelsim_lib/msim/axi_lite_ipif_v3_0_4
vlib modelsim_lib/msim/lib_cdc_v1_0_2
vlib modelsim_lib/msim/interrupt_control_v3_1_4
vlib modelsim_lib/msim/axi_gpio_v2_0_20
vlib modelsim_lib/msim/proc_sys_reset_v5_0_13
vlib modelsim_lib/msim/generic_baseblocks_v2_1_0
vlib modelsim_lib/msim/axi_register_slice_v2_1_18
vlib modelsim_lib/msim/fifo_generator_v13_2_3
vlib modelsim_lib/msim/axi_data_fifo_v2_1_17
vlib modelsim_lib/msim/axi_crossbar_v2_1_19
vlib modelsim_lib/msim/xlconcat_v2_1_1
vlib modelsim_lib/msim/axi_protocol_converter_v2_1_18

vmap xilinx_vip modelsim_lib/msim/xilinx_vip
vmap xil_defaultlib modelsim_lib/msim/xil_defaultlib
vmap xpm modelsim_lib/msim/xpm
vmap axi_infrastructure_v1_1_0 modelsim_lib/msim/axi_infrastructure_v1_1_0
vmap axi_vip_v1_1_4 modelsim_lib/msim/axi_vip_v1_1_4
vmap processing_system7_vip_v1_0_6 modelsim_lib/msim/processing_system7_vip_v1_0_6
vmap axi_lite_ipif_v3_0_4 modelsim_lib/msim/axi_lite_ipif_v3_0_4
vmap lib_cdc_v1_0_2 modelsim_lib/msim/lib_cdc_v1_0_2
vmap interrupt_control_v3_1_4 modelsim_lib/msim/interrupt_control_v3_1_4
vmap axi_gpio_v2_0_20 modelsim_lib/msim/axi_gpio_v2_0_20
vmap proc_sys_reset_v5_0_13 modelsim_lib/msim/proc_sys_reset_v5_0_13
vmap generic_baseblocks_v2_1_0 modelsim_lib/msim/generic_baseblocks_v2_1_0
vmap axi_register_slice_v2_1_18 modelsim_lib/msim/axi_register_slice_v2_1_18
vmap fifo_generator_v13_2_3 modelsim_lib/msim/fifo_generator_v13_2_3
vmap axi_data_fifo_v2_1_17 modelsim_lib/msim/axi_data_fifo_v2_1_17
vmap axi_crossbar_v2_1_19 modelsim_lib/msim/axi_crossbar_v2_1_19
vmap xlconcat_v2_1_1 modelsim_lib/msim/xlconcat_v2_1_1
vmap axi_protocol_converter_v2_1_18 modelsim_lib/msim/axi_protocol_converter_v2_1_18

vlog -work xilinx_vip -64 -incr -sv -L axi_vip_v1_1_4 -L processing_system7_vip_v1_0_6 -L xilinx_vip "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi_vip_if.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/clk_vip_if.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/rst_vip_if.sv" \

vlog -work xil_defaultlib -64 -incr -sv -L axi_vip_v1_1_4 -L processing_system7_vip_v1_0_6 -L xilinx_vip "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \

vcom -work xpm -64 -93 \
"D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_VCOMP.vhd" \

vlog -work axi_infrastructure_v1_1_0 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \

vlog -work axi_vip_v1_1_4 -64 -incr -sv -L axi_vip_v1_1_4 -L processing_system7_vip_v1_0_6 -L xilinx_vip "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/98af/hdl/axi_vip_v1_1_vl_rfs.sv" \

vlog -work processing_system7_vip_v1_0_6 -64 -incr -sv -L axi_vip_v1_1_4 -L processing_system7_vip_v1_0_6 -L xilinx_vip "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl/processing_system7_vip_v1_0_vl_rfs.sv" \

vlog -work xil_defaultlib -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_processing_system7_0_0/sim/design_1_processing_system7_0_0.v" \
"../../../bd/design_1/ipshared/1cbb/src/ax_pwm.v" \
"../../../bd/design_1/ipshared/1cbb/hdl/ax_pwm_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/1cbb/hdl/ax_pwm_v1_0.v" \
"../../../bd/design_1/ip/design_1_ax_pwm_0_1/sim/design_1_ax_pwm_0_1.v" \
"../../../bd/design_1/ipshared/54e5/src/AK5394_2QSW.v" \
"../../../bd/design_1/ipshared/54e5/hdl/AK5394_LF398_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/54e5/src/fifo_hpy.v" \
"../../../bd/design_1/ipshared/54e5/hdl/AK5394_LF398_v1_0.v" \
"../../../bd/design_1/ip/design_1_AK5394_LF398_0_0/sim/design_1_AK5394_LF398_0_0.v" \

vcom -work axi_lite_ipif_v3_0_4 -64 -93 \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/66ea/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \

vcom -work lib_cdc_v1_0_2 -64 -93 \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ef1e/hdl/lib_cdc_v1_0_rfs.vhd" \

vcom -work interrupt_control_v3_1_4 -64 -93 \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/a040/hdl/interrupt_control_v3_1_vh_rfs.vhd" \

vcom -work axi_gpio_v2_0_20 -64 -93 \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/a7c9/hdl/axi_gpio_v2_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93 \
"../../../bd/design_1/ip/design_1_axi_gpio_0_0/sim/design_1_axi_gpio_0_0.vhd" \

vcom -work proc_sys_reset_v5_0_13 -64 -93 \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/8842/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \

vcom -work xil_defaultlib -64 -93 \
"../../../bd/design_1/ip/design_1_rst_ps7_0_142M_1/sim/design_1_rst_ps7_0_142M_1.vhd" \

vlog -work generic_baseblocks_v2_1_0 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/b752/hdl/generic_baseblocks_v2_1_vl_rfs.v" \

vlog -work axi_register_slice_v2_1_18 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/cc23/hdl/axi_register_slice_v2_1_vl_rfs.v" \

vlog -work fifo_generator_v13_2_3 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/64f4/simulation/fifo_generator_vlog_beh.v" \

vcom -work fifo_generator_v13_2_3 -64 -93 \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/64f4/hdl/fifo_generator_v13_2_rfs.vhd" \

vlog -work fifo_generator_v13_2_3 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/64f4/hdl/fifo_generator_v13_2_rfs.v" \

vlog -work axi_data_fifo_v2_1_17 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/c4fd/hdl/axi_data_fifo_v2_1_vl_rfs.v" \

vlog -work axi_crossbar_v2_1_19 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/6c9d/hdl/axi_crossbar_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xbar_0/sim/design_1_xbar_0.v" \
"../../../bd/design_1/ipshared/e648/hdl/axi_lite_wrddr_v1_0_M00_AXI.v" \
"../../../bd/design_1/ipshared/e648/hdl/axi_lite_wrddr_v1_0.v" \
"../../../bd/design_1/ip/design_1_axi_lite_wrddr_0_0/sim/design_1_axi_lite_wrddr_0_0.v" \
"../../../bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0_clk_wiz.v" \
"../../../bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0.v" \

vcom -work xil_defaultlib -64 -93 \
"../../../bd/design_1/ip/design_1_axi_gpio_1_0/sim/design_1_axi_gpio_1_0.vhd" \

vlog -work xil_defaultlib -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../bd/design_1/ipshared/6881/hdl/FPGA_PWM_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/6881/hdl/FPGA_PWM_v1_0.v" \
"../../../bd/design_1/ip/design_1_FPGA_PWM_0_0/sim/design_1_FPGA_PWM_0_0.v" \
"../../../bd/design_1/ipshared/3b33/hdl/axi_debug_io_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/3b33/hdl/axi_debug_io_v1_0.v" \
"../../../bd/design_1/ip/design_1_axi_debug_io_0_0/sim/design_1_axi_debug_io_0_0.v" \
"../../../bd/design_1/ipshared/d984/hdl/DAC7631_V2.v" \
"../../../bd/design_1/ipshared/d984/hdl/DAC7631_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/d984/src/clk_div.v" \
"../../../bd/design_1/ipshared/d984/hdl/DAC7631_v1_0.v" \
"../../../bd/design_1/ip/design_1_DAC7631_0_0/sim/design_1_DAC7631_0_0.v" \
"../../../bd/design_1/ipshared/944a/hdl/FreqMeasure_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/944a/hdl/FreqMeasure_v1_0.v" \
"../../../bd/design_1/ip/design_1_FreqMeasure_0_0/sim/design_1_FreqMeasure_0_0.v" \

vlog -work xlconcat_v2_1_1 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/2f66/hdl/xlconcat_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_xlconcat_0_0/sim/design_1_xlconcat_0_0.v" \
"../../../bd/design_1/ipshared/2476/hdl/AXI_SIG_CFG_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/2476/hdl/cut_sig.v" \
"../../../bd/design_1/ipshared/2476/hdl/AXI_SIG_CFG_v1_0.v" \
"../../../bd/design_1/ip/design_1_AXI_SIG_CFG_0_0/sim/design_1_AXI_SIG_CFG_0_0.v" \
"../../../bd/design_1/ipshared/ffd4/src/DHT11.v" \
"../../../bd/design_1/ipshared/ffd4/hdl/Sensor_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/ffd4/hdl/Sensor_v1_0.v" \
"../../../bd/design_1/ip/design_1_Sensor_0_0/sim/design_1_Sensor_0_0.v" \
"../../../bd/design_1/ipshared/baac/hdl/TAS5111_v1_0_S00_AXI.v" \
"../../../bd/design_1/ipshared/baac/hdl/TAS5111_v1_0.v" \
"../../../bd/design_1/ip/design_1_TAS5111_0_0/sim/design_1_TAS5111_0_0.v" \
"../../../bd/design_1/sim/design_1.v" \

vlog -work axi_protocol_converter_v2_1_18 -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/7a04/hdl/axi_protocol_converter_v2_1_vl_rfs.v" \

vlog -work xil_defaultlib -64 -incr "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/85a3" "+incdir+../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ip/design_1_processing_system7_0_0" "+incdir+D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/include" \
"../../../bd/design_1/ip/design_1_auto_pc_0/sim/design_1_auto_pc_0.v" \
"../../../bd/design_1/ip/design_1_s00_data_fifo_0/sim/design_1_s00_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_auto_pc_1/sim/design_1_auto_pc_1.v" \
"../../../bd/design_1/ip/design_1_s01_data_fifo_0/sim/design_1_s01_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_auto_pc_2/sim/design_1_auto_pc_2.v" \
"../../../bd/design_1/ip/design_1_m00_data_fifo_0/sim/design_1_m00_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m01_data_fifo_0/sim/design_1_m01_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m02_data_fifo_0/sim/design_1_m02_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m03_data_fifo_0/sim/design_1_m03_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m04_data_fifo_0/sim/design_1_m04_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m05_data_fifo_0/sim/design_1_m05_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m06_data_fifo_0/sim/design_1_m06_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m07_data_fifo_0/sim/design_1_m07_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m08_data_fifo_0/sim/design_1_m08_data_fifo_0.v" \
"../../../bd/design_1/ip/design_1_m09_data_fifo_0/sim/design_1_m09_data_fifo_0.v" \

vlog -work xil_defaultlib \
"glbl.v"

