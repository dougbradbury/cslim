################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tests/CSlim/TestSlim.c 

OBJS += \
./tests/CSlim/TestSlim.o 

C_DEPS += \
./tests/CSlim/TestSlim.d 


# Each subdirectory must supply rules for building sources it contributes
tests/CSlim/%.o: ../tests/CSlim/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


