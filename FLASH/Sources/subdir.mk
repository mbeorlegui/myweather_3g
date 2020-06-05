################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/BMP180.c" \
"../Sources/CLK.c" \
"../Sources/DHT11.c" \
"../Sources/I2C.c" \
"../Sources/LCD_I2C.c" \
"../Sources/RTC.c" \
"../Sources/Radiacion.c" \
"../Sources/main.c" \

C_SRCS += \
../Sources/BMP180.c \
../Sources/CLK.c \
../Sources/DHT11.c \
../Sources/I2C.c \
../Sources/LCD_I2C.c \
../Sources/RTC.c \
../Sources/Radiacion.c \
../Sources/main.c \

OBJS += \
./Sources/BMP180_c.obj \
./Sources/CLK_c.obj \
./Sources/DHT11_c.obj \
./Sources/I2C_c.obj \
./Sources/LCD_I2C_c.obj \
./Sources/RTC_c.obj \
./Sources/Radiacion_c.obj \
./Sources/main_c.obj \

OBJS_QUOTED += \
"./Sources/BMP180_c.obj" \
"./Sources/CLK_c.obj" \
"./Sources/DHT11_c.obj" \
"./Sources/I2C_c.obj" \
"./Sources/LCD_I2C_c.obj" \
"./Sources/RTC_c.obj" \
"./Sources/Radiacion_c.obj" \
"./Sources/main_c.obj" \

C_DEPS += \
./Sources/BMP180_c.d \
./Sources/CLK_c.d \
./Sources/DHT11_c.d \
./Sources/I2C_c.d \
./Sources/LCD_I2C_c.d \
./Sources/RTC_c.d \
./Sources/Radiacion_c.d \
./Sources/main_c.d \

C_DEPS_QUOTED += \
"./Sources/BMP180_c.d" \
"./Sources/CLK_c.d" \
"./Sources/DHT11_c.d" \
"./Sources/I2C_c.d" \
"./Sources/LCD_I2C_c.d" \
"./Sources/RTC_c.d" \
"./Sources/Radiacion_c.d" \
"./Sources/main_c.d" \

OBJS_OS_FORMAT += \
./Sources/BMP180_c.obj \
./Sources/CLK_c.obj \
./Sources/DHT11_c.obj \
./Sources/I2C_c.obj \
./Sources/LCD_I2C_c.obj \
./Sources/RTC_c.obj \
./Sources/Radiacion_c.obj \
./Sources/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/BMP180_c.obj: ../Sources/BMP180.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/BMP180.args" -ObjN="Sources/BMP180_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/CLK_c.obj: ../Sources/CLK.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/CLK.args" -ObjN="Sources/CLK_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/DHT11_c.obj: ../Sources/DHT11.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/DHT11.args" -ObjN="Sources/DHT11_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/I2C_c.obj: ../Sources/I2C.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/I2C.args" -ObjN="Sources/I2C_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/LCD_I2C_c.obj: ../Sources/LCD_I2C.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/LCD_I2C.args" -ObjN="Sources/LCD_I2C_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/RTC_c.obj: ../Sources/RTC.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/RTC.args" -ObjN="Sources/RTC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/Radiacion_c.obj: ../Sources/Radiacion.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Radiacion.args" -ObjN="Sources/Radiacion_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


