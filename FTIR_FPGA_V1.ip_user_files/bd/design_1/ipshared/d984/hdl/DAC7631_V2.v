`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/11/11 17:13:23
// Design Name: 
// Module Name: DAC7631_V2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


/* dac7631.v */
/*                 DAC7631驱动模块
---------------------------------------------------------------------
    版本  |         说明                             |修改人
---------------------------------------------------------------------
   v 2.0  |   设为33MHz 								| hpy
          |
---------------------------------------------------------------------*/
module	DAC7631_V2(
	input		clk			,//基础时钟，决定了SPI时钟的频率
	input		rst_n		,
	input [31:0]set_data	,//{dataA，dataB} 当检测到数据和上一次的数据不同时更新数据
	input		update		,//手动更新DAC的值，优先级在自动检测数据更新之上
	output reg	DAC_SCK		,//驱动DAC7631的时钟
	output reg	DAC_SDO_A	,//驱动DAC7631数据线A
	output reg	DAC_SDO_B	,//驱动DAC7631数据线B
	output reg	DAC_CS		,//AB共用的片选信号
	output reg	DAC_LDAC_A	,//DAC7631 A的数据更新信号
	output reg	DAC_LDAC_B	,//DAC7631 B的数据更新信号
	output reg	DAC_RST		,//AB共用的复位线
	output reg	DAC_LOAD	,	//AB共用的输入数据装载线	
	output reg	busy		,
	output [3:0]   ostate	   ,
	output oup
);
reg [31:0]cdata;
reg [31:0]ldata;

/*
localparam IDEL				= 16'b0000000000000001;//待机状态
localparam LOAD_DATA        = 16'b0000000000000010;//
localparam SPI_SCK0			= 16'b0000000000000100;//
localparam SPI_SCK1			= 16'b0000000000001000;//
localparam CS_SET_HIGH		= 16'b0000000000010000;//
localparam LOAD_SET_LOW		= 16'b0000000000100000;//
localparam LOAD_SET_HIGH	= 16'b0000000001000000;//
localparam LDAC_SET_LOW		= 16'b0000000010000000;//
localparam LDAC_SET_HIGH	= 16'b0000000100000000;
localparam RST_SET_LOW		= 16'b0000001000000000;
localparam RST_SET_HIGH		= 16'b0000010000000000;*/

 //16'b0000010000000000;
 
localparam RST_SET_LOW		= 16'b0000000000000000;//
localparam RST_SET_HIGH		= 16'b0000000000000001;//
localparam IDEL				= 16'b0000000000000010;//
localparam LOAD_DATA        = 16'b0000000000000100;//
localparam SPI_SCK0			= 16'b0000000000001000;//
localparam SPI_SCK1			= 16'b0000000000010000;//
localparam CS_SET_HIGH		= 16'b0000000000100000;//
localparam LOAD_SET_LOW		= 16'b0000000001000000;//
localparam LOAD_SET_HIGH	= 16'b0000000010000000;//
localparam LDAC_SET_LOW		= 16'b0000000100000000;
localparam LDAC_SET_HIGH	= 16'b0000001000000000;
localparam TX_WAIT_ONE_CLK	= 16'b0000010000000000;//发送完16bit等待一个clk

reg [15:0]cstate ;
reg [15:0]nstate ;		  
assign ostate = cstate;
reg [5:0]cnt;//发送数据计数器 
reg autoupdate;//自动检测更新
//第一段:状态机更新状态
always@(posedge clk or negedge rst_n)
begin
	if(!rst_n)begin
		cstate = RST_SET_LOW;
	end
	else begin
		cstate <= nstate;
	end
end
//第二段:状态转移
reg [15:0] dataA , dataB;
always@(cstate)
begin
	nstate = 16'bx; 
	case(cstate)
		RST_SET_LOW:begin
			if(cnt>= 5'd30)nstate	= RST_SET_HIGH	;
			else nstate = RST_SET_LOW;
		end
		RST_SET_HIGH:begin
			nstate = IDEL	;
		end
		IDEL:begin//待机状态
			if(autoupdate || update)begin//手动更新数据，或者数据和上一次数据不一致，更新数据
				nstate		= LOAD_DATA			;	 
			end	 
			else nstate = IDEL;
		end
		LOAD_DATA:begin
		    nstate = SPI_SCK0;
		end
		SPI_SCK0:begin//spi时钟设为0
			nstate = SPI_SCK1;
		end
		SPI_SCK1:begin//spi时钟设为1
			if(cnt == 5'd16)begin//数据发送结束
				nstate	= TX_WAIT_ONE_CLK	;
			end
			else begin
				nstate	= SPI_SCK0		;
			end
		end
		TX_WAIT_ONE_CLK:begin
			nstate	= CS_SET_HIGH	;
		end
		
		CS_SET_HIGH:begin//延时拉高片选
			nstate	= LOAD_SET_LOW	;
		end
		LOAD_SET_LOW:begin 
			nstate = LOAD_SET_HIGH	;
		end
		LOAD_SET_HIGH:begin
			nstate = LDAC_SET_LOW	;
		end
		LDAC_SET_LOW:begin	//最大频率40MHz		
			if(cnt >= 5'd4)nstate = LDAC_SET_HIGH	;
			else nstate = LDAC_SET_LOW;
		end
		LDAC_SET_HIGH:begin
			nstate = IDEL;
		end
		default:nstate = IDEL;
	endcase
end
//第三段：状态处理，产生输出
always@(posedge clk or negedge rst_n)
begin
	if(!rst_n)begin
		DAC_SCK		<= 1'b1		;
		DAC_SDO_A	<= 1'b1		;
		DAC_SDO_B	<= 1'b1		;
		DAC_CS		<= 1'b1		;
		DAC_LDAC_A	<= 1'b1		;
		DAC_LDAC_B	<= 1'b1		;
		DAC_RST		<= 1'b0		;
		cnt			<= 5'd0		; 
		busy		<= 1'b0		;
	end
	else begin
		case(nstate)
			RST_SET_LOW:begin
				DAC_RST		<= 1'b0		;
				cnt			<= cnt+1'b1	;
				busy        <= 1'b1;
			end
			RST_SET_HIGH:begin
				DAC_RST		<= 1'b1		;
				cnt			<= 5'd0		;
				busy        <= 1'b1     ;
			end
			IDEL:begin
				DAC_SCK		<= 1'b1		;
				DAC_SDO_A	<= 1'b0     ;
				DAC_SDO_B	<= 1'b0     ;
				DAC_CS		<= 1'b1		;
				DAC_LDAC_A	<= 1'b1		;
				DAC_LDAC_B	<= 1'b1     ;
				DAC_RST     <= 1'b1     ;
				DAC_LOAD	<= 1'b1		;
				cnt			<= 5'd15	;  
				//dataA		<= set_data[31 : 16]	;
				//dataB		<= set_data[15 :  0]	; 
				busy		<= 1'b0		;
			end
			LOAD_DATA:begin
			    DAC_SCK		<= 1'b1		;
				DAC_SDO_A	<= 1'b0     ;
				DAC_SDO_B	<= 1'b0     ;
				DAC_CS		<= 1'b1		;
				DAC_LDAC_A	<= 1'b1		;
				DAC_LDAC_B	<= 1'b1     ;
				DAC_RST     <= 1'b1     ;
				DAC_LOAD	<= 1'b1		;
				cnt			<= 5'd0		;  
				dataA		<= set_data[31 : 16]	;
				dataB		<= set_data[15 :  0]	; 
				busy		<= 1'b1		;
			end
			SPI_SCK0:begin
				DAC_SCK		<= 1'b0		;
				DAC_SDO_A	<= dataA[15];
				DAC_SDO_B	<= dataB[15];
				DAC_CS		<= 1'b0		;
				DAC_LDAC_A	<= 1'b1		;
				DAC_LDAC_B	<= 1'b1		;
                DAC_RST     <= 1'b1     ;
				DAC_LOAD	<= 1'b1		;
				dataA		<= dataA<<1	;
				dataB		<= dataB<<1	;  
				busy		<= 1'b1		;
			end
			SPI_SCK1:begin
				DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= DAC_SDO_A    ;
                DAC_SDO_B	<= DAC_SDO_B    ;
                DAC_CS		<= 1'b0         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
                DAC_LOAD	<= 1'b1         ;
                busy        <= 1'b1         ;
				cnt			<= cnt+1'b1		;
			end
			TX_WAIT_ONE_CLK:begin
				DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b0         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
                DAC_LOAD	<= 1'b1         ;
                busy        <= 1'b1         ;
			end
			CS_SET_HIGH:begin
				DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b1         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
                DAC_LOAD	<= 1'b1         ;
                busy        <= 1'b1         ;
			end
			LOAD_SET_LOW:begin
			    DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b1         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
				DAC_LOAD	<= 1'b0         ;
				busy        <= 1'b1         ;
			end
			LOAD_SET_HIGH:begin
			    DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b1         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
				DAC_LOAD	<= 1'b1	    	;
				busy        <= 1'b1         ;
				cnt			<= 5'd0		    ;
			end
			LDAC_SET_LOW:begin
				DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b1         ;
                DAC_LDAC_A	<= 1'b0         ;
                DAC_LDAC_B	<= 1'b0         ;
                DAC_RST     <= 1'b1         ;
				DAC_LOAD	<= 1'b1	    	;
				busy        <= 1'b1         ;
				cnt<=cnt+1'b1		        ;		
			end
			LDAC_SET_HIGH:begin
				DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b1         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
				DAC_LOAD	<= 1'b1	    	;	
				cnt			<= 5'd0         ;
				busy        <= 1'b1         ;
			end
			default:begin
				DAC_SCK		<= 1'b1			;
				DAC_SDO_A	<= 1'b0         ;
                DAC_SDO_B	<= 1'b0         ;
                DAC_CS		<= 1'b1         ;
                DAC_LDAC_A	<= 1'b1         ;
                DAC_LDAC_B	<= 1'b1         ;
                DAC_RST     <= 1'b1         ;
				DAC_LOAD	<= 1'b1	    	;	
				cnt			<= 5'd0         ;
				busy        <= 1'b1         ;
			end
		endcase
	end
end
//在待机状态下存下上一次的DA数据和当前DA数据，用以判断是否更新数据	
reg [31:0] din;
always@(posedge clk or negedge rst_n)
begin
	if(!rst_n)begin
		cdata <= 32'd0		;	   
		ldata <= 32'd0		;			 
		autoupdate <= 1'b0	;
		din		<=	32'd0	;
	end
	else begin
		din 	<= set_data;
		cdata	<= din					;
        if(nstate == LOAD_DATA)ldata	<= cdata					; 
		if((cdata != ldata)) begin
			autoupdate <= 1'b1	;
		end	
		else  begin
			autoupdate <= 1'b0	;
		end			
	end
end	 

assign oup =   autoupdate;
	
endmodule
