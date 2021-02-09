
#include "cyhal.h"
#include "ew_tle9879_system.h"
#include "ew_tle9879_board_driver.h"

//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
//code released under the therms of the GNU GPL 3.0 license
//at http://www.leonardomiliani.com/en/2013/un-semplice-crc8-per-arduino/
uint8_t ew_tle9879_CRC8(uint8_t *data, uint8_t len)
{
	uint8_t crc = 0x55;
	while (len--)
	{
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--)
		{
			uint8_t sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum)
			{
				crc ^= 0x8C;
			}
			extract >>= 1;
		}
	}
	return crc;
}

bool ew_tle9879_checkuint16val(uint8_t mode, uint8_t index, uint16_t *data)
{
	switch(mode)
	{
		case BEMF:
			switch(index)
			{
				case BEMF_START_FREQ_ZERO: // 1/0
					if(*data > 0)
					{
						*data = 1; 
					}
					break;
				case BEMF_SPEED_TEST_ENABLE: // 1/0
					if(*data > 0)
					{
						*data = 1; 
					}
					break;
				case BEMF_SPIKE_FILT: // code 1/2/4/8
					switch (*data)
					{
						case 1: *data = 0; break;
						case 2: *data = 1; break;
						case 4: *data = 2; break;
						case 8: *data = 3; break;
						default:							
							DEBUG_PRINTS("Invalid value '%u' for BEMF_SPIKE_FILT. Valid values are: 1, 2, 4, 8", (unsigned int) data);
							return false;
					}
					break;
				case BEMF_BLANK_FILT: // code 3/6/8/12/16
					switch (*data)
					{
						case 3: *data = 4; break;
						case 6: *data = 0; break;
						case 8: *data = 1; break;
						case 12: *data = 2; break;
						case 16: *data = 3; break;
						default:
							DEBUG_PRINTS("Invalid value '%u' for BEMF_BLANK_FILT. Valid values are: 3, 6, 8, 12, 16", (unsigned int) data);
							return false;
					}
					break;
			}
			break;
		case HALL:
			switch(index)
			{
				case HALL_INIT_DUTY: // 0-100
					if((*data < 0) || (*data > 100))
					{
						DEBUG_PRINTS("Invalid value '%u' for HALL_INIT_DUTY. Valid values are: 0-100", (unsigned int) data);
						return false;
					}
					break;
				case HALL_INPUT_A: // 0/1/2
					if(*data > 2)
					{
						DEBUG_PRINTS("Invalid value '%u' for HALL_INPUT_A. Valid values are: 0/1/2", (unsigned int) data);
						return false;
					}
					break;
				case HALL_INPUT_B: // 0/1/2
					if(*data > 2)
					{
						DEBUG_PRINTS("Invalid value '%u' for HALL_INPUT_B. Valid values are: 0/1/2", (unsigned int) data);
						return false;
					}
					break;
				case HALL_INPUT_C: // 0/1/2
					if(*data > 2)
					{
						DEBUG_PRINTS("Invalid value '%u' for HALL_INPUT_C. Valid values are: 0/1/2", (unsigned int) data);
						return false;
					}
					break;
				case HALL_OFFSET_60DEGREE_EN: // 0/1
					if(*data > 1)
					{
						*data = 1;
					}
					break;
				case HALL_ANGLE_DELAY_EN: // 0/1
					if(*data > 1)
					{
						*data = 1; 
					}
					break;
				case HALL_DELAY_ANGLE: // 0-59
					if(*data > 59)
					{
						DEBUG_PRINTS("Invalid value '%u' for HALL_DELAY_ANGLE. Valid values are: 0-59", (unsigned int) data);
						return false;
					}
					break;
				case HALL_DELAY_MINSPEED: // 0-2000
					if(*data > 2000)
					{
						DEBUG_PRINTS("Invalid value '%u' for HALL_DELAY_MINSPEED. Valid values are: 0-2000", (unsigned int) data);
						return false;
					}
					break;
			}
			break;
		case FOC:
			switch(index)
			{
				case FOC_SPEED_KP: // >0
					if(*data == 0)
					{
						DEBUG_PRINTS("Invalid value '%u' for FOC_SPEED_KP. Valid values are: >0", (unsigned int) data);
						return false;
					}
					break;
				case FOC_SPEED_KI: // >0
					if(*data == 0)
					{
						DEBUG_PRINTS("Invalid value %u' for FOC_SPEED_KI. Valid values are: >0'", (unsigned int) data);
						return false;
					}
					break;
				case FOC_START_FREQ_ZERO: // 0/1
					if(*data > 1)
					{
						*data = 1; 
					}
					break;
			}
			break;
	}
	return true;
}

