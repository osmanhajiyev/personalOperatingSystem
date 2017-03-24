#include <xeroskernel.h>
#include <xeroslib.h>
#include <i386.h>
#include <stdarg.h>
#include <kbd.h>
#include <tty.h>

#define KEY_UP   0x80            /* If this bit is on then it is a key   */
                                 /* up event instead of a key down event */
 
/* Control code */
#define LSHIFT  0x2a
#define RSHIFT  0x36
#define LMETA   0x38
 
#define LCTL    0x1d
#define CAPSL   0x3a
 
 
/* scan state flags */
#define INCTL           0x01    /* control key is down          */
#define INSHIFT         0x02    /* shift key is down            */
#define CAPSLOCK        0x04    /* caps lock mode               */
#define INMETA          0x08    /* meta (alt) key is down       */
#define EXTENDED        0x10    /* in extended character mode   */
 
#define EXTESC          0xe0    /* extended character escape    */
#define NOCHAR          256
#define KBD_BUFSIZE     4
#define EOF             53
#define NONECHO         55
#define ECHO            56
#define EOF_SYMBOL      4

static int eof; // eof symbol

static  int     state;

static  int     kbd_echo; /* the state of the keyboard, set by ioctl */
                          /* nonecho on startup */

// buffer for non echo kbd
static  unsigned char  buffer_c[KBD_BUFSIZE];
// pointers to manage buffer
static int bufhead;
static int buftail;

 
/*  Normal table to translate scan code  */
unsigned char   kbcode[] = { 0,
          27,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',
         '0',  '-',  '=', '\b', '\t',  'q',  'w',  'e',  'r',  't',
         'y',  'u',  'i',  'o',  'p',  '[',  ']', '\n',    0,  'a',
         's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';', '\'',
         '`',    0, '\\',  'z',  'x',  'c',  'v',  'b',  'n',  'm',
         ',',  '.',  '/',    0,    0,    0,  ' ' };
 
/* captialized ascii code table to tranlate scan code */
unsigned char   kbshift[] = { 0,
           0,  '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',
         ')',  '_',  '+', '\b', '\t',  'Q',  'W',  'E',  'R',  'T',
         'Y',  'U',  'I',  'O',  'P',  '{',  '}', '\n',    0,  'A',
         'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  '"',
         '~',    0,  '|',  'Z',  'X',  'C',  'V',  'B',  'N',  'M',
         '<',  '>',  '?',    0,    0,    0,  ' ' };
/* extended ascii code table to translate scan code */
unsigned char   kbctl[] = { 0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,   31,    0, '\b', '\t',   17,   23,    5,   18,   20,
          25,   21,    9,   15,   16,   27,   29, '\n',    0,    1,
          19,    4,    6,    7,    8,   10,   11,   12,    0,    0,
           0,    0,   28,   26,   24,    3,   22,    2,   14,   13 };
 
/**
 * [kbdInterruptHandler description]
 */
void kbdInterruptHandler() {
    unsigned char c_byte = inb(0x60);
    c_byte = kbtoa(c_byte);
    unsigned char byteDump;


    if(c_byte == eof){ // see if Control D is pressed if yes then close and dump whatever is inside a buffer
      kprintf("stopping keyboard, leftover elements in buffer:");
      while(bufhead != buftail + 1 % KBD_BUFSIZE){
        byteDump = buffer_c[bufhead];
        kprintf("%c", byteDump);
        if(bufhead == buftail){
          break;
        }
        bufhead = ((bufhead + 1) % KBD_BUFSIZE);
      }
      kprintf("\n");
      closeKbd();
    }


    if (kbd_echo == 1) {
        if (c_byte > 0){
            kprintf("%c", c_byte);
          }

    } else {
        // kprintf("3:%c\n", c_byte);
        if((buftail + 1) % (KBD_BUFSIZE) != bufhead && (c_byte > 0) && kbd_echo == 0){
          if(bufhead == buftail && buffer_c[buftail] == NULL){
            buffer_c[bufhead] = c_byte;
            //kprintf("wrote %c at location %d\n", c_byte, bufhead);
            // kprintf("!!\n");
          } else {
            if(bufhead == buftail){
            buftail = (buftail +1) % (KBD_BUFSIZE);
            buffer_c[buftail] = c_byte;
            //kprintf("wrote %c at location %d\n", c_byte, bufhead);
            // kprintf("@@\n");
          } else {
            buftail = (buftail +1) % (KBD_BUFSIZE);
            buffer_c[buftail] = c_byte; 
            // kprintf("##\n");
          }
            
          }
          
        } 
    }
}


