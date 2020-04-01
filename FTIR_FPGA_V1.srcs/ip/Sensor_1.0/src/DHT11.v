/**
*DHT11 verilog驱动
*Author:hpy
*version:v1.0
*/
module DHT11(
	input wire       	clk			,  //1MHz时钟
	input wire 		  	start     ,//上升沿触发采集
	input wire	     	rst_n		,
	inout	          	dat_io		,
	output  reg [39:0]	data     ,
	output  			error					,//数据度错误时为1
	output  			done//完成一次转换后数据更新
);
	wire din;//读取的数据
	reg read_flag;
	reg dout;
	reg[3:0] state;
	localparam s1 = 0;
	localparam s2 = 1;
	localparam s3 = 2;
	localparam s4 = 3;
	localparam s5 = 4;
	localparam s6 = 5;
	localparam s7 = 6;
	localparam s8 = 7;
	localparam s9 = 8;
	localparam s10 = 9;
	
	assign dat_io = read_flag ? 1'bz : dout;
	assign din = dat_io;
	assign done = (state == s10)?1'b1:1'b0;
	assign error = (data[7:0] == data[15:8] + data[23:16] + data[31:24] + data[39:32])?1'b0:1'b1;
	reg [5:0]data_cnt;
	reg start_f1,start_f2,start_rising;
	always@(posedge clk)
	begin
		if(!rst_n)begin
			start_f1 <=1'b0;
			start_f2 <= 1'b0;
			start_rising<= 1'b0;
		end
		else begin
			start_f1 <= start;
			start_f2 <= start_f1;
			start_rising <= start_f1 & (~start_f2);
		end
	end
	reg [39:0] data_buf;
	reg [15:0]cnt ;
	always@(posedge clk or negedge rst_n)
	begin
		if(rst_n == 1'b0)begin
			read_flag <= 1'b1;
			state <= s1;
			dout <= 1'b1;
			data_buf <= 40'd0;
			cnt <= 16'd0;
			data_cnt <= 6'd0;
			data<=40'd0;
		end
		else begin
			case(state)
				s1:begin//当数据总线空闲时，收到数据采集时开启采集
						if(start_rising && din==1'b1)begin
							state <= s2;
							read_flag <= 1'b0;//主机获取总线
							dout <= 1'b0;//拉低
							cnt <= 16'd0;
							data_cnt <= 6'd0;
						end
						else begin
							read_flag <= 1'b1;
							dout<=1'b1;
							cnt<=16'd0;
						end	
					end
				s2:begin//主机输出低电平延时19ms,结束后主机发出高电平
						if(cnt >= 16'd19000)begin
							state <= s3;
							dout <= 1'b1;
							cnt <= 16'd0;
						end
						else begin
							cnt<= cnt + 1'b1;
						end
					end
				s3:begin//主机延时20-40us,结束后释放数据总线,准备读取数据
						if(cnt>=16'd20)begin
							cnt<=16'd0;
							read_flag <= 1'b1;
							state <= s4;
						end
						else begin
							cnt <= cnt + 1'b1;
						end
					end
				s4:begin//等待从机响应
						if(din == 1'b0)begin//从机响应
							state<= s5;
							cnt <= 16'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin//超时自恢复
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end	
						end
					end
				s5:begin//检查从机是否回应
						if(din==1'b1)begin
							state <= s6;
							cnt<=16'd0;
							data_cnt <= 6'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin//超时自恢复
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end								
						end
					end
				s6:begin//等待第一个数据的起始信号点
						if(din == 1'b0)begin//数据bit开始接收
							state <= s7;
							cnt <= cnt + 1'b1;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin//超时自恢复
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end							
						end
					end
				s7:begin//
						if(din == 1'b1)begin//决定数据的高电平起始点
							state <= s8;
							cnt <= 16'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin//超时自恢复
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end							
						end
					end
				s8:begin//检测高电平的时间,并判断数据的 0 1
						if(din == 1'b0)begin
							data_cnt <= data_cnt + 1'b1;
							state <= (data_cnt >= 6'd39)?s9:s7;//40bit数据接收完进入s9，否则进入s7继续接收下一bit
							cnt<=16'd0;
							if(cnt >= 16'd60)begin
								data_buf<={data_buf[39:0],1'b1};
							end
							else begin
								data_buf<={data_buf[39:0],1'b0};
							end
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin//超时自恢复
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end	
						end
					end
				s9:begin//锁存数据，并等待从机释放总线
						//data <= (data_buf[7:0] == (data_buf[15:8] + data_buf[23:16] + data_buf[31:24] + data_buf[39:32]))?data_buf : data;
						data <= data_buf;
						if(din == 1'b1)begin
							state <= s10;
							cnt<=16'd0;
						end
						else begin
							cnt <= cnt + 1'b1;
							if(cnt >= 16'd65500)begin//超时自恢复
								state <= s1;
								cnt<=16'd0;
								read_flag <= 1'b1;
							end	
						end
					end
				s10:begin//空一拍，产生完成一次读数据的信号
						state <= s1;
						cnt <= 16'd0;
					end
				default:begin
						state <= s1;
						cnt <= 16'd0;
					end	
			endcase
		end		
	end
	
endmodule 