################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
MIMIKURI_V006F8P6.hex \

SECONDARY_LIST += \
MIMIKURI_V006F8P6.lst \

SECONDARY_SIZE += \
MIMIKURI_V006F8P6.siz \


# All Target
all: MIMIKURI_V006F8P6.elf secondary-outputs

# Tool invocations
MIMIKURI_V006F8P6.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-elf-gcc -march=rv32ec_zmmul_xw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "C:\ooizumis\WCH\MIMIKURI\MIMIKURI_V006F8P6\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"MIMIKURI_V006F8P6.map" --specs=nano.specs --specs=nosys.specs -o "MIMIKURI_V006F8P6.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintf
	@	@
MIMIKURI_V006F8P6.hex: MIMIKURI_V006F8P6.elf
	@	riscv-none-elf-objcopy -O ihex "MIMIKURI_V006F8P6.elf"  "MIMIKURI_V006F8P6.hex"
	@	@
MIMIKURI_V006F8P6.lst: MIMIKURI_V006F8P6.elf
	@	riscv-none-elf-objdump --all-headers --demangle --disassemble -M xw "MIMIKURI_V006F8P6.elf" > "MIMIKURI_V006F8P6.lst"
	@	@
MIMIKURI_V006F8P6.siz: MIMIKURI_V006F8P6.elf
	@	riscv-none-elf-size --format=berkeley "MIMIKURI_V006F8P6.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) MIMIKURI_V006F8P6.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
