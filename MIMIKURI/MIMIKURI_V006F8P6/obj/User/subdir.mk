################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../User/eeprom.s 

C_SRCS += \
../User/main.c 

OBJS += \
./User/eeprom.o \
./User/main.o 

S_DEPS += \
./User/eeprom.d 

C_DEPS += \
./User/main.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.s
	@	@	riscv-none-elf-gcc -march=rv32ec_zmmul_xw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"C:\ooizumis\WCH\MIMIKURI\MIMIKURI_V006F8P6\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
User/%.o: ../User/%.c
	@	@	riscv-none-elf-gcc -march=rv32ec_zmmul_xw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\ooizumis\WCH\MIMIKURI\MIMIKURI_V006F8P6\Debug" -I"C:\ooizumis\WCH\MIMIKURI\MIMIKURI_V006F8P6\Core" -I"C:\ooizumis\WCH\MIMIKURI\MIMIKURI_V006F8P6\User" -I"C:\ooizumis\WCH\MIMIKURI\MIMIKURI_V006F8P6\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

