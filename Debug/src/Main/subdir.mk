################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main/DecisionTableExample.c \
../src/Main/ExceptionsExample.c \
../src/Main/Fixtures.c \
../src/Main/Main.c \
../src/Main/QueryTableExample.c \
../src/Main/ScriptTableExample.c 

O_SRCS += \
../src/Main/DecisionTableExample.o \
../src/Main/ExceptionsExample.o \
../src/Main/FixtureInCpp.o \
../src/Main/Fixtures.o \
../src/Main/Main.o \
../src/Main/QueryTableExample.o \
../src/Main/ScriptTableExample.o 

OBJS += \
./src/Main/DecisionTableExample.o \
./src/Main/ExceptionsExample.o \
./src/Main/Fixtures.o \
./src/Main/Main.o \
./src/Main/QueryTableExample.o \
./src/Main/ScriptTableExample.o 

C_DEPS += \
./src/Main/DecisionTableExample.d \
./src/Main/ExceptionsExample.d \
./src/Main/Fixtures.d \
./src/Main/Main.d \
./src/Main/QueryTableExample.d \
./src/Main/ScriptTableExample.d 


# Each subdirectory must supply rules for building sources it contributes
src/Main/%.o: ../src/Main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


