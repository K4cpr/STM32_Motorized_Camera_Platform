################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/lpuart.c \
../Src/main.c \
../Src/state_machine.c \
../Src/sys_clocks.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tim.c \
../Src/uart_protocol.c 

OBJS += \
./Src/dma.o \
./Src/lpuart.o \
./Src/main.o \
./Src/state_machine.o \
./Src/sys_clocks.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tim.o \
./Src/uart_protocol.o 

C_DEPS += \
./Src/dma.d \
./Src/lpuart.d \
./Src/main.d \
./Src/state_machine.d \
./Src/sys_clocks.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tim.d \
./Src/uart_protocol.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32L4 -DSTM32L496xx -DSTM32L496ZGTx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.cyclo ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/lpuart.cyclo ./Src/lpuart.d ./Src/lpuart.o ./Src/lpuart.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/state_machine.cyclo ./Src/state_machine.d ./Src/state_machine.o ./Src/state_machine.su ./Src/sys_clocks.cyclo ./Src/sys_clocks.d ./Src/sys_clocks.o ./Src/sys_clocks.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/tim.cyclo ./Src/tim.d ./Src/tim.o ./Src/tim.su ./Src/uart_protocol.cyclo ./Src/uart_protocol.d ./Src/uart_protocol.o ./Src/uart_protocol.su

.PHONY: clean-Src

