
#include "Seesaw.h"
#include "I2C.h"


int init(void) 
{   
    begin(ADDRESS);
    beginTrans(1);
    write(STATUS_BASE);// STATUS_HW_ID, 0xFF);
    write(STATUS_HW_ID);
    write(0xFF);
    delay(500);
    endTrans();

    endTrans();


    beginTrans(1);
    write(STATUS_BASE);// STATUS_HW_ID, 0x00);
    write(STATUS_HW_ID);
    write(0x00);
    endTrans();
    uint8_t c;
    beginTrans(0);
    read8(&c);
    endTrans();
    if (c != SEESAW_HW_ID_CODE)
    {
        return 0;
    }
    return 1;
}


float tempRead(void)
{
    uint8_t buf[4];
    read(STATUS_BASE, STATUS_TEMP, buf, 4, 1000, 1);
    int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    return (1.0 / (1UL << 16)) * ret;
}

uint16_t moistureRead(void)
{
    uint8_t buf[2];
    uint16_t ret = 65535;

    int worked = read(TOUCH_BASE, TOUCH_CHANNEL_OFFSET,
                           buf, 2, 1000, 3);

    if (worked) {
        ret = ((uint16_t)buf[0] << 8) | buf[1];
    }
    return ret;
}