/*
 * 	ads1298r.h
 *
 *  Created on: 20th Jan. 2023
 *      Author: Ruben Nieto (URJC)
 */

#ifndef ads1298r_h
#define ads1298r_h

#include "math.h"

// ADS supported chips:
#define ADS_1294 1
#define ADS_1294R 2
#define ADS_1296 3
#define ADS_1296R 4
#define ADS_1298 5
#define ADS_1298R 6

/* ============ User editable ============= */
#define ADS_CHIP_USED 	ADS_1298R
// Number of bits per channel that ADS chip will sent over SPI. It doesn't mean that sample was adquiered with this resolution.
// The number of bits per channel depends with data rate (sample frequency). See page 53, section 9.4.1.3.2 Readback length in
// the datasheet for more information
// For 32 or 64 kSPS (kilo sample per second) data rate, use 16. For all another data rates (from 250 SPS to 16 kSPS), use 24
//
// Note: Datasheet says in 32 kSPS and 64 kSPS, the bits per channel send by ADS is 16 BUT ADS max sample frequency is 32 kSPS.
//       I think that maybe datasheet wanted to say 16 kSPS instead of 64 kSPS. I don't know. I used the datasheet, revision K as reference.
//       If datasheet is wrong, in ads129xDatasheetConstants.h is incorrect (look for usage of ADS_BITS_PER_CHANNEL)
#define ADS_BITS_PER_CHANNEL 24 // Could be 17 (32 ksps), 19 (16 ksps) or 24 (<=8 ksps).

// This define specifies how much verbosity you want when you use the library. It can take the following valuesre:
//    0 -> No messages (recomended if memory program is critical. It avoids to import Serial library and send message through Serial.print() )
//    1-> only errors and a few selected messages (recommended)
//    2-> all (for debug purposes only)
// ToDo: #define ADS_LIBRARY_VERBOSE_LEVEL 1 // 0, 1, 2

/* ============== DON'T TOUCH =========== */
// For each ADS type, we define the number of the channels (ADS_N_CHANNELS) and if has integrated the respiration module to
// mesure respiration without external hardware (ADS_HAS_RESPIRATION_MODULE).

#if ADS_CHIP_USED == ADS_1294
#define ADS_N_CHANNELS 4 // Number of channels that ADS has. Could be 4, 6 or 8
#define ADS_HAS_RESPIRATION_MODULE false // If ADS has the respiration module (if its coding ends with R or not).

#elif ADS_CHIP_USED == ADS_1294R
#define ADS_N_CHANNELS 4 // Number of channels that ADS has. Could be 4, 6 or 8
#define ADS_HAS_RESPIRATION_MODULE true // If ADS has the respiration module (if its coding ends with R or not).

#elif ADS_CHIP_USED == ADS_1296
#define ADS_N_CHANNELS 6 // Number of channels that ADS has. Could be 4, 6 or 8
#define ADS_HAS_RESPIRATION_MODULE false // If ADS has the respiration module (if its coding ends with R or not).

#elif ADS_CHIP_USED == ADS_1296R
#define ADS_N_CHANNELS 6 // Number of channels that ADS has. Could be 4, 6 or 8
#define ADS_HAS_RESPIRATION_MODULE true // If ADS has the respiration module (if its coding ends with R or not).

#elif ADS_CHIP_USED == ADS_1298
#define ADS_N_CHANNELS 8 // Number of channels that ADS has. Could be 4, 6 or 8
#define ADS_HAS_RESPIRATION_MODULE false // If ADS has the respiration module (if its coding ends with R or not).

#elif ADS_CHIP_USED == ADS_1298R
#define ADS_N_CHANNELS 8 // Number of channels that ADS has. Could be 4, 6 or 8
#define ADS_HAS_RESPIRATION_MODULE true // If ADS has the respiration module (if its coding ends with R or not).

#else
"The value set in ADS_CHIP_USED is not in ADS supported chips defines!!!!!!"
#endif


// Definitions
// Time constants. See section 7.6, Timing Requirements: Serial Interface in the datasheet
// See page 17, section 9 7.6 Timing Requirements: Serial Interface, in the datasheet.
#define _ADS_FREQ_MHZ XPAR_XQSPIPS_0_QSPI_CLK_FREQ_HZ/1000000 // 4 MHz
// #define _ADS_T_CLK  ceil(1/_ADS_FREQ_MHZ) // us 0.25 us
#define _ADS_T_CLK  4 // us 0.25 us
// #define _ADS_T_CLK_2   ceil(2 * _ADS_T_CLK) // us
#define _ADS_T_CLK_2   ceil(2 * _ADS_T_CLK) // us
// #define _ADS_T_CLK_4   ceil(4 * _ADS_T_CLK) // us
#define _ADS_T_CLK_4   ceil(4 * _ADS_T_CLK) // us
// #define _ADS_T_CLK_18  ceil(18 * _ADS_T_CLK) // us
#define _ADS_T_CLK_18  ceil(18 * _ADS_T_CLK) // 4.5 us fot 4MHz SPI_T_CLK

