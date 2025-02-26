README - TFJ V3.0 Project

Description

This project implements a temperature measurement system using a touchscreen LCD TFT display and two type K temperature sensors (thermocouples). The software allows the user to view and update temperature readings through the touchscreen interface.

Hardware Used

LCD TFT display compatible with MCUFRIEND_kbv

Touchscreen module

Nanoshield Thermocouple module

Type K temperature sensors

Arduino or compatible microcontroller

Libraries Used

MCUFRIEND_kbv: Control of the LCD TFT display

TouchScreen: Touchscreen reading

Adafruit_GFX: Graphic functions for the display

Nanoshield_Thermocouple: Control of temperature sensors

SPI: SPI communication

Nanoshield_LCD: Additional LCD control

Features

Welcome screen display

Touchscreen interface for temperature reading

Update of temperature readings on touch

Detects errors in temperature sensors

Pin Configuration

Touchscreen:

XM: A2

YP: A3

YM: 9

XP: 8

LCD TFT:

LCD_RESET: A4

LCD_CS: A3

LCD_CD: A2

LCD_WR: A1

LCD_RD: A0

Thermocouples:

Sensor 1: Pin 43

Sensor 2: Pin 44

Usage

Connect the hardware according to the specifications.

Upload the code to the microcontroller.

The display will show the welcome screen.

Use the touchscreen to update temperature readings.

Code Structure

setup(): Initializes sensors, display, and touchscreen.

loop(): Checks for screen touches and updates temperature readings.

telaBoasVindas(): Displays the system's welcome screen.

telaInicial(): Draws the temperature measurement interface.

escreveEstado(): Displays text on the screen.

criarBotao(): Draws interactive buttons.

License

This project is freely available for educational and experimental purposes. 
