/*! @file radio_config.h
* @brief This file contains the automatically generated
* configurations.
*
* @n WDS GUI Version: 3.2.2.0
* @n Device: Si4362 Rev.: B1
*
* @b COPYRIGHT
* @n Silicon Laboratories Confidential
* @n Copyright 2013 Silicon Laboratories, Inc.
* @n http://www.silabs.com
*/

#ifndef RADIO_CONFIG_H_
#define RADIO_CONFIG_H_

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
//%%        Crys_freq(Hz)        Crys_tol(ppm)        IF_mode        High_perf_Ch_Fil        OSRtune        Ch_Fil_Bw_AFC        ANT_DIV        PM_pattern
//         30000000        20        2        1        0        0        0        15
//%%        MOD_type        Rsymb(sps)        Fdev(Hz)        RXBW(Hz)        Mancheste        AFC_en        Rsymb_error        Chip-Version
//         3        9600        2400        25000        0        0        0.0        2
//%%        RF Freq.(MHz)        API_TC        fhst        inputBW        BERT        RAW_dout        D_source        Hi_pfm_div
//         161.975        29        50000        1        0        0        0        1
//
// # WB filter 2 (BW = 25.77 kHz); NB-filter 2 (BW = 25.77 kHz)

//
// Modulation index: 0
*/


// CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ {30000000L}
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER {0x00}
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH {0x07}
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP {0x03}
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET {0xF000}


// CONFIGURATION COMMANDS

/*
// Command: RF_POWER_UP
// Description: Power-up the device with the specified function. Power-up is complete when the CTS bit is set. This command may take longer to set the CTS bit than other commands.
*/
#define RF_POWER_UP 0x02, 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80

/*
// Command: RF_GPIO_PIN_CFG
// Description: Configures the GPIO pins.
// GPIO0: 0x1A = SYNC_WORD_DETECTED        (1 when sync word detected, back to 0 when end of packet)
// GPIO1: 0x00 = DEFAULT = CTS
// GPIO2: 0x11 = RX_DATA_CLK
// GPIO3: 0x14 = RX_DATA
// NIRQ: 0x1B = CCA                (1 when RSSI threshold exceeded detected, back to 0 when below threshold) - 0x00 = DEFAULT = IRQ (active low)
// SDO: 0x00 = DEFAULT = SDO (SPI data out)
*/
//#define RF_GPIO_PIN_CFG 0x13, 0x1A, 0x00, 0x11, 0x14, 0x18, 0x00
#define RF_GPIO_PIN_CFG 0x13, 0x1A, 0x00, 0x11, 0x14, 0x1B, 0x00

/*
// Set properties: RF_GLOBAL_XO_TUNE_1
// Number of properties: 1
// Group ID: 00
// Start ID: 00
// Default values: 0x40,
// Descriptions:
// GLOBAL_XO_TUNE - Crystal oscillator frequency tuning value. 0x00 is maximum frequency value and 0x7F is lowest frequency value. Each LSB code corresponds to a 70 fF capacitance change. The total adjustment range assuming a 30 MHz XTAL is +/-100 ppm.
*/
#define RF_GLOBAL_XO_TUNE_1 0x11, 0x00, 0x01, 0x00, 0x40	//0x52

/*
// Set properties: RF_GLOBAL_CONFIG_1
// Number of properties: 1
// Group ID: 00
// Start ID: 03
// Default values: 0x20,
// Descriptions:
// GLOBAL_CONFIG - Various settings that affect entire chip. If PROTOCOL is specified, the chip is placed into protocol aware state.
*/
#define RF_GLOBAL_CONFIG_1 0x11, 0x00, 0x01, 0x03, 0x40

/*
// Set properties: RF_INT_CTL_ENABLE_1
// Number of properties: 1
// Group ID: 01
// Start ID: 00
// Default values: 0x04,
// Descriptions:
// INT_CTL_ENABLE - Enables top-level interrupt sources to generate HW interrupts at the NIRQ pin. The three interrupt groups are Chip, Modem and Packet Handler. Each of them contains multiple possible interrupt sources that must be individually enabled via the INT_CTL_PH_ENABLE, INT_CTL_MODEM_ENABLE, INT_CTL_CHIP_ENABLE properties. Note that this property only provides for global enabling/disabling of the HW interrupt indication on the NIRQ output pin. An internal interrupt event may still be generated even if the indication on the NIRQ output pin is disabled. The interrupt event may always be monitored by polling a GPIO pin, or via GET_INT_STATUS, GET_CHIP_STATUS, GET_PH_STATUS, or GET_MODEM_STATUS commands.
*/
#define RF_INT_CTL_ENABLE_1 0x11, 0x01, 0x01, 0x00, 0x00

/*
// Set properties: RF_FRR_CTL_A_MODE_4
// Number of properties: 4
// Group ID: 02
// Start ID: 00
// Default values: 0x01, 0x02, 0x09, 0x00,
// Descriptions:
// FRR_CTL_A_MODE - Set the data that is present in fast response register A.
// FRR_CTL_B_MODE - Set the data that is present in fast response register B.
// FRR_CTL_C_MODE - Set the data that is present in fast response register C.
// FRR_CTL_D_MODE - Set the data that is present in fast response register D.
*/
//#define RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
#define RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00			// latched RSSI in FRR A