// Timing requeriments for SPI interface. *** IGNORED ***
// #define _ADS_T_CSSC 1 // Is 6 or 17 ns but it doesn't have a function to wait nanoseconds.
// #define _ADS_T_SCCS _ADS_T_CLK_4
// #define _ADS_T_CSH _ADS_T_CLK_2

#define _ADS_POWER_UP_DELAY_US 1000000

/*** ADS 129xX commands ***/
// system commands
#define COMMAND_WAKEUP 0x02
#define COMMAND_STANDBY 0x04
#define COMMAND_RESET 0x06
#define COMMAND_START 0x08
#define COMMAND_STOP 0x0a

// read commands
#define COMMAND_RDATAC 0x10
#define COMMAND_SDATAC 0x11
#define COMMAND_RDATA 0x12

// register commands
#define COMMAND_RREG 0x20
#define COMMAND_WREG 0x40

// ** Registers **
// ** ID **
#define REGID_REG_ADDR  0x00
#define REGID_RESERVED_BITS  0x10
#define REGID_READ_ONLY_REGISTER  true

#define REGID_B_DEV_ID7  0x80
#define REGID_B_DEV_ID6  0x40
#define REGID_B_DEV_ID5  0x20
#define REGID_B_DEV_ID2  0x04
#define REGID_B_DEV_ID1  0x02
#define REGID_B_DEV_ID0  0x01

#define REGID_ID_ADS129x  REGID_B_DEV_ID7
#define REGID_ID_ADS129xR  (REGID_B_DEV_ID7 | REGID_B_DEV_ID6)

#define REGID_ID_4CHAN  0x00
#define REGID_ID_6CHAN  REGID_B_DEV_ID0
#define REGID_ID_8CHAN  REGID_B_DEV_ID1

#define REGID_ID_ADS1294  (REGID_ID_ADS129x | REGID_ID_4CHAN | REGID_RESERVED_BITS)
#define REGID_ID_ADS1296  (REGID_ID_ADS129x | REGID_ID_6CHAN | REGID_RESERVED_BITS)
#define REGID_ID_ADS1298  (REGID_ID_ADS129x | REGID_ID_8CHAN | REGID_RESERVED_BITS)
#define REGID_ID_ADS1294R  (REGID_ID_ADS129xR | REGID_ID_4CHAN | REGID_RESERVED_BITS)
#define REGID_ID_ADS1296R  (REGID_ID_ADS129xR | REGID_ID_6CHAN | REGID_RESERVED_BITS)
#define REGID_ID_ADS1298R  (REGID_ID_ADS129xR | REGID_ID_8CHAN | REGID_RESERVED_BITS)

// ** CONFIG1 **
#define CONFIG1_REG_ADDR  0X01
#define CONFIG1_RESERVED_BITS   0x00
#define CONFIG1_READ_ONLY_REGISTER  false
#define CONFIG1_RESET_VALUE  0x06

// Constants helpers
#define CONFIG1_B_HR  0x80
#define CONFIG1_B_DR2  0x04
#define CONFIG1_B_DR1  0x02
#define CONFIG1_B_DR0  0x01

#define CONFIG1_B_DAISY_EN  0x40
#define CONFIG1_B_CLK_EN  0x20

// FIXME: Datasheet says in 32 kSPS and 64 kSPS, the bits per channel send by ADS is 16 BUT ADS max sample frequency is 32 kSPS. Check it page 53, Readback length) when new version of the datasheet is available.
// High resolution mode
#if ADS_BITS_PER_CHANNEL == 17
#define CONFIG1_HIGH_RES_32k_SPS  (CONFIG1_B_HR | CONFIG1_RESERVED_BITS)
#elif ADS_BITS_PER_CHANNEL == 19
#define CONFIG1_HIGH_RES_16k_SPS  (CONFIG1_B_HR | CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
// Low resolution mode
#define CONFIG1_LOW_POWR_16k_SPS  (0x00 | CONFIG1_RESERVED_BITS)
#elif ADS_BITS_PER_CHANNEL == 24

#define CONFIG1_HIGH_RES_8k_SPS  (CONFIG1_B_HR | CONFIG1_B_DR1 | CONFIG1_RESERVED_BITS)
#define CONFIG1_HIGH_RES_4k_SPS  (CONFIG1_B_HR | CONFIG1_B_DR1 | CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
#define CONFIG1_HIGH_RES_2k_SPS  (CONFIG1_B_HR | CONFIG1_B_DR2 | CONFIG1_RESERVED_BITS)
#define CONFIG1_HIGH_RES_1k_SPS  (CONFIG1_B_HR | CONFIG1_B_DR2 | CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
#define CONFIG1_HIGH_RES_500_SPS  (CONFIG1_B_HR | CONFIG1_B_DR2 | CONFIG1_B_DR1 | CONFIG1_RESERVED_BITS)

