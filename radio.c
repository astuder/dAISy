/*
 * Library to control Silicon Laboratories SI6342 radio
 * Author: Adrian Studer
 */

#include <msp430.h>
#include <inttypes.h>

#include "radio.h"
#include "radio_config.h"
//#include "radio_config_Si4362 with CRC.h"
#include "spi.h"

#define	SPI_NSEL	BIT4						// chip select on pin 1.4
#define SPI_ON		P1OUT &= ~SPI_NSEL;			// turn SPI on (NSEL=0)
#define SPI_OFF		P1OUT |= SPI_NSEL;			// turn SPI off (NSEL=1)

#define GPIO_0		BIT0	// 2.0 configurable, e.g. sync word, high when detected
#define GPIO_1		BIT1	// 2.1 configurable, default is CTS - this library relies on this!
#define GPIO_2		BIT2	// 2.2 configurable, e.g. RX data clock
#define GPIO_3		BIT3	// 2.3 configurable, e.g. RX data
#define SDN			BIT4	// 2.4 chip shutdown, set high for 1us to reset radio, pulled low by 100k resistor
#define NIRQ		BIT5	// 2.5 condifurable, e.g. preamble, high when detected (for debug only, use sync word for actual package detection)

#define CTS			GPIO_1	// when low, chip is busy/not ready

#define RADIO_READY			(P2IN & CTS)

#define CMD_NOP						0x00
#define CMD_PART_INFO				0x01
#define CMD_FUNC_INFO				0x10
#define CMD_POWER_UP				0x12
#define CMD_FIFO_INFO				0x15
#define CMD_GET_INT_STATUS			0x20
#define CMD_GET_PH_STATUS			0x21
#define CMD_GET_MODEM_STATUS		0x22
#define CMD_GET_CHIP_STATUS			0x23
#define CMD_START_RX				0x32
#define CMD_REQUEST_DEVICE_STATE	0x33
#define CMD_CHANGE_STATE			0x34
#define CMD_READ_CMD_BUFF			0x44
#define CMD_READ_RX_FIFO			0x77

const uint8_t radio_configuration[] = RADIO_CONFIGURATION_DATA_ARRAY;

union radio_buffer_u radio_buffer;

void send_command(uint8_t cmd, const uint8_t *send_buffer, uint8_t send_length, uint8_t response_length);
int receive_result(uint8_t length);

void radio_setup(void)
{
	// initialize SPI pins
	P1SEL &= ~SPI_NSEL;								// NSEL pin as I/O
	P1DIR |= SPI_NSEL;								// set NSEL pin to output
	SPI_OFF;										// turn off chip select

	spi_init();

	// initialize CTS pin as input
	P2SEL &= ~(CTS);
	P2DIR &= ~(CTS);

	// initialize shutdown pin
	P2SEL &= ~SDN;									// shutdown pin is I/O
	P2DIR |= SDN;									// shutdown pin is output

	return;
}

void radio_configure(void)
{
	// reset radio: SDN=1, wait >1us, SDN=0
	P2OUT |= SDN;
	_delay_cycles(1000);
	P2OUT &= ~SDN;

	while (!RADIO_READY);						// wait for chip to wake up

	// transfer radio configuration
	const uint8_t *cfg = radio_configuration;
	while (*cfg)	{							// configuration array stops with 0
		char count = (*cfg++) - 1;				// 1st byte: number of bytes, incl. command
		char cmd = *cfg++;						// 2nd byte: command
		send_command(cmd, cfg, count, 0);		// send bytes to chip
		cfg += count;							// point at next line
		while (!RADIO_READY);					// wait for chip to complete operation
	}

	return;
}

void radio_part_info(void)
{
	send_command(CMD_PART_INFO, 0, 0, sizeof(radio_buffer.part_info));
}

void radio_func_info(void)
{
	send_command(CMD_FUNC_INFO, 0, 0, sizeof(radio_buffer.func_info));
}

void radio_fifo_info(uint8_t reset_fifo)
{
	radio_buffer.data[0] = reset_fifo;
	send_command(CMD_FIFO_INFO, radio_buffer.data, 1, sizeof(radio_buffer.fifo_info));
}

