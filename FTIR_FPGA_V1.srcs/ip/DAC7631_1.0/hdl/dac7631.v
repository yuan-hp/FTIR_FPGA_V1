`timescale 1ns / 1ps
/*                 DAC7631����ģ��
---------------------------------------------------------------------------
    �汾  |         ˵��                             |�޸���
--------------------------------------------------------------------------
   v 1.0  |   ���ɵ�spiʱ��������СΪ30ns����Ϊ30MHz | hpy
-------------------------------------------------------------------------*/

module dac7631
(
	input wire clk,
	input wire rst_n,
	input wire [15:0]axi_data,
	input wire tx_start,
	input wire dac_select,
	input wire [31:0]cfg_spi_clk,//����spiʱ��
	input wire [31:0]c_reg,  //ģ����ƼĴ���
	
	output  wire dac_clk,
	output  wire dac_cs,
	output  wire dac_sdo,
	output  wire dac_rst,
	output  wire dac_ldac_a,
	output  wire dac_ldac_b,
	output  wire dac_load_a,	
	output  wire dac_load_b
);
	localparam DAC_IDEL        =    6'b000001;
	localparam DAC_LOAD_DATA   =    6'b000010;
	localparam DAC_SEND_DATA   =    6'b000100;
	localparam DAC_UPDATE_DATA =    6'b001000;
	localparam DAC_WRITE_INPUT =    6'b010000;
	localparam DAC_UPDATE_DAC  =    6'b100000;
	
	localparam DAC_DATA_LEN = 5'd16;//���ݳ���
//wire 
wire tx_start_p;//�������ݴ�������
wire b_clk;//spiʱ��base
	

//reg
reg tx_start_ff1;
reg tx_start_ff2;
reg b_clk_r;
reg [31:0]clk_cnt;
reg spi_flag , spi_flag_arc;
reg dac_clk_r   ;
reg dac_cs_r    ;
reg dac_sdo_r   ;
reg dac_rst_r   ;
reg dac_ldac_a_r;
reg dac_ldac_b_r;
reg dac_load_a_r;
reg dac_load_b_r;

//assign
assign b_clk = (cfg_spi_clk == 32'd1) ? clk : b_clk_r ; 
assign tx_start_p = tx_start_ff1 & (~tx_start_ff2);

	always@(posedge clk)
	begin
		if(!rst_n)begin
			tx_start_ff1 <= 1'b0;
			tx_start_ff2 <= 1'b0;
		end
		else begin
			tx_start_ff1 <= tx_start;
			tx_start_ff2 <= tx_start_ff1 ;		
		end
	end

	always@(posedge clk)
		if(!rst_n)dac_rst_r <= 1'b0;
		else dac_rst_r <= 1'b1;

	//��֪spi����ģ�鿪ʼ��������
	always@(posedge clk)
	begin
		if(!rst_n)begin
			spi_flag <= 1'b0;	
		end
		else begin
			if(tx_start_p)spi_flag <= 1'b1;
			else spi_flag <= spi_flag_arc ? 1'b0 : spi_flag;
		end
	end
	

	reg [5:0]state;
	reg [15:0]data;
	reg select;
	reg [4:0]cnt;
	always@(posedge b_clk)
	begin
		if(rst_n == 1'b0)begin
			spi_flag_arc <= 1'b0;
			state <= DAC_IDEL;
			dac_clk_r <=1'b1;
			dac_cs_r <= 1'b1;
			dac_sdo_r <= 1'b0;
			dac_ldac_a_r <= 1'b0;
			dac_ldac_b_r <= 1'b0;
			dac_load_a_r <= 1'b0;
			dac_load_b_r <= 1'b0;
			cnt <= 5'd0;
			select <= 1'b0;
		end
		else begin
			case(state)
				DAC_IDEL:begin  //�ȴ�״̬
						if(spi_flag == 1'b1)begin
							data <= axi_data;
							select <= dac_select;
							spi_flag_arc <= 1'b1;
							dac_clk_r <= 1'b1; 
							dac_ldac_a_r <= 1'b0;
							dac_ldac_b_r <= 1'b0;
							state <= DAC_LOAD_DATA;
						end
						else begin
							state <= DAC_IDEL;
							dac_ldac_a_r <= 1'b0;
							dac_ldac_b_r <= 1'b0;
							spi_flag_arc <= 1'b0;
						end
						cnt <= 5'd0;
					end
				DAC_LOAD_DATA:begin //��ȡ����
						spi_flag_arc <=1'b0;
						dac_clk_r <= 1'b0;
						dac_cs_r <= 1'b0;
						dac_sdo_r <= data[15];
						data <= {data[14:0],1'b0};
						dac_load_a_r <= 1'b1;
						dac_load_b_r <= 1'b1;
						dac_ldac_a_r <= 1'b0;
						dac_ldac_b_r <= 1'b0;
						cnt <= cnt + 1'b1;
						state <= DAC_SEND_DATA;
					end
				DAC_SEND_DATA:begin //��������
						dac_clk_r <= 1'b1;
						state <= (cnt>= DAC_DATA_LEN) ? DAC_UPDATE_DATA : DAC_LOAD_DATA ;
					end
				DAC_UPDATE_DATA:begin //��ʼ��������
						dac_cs_r <= dac_clk_r ? 1'b1 : dac_cs_r;
						state <= dac_clk_r ? DAC_WRITE_INPUT : DAC_UPDATE_DATA;
						cnt <=5'd0;
					end
				DAC_WRITE_INPUT:begin
						dac_load_a_r <= 1'b0 ;
						dac_load_b_r <= 1'b0 ;						
						state <=( cnt >=5'd1)?DAC_UPDATE_DAC : DAC_WRITE_INPUT ;
						cnt <= (cnt>=5'd1)? cnt<=5'd0:cnt + 1'b1;
					end
				DAC_UPDATE_DAC:begin
						dac_load_a_r <= 1'b1;
						dac_load_b_r <= 1'b1;
						dac_ldac_a_r <= select ? 1'b0 : 1'b1;
						dac_ldac_b_r <= select ? 1'b1 : 1'b0;
						cnt <= cnt + 1'b1;
						state <= (cnt>=5'd3) ? DAC_IDEL : DAC_UPDATE_DAC;
					end
				default : state <= DAC_IDEL;
			endcase
		end
	end
	
	//generate spi base clk
	always@(posedge clk)
	begin
		if(rst_n == 1'b0)begin
			b_clk_r <= 1'b0;
			clk_cnt <= 'd0;
		end
		else if(cfg_spi_clk==32'd1) b_clk_r <= 1'b1;
		else begin
			//�б����
			if(clk_cnt <= (cfg_spi_clk >> 1) - 1'b1) b_clk_r <= 1'b1;
			else b_clk_r <= 1'b0;
			//cnt
			if(clk_cnt >= cfg_spi_clk - 1'b1)clk_cnt<= 32'd0;
			else clk_cnt <= clk_cnt + 1'b1;
		end
	end	
/*-----------------���ƼĴ��������б�--------------------------------
                             c_reg
---------------------------------------------------------------------
λ��  |  [31]  |
---------------------------------------------------------------------
      |����ģʽ|
����  |1:����  |
	  |0:�ر�  |
---------------------------------------------------------------------
Ĭ��ֵ|   0    |
--------------------------------------------------------------------*/

assign dac_clk    = (c_reg[31]==1'b0)? dac_clk_r    : b_clk;
assign dac_cs  	  = (c_reg[31]==1'b0)? dac_cs_r     : 1'b1; 
assign dac_sdo 	  = (c_reg[31]==1'b0)? dac_sdo_r    : b_clk; 
assign dac_rst    = (c_reg[31]==1'b0)? dac_rst_r    : 1'b1; 
assign dac_ldac_a = (c_reg[31]==1'b0)? (~dac_ldac_a_r) : b_clk;
assign dac_ldac_b = (c_reg[31]==1'b0)? (~dac_ldac_b_r) : b_clk;
assign dac_load_a = (c_reg[31]==1'b0)? dac_load_a_r : b_clk;
assign dac_load_b = (c_reg[31]==1'b0)? dac_load_b_r : b_clk;



endmodule 