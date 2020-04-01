/*****************************************************************
*˵����AK5394+LF398�Ŀ�������,����qsw�ĺ���AD��·��
*Author:hpy
*Date:2019/6/6
*****************************************************************/


/*------------------------------�޸ļ�¼----------------------------
         �汾          |  ˵��    |      �޸���
version��AD_LF398_V1.0 | ��һ��   | hpy
version��AD_LF398_V1.1 | �ڶ���   | hpy  ������Ӳ���ɼ�����
version��AD_LF398_V2.0 | ������   | hpy  ������ʱ���¼����
-----------------------------------------------------------------*/
`define _DEBUG_ 
module AK5394_2QSW #
(
	parameter integer DDR_BASE_ADDR	= 32'H04000000
)
( 
		input rst_n, 
		input clk,//ϵͳʱ�� 50MHz / 150MHz
		
		input [31:0]MCU2SLAVE,  //��λ�����͵�ָ��Ĵ���  [31:28]ָ��λ  [27:0]����λ
		input FPGA_DongJ_DIR,//�����ƶ�����
		input FPGA_LASER_PULSE,  //�����������β�����
		output reg FPGA_LOGIC_LF398,//LF398�߼���ƽ
	//========ak5394==========
		
		input FPGA_AD_SCLK,  //CPLD_ADSCLK
		input FPGA_AD_LRCK,  //CPLD_ADFRAMESYN
		input FPGA_AD_SDATA, //CPLD_ADDATA
		input FPGA_AD_FSYNC,
		
		output FPGA_AD_MCLK,  //����ʱ��(���ã�����ʱ��ʹ��PLL��ã��˴����ź�����ʱ�������ʱ��)
		output FPGA_AD_ZCAL,  //
		output FPGA_AD_RSTn,  //
		
		output reg ready,  //���LF398����������ɣ������һ��10us�ĸߵ�ƽ���������ж��źŻ��߲�ѯ�źţ���ѯ�źű�д����STATE�Ĵ���
		
		output [31:0]AD_DATA1,
		output [31:0]AD_DATA2,
		output reg[31:0] STATUS, //״̬�Ĵ���
		//write ddr ad singel
		input wire ddr_write_done_arc,
        output wire wr_data_ready,  
    	output wire [95:0]wr_data_buf,
    	output wire [95:0]wr_addr_buf,

    	output wire ddr_ad_data_inte,// posedege signal active
    	output wire  ddr_ad_chk, //choose read DDR address	
		output reg  miss_data_led//��Ӳ���ɼ���ʧ���ݵ�ʱ���0
	//======����ʹ��==========	
);

/**********************************FUNCTION************************************
*����дDDR������,�������޷���1�����򷵻�0,���Ǳ�֤�����ڴ治�����ǣ�����ǰ���
*���������п��ܻᱻ���ǵ������ǲ���Ӱ���������
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
*����дDDR������,�������޷���1�����򷵻�0
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
//Ӳ���ɼ���ʧ���ݼ�����
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
reg [7:0]AD_DATA1_H8_reg,AD_DATA2_H8_reg;//AD������ֻ��24λ����8λû��ʹ���������ָ�����ж��Ƿ�����8λ����Ч��
reg AD_RSTn,AD_ZCAL;
//reg Suspend_AD;//�Ƿ���LF398���������AD����
wire time_base_clk;//ʱ���ʱʱ��
//����תԭ��  24λΪ����λ 0+  1-
wire [23:0]data1_code;
wire [23:0]data2_code;

wire [31:0]data1_int; //int��ʽ�洢
wire [31:0]data2_int;
/****************start 24  to 32 int*************************/
wire [31:0]ddr_data1_int;
wire [31:0]ddr_data2_int;
assign ddr_data1_int[31] = data1[23];
assign ddr_data2_int[31] = data2[23];
assign ddr_data1_int[30:0] = data1[23]?{8'hff,data1[22:0]}:{8'd0,data1[22:0]} ;//����λ��
assign ddr_data2_int[30:0] = data2[23]?{8'hff,data1[22:0]}:{8'd0,data2[22:0]} ;//����λ��
/****************end 24  to 32 int*************************/

assign data1_code[23] = AD_DATA1_reg[23];
assign data2_code[23] = AD_DATA2_reg[23];

assign data1_code[22:0] = AD_DATA1_reg[23]?( AD_DATA1_reg[22:0] ^ 23'h7fffff) + 1'b1:AD_DATA1_reg[22:0];
assign data2_code[22:0] = AD_DATA2_reg[23]?( AD_DATA2_reg[22:0] ^ 23'h7fffff) + 1'b1:AD_DATA2_reg[22:0];

assign data1_int[31] = AD_DATA1_reg[23];//����λ  ��24λ������չΪ32λ��int����
assign data2_int[31] = AD_DATA2_reg[23];

assign data1_int[30:0] = AD_DATA1_reg[23]?{8'hff,AD_DATA1_reg[22:0]}:{8'd0,AD_DATA1_reg[22:0]} ;//����λ��
assign data2_int[30:0] = AD_DATA2_reg[23]?{8'hff,AD_DATA2_reg[22:0]}:{8'd0,AD_DATA2_reg[22:0]} ;//����λ��

//assign AD_DATA1 = (Suspend_AD ?data1_lf398 : data1_int);
//assign AD_DATA2 = (mcu2fpga_open_reuse != 4'd0)? miss_data_cnt : (Suspend_AD ?data2_lf398 : data2_int);
//assign AD_DATA2 = (mcu2fpga_open_reuse == 4'd0)?  (Suspend_AD ?data2_lf398 : data2_int): ((mcu2fpga_open_reuse == 4'd1)?miss_data_cnt : ad_valid_data) ;

assign FPGA_AD_RSTn = AD_RSTn;
assign FPGA_AD_MCLK = (CLK_DIV ==28'd1)?clk: AD_MCLK_reg;
assign FPGA_AD_ZCAL= AD_ZCAL;


//----------------------��λ��2��λ��------------------------
//����λ����2��λ�������ݷ�Ϊ����
reg [3  : 0]M2S_ORDER;  //ָ��
reg [27 : 0]M2S_DATA;  //����

always@(posedge clk) //����M2S����
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
//-------------------------M2Sָ������---------------------
//when clk = 50MHz
//[31:28] = 0:Ĭ�ϲ���
//[31:28] = 1:����LF398�����źŵĸߵ�ƽʱ��Ϊ [27:0]/50 us��������Ϊ10us����[277:0]=10*50=500
//[31:28] = 2:����AD��8λ���ݵ������ԣ�31λΪLF398�������壬30λΪ���������źţ�29�����������ź�
//[31:28] = 3:����ak5394��λ���ŵ�ƽ��[0]��ֵΪ��ƽֵ
//[31:28] = 4:����ak5394��ZCAL���ŵ�ƽ��[0]��ֵΪ��ƽֵ
//[31:28] = 5:����ak5394��MCLK��ʱ�ӣ�[27:0]�Ƕ�50MHzʱ�ӵķ�Ƶ��
//[31:28] = 6:����LF398��ѹ�����ȴ�ADת����ʱ�� Ĭ��12us������Ĭ��waiteTime = 12 * 50 =600
//[31:28] = 7:ģ��������üĴ�������
/*-----------------------------------------------------------------------------------------------
|							7:mcu2fpga_flag_reg ������λΪ28λ ,��־�����б�
-------------------------------------------------------------------------------------------------
|  [27]      |  [26]  |   25        |   24    |   23    |  22    |     21     |    
-------------------------------------------------------------------------------------------------
|ʱ��Ĵ�    |Ӳ���˲�|����reg1Ϊ   |����LF398|����LF398|Ӳ���ж�|��ʧ����    |
|��λ��ѡ    |Ĭ��ֵ  |��������ʱ�� |�߼���ѹ |�����ƽ |�źţ�ͬ|���ͨ��    |
|��          |0       |������       |�ֶ����� |1/0      |��AD����|DDR         |
-------------------------------------------------------------------------------------------------
|0:32λ      |1:����  |1:����       |1:����   |1:H      |1:ͬ��  |1:AD����    |
|1:16λ      |0:�ر�  |0:�ر�       |0:�ر�   |0:L      |0:�첽  |0:PULSER����|
-------------------------------------------------------------------------------------------------*/

//[31:28] = 8:����ak5394��ready�źŸߵ�ƽ����ʱ�䣬[27:0]/50 us   ������Ϊ10us����[27:0]=10*50=500
//[31:28] = 9:����AD�����������������DDR�У�Ĭ��8192����
//[31:28] = 10:���ü�ʱ����ʱ��Ƶ��  ,��AXIʱ�ӷ�Ƶ�õ�
              //Ĭ��Ϊ��Ƭ
//[31:28] = 11:����ADӲ���ɼ����ݵķ�ʽ ��0 ����������  1����ʱ�ɼ�   2�������ɼ�
//[31:28] = 12:����ͨ��2��ad���ݼĴ�������ΪӲ���ɼ����ݶ�ʧ������ ��mcu2fpga_open_reuse ->1:���ض�ʧ����  0:�رո���  other:����Ӳ���ɼ���DDR����Ч������//[31:28] = 13:��λ��֪ͨģ���Ƿ�ɼ����ݣ�Ӳ���ɼ�����1���ɼ�   2��ֹͣ�ɼ�
reg [27:0]pulse_delay_us,CLK_DIV,waiteTime;//ʱ�ӷ�Ƶ��
reg [27:0]TestData;
reg [27:0]testData1;
reg [27:0]readyDelay;
reg [27:0]mcu2fpga_ad_data_num;
reg [3:0]mcu2fpga_getdata_mode;
reg [27:0]mcu2fpga_time_clk_div;
reg mcu2fpga_grab_data;
reg [27:0]mcu2fpga_flag_reg;//��־�Ĵ�����
always@(posedge clk)//����д����  --����ʼ��
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
		end  //Ĭ�ϲ���   10us
		4'd1: begin pulse_delay_us <=  M2S_DATA;end//����������ʱ
		4'd2: begin isOpen_AD_H8 <= M2S_DATA[0];end//����AD��8λ������Ч��
		4'd3: begin AD_RSTn <= M2S_DATA[0];end//��λ�źſ���
		4'd4: begin AD_ZCAL <= M2S_DATA[0];end //FPGA_AD_ZCAL�źſ���
		4'd5: begin CLK_DIV <= M2S_DATA;end //����FPGA_AD_MCLKʱ�ӣ�����ģ�����������ʱ�ӣ�
		4'd6: begin waiteTime <= M2S_DATA;end //����LF398��ѹ�����ȴ�ADת����ʱ�� Ĭ��12us������Ĭ��waiteTime = 12 * 50 =600;
		4'd7: begin mcu2fpga_flag_reg <= M2S_DATA;end //ʹ��LF398�����ֻ��ready�ź���Чʱ����AD����,,Ĭ��Ϊ0���������ݺ����������AD����  mcu2fpga_flag_reg:��־�Ĵ��� 
		4'd8: begin readyDelay <= M2S_DATA[27:0];end
		4'd9: begin mcu2fpga_ad_data_num <= {4'd0,M2S_DATA[27:0]};end//����AD�����������������DDR�У�Ĭ��8192����
		4'd10:begin mcu2fpga_time_clk_div <= M2S_DATA[27:0];end  //���ü�ʱ����ʱ��Ƶ��  ,��AXIʱ�ӷ�Ƶ�õ�
		4'd11:begin mcu2fpga_getdata_mode <= M2S_DATA[3:0];end//����ADӲ���ɼ����ݵķ�ʽ ��0 ����������  1����ʱ�ɼ�   2�������ɼ�  3:�Բ���ģʽ
		4'd12:begin mcu2fpga_open_reuse <= M2S_DATA[3:0];end//����ͨ��2��ad���ݼĴ�������ΪӲ���ɼ����ݶ�ʧ������ ��mcu2fpga_open_reuse ->1:���ض�ʧ����  0:�رո���  other:����Ӳ���ɼ���DDR����Ч������
		4'd13:begin mcu2fpga_grab_data <= M2S_DATA[0];end//��λ��֪ͨģ���Ƿ�ɼ����ݣ�1���ɼ�   0��ֹͣ�ɼ�
		
		4'd14:begin testData1 <= M2S_DATA[27:0];end//��������1
		4'd15:begin TestData <= M2S_DATA[27:0]; end//��������
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
		end  //Ĭ�ϲ���   10us
	endcase
end


reg finish_flag;//���һ������ת����־
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
//-----------------------����������ȡ----------------------------
reg [27:0]delay_cnt;
reg FPGA_LASER_PULSE_f1,FPGA_LASER_PULSE_f2;
wire FPGA_LASER_PULSE_P,FPGA_LASER_PULSE_N;
assign FPGA_LASER_PULSE_P =FPGA_LASER_PULSE_f1 & (~FPGA_LASER_PULSE_f2);//����������������ȡ

always@(posedge clk)
begin
	if(rst_n == 1'b0)begin
		FPGA_LASER_PULSE_f1 <= 1'b0;
		FPGA_LASER_PULSE_f2 <= 1'b0;
	end 
	else begin
		FPGA_LASER_PULSE_f1 <= FPGA_LASER_PULSE;  //���κ�ʱ��ͬ��
		FPGA_LASER_PULSE_f2 <= FPGA_LASER_PULSE_f1;	
	end
end

always@(posedge clk)	
begin
    if(rst_n ==1'b0)begin
        FPGA_LOGIC_LF398 <= 1'b0;
    end
	else if(mcu2fpga_flag_reg[24])begin 
		FPGA_LOGIC_LF398 <= mcu2fpga_flag_reg[23];//�ֶ�����LF398���߼���ѹ
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
wire FPGA_LOGIC_LF398_N;//����½���
assign FPGA_LOGIC_LF398_N = (~FPGA_LOGIC_LF398_delay1) & (FPGA_LOGIC_LF398_delay2); //negedge
wire [27:0]waitTime1;
assign waitTime1 = waiteTime + readyDelay;
//��ȡ�ź�,��LF398����ʱ����ready�ź�
always@(posedge clk)
begin
	FPGA_LOGIC_LF398_delay1 <= FPGA_LOGIC_LF398;  //����������ȡ�ź�
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

//��������
always@(posedge ready)
begin
	data1_lf398 <= data1_int;
	data2_lf398 <= data2_int;	
end
//--------------------ʱ�ӷ�Ƶ-���FPGA_AD_MCLK�ͼ�ʱ��ʱ��-------------------------
//��1.1�汾�������Ƶ�������Ӳ���ɼ����ݵ��Բ⣬����AK5394��ʱ��ʹ��PLL
//����!
wire clk_base;
reg [27:0]AD_MCLK_CNT;
reg AD_MCLK_reg;
reg time_base_clk_reg;
reg [27:0]time_base_clk_cnt;
assign time_base_clk = (mcu2fpga_time_clk_div == 28'd1)? clk : time_base_clk_reg;//��ȡ��ʱ����ʱ��
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
				
			//������
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
				
			//������
			if(time_base_clk_cnt >= mcu2fpga_time_clk_div - 1'b1)
				time_base_clk_cnt <= 28'd0;
			else
				time_base_clk_cnt <= time_base_clk_cnt + 1'b1;
		end
	end
end
//---------------------------����DDRд����Դ------------------
reg [1:0]getdata_cnt;
reg cache_flag;
reg [31:0]data_reg;
reg finish_flag_ff1;
reg finish_flag_ff2;
wire tran_data_done_flag ;
assign tran_data_done_flag = (~finish_flag_ff1) & finish_flag_ff2;//AK5394�������ת���������������浽�Ĵ�����
//ģʽ1�����źţ������ɼ�����
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
//���������ʱ���������浽�������ݼĴ���
reg [127:0] data_reg_buf;//���ݻ������
reg [31:0]data_temp;
always@(negedge clk)
begin
     if(rst_n == 1'b0)begin
         data_reg <= 32'd0; 
		 data_reg_buf <= 32'd0;
		 data_temp <= 32'd0;
    end
    else begin  	
		//data_reg <= ddr_data1_int; //��������
		if(data_latch)begin
			data_temp[31] <= data1[23];
			data_temp[30:0] <=data1[23]?{8'hff,data1[22:0]}:{8'd0,data1[22:0]} ;//����λ��
			data_reg_buf <= {data_reg_buf[95:0],data_temp} ;//�����ȥ��3�����ݣ����ڽ����ǰ���ݳ��������,��ǰ���ݳ����ͽ���ȡ��һ�ε����ݴ���
			data_reg <= (mcu2fpga_flag_reg[26] == 1'b0)? data_reg_buf[31:0] ://ֱ�ӷ����ݣ�����ΪӲ���˲�
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
//ģʽ3�����źţ��ⲿ�����ɼ�����
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
////ģʽ2�����źţ���ʱ�ɼ������ź�
wire pl_timer;//ģ�鶨ʱ�����������ö�ʱ�ɼ����ڲ���ģʽ�£���ʱ��ģ�⼤������
assign pl_timer = FPGA_AD_MCLK;//ʹ��ԭ������ȷ��AK5394��ʱ�ӣ���������ak5394��ʱ��ʹ��PLL����
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
//ģʽ4�Բⴥ���źţ����ɲ������ݺ�DDR�����ź�
reg testSelf_trigger;
reg [31:0]testSelf_data;
reg [27:0]testSelf_cnt;
reg [27:0]testNum_cnt;
reg testDir ;
reg testDir_ff1;
wire testDirNP;
assign testDirNP = (testDir & (~testDir_ff1)) | ((~testDir) & testDir_ff1);//��ȡģ���Բ�������
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
//���������źŽ�ȡ������
reg move_dir_ff1;
reg move_dir_ff2;
wire move_dir_p;//��ȡ�������������� ,���źű������ڵ�һ�������źŵ�ʱ��Ч
wire move_dir_pn;//���������ź�
assign move_dir_p = move_dir_ff1 & (~move_dir_ff2);
assign move_dir_pn = (move_dir_ff1 & (~move_dir_ff2)) | ((~move_dir_ff1) & move_dir_ff2);
reg move_dir;
reg [1:0]dir_change_state;
reg tx_clear_flag;
//assign move_dir = move_dir_ff1;//�Զ��������źŽ���ͬ������
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
///��¼ÿ�����ʱ��,�ڶ��������ʱ�������������
//�������¼���
reg [31:0]ad_time_cnt;
wire time_clk;
assign time_clk = time_base_clk; //ʱ�������ʱ�ӣ�ʱ����λ������Ϊ10MHz��
wire time_cnt_clear ;
//����N����,�����źŷ�������ʱ������һ���ߵ�ƽ�ź�,���ڴ�ʱ�����������ֵ
assign time_cnt_clear = (mcu2fpga_getdata_mode == 4'd3)? ((testDirNP) & rst_n) :((move_dir_pn) & rst_n);  //testDirNP
reg send_1st_data_flag;//���͵�һ�����ݱ�־
reg send_1st_data_arc;//���ݷ��ͺ������źţ���������͵�һ�����ݵı�־��Ч��
//�ڲ�ʱ���ʱ��
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
//ģ���źŽ�������֪����׼��ģ�鸴λ��ַ����⵽����׼��ģ����ɺ�λ֪ͨ�ź�
reg send_1st_data_flag_ff1;
wire send_1st_data_flag_p ;
assign send_1st_data_flag_p = send_1st_data_flag &(~send_1st_data_flag_ff1);
always@(posedge clk or posedge time_cnt_clear)
begin
	if( time_cnt_clear == 1'b1 )begin
		send_1st_data_flag <= rst_n ? 1'b1 : 1'b0 ;//֪ͨ����׼��ģ�鷢�͵�һ�����ݣ�
		//send_1st_data_flag_ff1 <= send_1st_data_flag;
	end
	else begin//��÷��͵�һ�����ݷ����󣬽�֪ͨ�ź����͸�λ
		send_1st_data_flag <= send_1st_data_arc ? 1'b0 : send_1st_data_flag ;
		//send_1st_data_flag_ff1 <= send_1st_data_flag;
	end
end
always@(posedge clk)//��ʱһ��clk
	send_1st_data_flag_ff1 <= send_1st_data_flag;
//---------------------------��¼���������ʱ��,������д��DDR��----------------------------------------
//step1:ͬ����������  ǰ���Ѿ��õ� ���������ز����ߵ�ƽ FPGA_LASER_PULSE_P
//step3:�������͵�һ����������,���ȴ���һ��������������ָ�״̬
wire laser_p;//��������������
//����ģʽ��ʹ��FPGA_AD_MCLK(pl_timer)��Ϊģ�⼤������Դ
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

//step5:��DDRд����
reg [31:0] pluser_time_valid_data ;//һ�������������Ч��

////��AXI�����������źţ������͸�AXI���ߵ����ݡ�
reg [31:0]send_axi_data;//���͸�AXI���ߵ�ad���ݡ�
reg [31:0]send_axi_time;//���͸�AXI���ߵ�ʱ��t���ݡ�
reg tx_flag;//����data��Ч��־,�������ݴ���
//reg [31 : 0]same_dir_data_cnt;//��¼ͬһ�����ad���ݸ���
reg send_data_rst;
reg [31:0]ad_data_cnt ;
reg [31:0]laser_pulse_time_cnt;

//��������ok�жϣ�����ARMˢ�¶�Ӧ�Ļ��棬�������ݽ��ж�ȡ
wire [31:0]DDR_TIME_BASE_ADDR;//�洢ʱ���DDR����ַ
wire [31:0] DDR_PULSE_TIME_BASE_ADDR;//��̬����DDR������������д��Ļ���ַ
wire TIME_CNT_BIT;//ʱ���ʱ��λ��ѡ�� 0:32  1:16
assign TIME_CNT_BIT = mcu2fpga_flag_reg[27];
assign DDR_TIME_BASE_ADDR = DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 3);//����дʱ��Ļ���ַ
assign ddr_ad_chk = ddr_chk ; //�ж��ź�
assign DDR_PULSE_TIME_BASE_ADDR = DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 3);

reg [95:0]send_axi_data_buf;// [95:64]ad data  [63:32]ad time data  [31:0]laser time data
reg [95:0]send_axi_addr_buf;//��Ӧaxi���ݵ�д���ַ

//-----------------------------����fifo����������ٶ�ƥ������-------------------------------------------------------
//��������ĺͻ�ȡAD������֮������ٶȲ�һ�µ����⣬�������ݲ���ͬһʱ�̸��£�ad���ݻ�ȡ���ٶ�Ҫ�ȼ����������ݸ��µ�Ƶ�ʿ�
//��˵��������嵽��ʱ������ʱ������д�����Ϊ16��fifo�У���ad���ݸ���ʱ����ѯfifo����fifo�ǿգ��ʹ�fifo��ȡ��һ�����ݣ�����
//ad����һ��д��DDR�У������Ĳ�����֤��ad��������һ���Ǵ��ڵ��ڼ����������������
reg wr_en;
reg rd_en;

reg fifo_rst_n;//FIFO��λ�ź�
reg [31:0]fifo_addr;//��һ��fifo��д������
reg [31:0]fifo_data;//�ڶ���fifo��д������
wire[31:0]fifo_data_o;//������fifo����
wire[31:0]fifo_addr_o;
wire fifo_full1;
wire fifo_full2;
wire fifo_full;
wire fifo_empty1;//fifo�жϿ���
wire fifo_empty2;
wire fifo_empty;
assign fifo_empty = fifo_empty1 & fifo_empty2;//����fifo���źźϲ�������fifo��ͬ���ģ�ÿһʱ������ȡ����������һ���ģ�д��Ͷ�������ͬ���ģ�
assign fifo_full = fifo_full1 & fifo_full2;
fifo_hpy #(
	.BUF_WIDTH(4),//��дָ������λ��
	.BUF_SIZE(16),//fifo�����
	.DATA_WIDTH(32)//fifo�ж�д���ݵ�λ��
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
//����fifoʹ��д����һ��clk���Ա�֤fifo��λ,�������
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
//----------------------------------��������������д��fifo��---------------------------------------
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
//-------------------------------------�����ݺϳ�����AXIд�뵽DDR��---------------------------------------------------
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
  //��������ʱ���
/*
			if(send_1st_data_flag_p)begin//�����������ź�
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
 //ADʱ���AD����
			case(mcu2fpga_getdata_mode)
				4'd0:begin  //�����ɼ�,��������  
						if(tran_data_done_flag)begin
							send_axi_data <=  data_reg ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							//send_axi_data_buf[95:32] <= {data_reg , ad_time_cnt};//��������
							//                                                             16λ������                            32λλ��
							send_axi_data_buf[95:32] <= mcu2fpga_flag_reg[27] ?  {data_reg , {16'd0,ad_time_cnt[15:0]}} : {data_reg , ad_time_cnt};
							if(FPGA_DongJ_DIR)begin //���͵�ַ
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//������Ч����
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
									ad_valid_data <= ad_data_cnt ;//������Ч����
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2: send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end
					///////////////////////////////��FIFO����//////////////////////////////////////////
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
				/////////////////////////////////FIFO��ȡ���////////////////////////////////////////////
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
				4'd1:begin  //��ʱ�ɼ�
						if(timer_trigger)begin
							send_axi_data <=  data_reg ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							send_axi_data_buf[95:32] <= mcu2fpga_flag_reg[27] ? {data_reg ,{16'd0,ad_time_cnt[15:0]} } : {data_reg , ad_time_cnt};//��������
							if(FPGA_DongJ_DIR)begin //���͵�ַ
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//������Ч����
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
									ad_valid_data <= ad_data_cnt ;//������Ч����
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end	
					///////////////////////////////��FIFO����//////////////////////////////////////////
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
				/////////////////////////////////FIFO��ȡ���////////////////////////////////////////////
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
				4'd2:begin //�����ɼ�
						if(ready_one_trigger)begin
							send_axi_data <=  data_reg ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;	
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							send_axi_data_buf[95:32] <= mcu2fpga_flag_reg[27] ? {data_reg , { 16'd0, ad_time_cnt[16:0]}} : {data_reg , ad_time_cnt};//��������
							if(FPGA_DongJ_DIR)begin //���͵�ַ
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//������Ч����
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd4 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
									ad_valid_data <= ad_data_cnt ;//������Ч����
								end
							end
							else begin
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR + (mcu2fpga_ad_data_num << 2);
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//������Ч����
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2: send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2: send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= 32'h3fff0000;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end	
					///////////////////////////////��FIFO����//////////////////////////////////////////
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
				/////////////////////////////////FIFO��ȡ���////////////////////////////////////////////							
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
							send_axi_addr_buf[31:0] <= send_axi_addr_buf[31:0];//��������ʱ������
							send_axi_data_buf[31:0] <= send_axi_data_buf[31:0];
							ad_data_cnt <= ad_data_cnt ;
							rd_en <= 1'b0;
							laser_pulse_time_cnt <= laser_pulse_time_cnt;
						end
						move_dir <= FPGA_DongJ_DIR;
					end
				4'd3:begin //ģ���Բ�
						if(testSelf_trigger)begin
							send_axi_data <=  send_1st_data_flag? {4'd0,TestData}  :send_axi_data + 1'b1 ;
							tx_flag <= 1'b1 ;
							send_axi_time <= ad_time_cnt;//send_1st_data_flag? 32'd0  :send_axi_time + 1'b1 ;
							send_data_rst <= send_1st_data_flag ? 1'b1:1'b0;
							///////////////////////////////////
							if(mcu2fpga_flag_reg[27] == 1'b0)send_axi_data_buf[95:32] <= send_1st_data_flag?{32'd0 , ad_time_cnt} : {{send_axi_data_buf[95:64] + 1'b1} ,ad_time_cnt};//��������
							else send_axi_data_buf[95:32] <= send_1st_data_flag ? {32'd0 , {16'd0,ad_time_cnt[15:0]} } : {{ 16'd0,{send_axi_data_buf[79:64] + 1'b1} } ,{16'd0,ad_time_cnt[15:0]}};//��������
							//send_axi_data_buf[31:0] <= send_1st_data_flag?32'd0: send_axi_data_buf[31:0]  + 1'b1;
							if(testDir)begin //���͵�ַ
								if(send_1st_data_flag)begin
									send_axi_addr_buf[95:64] <= DDR_BASE_ADDR;
									send_axi_addr_buf[63:32] <= DDR_TIME_BASE_ADDR;

									//send_axi_addr_buf[31:0 ] <= DDR_PULSE_TIME_BASE_ADDR;
									ad_data_cnt <= 32'd0;
									ad_valid_data <= ad_data_cnt ;//������Ч����
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
									ad_valid_data <= ad_data_cnt ;//������Ч����
									pluser_time_valid_data <= laser_pulse_time_cnt;
								end
								else begin
									send_axi_addr_buf[95:64] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[95:64] + 32'd2 : send_axi_addr_buf[95:64] + 32'd4;
									send_axi_addr_buf[63:32] <= mcu2fpga_flag_reg[27] ? send_axi_addr_buf[63:32] + 32'd2 : send_axi_addr_buf[63:32] + 32'd4;
									//send_axi_addr_buf[31:0 ] <= send_axi_addr_buf[31:0 ] + 32'd4;
									ad_data_cnt <= ad_data_cnt + 1'b1;
								end							
							end	
					///////////////////////////////��FIFO����//////////////////////////////////////////
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
				/////////////////////////////////FIFO��ȡ���////////////////////////////////////////////
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
            endcase//end �ɼ�ģʽ	
			send_1st_data_arc <= send_data_rst ? 1'b1 : 1'b0;
    end  
end

//Generate a pulse to initiate send data to ddr.


//--------------------------------------���DDR����д��---------------------------------
reg [63:0] wr_data_reg;  //[ad���ݣ�ʱ������]
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
reg [31:0]ad_valid_data;//����ͬһ�����������������ֻ���ڶ�������ı��ʱ��������ݡ�
reg [31:0]time_dst_r;//ʱ��洢��ַƫ��

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
						if(move_dir)begin//����������һ�������˶�
							if(send_data_rst)begin
								ddr_chk <= 1'b1;
								inte1 <= 1'b1;
								//ad_valid_data <= ad_data_cnt ;//������Ч����
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
								//ad_valid_data <= ad_data_cnt;//������Ч����
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





//��ȡ�ж��ź�
//˵�����˴���ȡ�ж��ź������أ���⵽�����غ󣬽��ж��źŸߵ�ƽ����һ��clock��ʱ��
//�����źŽ���axi GPIO,ʹ��axi gpio�жϴ���ARM����Ϊaxi gpio���ź������غ��½��ض���
//����һ���ߵ�ƽ��Ϊ����armֻ��һ�����ݸ����б�����һ�Σ�ͨ����ģ�����һ���ߵ�ƽ��
//��ʱ��Ϊһ��clock���жϣ���axi gpio��⵽�����غ󣬽����ڸ��ź������غ��½��ض�����
//һ��clock�ĸߵ�ƽ�����axi gpio���յ�����ж��źź������ߵ�ƽ����ʱ��Ϊ2��clock��
//�ж��źţ�ARM���յ��ж��źźͲ������ж��źţ������ͺ�����Ϊ1��ʱ�����ڡ�
//assign ddr_ad_data_inte = inte1 &(~inte2) ;//get posedge signal
                   ////////////////////////////////ͬ��///////////////�첽/////////////
assign ddr_ad_data_inte =inte1 &(~inte2);//get posedge signal
//------------------------���״̬����PS��ȡIP��״̬����-----------------------------
//�����źŵ�״̬���
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
//��ʧ���ݼ�����
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
//AXI ����
assign AD_DATA1 = mcu2fpga_flag_reg[25] ? pluser_time_valid_data : data_reg ;
//assign AD_DATA2 = (mcu2fpga_open_reuse != 4'd0)? miss_data_cnt : (Suspend_AD ?data2_lf398 : data2_int);
assign AD_DATA2 = (mcu2fpga_open_reuse == 4'd0)?  (data2_int): ((mcu2fpga_open_reuse == 4'd1)?miss_data_cnt : ad_valid_data) ;


//�����AXI�ĵ�ַ�����ݣ��Լ���������
assign wr_data_buf = wr_data_buf_reg;
assign wr_addr_buf = wr_addr_buf_reg;
assign wr_data_ready = wr_data_ready_reg;
//����ȷ���ڶ����������ݵĵ�ַ��һֱ��ad���ݴ洢��ַ�����ͬ��ַ����֤��һ��ad���ݣ��ڶ�Ӧ��λ�ô洢ʱ��
//assign addr_offset = (mcu2fpga_ad_data_num << 3);


//--------------------------------------------------------------------------------------
endmodule 
