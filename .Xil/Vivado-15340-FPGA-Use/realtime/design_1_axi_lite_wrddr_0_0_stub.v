// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "axi_lite_wrddr_v1_0,Vivado 2018.3" *)
module design_1_axi_lite_wrddr_0_0(wr_data, wr_addr, addr_offset, m00_axi_awaddr, 
  m00_axi_awprot, m00_axi_awvalid, m00_axi_awready, m00_axi_wdata, m00_axi_wstrb, 
  m00_axi_wvalid, m00_axi_wready, m00_axi_bresp, m00_axi_bvalid, m00_axi_bready, 
  m00_axi_araddr, m00_axi_arprot, m00_axi_arvalid, m00_axi_arready, m00_axi_rdata, 
  m00_axi_rresp, m00_axi_rvalid, m00_axi_rready, m00_axi_aclk, m00_axi_aresetn, 
  m00_axi_init_axi_txn, m00_axi_error, m00_axi_txn_done);
  input [63:0]wr_data;
  input [31:0]wr_addr;
  input [31:0]addr_offset;
  output [31:0]m00_axi_awaddr;
  output [2:0]m00_axi_awprot;
  output m00_axi_awvalid;
  input m00_axi_awready;
  output [31:0]m00_axi_wdata;
  output [3:0]m00_axi_wstrb;
  output m00_axi_wvalid;
  input m00_axi_wready;
  input [1:0]m00_axi_bresp;
  input m00_axi_bvalid;
  output m00_axi_bready;
  output [31:0]m00_axi_araddr;
  output [2:0]m00_axi_arprot;
  output m00_axi_arvalid;
  input m00_axi_arready;
  input [31:0]m00_axi_rdata;
  input [1:0]m00_axi_rresp;
  input m00_axi_rvalid;
  output m00_axi_rready;
  input m00_axi_aclk;
  input m00_axi_aresetn;
  input m00_axi_init_axi_txn;
  output m00_axi_error;
  output m00_axi_txn_done;
endmodule
