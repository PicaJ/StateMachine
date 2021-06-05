################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/StateMachinerun.c \
../src/StateOne.c \
../src/StateThree.c \
../src/StateTwo.c \
../src/main.c \
../src/queue.c \
../src/statebase.c 

OBJS += \
./src/StateMachinerun.o \
./src/StateOne.o \
./src/StateThree.o \
./src/StateTwo.o \
./src/main.o \
./src/queue.o \
./src/statebase.o 

C_DEPS += \
./src/StateMachinerun.d \
./src/StateOne.d \
./src/StateThree.d \
./src/StateTwo.d \
./src/main.d \
./src/queue.d \
./src/statebase.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/hi/workspace/StateMachine/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


