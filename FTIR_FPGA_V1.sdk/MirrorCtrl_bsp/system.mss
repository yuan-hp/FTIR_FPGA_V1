
 PARAMETER VERSION = 2.2.0


BEGIN OS
 PARAMETER OS_NAME = standalone
 PARAMETER OS_VER = 6.8
 PARAMETER PROC_INSTANCE = ps7_cortexa9_1
 PARAMETER stdin = ps7_uart_1
 PARAMETER stdout = ps7_uart_1
END


BEGIN PROCESSOR
 PARAMETER DRIVER_NAME = cpu_cortexa9
 PARAMETER DRIVER_VER = 2.7
 PARAMETER HW_INSTANCE = ps7_cortexa9_1
 PARAMETER extra_compiler_flags = -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -nostartfiles -g -Wall -Wextra -DUSE_AMP=1
END


BEGIN DRIVER
 PARAMETER DRIVER_NAME = AK5394_LF398
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = AK5394_LF398_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = DAC7631
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = DAC7631_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = FPGA_PWM
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = FPGA_PWM_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = FreqMeasure
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = FreqMeasure_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = ax_pwm
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = ax_pwm_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = axi_debug_io
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = axi_debug_io_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 4.3
 PARAMETER HW_INSTANCE = axi_gpio_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpio
 PARAMETER DRIVER_VER = 4.3
 PARAMETER HW_INSTANCE = axi_gpio_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_afi_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_afi_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_afi_2
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_afi_3
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = coresightps_dcc
 PARAMETER DRIVER_VER = 1.4
 PARAMETER HW_INSTANCE = ps7_coresight_comp_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = ddrps
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = ps7_ddr_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_ddrc_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = devcfg
 PARAMETER DRIVER_VER = 3.5
 PARAMETER HW_INSTANCE = ps7_dev_cfg_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = dmaps
 PARAMETER DRIVER_VER = 2.4
 PARAMETER HW_INSTANCE = ps7_dma_ns
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = dmaps
 PARAMETER DRIVER_VER = 2.4
 PARAMETER HW_INSTANCE = ps7_dma_s
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = emacps
 PARAMETER DRIVER_VER = 3.8
 PARAMETER HW_INSTANCE = ps7_ethernet_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_globaltimer_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = gpiops
 PARAMETER DRIVER_VER = 3.4
 PARAMETER HW_INSTANCE = ps7_gpio_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_gpv_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_intc_dist_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_iop_bus_config_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_l2cachec_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_ocmc_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_pl310_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_pmu_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = qspips
 PARAMETER DRIVER_VER = 3.5
 PARAMETER HW_INSTANCE = ps7_qspi_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_qspi_linear_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_ram_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_ram_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_scuc_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = scugic
 PARAMETER DRIVER_VER = 3.10
 PARAMETER HW_INSTANCE = ps7_scugic_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = scutimer
 PARAMETER DRIVER_VER = 2.1
 PARAMETER HW_INSTANCE = ps7_scutimer_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = scuwdt
 PARAMETER DRIVER_VER = 2.1
 PARAMETER HW_INSTANCE = ps7_scuwdt_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = generic
 PARAMETER DRIVER_VER = 2.0
 PARAMETER HW_INSTANCE = ps7_slcr_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = spips
 PARAMETER DRIVER_VER = 3.1
 PARAMETER HW_INSTANCE = ps7_spi_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = ttcps
 PARAMETER DRIVER_VER = 3.7
 PARAMETER HW_INSTANCE = ps7_ttc_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartps
 PARAMETER DRIVER_VER = 3.7
 PARAMETER HW_INSTANCE = ps7_uart_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = uartps
 PARAMETER DRIVER_VER = 3.7
 PARAMETER HW_INSTANCE = ps7_uart_1
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = xadcps
 PARAMETER DRIVER_VER = 2.3
 PARAMETER HW_INSTANCE = ps7_xadc_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = AXI_SIG_CFG
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = AXI_SIG_CFG_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = Sensor
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = Sensor_0
END

BEGIN DRIVER
 PARAMETER DRIVER_NAME = TAS5111
 PARAMETER DRIVER_VER = 1.0
 PARAMETER HW_INSTANCE = TAS5111_0
END