bool ew_tle9879_checkfloatval(uint8_t mode, uint8_t index, float *data)
{
	switch(mode)
	{
		case BEMF:
			switch(index)
			{
				case BEMF_TIME_CONST_SPEED_FILT_TIME: // 0.01-1
					if((*data < 0.01) || (*data > 1))
					{
						DEBUG_PRINTS("Invalid value '%f' for BEMF_SPEED_FILT_TIME. Valid values are: 0.01-1.", (float) *data);
						return false;
					}
					break;
				case BEMF_START_SPEED_PWM_MIN: // 0-0.95
					if((*data < 0) || (*data > 0.95))
					{
						DEBUG_PRINTS("Invalid value '%f' for BEMF_START_SPEED_PWM_MIN_OFFSET. Valid values are: 0-0.95.", (float) *data);
						return false;
					}
					break;
				case BEMF_START_SPEED_PWM_MIN_OFFSET: // 0-0.95
					if((*data < 0) || (*data > 0.95))
					{
						DEBUG_PRINTS("Invalid value '%f' for BEMF_START_SPEED_PWM_MIN_OFFSET. Valid values are: 0-0.95.", (float) *data);
						return false;
					}
					break;
				case BEMF_SPEED_BEGIN_PWM_MIN: // 0-0.95
					if((*data < 0) || (*data > 0.95))
					{
						DEBUG_PRINTS("Invalid value '%f' for BEMF_RUN_SPEED_PWM_MIN. Valid values are: 0-0.95.", (float) *data);
						return false;
					}
					break;
				case BEMF_SPEED_PWM_MIN: // 0-0.95
					if((*data < 0) || (*data > 0.95))
					{
						DEBUG_PRINTS("Invalid value '%f' for BEMF_END_SPEED_PWM_MIN. Valid values are: 0-0.95.", (float) *data);
						return false;
					}
					break;
			}
			break;

		case HALL:
			switch(index)
			{
				case HALL_SPEED_IMIN: // 0-100
					if((*data < 0) || (*data > 100))
					{
						DEBUG_PRINTS("Invalid value '%f' for HALL_SPEED_IMIN. Valid values are: 0-100.", (float) *data);
						return false;
					}
					break;
				case HALL_SPEED_IMAX: // 0-100
					if((*data < 0) || (*data > 100))
					{
						DEBUG_PRINTS("Invalid value '%f' for HALL_SPEED_IMAX. Valid values are: 0-100.", (float) *data);
						return false;
					}
					break;
				case HALL_SPEED_PIMIN: // 0-100
					if((*data < 0) || (*data > 100))
					{
						DEBUG_PRINTS("Invalid value '%f' for HALL_SPEED_PIMIN. Valid values are: 0-100.", (float) *data);
						return false;
					}
					break;
				case HALL_SPEED_PIMAX: // 0-100
					if((*data < 0) || (*data > 100))
					{
						DEBUG_PRINTS("Invalid value '%f' for HALL_SPEED_PIMAX. Valid values are: 0-100.", (float) *data);
						return false;
					}
					break;
			}
			break;

		case FOC:
			switch(index)
			{
				case FOC_CUR_ADJUST: // 0.01-1
					if((*data < 0.01) || (*data > 1))
					{
						DEBUG_PRINTS("Invalid value '%f' for FOC_CUR_ADJUST. Valid values are: 0.01-1", (float) *data);
						return false;
					}
					break;
				case FOC_FLUX_ADJUST: // 0.01-1
					if((*data < 0.01) || (*data > 1))
					{
						DEBUG_PRINTS("Invalid value '%f' for FOC_FLUX_ADJUST. Valid values are: 0.01-1", (float) *data);
						return false;
					}
					break;
				case FOC_SPEED_FILT_TIME: // 0.01-1
					if((*data < 0.01) || (*data > 1))
					{
						DEBUG_PRINTS("Invalid value '%f' for FOC_SPEED_FILT_TIME. Valid values are: 0.01-1", (float) *data);
						return false;
					}
					break;
				case FOC_MIN_NEG_REF_CUR: // <0
					if(*data > 0)
					{
						DEBUG_PRINTS("Invalid value '%f' for FOC_MIN_NEG_REF_CUR. Valid values are: <0", (float) *data);
						return false;
					}
					break;
				case FOC_MIN_CUR_SPEED: // <0
					if(*data > 0)
					{
						DEBUG_PRINTS("Invalid value '%f' for FOC_MIN_CUR_SPEED. Valid values are: <0", (float) *data);
						return false;
					}
					break;
				case FOC_MAX_NEG_REF_CUR: // <0
					if(*data > 0)
					{
						DEBUG_PRINTS("Invalid value '%f' for FOC_MAX_NEG_REF_CUR. Valid values are: <0", (float) *data);
						return false;
					}
					break;
			}
			break;
	}
	return true;
}

