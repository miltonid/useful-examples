#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define START_BYTE0 0xFA
#define START_BYTE1 0xFC

//Main Index of Header protocol
#define SIZE_IDX 2
#define TAG_IDX 4
#define PAYLOAD_IDX 5

//States for states machine receive data frame
#define START0_ST 0
#define START1_ST 1
#define SIZE_ST 2
#define TAG_ST 3
#define PAYLOAD_ST 4

uint8_t packetReceived = false; // whether the rcv packet is complete
uint8_t buffer[200];

int32_t getValueFromArray(uint8_t *b, int size) {
    int8_t shift = 0;
    int32_t val = 0;
    for (int i = 0; i < size; i++) {
        val |= ( (0x000000FF << shift) & (b[i] << shift) );
        shift += 8;
    }
    return val;
}

void processRcvBytes(uint8_t byte) {
  static uint8_t state = START0_ST;
  static uint8_t cnt = 0;
  static uint8_t bytesToBeReceived;
  uint16_t len;
  
  switch(state) {
    
    case START0_ST:
    if(byte == START_BYTE0) {
      state = START1_ST;
      buffer[0] = byte;
    }
    break;
    
    case START1_ST:
    if(byte == START_BYTE1) {
      state = SIZE_ST;
      bytesToBeReceived = 3;
      cnt = 2; //set offset 2 bytes header size
      buffer[1] = byte;
    }else {
      state = START0_ST;
    }
    break;
    
    case SIZE_ST:
    buffer[cnt] = byte;
    cnt++;
    if (cnt > bytesToBeReceived ) {
      state = TAG_ST;
    }
    break;
    
    case TAG_ST:
     buffer[cnt] = byte;
     len = (uint16_t) getValueFromArray(&buffer[SIZE_ST], sizeof(uint16_t));
     cnt++;
     bytesToBeReceived = len + cnt;
     state = PAYLOAD_ST;
    break;
    
    case PAYLOAD_ST:
    buffer[cnt] = byte;
    cnt++;
    if (cnt > bytesToBeReceived ) {
      packetReceived = true;
      state = START0_ST;
    }
    break;
  }
}

void serialPortInterrupt(uint8_t *bufferSerial) {

    for (int i = 0; i < 50; i++) {
        processRcvBytes(bufferSerial[i]);
    }
}

uint8_t *getRcvPacket() {
  return &buffer[0];
}

void parser(uint8_t *buff) {
  uint8_t tag =  buff[TAG_IDX];

  switch(tag) {

    case 1:
    //paylodad uint32_t
    printf("\nparser Tag %d  Payload uint32_t Received %d \n", buff[TAG_IDX], 
            getValueFromArray(&buff[PAYLOAD_IDX], sizeof(int32_t)));
    break;

    case 2:
    //Payload uint16_t
    printf("\nparser Tag %d  Payload uint16_t Received %d \n", buff[TAG_IDX], 
        getValueFromArray(&buff[PAYLOAD_IDX], sizeof(int16_t)));
    break;

  }
}

int main()
{

  uint8_t msg1[9];
  msg1[0] = START_BYTE0;
  msg1[1] = START_BYTE1;
  msg1[2] = 0x04;
  msg1[3] = 0x00;
  msg1[4] = 0x01;
  msg1[5] = 0xdc;
  msg1[6] = 0xff;
  msg1[7] = 0xff;
  msg1[8] = 0xff;

  uint8_t msg2[7];
  msg2[0] = START_BYTE0;
  msg2[1] = START_BYTE1;
  msg2[2] = 0x02;
  msg2[3] = 0x00;
  msg2[4] = 0x02;
  msg2[5] = 0x0a;
  msg2[6] = 0x00;

  printf("\n\n");
  //emulated serial port interrupt for msg 1
  serialPortInterrupt(msg1);

  //this block of code must run in the infinite loop 
  //of a microcontroller
  if(packetReceived) {
    printf("Received Msg 1: ");
    for(int i = 0; i < 9; i++) {
        printf("%02x ", buffer[i]);
    }
    parser(getRcvPacket());
    packetReceived = false;
  }

  //emulated serial port interrupt for msg 2
  serialPortInterrupt(msg2);

  //this block of code must run in the infinite loop 
  //of a microcontroller
  if(packetReceived) {
    printf("\n\nReceived Msg 2: ");
    for(int i = 0; i < 7; i++) {
        printf("%02x ", buffer[i]);
    }
    parser(getRcvPacket());
    packetReceived = false;
  }


    return 0;
}

