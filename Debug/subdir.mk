################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../srbfs.c \
../srbfs_init_partition.c \
../srbfs_io.c \
../srbfs_util.c 

OBJS += \
./srbfs.o \
./srbfs_init_partition.o \
./srbfs_io.o \
./srbfs_util.o 

C_DEPS += \
./srbfs.d \
./srbfs_init_partition.d \
./srbfs_io.d \
./srbfs_util.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/work/opensource/personal/microcontroller/srofs" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