uint8_t ew_tle9879_isvalueinarray(uint8_t val, const uint8_t *indices_16bit_BEMF, const uint8_t size)
{
    int i;
    for (i=0; i < size; i++)
	{
        if (indices_16bit_BEMF[i] == val)
		{
			return 1;
		}
    }
    return 0;
}




// functions for communication
void ew_tle9879_sendMessage(ew_tle9879_board_t* obj, uint16_t data)
{

	uint8_t txdata[2];
	uint8_t rxdata[2];

	txdata[0] = (uint8_t) ((data & 0xFF00) >> 8);
	txdata[1] = (uint8_t) (data & 0x00FF);

	cyhal_gpio_write(obj->slaveselectpin, TLE9879_SPI_SELECT_ENABLE);  // take the SS pin low to select board
    cyhal_system_delay_us(100);
   	cyhal_spi_transfer(obj->tle9879_spi, (const uint8_t *) txdata, 2, rxdata, 2, 0x00);
   	cyhal_system_delay_us(100);
   	cyhal_gpio_write(obj->slaveselectpin, TLE9879_SPI_SELECT_DISABLE);  // take the SS pin high to de-select board
   	cyhal_system_delay_us(100);
}

uint16_t ew_tle9879_readAnswer(ew_tle9879_board_t* obj)
{
	uint8_t rxd[2];
	uint16_t receivedMessage;
	const uint8_t emptytx[2] = {0, 0};

	cyhal_system_delay_us(100);
	cyhal_gpio_write(obj->slaveselectpin, TLE9879_SPI_SELECT_ENABLE);
	cyhal_system_delay_us(100);
    cyhal_spi_transfer(obj->tle9879_spi, emptytx, 2, rxd, 2, 0x00);
	cyhal_system_delay_us(100);
	cyhal_gpio_write(obj->slaveselectpin, TLE9879_SPI_SELECT_DISABLE);

	receivedMessage = ((uint16_t) (rxd[0] << 8)) + (uint16_t) rxd[1];
	return receivedMessage;
}

bool ew_tle9879_sendMessageAndCheckAnswer(ew_tle9879_board_t* obj, uint16_t command)
{
	ew_tle9879_sendMessage(obj, command);
	cyhal_system_delay_us(100);
	uint16_t answer = ew_tle9879_readAnswer(obj);

	if(answer == (command + CONFIRM_OFFSET)) return true;
	else
	{
		DEBUG_PRINTS("WARNING: Board[%u]; Command[0x%02X]; Answer[0x%02X]\r\n", obj->boardnr, command, answer);
		return false;
	}
}

bool ew_tle9879_isAvailable(ew_tle9879_board_t* obj)
{
	return obj->board_available;
}

