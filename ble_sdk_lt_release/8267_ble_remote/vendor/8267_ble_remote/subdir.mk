################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/8267_ble_remote/app_att.c \
../vendor/8267_ble_remote/ble_remote.c \
../vendor/8267_ble_remote/main.c \
../vendor/8267_ble_remote/rc_ir.c 

OBJS += \
./vendor/8267_ble_remote/app_att.o \
./vendor/8267_ble_remote/ble_remote.o \
./vendor/8267_ble_remote/main.o \
./vendor/8267_ble_remote/rc_ir.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/8267_ble_remote/%.o: ../vendor/8267_ble_remote/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -D__PROJECT_BLE_REMOTE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


