################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Com/SocketServer.c \
../src/Com/TcpComLink.c 

O_SRCS += \
../src/Com/SocketServer.o \
../src/Com/TcpComLink.o 

OBJS += \
./src/Com/SocketServer.o \
./src/Com/TcpComLink.o 

C_DEPS += \
./src/Com/SocketServer.d \
./src/Com/TcpComLink.d 


# Each subdirectory must supply rules for building sources it contributes
src/Com/%.o: ../src/Com/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


