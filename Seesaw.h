
#include <pic32mx.h>
#include <stdint.h>
#include "I2C.h"


// Info about Seesaw
#define ADDRESS 0x36
#define SEESAW_HW_ID_CODE 0x55

// Useful memory bases
#define STATUS_BASE 0x00
#define TOUCH_BASE 0x0F

// Offsets
#define STATUS_HW_ID 0x01
#define STATUS_TEMP 0x04
#define TOUCH_CHANNEL_OFFSET 0x10


int init(void);
float tempRead(void);
uint16_t moistureRead(void);