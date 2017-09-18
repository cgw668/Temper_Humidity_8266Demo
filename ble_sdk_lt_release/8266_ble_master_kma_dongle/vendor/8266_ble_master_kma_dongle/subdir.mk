################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/8266_ble_master_kma_dongle/audio_buffer.c \
../vendor/8266_ble_master_kma_dongle/dongle.c \
../vendor/8266_ble_master_kma_dongle/main.c 

OBJS += \
./vendor/8266_ble_master_kma_dongle/audio_buffer.o \
./vendor/8266_ble_master_kma_dongle/dongle.o \
./vendor/8266_ble_master_kma_dongle/main.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/8266_ble_master_kma_dongle/%.o: ../vendor/8266_ble_master_kma_dongle/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -D__PROJECT_BLE_MASTER_KMA_DONGLE__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


