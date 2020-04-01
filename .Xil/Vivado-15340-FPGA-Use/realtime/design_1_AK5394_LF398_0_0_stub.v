// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "AK5394_LF398_v1_0,Vivado 2018.3" *)
module design_1_AK5394_LF398_0_0(PLL_CLK, FPGA_DongJ_DIR, FPGA_LASER_PULSE, 
  FPGA_LOGIC_LF398, FPGA_AD_SCLK, FPGA_AD_LRCK, FPGA_AD_SDATA, FPGA_AD_FSYNC, FPGA_AD_MCLK, 
  FPGA_AD_ZCAL, FPGA_AD_RSTn, ready, miss_data_led, addr_offset, ddr_write_done_arc, 
  wr_data_ready, wr_data, wraddr_base, ddr_ad_data_inte, ddr_ad_chk, s00_axi_awaddr, 
  s00_axi_awprot, s00_axi_awvalid, s00_axi_awready, s00_axi_wdata, s00_axi_wstrb, 
  s00_axi_wvalid, s00_axi_wready, s00_axi_bresp, s00_axi_bvalid, s00_axi_bready, 
  s00_axi_araddr, s00_axi_arprot, s00_axi_arvalid, s00_axi_arready, s00_axi_rdata, 
  s00_axi_rresp, s00_axi_rvalid, s00_axi_rready, s00_axi_aclk, s00_axi_aresetn);
  input PLL_CLK;
  input FPGA_DongJ_DIR;
  input FPGA_LASER_PULSE;
  output FPGA_LOGIC_LF398;
  input FPGA_AD_SCLK;
  input FPGA_AD_LRCK;
  input FPGA_AD_SDATA;
  input FPGA_AD_FSYNC;
  output FPGA_AD_MCLK;
  output FPGA_AD_ZCAL;
  output FPGA_AD_RSTn;
  output ready;
  output miss_data_led;
  output [31:0]addr_offset;
  input ddr_write_done_arc;
  output wr_data_ready;
  output [63:0]wr_data;
  output [31:0]wraddr_base;
  output ddr_ad_data_inte;
  output ddr_ad_chk;
  input [3:0]s00_axi_awaddr;
  input [2:0]s00_axi_awprot;
  input s00_axi_awvalid;
  output s00_axi_awready;
  input [31:0]s00_axi_wdata;
  input [3:0]s00_axi_wstrb;
  input s00_axi_wvalid;
  output s00_axi_wready;
  output [1:0]s00_axi_bresp;
  output s00_axi_bvalid;
  input s00_axi_bready;
  input [3:0]s00_axi_araddr;
  input [2:0]s00_axi_arprot;
  input s00_axi_arvalid;
  output s00_axi_arready;
  output [31:0]s00_axi_rdata;
  output [1:0]s00_axi_rresp;
  output s00_axi_rvalid;
  input s00_axi_rready;
  input s00_axi_aclk;
  input s00_axi_aresetn;
endmodule
