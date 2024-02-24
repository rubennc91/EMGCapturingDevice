/*****************************************************************************/
/**
* @file main.c
* This file contains a design example using the Spi driver for ADS1298R.
*
******************************************************************************/

/***************************** Include Files *********************************/



#include "xparameters.h"	/* XPAR parameters */
#include "sleep.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "xil_types.h"
#include "platform.h"
#include "ethernet.h"
#include "xspi.h"			/* SPI device driver */
#include "xil_exception.h"
#include "xil_printf.h"
#include "ads1298r_api.h"
#include "spi_api.h"
#include "gic_api.h"
#include "dma_api.h" // Para memoria
#include "ads_spi_mux.h"
#include "ads_spi_ctrl_top.h"
#include "xaxidma.h"

#include "xtime_l.h"

#include "ads1298r.h"


// data
uint8_t hasNewData = 0;
ads_data_t *adsData;

uint32_t dataCount;
unsigned int inicio = 0;
unsigned int fin = 72; // Comprobar
extern unsigned int columini;
//unsigned int columini = 0;
uint8_t contadornueve;
unsigned int filascont;

extern struct netif *echo_netif;
extern int establecida;
extern int DMAlista;

int ContadorLeido = 0;
extern int ContadorDMA;
extern int transmision;
extern int iniciado;

unsigned int Statusword;

//err_t transfer_data(unsigned int data1);
err_t transfer_data1(float data2, float data3, float data4, float data5);
err_t transfer_data2(float data6, float data7, float data8, float data9);

//unsigned int tiempodato = 0;

void Xil_DCacheDisable(void);