// commands
// 0x01: modeControl
bool ew_tle9879_modeControl(ew_tle9879_board_t* obj, uint8_t requestedmode)
{	
	// is the board available
	if(!ew_tle9879_isAvailable(obj)) return false;

	// get current mode
	if(requestedmode == GETCURRENTMODE)
	{
		ew_tle9879_sendMessage(obj, MODECONTROL + GETCURRENTMODE);
		uint16_t answer = ew_tle9879_readAnswer(obj);

		if((answer == 0x1110) || (answer == 0x1111) || (answer == 0x1112) || (answer == 0x1113))
		{
			uint8_t answeredmode = (uint8_t)(answer & 0x000F);
			obj->currentmode = answeredmode;
			return true;
		}
		else
		{
			obj->status.code = ERR_MODE_READING_FAILED;
			return false;
		}
	}

	// already in the requested mode
	if(requestedmode == obj->currentmode) return true;
	
	// requested mode is not valid
	if(requestedmode > 3)
	{
		obj->status.code = ERR_INVALID_PARAMETER;
		obj->status.additionalInfo[0] = (uint16_t) requestedmode;
		return false;
	}

	// send mode change command
	ew_tle9879_sendMessage(obj, MODECONTROL + requestedmode);
	// wait while slave is processing modeControl
	
	cyhal_system_delay_ms(1000);
	uint16_t answer = ew_tle9879_readAnswer(obj);
	if(answer != (MODECONTROL + requestedmode + CONFIRM_OFFSET))
	{		
		// try to read current mode from TLE9879_Board
		cyhal_system_delay_ms(1000);
		ew_tle9879_sendMessage(obj, MODECONTROL + GETCURRENTMODE);
		cyhal_system_delay_ms(100);
		uint16_t answer = ew_tle9879_readAnswer(obj);
		uint16_t successanswer = MODECONTROL + GETCURRENTMODE + requestedmode + CONFIRM_OFFSET;

		// mode change failed
		if(answer != successanswer)
		{
			obj->status.code = ERR_MODE_CHANGE_FAILED;
			obj->status.additionalInfo[0] = (uint16_t) obj->currentmode;
			obj->status.additionalInfo[1] = (uint16_t) requestedmode;
			obj->status.additionalInfo[2] = (uint16_t) answer;
			return false;
		}
	}
	
	obj->currentmode = requestedmode;
	return true;
}

// 0x02: loadDataset
bool ew_tle9879_loadDataset(ew_tle9879_board_t* obj, uint8_t datasetnr)
{
	if(!ew_tle9879_isAvailable(obj)) return false;
	if(obj->currentmode == BOOTLOADER)
	{
		obj->status.code = ERR_STILL_IN_BOOTLOADER;
		return false;
	}
	if(((datasetnr > 3) && (datasetnr < 0x10)) || (datasetnr > 0x13))
	{
		obj->status.code = ERR_INVALID_DATASET_NUMBER;
		obj->status.additionalInfo[0] = datasetnr;
		return false;
	}
	bool success = ew_tle9879_sendMessageAndCheckAnswer(obj, LOADDATASET + datasetnr);
	if(!success)
	{
		obj->status.code = ERR_FAILED;
		obj->status.additionalInfo[0] = datasetnr;
		return false;
	}
	return true;
}

