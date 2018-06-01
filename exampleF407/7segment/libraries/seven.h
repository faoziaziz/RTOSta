
#ifndef __SINE_H_
#define __SINE_H_





//static uint8_t const SevSgmnt[17]={0xF6,0x14,0xA6,0xBC,0x5C,0xF8,0xFA,0x94,0xFE,0xFC,  0x3A,0xCA,0xEA,0x2A,0x6A,0x32,0x08};
static uint8_t const SevSgmnt[17] = {0x09,0xEB,0x51,0x43,0xA3,0x07,0x05,0x6B,0x01,0x03,  0xC5,0x35,0x15,0xD5,0x95,0xCD,0xF7};
                                                                                        // o   F    E    c    t     u   -



void SegmentSetSearch(void);
void Segment1Set(uint16_t value, uint8_t dot);
void Segment2Set(uint16_t value, uint8_t dot);
void SegmentSetCut(uint16_t value);
void SegmentSetSec(uint16_t value);
void SegmentSetOfs(uint16_t value);







#endif