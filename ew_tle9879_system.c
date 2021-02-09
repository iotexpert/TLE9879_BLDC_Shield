#include "cyhal.h"
#include "ew_tle9879_system.h"
#include "ew_tle9879_board_driver.h"

static cyhal_spi_t tle9879_spi_obj;
static ew_tle9879_board_t tle9879_boardA;
static ew_tle9879_board_t tle9879_boardB;
static ew_tle9879_board_t tle9879_boardC;
static ew_tle9879_board_t tle9879_boardD;

//private board communications functions


//private helper functions

void ew_tle9879sys_sendMessageToAll(uint16_t data)
{
	uint8_t txdata[2];
	uint8_t rxdata[2];

	txdata[0] = (uint8_t) ((data & 0xFF00) >> 8);
	txdata[1] = (uint8_t) (data & 0x00FF);

	// select all available boards
    if(tle9879_boardA.board_available) cyhal_gpio_write(tle9879_boardA.slaveselectpin, TLE9879_SPI_SELECT_ENABLE);
    if(tle9879_boardB.board_available) cyhal_gpio_write(tle9879_boardB.slaveselectpin, TLE9879_SPI_SELECT_ENABLE);
    if(tle9879_boardC.board_available) cyhal_gpio_write(tle9879_boardC.slaveselectpin, TLE9879_SPI_SELECT_ENABLE);
    if(tle9879_boardD.board_available) cyhal_gpio_write(tle9879_boardD.slaveselectpin, TLE9879_SPI_SELECT_ENABLE);

	// send the message to all boards
	cyhal_system_delay_us(100);
	cyhal_spi_transfer(&tle9879_spi_obj, (const uint8_t *) txdata, 2, rxdata, 2, 0x00);
	cyhal_system_delay_us(100);
	
	// deselect all available boards
    if(tle9879_boardA.board_available) cyhal_gpio_write(tle9879_boardA.slaveselectpin, TLE9879_SPI_SELECT_DISABLE);
    if(tle9879_boardB.board_available) cyhal_gpio_write(tle9879_boardB.slaveselectpin, TLE9879_SPI_SELECT_DISABLE);
    if(tle9879_boardC.board_available) cyhal_gpio_write(tle9879_boardC.slaveselectpin, TLE9879_SPI_SELECT_DISABLE);
    if(tle9879_boardD.board_available) cyhal_gpio_write(tle9879_boardD.slaveselectpin, TLE9879_SPI_SELECT_DISABLE);

	cyhal_system_delay_us(100);
}


//Public facing functions
void ew_tle9879sys_setMode(ew_tle9879_sys_t *obj, uint8_t mode, uint8_t boardnr, bool fastMode)
{
	
	if(fastMode)
	{
		ew_tle9879sys_sendMessageToAll(MODECONTROL + mode);
		cyhal_system_delay_ms(1000);
		// for( ; index < end; index++)
		// {
		// 	boards[index]->modeControl(GETCURRENTMODE);
		// 	if(boards[index]->getCurrentMode() != mode)
		// 	{
		// 		status->code = ERR_MODE_CHANGE_FAILED;
		// 		status->additionalInfo[0] = (uint16_t) boards[index]->getCurrentMode();
		// 		status->additionalInfo[1] = (uint16_t) mode;
		// 		status->additionalInfo[2] = (uint16_t) 0;
		// 		processStatusCodes(MODECONTROL, index + 1);
		// 	}
		// }
	} else
	{	
		{
			// boards[index]->modeControl(mode);
			// processStatusCodes(MODECONTROL, index + 1);
            ew_tle9879_modeControl(obj->board[boardnr - 1], mode);
		}
	}
	
}

void ew_tle9879sys_setMotorMode(ew_tle9879_sys_t *obj, uint8_t mode, uint8_t boardnr)
{
	if (mode > STOP_MOTOR)
	{
		// obj->status.code = ERR_INVALID_PARAMETER;
		// obj->status.additionalInfo[0] = mode;
		return;
	}
	
		ew_tle9879_motorControl(obj->board[boardnr - 1], mode);
		// processStatusCodes(MOTORCONTROL, index + 1);
	
}

