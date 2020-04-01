################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/Ethernet.c \
../src/HTimer.c \
../src/H_IRQ.c \
../src/Hak5394.c \
../src/Haxi_gpio.c \
../src/Hcfg_sig.c \
../src/Hdac7631.c \
../src/Hgpio.c \
../src/Hmfreq.c \
../src/Hspi.c \
../src/Htas5111.c \
../src/Htest.c \
../src/Huart.c \
../src/i2c_access.c \
../src/iic_phyreset.c \
../src/main.c \
../src/platform.c \
../src/platform_mb.c \
../src/platform_ppc.c \
../src/platform_zynq.c \
../src/platform_zynqmp.c \
../src/sfp.c \
../src/si5324.c \
../src/system.c 

OBJS += \
./src/Ethernet.o \
./src/HTimer.o \
./src/H_IRQ.o \
./src/Hak5394.o \
./src/Haxi_gpio.o \
./src/Hcfg_sig.o \
./src/Hdac7631.o \
./src/Hgpio.o \
./src/Hmfreq.o \
./src/Hspi.o \
./src/Htas5111.o \
./src/Htest.o \
./src/Huart.o \
./src/i2c_access.o \
./src/iic_phyreset.o \
./src/main.o \
./src/platform.o \
./src/platform_mb.o \
./src/platform_ppc.o \
./src/platform_zynq.o \
./src/platform_zynqmp.o \
./src/sfp.o \
./src/si5324.o \
./src/system.o 

C_DEPS += \
./src/Ethernet.d \
./src/HTimer.d \
./src/H_IRQ.d \
./src/Hak5394.d \
./src/Haxi_gpio.d \
./src/Hcfg_sig.d \
./src/Hdac7631.d \
./src/Hgpio.d \
./src/Hmfreq.d \
./src/Hspi.d \
./src/Htas5111.d \
./src/Htest.d \
./src/Huart.d \
./src/i2c_access.d \
./src/iic_phyreset.d \
./src/main.d \
./src/platform.d \
./src/platform_mb.d \
./src/platform_ppc.d \
./src/platform_zynq.d \
./src/platform_zynqmp.d \
./src/sfp.d \
./src/si5324.d \
./src/system.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"F:\Awork_space\LF398_Debug\Xilinx\ftir_fpga_design\FTIR_FPGA_V1\FTIR_FPGA_V1.sdk\ftir_main\debug_src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../ftir_main_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

