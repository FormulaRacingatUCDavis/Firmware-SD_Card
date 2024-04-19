#ifndef __CAN_SD_H__
#define __CAN_SD_H__

#include "fatfs.h"
extern CAN_HandleTypeDef hcan1;

extern FATFS SDFatFS;
extern FIL SDFile;
char buffer[8192];
FRESULT res; /* FatFs function common result code */
DWORD fileSize;
uint32_t byteswritten, bytesread; /* File write/read counts */


CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8];
uint8_t RxData[8];

uint32_t TxMailbox;


void bufclear(void){
	for(int i = 0; i<8192; i++){
		buffer[i] = '\0';
	}
}

void Mount_SD_Card(void){
  uint32_t startTime = HAL_GetTick();


  	 res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
	           //Open file for writing (Create)
	 res = f_open(&SDFile, "CANData.txt",  FA_OPEN_APPEND | FA_OPEN_ALWAYS | FA_WRITE);
	 strcpy (buffer, "This file is for saving CAN Data\n");
	 res = f_write(&SDFile, buffer, strlen((char *)buffer), (void *)&byteswritten);
	 res = f_close(&SDFile);
	 bufclear();
}

void CAN(void){
  res = HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, &RxData);
//HAL_CAN_GetTxMessage(&hcan1, CAN_TX_FIFO0, &RxHeader, &RxData);

int TimeRightAfter_Get_From_Slave = HAL_GetTick();

  res =f_open(&SDFile, "CANData.txt",  FA_OPEN_APPEND | FA_WRITE);
  sprintf(buffer,"%X, %d, %d, %d, %d, %d, %d, %d, %d, %d \n ", RxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7], TimeRightAfter_Get_From_Slave );
  f_write(&SDFile, buffer, strlen((char *)buffer), (void *)&byteswritten);
  //sprintf(buffer,"%X, %d, %d, %d, %d, %d, %d, %d, %d, %d \n ", TxHeader.StdId, TxData[0], TxData[1], TxData[2], TxData[3], TxData[4], TxData[5], TxData[6], TxData[7], TimeRightAfter_Get_From_Slave );
  //f_write(&SDFile, buffer, strlen((char *)buffer), (void *)&byteswritten);

  f_close(&SDFile);
  bufclear();
}

#endif /* __CAN_SD_H__ */
