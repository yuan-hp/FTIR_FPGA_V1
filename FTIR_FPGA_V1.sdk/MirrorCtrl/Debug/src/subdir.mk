################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/A_MirrorCtrl.c \
../src/HTimer.c \
../src/Hak5394.c \
../src/Haxi_pwm.c \
../src/Hdac7631.c \
../src/Hgpio.c \
../src/Hmfreq.c \
../src/Htas5111.c \
../src/Htest.c \
../src/PID.c \
../src/platform.c \
../src/system.c 

OBJS += \
./src/A_MirrorCtrl.o \
./src/HTimer.o \
./src/Hak5394.o \
./src/Haxi_pwm.o \
./src/Hdac7631.o \
./src/Hgpio.o \
./src/Hmfreq.o \
./src/Htas5111.o \
./src/Htest.o \
./src/PID.o \
./src/platform.o \
./src/system.o 

C_DEPS += \
./src/A_MirrorCtrl.d \
./src/HTimer.d \
./src/Hak5394.d \
./src/Haxi_pwm.d \
./src/Hdac7631.d \
./src/Hgpio.d \
./src/Hmfreq.d \
./src/Htas5111.d \
./src/Htest.d \
./src/PID.d \
./src/platform.d \
./src/system.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../MirrorCtrl_bsp/ps7_cortexa9_1/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