void ew_tle9879sys_setMotorSpeed(ew_tle9879_sys_t *obj, float motorspeed, uint8_t boardnr)
{
    ew_tle9879_setMotorspeed(obj->board[boardnr - 1], motorspeed);
		// processStatusCodes(SETMOTORSPEED, index + 1);
	
}

void ew_tle9879sys_setLed(ew_tle9879_sys_t *obj, uint16_t led, uint16_t mode, uint8_t boardnr)
{	
    if(mode == LED_ON) ew_tle9879_LEDOn(obj->board[boardnr - 1],led);
    else if(mode == LED_OFF) ew_tle9879_LEDOff(obj->board[boardnr - 1], led);
    // processStatusCodes(LED, index + 1);
}

void ew_tle9879sys_setLedColor(ew_tle9879_sys_t *obj, uint8_t color, uint8_t boardnr)
{
	// blue LED
	if(color & 1) ew_tle9879sys_setLed(obj, LED_BLUE, LED_ON, boardnr);
	else ew_tle9879sys_setLed(obj, LED_BLUE, LED_OFF, boardnr);
	
	// // green LED
	if(color & 2) ew_tle9879sys_setLed(obj, LED_GREEN, LED_ON, boardnr);
	else ew_tle9879sys_setLed(obj, LED_GREEN, LED_OFF, boardnr);
	
	// // red LED
	if(color & 4) ew_tle9879sys_setLed(obj, LED_RED, LED_ON, boardnr);
	else ew_tle9879sys_setLed(obj, LED_RED, LED_OFF, boardnr);
}

void ew_tle9879sys_resetAllBoards()
{
	// stop all motors (they may be running)
	ew_tle9879sys_sendMessageToAll(MOTORCONTROL + STOP_MOTOR);
	cyhal_system_delay_ms(10);
	// reset all TLE9879_Boards
	ew_tle9879sys_sendMessageToAll(BOARDCONTROL + RESET);
	cyhal_system_delay_ms(1000); // TODO reduce time, let slaves do reset  (note this TODO was in original Arduino code kmwh)
}

