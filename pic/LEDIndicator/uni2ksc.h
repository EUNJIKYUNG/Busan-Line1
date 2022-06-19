#ifndef UNI2KSC
#define UNI2KSC
#include <stdint.h>


int bsearch_unitab(int unicode, int table[][2], int n);
int correct_Sejong2007(FILE *fpout, int unicode);
void put_BOM(FILE *fpout, int out_mode);
void conv_uni2utf8(int unicode, char utf8[]);
int get_nextcode(FILE *fpin, int in_mode);
char set_inputmode(FILE *fpin, char in_mode);
void conv_uni2ksc(uint16_t *uni, uint8_t *ksc);
uint8_t toxdigit(int i);




#endif