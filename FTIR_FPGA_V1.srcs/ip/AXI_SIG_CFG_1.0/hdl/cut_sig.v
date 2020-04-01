`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/10/21 22:40:35
// Design Name: 
// Module Name: cut_sig
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


module cut_sig(
	input wire clk,
	input wire rst_n,
	input wire sig_in,
	input wire [7:0]sig_cfg,
	input wire [15:0] keep_time,
	output wire sig_tmp,
	output wire sig_out
    );


	reg laser_f1 , laser_f2 ,laser_f3;
	wire laser_p , laser_n ,laser_np;
	wire laser_sig;
	assign laser_p = laser_f2 & (~laser_f3);
	assign laser_n = laser_f3 & (~laser_f2);
	assign laser_np = laser_p | laser_n;
	assign laser_sig = (sig_cfg[7]==1'b1)?laser_np:
						(sig_cfg[6]==1'b1)?laser_p :	
											laser_n ;

	always@(posedge clk)
	begin
		if(!rst_n)begin
			laser_f1 <= 1'b0;
			laser_f2 <= 1'b0;
			laser_f3 <= 1'b0;
		end
		else begin
			laser_f1 <= sig_in;
			laser_f2 <= laser_f1;	
			laser_f3 <= laser_f2;
		end
	end
	reg laser_bf_r;
	reg [15:0]cnt1;
	reg [2:0]st;
	always@(posedge clk)
	begin
		if(!rst_n)begin
			laser_bf_r <= 1'b0;
			st <= 3'b001;
		end
		else begin
			case(st)
				3'b001:begin
					st <= laser_sig?3'b010 : 3'b001;
					laser_bf_r <= laser_sig? 1'b1 : 1'b0;
					cnt1 <= 16'd0;
				end
				3'b010:begin
					if(cnt1 >= keep_time) begin
						st <= 3'b001;
						cnt1 <= 16'd0;
						laser_bf_r <= 1'b0;
					end
					else begin
						st <= 3'b010;
						cnt1 <= cnt1 + 1'b1;
						laser_bf_r <= 1'b1;					
					end
				end
				default: begin
					st<= 3'b001;
					cnt1 <= 16'd0;
					laser_bf_r <= 1'b0;	
				end
			endcase 
		end
	end


	assign sig_out = laser_bf_r;
	assign sig_tmp = laser_sig;
endmodule