void ew_tle9879sys_init(ew_tle9879_sys_t *obj, cyhal_gpio_t mosi, cyhal_gpio_t miso, cyhal_gpio_t sclk, const cyhal_clock_t *clk, 
                    cyhal_gpio_t ss1, cyhal_gpio_t ss2, cyhal_gpio_t ss3, cyhal_gpio_t ss4, uint8_t* numboards)
{
	cy_rslt_t result;

    tle9879_boardA.tle9879_spi = &tle9879_spi_obj;
    tle9879_boardB.tle9879_spi = &tle9879_spi_obj;
    tle9879_boardC.tle9879_spi = &tle9879_spi_obj;
    tle9879_boardD.tle9879_spi = &tle9879_spi_obj;

    obj->board_count = 0;

    DEBUG_PRINTS("INFO: Starting initialization\r\n");
	
	// set the auto-addressing pin to high
    cyhal_gpio_init(CYBSP_D8, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1);
	
    tle9879_boardA.slaveselectpin = ss1;
    tle9879_boardB.slaveselectpin = ss2;
    tle9879_boardC.slaveselectpin = ss3;
    tle9879_boardD.slaveselectpin = ss4;

    tle9879_boardA.board_available = false;
    tle9879_boardB.board_available = false;
    tle9879_boardC.board_available = false;
    tle9879_boardD.board_available = false;

	// init and disable all the slave-select-pins
    cyhal_gpio_init(ss1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, TLE9879_SPI_SELECT_DISABLE);
    cyhal_gpio_init(ss2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, TLE9879_SPI_SELECT_DISABLE);
    cyhal_gpio_init(ss3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, TLE9879_SPI_SELECT_DISABLE);
    cyhal_gpio_init(ss4, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, TLE9879_SPI_SELECT_DISABLE);
    
    cyhal_system_delay_ms(500);
	
    result = cyhal_spi_init(&tle9879_spi_obj, mosi, miso, sclk, NC, clk, 8, CYHAL_SPI_MODE_10_MSB, false);

    if(result == CY_RSLT_SUCCESS)
    {
        cyhal_spi_set_frequency(&tle9879_spi_obj, 100000);
    }
    else
    {
        DEBUG_PRINTS("Error initializing motor driver SPI\r\n");
    }
    
    cyhal_system_delay_ms(250);

	// all boards are reset and set to bootloader mode
	ew_tle9879sys_resetAllBoards();
	cyhal_system_delay_ms(500);
	
    uint8_t check_counts = 0;

    while(!tle9879_boardA.board_available)
    {
        // DEBUG_PRINTS("Checking board A availability\r\n");
        ew_tle9879_boardControl(&tle9879_boardA);
        cyhal_system_delay_ms(50);

        check_counts++;
        if(check_counts > 3) break;
    }

    if(tle9879_boardA.board_available)
    {
        DEBUG_PRINTS("Board A available\r\n");
        ew_tle9879_LEDOn(&tle9879_boardA, LED_BLUE);
        obj->board[obj->board_count] = &tle9879_boardA;
        obj->board_count++;
        tle9879_boardA.boardnr = obj->board_count;
    }
    else
    {
        DEBUG_PRINTS("Board A not avialable\r\n");
    }

    check_counts = 0;

    while(!tle9879_boardB.board_available)
    {
        // DEBUG_PRINTS("Checking board B availability\r\n");
        ew_tle9879_boardControl(&tle9879_boardB);
        cyhal_system_delay_ms(50);

        check_counts++;
        if(check_counts > 3) break;
    }

    if(tle9879_boardB.board_available)
    {
        DEBUG_PRINTS("Board B available\r\n");
        ew_tle9879_LEDOn(&tle9879_boardB, LED_BLUE);
        obj->board[obj->board_count] = &tle9879_boardB;
        obj->board_count++;
        tle9879_boardB.boardnr = obj->board_count;
    }
    else
    {
        DEBUG_PRINTS("Board B not avialable\r\n");
    }

    check_counts = 0;

    while(!tle9879_boardC.board_available)
    {
        // DEBUG_PRINTS("Checking board C availability\r\n");
        ew_tle9879_boardControl(&tle9879_boardC);
        cyhal_system_delay_ms(50);

        check_counts++;
        if(check_counts > 3) break;
    }

    if(tle9879_boardC.board_available)
    {
        // DEBUG_PRINTS("Board C available\r\n");
        ew_tle9879_LEDOn(&tle9879_boardA, LED_BLUE);
        obj->board[obj->board_count] = &tle9879_boardC;
        obj->board_count++;
        tle9879_boardC.boardnr = obj->board_count;
    }
    else
    {
        DEBUG_PRINTS("Board C not avialable\r\n");
    }

    check_counts = 0;

    while(!tle9879_boardD.board_available)
    {
        // DEBUG_PRINTS("Checking board D availability\r\n");
        ew_tle9879_boardControl(&tle9879_boardD);
        cyhal_system_delay_ms(50);

        check_counts++;
        if(check_counts > 3) break;
    }

    if(tle9879_boardD.board_available)
    {
        DEBUG_PRINTS("Board D available\r\n");
        ew_tle9879_LEDOn(&tle9879_boardD, LED_BLUE);
        obj->board[obj->board_count] = &tle9879_boardD;
        obj->board_count++;
        tle9879_boardD.boardnr = obj->board_count;
    }
    else
    {
        DEBUG_PRINTS("Board D not avialable\r\n");
    }
	
    DEBUG_PRINTS("Expected %u boards; found %u\r\n", *numboards, obj->board_count);
    *numboards = obj->board_count;
	DEBUG_PRINTS("TLE9879 boards intialized\r\n");

}

void ew_tle9879sys_free(ew_tle9879_sys_t *obj)
{
    obj->board_count = 0;
    for(uint8_t index = 0; index < TLE9879_MAX_NUM_BOARDS; index++)
    {
        obj->board[index] = NULL;
    }

    tle9879_boardA.slaveselectpin = NC;
    tle9879_boardB.slaveselectpin = NC;
    tle9879_boardC.slaveselectpin = NC;
    tle9879_boardD.slaveselectpin = NC;

    tle9879_boardA.board_available = false;
    tle9879_boardB.board_available = false;
    tle9879_boardC.board_available = false;
    tle9879_boardD.board_available = false;

    cyhal_spi_free(&tle9879_spi_obj);
}