// open keyboard routine
void openKbd(){
  outb(0x64, 0xAE);
} 
// close the keyboard
void closeKbd(){
  kbd_echo = -1;
  outb(0x64, 0xAD);
}

// *********** DEVICE READ: returns 0 on success and -1 on failure
int readKbd( unsigned char *buff, int bufflen) { // may need to add callback function and argument (seperately as a parameter) to call back
  unsigned char byteDump;
  if(buftail != bufhead || buffer_c[buftail] != NULL){
    *buff = buffer_c[bufhead];
    // kprintf("1:%c\n", buffer_c[bufhead]);
    // kprintf("bufhead:%d, buftail: %d\n", bufhead, buftail);
    buffer_c[bufhead] = 0;
    if(buftail != bufhead){
      ++bufhead;
    }
    


    if(*buff == eof){ // see if Control D is pressed if yes then close and dump whatever is inside a buffer
      kprintf("stopping keyboard, leftover elements in buffer:");
      while(bufhead != buftail + 1 % KBD_BUFSIZE){
        byteDump = buffer_c[bufhead];
        kprintf("%c", byteDump);
        if(bufhead == buftail){
          break;
        }
        bufhead = ((bufhead + 1) % KBD_BUFSIZE);

      }
      kprintf("\n");
      closeKbd();
    }
    return 0;
  }

  return -1;
  // may need call back to let user process know that there is more to read, success is not enough we have to invoke a function
  // lower half knows knowthing about upper half or kernel thus u cannot define global function and u have to define call back
}

// *********** DEVICE WRITE
int writeKbd(unsigned char *buff, int bufflen) {
  return -1; // Writing to keyboard not supported
}

// *********** DEVICE IOCTL
int ioctlKbd(unsigned long command, int command2){
    if (command == EOF) {
        eof = command2;

        return 0;
    }
    else if (command == NONECHO) { // turn echoing off
        kbd_echo = 0;
        return 0;
    }
    else if (command == ECHO) { // turn echoing on
        kbd_echo = 1;
        return 0;
    }
    return -1;
}

void keyboardInit(){ // echo keyboard
  eof = 4;
  bufhead = 0;
  buftail = 0;
  devtab[KBD].dvopen = &openKbd;
  devtab[KBD].dvclose = &closeKbd;
  devtab[KBD].dvread = &readKbd;
  devtab[KBD].dvwrite = &writeKbd;
  devtab[KBD].dvioctl = &ioctlKbd;
}

static int extchar(unsigned char code)
{
    state &= ~EXTENDED;
} 
 
 
unsigned int kbtoa( unsigned char code )
{
  unsigned int    ch;
 
  if (state & EXTENDED)
    return extchar(code);
  if (code & KEY_UP) {
    switch (code & 0x7f) {
    case LSHIFT:
    case RSHIFT:
      state &= ~INSHIFT;
      break;
    case CAPSL:
      // printf("Capslock off detected\n");
      state &= ~CAPSLOCK;
      break;
    case LCTL:
      state &= ~INCTL;
      break;
    case LMETA:
      state &= ~INMETA;
      break;
    }
   
    return NOCHAR;
  }
 
 
  /* check for special keys */
  switch (code) {
  case LSHIFT:
  case RSHIFT:
    state |= INSHIFT;
    // printf("shift detected!\n");
    return NOCHAR;
  case CAPSL:
    state |= CAPSLOCK;
    // printf("Capslock ON detected!\n");
    return NOCHAR;
  case LCTL:
    state |= INCTL;
    return NOCHAR;
  case LMETA:
    state |= INMETA;
    return NOCHAR;
  case EXTESC:
    state |= EXTENDED;
    return NOCHAR;
  }
 
  ch = NOCHAR;
 
  if (code < sizeof(kbcode)){
    if ( state & CAPSLOCK )
      ch = kbshift[code];
      else
        ch = kbcode[code];
  }
  if (state & INSHIFT) {
    if (code >= sizeof(kbshift))
      return NOCHAR;
    if ( state & CAPSLOCK )
      ch = kbcode[code];
    else
      ch = kbshift[code];
  }
  if (state & INCTL) {
    if (code >= sizeof(kbctl))
      return NOCHAR;
    ch = kbctl[code];
  }
  if (state & INMETA)
    ch += 0x80;
  return ch;
}


