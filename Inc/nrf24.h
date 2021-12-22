/*
 * nrf24.h
 *
 *  Created on: Nov 12, 2021
 *      Author: alibl
 */

#ifndef INC_NRF24_H_
#define INC_NRF24_H_

#include "stm32f4xx_hal.h"

/* Memory Map */
#define REG_CONFIG      	0x00
#define REG_EN_AA       	0x01
#define REG_EN_RXADDR   	0x02
#define REG_SETUP_AW    	0x03
#define REG_SETUP_RETR  	0x04
#define REG_RF_CH       	0x05
#define REG_RF_SETUP    	0x06
#define REG_STATUS      	0x07
#define REG_OBSERVE_TX  	0x08
#define REG_RPD          	0x09
#define REG_RX_ADDR_P0  	0x0A
#define REG_RX_ADDR_P1  	0x0B
#define REG_RX_ADDR_P2  	0x0C
#define REG_RX_ADDR_P3  	0x0D
#define REG_RX_ADDR_P4  	0x0E
#define REG_RX_ADDR_P5  	0x0F
#define REG_TX_ADDR     	0x10
#define REG_RX_PW_P0    	0x11
#define REG_RX_PW_P1    	0x12
#define REG_RX_PW_P2    	0x13
#define REG_RX_PW_P3    	0x14
#define REG_RX_PW_P4    	0x15
#define REG_RX_PW_P5    	0x16
#define REG_FIFO_STATUS 	0x17
#define REG_DYNPD	    	0x1C
#define REG_FEATURE	    	0x1D

//SPI Commands
#define R_RX_PAYLOAD		0x61
#define W_TX_PAYLOAD		0xA0
#define FLUSH_TX			0xE1
#define FLUSH_RX			0xE2
#define REUSE_TX_PL			0xE3
#define R_RX_PL_WID			0x60
#define W_ACK_PAYLOAD_MASK	0x68 //0:2 bits for pipe line number
#define W_TX_PAYLOAD_NOACK	0xB0
#define NOP					0xFF

enum AUTO_ACK {
	AUTO_ACK_DISABLE, AUTO_ACK_ENABLE
};
enum CRC_STATE {
	CRC_Enable, CRC_Disable

};

enum CRC_BYTE {
	CRC_Byte_1, CRC_Byte_2

};

enum POWER_UP_DOWN {
	PWR_Up, PWR_Down

};
enum MODE {
	MODE_PTX, MODE_PRX

};
enum ADDR_WIDTH {
	ADDR_WIDTH_ILLEGAL, ADDR_WIFTH_Byte_3, ADDR_WIFTH_Byte_4, ADDR_WIFTH_Byte_5
};

enum AUTO_RETR_DELAY {
	RETR_DELAY_250,
	RETR_DELAY_500,
	RETR_DELAY_750,
	RETR_DELAY_1000,
	RETR_DELAY_1250,
	RETR_DELAY_1500,
	RETR_DELAY_1750,
	RETR_DELAY_2000,
	RETR_DELAY_2250,
	RETR_DELAY_2500,
	RETR_DELAY_2750,
	RETR_DELAY_3000,
	RETR_DELAY_3250,
	RETR_DELAY_3500,
	RETR_DELAY_3750,
	RETR_DELAY_4000
};
enum AUTO_RETR_COUNT {
	RETR_DISABlE,
	RETR_CNT_1,
	RETR_CNT_2,
	RETR_CNT_3,
	RETR_CNT_4,
	RETR_CNT_5,
	RETR_CNT_6,
	RETR_CNT_7,
	RETR_CNT_8,
	RETR_CNT_9,
	RETR_CNT_10,
	RETR_CNT_11,
	RETR_CNT_12,
	RETR_CNT_13,
	RETR_CNT_14,
	RETR_CNT_15
};
enum SPEED_RATE {
	SPEED_1Mbps, SPEED_2Mbps
};
enum OUTPUT_PWR {
	OUTPUT_PWR_M18Db, OUTPUT_PWR_M12Db, OUTPUT_PWR_M6Db, OUTPUT_PWR_0Db,
};
enum RX_PIPE_LINES {
	RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
};
enum DYNAMIC_PAYLOAD {
	DYNAMIC_PL_DISABLE, DYNAMIC_PL_ENABLE
};
enum PAYLOAD_W_ACK {
	PL_W_ACK_DISABLE, PL_W_ACK_ENABLE
};

