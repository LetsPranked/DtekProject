
#include "I2C.h"


uint8_t i2cAddress;

void begin(uint8_t address)
{
    i2cAddress = address;
    double BRG;

    I2C1CON = 0;            // Turn off I2C1 module
    I2C1CON = 1<<9; // Disable slew rate for 100kHz

    BRG = (1 / (2 * 100000)) - 0.000000104;
    BRG *= (SYS_FREQ / 2) - 2;    

    I2C1BRG = (int) BRG;     // Set baud rate
    
    I2C1STAT = 0;
    I2C1CONSET = 1 << 13; // 
    I2C1CONSET = 1 << 15; // Enable i2c

}

void wait_for_idle(void)
{
    while(I2C1CON & 0x1F); // Acknowledge sequence not in progress
                                // Receive sequence not in progress
                                // Stop condition not in progress
                                // Repeated Start condition not in progress
                                // Start condition not in progress
    while((I2C1STAT>>14) & 0x1); // Bit = 0 ? Master transmit is not in progress
}

// Set writebit 1 for writing to slave
void beginTrans(int writebit) {
    wait_for_idle();
    I2C1CONSET = 0x1;
    while ((I2C1CON & 0x1) == 1);
    uint8_t addressWithRW =(((i2cAddress << 1) | 1) & ~writebit);
    write(addressWithRW);
}

void endTrans(void)
{
    wait_for_idle();
    I2C1CONSET = 0x4;
}

// I2C_ack() sends an ACK condition
void ack(void)
{
    wait_for_idle();
    I2C1CONSET = 0x10; // Set hardware to send ACK bit
                    // Send ACK bit, will be automatically cleared by hardware when sent  
    while((I2C1CON>>4) & 0x1); // Wait until ACKEN bit is cleared, meaning ACK bit has been sent
}

void write(uint8_t data)
{
    wait_for_idle();
    I2C1TRN = data;
    while ((I2C1STAT & 0x1) == 1);
    while ((I2C1STAT >> 15));
}

void read8(uint8_t * value)
{
    I2C1CONSET = 0x8;               // Receive enable
    while (I2C1CON & 0x8);           // Wait until RCEN is cleared (automatic)  
    while (!(I2C1STAT & 0x2));      // Wait until Receive Buffer is Full (RBF flag)  
    *value = I2C1RCV;
    ack();
}

uint8_t min(uint8_t a, uint8_t b) {
    if (a < b) {
        return a;
    }
    return b;
}

int read(uint8_t base, uint8_t offset, uint8_t *buf, uint8_t num, uint16_t ms, int retries) {
    beginTrans(1);
    write(base);
    write(offset);
    endTrans();

    beginTrans(0);
    uint8_t pos;
    while (pos < num) {
        uint8_t read_now = min(32, num - pos);

        int i = 0;
        while (i < retries) {
            delay(ms);
            i++;
        }

        i = 0;
        while (i < read_now) {
            read8(&buf[pos]);
            pos++;
            i++;
        }
    }
    endTrans();
    return 1;
}