// 0x03: readDataset
uint8_t ew_tle9879_readDataset(ew_tle9879_board_t* obj)
{
	if(!ew_tle9879_isAvailable(obj)) return 0;

	// find out mode to decide which dataset needs to be read
	bool success = ew_tle9879_modeControl(obj, GETCURRENTMODE);
	if(!success)
	{
		obj->status.code = ERR_MODE_READING_FAILED;
		return 0;
	}

	uint8_t calcCRC, recvCRC;
	uint16_t nrofmessages;
	uint8_t *uint8ptr;

	switch(obj->currentmode)
	{
		case BOOTLOADER:
			obj->status.code = ERR_STILL_IN_BOOTLOADER;
			return 0;

		case BEMF:			
			ew_tle9879_sendMessage(obj, READDATASET);
			nrofmessages = ew_tle9879_readAnswer(obj);

			for(uint8_t i = 0; i < nrofmessages; i++)
			{
				obj->data_BEMF.dataarray_BEMF[i] = ew_tle9879_readAnswer(obj);
			}

			uint8ptr = (uint8_t*) &(obj->data_BEMF);
			calcCRC = ew_tle9879_CRC8(uint8ptr, NUMBEROF_BYTES_BEMF);
			recvCRC = (uint8_t)ew_tle9879_readAnswer(obj);
			break;

		case HALL:
			ew_tle9879_sendMessage(obj, READDATASET);
			nrofmessages = ew_tle9879_readAnswer(obj);

			for(uint8_t i = 0; i < nrofmessages; i++)
			{
				obj->data_HALL.dataarray_HALL[i] = ew_tle9879_readAnswer(obj);
			}

			uint8ptr = (uint8_t*) &(obj->data_HALL);
			calcCRC = ew_tle9879_CRC8(uint8ptr, NUMBEROF_BYTES_HALL);
			recvCRC = (uint8_t)ew_tle9879_readAnswer(obj);
			break;

		case FOC:
			ew_tle9879_sendMessage(obj, READDATASET);
			nrofmessages = ew_tle9879_readAnswer(obj);
			ew_tle9879_readAnswer(obj);

			for(uint8_t i = 0; i < nrofmessages; i++)
			{
				obj->data_FOC.dataarray_FOC[i] = ew_tle9879_readAnswer(obj);
			}

			uint8ptr = (uint8_t*) &(obj->data_FOC);
			calcCRC = ew_tle9879_CRC8(uint8ptr, NUMBEROF_BYTES_FOC);
			recvCRC = (uint8_t) ew_tle9879_readAnswer(obj);
			break;
			
		default:
			return 0;
	}
	
	if(calcCRC != recvCRC)
	{
		obj->status.code = ERR_CHECKSUM_IS_WRONG;
		obj->status.additionalInfo[0] = calcCRC;
		obj->status.additionalInfo[1] = recvCRC;
		return 0;
	}
	return 3;
}

// 0x04: writeDataset
bool ew_tle9879_writeDataset(ew_tle9879_board_t* obj)
{
	uint8_t nrofmessages = 0;
	uint8_t calcCRC = 0;
	uint8_t *uint8ptr;

	if(!ew_tle9879_isAvailable(obj)) return false;

	bool success = ew_tle9879_modeControl(obj, GETCURRENTMODE);
	if(!success)
	{
		obj->status.code = ERR_MODE_READING_FAILED;
		return false;
	}

	switch(obj->currentmode)
	{
		case BOOTLOADER:
			obj->status.code = ERR_STILL_IN_BOOTLOADER;
			return false;
			break;
			
		case BEMF:
			nrofmessages = NUMBEROF_MESSAGES_BEMF;
			uint8ptr = (uint8_t*) &(obj->data_BEMF.dataarray_BEMF);
			calcCRC = ew_tle9879_CRC8(uint8ptr, NUMBEROF_BYTES_BEMF);
			ew_tle9879_sendMessage(obj, WRITEDATASET + nrofmessages);
			for(int i = 0; i < nrofmessages; i++) 
			{
				ew_tle9879_sendMessage(obj, obj->data_BEMF.dataarray_BEMF[i]);
			}
			break;
			
		case HALL:
			nrofmessages = NUMBEROF_MESSAGES_HALL;
			uint8ptr = (uint8_t*) &(obj->data_HALL.dataarray_HALL);
			calcCRC = ew_tle9879_CRC8(uint8ptr, NUMBEROF_BYTES_HALL);
			ew_tle9879_sendMessage(obj, WRITEDATASET + nrofmessages);
			for(int i = 0; i < nrofmessages; i++)
			{
				ew_tle9879_sendMessage(obj, obj->data_HALL.dataarray_HALL[i]);
			}
			break;

		case FOC:
			nrofmessages = NUMBEROF_MESSAGES_FOC;
			uint8ptr = (uint8_t*) &(obj->data_FOC.dataarray_FOC);
			calcCRC = ew_tle9879_CRC8(uint8ptr, NUMBEROF_BYTES_FOC);
			ew_tle9879_sendMessage(obj, WRITEDATASET + nrofmessages);
			for(int i = 0; i < nrofmessages; i++)
			{
				ew_tle9879_sendMessage(obj, obj->data_FOC.dataarray_FOC[i]);
			}
			break;
	}

	ew_tle9879_sendMessage(obj, calcCRC);
	cyhal_system_delay_ms(10);
	uint16_t finalmessage = ew_tle9879_readAnswer(obj);

	if(finalmessage != (WRITEDATASET + nrofmessages + CONFIRM_OFFSET))
	{
		obj->status.code = ERR_FAILED_ANSWER;
		obj->status.additionalInfo[0] = finalmessage;
		obj->status.additionalInfo[1] = WRITEDATASET + nrofmessages + CONFIRM_OFFSET;
		return false;
	}
	else return true;
}

