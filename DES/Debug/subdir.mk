################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BitByteMath.cpp \
../DES.cpp \
../DefinitionTables.cpp \
../Expansion.cpp \
../Feistel.cpp \
../Filereader.cpp \
../PermutationSteps.cpp \
../main.cpp 

OBJS += \
./BitByteMath.o \
./DES.o \
./DefinitionTables.o \
./Expansion.o \
./Feistel.o \
./Filereader.o \
./PermutationSteps.o \
./main.o 

CPP_DEPS += \
./BitByteMath.d \
./DES.d \
./DefinitionTables.d \
./Expansion.d \
./Feistel.d \
./Filereader.d \
./PermutationSteps.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"C:\cygwin64\lib\gcc" -I"C:\cygwin64\lib\gcc\i686-pc-cygwin\6.4.0\include\c++" -I"C:\cygwin64\lib\gcc\i686-pc-cygwin\6.4.0\include\c++\i686-pc-cygwin" -I"C:\cygwin64\lib\gcc\i686-pc-cygwin\6.4.0\include\c++\backward" -I"C:\cygwin64\usr\include" -I"C:\cygwin64\usr\include\w32api" -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


