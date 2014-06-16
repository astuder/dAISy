/*
 * Library to control Silicon Laboratories SI6342 radio
 * Author: Adrian Studer
 * License: CC BY-NC-SA Creative Commons Attribution-NonCommercial-ShareAlike
 * 			http://creativecommons.org/licenses/by-nc-sa/4.0/
 * 			Please contact the author if you want to use this work in a commercial product
 */

#ifndef RADIO_H_
#define RADIO_H_

#define RADIO_PORT 			2
#define RADIO_GPIO_0		BIT0	// 2.0 configurable, e.g. sync word, high when detected
#define RADIO_GPIO_1		BIT1	// 2.1 configurable, default is CTS - this library relies on this!
#define RADIO_GPIO_2		BIT2	// 2.2 configurable, e.g. RX data clock
#define RADIO_GPIO_3		BIT3	// 2.3 configurable, e.g. RX data
#define RADIO_SDN			BIT4	// 2.4 chip shutdown, set high for 1us to reset radio, pulled low by 100k resistor
#define RADIO_NIRQ			BIT5	// 2.5 configurable, e.g. CCA, high when RSSI exceeding threshold

#define RADIO_CTS			RADIO_GPIO_1	// when low, chip is busy/not ready
#ifndef TEST
#define RADIO_READY	(P2IN & RADIO_CTS)
#else
#define RADIO_READY (1)
#endif

#define RADIO_CCA			RADIO_NIRQ		// when high, signal strength exceeds RSSI threshold
#ifndef TEST
#define RADIO_SIGNAL (P2IN & RADIO_CCA)
#else
#define RADIO_SIGNAL (1)
#endif

// convert RSSI to dBm: RSSI / 2 - RSSI_COMP - 70 and vice versa
#define RADIO_RSSI_TO_DBM(val) (((int) val >> 1) - 0x40 - 70)
#define RADIO_DBM_TO_RSSI(val) ((val + 70 + 0x40) << 1)

// functions to start up / reset chip
void radio_setup(void);								// set up MSP430 pins and SPI for interfacing w/ radio
void radio_configure(void);							// configure radio using radio_config_Si4362.h
void radio_calibrate_ir(void);						// run image rejection self-calibration (takes approx. 250ms)

void radio_shutdown(void);							// turn off radio

void radio_wait_for_CTS(void);						// waits until radio completed previous command, e.g. to wait for RX to start

// helpers
void radio_debug(void);								// debug code, reading chip status, version etc.

// wrappers for chip functionality, see EZRadioPRO API Documentation for detailed information
// http://www.silabs.com/products/wireless/ezradiopro/pages/si4362.aspx

void radio_start_rx(								// switch modem to RX state
					uint8_t channel, 					// channel, 0 = configured base frequency
					uint8_t start_condition,			// start condition, 0 = start immediatly, 1 = wait for WUT to expire
					uint16_t rx_length,					// ??? number of bytes or number of packets?
					uint8_t rx_timeout_state,			// next state when no preamble is detected
					uint8_t rx_valid_state,				// next state when valid packet is received
					uint8_t rx_invalid_state);			// next state when invalid packet is received (CRC error)

void radio_change_state(							// change state of radio, e.g. to READY from RX
					uint8_t next_state);				// target state

void radio_fifo_info(								// read FIFO information, like pending bytes, result in radio_buffer.fifo_info
					uint8_t reset_reset_fifo);			// if 2, RX FIFO will be reset

void radio_part_info(void);							// read part information, like part number (4362), result in radio_buffer.part_info
void radio_func_info(void);							// read firmware information, like firmware version, result in radio_buffer.func_info

void radio_get_int_status(							// read interrupt status, result in radio_buffer.int_status
					uint8_t ph_clr_pending,				// 0 = clear pending packet handler interrupts, set bits leave respective int pending
					uint8_t modem_clr_pending,			// 0 = clear pending modem interrupts, set bits leave respective int pending
					uint8_t chip_clr_pending);			// 0 = clear pending chip interrupts, set bits leave respective int pending

void radio_get_ph_status(							// read packet handler status, result in radio_buffer.ph_status
					uint8_t clr_pending);				// 0 = clear pending interrupts, set bits leave respective int pending

