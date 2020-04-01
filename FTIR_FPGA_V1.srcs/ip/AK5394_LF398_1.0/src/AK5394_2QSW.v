/*****************************************************************
*说明：AK5394+LF398的控制驱动,兼容qsw的红外AD电路板
*Author:hpy
*Date:2019/6/6
*****************************************************************/


/*------------------------------修改记录----------------------------
         版本          |  说明    |      修改人
version：AD_LF398_V1.0 | 第一版   | hpy
version：AD_LF398_V1.1 | 第二版   | hpy  ：加入硬件采集数据
version：AD_LF398_V2.0 | 第三版   | hpy  ：加入时间记录数据
-----------------------------------------------------------------*/
`define _DEBUG_ 
module AK5394_2QSW #
(
	parameter integer DDR_BASE_ADDR	= 32'H04000000
)
( 
		input rst_n, 
		input clk,//系统时钟 50MHz / 150MHz
		
		input [31:0]MCU2SLAVE,  //上位机发送的指令寄存器  [31:28]指令位  [27:0]数据位
		input FPGA_DongJ_DIR,//动镜移动方向
		input FPGA_LASER_PULSE,  //激光检测器整形波输入
		output reg FPGA_LOGIC_LF398,//LF398逻辑电平
	//========ak5394==========
		
		input FPGA_AD_SCLK,  //CPLD_ADSCLK
		input FPGA_AD_LRCK,  //CPLD_ADFRAMESYN
		input FPGA_AD_SDATA, //CPLD_ADDATA
		input FPGA_AD_FSYNC,
		
		output FPGA_AD_MCLK,  //主机时钟(不用，主机时钟使用PLL获得，此处该信号用于时间计数的时钟)
		output FPGA_AD_ZCAL,  //
		output FPGA_AD_RSTn,  //
		
		output reg ready,  //如果LF398数据锁存完成，则输出一个10us的高电平，可用做中断信号或者查询信号，查询信号被写入了STATE寄存器
		
		output [31:0]AD_DATA1,
		output [31:0]AD_DATA2,
		output reg[31:0] STATUS, //状态寄存器
		//write ddr ad singel
		input wire ddr_write_done_arc,
        output wire wr_data_ready,  
    	output wire [95:0]wr_data_buf,
    	output wire [95:0]wr_addr_buf,

    	output wire ddr_ad_data_inte,// posedege signal active
    	output wire  ddr_ad_chk, //choose read DDR address	
		output reg  miss_data_led//当硬件采集丢失数据的时输出0
	//======测试使用==========	
);

/**********************************FUNCTION************************************
*计算写DDR的上限,到达上限返回1，否则返回0,这是保证程序内存不被覆盖，但是前面的
*数据依旧有可能会被覆盖掉，但是不会影响程序运行
*******************************************************************************/
function IsBufMax(input [31:0]base_addr,input [95:0]now_addr,input [31:0] length);
	reg flag1 ,flag2,flag3;
	begin
		flag1 = (now_addr[31:0] <= (base_addr + (length<<2) + (length<<2) + (length<<2) + (length<<2) +(length<<2)+(length<<2) - 32'd5))?1'b0:1'b1;
		flag2 = (now_addr[63:32] <= (base_addr + (length<<2) + (length<<2) + (length<<2) + (length<<2) +(length<<2)+(length<<2) - 32'd5))?1'b0:1'b1;
		flag3 = (now_addr[95:64] <= (base_addr + (length<<2) + (length<<2) + (length<<2) + (length<<2) +(length<<2)+(length<<2) - 32'd5))?1'b0:1'b1;
		IsBufMax =flag1 | flag2 |flag3 ;
	end
endfunction
/***************************************REG************************************
*计算写DDR的上限,到达上限返回1，否则返回0
*******************************************************************************/
reg LRCK;
wire SCLK;
reg SDATA;
reg [31:0]tdata1;
reg [31:0]tdata2;
reg [31:0]tdata1_o;
reg [31:0]tdata2_o;
reg [31:0]data1_lf398;
reg [31:0]data2_lf398;
//硬件采集丢失数据计数器
reg [31:0]miss_data_cnt;
reg [3:0]mcu2fpga_open_reuse;

//assign LRCK = FPGA_AD_LRCK;//CPLD_ADFRAMESYNC;
assign SCLK = FPGA_AD_SCLK;//CPLD_ADSCLK;
//ssign SDATA = FPGA_AD_SDATA;//CPLD_ADDATA;
always@(negedge SCLK)
begin
    LRCK <= FPGA_AD_LRCK;
    SDATA <= FPGA_AD_SDATA;
end

reg [5:0] cnt;
reg [23:0]data1,data2;
reg isOpen_AD_H8;
reg [23:0]AD_DATA1_reg , AD_DATA2_reg;
reg [7:0]AD_DATA1_H8_reg,AD_DATA2_H8_reg;//AD的数据只有24位，高8位没有使用这里根据指令来判断是否开启高8位的有效性
reg AD_RSTn,AD_ZCAL;
//reg Suspend_AD;//是否在LF398锁存后锁存AD读数
wire time_base_clk;//时间计时时钟
//补码转原码  24位为符号位 0+  1-
wire [23:0]data1_code;
wire [23:0]data2_code;

wire [31:0]data1_int; //int格式存储
wire [31:0]data2_int;
/****************start 24  to 32 int*************************/
wire [31:0]ddr_data1_int;
wire [31:0]ddr_data2_int;
assign ddr_data1_int[31] = data1[23];
assign ddr_data2_int[31] = data2[23];
assign ddr_data1_int[30:0] = data1[23]?{8'hff,data1[22:0]}:{8'd0,data1[22:0]} ;//数据位置
assign ddr_data2_int[30:0] = data2[23]?{8'hff,data1[22:0]}:{8'd0,data2[22:0]} ;//数据位置
/****************end 24  to 32 int*************************/

assign data1_code[23] = AD_DATA1_reg[23];
assign data2_code[23] = AD_DATA2_reg[23];

assign data1_code[22:0] = AD_DATA1_reg[23]?( AD_DATA1_reg[22:0] ^ 23'h7fffff) + 1'b1:AD_DATA1_reg[22:0];
assign data2_code[22:0] = AD_DATA2_reg[23]?( AD_DATA2_reg[22:0] ^ 23'h7fffff) + 1'b1:AD_DATA2_reg[22:0];

assign data1_int[31] = AD_DATA1_reg[23];//符号位  将24位补码扩展为32位的int补码
assign data2_int[31] = AD_DATA2_reg[23];

assign data1_int[30:0] = AD_DATA1_reg[23]?{8'hff,AD_DATA1_reg[22:0]}:{8'd0,AD_DATA1_reg[22:0]} ;//数据位置
assign data2_int[30:0] = AD_DATA2_reg[23]?{8'hff,AD_DATA2_reg[22:0]}:{8'd0,AD_DATA2_reg[22:0]} ;//数据位置

//assign AD_DATA1 = (Suspend_AD ?data1_lf398 : data1_int);
//assign AD_DATA2 = (mcu2fpga_open_reuse != 4'd0)? miss_data_cnt : (Suspend_AD ?data2_lf398 : data2_int);
//assign AD_DATA2 = (mcu2fpga_open_reuse == 4'd0)?  (Suspend_AD ?data2_lf398 : data2_int): ((mcu2fpga_open_reuse == 4'd1)?miss_data_cnt : ad_valid_data) ;

assign FPGA_AD_RSTn = AD_RSTn;
assign FPGA_AD_MCLK = (CLK_DIV ==28'd1)?clk: AD_MCLK_reg;
assign FPGA_AD_ZCAL= AD_ZCAL;


//----------------------上位机2下位机------------------------
//将上位机的2下位机的数据分为两个
reg [3  : 0]M2S_ORDER;  //指令
reg [27 : 0]M2S_DATA;  //数据

always@(posedge clk) //更新M2S数据
begin
	if(!rst_n)begin
		M2S_ORDER <= 4'd0;
		M2S_DATA <= 28'd0;
	end
	else begin
		M2S_ORDER[3:0] <= MCU2SLAVE[31:28];
		M2S_DATA[27:0] <= MCU2SLAVE[27:0];
	end
end
//-------------------------M2S指令译码---------------------
//when clk = 50MHz
//[31:28] = 0:默认参数
//[31:28] = 1:设置LF398触发信号的高电平时间为 [27:0]/50 us，如设置为10us，则[277:0]=10*50=500
//[31:28] = 2:开启AD高8位数据的有用性，31位为LF398触发脉冲，30位为激光脉冲信号，29：动镜方向信号
//[31:28] = 3:设置ak5394复位引脚电平，[0]的值为电平值
//[31:28] = 4:设置ak5394的ZCAL引脚电平，[0]的值为电平值
//[31:28] = 5:设置ak5394的MCLK的时钟，[27:0]是对50MHz时钟的分频数
//[31:28] = 6:设置LF398电压锁存后等待AD转换的时间 默认12us，所以默认waiteTime = 12 * 50 =600
//[31:28] = 7:模块参数设置寄存器设置
/*-----------------------------------------------------------------------------------------------
|							7:mcu2fpga_flag_reg ，数据位为28位 ,标志功能列表
-------------------------------------------------------------------------------------------------
|  [27]      |  [26]  |   25        |   24    |   23    |  22    |     21     |    
-------------------------------------------------------------------------------------------------
|时间寄存    |硬件滤波|复用reg1为   |开启LF398|设置LF398|硬件中断|丢失数据    |
|器位宽选    |默认值  |激光脉冲时间 |逻辑电压 |输出电平 |信号，同|监测通道    |
|择          |0       |数据量       |手动设置 |1/0      |步AD数据|DDR         |
-------------------------------------------------------------------------------------------------
|0:32位      |1:开启  |1:复用       |1:开启   |1:H      |1:同步  |1:AD数据    |
|1:16位      |0:关闭  |0:关闭       |0:关闭   |0:L      |0:异步  |0:PULSER数据|
-------------------------------------------------------------------------------------------------*/

//[31:28] = 8:设置ak5394的ready信号高电平持续时间，[27:0]/50 us   如设置为10us，则[27:0]=10*50=500
//[31:28] = 9:设置AD采样点数，满后存入DDR中，默认8192个点
//[31:28] = 10:设置计时器基时钟频率  ,对AXI时钟分频得到
              //默认为两片
//[31:28] = 11:设置AD硬件采集数据的方式 ，0 ：连续采样  1：定时采集   2：触发采集
//[31:28] = 12:开启通道2的ad数据寄存器服用为硬件采集数据丢失数据量 ，mcu2fpga_open_reuse ->1:返回丢失数量  0:关闭复用  other:返回硬件采集的DDR的有效数据量//[31:28] = 13:上位机通知模块是否采集数据（硬件采集），1：采集   2：停止采集
reg [27:0]pulse_delay_us,CLK_DIV,waiteTime;//时钟分频数
reg [27:0]TestData;
reg [27:0]testData1;
reg [27:0]readyDelay;
reg [27:0]mcu2fpga_ad_data_num;
reg [3:0]mcu2fpga_getdata_mode;
reg [27:0]mcu2fpga_time_clk_div;
reg mcu2fpga_grab_data;
reg [27:0]mcu2fpga_flag_reg;//标志寄存器，
always@(posedge clk)//串行写参数  --》初始化
begin
	case(M2S_ORDER)
		4'd0: begin 
			mcu2fpga_open_reuse <= 4'd2;
			mcu2fpga_grab_data<=1'b0; 
			pulse_delay_us <=  28'd1500;
			isOpen_AD_H8 <= 1'b0;
			AD_RSTn <= 1'b1;
			AD_ZCAL <= 1'b1;
			CLK_DIV <= 28'd0;
			waiteTime <=28'd1350;
			readyDelay<=28'd0;
			mcu2fpga_ad_data_num <= {4'd0,28'd8192} ;
			mcu2fpga_getdata_mode <= 4'd0;
			mcu2fpga_open_reuse<=1'b1; 
			mcu2fpga_time_clk_div <= 28'd0;
			mcu2fpga_flag_reg <= 28'd0;
		end  //默认参数   10us
		4'd1: begin pulse_delay_us <=  M2S_DATA;end//设置脉冲延时
		4'd2: begin isOpen_AD_H8 <= M2S_DATA[0];end//开启AD高8位数据有效性
		4'd3: begin AD_RSTn <= M2S_DATA[0];end//复位信号控制
		4'd4: begin AD_ZCAL <= M2S_DATA[0];end //FPGA_AD_ZCAL信号控制
		4'd5: begin CLK_DIV <= M2S_DATA;end //设置FPGA_AD_MCLK时钟（设置模块计数器基础时钟）
		4'd6: begin waiteTime <= M2S_DATA;end //设置LF398电压锁存后等待AD转换的时间 默认12us，所以默认waiteTime = 12 * 50 =600;
		4'd7: begin mcu2fpga_flag_reg <= M2S_DATA;end //使用LF398锁存后，只在ready信号有效时更新AD数据,,默认为0，接收数据后就立马更新AD读数  mcu2fpga_flag_reg:标志寄存器 
		4'd8: begin readyDelay <= M2S_DATA[27:0];end
		4'd9: begin mcu2fpga_ad_data_num <= {4'd0,M2S_DATA[27:0]};end//设置AD采样点数，满后存入DDR中，默认8192个点
		4'd10:begin mcu2fpga_time_clk_div <= M2S_DATA[27:0];end  //设置计时器基时钟频率  ,对AXI时钟分频得到
		4'd11:begin mcu2fpga_getdata_mode <= M2S_DATA[3:0];end//设置AD硬件采集数据的方式 ，0 ：连续采样  1：定时采集   2：触发采集  3:自测试模式
		4'd12:begin mcu2fpga_open_reuse <= M2S_DATA[3:0];end//开启通道2的ad数据寄存器服用为硬件采集数据丢失数据量 ，mcu2fpga_open_reuse ->1:返回丢失数量  0:关闭复用  other:返回硬件采集的DDR的有效数据量
		4'd13:begin mcu2fpga_grab_data <= M2S_DATA[0];end//上位机通知模块是否采集数据，1：采集   0：停止采集
		
		4'd14:begin testData1 <= M2S_DATA[27:0];end//测试数据1
		4'd15:begin TestData <= M2S_DATA[27:0]; end//测试数据
		default:begin 
			mcu2fpga_open_reuse <= 4'd2;
			pulse_delay_us <=  28'd1500;
			isOpen_AD_H8 <= 1'b0;
			AD_RSTn <= 1'b1;
			AD_ZCAL <= 1'b1;
			CLK_DIV <= 28'd0;
			waiteTime <=28'd1350;
			readyDelay<=28'd0;
			mcu2fpga_ad_data_num <= {4'd0,28'd8192} ;
			mcu2fpga_getdata_mode <= 4'd0;
		end  //默认参数   10us
	endcase
end


reg finish_flag;//完成一次数据转换标志
reg [5:0]R_cnt;
reg [5:0]L_cnt;
reg [31:0]ak5394_data_int;
always@(negedge finish_flag)
begin
	if(!rst_n)begin
		AD_DATA1_reg <= 24'd0;
		AD_DATA2_reg <= 24'd0;
		ak5394_data_int <= 32'd0;
	end
	else begin
		AD_DATA1_reg <= data1;
		AD_DATA2_reg <= data2;
		ak5394_data_int <= ddr_data1_int;
	end
end
always@(posedge SCLK)
begin
	if(!rst_n)begin
		data1 <= 24'd0;
		data2 <= 24'd0;
		R_cnt <= 6'd0;
		L_cnt <= 6'd0;
		finish_flag <= 1'b0;
	end
	else begin
		//finish_flag <= 1'b0;
		if(LRCK == 1'b0)begin
			R_cnt <= 6'd0;
			if(L_cnt < 6'd25)begin
				L_cnt <= L_cnt + 1'b1;
				data1 <= {data1[22:0],SDATA};
			end
			else begin
				L_cnt <= L_cnt;
				data1 <= data1;
			end
		end
		else begin
			 L_cnt <= 6'd0;
			 if(R_cnt< 6'd25)begin
				R_cnt <=  R_cnt + 1'b1;
				data2  <=  {data2[22:0],SDATA};               
			 end
			 else begin
				R_cnt <= R_cnt;
				data2 <= data2;			 
			 end
		end

		if(R_cnt == 6'd24)begin
			finish_flag <= 1'b1;
		end	
		else finish_flag <= 1'b0;
	end
end
//-----------------------触发脉宽截取----------------------------
reg [27:0]delay_cnt;
reg FPGA_LASER_PULSE_f1,FPGA_LASER_PULSE_f2;
wire FPGA_LASER_PULSE_P,FPGA_LASER_PULSE_N;
assign FPGA_LASER_PULSE_P =FPGA_LASER_PULSE_f1 & (~FPGA_LASER_PULSE_f2);//激光脉冲上升沿提取

always@(posedge clk)
begin
	if(rst_n == 1'b0)begin
		FPGA_LASER_PULSE_f1 <= 1'b0;
		FPGA_LASER_PULSE_f2 <= 1'b0;
	end 
	else begin
		FPGA_LASER_PULSE_f1 <= FPGA_LASER_PULSE;  //波形和时钟同步
		FPGA_LASER_PULSE_f2 <= FPGA_LASER_PULSE_f1;	
	end
end

always@(posedge clk)	
begin
    if(rst_n ==1'b0)begin
        FPGA_LOGIC_LF398 <= 1'b0;
    end
	else if(mcu2fpga_flag_reg[24])begin 
		FPGA_LOGIC_LF398 <= mcu2fpga_flag_reg[23];//手动设置LF398的逻辑电压
	end
	else if(FPGA_LASER_PULSE_P)begin
		delay_cnt <= 28'd0;
		FPGA_LOGIC_LF398 <= 1'b1;
	end
	else begin
		if(delay_cnt < pulse_delay_us - 1'b1)begin
			FPGA_LOGIC_LF398 <= 1'b1;
			delay_cnt <= delay_cnt + 28'd1;
		end
		else begin
			delay_cnt <= delay_cnt;
			FPGA_LOGIC_LF398 <= 1'b0;
		end
	end
end

reg FPGA_LOGIC_LF398_delay1,FPGA_LOGIC_LF398_delay2;
reg [27:0]delay_cnt1;
wire FPGA_LOGIC_LF398_N;//检测下降沿
assign FPGA_LOGIC_LF398_N = (~FPGA_LOGIC_LF398_delay1) & (FPGA_LOGIC_LF398_delay2); //negedge
wire [27:0]waitTime1;
assign waitTime1 = waiteTime + readyDelay;
//读取信号,当LF398锁存时产生ready信号
always@(posedge clk)
begin
	FPGA_LOGIC_LF398_delay1 <= FPGA_LOGIC_LF398;  //产生边沿提取信号
	FPGA_LOGIC_LF398_delay2 <= FPGA_LOGIC_LF398_delay1;
	if(FPGA_LOGIC_LF398_N == 1'b1)begin
		delay_cnt1 <= 28'd0;
		ready <= 1'b0;
	end
	else begin
			if(delay_cnt1 < waiteTime - 1'b1)begin
				delay_cnt1 <= delay_cnt1 + 1'b1;
				ready <= 1'b0;
			end
			else begin
				if(delay_cnt1 < waitTime1  )begin
					ready <= 1'b1;
					delay_cnt1 <= delay_cnt1 + 1'b1;
				end
				else begin
					ready <= 1'b0;
					delay_cnt1 <= delay_cnt1;
				end
			end
	end
end

//锁存数据
always@(posedge ready)
begin
	data1_lf398 <= data1_int;
	data2_lf398 <= data2_int;	
end
//--------------------时钟分频-输出FPGA_AD_MCLK和记时间时钟-------------------------
//在1.1版本，这个分频设计用于硬件采集数据的自测，驱动AK5394的时钟使用PLL
//产生!
wire clk_base;
reg [27:0]AD_MCLK_CNT;
reg AD_MCLK_reg;
reg time_base_clk_reg;
reg [27:0]time_base_clk_cnt;
assign time_base_clk = (mcu2fpga_time_clk_div == 28'd1)? clk : time_base_clk_reg;//获取计时器基时钟
always@(posedge clk)
begin
	if(CLK_DIV == 28'd0)begin
		AD_MCLK_reg <= 1'b0;
	end
	else begin
		if(CLK_DIV == 28'd1)begin
			AD_MCLK_reg <= AD_MCLK_reg;
		end
		else begin
			if(AD_MCLK_CNT <= (CLK_DIV>>1) - 1'b1)
				AD_MCLK_reg <= 1'b1;
			else 
				AD_MCLK_reg <= 1'b0;
				
			//计数器
			if(AD_MCLK_CNT >= CLK_DIV - 1'b1)
				AD_MCLK_CNT <= 28'd0;
			else
				AD_MCLK_CNT <= AD_MCLK_CNT + 1'b1;
		end
	end
////////////////////TIME_BASE_CLK////////////////////////////
	if(mcu2fpga_time_clk_div == 28'd0)begin
		time_base_clk_reg <= 1'b0;
	end
	else begin
		if(mcu2fpga_time_clk_div == 28'd1)begin
			time_base_clk_reg <= time_base_clk_reg;
		end
		else begin
			if(time_base_clk_cnt <= (mcu2fpga_time_clk_div>>1) - 1'b1)
				time_base_clk_reg <= 1'b1;
			else 
				time_base_clk_reg <= 1'b0;
				
			//计数器
			if(time_base_clk_cnt >= mcu2fpga_time_clk_div - 1'b1)
				time_base_clk_cnt <= 28'd0;
			else
				time_base_clk_cnt <= time_base_clk_cnt + 1'b1;
		end
	end
end
//---------------------------更新DDR写数据源------------------
reg [1:0]getdata_cnt;
reg cache_flag;
reg [31:0]data_reg;
reg finish_flag_ff1;
reg finish_flag_ff2;
wire tran_data_done_flag ;
assign tran_data_done_flag = (~finish_flag_ff1) & finish_flag_ff2;//AK5394完成数据转换，并将数据所存到寄存器了
//模式1触发信号：连续采集数据
always@(posedge clk)
begin
    if(rst_n == 1'b0)
    begin
        finish_flag_ff1 <= 1'b0;
        finish_flag_ff2 <= 1'b0;
    end
    else 
        begin
             finish_flag_ff1 <= finish_flag;
             finish_flag_ff2 <= finish_flag_ff1;
        end
end
//----------------------------------------------------
wire data_latch;
reg n_finished_f1;
reg n_finished_f2;
assign data_latch = n_finished_f1 & (~n_finished_f2);
always@(negedge clk)
begin
	if(rst_n ==1'b0)begin
		n_finished_f1 <= 1'b0;
		n_finished_f2 <= 1'b0;
	end
	else begin
		n_finished_f1 <= finish_flag;
		n_finished_f2 <= n_finished_f1;
	end
end
//当数据完成时将数据锁存到发送数据寄存器
reg [127:0] data_reg_buf;//数据缓存队列
reg [31:0]data_temp;
always@(negedge clk)
begin
     if(rst_n == 1'b0)begin
         data_reg <= 32'd0; 
		 data_reg_buf <= 32'd0;
		 data_temp <= 32'd0;
    end
    else begin  	
		//data_reg <= ddr_data1_int; //数据锁存
		if(data_latch)begin
			data_temp[31] <= data1[23];
			data_temp[30:0] <=data1[23]?{8'hff,data1[22:0]}:{8'd0,data1[22:0]} ;//数据位置
			data_reg_buf <= {data_reg_buf[95:0],data_temp} ;//缓存过去的3个数据，用于解决当前数据出错的情况,当前数据出错就进行取上一次的数据代替
			data_reg <= (mcu2fpga_flag_reg[26] == 1'b0)? data_reg_buf[31:0] ://直接发数据，以下为硬件滤波
						(data_reg_buf[31:23] == 9'd0 || data_reg_buf[31:23] == 9'b1111_1111_1) ? data_reg_buf[31 : 0] :
						(data_reg_buf[63:55] == 9'd0 || data_reg_buf[63:55] == 9'b1111_1111_1) ? data_reg_buf[63 :32] :
						(data_reg_buf[95:87] == 9'd0 || data_reg_buf[95:87] == 9'b1111_1111_1) ? data_reg_buf[95 :64] :
						data_reg_buf[127:96];		
		end
		else begin
			data_reg <= data_reg;
		end
    end
end
//模式3触发信号：外部触发采集数据
reg ready_ff1;
reg ready_ff2;
wire ready_one_trigger;
assign ready_one_trigger = ready_ff1 & (~ready_ff2);
always@(posedge clk)
begin
    if(rst_n == 1'b0)begin
        ready_ff1 <= 1'b0;
        ready_ff2 <=1'b0;
    end
    else begin
        ready_ff1 <= ready;
        ready_ff2 <= ready_ff1;    
    end
end
////模式2触发信号：定时采集触发信号
wire pl_timer;//模块定时器，用于设置定时采集，在测试模式下，该时钟模拟激光脉冲
assign pl_timer = FPGA_AD_MCLK;//使用原本用于确定AK5394的时钟，现在驱动ak5394的时钟使用PLL产生
reg timer_ff1;
reg timer_ff2;
wire timer_trigger;
assign timer_trigger = timer_ff1 & (~timer_ff2);//get posedge
always@(posedge clk)
begin
    if(rst_n == 1'b0)begin
        timer_ff1 <= 1'b0;
        timer_ff2 <=1'b0;
    end
    else begin
        timer_ff1 <= pl_timer;//AD_MCLK_reg;
        timer_ff2 <= timer_ff1;    
    end
end
//模式4自测触发信号，生成测试数据和DDR触发信号
reg testSelf_trigger;
reg [31:0]testSelf_data;
reg [27:0]testSelf_cnt;
reg [27:0]testNum_cnt;
reg testDir ;
reg testDir_ff1;
wire testDirNP;
assign testDirNP = (testDir & (~testDir_ff1)) | ((~testDir) & testDir_ff1);//获取模块自测跳变沿
always@(posedge clk)
begin
	if((rst_n == 1'b0) || (mcu2fpga_grab_data == 1'b0))begin
		testSelf_trigger <= 1'b0;
		testSelf_data <= {4'd0,TestData};
		testSelf_cnt <= 16'd0;
		testNum_cnt <= 28'd0;
		testDir = 1'b0;
		testDir_ff1 <= 1'b0;
	end
	else begin
		if(testSelf_cnt >= testData1[27:0])begin
			testSelf_cnt <= 28'd0;
			testSelf_trigger <= 1'b1;
			testSelf_data <= (send_data_rst)? {4'd0,TestData} :testSelf_data + 32'd1 ;
			testNum_cnt <= testNum_cnt + 1'b1;
		end
		else begin
			testSelf_cnt <= testSelf_cnt +1'b1;
			testSelf_trigger <= 1'b0;

			if(testNum_cnt >= TestData)begin
				if(testNum_cnt >= TestData + 100)begin
					testDir <= ~testDir;
					testNum_cnt <= 32'd0;
				end
				else begin
					testNum_cnt <= testNum_cnt + 1'b1;
				end
			end
		end
		testDir_ff1 <= testDir;
	end
end
//动镜方向信号截取，产生
reg move_dir_ff1;
reg move_dir_ff2;
wire move_dir_p;//提取动镜方向上升沿 ,该信号必须与在第一个触发信号到时有效
wire move_dir_pn;//动镜换向信号
assign move_dir_p = move_dir_ff1 & (~move_dir_ff2);
assign move_dir_pn = (move_dir_ff1 & (~move_dir_ff2)) | ((~move_dir_ff1) & move_dir_ff2);
reg move_dir;
reg [1:0]dir_change_state;
reg tx_clear_flag;
//assign move_dir = move_dir_ff1;//对动镜方向信号进行同步操作
always@(posedge clk)
begin
	if(rst_n == 1'b0)begin
		move_dir_ff1 <= 1'b0;
		move_dir_ff2 <= 1'b0;
	end
	else begin
		move_dir_ff1 <= FPGA_DongJ_DIR;
		move_dir_ff2 <= move_dir_ff1;		
	end
end
///记录每个点的时间,在动镜反向的时候清零计数器，
//进行重新计数
reg [31:0]ad_time_cnt;
wire time_clk;
assign time_clk = time_base_clk; //时间计数基时钟（时钟上位机设置为10MHz）
wire time_cnt_clear ;
//利用N触发,方向信号发生跳变时产生的一个高电平信号,并在此时清零计数器的值
assign time_cnt_clear = (mcu2fpga_getdata_mode == 4'd3)? ((testDirNP) & rst_n) :((move_dir_pn) & rst_n);  //testDirNP
reg send_1st_data_flag;//发送第一个数据标志
reg send_1st_data_arc;//数据发送后反馈该信号，以清除发送第一个数据的标志有效性
//内部时间计时器
reg [31:0] pulse_time_cnt;
always@(negedge time_clk or posedge time_cnt_clear )//posedge time_clk
begin
	if(time_cnt_clear == 1'b1)begin
		ad_time_cnt <= 'd0;
		pulse_time_cnt <= 32'd0;
	end
	else begin
		ad_time_cnt <=  ad_time_cnt +1'b1 ;
		pulse_time_cnt <= pulse_time_cnt + 1'b1;
	end
end
//模块信号交互，告知数据准备模块复位地址，检测到数据准备模块完成后复位通知信号
reg send_1st_data_flag_ff1;
wire send_1st_data_flag_p ;
assign send_1st_data_flag_p = send_1st_data_flag &(~send_1st_data_flag_ff1);
always@(posedge clk or posedge time_cnt_clear)
begin
	if( time_cnt_clear == 1'b1 )begin
		send_1st_data_flag <= rst_n ? 1'b1 : 1'b0 ;//通知数据准备模块发送第一个数据，
		//send_1st_data_flag_ff1 <= send_1st_data_flag;
	end
	else begin//获得发送第一个数据反馈后，将通知信号拉低复位
		send_1st_data_flag <= send_1st_data_arc ? 1'b0 : send_1st_data_flag ;
		//send_1st_data_flag_ff1 <= send_1st_data_flag;
	end
end
always@(posedge clk)//延时一个clk
	send_1st_data_flag_ff1 <= send_1st_data_flag;
//---------------------------记录激光脉冲的时间,并将其写入DDR中----------------------------------------
//step1:同步激光脉冲  前面已经得到 激光上升沿产生高电平 FPGA_LASER_PULSE_P
//step3:产生发送第一个数据请求,并等待第一个激光脉冲来后恢复状态
wire laser_p;//激光脉冲上升沿
//调试模式下使用FPGA_AD_MCLK(pl_timer)作为模拟激光脉冲源
	wire debug_laser_p;
	reg debug_laser_ff1;
	reg debug_laser_ff2;
	assign debug_laser_p = pl_timer &(~debug_laser_ff1);
	assign laser_p = (mcu2fpga_getdata_mode==4'd3)? debug_laser_p : FPGA_LASER_PULSE_P;//;FPGA_LASER_PULSE_P ;
	always@(posedge clk)
	begin
		if(rst_n == 1'b0)begin
			debug_laser_ff1 <= 1'b0;
			//debug_laser_ff2 <=1'b0;
		end
		else begin
			debug_laser_ff1 <= pl_timer;
			//debug_laser_ff2 <=debug_laser_ff1;			
		end
	end

//-------------

//step5:向DDR写数据
reg [31:0] pluser_time_valid_data ;//一个方向的数据有效量

////向AXI请求发送数据信号，并发送给AXI总线的数据。
reg [31:0]send_axi_data;//发送给AXI总线的ad数据。
reg [31:0]send_axi_time;//发送给AXI总线的时间t数据。
reg tx_flag;//发送data有效标志,启动数据传输
//reg [31 : 0]same_dir_data_cnt;//记录同一方向的ad数据个数
reg send_data_rst;
reg [31:0]ad_data_cnt ;
reg [31:0]laser_pulse_time_cnt;

//产生数据ok中断，触发ARM刷新对应的缓存，并对数据进行读取
wire [31:0]DDR_TIME_BASE_ADDR;//存储时间的DDR基地址
wire [31:0] DDR_PULSE_TIME_BASE_ADDR;//动态计算DDR激光脉冲数据写入的基地址
wire TIME_CNT_BIT;//时间计时器位宽选择 0:32  1:16
assign TIME_CNT_BIT = mcu2fpga_flag_reg[27];
assign DDR_TIME_BASE_ADDR = DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 3);//计算写时间的基地址
assign ddr_ad_chk = ddr_chk ; //中断信号
assign DDR_PULSE_TIME_BASE_ADDR = DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 3);

reg [95:0]send_axi_data_buf;// [95:64]ad data  [63:32]ad time data  [31:0]laser time data
reg [95:0]send_axi_addr_buf;//对应axi数据的写入地址

//-----------------------------调用fifo，解决数据速度匹配问题-------------------------------------------------------
//激光脉冲的和获取AD的数据之间存在速度不一致的问题，就是数据不是同一时刻更新，ad数据获取的速度要比激光脉冲数据更新的频率快
//因此当激光脉冲到来时，将其时间数据写入深度为16的fifo中，在ad数据更新时，查询fifo，若fifo非空，就从fifo中取出一个数据，跟随
//ad数据一起写入DDR中，这样的操作保证了ad的数据量一定是大于等于激光脉冲的数据量的
reg wr_en;
reg rd_en;

reg fifo_rst_n;//FIFO复位信号
reg [31:0]fifo_addr;//第一个fifo的写入数据
reg [31:0]fifo_data;//第二个fifo的写入数据
wire[31:0]fifo_data_o;//读出的fifo数据
wire[31:0]fifo_addr_o;
wire fifo_full1;
wire fifo_full2;
wire fifo_full;
wire fifo_empty1;//fifo判断空满
wire fifo_empty2;
wire fifo_empty;
assign fifo_empty = fifo_empty1 & fifo_empty2;//两个fifo空信号合并（两个fifo是同步的，每一时刻所存取的数据量是一样的，写入和读出都是同步的）
assign fifo_full = fifo_full1 & fifo_full2;
fifo_hpy #(
	.BUF_WIDTH(4),//读写指针数据位宽
	.BUF_SIZE(16),//fifo的深度
	.DATA_WIDTH(32)//fifo中读写数据的位宽
)fifo_data_inst 
(
	.clk(clk),
	.rst_n(fifo_rst_n),
	.buf_in(fifo_data),
	.buf_out(fifo_data_o),
	.wr_en(wr_en),
	.rd_en(rd_en),
	.buf_empty(fifo_empty1),
	.buf_full(fifo_full1)
	//.fifo_cnt()
);

fifo_hpy #(
	.BUF_WIDTH(4),
	.BUF_SIZE(16),
	.DATA_WIDTH(32)
) fifo_addr_inst
(
	.clk(clk),
	.rst_n(fifo_rst_n),
	.buf_in(fifo_addr),
	.buf_out(fifo_addr_o),
	.wr_en(wr_en),
	.rd_en(rd_en),
	.buf_empty(fifo_empty2),
	.buf_full(fifo_full2)
	//.fifo_cnt()
);
//迟滞fifo使能写数据一个clk，以保证fifo复位,清空数据
reg wr_en_d;
reg [31:0]fifo_addr_d;
reg [31:0]fifo_data_d;
always@(posedge clk)
begin
	if(!rst_n)begin
		wr_en <= 1'b0;
		fifo_addr <= 32'd0;
		fifo_data <= 32'd0;
	end
	else begin
		wr_en <= wr_en_d;
		fifo_addr <= fifo_addr_d;
		fifo_data <= fifo_data_d;
	end
end
//----------------------------------将激光脉冲数据写入fifo中---------------------------------------
always@(posedge clk)
begin
	if(rst_n == 1'b0)begin
		fifo_rst_n <= 1'b0;
		wr_en_d <= 1'b0;
	end
	else begin
		if(send_1st_data_flag_p)begin
			fifo_addr_d <= (mcu2fpga_getdata_mode == 4'd3)? 
									(testDir? DDR_PULSE_TIME_BASE_ADDR:DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2)):
									(FPGA_DongJ_DIR?DDR_PULSE_TIME_BASE_ADDR : DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2));
			fifo_data_d <= mcu2fpga_flag_reg[27] ? {16'd0 , pulse_time_cnt[15:0] } :pulse_time_cnt;
			fifo_rst_n <= 1'b0;
			wr_en_d <= 1'b1;		
		end
		else begin
			if(laser_p)begin
				fifo_addr_d <= mcu2fpga_flag_reg[27] ? fifo_addr_d + 32'd2 : fifo_addr_d + 32'd4;
				fifo_data_d <= mcu2fpga_flag_reg[27] ? {16'd0,pulse_time_cnt[15:0]} : pulse_time_cnt;
				fifo_rst_n <= 1'b1;
				wr_en_d <= 1'b1;	
			end
			else begin
				fifo_addr_d <= fifo_addr_d;
				fifo_data_d <= fifo_data_d;
				fifo_rst_n <= 1'b1;
				wr_en_d <= 1'b0;			
			end
		end
	end
end
//-------------------------------------将数据合成送入AXI写入到DDR中---------------------------------------------------
always@(posedge clk)
begin
    if(rst_n == 1'b0)begin
		tx_flag <= 1'b0;
		send_1st_data_arc <= 1'b0;
		send_data_rst <= 1'b0;
		ad_data_cnt <= 32'd0;
		laser_pulse_time_cnt <= 32'd0;
		send_axi_data_buf[95:0] <= 95'd0;
		send_axi_addr_buf <= {DDR_BASE_ADDR , DDR_TIME_BASE_ADDR , DDR_PULSE_TIME_BASE_ADDR};
		rd_en <= 1'b0;
    end
    else begin
  //激光脉冲时间和
/*
			if(send_1st_data_flag_p)begin//换向上升沿信号
				send_axi_addr_buf[31:0 ] <= (mcu2fpga_getdata_mode == 4'd3)? (testDir? DDR_PULSE_TIME_BASE_ADDR:DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2)):
																		(FPGA_DongJ_DIR?DDR_PULSE_TIME_BASE_ADDR : DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2));
				send_axi_data_buf[31:0] <= pulse_time_cnt;//pulse_time_cnt;
				laser_pulse_time_cnt <= 32'd0;
				pluser_time_valid_data <= laser_pulse_time_cnt;
			end
			else begin
				if(laser_p)begin //laser_p
					send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ] + 32'd4;
					send_axi_data_buf[31:0] <= pulse_time_cnt;
					laser_pulse_time_cnt <= laser_pulse_time_cnt + 1'b1;
				end
				else begin
					send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0];
					send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
				end		
			end
*/			
 //AD时间和AD数据
			case(mcu2fpga_getdata_mode)
				4'd0:begin  //连续采集,不丢数据  
						if(tran_data_done_flag)begin
							send_axi_data <=  data_reg ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							//send_axi_data_buf[95:32] <= {data_reg , ad_time_cnt};//发送数据
							//                                                             16位宽数据                            32位位宽
							send_axi_data_buf[95:32] <= mcu2fpga_flag_reg[27] ?  {data_reg , {16'd0,ad_time_cnt[15:0]}} : {data_reg , ad_time_cnt};
							if(FPGA_DongJ_DIR)begin //发送地址
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end
							end
							else begin
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2: send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end
					///////////////////////////////读FIFO数据//////////////////////////////////////////
							if(!fifo_empty)begin
								rd_en <= 1'b1;
								send_axi_addr_buf[31:0] <= fifo_addr_o;
								send_axi_data_buf[31:0] <= fifo_data_o;
								laser_pulse_time_cnt <= send_1st_data_flag ? 32'd0 : laser_pulse_time_cnt + 1'b1;
							end
							else begin
								rd_en <= 1'b0;
								send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
								send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
								laser_pulse_time_cnt <= laser_pulse_time_cnt;
							end
				/////////////////////////////////FIFO读取完成////////////////////////////////////////////
						end
						else begin
							send_axi_data <= send_axi_data;
							tx_flag <= 1'b0;
							send_axi_time <= send_axi_time;
							send_data_rst <= 1'b0;
							/////////////////////////////////////
							send_axi_data_buf <= send_axi_data_buf;
							send_axi_addr_buf[95:64] <= send_axi_addr_buf[95:64] ;
							send_axi_addr_buf[63:32] <= send_axi_addr_buf[63:32] ;
							//send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ];
							send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
							send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
							ad_data_cnt <= ad_data_cnt;
							rd_en <= 1'b0;
							laser_pulse_time_cnt <= laser_pulse_time_cnt;
						end
						move_dir <= FPGA_DongJ_DIR;
					end
				4'd1:begin  //定时采集
						if(timer_trigger)begin
							send_axi_data <=  data_reg ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							send_axi_data_buf[95:32] <= mcu2fpga_flag_reg[27] ? {data_reg ,{16'd0,ad_time_cnt[15:0]} } : {data_reg , ad_time_cnt};//发送数据
							if(FPGA_DongJ_DIR)begin //发送地址
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end
							end
							else begin
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR  + (mcu2fpga_ad_data_num << 2);
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end	
					///////////////////////////////读FIFO数据//////////////////////////////////////////
							if(!fifo_empty)begin
								rd_en <= 1'b1;
								send_axi_addr_buf[31:0] <= fifo_addr_o;
								send_axi_data_buf[31:0] <= fifo_data_o;
								laser_pulse_time_cnt <= send_1st_data_flag ? 32'd0 : laser_pulse_time_cnt + 1'b1;
							end
							else begin
								rd_en <= 1'b0;
								send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
								send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
								laser_pulse_time_cnt <= laser_pulse_time_cnt;
							end
				/////////////////////////////////FIFO读取完成////////////////////////////////////////////
						end
						else begin
							send_axi_data <= send_axi_data;
							tx_flag <= 1'b0;
							send_axi_time <= send_axi_time;
							send_data_rst <= 1'b0;	
							/////////////////////////////////////
							send_axi_data_buf[95:32] <= send_axi_data_buf[95:32];
							send_axi_addr_buf[95:64] <= send_axi_addr_buf[95:64] ;
							send_axi_addr_buf[63:32] <= send_axi_addr_buf[63:32] ;
							//send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ];	
							send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
							send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
							ad_data_cnt <= ad_data_cnt;
							rd_en <= 1'b0;
							laser_pulse_time_cnt <= laser_pulse_time_cnt;
						end
						move_dir <= FPGA_DongJ_DIR;
					end
				4'd2:begin //触发采集
						if(ready_one_trigger)begin
							send_axi_data <=  data_reg ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;	
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							send_axi_data_buf[95:32] <= mcu2fpga_flag_reg[27] ? {data_reg , { 16'd0, ad_time_cnt[16:0]}} : {data_reg , ad_time_cnt};//发送数据
							if(FPGA_DongJ_DIR)begin //发送地址
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd4 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
								end
							end
							else begin
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2: send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2: send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end	
					///////////////////////////////读FIFO数据//////////////////////////////////////////
							if(!fifo_empty)begin
								rd_en <= 1'b1;
								send_axi_addr_buf[31:0] <= fifo_addr_o;
								send_axi_data_buf[31:0] <= fifo_data_o;
								laser_pulse_time_cnt <= send_1st_data_flag ? 32'd0 : laser_pulse_time_cnt + 1'b1;
							end
							else begin
								rd_en <= 1'b0;
								send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
								send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
								laser_pulse_time_cnt <= laser_pulse_time_cnt;
							end
				/////////////////////////////////FIFO读取完成////////////////////////////////////////////							
						end
						else begin
							send_axi_data <= send_axi_data;
							tx_flag <= 1'b0;
							send_axi_time <= send_axi_time;
							send_data_rst <= 1'b0;	
							/////////////////////////////////////
							send_axi_data_buf[95:32] <= send_axi_data_buf[95:32];
							send_axi_addr_buf[95:64] <= send_axi_addr_buf[95:64] ;
							send_axi_addr_buf[63:32] <= send_axi_addr_buf[63:32] ;
							//send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ];
							send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];//激光脉冲时标数据
							send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
							ad_data_cnt <= ad_data_cnt ;
							rd_en <= 1'b0;
							laser_pulse_time_cnt <= laser_pulse_time_cnt;
						end
						move_dir <= FPGA_DongJ_DIR;
					end
				4'd3:begin //模块自测
						if(testSelf_trigger)begin
							send_axi_data <=  send_1st_data_flag? {4'd0,TestData}  :send_axi_data + 1'b1 ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;//send_1st_data_flag? 32'd0  :send_axi_time + 1'b1 ;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							if(mcu2fpga_flag_reg[27] == 1'b0)send_axi_data_buf[95:32] <= send_1st_data_flag?{32'd0 , ad_time_cnt} : {{send_axi_data_buf[95:64] + 1'b1} ,ad_time_cnt};//发送数据
							else send_axi_data_buf[95:32] <= send_1st_data_flag ? {32'd0 , {16'd0,ad_time_cnt[15:0]} } : {{ 16'd0,{send_axi_data_buf[79:64] + 1'b1} } ,{16'd0,ad_time_cnt[15:0]}};//发送数据
							//send_axi_data_buf[31:0] <= send_1st_data_flag?32'd0: send_axi_data_buf[31:0]  + 1'b1;
							if(testDir)begin //发送地址
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;

									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2: send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ] + 32'd4;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end
							end
							else begin
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//更新有效数据
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ] + 32'd4;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end	
					///////////////////////////////读FIFO数据//////////////////////////////////////////
							if(!fifo_empty)begin
								rd_en <= 1'b1;
								send_axi_addr_buf[31:0] <= fifo_addr_o;
								send_axi_data_buf[31:0] <= fifo_data_o;
								laser_pulse_time_cnt <= send_1st_data_flag ? 32'd0 : laser_pulse_time_cnt + 1'b1;
							end
							else begin
								rd_en <= 1'b0;
								send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
								send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
								laser_pulse_time_cnt <= laser_pulse_time_cnt;
							end
				/////////////////////////////////FIFO读取完成////////////////////////////////////////////
						end
						else begin
							send_axi_data <= send_axi_data;
							tx_flag <= 1'b0;
							send_axi_time <= send_axi_time;
							send_data_rst <= 1'b0;
							//////////////////////////
							send_axi_data_buf[95:32] <= send_axi_data_buf[95:32];
							send_axi_addr_buf[95:64] <= send_axi_addr_buf[95:64] ;
							send_axi_addr_buf[63:32] <= send_axi_addr_buf[63:32] ;
							send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];
							send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
							ad_data_cnt <= ad_data_cnt;
							rd_en <= 1'b0;
							laser_pulse_time_cnt <= laser_pulse_time_cnt;
						end
						move_dir <= testDir;
				end
				default: begin
					tx_flag <= 1'b0;
					send_axi_data <= 32'd0;
					send_axi_time <= 32'd0;
					move_dir <= move_dir;
					/////////////////////////////////////
					send_axi_data_buf[95:0] <= send_axi_data_buf[95:0];
					send_axi_addr_buf[95:0] <= send_axi_addr_buf[95:0];
					ad_data_cnt <= ad_data_cnt;
					rd_en <= 1'b0;
				end           
            endcase//end 采集模式	
			send_1st_data_arc <= send_data_rst ? 1'b1 : 1'b0;
    end  
end

//Generate a pulse to initiate send data to ddr.


//--------------------------------------完成DDR数据写入---------------------------------
reg [63:0] wr_data_reg;  //[ad数据，时间数据]
reg  wr_data_ready_reg;
reg [31:0] wraddr_base_reg;

reg [31:0] wr_time_reg;
//reg  wr_time_ready_reg;
//reg [31:0]wraddr_base_time_reg;

reg ddr_chk;
reg [2:0]pre_ddr_state;
reg [27:0]addr_cnt;
reg inte1;
reg inte2;
reg [31:0]ad_valid_data;//动镜同一方向的数据量，该数只会在动镜方向改变的时候更新数据。
reg [31:0]time_dst_r;//时间存储地址偏移

reg [95:0]wr_data_buf_reg;
reg [95:0]wr_addr_buf_reg;

always@(posedge clk)
begin
    if( rst_n == 1'b0)begin
        pre_ddr_state <= 3'b001;  
        wr_data_ready_reg <= 1'b0;
		inte1 <= 1'b0;
		ddr_chk <= 1'b0;
		
		wr_data_buf_reg <= 96'd0;
		wr_addr_buf_reg <= {DDR_BASE_ADDR , DDR_TIME_BASE_ADDR ,DDR_PULSE_TIME_BASE_ADDR};
    end
    else begin
        case(pre_ddr_state) //The state machine
            3'b001:begin  //start awake PL write DDR
                    wr_data_ready_reg <= 1'b1;                 
                    addr_cnt<=28'd0; 
					pre_ddr_state <= 3'b010;
                end    
             3'b010:begin  //END awake PL write DDR 
                    wr_data_ready_reg <= 1'b0;             
                    addr_cnt<=28'd0;
					pre_ddr_state <= 3'b100;
                end 
             3'b100:begin  //write DDR signal 
                    if(ddr_write_done_arc && tx_flag && mcu2fpga_grab_data && (1'b0 == IsBufMax(DDR_BASE_ADDR , send_axi_addr_buf , mcu2fpga_ad_data_num )) )begin                
                        wr_data_ready_reg <= 1'b1; //genarate puls 
						wr_data_buf_reg <= send_axi_data_buf;
						wr_addr_buf_reg <= send_axi_addr_buf;
						//////////////////////////////////////////
						if(move_dir)begin//动镜向其中一个方向运动
							if(send_data_rst)begin
								ddr_chk <= 1'b1;
								inte1 <= 1'b1;
								//ad_valid_data <= ad_data_cnt ;//更新有效数据
								//pluser_time_valid_data <= laser_pulse_time_cnt;
							end
							else begin
								inte1 <= 1'b0;
							end
						end
						else begin
							if(send_data_rst)begin
								ddr_chk <= 1'b0;
								inte1 <= 1'b1;
								//ad_valid_data <= ad_data_cnt;//更新有效数据
								//pluser_time_valid_data <= laser_pulse_time_cnt;
							end
							else begin
								inte1 <= 1'b0;
							end
						end            
                    end
                    else begin
						inte1 <= 1'b0;
						wr_data_ready_reg <= 1'b0;
                    end
					inte2 <= inte1;
                    pre_ddr_state <= 3'b100;
                end 
              default: pre_ddr_state <= 3'b001;              
        endcase 
    end
end





//提取中断信号
//说明：此处提取中断信号上升沿，检测到上升沿后，将中断信号高电平持续一个clock的时间
//将该信号接入axi GPIO,使用axi gpio中断传入ARM，因为axi gpio在信号上升沿和下降沿都会
//产生一个高电平，为了让arm只在一次数据更新中北触发一次，通过该模块产生一个高电平持
//续时间为一个clock的中断，在axi gpio检测到到边沿后，将会在该信号上升沿和下降沿都产生
//一个clock的高电平，因此axi gpio接收到这个中断信号后会产生高电平持续时间为2个clock的
//中断信号，ARM接收到中断信号和产生的中断信号，将会滞后至少为1个时钟周期。
//assign ddr_ad_data_inte = inte1 &(~inte2) ;//get posedge signal
                   ////////////////////////////////同步///////////////异步/////////////
assign ddr_ad_data_inte =inte1 &(~inte2);//get posedge signal
//------------------------输出状态，让PS获取IP的状态数据-----------------------------
//将各信号的状态输出
always@(posedge clk)
begin
	if(rst_n == 1'b0)begin
		STATUS[0] <=  32'd0;
	end
	else 
		begin
			 STATUS[31] <= ready;
			 STATUS[30] <= ddr_chk; 
			 STATUS[29] <= ddr_ad_data_inte;
			 STATUS[28] <= mcu2fpga_flag_reg[27];
			 STATUS[27] <= mcu2fpga_flag_reg[25];
			 STATUS[26] <= mcu2fpga_flag_reg[24];
			 STATUS[25] <= mcu2fpga_flag_reg[23];
			 STATUS[24] <= mcu2fpga_flag_reg[22];
			 STATUS[23] <= mcu2fpga_flag_reg[21];

			 STATUS[10]<=isOpen_AD_H8;
			 STATUS[9]<=FPGA_AD_RSTn;
			 STATUS[8]<=FPGA_AD_ZCAL;
			 STATUS[7]<=FPGA_AD_MCLK;
			 STATUS[6]<=FPGA_AD_FSYNC;
			 STATUS[5]<=FPGA_AD_SDATA;
			 STATUS[4]<=FPGA_AD_LRCK;
			 STATUS[3]<=FPGA_AD_SCLK;
			 STATUS[2]<=FPGA_LOGIC_LF398;
			 STATUS[1]<=FPGA_LASER_PULSE;
			 STATUS[0]<=FPGA_DongJ_DIR;
		end 
end
//丢失数据计数器
//reg [31:0]miss_data_cnt;  mcu2fpga_open_reuse
always@(posedge clk)
begin
	if(rst_n == 1'b0 || addr_cnt==28'd0)begin
		miss_data_cnt <= 32'd0;
	end
	else begin
		if(tx_flag && (!ddr_write_done_arc) )miss_data_cnt <= miss_data_cnt + 1'b1;
		else miss_data_cnt <= miss_data_cnt;			
	end
end
always@(posedge clk)
begin
	miss_data_led <= (tx_flag && (!ddr_write_done_arc) && (pre_ddr_state == 3'b100) && mcu2fpga_grab_data) ? 1'b0:1'b1 ; 
end
//AXI 数据
assign AD_DATA1 = mcu2fpga_flag_reg[25] ? pluser_time_valid_data : data_reg ;
//assign AD_DATA2 = (mcu2fpga_open_reuse != 4'd0)? miss_data_cnt : (Suspend_AD ?data2_lf398 : data2_int);
assign AD_DATA2 = (mcu2fpga_open_reuse == 4'd0)?  (data2_int): ((mcu2fpga_open_reuse == 4'd1)?miss_data_cnt : ad_valid_data) ;


//输出，AXI的地址和数据，以及发送请求
assign wr_data_buf = wr_data_buf_reg;
assign wr_addr_buf = wr_addr_buf_reg;
assign wr_data_ready = wr_data_ready_reg;
//用于确定第二个发送数据的地址，一直和ad数据存储地址相差相同地址，保证存一个ad数据，在对应的位置存储时间
//assign addr_offset = (mcu2fpga_ad_data_num << 3);


//--------------------------------------------------------------------------------------
endmodule 

