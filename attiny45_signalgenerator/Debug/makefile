################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
attiny45_signalgenerator.lss \

FLASH_IMAGE += \
attiny45_signalgenerator.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: attiny45_signalgenerator.elf secondary-outputs

# Tool invocations
attiny45_signalgenerator.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-g++ -Wl,-Map,attiny45_signalgenerator.map,--cref -mmcu=attiny45 -o "attiny45_signalgenerator.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

attiny45_signalgenerator.lss: attiny45_signalgenerator.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S attiny45_signalgenerator.elf  >"attiny45_signalgenerator.lss"
	@echo 'Finished building: $@'
	@echo ' '

attiny45_signalgenerator.hex: attiny45_signalgenerator.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex attiny45_signalgenerator.elf  "attiny45_signalgenerator.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: attiny45_signalgenerator.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny45 attiny45_signalgenerator.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) attiny45_signalgenerator.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