/*
// Set properties: RF_PREAMBLE_TX_LENGTH_9
// Number of properties: 9
// Group ID: 10
// Start ID: 00
// Default values: 0x08, 0x14, 0x00, 0x0F, 0x21, 0x00, 0x00, 0x00, 0x00,
// Descriptions:
// PREAMBLE_TX_LENGTH - This property is used to configure the length of the Preamble field in TX mode for both Standard and Non-Standard Preambles. This property is not used by the chip in RX mode except when receiving a Non-Standard Preamble. In such a case, this property must be configured with the expected Preamble length (to provide an upper timeout limit on the Sync Word search algorithm). This property is only applicable in TX Packet Handler FIFO mode; if TX Direct Synchronous or TX Direct Asynchronous mode is selected (see MODEM_MOD_TYPE property), the entire transmission is obtained from a real-time TXDATA input stream on a GPIO pin and no automatic field construction is possible. The units of this property are in either nibbles or bytes, depending upon the value of PREAMBLE_CONFIG:LENGTH_CONFIG. Setting PREAMBLE_TX_LENGTH = 0x00 is a valid value, and will result in skipping transmission of the Preamble field; in such a case, the Sync Word will be the first transmitted field.
// PREAMBLE_CONFIG_STD_1 - The fields in this property are applicable only in RX mode, and apply primarily to reception of Standard Preamble patterns (e.g., 1010 or 0101 patterns). However, see note on required configuration of RX_THRESH during reception of Non-Standard Preamble patterns.
// PREAMBLE_CONFIG_NSTD - The fields in this property apply only to transmission and reception of packets with Non-Standard Preamble patterns (e.g., other than 1010 or 0101 patterns).
// PREAMBLE_CONFIG_STD_2 - During initial reception and acquisition of a packet, it is often useful to define a timeout period to limit the amount of time the chip remains on-channel awaiting the incoming packet. This property allows configuration of this preamble search timeout period. If a valid preamble is not found within the specified timeout period, an INVALID_PREAMBLE event is generated and may be used to generate an interrupt and/or exit automatically from RX mode. The fields in this property are applicable only in RX mode, and only apply to reception of Standard Preamble patterns (e.g., 1010 or 0101 patterns). Detection of INVALID_PREAMBLE is not possible when configured to receive a Non-Standard Preamble pattern. Two different timeout configuration fields are provided to allow definition of timeout periods with significantly different lengths and bit resolution.
// PREAMBLE_CONFIG - Miscellaneous preamble configuration bits.
// PREAMBLE_PATTERN_31_24 - The PREAMBLE_PATTERN_XX properties define the pattern to be transmitted in TX mode and the pattern expected to be received in RX mode, but only when PREAMBLE_CONFIG:STANDARD_PREAM has been set to 0x0 (i.e., use Non-Standard Preamble). The PREAMBLE_PATTERN_XX properties are used to define the actual bit values describing the Non-Standard Preamble pattern; PREAMBLE_CONFIG_NSTD:PATTERN_LENGTH is used to define how many bits of the PREAMBLE_PATTERN_XX values are used. In TX mode, if the value of PREAMBLE_TX_LENGTH is larger than PREAMBLE_CONFIG_NSTD:PATTERN_LENGTH, the PREAMBLE_PATTERN will be transmitted repeatedly until the full number of bits has been sent. In RX mode, the full number of bits defined by PREAMBLE_CONFIG_NSTD:PATTERN_LENGTH must be received once before the PREAMBLE_VALID signal/interrupt is generated. If Manchester encoding/decoding is enabled, the values of the PREAMBLE_PATTERN_XX properties are expressed in chips (i.e., after Manchester encoding in TX mode, or before Manchester decoding in RX mode). The bits of the PREAMBLE_PATTERN_XX are always sent bits 0-31 time-wise (e.g., PREAMBLE_PATTERN_7_0 bit 0 is sent first).
// PREAMBLE_PATTERN_23_16 - Bits 23-16 of the Non-Standard Preamble pattern to be transmitted or expected to be received. Please refer to the text description for PREAMBLE_PATTERN_31_24 for more details regarding the Non-Standard Preamble functionality.
// PREAMBLE_PATTERN_15_8 - Bits 15-8 of the Non-Standard Preamble pattern to be transmitted or expected to be received. Please refer to the text description for PREAMBLE_PATTERN_31_24 for more details regarding the Non-Standard Preamble functionality.
// PREAMBLE_PATTERN_7_0 - Bits 7-0 of the Non-Standard Preamble pattern to be transmitted or expected to be received. Please refer to the text description for PREAMBLE_PATTERN_31_24 for more details regarding the Non-Standard Preamble functionality.
*/
//#define RF_PREAMBLE_TX_LENGTH_9 0x11, 0x10, 0x09, 0x00, 0x08, 0x10, 0x00, 0x0F, 0x12, 0x00, 0x00, 0x00, 0x00                // standard 1010 preamble
#define RF_PREAMBLE_TX_LENGTH_9 0x11, 0x10, 0x09, 0x00, 0x03, 0x14, 0x10, 0x0F, 0x10, 0x00, 0xCC, 0xCC, 0xCC                // NRZI encoded preamble = 00110011

/*
// Set properties: RF_SYNC_CONFIG_5
// Number of properties: 5
// Group ID: 11
// Start ID: 00
// Default values: 0x01, 0x2D, 0xD4, 0x2D, 0xD4,
// Descriptions:
// SYNC_CONFIG - Configuration of miscellaneous Sync Word bits. NOTE: each byte of the Sync Word is transmitted/received in little-endian fashion (i.e., least significant bit first). Byte(s) of the Sync Word are transmitted/received in descending order (i.e., Byte 3 first, followed by Byte 2, etc.)
// SYNC_BITS_31_24 - Define the value of Byte 3 of the Sync Word for both TX and RX operation. NOTE: Although the Sync Word byte(s) are transmitted/received in descending order (i.e., Byte 3 first, followed by Byte 2, etc.), each byte is transmitted/received in little-endian fashion (i.e., least significant bit first).
// SYNC_BITS_23_16 - Sync bytes are always sent bit 0 first.
// SYNC_BITS_15_8 - Sync bytes are always sent bit 0 first.
// SYNC_BITS_7_0 - Sync bytes are always sent bit 0 first.
*/
#define RF_SYNC_CONFIG_5 0x11, 0x11, 0x05, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00

/*
// Set properties: RF_PKT_CRC_CONFIG_1
// Number of properties: 1
// Group ID: 12
// Start ID: 00
// Default values: 0x00,
// Descriptions:
// PKT_CRC_CONFIG - The chip contains a 32-bit CRC engine for the purpose of generating and comparing a checksum across selected data fields. This property is used to select the desired CRC polynomial and CRC seed value. If a 8-bit CRC polynomial is selected, the length of the resulting checksum is 1-bytes. If a 16-bit CRC polynomial is selected, the length of the resulting checksum is 2-bytes. If a 32-bit CRC polynomial is selected, the length of the resulting checksum is 4-bytes. The configuration bits in this property are common to both TX and RX modes; however, this property is only applicable when automatic Packet Handling is enabled (e.g., when using the TX FIFO as the modulation source in TX mode, or when PKT_CONFIG1:PH_RX_DISABLE is cleared in RX mode).
*/
#define RF_PKT_CRC_CONFIG_1 0x11, 0x12, 0x01, 0x00, 0x80

/*
// Set properties: RF_PKT_CONFIG1_1
// Number of properties: 1
// Group ID: 12
// Start ID: 06
// Default values: 0x00,
// Descriptions:
// PKT_CONFIG1 - General packet configuration bits.
*/
#define RF_PKT_CONFIG1_1 0x11, 0x12, 0x01, 0x06, 0x02