void radio_get_int_status(uint8_t ph_clr_pending, uint8_t modem_clr_pending, uint8_t chip_clr_pending)
{
	radio_buffer.data[0] = ph_clr_pending;
	radio_buffer.data[1] = modem_clr_pending;
	radio_buffer.data[2] = chip_clr_pending;
	send_command(CMD_GET_INT_STATUS, radio_buffer.data, 3, sizeof(radio_buffer.int_status));
}

void radio_get_ph_status(uint8_t clr_pending)
{
	radio_buffer.data[0] = clr_pending;
	send_command(CMD_GET_PH_STATUS, radio_buffer.data, 1, sizeof(radio_buffer.ph_status));
}

void radio_get_chip_status(uint8_t clr_pending)
{
	radio_buffer.data[0] = clr_pending;
	send_command(CMD_GET_CHIP_STATUS, radio_buffer.data, 1, sizeof(radio_buffer.chip_status));
}

void radio_get_modem_status(uint8_t clr_pending)
{
	radio_buffer.data[0] = clr_pending;
	send_command(CMD_GET_MODEM_STATUS, radio_buffer.data, 1, sizeof(radio_buffer.modem_status));
}

void radio_start_rx(uint8_t channel, uint8_t start_condition, uint16_t rx_length, uint8_t rx_timeout_state,	uint8_t rx_valid_state,	uint8_t rx_invalid_state)
{
	radio_buffer.data[0] = channel;
	radio_buffer.data[1] = start_condition;
	radio_buffer.data[2] = rx_length >> 8;
	radio_buffer.data[3] = rx_length & 0xff;
	radio_buffer.data[4] = rx_timeout_state;
	radio_buffer.data[5] = rx_valid_state;
	radio_buffer.data[6] = rx_invalid_state;
	send_command(CMD_START_RX, radio_buffer.data, 7, 0);
}

void radio_request_device_state(void)
{
	send_command(CMD_REQUEST_DEVICE_STATE, 0, 0, sizeof(radio_buffer.device_state));
}

void radio_change_state(uint8_t next_state)
{
	radio_buffer.data[0] = next_state;
	send_command(CMD_CHANGE_STATE, radio_buffer.data, 1, 0);
}

uint8_t radio_read_rx_fifo(void)
{
	uint8_t fifo_count = 0;

	radio_fifo_info(0);
	fifo_count = radio_buffer.fifo_info.rx_fifo_count;

	if (fifo_count)	{

		SPI_ON;
		spi_transfer(CMD_READ_RX_FIFO);						// send command to read FIFO

		uint8_t i = 0;
		while (i < fifo_count) {
			radio_buffer.data[i] = spi_transfer(0);			// receive response
			i++;
		}

		SPI_OFF;
	}

	return fifo_count;
}

void radio_debug(void)
{
	radio_get_int_status(0, 0, 0);
	radio_get_chip_status(0);
	radio_get_modem_status(0);
	radio_part_info();
	radio_func_info();
	radio_request_device_state();
}

// send command, including optional parameters if sendBuffer != 0
void send_command(uint8_t cmd, const uint8_t *send_buffer, uint8_t send_length, uint8_t response_length)
{
	SPI_ON;

	spi_transfer(cmd);						// transmit command

	if (send_length && send_buffer) {		// if there are parameters to send, do so
		uint8_t c = 0;
		while (c != send_length) {
			spi_transfer(send_buffer[c]);	// transmit byte
			c++;
		}
	}

	SPI_OFF;

	while (!RADIO_READY);					// always wait for completion

	if (response_length) {
		while (receive_result(response_length) == 0);
	}

	return;
}

// read result: write 44h, read CTS byte, if 0xff read result bytes, else loop (cycle NSEL)
int receive_result(uint8_t length)
{
	SPI_ON;

	spi_transfer(CMD_READ_CMD_BUFF);		// send 0x44 to receive data/CTS
	if(spi_transfer(0) != 0xff)	{			// if CTS is not 0xff
		SPI_OFF;
		return 0;							// data not ready yet
	}

	uint8_t i = 0;							// data ready, read result into buffer
	while (i < length) {
		radio_buffer.data[i] = spi_transfer(0);		// receive byte
		i++;
	}

	SPI_OFF;

	return 1;						// data received
}