// Low resolution mode
#define CONFIG1_LOW_POWR_8k_SPS  (CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
#define CONFIG1_LOW_POWR_4k_SPS  (CONFIG1_B_DR1 | CONFIG1_RESERVED_BITS)
#define CONFIG1_LOW_POWR_2k_SPS  (CONFIG1_B_DR1 | CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
#define CONFIG1_LOW_POWR_1k_SPS  (CONFIG1_B_DR2 | CONFIG1_RESERVED_BITS)
#define CONFIG1_LOW_POWR_500_SPS  (CONFIG1_B_DR2 | CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
#define CONFIG1_LOW_POWR_250_SPS  (CONFIG1_B_DR2 | CONFIG1_B_DR0 | CONFIG1_RESERVED_BITS)
#endif

// ** CONFIG2 **
#define CONFIG2_REG_ADDR  0x02
#define CONFIG2_RESERVED_BITS  0x00
#define CONFIG2_READ_ONLY_REGISTER  false
// FIXME: Datasheet says that RESET_VALUE is 0x40 BUT also says that in bit 7 and 8, zero must be written. I check the value and it is zero for ADS1294. So I put here 0x00. Check it page 68, Config2: configuration ...) when new version of the datasheet is available.
#define CONFIG2_RESET_VALUE  0x00

#define CONFIG2_B_WCT_CHOP  0x20
#define CONFIG2_B_INT_TEST  0x10
#define CONFIG2_B_TEST_AMP  0x04

#define CONFIG2_TEST_SOURCE_EXTERNAL  (CONFIG2_RESERVED_BITS)
#define CONFIG2_TEST_SOURCE_INTERNAL  (CONFIG2_B_INT_TEST | CONFIG2_RESERVED_BITS)

#define CONFIG2_TEST_FREQ_2HZ  (CONFIG2_B_INT_TEST | 0x00 | CONFIG2_RESERVED_BITS)
#define CONFIG2_TEST_FREQ_4HZ  (CONFIG2_B_INT_TEST | 0x01 | CONFIG2_RESERVED_BITS)
#define CONFIG2_TEST_FREQ_DC  (CONFIG2_B_INT_TEST | 0x03 | CONFIG2_RESERVED_BITS)

// ** CONFIG3 **
#define CONFIG3_REG_ADDR  0X03
#define CONFIG3_RESERVED_BITS  0x40
#define CONFIG3_READ_ONLY_REGISTER  false
#define CONFIG3_RESET_VALUE  0x40
// Remember to wait 150 microseconds if internal reference will be used. See page 15, section Electrical Characteristicsm - Internal Reference, in the datasheet
#define CONFIG3_B_PD_REFBUF  0x80
#define CONFIG3_B_VREF_4V  0x20
#define CONFIG3_B_RLD_MEAS  0x10
#define CONFIG3_B_RLDREF_INT  0x08
#define CONFIG3_B_PD_RLD  0x04
#define CONFIG3_B_RLD_LOFF_SENS  0x02
#define CONFIG3_B_RLD_STAT  0x01

// ** LOFF **
#define LOFF_REG_ADDR  0X04
#define LOFF_RESERVED_BITS  0x00
#define LOFF_READ_ONLY_REGISTER  false
#define LOFF_RESET_VALUE  0x00

#define LOFF_B_COMP_TH2  0x80
#define LOFF_B_COMP_TH1  0x40
#define LOFF_B_COMP_TH0  0x20
#define LOFF_B_VLEAD_OFF_EN  0x10
#define LOFF_B_ILEAD_OFF1  0x08
#define LOFF_B_ILEAD_OFF0  0x04
#define LOFF_B_FLEAD_OFF1  0x02
#define LOFF_B_FLEAD_OFF0  0x01

#define LOFF_COMP_TH_95  (0x00 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_92_5  (LOFF_B_COMP_TH0 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_90  (LOFF_B_COMP_TH1 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_87_5  (LOFF_B_COMP_TH1 | LOFF_B_COMP_TH0 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_85  (LOFF_B_COMP_TH2 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_80  (LOFF_B_COMP_TH2 | LOFF_B_COMP_TH0 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_75  (LOFF_B_COMP_TH2 | LOFF_B_COMP_TH1 | LOFF_RESERVED_BITS)
#define LOFF_COMP_TH_70  (LOFF_B_COMP_TH2 | LOFF_B_COMP_TH1 | LOFF_B_COMP_TH0 | LOFF_RESERVED_BITS)