/****************************************************************************
*
* Main function to call the Spi interrupt example.
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
******************************************************************************/
int main(void)
{

	Xil_DCacheDisable(); // Desactiva el uso del cache para la DDR

	// ENVIAR A slv_reg0 DEL MUX (adrss 0x43c0_0000 + ADRESS reg 0) un 01 con S_AXI_WDATA
	// LA DIRECCI�N SE MANDA POR S_AXI_AWADDR
	ADS_SPI_MUX_mWriteReg(XPAR_ADS_SPI_MUX_0_S00_AXI_BASEADDR, ADS_SPI_MUX_S00_AXI_SLV_REG0_OFFSET, 0x00);
	usleep(1);
	ADS_SPI_MUX_mWriteReg(XPAR_ADS_SPI_MUX_0_S00_AXI_BASEADDR, ADS_SPI_MUX_S00_AXI_SLV_REG0_OFFSET, 0x01);
	usleep(1);

	ADS_SPI_CTRL_TOP_mWriteReg(XPAR_ADS_SPI_CTRL_TOP_0_S00_AXI_BASEADDR, ADS_SPI_MUX_S00_AXI_SLV_REG0_OFFSET, 0x00);

	int Status;
	uint8_t adsID;
	uint8_t readDATA;

	xil_printf("[INFO] Init ADS Example\r\n");


	ADS_begin();



	// Initialize DMA (Set bits 0 and 12 of the DMA control register)


	// It is not need. Only for example purposes
	xil_printf("[INFO] Example: reset value for all registers without reset command\r\n");
	ADS_setAllRegisterToReset();

	// Read ADS129x ID: 0xD2 for ADS1298R
	Status = SPIreadREGISTER(REGID_REG_ADDR, &adsID);
	if (Status != 0) {
		xil_printf("[ERROR] SPI Configuration Failed\r\n");
	}
	// Check ADS ID
	xil_printf("[INFO] Data read at ID_REG (Used: 0x%x): 0x%x \r\n", REGID_ID_ADS1298R, adsID);


	xil_printf("[INFO] Set sampling read to 1 kHz and low-power mode\r\n");
	xil_printf("[INFO] Keep in mind that when config1 or resp registers are changed, internal reset is performed. See the datasheet, section Reset\r\n");
	Status = SPIreadREGISTER(CONFIG1_REG_ADDR, &readDATA);
	xil_printf("[INFO] The previous value CONFIG1 register is: 0x%x \r\n", readDATA);

	// By default, ADS12xx is in low-power consumption and with a sample frequency of 250 Hz
	SPIwriteREGISTER(CONFIG1_REG_ADDR, CONFIG1_HIGH_RES_8k_SPS);
	Status = SPIreadREGISTER(CONFIG1_REG_ADDR, &readDATA);
 	xil_printf("[INFO] The new value CONFIG1 register is (Configured 0x%x): 0x%x \r\n", CONFIG1_HIGH_RES_8k_SPS, readDATA);

	// Setup of my circuit. In my case, it hadn't external reference,
	xil_printf("[INFO] Enabling internal reference buffer --> set PD_REFBUF to 1\r\n");
	// If you change individual bits with constants B_xx, you must add with the RESERVED_BITS constant value to be sure that you will
	// write the right bits in the reserved bits in the register.
	// Remember to write all desired configuration in a register  simultaneously. When you write a register, you delete all previous values

	Status = SPIreadREGISTER(CONFIG3_REG_ADDR, &readDATA);
	xil_printf("[INFO] The previous value CONFIG3 register is: 0x%x \r\n", readDATA);
	SPIwriteREGISTER(CONFIG3_REG_ADDR, CONFIG3_B_PD_REFBUF | CONFIG3_RESERVED_BITS);
	Status = SPIreadREGISTER(CONFIG3_REG_ADDR, &readDATA);
	xil_printf("[INFO] The value CONFIG3 register is: 0x%x \r\n", readDATA);

	// Wait for internal reference to wake up. See page 15, section Electrical Characteristics in the datasheet,
	usleep(150000); // 150 ms

	// Select test signal from chip
	// As example, this 2 methods will keep the SPI open for ADS129x chip for faster configuration. The difference It's not noticeable for humans
	// Be careful when you use this option. Read the documentation before using it.
	Status = SPIreadREGISTER(CONFIG2_REG_ADDR, &readDATA);
	xil_printf("[INFO] The previous value CONFIG2 register is: 0x%x \r\n", readDATA);
//	SPIwriteREGISTER(CONFIG2_REG_ADDR, CONFIG2_TEST_SOURCE_INTERNAL);

	// We will use the square signal at 4 Hz
	SPIwriteREGISTER(CONFIG2_REG_ADDR, CONFIG2_TEST_FREQ_4HZ);
	Status = SPIreadREGISTER(CONFIG2_REG_ADDR, &readDATA);
	xil_printf("[INFO] The value CONFIG2 register is: 0x%x \r\n", readDATA);

	xil_printf("[INFO] Starting channels configuration.\r\n");
	xil_printf("[INFO] Channel 1: gain 1 and test signal as input.\r\n");
	ADS_enableChannelAndSetGain(1, CHNSET_GAIN_6X, CHNSET_TEST_SIGNAL);

	xil_printf("[INFO] Channel 2: gain 1 and test signal as input.\r\n");
	ADS_enableChannelAndSetGain(2, CHNSET_GAIN_6X, CHNSET_TEST_SIGNAL); //CHNSET_GAIN_6X

	// xil_printf("[INFO] Channel 3: power-down and its inputs shorted (as Texas Instruments recommends).\r\n");
	xil_printf("[INFO] Channel 3: gain 1 and test signal as input.\r\n");
	ADS_enableChannelAndSetGain(3, CHNSET_GAIN_6X, CHNSET_ELECTRODE_INPUT); //CHNSET_TEST_SIGNAL
	// ADS_disableChannel(3);

	xil_printf("[INFO] Channel 4 to 8 channel: set gain 1 and test signal as input.\r\n");
	for (uint8_t i = 4; i <= ADS_N_CHANNELS; i++)
		ADS_enableChannelAndSetGain(i, CHNSET_GAIN_6X, CHNSET_TEST_SIGNAL); // CHNSET_ELECTRODE_INPUT

	xil_printf("[INFO] Starting channels configuration.\r\n");
	SendSPICommandSTART(0);

	uint8_t registerValue = 0x00;
	SPIreadREGISTER(CHNSET__BASE_REG_ADDR + 1, &registerValue);
	xil_printf("[INFO] Config final canal %0x es: %0x\n\r", 1, registerValue);

	// We need to put ADS in DATA or RDATC mode to receive new data
	// Remember that in RDATAC mode, ADS ignores any SPI command sent if it is not SDATAC command
	xil_printf("[INFO] Set ADS chip in read data (RDATAC) mode.\r\n");
	sendSPICommandRDATAC(0);

	//ENVIAR A slv_reg0 DEL MUX (adrss 0x43c0_0000 + ADRESS reg 0) un 11
	xil_printf("[INFO] SPI Control to MUX.\r\n");
	ADS_SPI_MUX_mWriteReg(XPAR_ADS_SPI_MUX_0_S00_AXI_BASEADDR, ADS_SPI_MUX_S00_AXI_SLV_REG0_OFFSET, 0x03);

	xil_printf("[INFO] Start loop obtaining the new data.\r\n");

	configure_ethernet();
	enable_ethernet();

	DMAConfig();

	while (Xil_In32(OFFSET_MEM_WRITE +4*(columini)) != 0xFFC00000){
		columini++;
		contadornueve++;
		if (contadornueve == 9) {
			contadornueve = 0;
			filascont++;
		}
	}

	columini = columini - (filascont*9);

	inicio = columini;
	fin = fin + inicio;

	iniciado = 1;

	while(1){

		if (TcpFastTmrFlag) {
			tcp_fasttmr();
			TcpFastTmrFlag = 0;
		}
		if (TcpSlowTmrFlag) {
			tcp_slowtmr();
			TcpSlowTmrFlag = 0;
		}
		xemacif_input(echo_netif);

		if(transmision == 1 && hasNewData){

			// xil_printf("[INFO] New data available.\r\n");
			hasNewData = 0;

				for (int i=inicio; i<fin; i++){

//					if (i == (64*(8+columini))+1){
//						ContadorLeido++;
//					}
//
//					if (ContadorLeido < ContadorDMA){
//						xil_printf("Datos pisados %d\n\r", tiempodato);
//					}
//
//					tiempodato++;

					int Canal1 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+1)), 0x20);
					int Canal2 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+2)), 0x20);
					int Canal3 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+3)), 0x20);
					int Canal4 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+4)), 0x20);
					int Canal5 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+5)), 0x20);
					int Canal6 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+6)), 0x20);
					int Canal7 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+7)), 0x20);
					int Canal8 = complementoADosADecimal(Xil_In32(OFFSET_MEM_WRITE +4*(i+8)), 0x20);

					Statusword = Xil_In32(OFFSET_MEM_WRITE +4*(i)); // Canal 0 // .4f
					float PhysicalCanal1 = (2.4*(float)(Canal1)) /(6*((1<<23)-1)); // Canal 1
					float PhysicalCanal2 = (2.4*(float)(Canal2)) /(6*((1<<23)-1)); // Canal 2
					float PhysicalCanal3 = (2.4*(float)(Canal3)) /(6*((1<<23)-1)); // Canal 3
					float PhysicalCanal4 = (2.4*(float)(Canal4)) /(6*((1<<23)-1)); // Canal 4
					float PhysicalCanal5 = (2.4*(float)(Canal5)) /(6*((1<<23)-1)); // Canal 5
					float PhysicalCanal6 = (2.4*(float)(Canal6)) /(6*((1<<23)-1)); // Canal 6
					float PhysicalCanal7 = (2.4*(float)(Canal7)) /(6*((1<<23)-1)); // Canal 7
					float PhysicalCanal8 = (2.4*(float)(Canal8)) /(6*((1<<23)-1)); // Canal 8

					transfer_data(Statusword, PhysicalCanal1, PhysicalCanal1, PhysicalCanal1); // Solo se usa la primera. PD: VITIS ES MIERDA
					transfer_data1(PhysicalCanal1, PhysicalCanal2, PhysicalCanal3, PhysicalCanal4);
					transfer_data2(PhysicalCanal5, PhysicalCanal6, PhysicalCanal7, PhysicalCanal8);


					i = i+8;
				}

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				if (fin>=FRAME_COUNT_MAX*NUM_OF_WORDS){ //576000
					// When the end of the memory assign to the DMA is reached.
					fin = columini; // The reading cursor is reset.

					Statusword = Xil_In32(OFFSET_MEM_WRITE +4*(FRAME_COUNT_MAX*NUM_OF_WORDS)); // Canal 0 // .4f

					transfer_data(Statusword, columini, columini, columini);

				}

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


				inicio = fin;
				fin = fin+72;  // Comprobar

			}
		}


	// You can could the method end() to free GPIO used pins and resources. if you don't need any more de ADS
	// You have to call begin() if you want to use again the ADS
	ADS_end();

	xil_printf("[INFO] Successfully ran ADS Example.\r\n");
	return XST_SUCCESS;
}