// void ew_tle9879sys_processStatusCodes(uint16_t action, uint8_t boardnr)
// {
// 	if(status->code == ERR_NONE) return;
// 	Serial.print(F("WARNING: Board["));
// 	Serial.print(boardnr);
// 	Serial.print(F("]; Action["));
// 	printAction(action);
// 	Serial.print(F("]; "));
	
// 	switch(status->code)
// 	{
// 		case ERR_STILL_IN_BOOTLOADER:
// 			Serial.println(F("action cannot be performed in bootloader"));
// 			break;
// 		case ERR_CHECKSUM_IS_WRONG:
// 			Serial.print(F("checksum does not match -> calc: "));
// 			Serial.print(status->additionalInfo[0]);
// 			Serial.print(F("; recv: "));
// 			Serial.println(status->additionalInfo[1]);
// 			break;
// 		case ERR_NOT_AVAILABLE:
// 			Serial.println(F("not available"));
// 			break;
// 		case ERR_MODE_READING_FAILED:
// 			Serial.println(F("mode could not be read"));
// 			break;
// 		case ERR_MODE_CHANGE_FAILED:
// 			Serial.print(F("failed -> from "));
// 			Serial.print(Board::modenames[status->additionalInfo[0]]);
// 			Serial.print(F(" to "));
// 			Serial.print(Board::modenames[status->additionalInfo[1]]);
// 			Serial.print(F("; Answer: "));
// 			Serial.println(status->additionalInfo[2], HEX);
// 			break;
// 		case ERR_INVALID_DATASET_NUMBER:
// 			Serial.print(F("the dataset number was invalid -> "));
// 			Serial.println(status->additionalInfo[0]);
// 			break;
// 		case ERR_INVALID_PARAMETER_INDEX:
// 			Serial.print(F("parameter index is invalid -> "));
// 			Serial.print(Board::modenames[status->additionalInfo[0]]);
// 			Serial.print(F("; "));
// 			Serial.println(status->additionalInfo[1]);
// 			break;
// 		case ERR_FAILED:
// 			Serial.println(F("failed"));
// 			break;
// 		case ERR_INVALID_PARAMETER:
// 			Serial.print(F("the given parameter is invalid -> "));
// 			Serial.println(status->additionalInfo[0]);
// 			break;
// 		case ERR_INVALID_BOARD_NR:
// 			Serial.print(F("the given boardnr is invalid ->"));
// 			Serial.println(boardnr);
// 			break;
// 		case ERR_FAILED_ANSWER:
// 			Serial.print(F("shield failed to confirm successful command -> answer: 0x"));
// 			Serial.print(status->additionalInfo[0], HEX);
// 			Serial.print(F(", expected: 0x"));
// 			Serial.println(status->additionalInfo[1], HEX);
// 			break;
// 		default:
// 			Serial.print(F("Error code is not available ->"));
// 			Serial.println(status->code);
// 			break;
// 	}
	
// 	status->code = ERR_NONE;
// }

// void ew_tle9879sys_printAction(uint16_t action)
// {
// 	switch(action)
// 	{
// 		case MODECONTROL: Serial.print(F("mode control")); break;
// 		case LOADDATASET: Serial.print(F("load dataset")); break;
// 		case READDATASET: Serial.print(F("read dataset")); break;
// 		case WRITEDATASET: Serial.print(F("write dataset")); break;
// 		case CHANGEPARAMETER: Serial.print(F("change parameter")); break;
// 		case SAVEDATASET: Serial.print(F("save dataset")); break;
// 		case SETMOTORSPEED: Serial.print(F("set motor speed")); break;
// 		case MOTORCONTROL: Serial.print(F("motor control")); break;
// 		case BOARDCONTROL: Serial.print(F("board control")); break;
// 		case COPYDATASET: Serial.print(F("copy dataset")); break;
// 		case LED: Serial.print(F("LED control")); break;
// 		default: break;
// 	}
// }

