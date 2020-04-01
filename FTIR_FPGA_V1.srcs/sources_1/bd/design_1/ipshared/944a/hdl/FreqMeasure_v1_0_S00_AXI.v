
`timescale 1 ns / 1 ps

	module FreqMeasure_v1_0_S00_AXI #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line

		// Width of S_AXI data bus
		parameter integer C_S_AXI_DATA_WIDTH	= 32,
		// Width of S_AXI address bus
		parameter integer C_S_AXI_ADDR_WIDTH	= 4
	)
	(
		// Users to add ports here
		input wire p_in,//输入的测量脉冲
		output wire move_dir,//检测到的动镜移动方向
		// User ports ends
		// Do not modify the ports beyond this line

		// Global Clock Signal
		input wire  S_AXI_ACLK,
		// Global Reset Signal. This Signal is Active LOW
		input wire  S_AXI_ARESETN,
		// Write address (issued by master, acceped by Slave)
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] S_AXI_AWADDR,
		// Write channel Protection type. This signal indicates the
    		// privilege and security level of the transaction, and whether
    		// the transaction is a data access or an instruction access.
		input wire [2 : 0] S_AXI_AWPROT,
		// Write address valid. This signal indicates that the master signaling
    		// valid write address and control information.
		input wire  S_AXI_AWVALID,
		// Write address ready. This signal indicates that the slave is ready
    		// to accept an address and associated control signals.
		output wire  S_AXI_AWREADY,
		// Write data (issued by master, acceped by Slave) 
		input wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_WDATA,
		// Write strobes. This signal indicates which byte lanes hold
    		// valid data. There is one write strobe bit for each eight
    		// bits of the write data bus.    
		input wire [(C_S_AXI_DATA_WIDTH/8)-1 : 0] S_AXI_WSTRB,
		// Write valid. This signal indicates that valid write
    		// data and strobes are available.
		input wire  S_AXI_WVALID,
		// Write ready. This signal indicates that the slave
    		// can accept the write data.
		output wire  S_AXI_WREADY,
		// Write response. This signal indicates the status
    		// of the write transaction.
		output wire [1 : 0] S_AXI_BRESP,
		// Write response valid. This signal indicates that the channel
    		// is signaling a valid write response.
		output wire  S_AXI_BVALID,
		// Response ready. This signal indicates that the master
    		// can accept a write response.
		input wire  S_AXI_BREADY,
		// Read address (issued by master, acceped by Slave)
		input wire [C_S_AXI_ADDR_WIDTH-1 : 0] S_AXI_ARADDR,
		// Protection type. This signal indicates the privilege
    		// and security level of the transaction, and whether the
    		// transaction is a data access or an instruction access.
		input wire [2 : 0] S_AXI_ARPROT,
		// Read address valid. This signal indicates that the channel
    		// is signaling valid read address and control information.
		input wire  S_AXI_ARVALID,
		// Read address ready. This signal indicates that the slave is
    		// ready to accept an address and associated control signals.
		output wire  S_AXI_ARREADY,
		// Read data (issued by slave)
		output wire [C_S_AXI_DATA_WIDTH-1 : 0] S_AXI_RDATA,
		// Read response. This signal indicates the status of the
    		// read transfer.
		output wire [1 : 0] S_AXI_RRESP,
		// Read valid. This signal indicates that the channel is
    		// signaling the required read data.
		output wire  S_AXI_RVALID,
		// Read ready. This signal indicates that the master can
    		// accept the read data and response information.
		input wire  S_AXI_RREADY
	);

	// AXI4LITE signals
	reg [C_S_AXI_ADDR_WIDTH-1 : 0] 	axi_awaddr;
	reg  	axi_awready;
	reg  	axi_wready;
	reg [1 : 0] 	axi_bresp;
	reg  	axi_bvalid;
	reg [C_S_AXI_ADDR_WIDTH-1 : 0] 	axi_araddr;
	reg  	axi_arready;
	reg [C_S_AXI_DATA_WIDTH-1 : 0] 	axi_rdata;
	reg [1 : 0] 	axi_rresp;
	reg  	axi_rvalid;

	// Example-specific design signals
	// local parameter for addressing 32 bit / 64 bit C_S_AXI_DATA_WIDTH
	// ADDR_LSB is used for addressing 32/64 bit registers/memories
	// ADDR_LSB = 2 for 32 bits (n downto 2)
	// ADDR_LSB = 3 for 64 bits (n downto 3)
	localparam integer ADDR_LSB = (C_S_AXI_DATA_WIDTH/32) + 1;
	localparam integer OPT_MEM_ADDR_BITS = 1;
	//----------------------------------------------
	//-- Signals for user logic register space example
	//------------------------------------------------
	//-- Number of Slave Registers 4
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg0;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg1;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg2;
	reg [C_S_AXI_DATA_WIDTH-1:0]	slv_reg3;
	wire	 slv_reg_rden;
	wire	 slv_reg_wren;
	reg [C_S_AXI_DATA_WIDTH-1:0]	 reg_data_out;
	integer	 byte_index;
	reg	 aw_en;

	// I/O Connections assignments

	assign S_AXI_AWREADY	= axi_awready;
	assign S_AXI_WREADY	= axi_wready;
	assign S_AXI_BRESP	= axi_bresp;
	assign S_AXI_BVALID	= axi_bvalid;
	assign S_AXI_ARREADY	= axi_arready;
	assign S_AXI_RDATA	= axi_rdata;
	assign S_AXI_RRESP	= axi_rresp;
	assign S_AXI_RVALID	= axi_rvalid;
	// Implement axi_awready generation
	// axi_awready is asserted for one S_AXI_ACLK clock cycle when both
	// S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_awready is
	// de-asserted when reset is low.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_awready <= 1'b0;
	      aw_en <= 1'b1;
	    end 
	  else
	    begin    
	      if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	        begin
	          // slave is ready to accept write address when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_awready <= 1'b1;
	          aw_en <= 1'b0;
	        end
	        else if (S_AXI_BREADY && axi_bvalid)
	            begin
	              aw_en <= 1'b1;
	              axi_awready <= 1'b0;
	            end
	      else           
	        begin
	          axi_awready <= 1'b0;
	        end
	    end 
	end       

	// Implement axi_awaddr latching
	// This process is used to latch the address when both 
	// S_AXI_AWVALID and S_AXI_WVALID are valid. 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_awaddr <= 0;
	    end 
	  else
	    begin    
	      if (~axi_awready && S_AXI_AWVALID && S_AXI_WVALID && aw_en)
	        begin
	          // Write Address latching 
	          axi_awaddr <= S_AXI_AWADDR;
	        end
	    end 
	end       

	// Implement axi_wready generation
	// axi_wready is asserted for one S_AXI_ACLK clock cycle when both
	// S_AXI_AWVALID and S_AXI_WVALID are asserted. axi_wready is 
	// de-asserted when reset is low. 

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_wready <= 1'b0;
	    end 
	  else
	    begin    
	      if (~axi_wready && S_AXI_WVALID && S_AXI_AWVALID && aw_en )
	        begin
	          // slave is ready to accept write data when 
	          // there is a valid write address and write data
	          // on the write address and data bus. This design 
	          // expects no outstanding transactions. 
	          axi_wready <= 1'b1;
	        end
	      else
	        begin
	          axi_wready <= 1'b0;
	        end
	    end 
	end       

	// Implement memory mapped register select and write logic generation
	// The write data is accepted and written to memory mapped registers when
	// axi_awready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted. Write strobes are used to
	// select byte enables of slave registers while writing.
	// These registers are cleared when reset (active low) is applied.
	// Slave register write enable is asserted when valid address and data are available
	// and the slave is ready to accept the write address and write data.
	assign slv_reg_wren = axi_wready && S_AXI_WVALID && axi_awready && S_AXI_AWVALID;

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      slv_reg0 <= 0;
	      slv_reg1 <= 0;
	      slv_reg2 <= 0;
	      slv_reg3 <= 0;
	    end 
	  else begin
	    if (slv_reg_wren)
	      begin
	        case ( axi_awaddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
	          2'h0:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 0
	                slv_reg0[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          2'h1:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 1
	                slv_reg1[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          2'h2:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 2
	                slv_reg2[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          2'h3:
	            for ( byte_index = 0; byte_index <= (C_S_AXI_DATA_WIDTH/8)-1; byte_index = byte_index+1 )
	              if ( S_AXI_WSTRB[byte_index] == 1 ) begin
	                // Respective byte enables are asserted as per write strobes 
	                // Slave register 3
	                slv_reg3[(byte_index*8) +: 8] <= S_AXI_WDATA[(byte_index*8) +: 8];
	              end  
	          default : begin
	                      slv_reg0 <= slv_reg0;
	                      slv_reg1 <= slv_reg1;
	                      slv_reg2 <= slv_reg2;
	                      slv_reg3 <= slv_reg3;
	                    end
	        endcase
	      end
	  end
	end    

	// Implement write response logic generation
	// The write response and response valid signals are asserted by the slave 
	// when axi_wready, S_AXI_WVALID, axi_wready and S_AXI_WVALID are asserted.  
	// This marks the acceptance of address and indicates the status of 
	// write transaction.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_bvalid  <= 0;
	      axi_bresp   <= 2'b0;
	    end 
	  else
	    begin    
	      if (axi_awready && S_AXI_AWVALID && ~axi_bvalid && axi_wready && S_AXI_WVALID)
	        begin
	          // indicates a valid write response is available
	          axi_bvalid <= 1'b1;
	          axi_bresp  <= 2'b0; // 'OKAY' response 
	        end                   // work error responses in future
	      else
	        begin
	          if (S_AXI_BREADY && axi_bvalid) 
	            //check if bready is asserted while bvalid is high) 
	            //(there is a possibility that bready is always asserted high)   
	            begin
	              axi_bvalid <= 1'b0; 
	            end  
	        end
	    end
	end   

	// Implement axi_arready generation
	// axi_arready is asserted for one S_AXI_ACLK clock cycle when
	// S_AXI_ARVALID is asserted. axi_awready is 
	// de-asserted when reset (active low) is asserted. 
	// The read address is also latched when S_AXI_ARVALID is 
	// asserted. axi_araddr is reset to zero on reset assertion.

	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_arready <= 1'b0;
	      axi_araddr  <= 32'b0;
	    end 
	  else
	    begin    
	      if (~axi_arready && S_AXI_ARVALID)
	        begin
	          // indicates that the slave has acceped the valid read address
	          axi_arready <= 1'b1;
	          // Read address latching
	          axi_araddr  <= S_AXI_ARADDR;
	        end
	      else
	        begin
	          axi_arready <= 1'b0;
	        end
	    end 
	end       

	// Implement axi_arvalid generation
	// axi_rvalid is asserted for one S_AXI_ACLK clock cycle when both 
	// S_AXI_ARVALID and axi_arready are asserted. The slave registers 
	// data are available on the axi_rdata bus at this instance. The 
	// assertion of axi_rvalid marks the validity of read data on the 
	// bus and axi_rresp indicates the status of read transaction.axi_rvalid 
	// is deasserted on reset (active low). axi_rresp and axi_rdata are 
	// cleared to zero on reset (active low).  
	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_rvalid <= 0;
	      axi_rresp  <= 0;
	    end 
	  else
	    begin    
	      if (axi_arready && S_AXI_ARVALID && ~axi_rvalid)
	        begin
	          // Valid read data is available at the read data bus
	          axi_rvalid <= 1'b1;
	          axi_rresp  <= 2'b0; // 'OKAY' response
	        end   
	      else if (axi_rvalid && S_AXI_RREADY)
	        begin
	          // Read data is accepted by the master
	          axi_rvalid <= 1'b0;
	        end                
	    end
	end    

	// Implement memory mapped register select and read logic generation
	// Slave register read enable is asserted when valid address is available
	// and the slave is ready to accept the read address.
	reg [31:0]mcycle;//信号周期值
	reg [31:0]mstate;//状态信号
	assign slv_reg_rden = axi_arready & S_AXI_ARVALID & ~axi_rvalid;
	always @(*)
	begin
	      // Address decoding for reading registers
	      case ( axi_araddr[ADDR_LSB+OPT_MEM_ADDR_BITS:ADDR_LSB] )
	        2'h0   : reg_data_out <= slv_reg0;//
	        2'h1   : reg_data_out <= slv_reg1 ;//
	        2'h2   : reg_data_out <= mcycle;  //周期值反馈
	        2'h3   : reg_data_out <= mstate ;//反馈一些状态
	        default : reg_data_out <= 0;
	      endcase
	end

	// Output register or memory read data
	always @( posedge S_AXI_ACLK )
	begin
	  if ( S_AXI_ARESETN == 1'b0 )
	    begin
	      axi_rdata  <= 0;
	    end 
	  else
	    begin    
	      // When there is a valid read address (S_AXI_ARVALID) with 
	      // acceptance of read address by the slave (axi_arready), 
	      // output the read dada 
	      if (slv_reg_rden)
	        begin
	          axi_rdata <= reg_data_out;     // register read data
	        end   
	    end
	end    

	// Add user logic here
	/*---------------------------------------------------------------------------------------------------
	PS_PL:
	
	slv_reg0:作为PS_PL设置的控制寄存器
		slv_reg0[0]:设置检测待测信号的上升沿还是下降沿 0(默认):上升沿检测，1：下降沿检测
		slv_reg0[1]:设置使用硬件还是软件产生动镜移动方向信号  0(默认):软件设置  1:硬件设置
		slv_reg0[2]:ps(软件)设置动镜方向信号   0 , 1
		slv_reg0[3]:设置PL内部动镜方向初始值  1，0
		slv_reg0[4]:ps启动动镜控制
		slv_reg0[31]:模块自测控制位  1：开启模块自测   0：关闭模块自测

		

	slv_reg1:作为周期测量基础时钟的分频因子对AXI时钟分频得到
	slv_reg2:作为设置判断减速阈值的寄存器
	slv_reg3:
		slv_reg3[4:0]:ps设置硬件判断动镜方向所使用的加减速buf深度
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	PL_PS
	slv_reg0:返回控制寄存器的值
	slv_reg1:返回分频器的值(反应了检测精度)
	slv_reg2:返回当前计算的脉冲周期值
	slv_reg3:返回PL的部分状态
	----------------------------------------------------------------------------------------------------*/


	wire p_in_w;
	assign p_in_w = ( slv_reg0[0]) ?  p_in : ~p_in;
	reg [4:0]p_in_f;
	reg bclk_r;//测量基础脉冲
	reg [31:0]bcnt;//基础时钟计时器
	reg move_dir_r;//FPGA获得动镜移动方向

	wire p_in_p;//上升沿
	wire bclk;//实际基础脉冲
	assign p_in_p = p_in_f[1] & (p_in_f[2]) & (~p_in_f[3]) & (~p_in_f[4]);
	//获得基础测量时钟
	//使用slv_reg1,作为时钟分频值
	assign bclk = (slv_reg1 == 32'd1)? S_AXI_ACLK : bclk_r;
	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)begin
			bcnt <= 32'd0;
			bclk_r <= 1'b0;
		end
		else begin
			if(slv_reg1 == 32'd1)begin
				bclk_r <= 1'b0;
			end
			else begin
				//cnt
				bcnt <= (bcnt >= slv_reg1 - 1'b1)? 32'd0 : bcnt + 1'b1 ;
				//时钟生成
				if(bcnt <= (slv_reg1>>1) - 1'b1)
					bclk_r <= 1'b1;
				else bclk_r <= 1'b0;
			end

		end
	end
	//产生测试信号
	reg [31:0]tcnt;
	reg tsig;
	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)begin
			tcnt <= 32'd0;
			tsig = 1'b0;
		end
		else begin
			tcnt <= (tcnt>=32'd9999)?32'd0:tcnt + 1'b1;
			tsig <= (tcnt>=32'd9999)?~tsig : tsig;
		end
	end
	///使用测量基础 消除亚稳态
	always@(posedge bclk)
		if(!S_AXI_ARESETN)
			p_in_f <= 'd0;
		 else
			p_in_f <= slv_reg0[31]? {p_in_f[4:0],tsig} : {p_in_f[4:0],p_in_w};
	//测量周期
	reg [31:0]mcnt;
	always@(posedge bclk)
	begin
		if(!S_AXI_ARESETN)begin
			mcnt <= 32'd0;
		end	
		else begin
			mcnt <= p_in_p ? 32'd0 : mcnt + 1'b1;
		end
	end

	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)	begin
			mcycle <= 32'd0;
		end
		else begin
			if(p_in_p)
				mcycle <= mcnt;
			else 
				mcycle <= mcycle;
		end
	end
	//使用reg3通道作为信息反馈寄存器
	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)
			mstate <= 32'd0;
		else begin
			mstate[0] <= p_in_f[2] ;//输入的脉冲电平
			mstate[1] <= move_dir_r ;//PL检测的动镜移动方向
			mstate[2] <= bclk_r;
		end	
	end
	//FPGA检测动镜加减速
	reg [31:0]speed_f;//加减速缓存器，1加速(含有匀速)  0减速
	reg [31:0]th;
	//锁存判断减速的阈值
	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)begin
			th <= 32'd0;
		end
		begin
			th <= slv_reg2;
		end
	end
	//缓存动镜移动加减速情况
	reg [31:0]time_last;
	always@(negedge p_in_p)//利用下降沿，当mcycle更新数据后进行一次计算
	begin
		if(!S_AXI_ARESETN )begin
			speed_f <= ~32'd0;//初始值设置为全在加速状态
			time_last <= 32'd0;
		end
		else begin
			time_last <= mcycle;//用于记下上一次的周期值
			speed_f[31:1] <= speed_f[30:0];//移位加减速buf
			speed_f[0] <= speedUp(mcycle,time_last,speed_f[1],th);//记下当前是加速还是减速
		end
	end
	//提取方向改变边沿信号
	wire dir_change;
	assign dir_change = isChange(speed_f, slv_reg3[4:0]);
	reg dir_c_f1;
	reg dir_c_f2;
	wire dir_change_p;//方向改变上升沿
	assign dir_change_p = dir_c_f1 & (~dir_c_f2);
	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)begin
			dir_c_f1 <= 1'b0;
			dir_c_f2 <= 1'b0;
		end
		else begin
			dir_c_f1 <= dir_change;
			dir_c_f2 <= dir_c_f1;		
		end
	end
	//生成动镜移动方向信号
	localparam IDEL = 2'b01;
	localparam WORK = 2'b10;
	reg [1:0] c_state;
	always@(posedge S_AXI_ACLK)
	begin
		if(!S_AXI_ARESETN)begin
			move_dir_r <= 1'b0;
			c_state <= IDEL;
		end
		else begin
			case(c_state)
				IDEL:begin					
						if(slv_reg0[4]==1'b1)begin
							c_state <= WORK;
						end
						else begin
							move_dir_r <= slv_reg0[3];
						end
					end
				WORK:begin
						move_dir_r <= dir_change_p ? ~move_dir_r : move_dir_r ;
						if(slv_reg0[4]==1'b0)begin
							c_state <= IDEL;	
						end
						else begin
							c_state <= WORK;	
						end
						
					end
				default:c_state <= WORK;
			endcase
		end
	end
	/*******************************************************************************************
	*加减速判断函数
	*输入参数1：t1 ,表示当前速度的时间     
	*输入参数2：t2 ,表示当上一次速度的时间  
	*输入参数3：last_speed,表示上一次速度是加速还是减速
	*输入参数4：th,表示判断减速的阈值，只有当相差超过了阈值，才认为是减速过程
	*返回值：1加速   0减速
	********************************************************************************************/
	function speedUp(input [31:0]t1,input [31:0]t2,input last_speed,input [31:0]th);
		reg [31:0]t1_t2;
		reg [31:0]t2_t1;
		reg t1Biger;
		reg t1_t2_ans;
		//reg t2_t1_ans;
		begin
			t1_t2 = t1 - t2;//减速
			t2_t1 = t2 - t1;
			t1Biger = (t1 > t2)? 1'b1 : 1'b0;
			t1_t2_ans = (t1_t2 > th)? 1'b0 : last_speed;
			//t2_t1_ans = 1'b1 ;		
			speedUp = t1Biger? t1_t2_ans : 1'b1;
		end
	endfunction
	/*******************************************************************************************
	*判断是否是动镜移动方向换向，若是产生一个高电平
	*输入参数1：bf ,表示缓存的动镜加减速状态    
	*输入参数2：jd ,表示使用多深的缓存进行比较确定  该值的取值范围为 1-16  
	*返回值：1是换向点   0非换向点
	********************************************************************************************/
	function isChange(input [31:0]bf , input [4:0] jd);
		reg [31:0] temp;
		reg [31:0]data1;
		reg [31:0]data2;
		begin
			temp = bf>>jd;
			data1 = temp & ((~32'd0) >> (32-jd));
			data2 = bf & ((~32'd0) >> (32-jd));
			isChange = ((data1 == 32'd0) && (data2 == ((~32'd0) >> (32-jd)))) ;
		end
	endfunction
	assign move_dir = (slv_reg0[1] == 1'b0) ? slv_reg0[2] : move_dir_r;//输出动镜移动方向
	// User logic ends
	endmodule
