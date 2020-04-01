################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../debug_src/Haxi_pwm.c 

OBJS += \
./debug_src/Haxi_pwm.o 

C_DEPS += \
./debug_src/Haxi_pwm.d 


# Each subdirectory must supply rules for building sources it contributes
debug_src/%.o: ../debug_src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I"F:\Awork_space\LF398_Debug\Xilinx\ftir_fpga_design\FTIR_FPGA_V1\FTIR_FPGA_V1.sdk\ftir_main\debug_src" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../ftir_main_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