/*
// Set properties: RF_PKT_LEN_3
// Number of properties: 3
// Group ID: 12
// Start ID: 08
// Default values: 0x00, 0x00, 0x00,
// Descriptions:
// PKT_LEN - This property configures the chip for reception of a variable length packet. This property is applicable only in RX mode, and only when the automatic Packet Handler is enabled (i.e., PKT_CONFIG1:PH_RX_DISABLE is cleared). If enabled, the extracted length can be retrieved using the PACKET_INFO command.
// PKT_LEN_FIELD_SOURCE - This property configures the chip for reception of a variable length packet, and defines where the length field is in the packet. The packet length byte(s) must be the last byte(s) in a fixed length field and must precede the variable length field. This property is applicable only in RX mode, and only when the automatic Packet Handler is enabled (i.e., the PH_RX_DISABLE bit in the PKT_CONFIG1 property is cleared).
// PKT_LEN_ADJUST - It is possible to define the total length of a packet in a variety of ways (e.g., including/excluding the packet length bytes, including/excluding the CRC bytes, etc.) This property provides a means of adjusting/offsetting the received packet length value in order to comply with a wide range of packet length implementations. This property is applicable only in RX mode, and only when the automatic Packet Handler is enabled (i.e., the PH_RX_DISABLE bit in the PKT_CONFIG1 property is cleared). The value of the LEN_ADJUST field is added to the received value of the packet length byte(s), extracted from the field specified by the SRC_FIELD value in the PKT_LEN_FIELD_SOURCE property. The resulting value is then used to configure the length of the variable length field specified by PKT_LEN:DST_FIELD. The required constant value of the LEN_ADJUST field is typically defined in a protocol specification or regulatory standard, or can be derived from the specification. The default processing mode of the chip is that the received packet length value does NOT include the packet length bytes, and does NOT include CRC bytes. In such a case, the LEN_ADJUST field should be set to 0x00. If the received packet length value includes the packet length bytes and/or the CRC bytes, its effective value must be reduced by adding a negative offset. In such a case, the LEN_ADJUST field should be set to the negative value of the extra byte count (in 2's complement format). Example: the transmitted packet has a 2-byte packet length field followed by a 10-byte Payload field, and the value contained in the packet length bytes = 0x000C = 12 bytes. The value of LEN_ADJUST should be set = 0xFE = -2. The LEN_ADJUST field is a signed value.
*/
#define RF_PKT_LEN_3 0x11, 0x12, 0x03, 0x08, 0x00, 0x00, 0x00

/*
// Set properties: RF_PKT_FIELD_1_LENGTH_12_8_12
// Number of properties: 12
// Group ID: 12
// Start ID: 0D
// Default values: 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// Descriptions:
// PKT_FIELD_1_LENGTH_12_8 - The PKT_FIELD_1_LENGTH_XX properties specify the length of Field 1 in bytes. The field length is an unsigned 13-bit value; this property is Byte 1 of the length value, and is used along with the PKT_FIELD_1_LENGTH_7_0 property. Partitioning of the Payload into separate fields is provided to support variable length packets, and to allow different forms of data processing (e.g., Manchester encoding, data whitening, etc) across different fields. These properties are applicable in TX mode only if the START_TX command is sent with the parameter TX_LEN=0. These properties are applicable in RX mode only under the following conditions: RX Packet Handling is enabled by clearing PKT_CONFIG1:PH_RX_DISABLE, and The START_RX command is sent with the parameter RX_LEN=0, and PKT_CONFIG1:PH_FIELD_SPLIT is cleared. If the PH_FIELD_SPLIT bit is set, the lengths of the various fields in RX mode are determined by the PKT_RX_FIELD_LENGTH_XX properties. A value of zero in this property means that the field is not used. During TX mode, data will be retrieved from the TX FIFO until encountering the first field whose length has been set to zero. During RX mode, data will be stored in the RX FIFO until again encountering the first field whose length has been set to zero. It is possible (although not common) to set the length of Field 1 = 0 bytes, as this would indicate transmission/reception of a packet with no Payload data at all. Field 1 cannot be configured as a variable length field, as there is no preceding field that may contain the variable length byte(s).
// PKT_FIELD_1_LENGTH_7_0 - Byte 0 of the unsigned 13-bit Field 1 length value. Please refer to the text description for PKT_FIELD_1_LENGTH_12_8 for more details regarding Field 1 length configuration.
// PKT_FIELD_1_CONFIG - Partitioning of the Payload into separate fields is provided to support features such as variable length packets and field-specific data processing (e.g., Manchester encoding, data whitening, etc). This property provides configuration bits for field-specific processing on Field 1. The configuration bits in this property are common to both TX and RX modes dependent on PKT_CONFIG1:PH_FIELD_SPLIT; however, this property is only applicable when automatic Packet Handling is enabled (e.g., when using the TX FIFO as the modulation source in TX mode, or when PKT_CONFIG1:PH_RX_DISABLE is cleared in RX mode).
// PKT_FIELD_1_CRC_CONFIG - This property is use to control the calculation, transmission, and checking of CRC across Field 1.
// PKT_FIELD_2_LENGTH_12_8 - The PKT_FIELD_2_LENGTH_XX properties specify the length of Field 2 in bytes. The field length is an unsigned 13-bit value; this property is Byte 1 of the length value, and is used along with the PKT_FIELD_2_LENGTH_7_0 property. Field 2 may be configured as a variable length field by setting the appropriate value of DST_FIELD (in the PKT_LEN property); however, it is also necessary that a previous field contain the variable length byte(s). If configured as a variable length field, this property must be set with a value that represents the maximum expected length of the field. Please refer to the text description for PKT_FIELD_1_LENGTH_12_8 for more details regarding field length configuration.
// PKT_FIELD_2_LENGTH_7_0 - Byte 0 of the unsigned 13-bit Field 2 length value. Please refer to the text description for PKT_FIELD_2_LENGTH_12_8 for more details regarding Field 2 length configuration.
// PKT_FIELD_2_CONFIG - This property provides configuration bits for field-specific processing on Field 2. Please refer to the text description for PKT_FIELD_1_CONFIG for more details regarding enabling/disabling of field-specific processing.
// PKT_FIELD_2_CRC_CONFIG - This property is use to control the calculation, transmission, and checking of CRC across Field 2. Please refer to the text description for PKT_FIELD_1_CRC_CONFIG for more details regarding configuration of field-specific CRC calculation.
// PKT_FIELD_3_LENGTH_12_8 - The PKT_FIELD_3_LENGTH_XX properties specify the length of Field 3 in bytes. The field length is an unsigned 13-bit value; this property is Byte 1 of the length value, and is used along with the PKT_FIELD_3_LENGTH_7_0 property. Field 3 may be configured as a variable length field by setting the appropriate value of DST_FIELD (in the PKT_LEN property); however, it is also necessary that a previous field contain the variable length byte(s). If configured as a variable length field, this property must be set with a value that represents the maximum expected length of the field. Please refer to the text description for PKT_FIELD_1_LENGTH_12_8 for more details regarding field length configuration.
// PKT_FIELD_3_LENGTH_7_0 - Byte 0 of the unsigned 13-bit Field 3 length value. Please refer to the text description for PKT_FIELD_3_LENGTH_12_8 for more details regarding Field 3 length configuration.
// PKT_FIELD_3_CONFIG - This property provides configuration bits for field-specific processing on Field 3. Please refer to the text description for PKT_FIELD_1_CONFIG for more details regarding enabling/disabling of field-specific processing.
// PKT_FIELD_3_CRC_CONFIG - This property is use to control the calculation, transmission, and checking of CRC across Field 3. Please refer to the text description for PKT_FIELD_1_CRC_CONFIG for more details regarding configuration of field-specific CRC calculation.
*/
#define RF_PKT_FIELD_1_LENGTH_12_8_12 0x11, 0x12, 0x0C, 0x0D, 0x00, 0x1B, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties: RF_PKT_FIELD_4_LENGTH_12_8_8
// Number of properties: 8
// Group ID: 12
// Start ID: 19
// Default values: 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// Descriptions:
// PKT_FIELD_4_LENGTH_12_8 - The PKT_FIELD_4_LENGTH_XX properties specify the length of Field 4 in bytes. The field length is an unsigned 13-bit value; this property is Byte 1 of the length value, and is used along with the PKT_FIELD_4_LENGTH_7_0 property. Field 4 may be configured as a variable length field by setting the appropriate value of DST_FIELD (in the PKT_LEN property); however, it is also necessary that a previous field contain the variable length byte(s). If configured as a variable length field, this property must be set with a value that represents the maximum expected length of the field. Please refer to the text description for PKT_FIELD_1_LENGTH_12_8 for more details regarding field length configuration.
// PKT_FIELD_4_LENGTH_7_0 - Byte 0 of the unsigned 13-bit Field 4 length value. Please refer to the text description for PKT_FIELD_4_LENGTH_12_8 for more details regarding Field 4 length configuration.
// PKT_FIELD_4_CONFIG - This property provides configuration bits for field-specific processing on Field 4. Please refer to the text description for PKT_FIELD_1_CONFIG for more details regarding enabling/disabling of field-specific processing.
// PKT_FIELD_4_CRC_CONFIG - This property is use to control the calculation, transmission, and checking of CRC across Field 4. Please refer to the text description for PKT_FIELD_1_CRC_CONFIG for more details regarding configuration of field-specific CRC calculation.
// PKT_FIELD_5_LENGTH_12_8 - The PKT_FIELD_5_LENGTH_XX properties specify the length of Field 5 in bytes. The field length is an unsigned 13-bit value; this property is Byte 1 of the length value, and is used along with the PKT_FIELD_5_LENGTH_7_0 property. Field 5 may be configured as a variable length field by setting the appropriate value of DST_FIELD (in the PKT_LEN property); however, it is also necessary that a previous field contain the variable length byte(s). If configured as a variable length field, this property must be set with a value that represents the maximum expected length of the field. Please refer to the text description for PKT_FIELD_1_LENGTH_12_8 for more details regarding field length configuration.
// PKT_FIELD_5_LENGTH_7_0 - Byte 0 of the unsigned 13-bit Field 5 length value. Please refer to the text description for PKT_FIELD_5_LENGTH_12_8 for more details regarding Field 5 length configuration.
// PKT_FIELD_5_CONFIG - This property provides configuration bits for field-specific processing on Field 5. Please refer to the text description for PKT_FIELD_1_CONFIG for more details regarding enabling/disabling of field-specific processing.
// PKT_FIELD_5_CRC_CONFIG - This property is use to control the calculation, transmission, and checking of CRC across Field 5. Please refer to the text description for PKT_FIELD_1_CRC_CONFIG for more details regarding configuration of field-specific CRC calculation.
*/
#define RF_PKT_FIELD_4_LENGTH_12_8_8 0x11, 0x12, 0x08, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties: RF_MODEM_MOD_TYPE_12
// Number of properties: 12
// Group ID: 20
// Start ID: 00
// Default values: 0x02, 0x80, 0x07, 0x0F, 0x42, 0x40, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x06,
// Descriptions:
// MODEM_MOD_TYPE - This property configures the Modem in the chip for transmission/reception of the following modulation types: OOK, 2(G)FSK, and 4(G)FSK (if supported) modulation. In TX mode, this property is additionally used to select the source of the TX data stream, and whether that TX data stream is from a synchronous or asynchronous source (in Direct mode only). The Modem must be configured for transmission/reception of only one type of modulation throughout the entire packet; it is not possible (for example) to use OOK modulation over certain fields and (G)FSK modulation over other fields. If 4(G)FSK is supported, it is possible to switch between 2(G)FSK and 4(G)FSK on a field-specific basis; however, this is accomplished by configuring the Modem to 4(G)FSK mode for the entire packet and then performing special data processing in the Packet Handler.
// MODEM_MAP_CONTROL - Modem Mapping Control.
// MODEM_DSM_CTRL - Delta Sigma Modulator control.
// MODEM_DATA_RATE_2 - Data rate, unsigned 24-bit, 100 kbps by default.
// MODEM_DATA_RATE_1 - Data rate, unsigned 24-bit, 100 kbps by default.
// MODEM_DATA_RATE_0 - Data rate, unsigned 24-bit, 100 kbps by default.
// MODEM_TX_NCO_MODE_3 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
// MODEM_TX_NCO_MODE_2 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
// MODEM_TX_NCO_MODE_1 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
// MODEM_TX_NCO_MODE_0 - TX NCOs modulo, unsigned 26-bit, default value is 0x1C9C380. Together with the crystal frequency and data rate, this number will set the symbol rate. If data rate is greater than 50 kbps, it should be set to 0x1C9C380. Otherwise it will be 0x2DC6C0.
// MODEM_FREQ_DEV_2 - The TX frequency deviation is specified by an unsigned 17-bit value. This property defines Byte 2 of the frequency deviation value. The units of the MODEM_FREQ_DEV_X property are in increments of the LSB tuning resolution of the PLL Synthesizer, and are therefore a function of the crystal frequency and output frequency band. The formula for calculating the value of the MODEM_FREQ_DEV property is given by the following equation: For 2(G)FSK mode, the specified value is the peak deviation. For 4(G)FSK mode (if supported), the specified value is the inner deviation (i.e., between channel center frequency and the nearest symbol deviation level). The MODEM_FREQ_DEV_X properties are only used in TX mode.
// MODEM_FREQ_DEV_1 - The TX frequency deviation is specified by an unsigned 17-bit value. This property defines Byte 1 of the frequency deviation value. Please refer to the text description for MODEM_FREQ_DEV_2 for more details regarding TX frequency deviation.
*/
#define RF_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x03, 0x00, 0x07, 0x00, 0x96, 0x00, 0x04, 0x2D, 0xC6, 0xC0, 0x00, 0x01

