# PWM: Servo motor 

This is an application which control a servo motor using PWM.

### Board

[STM32 NUCLEO-L476RG](https://docs.zephyrproject.org/latest/boards/arm/nucleo_l476rg/doc/index.html)

### Wiring

Servo:
- GND (brown)     	         	- board GND
- 5V (red)                 		- board 5V
- Signal (orange)          		- A0 PIN

### Zephyr Environment Variables Setup
Set environment variables:
```console
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.11.2
```
Source environment:
```console
cd ~/zephyrproject/zephyr
source zephyr-env.sh
```

### Building And Flashing Servo Application
```console
cd zephyr_samples/servo/
west build -p auto -b nucleo_l476rg .
west flash
```

