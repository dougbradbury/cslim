################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CSlim/ListExecutor.c \
../src/CSlim/SlimConnectionHandler.c \
../src/CSlim/SlimList.c \
../src/CSlim/SlimListDeserializer.c \
../src/CSlim/SlimListSerializer.c \
../src/CSlim/SlimUtil.c \
../src/CSlim/StatementExecutor.c \
../src/CSlim/SymbolTable.c 

O_SRCS += \
../src/CSlim/ListExecutor.o \
../src/CSlim/SlimConnectionHandler.o \
../src/CSlim/SlimList.o \
../src/CSlim/SlimListDeserializer.o \
../src/CSlim/SlimListSerializer.o \
../src/CSlim/SlimUtil.o \
../src/CSlim/StatementExecutor.o \
../src/CSlim/SymbolTable.o 

OBJS += \
./src/CSlim/ListExecutor.o \
./src/CSlim/SlimConnectionHandler.o \
./src/CSlim/SlimList.o \
./src/CSlim/SlimListDeserializer.o \
./src/CSlim/SlimListSerializer.o \
./src/CSlim/SlimUtil.o \
./src/CSlim/StatementExecutor.o \
./src/CSlim/SymbolTable.o 

C_DEPS += \
./src/CSlim/ListExecutor.d \
./src/CSlim/SlimConnectionHandler.d \
./src/CSlim/SlimList.d \
./src/CSlim/SlimListDeserializer.d \
./src/CSlim/SlimListSerializer.d \
./src/CSlim/SlimUtil.d \
./src/CSlim/StatementExecutor.d \
./src/CSlim/SymbolTable.d 


# Each subdirectory must supply rules for building sources it contributes
src/CSlim/%.o: ../src/CSlim/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