#define	NRF24_CS_LOW(__HANDLE__) 	do{ (__HANDLE__)->CS_Port->BSRR = (uint32_t)(__HANDLE__)->CS_Pin << 16U;}while(0)
#define	NRF24_CS_HIGH(__HANDLE__) 	do{ (__HANDLE__)->CS_Port->BSRR = (__HANDLE__)->CS_Pin; }while(0)

#define	NRF24_CE_LOW(__HANDLE__) 	do{ (__HANDLE__)->CE_Port->BSRR = (uint32_t)(__HANDLE__)->CE_Pin << 16U; }while(0)
#define	NRF24_CE_HIGH(__HANDLE__) 	do{ (__HANDLE__)->CE_Port->BSRR = (__HANDLE__)->CE_Pin; }while(0)

typedef struct {

	SPI_HandleTypeDef *spi;
	GPIO_TypeDef *CE_Port;
	uint32_t CE_Pin;
	GPIO_TypeDef *CS_Port;
	uint32_t CS_Pin;

	uint8_t Mode;			//PTX or PRX
	uint8_t CRC_Mode;		//CRC enable disable
	uint8_t CRC_Byte;		//CRC byte 1 or 2
	uint8_t Address_Width;	//Address width 1-5
	uint8_t Auto_Retr_Mode;	//Auto reTransfer Count or disable
	uint8_t Auto_Retr_Delay;	//Auto reTrasnfer delay time
	uint8_t Speed;			//Data speed rate 1mbps 2mbps
	uint8_t Output_Power;	//Output power 0Db full power

	uint8_t Channel;		//COmmunication channel
	uint8_t Rx_Pipe_Number;	//RX pipe number 0-5
	uint8_t *Rx_Address; //Address writen backwards LSB ------ MSB
	uint8_t *Tx_Address; //Address writen backwards LSB ------ MSB

	uint8_t Auto_Ack;		//Rx mode Auto ack
	uint8_t Dynamic_Payload_Mode;	//Rx mode auto payload width
	uint8_t Payload_With_Ack_Mode;	//Rx mode auto ack with payload

} NRF24_Handle;

void NRF24_Init(NRF24_Handle *nrf24);
void NRF24_Config(NRF24_Handle *nrf24);

void NRF24_Transmit(NRF24_Handle *nrf24, uint8_t *data, uint8_t size);

uint8_t isAvailable(NRF24_Handle *nrf24, uint8_t pipe_Number);

void NRF24_Receive(NRF24_Handle *nrf24, uint8_t *data) ;

#endif /* INC_NRF24_H_ */

//Pipe Handle
////Enabled Pipe Numbered Register
//temp = 0;
//NRF24_Read_Reg(nrf24, REG_EN_RXADDR, &temp);
//temp |= (1 << 0);
//NRF24_Write_Reg(nrf24, REG_EN_RXADDR, temp);
//
////Enable Auta Ack data for pipe line
//if (nrf24->Auto_Ack) {
//	temp = 0;
//	NRF24_Read_Reg(nrf24, REG_EN_AA, &temp);
//	temp |= (1 << 0);
//	NRF24_Write_Reg(nrf24, REG_EN_AA, temp);
//}
//
////RX address Write the Pipe line
//NRF24_Write_Reg_Multiple(nrf24, REG_RX_ADDR_P0, nrf24->Rx_Address, 5);
//
////Pipe line data size config
//if (nrf24->Dynamic_Payload_Mode) {	//R_RX_PL_WID den rx dataasının boyunun öğren
//	//Enable dynamic payload
//	temp = 0;
//	NRF24_Read_Reg(nrf24, REG_FEATURE, &temp);
//	temp |= (1 << 2);
//	NRF24_Write_Reg(nrf24, REG_FEATURE, temp);
//	//Enable dynamic payload for pipe line
//	temp = 0;
//	NRF24_Read_Reg(nrf24, REG_DYNPD, &temp);
//	temp |= (1 << 0);
//	NRF24_Write_Reg(nrf24, REG_DYNPD, temp);
//}
//else {
//	//Fix size data width
//	NRF24_Write_Reg(nrf24, REG_RX_PW_P0, 32);
//}