/*
// Set properties: RF_MODEM_FREQ_DEV_0_1
// Number of properties: 1
// Group ID: 20
// Start ID: 0C
// Default values: 0xD3,
// Descriptions:
// MODEM_FREQ_DEV_0 - The TX frequency deviation is specified by an unsigned 17-bit value. This property defines Byte 0 of the frequency deviation value. Please refer to the text description for MODEM_FREQ_DEV_2 for more details regarding TX frequency deviation.
*/
#define RF_MODEM_FREQ_DEV_0_1 0x11, 0x20, 0x01, 0x0C, 0xF7

/*
// Set properties: RF_MODEM_MDM_CTRL_7
// Number of properties: 7
// Group ID: 20
// Start ID: 19
// Default values: 0x00, 0x08, 0x03, 0xC0, 0x00, 0x10, 0x20,
// Descriptions:
// MODEM_MDM_CTRL - Detector selection and PM pattern configuration.                // 0 with long preamble?
// MODEM_IF_CONTROL - Specifies fixed-IF(468.75 kHz), zero-IF, or scalable IF.
// MODEM_IF_FREQ_2 - Bits 16 an 17 of the 18-bit IF frequency. Specify fixed-IF(468.75kHz), zero-IF, or scalable IF.
// MODEM_IF_FREQ_1 - IF frequency is 18-bit.
// MODEM_IF_FREQ_0 - IF frequency is 18-bit.
// MODEM_DECIMATION_CFG1 - Specifies the exponent of decimation ratios of the three CIC decimators in RX filter chain.
// MODEM_DECIMATION_CFG0 - This property specifies the selection of the two polyphase filters preceding CIC filters, the droop compensation filter, the RX decimate by 8 2x filter and the channel filter power mode.
*/
#define RF_MODEM_MDM_CTRL_7 0x11, 0x20, 0x07, 0x19, 0x80, 0x08, 0x02, 0x80, 0x00, 0x70, 0x20

