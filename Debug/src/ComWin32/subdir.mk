################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ComWin32/Win32ComLink.c \
../src/ComWin32/Win32SocketServer.c 

OBJS += \
./src/ComWin32/Win32ComLink.o \
./src/ComWin32/Win32SocketServer.o 

C_DEPS += \
./src/ComWin32/Win32ComLink.d \
./src/ComWin32/Win32SocketServer.d 


# Each subdirectory must supply rules for building sources it contributes
src/ComWin32/%.o: ../src/ComWin32/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


