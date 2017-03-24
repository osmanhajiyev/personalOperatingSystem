/* initialize.c - initproc */

#include <i386.h>
#include <xeroskernel.h>
#include <xeroslib.h>

extern	int	entry( void );  /* start of kernel image, use &start    */
extern	int	end( void );    /* end of kernel image, use &end        */
extern  long	freemem; 	/* start of free memory (set in i386.c) */
extern  char	*maxaddr;	/* max memory address (set in i386.c)	*/

/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED.  The     ***/
/***   interrupt table has been initialized with a default handler    ***/
/***								      ***/
/***								      ***/
/************************************************************************/

/*------------------------------------------------------------------------
 *  The init process, this is where it all begins...
 *------------------------------------------------------------------------
 */
void initproc( void )				/* The beginning */
{
  kprintf( "\n\nCPSC 415, 2016W1 \n32 Bit Xeros 0.01 \nLocated at: %x to %x\n", 
	   &entry, &end); 
  
  /* Your code goes here */
  
  kprintf("max addr is %d %x\n", maxaddr, maxaddr);
  
  kmeminit();
  kprintf("memory inited\n");
  
  dispatchinit();
  kprintf("dispatcher inited\n");
  
  contextinit();
  kprintf("context inited\n");
  
  kpcbinit();
  kprintf("memory blocks inited\n");
  
  if (!idlecreate()) {
    kprintf("idle creation has failed!");
    for(;;);
  }
  kprintf("created idle process.\n");
  
  keyboardInit();

  
  /**
   * Assignment 2 tests
   */

  // kprintf("create inited\n");

  // create( root, PROC_STACK, 0 );




  /** 
   * Additional tests
   * UNCOMMENT ONE BY ONE TO SEE THE TEST RESULTS
   */
  // create(root2, PROC_STACK, 0);
  // create(root3, PROC_STACK, 0);
  // create(root4, PROC_STACK, 0);
  // create(root5, PROC_STACK, 0);
 
  /**
   * Assignment 3 tests
   * UNCOMMENT ONE BY ONE TO SEE THE TEST RESULTS
   */
  // create(getquantum_master_test, PROC_STACK, 0); // testing sysgetcputime()
  // create(sig_test, PROC_STACK, 0); // testing signals
  // create(wait_test, PROC_STACK, 0); // testing signals
  // create(keyboardInitTest, PROC_STACK, 0);
  // create(rootFinal, PROC_STACK, 0);
  create(keyboardExtraTest, PROC_STACK, 0);

  kprintf("create inited\n");
  dispatch();
  
  kprintf("Returned to init, you should never get here!\n");
  
  /* This code should never be reached after you are done */
  for(;;) ; /* loop forever */
  
}


