################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c \
../srofs.c \
../srofs_init_partition.c \
../srofs_io.c \
../srofs_util.c 

OBJS += \
./main.o \
./srofs.o \
./srofs_init_partition.o \
./srofs_io.o \
./srofs_util.o 

C_DEPS += \
./main.d \
./srofs.d \
./srofs_init_partition.d \
./srofs_io.d \
./srofs_util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/work/opensource/personal/microcontroller/srofs" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


