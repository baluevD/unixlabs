################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConfigReader.cpp \
../src/Daemon.cpp \
../src/helper.cpp \
../src/main.cpp 

OBJS += \
./src/ConfigReader.o \
./src/Daemon.o \
./src/helper.o \
./src/main.o 

CPP_DEPS += \
./src/ConfigReader.d \
./src/Daemon.d \
./src/helper.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


