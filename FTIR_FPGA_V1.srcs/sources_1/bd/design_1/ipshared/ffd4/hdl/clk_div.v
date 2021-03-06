`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/11/06 14:58:14
// Design Name: 
// Module Name: clk_div
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 时钟分频模块
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module clk_div#(
	parameter DIV_WIDTH = 32//数据位宽
)

(
    input			clk_in		,
    input			rst_n		,
	input [31:0]	div			,
    output			clk_ot		
    );

	reg [31:0]cnt;
	reg clk_ot_r;
	assign clk_ot = (div==32'd1)?clk_in:
					(div==32'd0)?1'b1:clk_ot_r;									
	always@(posedge clk_in)
	begin
		if(rst_n == 1'b0)begin
			cnt <= 'd0;
			clk_ot_r <= 1'b0;
		end
		else begin
			if(cnt >= div - 1'b1)cnt <= 'd0;
			else cnt <= cnt + 1'b1;

			if(cnt <= (div>>1) - 1'b1)clk_ot_r <= 1'b1;
			else clk_ot_r <= 1'b0;
		end
	end
	
endmodule
