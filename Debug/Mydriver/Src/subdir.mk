################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mydriver/Src/MPU6050.c \
../Mydriver/Src/MyAdc.c \
../Mydriver/Src/MyHcsr04.c \
../Mydriver/Src/MyLcd.c 

OBJS += \
./Mydriver/Src/MPU6050.o \
./Mydriver/Src/MyAdc.o \
./Mydriver/Src/MyHcsr04.o \
./Mydriver/Src/MyLcd.o 

C_DEPS += \
./Mydriver/Src/MPU6050.d \
./Mydriver/Src/MyAdc.d \
./Mydriver/Src/MyHcsr04.d \
./Mydriver/Src/MyLcd.d 


# Each subdirectory must supply rules for building sources it contributes
Mydriver/Src/%.o Mydriver/Src/%.su: ../Mydriver/Src/%.c Mydriver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I"C:/Users/Lenovo/STM32CubeIDE/workspace_1.9.0/AdcProject1/Mydriver/Inc" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Mydriver-2f-Src

clean-Mydriver-2f-Src:
	-$(RM) ./Mydriver/Src/MPU6050.d ./Mydriver/Src/MPU6050.o ./Mydriver/Src/MPU6050.su ./Mydriver/Src/MyAdc.d ./Mydriver/Src/MyAdc.o ./Mydriver/Src/MyAdc.su ./Mydriver/Src/MyHcsr04.d ./Mydriver/Src/MyHcsr04.o ./Mydriver/Src/MyHcsr04.su ./Mydriver/Src/MyLcd.d ./Mydriver/Src/MyLcd.o ./Mydriver/Src/MyLcd.su

.PHONY: clean-Mydriver-2f-Src