#define LOFF_ILEAD_OFF_6nA  (0x00 | LOFF_RESERVED_BITS)
#define LOFF_ILEAD_OFF_12nA  (LOFF_B_ILEAD_OFF0 | LOFF_RESERVED_BITS)
#define LOFF_ILEAD_OFF_18nA  (LOFF_B_ILEAD_OFF1 | LOFF_RESERVED_BITS)
#define LOFF_ILEAD_OFF_24nA  (LOFF_B_ILEAD_OFF1 | LOFF_B_ILEAD_OFF0 | LOFF_RESERVED_BITS)

#define LOFF_FLEAD_OFF_AC  (LOFF_B_FLEAD_OFF0 | LOFF_RESERVED_BITS)
#define LOFF_FLEAD_OFF_DC  (LOFF_B_FLEAD_OFF1 | LOFF_B_FLEAD_OFF0 | LOFF_RESERVED_BITS)

// ** CHNSET **
#define CHNSET__BASE_REG_ADDR  0x04 // Base register address
#define CHNSET_REG_ADDR_CH1SET  CHNSET__BASE_REG_ADDR + 1
#define CHNSET_REG_ADDR_CH2SET  CHNSET__BASE_REG_ADDR + 2
#define CHNSET_REG_ADDR_CH3SET  CHNSET__BASE_REG_ADDR + 3
#define CHNSET_REG_ADDR_CH4SET  CHNSET__BASE_REG_ADDR + 4

#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define CHNSET_REG_ADDR_CH5SET  CHNSET__BASE_REG_ADDR + 5
#define CHNSET_REG_ADDR_CH6SET  CHNSET__BASE_REG_ADDR + 6
#endif
#if ADS_N_CHANNELS > 6 // ADS1298 or ADS1298R
#define CHNSET_REG_ADDR_CH7SET  CHNSET__BASE_REG_ADDR + 7
#define CHNSET_REG_ADDR_CH8SET  CHNSET__BASE_REG_ADDR + 8
#endif

#define CHNSET_RESERVED_BITS  0x00
#define CHNSET_READ_ONLY_REGISTER  false
#define CHNSET_RESET_VALUE  0x00

#define CHNSET_B_PDn  0x80
#define CHNSET_B_GAINn2  0x40
#define CHNSET_B_GAINn1  0x20
#define CHNSET_B_GAINn0  0x10
#define CHNSET_B_MUXn2  0x04
#define CHNSET_B_MUXn1  0x02
#define CHNSET_B_MUXn0  0x01

#define CHNSET_ENABLE_CHANNEL  CHNSET_RESERVED_BITS
#define CHNSET_DISABLE_CHANNEL  CHNSET_B_PDn | CHNSET_RESERVED_BITS

#define CHNSET_GAIN_1X  (CHNSET_B_GAINn0 | CHNSET_RESERVED_BITS)
#define CHNSET_GAIN_2X  (CHNSET_B_GAINn1 | CHNSET_RESERVED_BITS)
#define CHNSET_GAIN_3X  (CHNSET_B_GAINn1 | CHNSET_B_GAINn0 | CHNSET_RESERVED_BITS)
#define CHNSET_GAIN_4X  (CHNSET_B_GAINn2 | CHNSET_RESERVED_BITS)
#define CHNSET_GAIN_6X  (0x00 | CHNSET_RESERVED_BITS)
#define CHNSET_GAIN_8X  (CHNSET_B_GAINn2 | CHNSET_B_GAINn0 | CHNSET_RESERVED_BITS)
#define CHNSET_GAIN_12X  (CHNSET_B_GAINn2 | CHNSET_B_GAINn1 | CHNSET_RESERVED_BITS)

#define CHNSET_ELECTRODE_INPUT  (0x00 | CHNSET_RESERVED_BITS)
#define CHNSET_SHORTED  (CHNSET_B_MUXn0 | CHNSET_RESERVED_BITS)
#define CHNSET_RLD_INPUT  (CHNSET_B_MUXn1 | CHNSET_RESERVED_BITS)
#define CHNSET_MVDD  (CHNSET_B_MUXn1 | CHNSET_B_MUXn0 | CHNSET_RESERVED_BITS)
#define CHNSET_TEMP  (CHNSET_B_MUXn2 | CHNSET_RESERVED_BITS)
#define CHNSET_TEST_SIGNAL  (CHNSET_B_MUXn2 | CHNSET_B_MUXn0 | CHNSET_RESERVED_BITS)
#define CHNSET_RLD_DRP  (CHNSET_B_MUXn2 | CHNSET_B_MUXn1 | CHNSET_RESERVED_BITS)
#define CHNSET_RLD_DRN  (CHNSET_B_MUXn2 | CHNSET_B_MUXn1 | CHNSET_B_MUXn0 | CHNSET_RESERVED_BITS)

