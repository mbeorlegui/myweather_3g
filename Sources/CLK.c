#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "CLK.h"

void CLK_Init(void)
{
  /**** Moving from FEI (FLL engaged internal) to PEE (PLL engaged external) mode. */ 
  /* switch from FEI to FBE (FLL bypassed external) */ 
  /* enable external clock source */
  MCGC2 = MCGC2_HGO_MASK       		/* oscillator in high gain mode */
          | MCGC2_EREFS_MASK   		/* because crystal is being used */
          | MCGC2_RANGE_MASK   		/* 12 MHz is in high freq range */
          | MCGC2_ERCLKEN_MASK;   /* activate external reference clock */
  while (MCGSC_OSCINIT == 0){}
  /* select clock mode */
  MCGC1 = (2<<6)         			    /* CLKS = 10 -> external reference clock. */
          |(3<<3);       			    /* RDIV = 3 -> 12MHz/8=1.5 MHz */
  
  /* wait for mode change to be done */
  while (MCGSC_IREFST != 0){}
  while (MCGSC_CLKST != 2){}
    
  /* switch from FBE to PBE (PLL bypassed internal) mode */
  MCGC3=MCGC3_PLLS_MASK
        | (8<<0);    				      /* VDIV=6 -> multiply by 32 -> 1.5MHz * 32 = 48MHz */
  while(MCGSC_PLLST != 1){}
  while(MCGSC_LOCK != 1){}
  /* finally switch from PBE to PEE (PLL enabled external mode) */
  MCGC1 = (0<<6)         			    /* CLKS = 0 -> PLL or FLL output clock. */
          | (3<<3);      			    /* RDIV = 3 -> 12MHz/8=1.5 MHz */
  while(MCGSC_CLKST!=3){}
  /* Now MCGOUT=48MHz, BUS_CLOCK=24MHz */  
}