void radio_get_chip_status(							// read chip status, including command errors, result in radio_buffer.chip_status
					uint8_t clr_pending);				// 0 = clear pending interrupts, set bits leave respective int pending

void radio_get_modem_status(						// read modem status, including RSSI, result in radio_buffer.modem_status
					uint8_t clr_pending);				// 0 = clear pending interrupts, set bits leave respective int pending

void radio_request_device_state(void);				// read current device state, result in radio_buffer.device_state

void radio_frr_read(								// read fast read registers, results in radio_buffer.data[0..3]
					uint8_t frr,					// start register 'A', 'B', 'C' or 'D'
					uint8_t count);					// number of registers to read (1-4)

void radio_set_property(							// directly set individual radio property
					uint8_t prop_group,				// property group, e.g. 0x20 for MODEM
					uint8_t prop_num,				// property number, e.g. 0x4a for RSSI threshold
					uint8_t value);					// property value, e.g. RADIO_DBM_TO_RSSI(-80)

// data structures of various responses, access via radio_buffer.* after calling respective radio_get_* function

struct part_info_s {
	uint8_t	chiprev;				// chip mask revision
	uint8_t part_msb;				// part number (4362 for Si4362)
	uint8_t part_lsb;
	uint8_t pbuild;					// part build
	uint8_t id_msb;					// part ID
	uint8_t id_lsb;
	uint8_t customer;				// customer ID
	uint8_t romid;					// ROM ID
};

struct func_info_s {
	uint8_t revext;					// external revision number
	uint8_t revbranch;				// branch revision number
	uint8_t revint;					// internal revision number
	uint8_t patch_msb;				// ID of applied patch, 0000 = no patch applied
	uint8_t patch_lsb;
	uint8_t func;					// current functional mode, 0 = in boot mode, 1 = running main application image
};

struct fifo_info_s {
	uint8_t rx_fifo_count;			// bytes waiting in RX FIFO?
};

struct packet_info_s {
	uint8_t length_msb;				// length extracted from packet
	uint8_t length_lsb;
};

struct int_status_s {
	uint8_t int_pend;
	uint8_t int_status;				// 0..2: PH_INT, MODEM_INT, CHIP_INT
	uint8_t ph_pend;
	uint8_t ph_status;				// 0..7: RX_FIFO_ALMOST_FULL, CRC_ERROR, PACKET_RX, FILTER_MISS, FILTER_MATCH
	uint8_t modem_pend;
	uint8_t modem_status;			// 0..5: SYNC_DETECT, PREAMBLE_DETECT, INVALID_PREAMBLE, RSSI, RSSI_JUMP, INVALID_SYNC
	uint8_t chip_pend;
	uint8_t chip_status;			// 0..7: WUT, LOW_BATT, CHIP_READY, CMD_ERROR, STATE_CHANGE, FIFO_UNDERFLOW_OVERFLOW_ERROR, CAL[0..1]
};

struct ph_status_s {
	uint8_t ph_pend;
	uint8_t ph_status;				// 0..7: RX_FIFO_ALMOST_FULL, TX_FIFO_ALMOST_EMPTY, x, CRC_ERROR, PACKET_RX, PACKET_SENT, FILTER_MISS, FILTER_MATCH
};

struct modem_status_s {
	uint8_t modem_pend;
	uint8_t modem_status;			// 0..5: SYNC_DETECT, PREAMBLE_DETECT, INVALID_PREAMBLE, RSSI, RSSI_JUMP, INVALID_SYNC
	uint8_t curr_rssi;				// current RSSI reading from the modem
	uint8_t latch_rssi;				// latched RSSI reading from the modem as configured by MODEM_RSSI_CONTROL. Reset to 0 at the start of every RX
	uint8_t ant1_rssi;				// RSSI of ANT1 while antenna diversity, latched during preamble evaluation and avaliable for reading after sync detection
	uint8_t ant2_rssi;				// RSSI of ANT2 while antenna diversity
	uint8_t afc_freq_offset_msb;	// AFC value that is generated by the AFC loop during receive mode
	uint8_t afc_freq_offset_lsb;
};

