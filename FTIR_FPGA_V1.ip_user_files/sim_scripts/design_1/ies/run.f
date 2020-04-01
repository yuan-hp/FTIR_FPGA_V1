-makelib ies_lib/xilinx_vip -sv \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi4stream_vip_axi4streampc.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi_vip_axi4pc.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/xil_common_vip_pkg.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi4stream_vip_pkg.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi_vip_pkg.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi4stream_vip_if.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/axi_vip_if.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/clk_vip_if.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/xilinx_vip/hdl/rst_vip_if.sv" \
-endlib
-makelib ies_lib/xil_defaultlib -sv \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \
-endlib
-makelib ies_lib/xpm \
  "D:/yhpsoft/Xilinx/vivado2018/Vivado/2018.3/data/ip/xpm/xpm_VCOMP.vhd" \
-endlib
-makelib ies_lib/axi_infrastructure_v1_1_0 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \
-endlib
-makelib ies_lib/axi_vip_v1_1_4 -sv \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/98af/hdl/axi_vip_v1_1_vl_rfs.sv" \
-endlib
-makelib ies_lib/processing_system7_vip_v1_0_6 -sv \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/70cf/hdl/processing_system7_vip_v1_0_vl_rfs.sv" \
-endlib
-makelib ies_lib/xil_defaultlib \
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
-endlib
-makelib ies_lib/axi_lite_ipif_v3_0_4 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/66ea/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \
-endlib
-makelib ies_lib/lib_cdc_v1_0_2 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/ef1e/hdl/lib_cdc_v1_0_rfs.vhd" \
-endlib
-makelib ies_lib/interrupt_control_v3_1_4 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/a040/hdl/interrupt_control_v3_1_vh_rfs.vhd" \
-endlib
-makelib ies_lib/axi_gpio_v2_0_20 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/a7c9/hdl/axi_gpio_v2_0_vh_rfs.vhd" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/design_1/ip/design_1_axi_gpio_0_0/sim/design_1_axi_gpio_0_0.vhd" \
-endlib
-makelib ies_lib/proc_sys_reset_v5_0_13 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/8842/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/design_1/ip/design_1_rst_ps7_0_142M_1/sim/design_1_rst_ps7_0_142M_1.vhd" \
-endlib
-makelib ies_lib/generic_baseblocks_v2_1_0 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/b752/hdl/generic_baseblocks_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/axi_register_slice_v2_1_18 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/cc23/hdl/axi_register_slice_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/fifo_generator_v13_2_3 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/64f4/simulation/fifo_generator_vlog_beh.v" \
-endlib
-makelib ies_lib/fifo_generator_v13_2_3 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/64f4/hdl/fifo_generator_v13_2_rfs.vhd" \
-endlib
-makelib ies_lib/fifo_generator_v13_2_3 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/64f4/hdl/fifo_generator_v13_2_rfs.v" \
-endlib
-makelib ies_lib/axi_data_fifo_v2_1_17 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/c4fd/hdl/axi_data_fifo_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/axi_crossbar_v2_1_19 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/6c9d/hdl/axi_crossbar_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/design_1/ip/design_1_xbar_0/sim/design_1_xbar_0.v" \
  "../../../bd/design_1/ipshared/e648/hdl/axi_lite_wrddr_v1_0_M00_AXI.v" \
  "../../../bd/design_1/ipshared/e648/hdl/axi_lite_wrddr_v1_0.v" \
  "../../../bd/design_1/ip/design_1_axi_lite_wrddr_0_0/sim/design_1_axi_lite_wrddr_0_0.v" \
  "../../../bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0_clk_wiz.v" \
  "../../../bd/design_1/ip/design_1_clk_wiz_0_0/design_1_clk_wiz_0_0.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/design_1/ip/design_1_axi_gpio_1_0/sim/design_1_axi_gpio_1_0.vhd" \
-endlib
-makelib ies_lib/xil_defaultlib \
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
-endlib
-makelib ies_lib/xlconcat_v2_1_1 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/2f66/hdl/xlconcat_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
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
-endlib
-makelib ies_lib/axi_protocol_converter_v2_1_18 \
  "../../../../FTIR_FPGA_V1.srcs/sources_1/bd/design_1/ipshared/7a04/hdl/axi_protocol_converter_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
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
-endlib
-makelib ies_lib/xil_defaultlib \
  glbl.v
-endlib