// ** RLDSENSP **
#define RLDSENSP_REG_ADDR  0X0D
#define RLDSENSP_RESERVED_BITS  0x00
#define RLDSENSP_READ_ONLY_REGISTER  false
#define RLDSENSP_RESET_VALUE  0x00

#if ADS_N_CHANNELS > 6 // ADS1298 or ADS1298R
#define RLDSENSP_B_RLD8P  (0x80 | RLDSENSP_RESERVED_BITS)
#define RLDSENSP_B_RLD7P  (0x40 | RLDSENSP_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define RLDSENSP_B_RLD6P  (0x20 | RLDSENSP_RESERVED_BITS)
#define RLDSENSP_B_RLD5P  (0x10 | RLDSENSP_RESERVED_BITS)
#endif
#define RLDSENSP_B_RLD4P  (0x08 | RLDSENSP_RESERVED_BITS)
#define RLDSENSP_B_RLD3P  (0x04 | RLDSENSP_RESERVED_BITS)
#define RLDSENSP_B_RLD2P  (0x02 | RLDSENSP_RESERVED_BITS)
#define RLDSENSP_B_RLD1P  (0x01 | RLDSENSP_RESERVED_BITS)

// ** RLDSENSN **
#define RLDSENSN_REG_ADDR  0X0E
#define RLDSENSN_RESERVED_BITS  0x00
#define RLDSENSN_READ_ONLY_REGISTER  false
#define RLDSENSN_RESET_VALUE  0x00

#if ADS_N_CHANNELS > 6 // ADS1298 or ADS1298R
#define RLDSENSN_B_RLD8N  (0x80 | RLDSENSN_RESERVED_BITS)
#define RLDSENSN_B_RLD7N  (0x40 | RLDSENSN_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define RLDSENSN_B_RLD6N  (0x20 | RLDSENSN_RESERVED_BITS)
#define RLDSENSN_B_RLD5N  (0x10 | RLDSENSN_RESERVED_BITS)
#endif
#define RLDSENSN_B_RLD4N  (0x08 | RLDSENSN_RESERVED_BITS)
#define RLDSENSN_B_RLD3N  (0x04 | RLDSENSN_RESERVED_BITS)
#define RLDSENSN_B_RLD2N  (0x02 | RLDSENSN_RESERVED_BITS)
#define RLDSENSN_B_RLD1N  (0x01 | RLDSENSN_RESERVED_BITS)

// ** LOFFSENSP **
#define LOFFSENSP_REG_ADDR 0X0F
#define LOFFSENSP_RESERVED_BITS 0x00
#define LOFFSENSP_READ_ONLY_REGISTER false
#define LOFFSENSP_RESET_VALUE 0x00

#if ADS_N_CHANNELS > 6 // ADS1298 or ADS1298R
#define LOFFSENSP_B_LOFF8P (0x80 | LOFFSENSP_RESERVED_BITS)
#define LOFFSENSP_B_LOFF7P (0x40 | LOFFSENSP_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define LOFFSENSP_B_LOFF6P (0x20 | LOFFSENSP_RESERVED_BITS)
#define LOFFSENSP_B_LOFF5P (0x10 | LOFFSENSP_RESERVED_BITS)
#endif
#define LOFFSENSP_B_LOFF4P (0x08 | LOFFSENSP_RESERVED_BITS)
#define LOFFSENSP_B_LOFF3P (0x04 | LOFFSENSP_RESERVED_BITS)
#define LOFFSENSP_B_LOFF2P (0x02 | LOFFSENSP_RESERVED_BITS)
#define LOFFSENSP_B_LOFF1P (0x01 | LOFFSENSP_RESERVED_BITS)

// ** LOFFSENSN **
#define LOFFSENSN_REG_ADDR  0X10
#define LOFFSENSN_RESERVED_BITS  0x00
#define LOFFSENSN_READ_ONLY_REGISTER  false
#define LOFFSENSN_RESET_VALUE  0x00

#if ADS_N_CHANNELS > 6 // ADS1298 or ADS1298R
#define LOFFSENSN_B_LOFF8N (0x80 | LOFFSENSN_RESERVED_BITS)
#define LOFFSENSN_B_LOFF7N (0x40 | LOFFSENSN_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define LOFFSENSN_B_LOFF6N (0x20 | LOFFSENSN_RESERVED_BITS)
#define LOFFSENSN_B_LOFF5N (0x10 | LOFFSENSN_RESERVED_BITS)
#endif
#define LOFFSENSN_B_LOFF4N (0x08 | LOFFSENSN_RESERVED_BITS)
#define LOFFSENSN_B_LOFF3N (0x04 | LOFFSENSN_RESERVED_BITS)
#define LOFFSENSN_B_LOFF2N (0x02 | LOFFSENSN_RESERVED_BITS)
#define LOFFSENSN_B_LOFF1N (0x01 | LOFFSENSN_RESERVED_BITS)