/*
// Set properties: RF_MODEM_BCR_OSR_1_9
// Number of properties: 9
// Group ID: 20
// Start ID: 22
// Default values: 0x00, 0x4B, 0x06, 0xD3, 0xA0, 0x06, 0xD3, 0x02, 0xC0,
// Descriptions:
// MODEM_BCR_OSR_1 - High byte of RX symbol oversampling rate at BCR/Slicer (12-bit unsigned number).
// MODEM_BCR_OSR_0 - Low byte of RX symbol oversampling rate at BCR/Slicer, total 12 bits.
// MODEM_BCR_NCO_OFFSET_2 - Specifies the 6 MSBs of the BCR NCO offset.
// MODEM_BCR_NCO_OFFSET_1 - Byte 1 of RX BCR NCO offset (an unsigned 22-bit number).
// MODEM_BCR_NCO_OFFSET_0 - Low byte of RX NCO offset.
// MODEM_BCR_GAIN_1 - High byte of clock recovery timing loop gain value.
// MODEM_BCR_GAIN_0 - Low byte of clock recovery timing loop gain value.
// MODEM_BCR_GEAR - RX BCR loop gear control. A reduced value of bit clock recovery gain is often desired after gear shifting (to reduce clock jitter). The BCR loop gain in both gear modes is obtained from a baseline clock recovery gain value (crgain in the MODEM_BCR_GAIN_x properties), scaled by the crfast and crslow values specified in this property. NOTE: larger values of crfast or crslow result in lower values of bit clock recovery gain.
// MODEM_BCR_MISC1 - This property configures miscellaneous options within the RX BCR loop.
*/
#define RF_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x00, 0x62, 0x05, 0x3E, 0x2D, 0x02, 0x9D, 0x00, 0xC2

/*
// Set properties: RF_MODEM_AFC_GEAR_7
// Number of properties: 7
// Group ID: 20
// Start ID: 2C
// Default values: 0x00, 0x23, 0x83, 0x69, 0x00, 0x40, 0xA0,
// Descriptions:
// MODEM_AFC_GEAR - Selection of gear switching source for both RX AFC and BCR. Control of gain for RX AFC in both FAST and SLOW modes (i.e., before and after gear switching)
// MODEM_AFC_WAIT - Short and long wait periods after AFC correction.
// MODEM_AFC_GAIN_1 - High byte of 13-bit AFC loop gain value. Enabling of AFC frequency error estimation. Enabling of dynamic IF bandwidth switching during the packet.
// MODEM_AFC_GAIN_0 - Low byte of 13-bit AFC loop gain value
// MODEM_AFC_LIMITER_1 - High byte of 15-bit AFC limiter value.
// MODEM_AFC_LIMITER_0 - Low byte of 15-bit AFC limiter value.
// MODEM_AFC_MISC - Specifies misc AFC control bits.
*/
#define RF_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x54, 0x36, 0x81, 0x01, 0x02, 0x4E, 0x80

/*
// Set properties: RF_MODEM_AGC_CONTROL_1
// Number of properties: 1
// Group ID: 20
// Start ID: 35
// Default values: 0xE0,
// Descriptions:
// MODEM_AGC_CONTROL - AGC control.
*/
#define RF_MODEM_AGC_CONTROL_1 0x11, 0x20, 0x01, 0x35, 0xE2

/*
// Set properties: RF_MODEM_AGC_WINDOW_SIZE_9
// Number of properties: 9
// Group ID: 20
// Start ID: 38
// Default values: 0x11, 0x10, 0x10, 0x0B, 0x1C, 0x40, 0x00, 0x00, 0x2B,
// Descriptions:
// MODEM_AGC_WINDOW_SIZE - Sets PGA and LNA settling time window and measurement time window.
// MODEM_AGC_RFPD_DECAY - Sets RF detector decay time.
// MODEM_AGC_IFPD_DECAY - Sets IF detector decay time.
// MODEM_FSK4_GAIN1 - Controls 4(G)FSK suppression gain.
// MODEM_FSK4_GAIN0 - Controls 4(G)FSK suppression gain.
// MODEM_FSK4_TH1 - High byte of 4(G)FSK slicer threshold.
// MODEM_FSK4_TH0 - Low byte of 4(G)FSK slicer threshold.
// MODEM_FSK4_MAP - 4(G)FSK symbol mapping code.
// MODEM_OOK_PDTC - OOK peak detector decay and attack time.
*/
#define RF_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09, 0x38, 0x11, 0x15, 0x15, 0x00, 0x1A, 0x20, 0x00, 0x00, 0x28

/*
// Set properties: RF_MODEM_OOK_CNT1_11
// Number of properties: 11
// Group ID: 20
// Start ID: 42
// Default values: 0xA4, 0x03, 0x56, 0x02, 0x00, 0xA3, 0x02, 0x80, 0xFF, 0x0C, 0x01,
// Descriptions:
// MODEM_OOK_CNT1 - OOK control.
// MODEM_OOK_MISC - Control OOK Peak Detector.
// MODEM_RAW_SEARCH - Searching period for high and low gear.
// MODEM_RAW_CONTROL - Defines gain and enable controls for raw / nonstandard mode.                // 83 = use standard detection for 1010 preamble pattern
// MODEM_RAW_EYE_1 - Eye-open detector threshold.
// MODEM_RAW_EYE_0 - Eye-open detector threshold. The reset of bits are set by property RAW_EYE_1.
// MODEM_ANT_DIV_MODE - Antenna diversity mode settings.
// MODEM_ANT_DIV_CONTROL - Specifies pm detection threshold and GPIO configuration in antenna diversity mode.
// MODEM_RSSI_THRESH - Selects the threshold for Clear Channel Assessment (CCA) and generation of the RSSI interrupt. If the Current RSSI value is above this threshold, a GPIO pin configured to output the CCA signal will be high. If the Current RSSI value crosses above this threshold, the RSSI interrupt will be generated. NOTE: the Latched RSSI value exceeding this threshold will not generate an RSSI interrupt. If enabled in the MODEM_RSSI_CONTROL property, the Latched RSSI value may also be compared against this threshold. The purpose of the comparison is to determine if the Latched RSSI value is below (not above) the threshold; if so, the chip proceeds to the specified START_RX:RXTIMEOUT_STATE and generates a PREAMBLE_INVALID interrupt.
// MODEM_RSSI_JUMP_THRESH - RSSI jumping detection threshold, step in 1 dB.
// MODEM_RSSI_CONTROL - Selects if the RSSI value is latched, and at what point in the packet it is latched. The Latched RSSI value may be read by a Fast Response Register, or returned by the GET_MODEM_STATUS command. Selects whether the RSSI value is updated every 1*Tb bit period, or whether the RSSI value is averaged over the previous 4*Tb bit periods. Selects if the Latched RSSI value is compared against the MODEM_RSSI_THRESH value, for the purpose of exiting to the RXTIMEOUT_STATE if below threshold.
*/
//#define RF_MODEM_OOK_CNT1_11 0x11, 0x20, 0x0B, 0x42, 0x84, 0x03, 0xD6, 0x83, 0x00, 0xA4, 0x01, 0x80, 0xFF, 0x0C, 0x02        // 02 = latch RSSI when sync word detected
#define RF_MODEM_OOK_CNT1_11 0x11, 0x20, 0x0B, 0x42, 0x84, 0x03, 0xD6, 0x83, 0x00, 0xA4, 0x01, 0x80, 0x58, 0x0C, 0x03        // 03 = latch RSSI 4*Tb, RSSI threshold at -90dBm

