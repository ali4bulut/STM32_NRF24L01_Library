/*
 * nrf24.c
 *
 *  Created on: Nov 12, 2021
 *      Author: alibl
 */
#include "nrf24.h"

void NRF24_Read_Reg(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t *data);
void NRF24_Read_Reg_Multiple(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t *data, uint8_t size);
void NRF24_Write_Reg(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t data);
void NRF24_Write_Reg_Multiple(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t *data, uint8_t size);
void NRF24_Send_Cmd(NRF24_Handle *nrf24, uint8_t cmd);
void NRF24_Pipe_Config(NRF24_Handle *nrf24, uint8_t Pipe_Rx_ADDR, uint8_t Pipe_Rx_Pw, uint8_t Pipe_Number);

void NRF24_Read_Reg(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t *data) {

	NRF24_CS_LOW(nrf24);

	uint8_t temp = reg_Addr & 0x1F;	//R_REGISTER command
	HAL_SPI_Transmit(nrf24->spi, &temp, 1, 100);
	HAL_SPI_Receive(nrf24->spi, data, 1, 100);

	NRF24_CS_HIGH(nrf24);
}

void NRF24_Read_Reg_Multiple(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t *data, uint8_t size) {

	NRF24_CS_LOW(nrf24);

	uint8_t temp = reg_Addr & 0x1F;	//R_REGISTER command
	HAL_SPI_Transmit(nrf24->spi, &temp, 1, 100);
	HAL_SPI_Receive(nrf24->spi, data, size, 100);

	NRF24_CS_HIGH(nrf24);
}

void NRF24_Write_Reg(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t data) {

	uint8_t buf[2] = { 0 };
	uint8_t temp = reg_Addr | (1 << 5); //W_REGISTER command
	buf[0] = temp;
	buf[1] = data;

	NRF24_CS_LOW(nrf24);

	HAL_SPI_Transmit(nrf24->spi, buf, 2, 100);

	NRF24_CS_HIGH(nrf24);
}

void NRF24_Write_Reg_Multiple(NRF24_Handle *nrf24, uint8_t reg_Addr, uint8_t *data, uint8_t size) {

	uint8_t temp = reg_Addr | (1 << 5);	//W_REGISTER command

	NRF24_CS_LOW(nrf24);

	HAL_SPI_Transmit(nrf24->spi, &temp, 1, 100);
	HAL_SPI_Transmit(nrf24->spi, data, size, 100);

	NRF24_CS_HIGH(nrf24);
}

void NRF24_Send_Cmd(NRF24_Handle *nrf24, uint8_t cmd) {
	HAL_SPI_Transmit(nrf24->spi, &cmd, 1, 100);
}