// ** LOFFFLIP **
#define LOFFFLIP_REG_ADDR  0X11
#define LOFFFLIP_RESERVED_BITS  0x00
#define LOFFFLIP_READ_ONLY_REGISTER  false
#define LOFFFLIP_RESET_VALUE   0x00

#if ADS_N_CHANNELS > 6 //  ADS1298 or ADS1298R
#define LOFFFLIP_B_LOFF_FLIP8 (0x80 | LOFFFLIP_RESERVED_BITS)
#define LOFFFLIP_B_LOFF_FLIP7 (0x40 | LOFFFLIP_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define LOFFFLIP_B_LOFF_FLIP6 (0x20 | LOFFFLIP_RESERVED_BITS)
#define LOFFFLIP_B_LOFF_FLIP5 (0x10 | LOFFFLIP_RESERVED_BITS)
#endif
#define LOFFFLIP_B_LOFF_FLIP4 (0x08 | LOFFFLIP_RESERVED_BITS)
#define LOFFFLIP_B_LOFF_FLIP3 (0x04 | LOFFFLIP_RESERVED_BITS)
#define LOFFFLIP_B_LOFF_FLIP2 (0x02 | LOFFFLIP_RESERVED_BITS)
#define LOFFFLIP_B_LOFF_FLIP1 (0x01 | LOFFFLIP_RESERVED_BITS)

// ** LOFFSTATP **
#define LOFFSTATP_REG_ADDR  0X12
#define LOFFSTATP_RESERVED_BITS  0x00
#define LOFFSTATP_READ_ONLY_REGISTER  true

#if ADS_N_CHANNELS > 6 // ADS1298 or ADS1298R
#define LOFFSTATP_B_IN8P_OFF  (0x80 | LOFFSTATP_RESERVED_BITS)
#define LOFFSTATP_B_IN7P_OFF  (0x40 | LOFFSTATP_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define LOFFSTATP_B_IN6P_OFF  (0x20 | LOFFSTATP_RESERVED_BITS)
#define LOFFSTATP_B_IN5P_OFF  (0x10 | LOFFSTATP_RESERVED_BITS)
#endif
#define LOFFSTATP_B_IN4P_OFF  (0x08 | LOFFSTATP_RESERVED_BITS)
#define LOFFSTATP_B_IN3P_OFF  (0x04 | LOFFSTATP_RESERVED_BITS)
#define LOFFSTATP_B_IN2P_OFF  (0x02 | LOFFSTATP_RESERVED_BITS)
#define LOFFSTATP_B_IN1P_OFF  (0x01 | LOFFSTATP_RESERVED_BITS)

// ** LOFFSTATN **
#define LOFFSTATN_REG_ADDR  0x13
#define LOFFSTATN_RESERVED_BITS  0x00
#define LOFFSTATN_READ_ONLY_REGISTER  true

#if ADS_N_CHANNELS > 6 //  ADS1298 or ADS1298R
#define LOFFSTATN_B_IN8N_OFF  (0x80 | LOFFSTATN_RESERVED_BITS)
#define LOFFSTATN_B_IN7N_OFF  (0x40 | LOFFSTATN_RESERVED_BITS)
#endif
#if ADS_N_CHANNELS > 4 // ADS1296, ADS1296R , ADS1298 or ADS1298R
#define LOFFSTATN_B_IN6N_OFF  (0x20 | LOFFSTATN_RESERVED_BITS)
#define LOFFSTATN_B_IN5N_OFF  (0x10 | LOFFSTATN_RESERVED_BITS)
#endif
#define LOFFSTATN_B_IN4N_OFF  (0x08 | LOFFSTATN_RESERVED_BITS)
#define LOFFSTATN_B_IN3N_OFF  (0x04 | LOFFSTATN_RESERVED_BITS)
#define LOFFSTATN_B_IN2N_OFF  (0x02 | LOFFSTATN_RESERVED_BITS)
#define LOFFSTATN_B_IN1N_OFF  (0x01 | LOFFSTATN_RESERVED_BITS)

// ** GPIO **
#define GPIO_REG_ADDR   0x14
#define GPIO_RESERVED_BITS   0x00
#define GPIO_READ_ONLY_REGISTER   false
#define GPIO_RESET_VALUE   0x0F

