# Infineon TLE9879 Brushless DC Motor Shield library for use with PSoC® 6

This is a driver library to interface with an Infineon TLE9879 BLDC Shield using a PSOC 6. Details on the shield can be found at [BLDC_SHIELD_TLE9879](https://www.infineon.com/cms/en/product/evaluation-boards/bldc_shield_tle9879). The library supports a system with up to 4 shields.


## Quick Start

To use the library:

1. Include the header file **tle9879\_system.h**.

2. Create a pointer of type **tle9879\_sys\_t** use as a handle to the object.

2. Call the function **tle9879sys\_init** to initialize the interface. The arguments are:

    | Argument | Description | 
    | -------- | ----------- |
    | tle9879_sys_t *obj  | Poiter to the TLE9879 sheild object. |
    | cyhal_gpio_t mosi      | SPI MOSI pin. |
    | cyhal_gpio_t miso      | SPI MISO pin. |
    | cyhal_gpio_t sclk      | SPI SCLK pin. |
    | cyhal_clock_t *clk     | Clock to use for the SPI instance. Can be set to NULL for auto clock assignment by the HAL. |
    | cyhal_gpio_t ss1       | Pin used to select Shiled 1. |
    | cyhal_gpio_t ss2       | Pin used to select Shield 2. |
    | cyhal_gpio_t ss3       | Pin used to select Shield 3. |
    | cyhal_gpio_t ss4       | Pin used to select Shield 4. |
    | uint8_t *nuboards      | Pointer to a variable indicating the number of shields used in the system. |

        Note: The first shield connected uses the ss2 pin.

3. Call the function **tle9879sys\_setMode** to select FOC mode. The arguments are:

    | Argument | Description | 
    | -------- | ----------- |
    | tle9879_sys_t *obj  |    Poiter to the TLE9879 sheild object. |
    | uint8_t mode           |    Mode: (MODECONTROL, BOOTLOADER, BEMF, HALL, FOC, GETCURRENTMODE) |
    | uint8_t boardnr        |    Board number |
    | bool fastMode          |    |

4. Call the function **tle9879sys\_setMotorSpeed**. The arguments are:

    | Argument | Description | 
    | -------- | ----------- |
    | tle9879_sys_t *obj  |   Poiter to the TLE9879 sheild object. |
    | float motorspeed       |   Motor speed in RPM. |
    v uint8_t boardnr        |   Board number |

5. Call the function **tle9879sys\_setMotorMode** to turn the motor on or off. The arguments are:

    | Argument | Description | 
    | -------- | ----------- |
    | tle9879_sys_t *obj  |   Poiter to the TLE9879 sheild object. |
    | uint8_t mode           |   Mode (START_MOTOR, STOP_MOTOR, MOTORCONTROL) |
    | uint8_t boardnr        |   Board number |

**Example Code**

The folowing code will initialize the driver, set the speed to 2000 RPM, start the motor, and then stop it after 1 second.

    #include "tle9879_system.h"
        
    tle9879_sys_t tle9879_sys;

    tle9879sys_init(&tle9879_sys, 
        CYBSP_D11, 
        CYBSP_D12, 
        CYBSP_D13, 
        NULL, 
        CYBSP_D4, 
        CYBSP_D5, 
        CYBSP_D6, 
        CYBSP_D7, 
        &numboards);

    tle9879sys_setMode(&tle9879_sys, FOC, 1, false);

    tle9879sys_setMotorSpeed(&tle9879_sys, 2000.0, 1);

    tle9879sys_setMotorMode(&tle9879_sys, START_MOTOR, 1);

    cyhal_system_delay_ms(1000);

    tle9879sys_setMotorMode(&tle9879_sys, STOP_MOTOR, 1);
