//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
//Date        : Wed Nov 20 17:09:36 2019
//Host        : FPGA-Use running 64-bit Service Pack 1  (build 7601)
//Command     : generate_target design_1_wrapper.bd
//Design      : design_1_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module design_1_wrapper
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    FPGA_AD_FSYNC,
    FPGA_AD_LRCK,
    FPGA_AD_MCLK,
    FPGA_AD_RSTn,
    FPGA_AD_SCLK,
    FPGA_AD_SDATA,
    FPGA_AD_ZCAL,
    FPGA_DAC_CS,
    FPGA_DAC_LDAC_A,
    FPGA_DAC_LDAC_B,
    FPGA_DAC_LOAD,
    FPGA_DAC_RST,
    FPGA_DAC_SCK,
    FPGA_DAC_SDO_A,
    FPGA_DAC_SDO_B,
    FPGA_DHT11_DATA,
    FPGA_DongJ_PWM_C0,
    FPGA_DongJ_PWM_C1,
    FPGA_LASER_PULSE,
    FPGA_LOGIC_LF398,
    FPGA_PWM_0,
    FPGA_PWM_C2,
    FPGA_PWM_DIR_0,
    FPGA_SPI0_CSn0,
    FPGA_SPI0_SCLK,
    FPGA_SPI0_SDO,
    FPGA_Switch,
    FPGA_TAS_OTWn,
    FPGA_TAS_PWM_A,
    FPGA_TAS_PWM_B,
    FPGA_TAS_RESETn,
    FPGA_TAS_SD,
    FPGA_UART0_RX,
    FPGA_UART0_TX,
    LED2,
    LED3,
    LED4,
    pwm_0);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  input FPGA_AD_FSYNC;
  input FPGA_AD_LRCK;
  output FPGA_AD_MCLK;
  output FPGA_AD_RSTn;
  input FPGA_AD_SCLK;
  input FPGA_AD_SDATA;
  output FPGA_AD_ZCAL;
  output FPGA_DAC_CS;
  output FPGA_DAC_LDAC_A;
  output FPGA_DAC_LDAC_B;
  output FPGA_DAC_LOAD;
  output FPGA_DAC_RST;
  output FPGA_DAC_SCK;
  output FPGA_DAC_SDO_A;
  output FPGA_DAC_SDO_B;
  inout FPGA_DHT11_DATA;
  output FPGA_DongJ_PWM_C0;
  output FPGA_DongJ_PWM_C1;
  input FPGA_LASER_PULSE;
  output FPGA_LOGIC_LF398;
  output [2:0]FPGA_PWM_0;
  output FPGA_PWM_C2;
  output [2:0]FPGA_PWM_DIR_0;
  output FPGA_SPI0_CSn0;
  output FPGA_SPI0_SCLK;
  output FPGA_SPI0_SDO;
  input FPGA_Switch;
  input FPGA_TAS_OTWn;
  output FPGA_TAS_PWM_A;
  output FPGA_TAS_PWM_B;
  output FPGA_TAS_RESETn;
  input FPGA_TAS_SD;
  input FPGA_UART0_RX;
  output FPGA_UART0_TX;
  output LED2;
  output LED3;
  output LED4;
  output pwm_0;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire FPGA_AD_FSYNC;
  wire FPGA_AD_LRCK;
  wire FPGA_AD_MCLK;
  wire FPGA_AD_RSTn;
  wire FPGA_AD_SCLK;
  wire FPGA_AD_SDATA;
  wire FPGA_AD_ZCAL;
  wire FPGA_DAC_CS;
  wire FPGA_DAC_LDAC_A;
  wire FPGA_DAC_LDAC_B;
  wire FPGA_DAC_LOAD;
  wire FPGA_DAC_RST;
  wire FPGA_DAC_SCK;
  wire FPGA_DAC_SDO_A;
  wire FPGA_DAC_SDO_B;
  wire FPGA_DHT11_DATA;
  wire FPGA_DongJ_PWM_C0;
  wire FPGA_DongJ_PWM_C1;
  wire FPGA_LASER_PULSE;
  wire FPGA_LOGIC_LF398;
  wire [2:0]FPGA_PWM_0;
  wire FPGA_PWM_C2;
  wire [2:0]FPGA_PWM_DIR_0;
  wire FPGA_SPI0_CSn0;
  wire FPGA_SPI0_SCLK;
  wire FPGA_SPI0_SDO;
  wire FPGA_Switch;
  wire FPGA_TAS_OTWn;
  wire FPGA_TAS_PWM_A;
  wire FPGA_TAS_PWM_B;
  wire FPGA_TAS_RESETn;
  wire FPGA_TAS_SD;
  wire FPGA_UART0_RX;
  wire FPGA_UART0_TX;
  wire LED2;
  wire LED3;
  wire LED4;
  wire pwm_0;

  design_1 design_1_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .FPGA_AD_FSYNC(FPGA_AD_FSYNC),
        .FPGA_AD_LRCK(FPGA_AD_LRCK),
        .FPGA_AD_MCLK(FPGA_AD_MCLK),
        .FPGA_AD_RSTn(FPGA_AD_RSTn),
        .FPGA_AD_SCLK(FPGA_AD_SCLK),
        .FPGA_AD_SDATA(FPGA_AD_SDATA),
        .FPGA_AD_ZCAL(FPGA_AD_ZCAL),
        .FPGA_DAC_CS(FPGA_DAC_CS),
        .FPGA_DAC_LDAC_A(FPGA_DAC_LDAC_A),
        .FPGA_DAC_LDAC_B(FPGA_DAC_LDAC_B),
        .FPGA_DAC_LOAD(FPGA_DAC_LOAD),
        .FPGA_DAC_RST(FPGA_DAC_RST),
        .FPGA_DAC_SCK(FPGA_DAC_SCK),
        .FPGA_DAC_SDO_A(FPGA_DAC_SDO_A),
        .FPGA_DAC_SDO_B(FPGA_DAC_SDO_B),
        .FPGA_DHT11_DATA(FPGA_DHT11_DATA),
        .FPGA_DongJ_PWM_C0(FPGA_DongJ_PWM_C0),
        .FPGA_DongJ_PWM_C1(FPGA_DongJ_PWM_C1),
        .FPGA_LASER_PULSE(FPGA_LASER_PULSE),
        .FPGA_LOGIC_LF398(FPGA_LOGIC_LF398),
        .FPGA_PWM_0(FPGA_PWM_0),
        .FPGA_PWM_C2(FPGA_PWM_C2),
        .FPGA_PWM_DIR_0(FPGA_PWM_DIR_0),
        .FPGA_SPI0_CSn0(FPGA_SPI0_CSn0),
        .FPGA_SPI0_SCLK(FPGA_SPI0_SCLK),
        .FPGA_SPI0_SDO(FPGA_SPI0_SDO),
        .FPGA_Switch(FPGA_Switch),
        .FPGA_TAS_OTWn(FPGA_TAS_OTWn),
        .FPGA_TAS_PWM_A(FPGA_TAS_PWM_A),
        .FPGA_TAS_PWM_B(FPGA_TAS_PWM_B),
        .FPGA_TAS_RESETn(FPGA_TAS_RESETn),
        .FPGA_TAS_SD(FPGA_TAS_SD),
        .FPGA_UART0_RX(FPGA_UART0_RX),
        .FPGA_UART0_TX(FPGA_UART0_TX),
        .LED2(LED2),
        .LED3(LED3),
        .LED4(LED4),
        .pwm_0(pwm_0));
endmodule