struct chip_status_s {
	uint8_t chip_pend;
	uint8_t chip_status;			// 0..7: WUT, LOW_BATT, CHIP_READY, CMD_ERROR, STATE_CHANGE, FIFO_UNDERFLOW_OVERFLOW_ERROR, CAL[0..1]
	uint8_t cmd_err_status;			// 0 no error, 0x10 bad command, 0x11 invalid cmd argument, 0x12 new cmd before prvs completed, 0x40 bad property ID
};

struct device_state_s {
	uint8_t curr_state;				// 0..4: current state - 0 no change, 1 sleep, 2 SPI active, 3 ready, 4 ready, 5 tune TX, 6 tune RX, 7 TX, 8 RX
	uint8_t current_channel;		// current channel
};

union radio_buffer_u {
	uint8_t data[64];							// data buffer, 64 bytes = FIFO length, when not using FIFO could be as low as 8 bytes
	struct part_info_s			part_info;		// basic information about the device
	struct func_info_s			func_info;		// function revision information of the device
	struct fifo_info_s			fifo_info;		// transmit and receive FIFO counts
	struct packet_info_s		packet_info;	// information about the last packet received
	struct int_status_s			int_status;		// interrupt status
	struct ph_status_s			ph_status;		// packet handler status
	struct modem_status_s		modem_status;	// modem status
	struct chip_status_s		chip_status;	// chip status
	struct device_state_s		device_state;	// current device state
};

//
extern union radio_buffer_u radio_buffer;

// states of radio, as used in radio_change_state and radio_request_device_state
#define RADIO_STATE_NO_CHANGE		0
#define RADIO_STATE_SLEEP			1
#define RADIO_STATE_SPI_ACTIVE		2
#define RADIO_STATE_READY			3
#define RADIO_STATE_READY2			4
#define RADIO_STATE_TUNE_RX			6
#define RADIO_STATE_RX				8

// command errors, as reported in radio_get_chip_status
#define RADIO_CMD_ERR_NONE				 0
#define RADIO_CMD_ERR_BAD_CMD			16
#define RADIO_CMD_ERR_INVALID_ARGUMENT	17
#define RADIO_CMD_ERR_CMD_BEFORE_READY	18
#define RADIO_CMD_ERR_INVALID_BOOTMODE	49
#define RADIO_CMD_ERR_BAD_PROPERY_ID	64

// bits in int_pend and int_status fields of radio_get_int_status
#define RADIO_PH_INT				BIT0
#define RADIO_MODEM_INT				BIT1
#define RADIO_CHIP_INT				BIT2

// bits in ph_pend and ph_status
// also use to clear/keep pending interrupts in radio_get_int_status and radio_get_ph_status
#define RADIO_RX_FIFO_ALMOST_FULL	BIT0
#define RADIO_CRC_ERROR				BIT3
#define RADIO_PACKET_RX				BIT4
#define RADIO_FILTER_MISS			BIT6
#define RADIO_FILTER_MATCH			BIT7

// bits in modem_pend and modem_status
// also use to clear/keep pending interrupts  in radio_get_int_status and radio_get_modem_status
#define RADIO_SYNC_DETECT			BIT0
#define RADIO_PREAMBLE_DETECT		BIT1
#define RADIO_INVALID_PREAMBLE		BIT2
#define RADIO_RSSI					BIT3
#define RADIO_RSSI_JUMP				BIT4
#define	RADIO_INVALID_SYNC			BIT5
#define RADIO_POSTAMBLE_DETECT		BIT6

// bits in chip_pend and chip_status
// also use to clear/keep pending interrupts  radio_get_int_status and radio_get_chip_status
#define RADIO_WUT					BIT0
#define RADIO_LOW_BATT				BIT1
#define RADIO_CHIP_READY			BIT2
#define RADIO_CMD_ERROR				BIT3
#define RADIO_STATE_CHANGE			BIT4
#define RADIO_FIFO_UNDERFLOW_OVERFLOW_ERROR		BIT5
#define RADIO_CAL					BIT6

#endif /* RADIO_H_ */
