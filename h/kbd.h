/* kbd.h */
#include <xeroskernel.h>
#include <xeroslib.h>


void openKbd(void);
void closeKbd(void);
int readKbd(unsigned char *buf, int size);
int writeKbd(unsigned char *buf, int size);
int ioctlKbd(unsigned long command, int command2);
void keyboard0Init(void);
void keyboard1Init(void);
void kbdInterruptHandler(void);
unsigned int kbtoa( unsigned char code );