// 0x05: changeParameter
bool ew_tle9879_changeParameter(ew_tle9879_board_t*obj, uint8_t index, float data)
{
	if(!ew_tle9879_isAvailable(obj)) return false;

	bool datacheckOK = false;
	bool dataIsUint16 = false;
	bool success = ew_tle9879_modeControl(obj, GETCURRENTMODE);
	if(!success)
	{
		obj->status.code = ERR_MODE_READING_FAILED;
		return false;
	}

	switch(obj->currentmode)
	{
		case BOOTLOADER:
			obj->status.code = ERR_STILL_IN_BOOTLOADER;
			return false;
			
		case BEMF:
			// no offset at index
			if(index >= HALLOFFSET)
			{
				obj->status.code = ERR_INVALID_PARAMETER_INDEX;
				obj->status.additionalInfo[0] = (uint16_t) BEMF;
				obj->status.additionalInfo[1] = (uint16_t) index;
				return false;
			}

			switch(index)
			{
				case BEMF_POLE_PAIRS:
				case BEMF_SPEED_KP:
				case BEMF_SPEED_KI:
				case BEMF_SPEED_TEST_ENABLE:
				case BEMF_START_FREQ_ZERO:
				case BEMF_SPIKE_FILT:
				case BEMF_BLANK_FILT:
					dataIsUint16 = true;
					break;

				default:
					dataIsUint16 = false;
					break;
				break;
			}
			
		case HALL:
			// offset between HALLOFFSER and FOCOFFSET at index
			if((index < HALLOFFSET) || index > FOCOFFSET)
			{
				obj->status.code = ERR_INVALID_PARAMETER_INDEX;
				obj->status.additionalInfo[0] = (uint16_t) HALL;
				obj->status.additionalInfo[1] = (uint16_t) index;
				return false;
			}
			index -= HALLOFFSET;
			// dataIsUint16 = ew_tle9879_isvalueinarray(index, indices_16bit_HALL, indices_16bit_HALL_size);
			break;
			
		case FOC:
			// offset FOCOFFSET at index
			if(index < FOCOFFSET)
			{
				obj->status.code = ERR_INVALID_PARAMETER_INDEX;
				obj->status.additionalInfo[0] = (uint16_t) FOC;
				obj->status.additionalInfo[1] = (uint16_t) index;
				return false;
			}
			index -= FOCOFFSET;
			// dataIsUint16 = ew_tle9879_isvalueinarray(index, indices_16bit_FOC, indices_16bit_FOC_size);
			break;
	}

	if (dataIsUint16) // 16bit variable
	{
		uint16_t uint16val = (uint16_t)(data);
		datacheckOK = ew_tle9879_checkuint16val(obj->currentmode, index, &uint16val);
		if(datacheckOK)
		{
			ew_tle9879_sendMessage(obj, CHANGEPARAMETER + index);
			ew_tle9879_sendMessage(obj, (uint16_t)uint16val);
		}
		else return false;
	}
	else
	{
		uint32_t *uint32ptr = (uint32_t*) &data;
		datacheckOK = ew_tle9879_checkfloatval(obj->currentmode, index, &data);
		if(datacheckOK)
		{
			ew_tle9879_sendMessage(obj, CHANGEPARAMETER + index);
			ew_tle9879_sendMessage(obj, (uint16_t)((*uint32ptr)>>16));
			ew_tle9879_sendMessage(obj, (uint16_t)(*uint32ptr));
		}
		else return false;
	}

	uint16_t answer = ew_tle9879_readAnswer(obj);
	if(answer != CHANGEPARAMETER + index + CONFIRM_OFFSET)
	{
		obj->status.code = ERR_FAILED;
		return false;
	}
	return true;
}