#define GPIO_B_GPIOD4   0x80
#define GPIO_B_GPIOD3   0x40
#define GPIO_B_GPIOD2   0x20
#define GPIO_B_GPIOD1   0x10
#define GPIO_B_GPIOC4   0x08
#define GPIO_B_GPIOC3   0x04
#define GPIO_B_GPIOC2   0x02
#define GPIO_B_GPIOC1   0x01

// ** PACE **
#define PACE_REG_ADDR  0x15
#define PACE_RESERVED_BITS  0x00
#define PACE_READ_ONLY_REGISTER  false
#define PACE_RESET_VALUE  0x00

#define PACE_B_PACEE1   0x10
#define PACE_B_PACEE0   0x08
#define PACE_B_PACEO1   0x04
#define PACE_B_PACEO0   0x02
#define PACE_B_PDB_PACE  0x01

#define PACE_PACEE_CHAN2  (0x00 | PACE_RESERVED_BITS)
#define PACE_PACEE_CHAN4  (PACE_B_PACEE0 | PACE_RESERVED_BITS)
#define PACE_PACEE_CHAN6  (PACE_B_PACEE1 | PACE_RESERVED_BITS)
#define PACE_PACEE_CHAN8  ((PACE_B_PACEE1 | PACE_B_PACEE0 | PACE_RESERVED_BITS))

#define PACE_PACEO_CHAN1  (0x00 | PACE_RESERVED_BITS)
#define PACE_PACEO_CHAN3  (PACE_B_PACEO0 | PACE_RESERVED_BITS)
#define PACE_PACEO_CHAN5  (PACE_B_PACEO1 | PACE_RESERVED_BITS)
#define PACE_PACEO_CHAN7  ((PACE_B_PACEO0 | PACE_B_PACEO1 | PACE_RESERVED_BITS))

// ** RESP **
#define RESP_REG_ADDR  0x16
// FIXME: the reset value is 0x00 but datasheet says "Always write 1" in bit 5. I check in ADS1294 that the reset value is 0x00. Check it (page 80) when a new version of the datasheet will be available
#define RESP_RESERVED_BITS  0x20
#define RESP_READ_ONLY_REGISTER  false
#define RESP_RESET_VALUE  0x00

#if ADS_HAS_RESPIRATION_MODULE
#define RESP_B_RESP_DEMOD_EN1  0x80
#define RESP_B_RESP_MOD_EN1  0x40
#endif
#define RESP_B_RESP_PH2  0x10
#define RESP_B_RESP_PH1  0x08
#define RESP_B_RESP_PH0  0x04
#define RESP_B_RESP_CTRL1  0x02
#define RESP_B_RESP_CTRL0  0x01

#define RESP_RESP_PH_22_5  (0x00 | RESP_RESERVED_BITS)
#define RESP_RESP_PH_45  (RESP_B_RESP_PH0 | RESP_RESERVED_BITS)
#define RESP_RESP_PH_67_5  (RESP_B_RESP_PH1 | RESP_RESERVED_BITS)
#define RESP_RESP_PH_90  (RESP_B_RESP_PH1 | RESP_B_RESP_PH0 | RESP_RESERVED_BITS)
#define RESP_RESP_PH_112_5  (RESP_B_RESP_PH2 | RESP_RESERVED_BITS)
#define RESP_RESP_PH_135  (RESP_B_RESP_PH2 | RESP_B_RESP_PH0 | RESP_RESERVED_BITS)
#define RESP_RESP_PH_157_5  (RESP_B_RESP_PH2 | RESP_B_RESP_PH1 | RESP_RESERVED_BITS)

#define RESP_RESP_NONE  (0x00 | RESP_RESERVED_BITS)
#define RESP_RESP_EXT  (RESP_B_RESP_CTRL0 | RESP_RESERVED_BITS)

#if ADS_HAS_RESPIRATION_MODULE
#define RESP_RESP_INT_SIG_INT  (RESP_B_RESP_CTRL1 | RESP_RESERVED_BITS)
#define RESP_RESP_INT_SIG_EXT  ((RESP_B_RESP_CTRL1 | RESP_B_RESP_CTRL0 | RESP_RESERVED_BITS))
#endif

// ** CONFIG4 **
#define CONFIG4_REG_ADDR  0x17
#define CONFIG4_RESERVED_BITS  0x00
#define CONFIG4_READ_ONLY_REGISTER  false
#define CONFIG4_RESET_VALUE  0x00

#define CONFIG4_B_RESP_FREQ2  0x80
#define CONFIG4_B_RESP_FREQ1  0x40
#define CONFIG4_B_RESP_FREQ0  0x20
#define CONFIG4_SINGLE_SHOT  0x08
#define CONFIG4_WCT_TO_RLD  0x04
#define CONFIG4_PD_LOFF_COMP  0x02