/*
// Set properties: RF_MODEM_RSSI_COMP_1
// Number of properties: 1
// Group ID: 20
// Start ID: 4E
// Default values: 0x32,
// Descriptions:
// MODEM_RSSI_COMP - Offsets RSSI curve in 1 dB steps. 32 is no offset, lower will adjust RSSI down, and higher will adjust RSSI up.
*/
#define RF_MODEM_RSSI_COMP_1 0x11, 0x20, 0x01, 0x4E, 0x40

/*
// Set properties: RF_MODEM_CLKGEN_BAND_1
// Number of properties: 1
// Group ID: 20
// Start ID: 51
// Default values: 0x08,
// Descriptions:
// MODEM_CLKGEN_BAND - Selects the divide ratio of the configurable divider at the output of the PLL Synthesizer as a function of the desired operating frequency band. Configures the PLL Synthesizer for High Performance or Low Power operating mode, allowing a tradeoff between tuning resolution and current consumption.
*/
#define RF_MODEM_CLKGEN_BAND_1 0x11, 0x20, 0x01, 0x51, 0x0D

/*
// Set properties: RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12
// Number of properties: 12
// Group ID: 21
// Start ID: 00
// Default values: 0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01,
// Descriptions:
// MODEM_CHFLT_RX1_CHFLT_COE13_7_0 - The chip implements the RX channel selection bandpass filtering in the digital domain as an FIR filter. The MODEM_CHFLT_RX_CHFLT_COEXX properties define the values for the filter tap coefficients. The chip provides for two different sets of RX filter coefficients (MODEM_CHFLT_RX1 and MODEM_CHFLT_RX2). These properties define the values for the first set of filter coefficients; see also the text description for the MODEM_CHFLT_RX2_CHFLT_COEXX properties for a discussion of the second set of filter coefficients and use of adaptive RX filter bandwidth across the packet. By default, the digital filter is 27 taps in length. However, it is possible to configure the RX Modem for a channel selection filter with only 15 taps; the advantage of the 15-tap filter is a reduction in filter processing delay at the expense of reduced filtering performance (e.g., adjacent channel selectivity). Please contact Silicon Labs for assistance with configuring the reduced performance filter. The values of the tap coefficients are symmetrical; that is, the value of COEFF26=COEFF0, COEFF25=COEFF1, and so on. Thus it is only necessary to store 14 filter coefficients; the internal circuitry obtains the remaining coefficients through symmetry. Each filter tap coefficient is a 10-bit signed value. The lower 8-bits of each coefficient are held in the MODEM_CHFLT_RX1_CHFLT_COEXX properties; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEMXX properties. Silicon Labs has pre-calculated 15 different sets of filter tap coefficients. The WDS Calculator will recommend one of these filter sets, based upon the RX filter bandwidth required to receive the desired signal. The filter bandwidth is a function of both the selected filter set, as well as the filter clock decimation ratio (see the MODEM_DECIMATION_CFG1/0 properties). This property sets the lower 8-bits of the 13th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property.
// MODEM_CHFLT_RX1_CHFLT_COE12_7_0 - This property sets the lower 8-bits of the 12th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE11_7_0 - This property sets the lower 8-bits of the 11th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE10_7_0 - This property sets the lower 8-bits of the 10th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE9_7_0 - This property sets the lower 8-bits of the 9th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE8_7_0 - This property sets the lower 8-bits of the 8th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE7_7_0 - This property sets the lower 8-bits of the 7th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE6_7_0 - This property sets the lower 8-bits of the 6th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE5_7_0 - This property sets the lower 8-bits of the 5th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE4_7_0 - This property sets the lower 8-bits of the 4th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE3_7_0 - This property sets the lower 8-bits of the 3rd filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE2_7_0 - This property sets the lower 8-bits of the 2nd filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
*/
#define RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C

/*
// Set properties: RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12
// Number of properties: 12
// Group ID: 21
// Start ID: 0C
// Default values: 0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5,
// Descriptions:
// MODEM_CHFLT_RX1_CHFLT_COE1_7_0 - This property sets the lower 8-bits of the 1st filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COE0_7_0 - This property sets the lower 8-bits of the 0th filter coefficient for the first set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX1_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX1_CHFLT_COEM0 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 13th through 10th filter coefficients for the first set of filter coefficients.
// MODEM_CHFLT_RX1_CHFLT_COEM1 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 9th through 6th filter coefficients for the first set of filter coefficients.
// MODEM_CHFLT_RX1_CHFLT_COEM2 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 5th through 2nd filter coefficients for the first set of filter coefficients.
// MODEM_CHFLT_RX1_CHFLT_COEM3 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 1st through 0th filter coefficients for the first set of filter coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE13_7_0 - The chip provides for two different sets of RX filter coefficients (MODEM_CHFLT_RX1 and MODEM_CHFLT_RX2). These properties define the values for the second set of filter coefficients; see also the text description for the MODEM_CHFLT_RX1_CHFLT_COEXX properties for a general discussion of the configuration of filter tap coefficients. Two different sets of filter tap coefficients (i.e., filter bandwidths) are provided to allow narrowing the RX bandwidth after the PLL-based AFC algorithm has acquired the signal and centered it in the filter passband. With no residual frequency error, the filter bandwidth may be reduced to pass only the modulation bandwidth of the signal, thus improving sensitivity over the remainder of the packet. This adaptive RX bandwidth feature is only applicable when PLL AFC is enabled (by setting MODEM_AFC_MISC:ENFBPLL), and the adaptive RX bandwidth function is enabled (by setting MODEM_AFC_GAIN_1:AFCBD). When the adaptive RX bandwidth feature is enabled, the first set of filter coefficients (defined in the MODEM_CHFLT_RX1 properties) is effective prior to gear-switching (e.g., detection of PREAMBLE_VALID) while the second set of filter coefficients (defined in the MODEM_CHFLT_RX2 properties) is effective after gear-switching. When the adaptive RX bandwidth feature is disabled, the first set of filter coefficients is used for the entire packet. This property sets the lower 8-bits of the 13th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property.
// MODEM_CHFLT_RX2_CHFLT_COE12_7_0 - This property sets the lower 8-bits of the 12th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE11_7_0 - This property sets the lower 8-bits of the 11th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE10_7_0 - This property sets the lower 8-bits of the 10th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM0 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE9_7_0 - This property sets the lower 8-bits of the 9th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_1 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE8_7_0 - This property sets the lower 8-bits of the 8th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
*/
#define RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5

