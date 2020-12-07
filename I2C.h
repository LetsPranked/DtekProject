
#include <pic32mx.h>
#include <stdint.h>

#define SYS_FREQ 80000000

void begin(uint8_t);

void beginTrans(int);
void endTrans();
void write(uint8_t);
int read(uint8_t, uint8_t , uint8_t *, uint8_t , uint16_t , int);