#define CONFIG4_RESP_FREQ_64k_Hz  (0x00 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_32k_Hz  (CONFIG4_B_RESP_FREQ0 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_16k_Hz  (CONFIG4_B_RESP_FREQ1 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_8k_Hz  (CONFIG4_B_RESP_FREQ1 | CONFIG4_B_RESP_FREQ0 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_4k_Hz  (CONFIG4_B_RESP_FREQ2 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_2k_Hz  (CONFIG4_B_RESP_FREQ2 | CONFIG4_B_RESP_FREQ0 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_1k_Hz  (CONFIG4_B_RESP_FREQ2 | CONFIG4_B_RESP_FREQ1 | CONFIG4_RESERVED_BITS)
#define CONFIG4_RESP_FREQ_500_Hz  (CONFIG4_B_RESP_FREQ2 | CONFIG4_B_RESP_FREQ1 | CONFIG4_B_RESP_FREQ0 | CONFIG4_RESERVED_BITS)

// ** WCT1 **
#define WCT1_REG_ADDR  0x18
#define WCT1_RESERVED_BITS  0x00
#define WCT1_READ_ONLY_REGISTER  false
#define WCT1_RESET_VALUE  0x00

#if ADS_N_CHANNELS > 6
#define WCT1_B_aVR_CH7  0x20
#endif
#if ADS_N_CHANNELS > 4
#define WCT1_B_aVF_CH6  0x80
#define WCT1_B_aVL_CH5  0x40
#endif
#define WCT1_B_avR_CH4  0x10
#define WCT1_B_PD_WCTA  0x08
#define WCT1_B_WCTA2   0x04
#define WCT1_B_WCTA1   0x02
#define WCT1_B_WCTA0   0x01

#define WCT1_WCTA_CH1P  (0x00 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH1N  (WCT1_B_WCTA0 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH2P  (WCT1_B_WCTA1 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH2N  (WCT1_B_WCTA1 | WCT1_B_WCTA0 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH3P  (WCT1_B_WCTA2 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH3N  (WCT1_B_WCTA2 | WCT1_B_WCTA0 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH4P  (WCT1_B_WCTA2 | WCT1_B_WCTA1 | WCT1_RESERVED_BITS)
#define WCT1_WCTA_CH4N  (WCT1_B_WCTA2 | WCT1_B_WCTA1 | WCT1_B_WCTA0 | WCT1_RESERVED_BITS)

// ** WCT2 **
#define WCT2_REG_ADDR  (0X19)
#define WCT2_RESERVED_BITS  (0x00)
#define WCT2_READ_ONLY_REGISTER  (false)
#define WCT2_RESET_VALUE  (0x00)

#define WCT2_B_PD_WCTC   0x80
#define WCT2_B_PD_WCTB   0x40
#define WCT2_B_WCTB2   0x20
#define WCT2_B_WCTB1   0x10
#define WCT2_B_WCTB0   0x08
#define WCT2_B_WCTC2   0x04
#define WCT2_B_WCTC1   0x02
#define WCT2_B_WCTC0   0x01

#define WCT2_WCTB_CH1P  (0x00 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH1N  (WCT2_B_WCTB0 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH2P  (WCT2_B_WCTB1 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH2N  (WCT2_B_WCTB1 | WCT2_B_WCTB0 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH3P  (WCT2_B_WCTB2 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH3N  (WCT2_B_WCTB2 | WCT2_B_WCTB0 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH4P  (WCT2_B_WCTB2 | WCT2_B_WCTB1 | WCT2_RESERVED_BITS)
#define WCT2_WCTB_CH4N  (WCT2_B_WCTB2 | WCT2_B_WCTB1 | WCT2_B_WCTB0 | WCT2_RESERVED_BITS)

#define WCT2_WCTC_CH1P  (0x00 | WCT2_RESERVED_BITS)
#define WCT2_WCTC_CH1N  (WCT2_B_WCTC0 | WCT2_RESERVED_BITS)
#define WCT2_WCTC_CH2P  (WCT2_B_WCTC1 | WCT2_RESERVED_BITS)
#define WCT2_WCTC_CH2N  (WCT2_B_WCTC1 | WCT2_B_WCTC0 | WCT2_RESERVED_BITS))
#define WCT2_WCTC_CH3P  (WCT2_B_WCTC2 | WCT2_RESERVED_BITS)
#define WCT2_WCTC_CH3N  (WCT2_B_WCTC2 | WCT2_B_WCTC0 | WCT2_RESERVED_BITS)
#define WCT2_WCTC_CH4P  (WCT2_B_WCTC2 | WCT2_B_WCTC1 | WCT2_RESERVED_BITS)
#define WCT2_WCTC_CH4N  (WCT2_B_WCTC2 | WCT2_B_WCTC1 | WCT2_B_WCTC0 | WCT2_RESERVED_BITS)


#endif