/*
// Set properties: RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12
// Number of properties: 12
// Group ID: 21
// Start ID: 18
// Default values: 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00,
// Descriptions:
// MODEM_CHFLT_RX2_CHFLT_COE7_7_0 - This property sets the lower 8-bits of the 7th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_1 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE6_7_0 - This property sets the lower 8-bits of the 6th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM1 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_1 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE5_7_0 - This property sets the lower 8-bits of the 5th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE4_7_0 - This property sets the lower 8-bits of the 4th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE3_7_0 - This property sets the lower 8-bits of the 3rd filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE2_7_0 - This property sets the lower 8-bits of the 2nd filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM2 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE1_7_0 - This property sets the lower 8-bits of the 1st filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COE0_7_0 - This property sets the lower 8-bits of the 0th filter coefficient for the second set of filter coefficients; the top two bits are packed into the MODEM_CHFLT_RX2_CHFLT_COEM3 property. Please refer to the text description for MODEM_CHFLT_RX1_CHFLT_COE13_7_0 for more details regarding programming of the channel selection filter tap coefficients.
// MODEM_CHFLT_RX2_CHFLT_COEM0 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 13th through 10th filter coefficients for the second set of filter coefficients.
// MODEM_CHFLT_RX2_CHFLT_COEM1 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 9th through 6th filter coefficients for the second set of filter coefficients.
// MODEM_CHFLT_RX2_CHFLT_COEM2 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 5th through 2nd filter coefficients for the second set of filter coefficients.
// MODEM_CHFLT_RX2_CHFLT_COEM3 - There are 14 filter tap coefficient values; each value is a signed 10-bit value. The lower 8-bits of each coefficient are set in the MODEM_CHFLT_RX_CHFLT_COEXX properties, while the top two bits are packed into the MODEM_CHFLT_RX_CHFLT_COEMXX properties. This property contains the top two bits of the 1st through 0th filter coefficients for the second set of filter coefficients.
*/
#define RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00

/*
// Set properties: RF_SYNTH_PFDCP_CPFF_7
// Number of properties: 7
// Group ID: 23
// Start ID: 00
// Default values: 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03,
// Descriptions:
// SYNTH_PFDCP_CPFF - The charge pump and loop filter in the PLL Synthesizer have two paths: an integral path and a feed-forward path. This property adjusts the charge pump gain (i.e., current) for the feed-forward path. The coding of the MSB (bit 5) of the CP_FF_CUR field is inverted. As a result, the minimum charge pump gain setting is obtained for a value of CP_FF_CUR=0x20, while CP_FF_CUR=0x00 corresponds to a mid-range charge pump gain setting and 0x1F corresponds to the maximum charge pump gain setting. The step size or resolution of the charge pump feed-forward current is in increments of 5 uA. Thus the range of values provided by the CP_FF_CUR field is from 0 uA to 315 uA. CP_FF_CUR_TEST is a test bit and is not recommended for general customer applications. The CP_FF_CUR_TEST bit is not binary-weighted with the CP_FF_CUR field but instead provides an additional 160 uA, when set. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
// SYNTH_PFDCP_CPINT - The charge pump and loop filter in the PLL Synthesizer have two paths: an integral path and a feed-forward path. This property adjusts the charge pump gain (i.e., current) for the integral path. The coding of the MSB (bit 3) of the CP_INT_CUR field is inverted. As a result, the minimum charge pump gain setting is obtained for a value of CP_INT_CUR=0x08, while CP_INT_CUR=0x00 corresponds to a mid-range charge pump gain setting and 0x07 corresponds to the maximum charge pump gain setting. The step size or resolution of the charge pump feed-forward current is in increments of 5 uA. Thus the range of values provided by the CP_INT_CUR field is from 0 uA to 75 uA. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
// SYNTH_VCO_KV - The charge pump, loop filter, and VCO tuning varactors in the PLL Synthesizer have two paths: an integral path and a feed-forward path. This property adjusts the gain scaling factors (Kv) for the tuning varactors in both the integral path and the feed-forward path. The adjustment of the gain factors is accomplished internally by switching in different number of varactor devices. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
// SYNTH_LPFILT3 - The loop filter in the feed-forward path contains three lowpass filter poles: R1-C1, R2-C2, and R3-C3. The LPF_FF_R2 field adjusts the value of resistor R2, and thus adjusts the cutoff frequency of the R2-C2 lowpass filter pole. Additionally, the resistor R2 affects the DC gain of the transistor Gm stage in the feed-forward path. Increasing the value of R2 has the effect of increasing the feed-forward path gain. The value of R2 is in increments of 18 kOhm, ranging from a minimum value of 18 kOhm to a maximum of 144 kOhm. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
// SYNTH_LPFILT2 - The loop filter in the feed-forward path contains three lowpass filter poles: R1-C1, R2-C2, and R3-C3. The LPF_FF_C2 field adjusts the value of capacitor C2, and thus adjusts the cutoff frequency of the R2-C2 lowpass filter pole. The value of C2 is in increments of 335 fF, ranging from a minimum value of 877 fF to a maximum of 11.25 pF. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
// SYNTH_LPFILT1 - The loop filter in the feed-forward path contains three lowpass filter poles: R1-C1, R2-C2, and R3-C3. The LPF_FF_C3 field adjusts the value of capacitor C3, and thus adjusts the cutoff frequency of the R3-C3 lowpass filter pole. The value of resistor R3 is not adjustable and is fixed at R3=3.94 kOhm. The value of C3 is in increments of 1 pF, ranging from a minimum value of 9 pF to a maximum of 12 pF. The total value of capacitor C1 is determined by a 3-bit binary-weighted field LPF_FF_C1 in increments of 380 fF, and a 2-bit offset field LPF_FF_C1_CODE in increments of 1 pF, plus an additional fixed capacitance of 4.55 pF. Thus the total value of capacitor C1 may be described by the following equation: C1 = 4.55pF + 380fF*LPF_FF_C1 + 1pF*LPF_FF_C1_CODE The value of resistor R1 is not adjustable and is fixed at R1=9.86 kOhm. NOTE: the primary purpose of the SYNTH group of properties is to adjust the PLL loop bandwidth to a value appropriate for the selected data rate. Silicon Labs does not recommend modifying these properties away from the values suggested by the WDS Calculator.
// SYNTH_LPFILT0 - Bias current of the loop filter, 25 uA, 34 uA, 50 uA or 100 uA.
*/
#define RF_SYNTH_PFDCP_CPFF_7 0x11, 0x23, 0x07, 0x00, 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03

