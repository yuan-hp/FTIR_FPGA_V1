
`timescale 1 ns / 1 ps

	module AK5394_LF398_v1_0 #
	(
		// Users to add parameters here
		parameter integer DDR_BASE_ADDR	= 32'H04000000,
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXI
		parameter integer C_S00_AXI_DATA_WIDTH	= 32,
		parameter integer C_S00_AXI_ADDR_WIDTH	= 4
	)
	(
		// Users to add ports here
		input wire PLL_CLK,//输入时钟
        input  wire FPGA_DongJ_DIR,//动镜移动方向
    	input  wire FPGA_LASER_PULSE,  //激光检测器整形波输入
    	output wire FPGA_LOGIC_LF398,//LF398逻辑电平		
    	input  wire FPGA_AD_SCLK,  //CPLD_ADSCLK
    	input  wire FPGA_AD_LRCK,  //CPLD_ADFRAMESYN
    	input  wire FPGA_AD_SDATA, //CPLD_ADDATA
    	input  wire FPGA_AD_FSYNC,
    	output wire FPGA_AD_MCLK,  //主机时钟
    	output wire FPGA_AD_ZCAL,  //
    	output wire FPGA_AD_RSTn,  //
    	output wire ready,	
		output wire miss_data_led,
        //write ddr singel
        input wire  ddr_write_done_arc,
        output wire wr_data_ready,  
    	output wire [95:0]wr_data_buf,
    	output wire [95:0]wr_addr_buf,
    	output wire ddr_ad_data_inte,// posedege signal active
    	output wire ddr_ad_chk,
		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface S00_AXI
		input wire  s00_axi_aclk,
		input wire  s00_axi_aresetn,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_awaddr,
		input wire [2 : 0] s00_axi_awprot,
		input wire  s00_axi_awvalid,
		output wire  s00_axi_awready,
		input wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_wdata,
		input wire [(C_S00_AXI_DATA_WIDTH/8)-1 : 0] s00_axi_wstrb,
		input wire  s00_axi_wvalid,
		output wire  s00_axi_wready,
		output wire [1 : 0] s00_axi_bresp,
		output wire  s00_axi_bvalid,
		input wire  s00_axi_bready,
		input wire [C_S00_AXI_ADDR_WIDTH-1 : 0] s00_axi_araddr,
		input wire [2 : 0] s00_axi_arprot,
		input wire  s00_axi_arvalid,
		output wire  s00_axi_arready,
		output wire [C_S00_AXI_DATA_WIDTH-1 : 0] s00_axi_rdata,
		output wire [1 : 0] s00_axi_rresp,
		output wire  s00_axi_rvalid,
		input wire  s00_axi_rready
	);
	

// Instantiation of Axi Bus Interface S00_AXI
wire AD_CLK_TEMP;
	AK5394_LF398_v1_0_S00_AXI # ( 
		.DDR_BASE_ADDR(DDR_BASE_ADDR),
		.C_S_AXI_DATA_WIDTH(C_S00_AXI_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_S00_AXI_ADDR_WIDTH)
	) AK5394_LF398_v1_0_S00_AXI_inst (
		.S_AXI_ACLK(s00_axi_aclk),
		.S_AXI_ARESETN(s00_axi_aresetn),
		.S_AXI_AWADDR(s00_axi_awaddr),
		.S_AXI_AWPROT(s00_axi_awprot),
		.S_AXI_AWVALID(s00_axi_awvalid),
		.S_AXI_AWREADY(s00_axi_awready),
		.S_AXI_WDATA(s00_axi_wdata),
		.S_AXI_WSTRB(s00_axi_wstrb),
		.S_AXI_WVALID(s00_axi_wvalid),
		.S_AXI_WREADY(s00_axi_wready),
		.S_AXI_BRESP(s00_axi_bresp),
		.S_AXI_BVALID(s00_axi_bvalid),
		.S_AXI_BREADY(s00_axi_bready),
		.S_AXI_ARADDR(s00_axi_araddr),
		.S_AXI_ARPROT(s00_axi_arprot),
		.S_AXI_ARVALID(s00_axi_arvalid),
		.S_AXI_ARREADY(s00_axi_arready),
		.S_AXI_RDATA(s00_axi_rdata),
		.S_AXI_RRESP(s00_axi_rresp),
		.S_AXI_RVALID(s00_axi_rvalid),
		.S_AXI_RREADY(s00_axi_rready),
		
    	.FPGA_DongJ_DIR(FPGA_DongJ_DIR),//动镜移动方向
    	.FPGA_LASER_PULSE(FPGA_LASER_PULSE),  //激光检测器整形波输入
        .FPGA_LOGIC_LF398(FPGA_LOGIC_LF398),//LF398逻辑电平		
	    .FPGA_AD_SCLK(FPGA_AD_SCLK),  //CPLD_ADSCLK
	    .FPGA_AD_LRCK(FPGA_AD_LRCK),  //CPLD_ADFRAMESYN
	    .FPGA_AD_SDATA(FPGA_AD_SDATA), //CPLD_ADDATA
        .FPGA_AD_FSYNC(FPGA_AD_FSYNC),
	    //.FPGA_AD_MCLK(AD_CLK_TEMP),  //主机时钟
	    .FPGA_AD_ZCAL(FPGA_AD_ZCAL),  //
	    .FPGA_AD_RSTn(FPGA_AD_RSTn),  //	
	    
	    .ddr_write_done_arc(ddr_write_done_arc),
	    .wr_data_ready(wr_data_ready),  
    	.wr_data_buf(wr_data_buf),
    	.wr_addr_buf(wr_addr_buf),
    	.ddr_ad_data_inte(ddr_ad_data_inte),// posedege signal active
    	.ddr_ad_chk(ddr_ad_chk),
		.miss_data_led(miss_data_led)
	);

	// Add user logic here
	assign FPGA_AD_MCLK = PLL_CLK;
	/*
	reg AD_MCLK;
	always@(PLL_CLK)
	begin
		AD_MCLK <= PLL_CLK;
	end	
	*/
	// User logic ends

	endmodule