// 0x06: saveDataset
bool ew_tle9879_saveDataset(ew_tle9879_board_t* obj, uint8_t position)
{
	if(!ew_tle9879_isAvailable(obj)) return false;

	if(obj->currentmode == BOOTLOADER)
	{
		obj->status.code = ERR_STILL_IN_BOOTLOADER;
		return false;
	}

	if(position > 3)
	{
		obj->status.code = ERR_INVALID_DATASET_NUMBER;
		obj->status.additionalInfo[0] = position;
		return false;
	}

	ew_tle9879_sendMessage(obj, SAVEDATASET + position);
	cyhal_system_delay_ms(10);
	uint16_t answer = ew_tle9879_readAnswer(obj);

	if(answer != SAVEDATASET + position + CONFIRM_OFFSET)
	{
		obj->status.code = ERR_FAILED_ANSWER;
		obj->status.additionalInfo[0] = answer;
		obj->status.additionalInfo[1] = SAVEDATASET + position + CONFIRM_OFFSET;
		return false;
	}
	return true;
}

// 0x07: setMotorspeed
bool ew_tle9879_setMotorspeed(ew_tle9879_board_t* obj, float motorspeed)
{
	if(!ew_tle9879_isAvailable(obj)) return false;
	if(obj->currentmode == BOOTLOADER)
	{
		obj->status.code = ERR_STILL_IN_BOOTLOADER;
		return false;
	}

	ew_tle9879_sendMessage(obj, SETMOTORSPEED);
	ew_tle9879_sendMessage(obj, (int16_t)motorspeed);
	uint16_t answer = ew_tle9879_readAnswer(obj);

	if(answer != SETMOTORSPEED +CONFIRM_OFFSET)
	{
		obj->status.code = ERR_FAILED;
		return false;
	}
	obj->motorspeed = motorspeed;
	return true;
}

// 0x08: motorControl-
bool ew_tle9879_motorControl(ew_tle9879_board_t* obj, uint8_t command)
{
	if(!ew_tle9879_isAvailable(obj)) return false;
	if(obj->currentmode == BOOTLOADER)
	{
		obj->status.code = ERR_STILL_IN_BOOTLOADER;
		return false;
	}
	if((command != START_MOTOR) && (command != STOP_MOTOR))
	{
		obj->status.code = ERR_INVALID_PARAMETER;
		obj->status.additionalInfo[0] = command;
		return false;
	}
	bool success = ew_tle9879_sendMessageAndCheckAnswer(obj, MOTORCONTROL + command);
	if(!success)
	{
		obj->status.code = ERR_FAILED;
		return false;
	}
	return true;
}

// 0x09: boardControl
bool ew_tle9879_boardControl(ew_tle9879_board_t* obj)
{	
	// board available?
	bool success = ew_tle9879_sendMessageAndCheckAnswer(obj, BOARDCONTROL + BOARD_AVAILABLE);
	obj->board_available = success;
	return success;
}

// 0x0A: LEDOn
bool ew_tle9879_LEDOn(ew_tle9879_board_t* obj, uint8_t led)
{
	if(!ew_tle9879_isAvailable(obj))
		return false;

	if((led != LED_RED) && (led != LED_GREEN) && (led != LED_BLUE))
	{
		DEBUG_PRINTS("Requested LED was not valid. Valid LEDs are: LED_RED, LED_GREEN, LED_BLUE\r\n");
		return false;
	}
	bool success = ew_tle9879_sendMessageAndCheckAnswer(obj, LED_ON + led);	
	return success;
}

// 0x0B: LEDOff
bool ew_tle9879_LEDOff(ew_tle9879_board_t* obj, uint8_t led)
{
	if(!ew_tle9879_isAvailable(obj))
		return false;

	if((led != LED_RED) && (led != LED_GREEN) && (led != LED_BLUE))
	{
		DEBUG_PRINTS("Requested LED was not valid. Valid LEDs are: LED_RED, LED_GREEN, LED_BLUE\r\n");
		return false;
	}
	bool success = ew_tle9879_sendMessageAndCheckAnswer(obj, LED_OFF + led);
	return success;
}

int16_t ew_tle9879_getMotorspeed(ew_tle9879_board_t* obj)
{
	return obj->motorspeed;
}

/*
bool ew_tle9879_reset() // TODO add status messages
{
	motorControl(STOP_MOTOR);
	sendMessage(BOARDCONTROL + RESET);
	return true;
}
*/

uint8_t ew_tle9879_getCurrentMode(ew_tle9879_board_t* obj)
{
	return obj->currentmode;
}