void NRF24_Init(NRF24_Handle *nrf24) {

	NRF24_CS_LOW(nrf24);
	NRF24_Send_Cmd(nrf24, NOP);
	NRF24_CS_HIGH(nrf24);

	NRF24_CE_LOW(nrf24);

	//Reset All Registers
	NRF24_Write_Reg(nrf24, REG_CONFIG, 0x08);
	NRF24_Write_Reg(nrf24, REG_EN_AA, 0x3F);
	NRF24_Write_Reg(nrf24, REG_EN_RXADDR, 0x03);
	NRF24_Write_Reg(nrf24, REG_SETUP_AW, 0x03);
	NRF24_Write_Reg(nrf24, REG_SETUP_RETR, 0x03);
	NRF24_Write_Reg(nrf24, REG_RF_CH, 0x02);
	NRF24_Write_Reg(nrf24, REG_RF_SETUP, 0x0E);
	NRF24_Write_Reg(nrf24, REG_STATUS, 0x7E);
	NRF24_Write_Reg(nrf24, REG_OBSERVE_TX, 0x00);
	NRF24_Write_Reg(nrf24, REG_RPD, 0x00);
	uint8_t temp[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
	NRF24_Write_Reg_Multiple(nrf24, REG_RX_ADDR_P0, temp, 5);
	uint8_t temp2[5] = { 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 };
	NRF24_Write_Reg_Multiple(nrf24, REG_RX_ADDR_P1, temp2, 5);
	NRF24_Write_Reg(nrf24, REG_RX_ADDR_P2, 0xC3);
	NRF24_Write_Reg(nrf24, REG_RX_ADDR_P3, 0xC4);
	NRF24_Write_Reg(nrf24, REG_RX_ADDR_P4, 0xC5);
	NRF24_Write_Reg(nrf24, REG_RX_ADDR_P5, 0xC6);
	uint8_t temp3[5] = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
	NRF24_Write_Reg_Multiple(nrf24, REG_TX_ADDR, temp3, 5);
	NRF24_Write_Reg(nrf24, REG_RX_PW_P0, 0x00);
	NRF24_Write_Reg(nrf24, REG_RX_PW_P1, 0x00);
	NRF24_Write_Reg(nrf24, REG_RX_PW_P2, 0x00);
	NRF24_Write_Reg(nrf24, REG_RX_PW_P3, 0x00);
	NRF24_Write_Reg(nrf24, REG_RX_PW_P4, 0x00);
	NRF24_Write_Reg(nrf24, REG_RX_PW_P5, 0x00);
	NRF24_Write_Reg(nrf24, REG_FIFO_STATUS, 0x11);
	NRF24_Write_Reg(nrf24, REG_DYNPD, 0x00);
	NRF24_Write_Reg(nrf24, REG_FEATURE, 0x00);

	NRF24_Write_Reg(nrf24, REG_EN_AA, 0x00);
//	NRF24_Write_Reg(nrf24, REG_EN_RXADDR, 0x00);
//	NRF24_Write_Reg(nrf24, REG_SETUP_RETR, 0x00);

	NRF24_CE_HIGH(nrf24);

	NRF24_CS_LOW(nrf24);
	NRF24_Send_Cmd(nrf24, FLUSH_TX);
	NRF24_Send_Cmd(nrf24, FLUSH_RX);
	NRF24_CS_HIGH(nrf24);

//	//Simple Tx
//	nrf24->CRC_Mode = CRC_Enable;
//	nrf24->CRC_Byte = CRC_Byte_2;
//	nrf24->Address_Width = ADDR_WIFTH_Byte_5;
//	nrf24->Auto_Ack = AUTO_ACK_DISABLE;
//	nrf24->Auto_Retr_Delay = RETR_DELAY_250;
//	nrf24->Channel = 76;
//	nrf24->Speed = SPEED_2Mbps;
//	nrf24->Output_Power = OUTPUT_PWR_0Db;
//	nrf24->Auto_Retr_Mode = RETR_DISABlE;
//	nrf24->Dynamic_Payload_Mode = DYNAMIC_PL_DISABLE;
//	nrf24->Payload_With_Ack_Mode = PL_W_ACK_DISABLE;

	//Simple Rx
	nrf24->CRC_Mode = CRC_Enable;
	nrf24->CRC_Byte = CRC_Byte_2;
	nrf24->Address_Width = ADDR_WIFTH_Byte_5;
	nrf24->Auto_Ack = AUTO_ACK_ENABLE;
	nrf24->Auto_Retr_Delay = RETR_DELAY_250;
	nrf24->Channel = 76;
	nrf24->Speed = SPEED_2Mbps;
	nrf24->Output_Power = OUTPUT_PWR_0Db;
	nrf24->Auto_Retr_Mode = RETR_DISABlE;
	nrf24->Dynamic_Payload_Mode = DYNAMIC_PL_ENABLE;
	nrf24->Payload_With_Ack_Mode = PL_W_ACK_DISABLE;

	HAL_Delay(1);

}

void NRF24_Config(NRF24_Handle *nrf24) {

	NRF24_CE_LOW(nrf24);

	uint8_t temp = 0;

	//CRC config
	if (nrf24->CRC_Mode == CRC_Enable) {
		NRF24_Read_Reg(nrf24, REG_CONFIG, &temp);
		temp |= (uint8_t) ((1 << 3) | (nrf24->CRC_Byte << 2));
		NRF24_Write_Reg(nrf24, REG_CONFIG, temp);
	}
	else {
		NRF24_Read_Reg(nrf24, REG_CONFIG, &temp);
		temp &= (uint8_t) ~(0x0C);
		NRF24_Write_Reg(nrf24, REG_CONFIG, temp);
	}

	if (nrf24->Auto_Ack == AUTO_ACK_ENABLE) {
		temp = 0x3F;
		NRF24_Write_Reg(nrf24, REG_EN_AA, temp);
	}

	if (nrf24->Dynamic_Payload_Mode == DYNAMIC_PL_ENABLE) {	//R_RX_PL_WID den rx dataasının boyunun öğren
		//Enable dynamic payload
		temp = 0;
		NRF24_Read_Reg(nrf24, REG_FEATURE, &temp);
		temp |= (1 << 2);
		NRF24_Write_Reg(nrf24, REG_FEATURE, temp);
		//Enable dynamic payload for pipe line
		temp = 0x3F;
		NRF24_Write_Reg(nrf24, REG_DYNPD, temp);
	}

	//Addresses Width config
	temp = nrf24->Address_Width & 0x03;
	NRF24_Write_Reg(nrf24, REG_SETUP_AW, temp);

	//Auto Re Transfer Mode Config
	temp = 0;
	temp = (uint8_t) ((nrf24->Auto_Retr_Delay << 4) | (nrf24->Auto_Retr_Mode));
	NRF24_Write_Reg(nrf24, REG_SETUP_RETR, temp);

	//Data speed rate and Output Power Config
	temp = 0;
	NRF24_Read_Reg(nrf24, REG_RF_SETUP, &temp);
	temp &= ~(0x0F);
	temp |= (uint8_t) ((nrf24->Speed << 3) | (nrf24->Output_Power << 1));
	NRF24_Write_Reg(nrf24, REG_RF_SETUP, temp);

	//Channel Select
	NRF24_Write_Reg(nrf24, REG_RF_CH, nrf24->Channel);

	//TX Mode
	if (nrf24->Mode == MODE_PTX) {
		NRF24_Write_Reg_Multiple(nrf24, REG_RX_ADDR_P0, nrf24->Tx_Address, 5);
		NRF24_Write_Reg_Multiple(nrf24, REG_TX_ADDR, nrf24->Tx_Address, 5);

		temp = 0;
		NRF24_Read_Reg(nrf24, REG_CONFIG, &temp);
		temp &= ~(0x01);
		temp |= (0x02);
		NRF24_Write_Reg(nrf24, REG_CONFIG, temp);
	}
	else if (nrf24->Mode == MODE_PRX) {
		switch (nrf24->Rx_Pipe_Number) {
			case 0:
				NRF24_Pipe_Config(nrf24, REG_RX_ADDR_P0, REG_RX_PW_P0, 0);
				break;
			case 1:
				NRF24_Pipe_Config(nrf24, REG_RX_ADDR_P1, REG_RX_PW_P1, 1);
				break;
			case 2:
				NRF24_Pipe_Config(nrf24, REG_RX_ADDR_P2, REG_RX_PW_P2, 2);
				break;
			case 3:
				NRF24_Pipe_Config(nrf24, REG_RX_ADDR_P3, REG_RX_PW_P3, 3);
				break;
			case 4:
				NRF24_Pipe_Config(nrf24, REG_RX_ADDR_P4, REG_RX_PW_P4, 4);
				break;
			case 5:
				NRF24_Pipe_Config(nrf24, REG_RX_ADDR_P5, REG_RX_PW_P5, 5);
				break;
			default:
				break;
		}

		temp = 0;
		NRF24_Read_Reg(nrf24, REG_CONFIG, &temp);
		temp |= (0x03);
		NRF24_Write_Reg(nrf24, REG_CONFIG, temp);

	}

	NRF24_CS_LOW(nrf24);
	NRF24_Send_Cmd(nrf24, FLUSH_TX);
	NRF24_Send_Cmd(nrf24, FLUSH_RX);
	NRF24_CS_HIGH(nrf24);
	NRF24_CE_HIGH(nrf24);
	HAL_Delay(1);
}

void NRF24_Pipe_Config(NRF24_Handle *nrf24, uint8_t Pipe_Rx_ADDR, uint8_t Pipe_Rx_Pw, uint8_t Pipe_Number) {
	uint8_t temp = 0;
	//Write Rx address to Pipe line register
	if (Pipe_Number <= 1) {

		NRF24_Write_Reg_Multiple(nrf24, Pipe_Rx_ADDR, nrf24->Rx_Address, 5);
	}
	else {
		//TODO for using pipe 2 3 4 5 pipe 1 need to be addressed
		uint8_t addr = nrf24->Rx_Address[0];
		nrf24->Rx_Address[0] += 1; //if 0xFF error
		NRF24_Write_Reg_Multiple(nrf24, REG_RX_ADDR_P1, nrf24->Rx_Address, 5);
		NRF24_Write_Reg(nrf24, Pipe_Rx_ADDR, addr);
	}

	NRF24_Write_Reg_Multiple(nrf24, REG_TX_ADDR, nrf24->Rx_Address, 5);

	//Pipe line data size config
	if (nrf24->Dynamic_Payload_Mode == DYNAMIC_PL_ENABLE) {	//R_RX_PL_WID den rx dataasının boyunun öğren
		//Enable dynamic payload
		temp = 0;
		NRF24_Read_Reg(nrf24, REG_FEATURE, &temp);
		temp |= (1 << 2);
		NRF24_Write_Reg(nrf24, REG_FEATURE, temp);
		//Enable dynamic payload for pipe line
		temp = 0;
		NRF24_Read_Reg(nrf24, REG_DYNPD, &temp);
		temp |= (1 << Pipe_Number);
		NRF24_Write_Reg(nrf24, REG_DYNPD, temp);
	}
	else {
		//Fix size data width
		NRF24_Write_Reg(nrf24, Pipe_Rx_Pw, 32);
	}

	//Enabled Pipe Numbered Register
	temp = 0;
	NRF24_Read_Reg(nrf24, REG_EN_RXADDR, &temp);
	temp |= ((1 << Pipe_Number) | 0x03);
	NRF24_Write_Reg(nrf24, REG_EN_RXADDR, temp);

	//Enable Auta Ack data for pipe line
	if (nrf24->Auto_Ack == AUTO_ACK_ENABLE) {
		temp = 0;
		NRF24_Read_Reg(nrf24, REG_EN_AA, &temp);
		temp |= ((1 << Pipe_Number) | 0x03);
		NRF24_Write_Reg(nrf24, REG_EN_AA, temp);
	}
	else {
		temp = 0x00;
		NRF24_Write_Reg(nrf24, REG_EN_AA, temp);
	}
}

void NRF24_Transmit(NRF24_Handle *nrf24, uint8_t *data, uint8_t size) {
	uint8_t state = 0;
	uint8_t temp = NOP;

	//Transmit data
	NRF24_CS_LOW(nrf24);
	NRF24_Send_Cmd(nrf24, W_TX_PAYLOAD);
	HAL_SPI_Transmit(nrf24->spi, data, size, 100);
	NRF24_CS_HIGH(nrf24);
	//Check the transmit is successful
	while (!(state & 0x20)) {
		NRF24_CS_LOW(nrf24);
		HAL_SPI_TransmitReceive(nrf24->spi, &temp, &state, 1, 100);
		NRF24_CS_HIGH(nrf24);
		//if MAX_RT bit is set have to cleared it to contiuine the com
		if (state & 0x10) {
			NRF24_Write_Reg(nrf24, REG_STATUS, 0x10);
		}
	}
	//Clear tx_DS bit and flush tx buffer
	NRF24_Write_Reg(nrf24, REG_STATUS, 0x30);
	NRF24_CS_LOW(nrf24);
	NRF24_Send_Cmd(nrf24, FLUSH_TX);
	NRF24_CS_HIGH(nrf24);
}

uint8_t isAvailable(NRF24_Handle *nrf24, uint8_t pipe_Number) {
	uint8_t temp = NOP;
	uint8_t state = 0;

	NRF24_CS_LOW(nrf24);
	HAL_SPI_TransmitReceive(nrf24->spi, &temp, &state, 1, 100);
	NRF24_CS_HIGH(nrf24);

	//RX_DR and RX_P_NO bit control and clear RX_DR flag && (state & 0x40) && (state & (pipe_Number << 1))
	if (((state) & (1 << 6)) && (((state & 0x0E) >> 1) == pipe_Number)) {
		NRF24_Write_Reg(nrf24, REG_STATUS, 0x40);
		return 1;
	}

	return 0;
}

void NRF24_Receive(NRF24_Handle *nrf24, uint8_t *data) {
	uint8_t state = 0;

	if (nrf24->Dynamic_Payload_Mode == DYNAMIC_PL_ENABLE) {
		uint8_t pl_Size = 0;
		NRF24_CS_LOW(nrf24);
		NRF24_Send_Cmd(nrf24, R_RX_PL_WID);
		HAL_SPI_Receive(nrf24->spi, &pl_Size, 1, 100);
		NRF24_CS_HIGH(nrf24);

		NRF24_CS_LOW(nrf24);
		NRF24_Send_Cmd(nrf24, R_RX_PAYLOAD);
		HAL_SPI_Receive(nrf24->spi, data, pl_Size, 100);
		NRF24_CS_HIGH(nrf24);
	}
	else {
		NRF24_CS_LOW(nrf24);
		NRF24_Send_Cmd(nrf24, R_RX_PAYLOAD);
		HAL_SPI_Receive(nrf24->spi, data, 32, 100);
		NRF24_CS_HIGH(nrf24);
	}

	while (!(state & 0x01)) {
		NRF24_Read_Reg(nrf24, REG_FIFO_STATUS, &state);
	}
	NRF24_CS_LOW(nrf24);
	NRF24_Send_Cmd(nrf24, FLUSH_RX);
	NRF24_CS_HIGH(nrf24);
}
