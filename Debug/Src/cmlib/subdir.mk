################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/cmlib/dn_endianness.c \
../Src/cmlib/dn_hdlc.c \
../Src/cmlib/dn_ipmg.c \
../Src/cmlib/dn_ipmt.c \
../Src/cmlib/dn_lock.c \
../Src/cmlib/dn_serial_mg.c \
../Src/cmlib/dn_serial_mt.c \
../Src/cmlib/dn_uart.c \
../Src/cmlib/dn_whmt.c \
../Src/cmlib/sm.c 

OBJS += \
./Src/cmlib/dn_endianness.o \
./Src/cmlib/dn_hdlc.o \
./Src/cmlib/dn_ipmg.o \
./Src/cmlib/dn_ipmt.o \
./Src/cmlib/dn_lock.o \
./Src/cmlib/dn_serial_mg.o \
./Src/cmlib/dn_serial_mt.o \
./Src/cmlib/dn_uart.o \
./Src/cmlib/dn_whmt.o \
./Src/cmlib/sm.o 

C_DEPS += \
./Src/cmlib/dn_endianness.d \
./Src/cmlib/dn_hdlc.d \
./Src/cmlib/dn_ipmg.d \
./Src/cmlib/dn_ipmt.d \
./Src/cmlib/dn_lock.d \
./Src/cmlib/dn_serial_mg.d \
./Src/cmlib/dn_serial_mt.d \
./Src/cmlib/dn_uart.d \
./Src/cmlib/dn_whmt.d \
./Src/cmlib/sm.d 


# Each subdirectory must supply rules for building sources it contributes
Src/cmlib/%.o: ../Src/cmlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xC -I"D:/Work/IPMote/Inc" -I"D:/Work/IPMote/Src/cmlib" -I"D:/Work/IPMote/Drivers/STM32F3xx_HAL_Driver/Inc" -I"D:/Work/IPMote/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"D:/Work/IPMote/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"D:/Work/IPMote/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


