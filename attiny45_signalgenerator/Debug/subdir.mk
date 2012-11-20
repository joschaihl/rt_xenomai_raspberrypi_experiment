################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AVRTimer0.cpp \
../GPIOPins.cpp \
../IActionListener.cpp \
../System.cpp \
../Timer.cpp \
../TimerScheduler.cpp \
../Timer_test.cpp \
../main.cpp 

OBJS += \
./AVRTimer0.o \
./GPIOPins.o \
./IActionListener.o \
./System.o \
./Timer.o \
./TimerScheduler.o \
./Timer_test.o \
./main.o 

CPP_DEPS += \
./AVRTimer0.d \
./GPIOPins.d \
./IActionListener.d \
./System.d \
./Timer.d \
./TimerScheduler.d \
./Timer_test.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=attiny45 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