/*
// Set properties: RF_MATCH_VALUE_1_12
// Number of properties: 12
// Group ID: 30
// Start ID: 00
// Default values: 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
// Descriptions:
// MATCH_VALUE_1 -
// MATCH_MASK_1 -
// MATCH_CTRL_1 - Enable packet match processing and pattern 1 matches or not.
// MATCH_VALUE_2 -
// MATCH_MASK_2 -
// MATCH_CTRL_2 - Enable pattern 2 matches or not.
// MATCH_VALUE_3 -
// MATCH_MASK_3 -
// MATCH_CTRL_3 - Enable pattern 3 matches or not.
// MATCH_VALUE_4 -
// MATCH_MASK_4 -
// MATCH_CTRL_4 - Enable pattern 4 matches or not.
*/
#define RF_MATCH_VALUE_1_12 0x11, 0x30, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties: RF_FREQ_CONTROL_INTE_8
// Number of properties: 8
// Group ID: 40
// Start ID: 00
// Default values: 0x3C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF,
// Descriptions:
// FREQ_CONTROL_INTE - The total divide ratio for the Fractional-N PLL Synthesizer consists of an integer portion and a fractional portion. This property defines the integer divide number; the fractional divide value is specified in properties FREQ_CONTROL_FRAC_2, FREQ_CONTROL_FRAC_1, and FREQ_CONTROL_FRAC_0. The formula for calculating RF channel frequency as a function of integer and fractional divide values is as follows: The output divider value OUTDIV is configured as a function of the desired frequency band, and is specified in property MODEM_CLKGEN_BAND:BAND. The entire FC_FRAC word is 20-bits in length, but the MSB should always be set to 1, and thus the term FC_FRAC/2^19 will always be between 1 and 2 in value. As a result, the integer term FC_INTE should be reduced by 1. Example: a total desired divide ratio of N = 60.135 should be implemented as FC_INTE = 59, FC_FRAC/2^19 = 1.135. Modifying this property will have no effect until the chip exits from TX or RX state, and then re-enters the state.
// FREQ_CONTROL_FRAC_2 - Please refer to the text description for FREQ_CONTROL_INTE for more details regarding frequency calculation.
// FREQ_CONTROL_FRAC_1 - Please refer to the text description for FREQ_CONTROL_INTE for more details regarding frequency calculation.
// FREQ_CONTROL_FRAC_0 - Please refer to the text description for FREQ_CONTROL_INTE for more details regarding frequency calculation.
// FREQ_CONTROL_CHANNEL_STEP_SIZE_1 - The EZ Frequency Programming method allows the user to control the frequency by selecting a desired channel number, given a base frequency and a channel step size. This property defines Byte 1 of the unsigned 16-bit channel step size value. The units of the FREQ_CONTROL_CHANNEL_STEP_SIZE_1/0 property are in increments of the LSB tuning resolution of the PLL Synthesizer, and are therefore a function of the crystal reference frequency and output frequency band. The formula for calculating the value of the FREQ_CONTROL_CHANNEL_STEP_SIZE property is given by the following equation: The base frequency is specified by the FREQ_CONTROL_INTE and FREQ_CONTROL_FRAC_2/1/0 properties. The channel number is specified by the CHANNEL parameter byte passed to the START_TX command. The commanded channel frequency is given by the formula:
// FREQ_CONTROL_CHANNEL_STEP_SIZE_0 - The EZ Frequency Programming method allows the user to control the frequency by selecting a desired channel number, given a base frequency and a channel step size. This property defines Byte 0 of the unsigned 16-bit channel step size value. Please refer to the text description for FREQ_CONTROL_CHANNEL_STEP_SIZE_1 for more details regarding the channel step size function.
// FREQ_CONTROL_W_SIZE - The chip performs a calibration of the VCO at each new commanded frequency. This is accomplished by counting cycles of the VCO frequency and comparing to an expected target count value. The length of the window during which cycles of the VCO frequency are counted is specified by this property. The units are in cycles of the crystal reference frequency (e.g., 30 MHz clock periods). This property does not need to change as a function of crystal reference frequency; the chip automatically calculates the target VCO count value as a function of crystal reference frequency and thus this property may remain constant. Silicon Labs recommends setting this property always to 0x20.
// FREQ_CONTROL_VCOCNT_RX_ADJ - The chip performs a calibration of the VCO at each new commanded frequency. This is accomplished by counting cycles of the VCO frequency and comparing to an expected target count value. As the chip's default mode of operation uses a low-IF architecture with low-side Mixer injection, the RX LO must shift downwards in frequency during RX mode. The expected target count value changes as a result of this shift in frequency. This field is a signed value that represents the amount by which the target count value must be adjusted in RX mode.
*/
#define RF_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x3F, 0x0E, 0x51, 0xEB, 0x28, 0xF6, 0x20, 0xFA

// AUTOMATICALLY GENERATED CODE!
// DO NOT EDIT/MODIFY BELOW THIS LINE!
// --------------------------------------------

#ifndef FIRMWARE_LOAD_COMPILE
#define RADIO_CONFIGURATION_DATA_ARRAY { \
0x07, RF_POWER_UP, \
0x07, RF_GPIO_PIN_CFG, \
0x05, RF_GLOBAL_XO_TUNE_1, \
0x05, RF_GLOBAL_CONFIG_1, \
0x05, RF_INT_CTL_ENABLE_1, \
0x08, RF_FRR_CTL_A_MODE_4, \
0x0D, RF_PREAMBLE_TX_LENGTH_9, \
0x09, RF_SYNC_CONFIG_5, \
0x05, RF_PKT_CRC_CONFIG_1, \
0x05, RF_PKT_CONFIG1_1, \
0x07, RF_PKT_LEN_3, \
0x10, RF_PKT_FIELD_1_LENGTH_12_8_12, \
0x0C, RF_PKT_FIELD_4_LENGTH_12_8_8, \
0x10, RF_MODEM_MOD_TYPE_12, \
0x05, RF_MODEM_FREQ_DEV_0_1, \
0x0B, RF_MODEM_MDM_CTRL_7, \
0x0D, RF_MODEM_BCR_OSR_1_9, \
0x0B, RF_MODEM_AFC_GEAR_7, \
0x05, RF_MODEM_AGC_CONTROL_1, \
0x0D, RF_MODEM_AGC_WINDOW_SIZE_9, \
0x0F, RF_MODEM_OOK_CNT1_11, \
0x05, RF_MODEM_RSSI_COMP_1, \
0x05, RF_MODEM_CLKGEN_BAND_1, \
0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \
0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \
0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \
0x0B, RF_SYNTH_PFDCP_CPFF_7, \
0x10, RF_MATCH_VALUE_1_12, \
0x0C, RF_FREQ_CONTROL_INTE_8, \
0x00 \
}
#else
#define RADIO_CONFIGURATION_DATA_ARRAY { 0 }
#endif

// DEFAULT VALUES FOR CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT 30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT 0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT 0x10
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT 0x01
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT 0x1000

#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_INCLUDED 0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_SIZE 0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH { }

#ifndef RADIO_CONFIGURATION_DATA_ARRAY
#error "This property must be defined!"
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ { RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER { RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH { RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP { RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT }
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET { RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT }
#endif

#define RADIO_CONFIGURATION_DATA { \
Radio_Configuration_Data_Array, \
RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER, \
RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH, \
RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP, \
RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET \
}

#endif /* RADIO_CONFIG_H_ */
