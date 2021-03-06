
module fifo_hpy #
(
	parameter integer BUF_WIDTH = 4,//寻址地址宽度
	parameter integer BUF_SIZE = 16,//数据个数
	parameter integer DATA_WIDTH = 32//数据位宽
)
( 
	input wire clk,
	input wire rst_n,
	input wire [DATA_WIDTH - 1:0]buf_in,
	output reg [DATA_WIDTH - 1:0]buf_out,
	input wire wr_en,
	input wire rd_en,
	output wire buf_empty,
	output wire buf_full,
	output reg [31 :0]fifo_cnt
);
	
	reg [BUF_WIDTH- 1:0] rd_ptr,wr_ptr;  //这个很重要，数据指针3位宽度，0-7索引，8个数据深度，循环指针0-7-0-7
	reg [DATA_WIDTH - 1:0] buf_mem[0:BUF_SIZE-1];
	
	//判断空满
	assign buf_empty = (fifo_cnt == 0);  //
	assign buf_full  = (fifo_cnt == BUF_SIZE);
	
	always @(posedge clk or negedge rst_n)begin
		if(!rst_n)
			fifo_cnt <= 0;
		else if((!buf_full&&wr_en)&&(!buf_empty&&rd_en)) //同时读写，数量不变
			fifo_cnt <= fifo_cnt;
		else if(!buf_full && wr_en)          //写数据
			fifo_cnt <= fifo_cnt + 1'b1;
		else if(!buf_empty && rd_en)         //读数据
			fifo_cnt <= fifo_cnt-1'b1;
		else 
			fifo_cnt <= fifo_cnt;
	end
	
	always @(posedge clk or negedge rst_n) begin   //读数据
		if(!rst_n)
			buf_out <= 0;
		else 
		if(rd_en && !buf_empty)
			buf_out <= buf_mem[rd_ptr];
	end
	
	always @(posedge clk) begin
		if(wr_en && !buf_full)
			buf_mem[wr_ptr] <= buf_in;
	end
	
	always @(posedge clk or negedge rst_n) begin
		if(!rst_n) begin
			wr_ptr <= 0;
			rd_ptr <= 0;
		end
		else begin
			if(!buf_full && wr_en)
				wr_ptr <= wr_ptr + 1'b1;
			if(!buf_empty && rd_en)
				rd_ptr <= rd_ptr + 1'b1;
		end
	end
	
endmodule 
