#include "cybsp.h"
#include "TLE9879_Group.h"


#define TLE9879_SPI_SELECT_ENABLE   0
#define TLE9879_SPI_SELECT_DISABLE  1

#define TLE9879_MAX_NUM_BOARDS  4

typedef struct{
    cyhal_spi_t* tle9879_spi;
    uint8_t boardnr;
	cyhal_gpio_t slaveselectpin;
	uint8_t currentmode;
	bool  board_available;
	
	int16_t motorspeed;
	StatusInfo status;

	data_FOC_union data_FOC;
	data_BEMF_union data_BEMF;
	data_HALL_union data_HALL;
}ew_tle9879_board_t;

typedef struct{
    ew_tle9879_board_t* board[TLE9879_MAX_NUM_BOARDS];
    uint8_t board_count;
}ew_tle9879_sys_t;

void ew_tle9879sys_init(ew_tle9879_sys_t *obj, cyhal_gpio_t mosi, cyhal_gpio_t miso, cyhal_gpio_t sclk, const cyhal_clock_t *clk, 
                    cyhal_gpio_t ss1, cyhal_gpio_t ss2, cyhal_gpio_t ss3, cyhal_gpio_t ss4, uint8_t* numboards);

void ew_tle9879sys_free(ew_tle9879_sys_t *obj);

void ew_tle9879sys_setMode(ew_tle9879_sys_t *obj, uint8_t mode, uint8_t boardnr, bool fastMode);
void ew_tle9879sys_setMotorMode(ew_tle9879_sys_t *obj, uint8_t mode, uint8_t boardnr);
void ew_tle9879sys_setMotorSpeed(ew_tle9879_sys_t *obj, float motorspeed, uint8_t boardnr);
void ew_tle9879sys_setLed(ew_tle9879_sys_t *obj, uint16_t led, uint16_t mode, uint8_t boardnr);
void ew_tle9879sys_setLedColor(ew_tle9879_sys_t *obj, uint8_t color, uint8_t boardnr);
