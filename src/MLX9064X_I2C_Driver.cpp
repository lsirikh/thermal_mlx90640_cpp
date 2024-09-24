// #include <Wire.h>
// #include <Arduino.h>

// #include "MLX9064X_I2C_Driver.h"

// void MLX9064x_I2CInit() {

// }

// //Read a number of words from startAddress. Store into Data array.
// //Returns 0 if successful, -1 if error
// int MLX9064x_I2CRead(uint8_t _deviceAddress, unsigned int startAddress, unsigned int nWordsRead, uint16_t* data) {

//     //Caller passes number of 'unsigned ints to read', increase this to 'bytes to read'
//     uint16_t bytesRemaining = nWordsRead * 2;

//     //It doesn't look like sequential read works. Do we need to re-issue the address command each time?

//     uint16_t dataSpot = 0; //Start at beginning of array

//     //Setup a series of chunked I2C_BUFFER_LENGTH byte reads
//     while (bytesRemaining > 0) {
//         Wire.beginTransmission(_deviceAddress);
//         Wire.write(startAddress >> 8); //MSB
//         Wire.write(startAddress & 0xFF); //LSB
//         if (Wire.endTransmission(false) != 0) { //Do not release bus
//             //Serial.println("No ack read");
//             return (0); //Sensor did not ACK
//         }

//         uint16_t numberOfBytesToRead = bytesRemaining;
//         if (numberOfBytesToRead > I2C_BUFFER_LENGTH) {
//             numberOfBytesToRead = I2C_BUFFER_LENGTH;
//         }

//         Wire.requestFrom((uint8_t)_deviceAddress, numberOfBytesToRead);
//         if (Wire.available()) {
//             for (uint16_t x = 0 ; x < numberOfBytesToRead / 2; x++) {
//                 //Store data into array
//                 data[dataSpot] = Wire.read() << 8; //MSB
//                 data[dataSpot] |= Wire.read(); //LSB

//                 dataSpot++;
//             }
//         }

//         bytesRemaining -= numberOfBytesToRead;

//         startAddress += numberOfBytesToRead / 2;
//     }

//     return (0); //Success
// }

// //Set I2C Freq, in kHz
// //MLX9064x_I2CFreqSet(1000) sets frequency to 1MHz
// void MLX9064x_I2CFreqSet(int freq) {
//     //i2c.frequency(1000 * freq);
//     Wire.setClock((long)1000 * freq);
// }

// //Write two bytes to a two byte address
// int MLX9064x_I2CWrite(uint8_t _deviceAddress, unsigned int writeAddress, uint16_t data) {
    // Wire.beginTransmission((uint8_t)_deviceAddress);
    // Wire.write(writeAddress >> 8); //MSB
    // Wire.write(writeAddress & 0xFF); //LSB
    // Wire.write(data >> 8); //MSB
    // Wire.write(data & 0xFF); //LSB
    // if (Wire.endTransmission() != 0) {
    //     //Sensor did not ACK
    //     //Serial.println("Error: Sensor did not ack");
    //     return (-1);
    // }

    // uint16_t dataCheck;
    // MLX9064x_I2CRead(_deviceAddress, writeAddress, 1, &dataCheck);
    // if (dataCheck != data) {
    //     //Serial.println("The write request didn't stick");
    //     return -2;
//     }

//     return (0); //Success
// }

#include "MLX9064X_I2C_Driver.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <linux/i2c-dev.h>

//#define I2C_MSG_FMT char
#ifndef I2C_FUNC_I2C
#include <linux/i2c.h>
#define I2C_MSG_FMT __u8
#endif

#include <sys/ioctl.h>

int i2c_fd = 0;
const char *i2c_device = "/dev/i2c-1";

void MLX9064x_I2CInit()
{

}

int MLX9064x_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
    if (!i2c_fd) {
        i2c_fd = open(i2c_device, O_RDWR);
    }

    int result;
    char cmd[2] = {(char)(startAddress >> 8), (char)(startAddress & 0xFF)};
    char buf[1664];
    uint16_t *p = data;
    struct i2c_msg i2c_messages[2];
    struct i2c_rdwr_ioctl_data i2c_messageset[1];

    i2c_messages[0].addr = slaveAddr;
    i2c_messages[0].flags = 0;
    i2c_messages[0].len = 2;
    i2c_messages[0].buf = (I2C_MSG_FMT *)cmd;

    i2c_messages[1].addr = slaveAddr;
    i2c_messages[1].flags = I2C_M_RD | I2C_M_NOSTART;
    i2c_messages[1].len = nMemAddressRead * 2;
    i2c_messages[1].buf = (I2C_MSG_FMT *)buf;

    //result = write(i2c_fd, cmd, 3);
    //result = read(i2c_fd, buf, nMemAddressRead*2);
    i2c_messageset[0].msgs = i2c_messages;
    i2c_messageset[0].nmsgs = 2;

    memset(buf, 0, nMemAddressRead * 2);

    if (ioctl(i2c_fd, I2C_RDWR, &i2c_messageset) < 0) {
        printf("I2C Read Error!\n");
        return -1;
    }

    for (int count = 0; count < nMemAddressRead; count++) {
        int i = count << 1;
        *p++ = ((uint16_t)buf[i] << 8) | buf[i + 1];
    }

    return 0;
}

void MLX9064x_I2CFreqSet(int freq)
{
}

int MLX9064x_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    char cmd[4] = {(char)(writeAddress >> 8), (char)(writeAddress & 0x00FF), (char)(data >> 8), (char)(data & 0x00FF)};
    int result;

    struct i2c_msg i2c_messages[1];
    struct i2c_rdwr_ioctl_data i2c_messageset[1];

    i2c_messages[0].addr = slaveAddr;
    i2c_messages[0].flags = 0;
    i2c_messages[0].len = 4;
    i2c_messages[0].buf = (I2C_MSG_FMT *)cmd;

    i2c_messageset[0].msgs = i2c_messages;
    i2c_messageset[0].nmsgs = 1;

    if (ioctl(i2c_fd, I2C_RDWR, &i2c_messageset) < 0) {
        printf("I2C Write Error!\n");
        return -1;
    }

    return 0;
}

int MLX9064x_I2CGeneralReset(void)
{
	MLX9064x_I2CWrite(0x33,0x06,0x00);
	return 0;
}
