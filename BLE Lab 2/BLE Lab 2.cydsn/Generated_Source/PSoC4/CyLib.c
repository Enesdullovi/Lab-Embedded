/*******************************************************************************
* File Name: CyLib.c
* Version 4.20
*
*  Description:
*   Provides a system API for the clocking, interrupts, and watchdog timer.
*
*  Note:
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2010-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CyLib.h"


/* Do not use these definitions directly in your application */
uint32 cydelayFreqHz  = CYDEV_BCLK__SYSCLK__HZ;
uint32 cydelayFreqKhz = (CYDEV_BCLK__SYSCLK__HZ + CY_DELAY_1K_MINUS_1_THRESHOLD) / CY_DELAY_1K_THRESHOLD;
uint8  cydelayFreqMhz = (uint8)((CYDEV_BCLK__SYSCLK__HZ + CY_DELAY_1M_MINUS_1_THRESHOLD) / CY_DELAY_1M_THRESHOLD);
uint32 cydelay32kMs   = CY_DELAY_MS_OVERFLOW * ((CYDEV_BCLK__SYSCLK__HZ + CY_DELAY_1K_MINUS_1_THRESHOLD) /
                        CY_DELAY_1K_THRESHOLD);

#if (CY_IP_WCO)
    static uint32 CySysClkGetLfclkSource(void);
#endif /* (CY_IP_WCO) */

#if(CY_IP_SRSSV2)
    /* CySysClkLfclkPosedgeCatch() / CySysClkLfclkPosedgeRestore() */
    uint32 lfclkPosedgeWdtCounter0Enabled = 0u;
    uint32 lfclkPosedgeEnabledWdtCounter  = 0u;
    uint32 lfclkPosedgeWdtCounter0Mode    = CY_SYS_WDT_MODE_NONE;

    static void CySysClkLfclkPosedgeCatch(void);
    static void CySysClkLfclkPosedgeRestore(void);

    static uint32 CySysWdtLocked(void);
    static uint32 CySysClkIloEnabled(void);
    #if (CY_IP_WCO)
        static uint32 CySysClkWcoEnabled(void);
    #endif /* (CY_IP_WCO) */
#endif /* (CY_IP_SRSSV2) */


static cySysTickCallback CySysTickCallbacks[CY_SYS_SYST_NUM_OF_CALLBACKS];
static void CySysTickServiceCallbacks(void);
uint32 CySysTickInitVar = 0u;


#if(CY_IP_SRSSV2)
        /* Conversion between CySysClkWriteImoFreq() parameter and register's value */
        const uint8 cyImoFreqMhz2Reg[CY_SYS_CLK_IMO_FREQ_TABLE_SIZE] = {
            /*  3 MHz */ 0x03u,  /*  4 MHz */ 0x04u,  /*  5 MHz */ 0x05u,  /*  6 MHz */ 0x06u,
            /*  7 MHz */ 0x07u,  /*  8 MHz */ 0x08u,  /*  9 MHz */ 0x09u,  /* 10 MHz */ 0x0Au,
            /* 11 MHz */ 0x0Bu,  /* 12 MHz */ 0x0Cu,  /* 13 MHz */ 0x0Eu,  /* 14 MHz */ 0x0Fu,
            /* 15 MHz */ 0x10u,  /* 16 MHz */ 0x11u,  /* 17 MHz */ 0x12u,  /* 18 MHz */ 0x13u,
            /* 19 MHz */ 0x14u,  /* 20 MHz */ 0x15u,  /* 21 MHz */ 0x16u,  /* 22 MHz */ 0x17u,
            /* 23 MHz */ 0x18u,  /* 24 MHz */ 0x19u,  /* 25 MHz */ 0x1Bu,  /* 26 MHz */ 0x1Cu,
            /* 27 MHz */ 0x1Du,  /* 28 MHz */ 0x1Eu,  /* 29 MHz */ 0x1Fu,  /* 30 MHz */ 0x20u,
            /* 31 MHz */ 0x21u,  /* 32 MHz */ 0x22u,  /* 33 MHz */ 0x23u,  /* 34 MHz */ 0x25u,
            /* 35 MHz */ 0x26u,  /* 36 MHz */ 0x27u,  /* 37 MHz */ 0x28u,  /* 38 MHz */ 0x29u,
            /* 39 MHz */ 0x2Au,  /* 40 MHz */ 0x2Bu,  /* 41 MHz */ 0x2Eu,  /* 42 MHz */ 0x2Fu,
            /* 43 MHz */ 0x30u,  /* 44 MHz */ 0x31u,  /* 45 MHz */ 0x32u,  /* 46 MHz */ 0x33u,
            /* 47 MHz */ 0x34u,  /* 48 MHz */ 0x35u };
#endif /* (CY_IP_SRSSV2) */


/*******************************************************************************
* Function Name: CySysClkImoStart
********************************************************************************
*
* Summary:
*  Enables the IMO.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysClkImoStart(void)
{
    CY_SYS_CLK_IMO_CONFIG_REG |= CY_SYS_CLK_IMO_CONFIG_ENABLE;
}


/*******************************************************************************
* Function Name: CySysClkImoStop
********************************************************************************
*
* Summary:
*  Disables the IMO.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysClkImoStop(void)
{
    CY_SYS_CLK_IMO_CONFIG_REG &= ( uint32 ) ( ~( uint32 )CY_SYS_CLK_IMO_CONFIG_ENABLE);
}


/*******************************************************************************
* Function Name: CySysClkIloStart
********************************************************************************
*
* Summary:
*  Enables the ILO. Refer to the device datasheet for the ILO startup time.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysClkIloStart(void)
{
    CY_SYS_CLK_ILO_CONFIG_REG |= CY_SYS_CLK_ILO_CONFIG_ENABLE;
}


/*******************************************************************************
* Function Name: CySysClkIloStop
********************************************************************************
*
* Summary:
*  Disables the ILO.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This function will have no effect if the WDT is locked (CySysWdtLock() is
*  called). Call CySysWdtUnlock() to unlock the WDT and be able to stop ILO.
*
*  PSoC 4100 / PSoC 4200: Note that ILO is required for WDT's operation.
*  The ILO is required for the WDT operation.
*
*  PSoC 4100-BL / PSoC 4200-BL:
*  Stopping the ILO will affect the peripheral clocked by the LFCLK, if the
*  LFCLK is configured to be sourced by the ILO.
*
*******************************************************************************/
void CySysClkIloStop(void)
{
    #if(CY_IP_SRSSV2)
        uint8  interruptState;

        /* Do nothing if WDT is locked or ILO is disabled */
        if (0u == CySysWdtLocked())
        {
            if (0u != CySysClkIloEnabled())
            {

            #if (CY_IP_WCO)
                if (CY_SYS_CLK_LFCLK_SRC_ILO == CySysClkGetLfclkSource())
                {
            #endif /* (CY_IP_WCO) */

                interruptState = CyEnterCriticalSection();
                CySysClkLfclkPosedgeCatch();
                CY_SYS_CLK_ILO_CONFIG_REG &= ( uint32 ) ( ~( uint32 )CY_SYS_CLK_ILO_CONFIG_ENABLE);
                CySysClkLfclkPosedgeRestore();
                CyExitCriticalSection(interruptState);

            #if (CY_IP_WCO)
                }
                else /* Safe to disable - shortened pulse does not impact peripheral */
                {
                    CY_SYS_CLK_ILO_CONFIG_REG &= ( uint32 ) ( ~( uint32 )CY_SYS_CLK_ILO_CONFIG_ENABLE);
                }
            #endif /* (CY_IP_WCO) */

            }
        }
    #else
        CY_SYS_CLK_ILO_CONFIG_REG &= ( uint32 ) ( ~( uint32 )CY_SYS_CLK_ILO_CONFIG_ENABLE);
    #endif /* (CY_IP_SRSSV2) */
}

#if(CY_IP_SRSSV2)
    /*******************************************************************************
    * Function Name: CySysClkIloEnabled
    ********************************************************************************
    *
    * Summary:
    *  Reports ILO enable state.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  1 if ILO is enabled, and 0 if ILO is disabled.
    *
    *******************************************************************************/
    static uint32 CySysClkIloEnabled(void)
    {
        /* Prohibits writing to WDT registers and ILO/WCO registers when not equal 0 */
        return ((0u != (CY_SYS_CLK_ILO_CONFIG_REG & (uint32)(CY_SYS_CLK_ILO_CONFIG_ENABLE))) ?
            (uint32) 1u :
            (uint32) 0u);
    }
#endif /* (CY_IP_SRSSV2) */


/*******************************************************************************
* Function Name: CySysClkGetLfclkSource
********************************************************************************
*
* Summary:
*  Gets the clock source for the LFCLK clock.
*  The function is applicable only for the PSoC 4100-BL and PSoC 4200-BL.
*
* Parameters:
*  None
*
* Return:
*  The LFCLK source:
*   CY_SYS_CLK_LFCLK_SRC_ILO    Internal Low Frequency (32 kHz) Oscillator (ILO)
*   CY_SYS_CLK_LFCLK_SRC_WCO    Low Frequency Watch Crystal Oscillator (WCO)
*
*******************************************************************************/
#if (CY_IP_WCO)
    static uint32 CySysClkGetLfclkSource(void)
    {
        uint32 lfclkSource;
        lfclkSource = CY_SYS_WDT_CONFIG_REG & CY_SYS_CLK_LFCLK_SEL_MASK;
        return (lfclkSource);
    }
#endif /* (CY_IP_WCO) */


/*******************************************************************************
* Function Name: CySysClkSetLfclkSource
********************************************************************************
*
* Summary:
*  Sets the clock source for the LFCLK clock.
*
*  The switch between LFCLK sources must be done between positive edges of
*  the LFCLK, because the glitch risk is around LFCLK positive edge. To ensure
*  that the switch can be done safely, the WDT counter value is read until it
*  changes.
*
*  That means that the positive edge just finished and the switch is performed.
*  The enabled WDT counter is used for that purpose. If no counters are enabled,
*  the counter 0 is enabled. And after LFCLK source is switched, the counter 0
*  configuration is restored back.
*
*  The function is applicable only for the PSoC 4100-BL and PSoC 4200-BL.
*
* Parameters:
*  source: One of the available LFCLK sources.
*   CY_SYS_CLK_LFCLK_SRC_ILO    Internal Low Frequency (32 kHz) Oscillator (ILO)
*   CY_SYS_CLK_LFCLK_SRC_WCO    Low Frequency Watch Crystal Oscillator (WCO)
*
* Return:
*  None
*
* Side Effects:
*  This function will have no effect if the WDT is locked (CySysWdtLock() is
*  called). Call CySysWdtUnlock() to unlock the WDT.
*
*  The current source and the new source must both be running and stable before
*  calling this function.
*
*  Changing the LFCLK clock source may change the LFCLK clock frequency and
*  affect the functionality that uses this clock. For example, watchdog timer
*  (WDT) is clocked by LFCLK.
*
*******************************************************************************/
#if (CY_IP_WCO)
    void CySysClkSetLfclkSource(uint32 source)
    {
        uint8  interruptState;

        if (CySysClkGetLfclkSource() != source)
        {
            interruptState = CyEnterCriticalSection();

            CySysClkLfclkPosedgeCatch();

            CY_SYS_WDT_CONFIG_REG = (CY_SYS_WDT_CONFIG_REG & (uint32)(~CY_SYS_CLK_LFCLK_SEL_MASK)) |
                                    (source & CY_SYS_CLK_LFCLK_SEL_MASK);

            CySysClkLfclkPosedgeRestore();

            CyExitCriticalSection(interruptState);
        }
    }
#endif /* (CY_IP_WCO) */


/*******************************************************************************
* Function Name: CySysClkWriteHfclkDirect
********************************************************************************
*
* Summary:
*  Selects the direct source for the HFCLK.
*
* Parameters:
*  clkSelect: One of the available HFCLK direct sources:
*   Define                    Source
*    CY_SYS_CLK_HFCLK_IMO      IMO
*    CY_SYS_CLK_HFCLK_EXTCLK   External clock pin
*    CY_SYS_CLK_HFCLK_ECO      External crystal oscillator
*                                           (applicable only for PSoC 4200-BL)
*
* Return:
*  None
*
* Side Effects:
*  The new source must be running and stable before calling this function.
*
*  PSoC 4000:
*  The SYSCLK has a maximum speed of 16 MHz, so HFCLK and SYSCLK dividers should
*  be selected in a way to not to exceed 16 MHz for the System clock.
*
*  If the SYSCLK clock frequency increases during device operation, call
*  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
*  of clock cycles the cache will wait before sampling data comes back from Flash.
*  If the SYSCLK clock frequency decreases, you can call
*  CySysFlashSetWaitCycles() to improve the CPU performance. See
*  CySysFlashSetWaitCycles() description for more information.
*
*******************************************************************************/
void CySysClkWriteHfclkDirect(uint32 clkSelect)
{
    uint8  interruptState;

    interruptState = CyEnterCriticalSection();

    CY_SYS_CLK_SELECT_REG = ((CY_SYS_CLK_SELECT_REG & (( uint32 ) ~(( uint32 )CY_SYS_CLK_SELECT_DIRECT_SEL_MASK))) |
                        (( uint32 ) (clkSelect & ( uint32 )CY_SYS_CLK_SELECT_DIRECT_SEL_PARAM_MASK)));

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CySysClkWriteSysclkDiv
********************************************************************************
*
* Summary:
*  Selects the prescaler divide amount for SYSCLK from HFCLK.
*
* Parameters:
*  divider: Power of 2 prescaler selection
*
*   Define                        Description
*   CY_SYS_CLK_SYSCLK_DIV1        SYSCLK = HFCLK / 1
*   CY_SYS_CLK_SYSCLK_DIV2        SYSCLK = HFCLK / 2
*   CY_SYS_CLK_SYSCLK_DIV4        SYSCLK = HFCLK / 4
*   CY_SYS_CLK_SYSCLK_DIV8        SYSCLK = HFCLK / 8
*   CY_SYS_CLK_SYSCLK_DIV16       SYSCLK = HFCLK / 16  (N/A for 4000 Family)
*   CY_SYS_CLK_SYSCLK_DIV32       SYSCLK = HFCLK / 32  (N/A for 4000 Family)
*   CY_SYS_CLK_SYSCLK_DIV64       SYSCLK = HFCLK / 64  (N/A for 4000 Family)
*   CY_SYS_CLK_SYSCLK_DIV128      SYSCLK = HFCLK / 128 (N/A for 4000 Family)
*
* Return:
*  None
*
* Side Effects:
*  4000 Family:
*  The SYS_CLK has the speed of 16 MHz, so dividers HF_CLK and SYS_CLK
*  dividers should be selected in a way, not to exceed 16 MHz for SYS_CLK.
*
*  If the SYSCLK clock frequency increases during the device operation, call
*  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
*  of clock cycles the cache will wait before sampling data comes back from Flash.
*  If the SYSCLK clock frequency decreases, you can call
*  CySysFlashSetWaitCycles() to improve the CPU performance. See
*  CySysFlashSetWaitCycles() description for more information.
*
*******************************************************************************/
void CySysClkWriteSysclkDiv(uint32 divider)
{
    uint8  interruptState;

    interruptState = CyEnterCriticalSection();

    CY_SYS_CLK_SELECT_REG = ((uint32)(((uint32)divider & CY_SYS_CLK_SELECT_SYSCLK_DIV_MASK) <<
                                    CY_SYS_CLK_SELECT_SYSCLK_DIV_SHIFT)) |
                            (CY_SYS_CLK_SELECT_REG & ((uint32)(~(uint32)(CY_SYS_CLK_SELECT_SYSCLK_DIV_MASK <<
                                    CY_SYS_CLK_SELECT_SYSCLK_DIV_SHIFT))));

    CyExitCriticalSection(interruptState);
}


#if(CY_IP_SRSSV2)

    /*******************************************************************************
    * Function Name: CySysClkWriteImoFreq
    ********************************************************************************
    *
    * Summary:
    *  Sets the frequency of the IMO.
    *
    * Parameters:
    *  freq: Valid range [3-48].  Frequency for operation of the IMO.
    *  Note: Invalid frequency will be ignored.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  If the SYSCLK clock frequency increases during the device operation, call
    *  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
    *  of clock cycles the cache will wait before sampling data comes back from Flash.
    *  If the SYSCLK clock frequency decreases, you can call
    *  CySysFlashSetWaitCycles() to improve the CPU performance.
    *  See CySysFlashSetWaitCycles() description for more information.
    *
    *******************************************************************************/
    void CySysClkWriteImoFreq(uint32 freq)
    {
        uint8  bgTrim4;
        uint8  bgTrim5;
        uint8  interruptState;
        uint8  newImoTrim2Value;
        uint8  currentImoTrim2Value;


        if ((freq >= CY_SYS_CLK_IMO_MIN_FREQ_MHZ) && (freq <= CY_SYS_CLK_IMO_MAX_FREQ_MHZ))
        {
            if(freq <= CY_SFLASH_IMO_MAXF0_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS0_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO0_REG;
            }
            else if(freq <= CY_SFLASH_IMO_MAXF1_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS1_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO1_REG;
            }
            else if(freq <= CY_SFLASH_IMO_MAXF2_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS2_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO2_REG;
            }
            else if(freq <= CY_SFLASH_IMO_MAXF3_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS3_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO3_REG;
            }
            else
            {
                bgTrim4 = CY_SFLASH_IMO_ABS4_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO4_REG;
            }

            /* Get IMO_TRIM2 value for the new frequency */
            newImoTrim2Value = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];


            interruptState = CyEnterCriticalSection();

            /***************************************************************************
            * The IMO can have a different trim per frequency. To avoid possible corner
            * cases where a trim change can exceed the maximum frequency, the trim must
            * be applied at a frequency that is low enough.
            *
            * Comparing IMO_TRIM2 values for the current and new frequencies, since
            * IMO_TRIM2 value as a function of IMO frequency is a strictly increasing
            * function and is time-invariant.
            ***************************************************************************/
            if ((newImoTrim2Value >= CY_SYS_CLK_IMO_BOUNDARY_FREQ_TRIM2) && (freq >= CY_SYS_CLK_IMO_BOUNDARY_FREQ_MHZ))
            {
                /* Set boundary IMO frequency: safe for IMO above 48 MHZ trimming */
                CY_SYS_CLK_IMO_TRIM2_REG = (uint32) cyImoFreqMhz2Reg[CY_SYS_CLK_IMO_TEMP_FREQ_MHZ -
                                                                     CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];

                CyDelayCycles(CY_SYS_CLK_IMO_FREQ_TIMEOUT_CYCLES);

                currentImoTrim2Value = CY_SYS_CLK_IMO_TEMP_FREQ_TRIM2;
            }
            else
            {
                currentImoTrim2Value = (uint8) (CY_SYS_CLK_IMO_TRIM2_REG & CY_SYS_CLK_IMO_FREQ_BITS_MASK);
            }


            /***************************************************************************
            * A trim change needs to be allowed to settle (within 5us) before the Freq
            * can be changed to a new frequency.
            *
            * Comparing IMO_TRIM2 values for the current and new frequencies, since
            * IMO_TRIM2 value as a function of IMO frequency is a strictly increasing
            * function and is time-invariant.
            ***************************************************************************/
            if (newImoTrim2Value < currentImoTrim2Value)
            {
                /* Set new IMO frequency */
                CY_SYS_CLK_IMO_TRIM2_REG = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];
                CyDelayCycles(CY_SYS_CLK_IMO_FREQ_TIMEOUT_CYCLES);
            }

            /* Set trims for the new IMO frequency */
            CY_SYS_CLK_IMO_TRIM1_REG = (uint32) CY_SFLASH_IMO_TRIM_REG(freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET);
            CY_PWR_BG_TRIM4_REG  = bgTrim4;
            CY_PWR_BG_TRIM5_REG  = bgTrim5;
            CyDelayUs(CY_SYS_CLK_IMO_TRIM_TIMEOUT_US);

            if (newImoTrim2Value > currentImoTrim2Value)
            {
                /* Set new IMO frequency */
                CY_SYS_CLK_IMO_TRIM2_REG = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];
                CyDelayCycles(CY_SYS_CLK_IMO_FREQ_TIMEOUT_CYCLES);
            }

            CyExitCriticalSection(interruptState);
        }
        else
        {
            /* Halt CPU in debug mode if new frequency is invalid */
            CYASSERT(0u != 0u);
        }
    }


#else


    /*******************************************************************************
    * Function Name: CySysClkWriteHfclkDiv
    ********************************************************************************
    *
    * Summary:
    *  Selects HF clk predivider value.
    *
    * Parameters:
    *  divider: HF clock divider value
    *   Define                        Description
    *   CY_SYS_CLK_HFCLK_DIV_NODIV    Transparent mode (w/o dividing)
    *   CY_SYS_CLK_HFCLK_DIV_2        Divide selected clock source by 2
    *   CY_SYS_CLK_HFCLK_DIV_4        Divide selected clock source by 4
    *   CY_SYS_CLK_HFCLK_DIV_8        Divide selected clock source by 8
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  If the SYSCLK clock frequency increases during the device operation, call
    *  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
    *  of clock cycles the cache will wait before sampling data comes back from Flash.
    *  If the SYSCLK clock frequency decreases, you can call
    *  CySysFlashSetWaitCycles() to improve the CPU performance. See
    *  CySysFlashSetWaitCycles() description for more information.
    *

    *
    *******************************************************************************/
    void CySysClkWriteHfclkDiv(uint32 divider)
    {
        uint8  interruptState;

        interruptState = CyEnterCriticalSection();

        CY_SYS_CLK_SELECT_REG = ((CY_SYS_CLK_SELECT_REG & ((uint32) (~(CY_SYS_CLK_SELECT_HFCLK_DIV_MASK <<
                                        CY_SYS_CLK_SELECT_HFCLK_DIV_SHIFT)))) |
                    ((uint32)((divider & CY_SYS_CLK_SELECT_HFCLK_DIV_MASK) << CY_SYS_CLK_SELECT_HFCLK_DIV_SHIFT)));

        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CySysClkWriteImoFreq
    ********************************************************************************
    *
    * Summary:
    *  Sets the frequency of the IMO.
    *
    * Parameters:
    *  freq: Valid values are 24, 32 and 48 MHz.
    *
    *  Note: The CPU is halted if new frequency is invalid and project is
    *  compiled in debug mode.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  If the SYSCLK clock frequency increases during the device operation, call
    *  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
    *  of clock cycles the cache will wait before sampling data comes back from Flash.
    *  If the SYSCLK clock frequency decreases, you can call
    *  CySysFlashSetWaitCycles() to improve the CPU performance. See
    *  CySysFlashSetWaitCycles() description for more information.
    *
    *  The System Clock (SYSCLK) has maximum speed of 16 MHz, so HFCLK and SYSCLK
    *  dividers should be selected in a way, to not to exceed 16 MHz for the System
    *  clock.
    *
    *******************************************************************************/
    void CySysClkWriteImoFreq(uint32 freq)
    {
        uint8  interruptState;

        if ((freq == 24u) || (freq == 32u) || (freq == 48u))
        {
            interruptState = CyEnterCriticalSection();

            /* Set IMO to 24 MHz - CLK_IMO_SELECT.FREQ = 0 */
            CY_SYS_CLK_IMO_SELECT_REG &= ((uint32) ~CY_SYS_CLK_IMO_SELECT_FREQ_MASK);

            /* Apply coarse trim */
            CY_SYS_CLK_IMO_TRIM1_REG = (CY_SYS_CLK_IMO_TRIM1_REG & ((uint32) ~CY_SYS_CLK_IMO_TRIM1_OFFSET_MASK)) |
                (CY_SFLASH_IMO_TRIM_REG(freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) & CY_SYS_CLK_IMO_TRIM1_OFFSET_MASK);

            /* Zero out fine trim */
            CY_SYS_CLK_IMO_TRIM2_REG = CY_SYS_CLK_IMO_TRIM2_REG & ((uint32) ~CY_SYS_CLK_IMO_TRIM2_FSOFFSET_MASK);

            /* Apply TC trim */
            CY_SYS_CLK_IMO_TRIM3_REG = (CY_SYS_CLK_IMO_TRIM3_REG & ((uint32) ~CY_SYS_CLK_IMO_TRIM3_VALUES_MASK)) |
                (CY_SFLASH_IMO_TCTRIM_REG(freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) & CY_SYS_CLK_IMO_TRIM3_VALUES_MASK);

            CyDelayCycles(CY_SYS_CLK_IMO_TRIM_DELAY_US);

            if (freq > CY_SYS_CLK_IMO_MIN_FREQ_MHZ)
            {
                /* Select nearby intermediate frequency */
                CY_SYS_CLK_IMO_SELECT_REG = (CY_SYS_CLK_IMO_SELECT_REG & ((uint32) ~CY_SYS_CLK_IMO_SELECT_FREQ_MASK)) |
                    (((freq - 4u - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) >> 2u) & CY_SYS_CLK_IMO_SELECT_FREQ_MASK);

                CyDelayCycles(CY_SYS_CLK_IMO_TRIM_DELAY_US);

                /* Make small step to final frequency */
                /* Select nearby intermediate frequency */
                CY_SYS_CLK_IMO_SELECT_REG = (CY_SYS_CLK_IMO_SELECT_REG & ((uint32) ~CY_SYS_CLK_IMO_SELECT_FREQ_MASK)) |
                    (((freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) >> 2u) & CY_SYS_CLK_IMO_SELECT_FREQ_MASK);
            }

            CyExitCriticalSection(interruptState);
        }
        else
        {
            /* Halt CPU in debug mode if new frequency is invalid */
            CYASSERT(0u != 0u);
        }
    }

#endif /* (CY_IP_SRSSV2) */


#if (CY_PSOC4_4100BL || CY_PSOC4_4200BL)

    /*******************************************************************************
    * Function Name: CySysClkEcoStart
    ********************************************************************************
    *
    * Summary:
    *  Starts the External Crystal Oscillator (ECO). Refer to the device datasheet
    *  for the ECO startup time.
    *
    *  The timeout interval is measured based on the system frequency defined by
    *  PSoC Creator at build time. If System clock frequency is changed in
    *  runtime, the CyDelayFreq() with the appropriate parameter should be called.
    *
    * Parameters:
    *  timeoutUs: Timeout in microseconds. If zero is specified, the function
    *             starts the crystal and returns CYRET_SUCCESS. If non-zero
    *             value is passed, the CYRET_SUCCESS is returned once crystal
    *             is oscillating and amplitude reached 60% and it does not mean
    *             24 MHz crystal is within 50 ppm. If it is not oscillating or
    *             amplitude didn't reach 60% after specified amount of time, the
    *             CYRET_TIMEOUT is returned.
    *
    * Return:
    *   CYRET_SUCCESS - Completed successfully. The ECO is oscillating and amplitude
    *                   reached 60% and it does not mean 24 MHz crystal is within
    *                   50 ppm.
    *   CYRET_TIMEOUT - Timeout occurred.
    *
    *******************************************************************************/
    cystatus CySysClkEcoStart(uint32 timeoutUs)
    {
        cystatus status = CYRET_SUCCESS;

        /* Enable the RF oscillator band gap */
        CY_SYS_XTAL_BLESS_RF_CONFIG_REG |= CY_SYS_XTAL_BLESS_RF_CONFIG_RF_ENABLE;

        /* Update trimming register */
        CY_SYS_XTAL_BLERD_BB_XO_REG = CY_SYS_XTAL_BLERD_BB_XO_TRIM;

        /* Enable the Crystal */
        CY_SYS_XTAL_BLERD_DBUS_REG |= CY_SYS_XTAL_BLERD_DBUS_XTAL_ENABLE;

        if(timeoutUs > 0u)
        {
            status = CYRET_TIMEOUT;

            for( ; timeoutUs > 0u; timeoutUs--)
            {
                CyDelayUs(1u);

                if(0u != CySysClkEcoReadStatus())
                {
                    status = CYRET_SUCCESS;
                    break;
                }
            }

        }

        return(status);
    }


    /*******************************************************************************
    * Function Name: CySysClkEcoStop
    ********************************************************************************
    *
    * Summary:
    *  Stops the megahertz crystal.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkEcoStop(void)
    {
        /* Disable the RF oscillator band gap */
        CY_SYS_XTAL_BLESS_RF_CONFIG_REG &= (uint32) ~CY_SYS_XTAL_BLESS_RF_CONFIG_RF_ENABLE;

        /* Disable the Crystal */
        CY_SYS_XTAL_BLERD_DBUS_REG &= (uint32) ~CY_SYS_XTAL_BLERD_DBUS_XTAL_ENABLE;
    }


    /*******************************************************************************
    * Function Name: CySysClkEcoReadStatus
    ********************************************************************************
    *
    * Summary:
    *  Read status bit (50 ppm reached) for the megahertz crystal.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Non-zero indicates that XTAL output reached 50 ppm.
    *
    *******************************************************************************/
    uint32 CySysClkEcoReadStatus(void)
    {
        return (CY_SYS_XTAL_BLERD_FSM_REG & CY_SYS_XTAL_BLERD_FSM_XO_AMP_DETECT);
    }


    /*******************************************************************************
    * Function Name: CySysClkWriteEcoDiv
    ********************************************************************************
    *
    * Summary:
    *  Selects value for the ECO divider.
    *
    * Parameters:
    *  divider: Power of 2 divider selection.
    *
    *   Define                        Description
    *   CY_SYS_CLK_ECO_DIV1             HFCLK = ECO / 1
    *   CY_SYS_CLK_ECO_DIV2             HFCLK = ECO / 2
    *   CY_SYS_CLK_ECO_DIV4             HFCLK = ECO / 4
    *   CY_SYS_CLK_ECO_DIV8             HFCLK = ECO / 8
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  If the SYSCLK clock frequency increases during the device operation, call
    *  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
    *  of clock cycles the cache will wait before sampling data comes back from Flash.
    *  If the SYSCLK clock frequency decreases, you can call
    *  CySysFlashSetWaitCycles() to improve the CPU performance. See
    *  CySysFlashSetWaitCycles() description for more information.
    *
    *******************************************************************************/
    void CySysClkWriteEcoDiv(uint32 divider)
    {
        uint8  interruptState;

        interruptState = CyEnterCriticalSection();

        CY_SYS_CLK_XTAL_CLK_DIV_CONFIG_REG =  (divider & CY_SYS_CLK_XTAL_CLK_DIV_MASK) |
                                              (CY_SYS_CLK_XTAL_CLK_DIV_CONFIG_REG & ((uint32) ~CY_SYS_CLK_XTAL_CLK_DIV_MASK));

        CyExitCriticalSection(interruptState);
    }
#endif /* (CY_PSOC4_4100BL || CY_PSOC4_4200BL) */


#if (CY_IP_WCO)
    /*******************************************************************************
    * Function Name: CySysClkWcoStart
    ********************************************************************************
    *
    * Summary:
    *  Enables the Watch Crystal Oscillator (WCO). The WCO is used as a source for
    *  LFCLK.  Similar to ILO, WCO is also available in all modes except Hibernate
    *  and Stop modes.
    *
    *  The WCO is always enabled in High Power Mode (HPM). Refer to the device
    *  datasheet for the  WCO startup time. Once WCO becomes stable it can be
    *  switched to the Low Power Mode (LPM). Note that oscillator can be unstable
    *  during switch and hence its output should not be used at the moment.
    *
    *  The CySysClkWcoSetPowerMode() function configures WCO power mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkWcoStart(void)
    {
        CySysClkWcoSetHighPowerMode();
        CY_SYS_CLK_WCO_CONFIG_REG |= CY_SYS_CLK_WCO_CONFIG_LPM_ENABLE;
    }


    /*******************************************************************************
    * Function Name: CySysClkWcoStop
    ********************************************************************************
    *
    * Summary:
    *  Disables the 32KHz Crystal Oscillator.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void  CySysClkWcoStop(void)
    {
        uint8  interruptState;

        if (0u != CySysClkWcoEnabled())
        {

            if (CY_SYS_CLK_LFCLK_SRC_WCO == CySysClkGetLfclkSource())
            {
                interruptState = CyEnterCriticalSection();
                CySysClkLfclkPosedgeCatch();
                CY_SYS_CLK_WCO_CONFIG_REG &= (uint32) ~CY_SYS_CLK_WCO_CONFIG_LPM_ENABLE;
                CySysClkLfclkPosedgeRestore();
                CyExitCriticalSection(interruptState);
            }
            else /* Safe to disable - shortened pulse does not impact peripheral */
            {
                CY_SYS_CLK_WCO_CONFIG_REG &= (uint32) ~CY_SYS_CLK_WCO_CONFIG_LPM_ENABLE;
            }
        } /* Otherwise do nothing. WCO configuration cannot be changed. */
    }


    /*******************************************************************************
    * Function Name: CySysClkWcoEnabled
    ********************************************************************************
    *
    * Summary:
    *  Reports WCO enable state.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  1 if WCO is enabled, and 0 if WCO is disabled.
    *
    *******************************************************************************/
    static uint32 CySysClkWcoEnabled(void)
    {
        return ((0u != (CY_SYS_CLK_WCO_CONFIG_REG & (uint32)(CY_SYS_CLK_WCO_CONFIG_LPM_ENABLE))) ?
                (uint32) 1u :
                (uint32) 0u);
    }


    /*******************************************************************************
    * Function Name: CySysClkWcoSetPowerMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the power mode for the 32 KHz WCO.
    *
    * Parameters:
    *  uint32 mode:
    *   CY_SYS_CLK_WCO_HPM  - High power mode
    *   CY_SYS_CLK_WCO_LPM  - Low power mode
    *
    * Return:
    *  Previous power mode. The same as parameters.
    *
    *******************************************************************************/
    uint32 CySysClkWcoSetPowerMode(uint32 mode)
    {
        uint32 status;

        status = CY_SYS_CLK_WCO_CONFIG_REG & CY_SYS_CLK_WCO_LPM;

        switch(mode)
        {
        case CY_SYS_CLK_WCO_HPM:
                CySysClkWcoSetHighPowerMode();
            break;

        case CY_SYS_CLK_WCO_LPM:
                CySysClkWcoSetLowPowerMode();
            break;

        default:
            CYASSERT(0u != 0u);
            break;
        }

        return (status);
    }

#endif  /* (CY_IP_WCO) */


#if(CY_IP_SRSSV2)


    /*******************************************************************************
    * Function Name: CySysWdtLock
    ********************************************************************************
    *
    * Summary:
    *  Locks out configuration changes to the Watchdog timer registers and ILO
    *  configuration register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Side effects:
    *  After this function was called, ILO clock can't be disabled until
    *  CySysWdtUnlock() is called.
    *
    *******************************************************************************/
    void CySysWdtLock(void)
    {
        uint8 interruptState;
        interruptState = CyEnterCriticalSection();

        CY_SYS_CLK_SELECT_REG = (CY_SYS_CLK_SELECT_REG & (uint32)(~CY_SYS_WDT_CLK_LOCK_BITS_MASK)) |
                 CY_SYS_WDT_CLK_LOCK_BITS_MASK;

        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CySysWdtLocked
    ********************************************************************************
    *
    * Summary:
    *  Reports WDT lock state.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  1 - WDT is locked, and 0 - WDT is unlocked.
    *
    *******************************************************************************/
    static uint32 CySysWdtLocked(void)
    {
        /* Prohibits writing to WDT registers and ILO/WCO registers when not equal 0 */
        return ((0u != (CY_SYS_CLK_SELECT_REG & (uint32)(CY_SYS_WDT_CLK_LOCK_BITS_MASK))) ? (uint32) 1u : (uint32) 0u);
    }


    /*******************************************************************************
    * Function Name: CySysWdtUnlock
    ********************************************************************************
    *
    * Summary:
    *  Unlocks the Watchdog Timer configuration register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysWdtUnlock(void)
    {
        uint8 interruptState;
        interruptState = CyEnterCriticalSection();

        /* Removing WDT lock requires two writes */
        CY_SYS_CLK_SELECT_REG = ((CY_SYS_CLK_SELECT_REG & (uint32)(~CY_SYS_WDT_CLK_LOCK_BITS_MASK)) |
                CY_SYS_WDT_CLK_LOCK_BIT0);

        CY_SYS_CLK_SELECT_REG = ((CY_SYS_CLK_SELECT_REG & (uint32)(~CY_SYS_WDT_CLK_LOCK_BITS_MASK)) |
                CY_SYS_WDT_CLK_LOCK_BIT1);

        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadEnabledStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the enabled status of one of the three WDT counters.
    *
    * Parameters:
    *  counterNum: Valid range [0-2].  Number of the WDT counter.
    *
    * Return:
    *  Status of WDT counter:
    *   0 - counter is disabled
    *   1 - counter is enabled
    *
    * Side Effects:
    *  This function returns actual WDT counter status from status register. It may
    *  take up to 3 LFCLK cycles since WDT counter was enabled for WDT status
    *  register to contain actual data.
    *
    *******************************************************************************/
    uint32 CySysWdtReadEnabledStatus(uint32 counterNum)
    {
        CYASSERT(counterNum < CY_SYS_WDT_COUNTERS_MAX);
        return ((CY_SYS_WDT_CONTROL_REG >> ((CY_SYS_WDT_CNT_SHIFT * counterNum) + CY_SYS_WDT_CNT_STTS_SHIFT)) & 0x01u);
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteMode
    ********************************************************************************
    *
    * Summary:
    *   Writes the mode of one of the three WDT counters.
    *
    * Parameters:
    *   counterNum: Valid range [0-2].  Number of the WDT counter.
    *   mode:       Mode of operation for the counter
    *       Define                   Mode
    *    CY_SYS_WDT_MODE_NONE       Free running
    *    CY_SYS_WDT_MODE_INT        Interrupt generated on match for counter 0
    *                               and 1, and on bit toggle for counter 2.
    *    CY_SYS_WDT_MODE_RESET      Reset on match (valid for counter 0 and 1
    *                               only)
    *    CY_SYS_WDT_MODE_INT_RESET  Generate an interrupt.  Generate a reset on
    *                                    the 3rd unhandled interrupt.
    *                                    (valid for counter 0 and 1 only)
    * Return:
    *   None
    *
    * Side Effects:
    *   WDT counter counterNum should be disabled to set mode. Otherwise this
    *   function call will have no effect. If the specified counter is enabled,
    *   call CySysWdtDisable() function with the corresponding parameter to
    *   disable specified counter and wait for it to stop.
    *
    *******************************************************************************/
    void CySysWdtWriteMode(uint32 counterNum, uint32 mode)
    {
        uint32 configRegValue;

        CYASSERT(counterNum < CY_SYS_WDT_COUNTERS_MAX);

        if(0u == CySysWdtReadEnabledStatus(counterNum))
        {
            configRegValue = CY_SYS_WDT_CONFIG_REG &
                                (uint32)~((uint32)(CY_SYS_WDT_MODE_MASK << (counterNum * CY_SYS_WDT_CNT_SHIFT)));
            configRegValue |= (uint32)((mode & CY_SYS_WDT_MODE_MASK) << (counterNum * CY_SYS_WDT_CNT_SHIFT));
            CY_SYS_WDT_CONFIG_REG = configRegValue;
        }
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadMode
    ********************************************************************************
    *
    * Summary:
    *  Reads the mode of one of the three WDT counters.
    *
    * Parameters:
    *  counterNum: Valid range [0-2].  Number of the WDT counter.
    *
    * Return:
    *  Mode of the counter.  Same enumerated values as mode parameter used in
    *  CySysWdtWriteMode().
    *
    *******************************************************************************/
    uint32 CySysWdtReadMode(uint32 counterNum)
    {
        return ((CY_SYS_WDT_CONFIG_REG >> (counterNum * CY_SYS_WDT_CNT_SHIFT)) & CY_SYS_WDT_MODE_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteClearOnMatch
    ********************************************************************************
    *
    * Summary:
    *  Configures the WDT counter clear on a match setting.  If configured to clear
    *  on match, the counter will count from 0 to the MatchValue giving it a
    *  period of (MatchValue + 1).
    *
    * Parameters:
    *  counterNum:
    *   Valid range [0-1].  Number of the WDT counter.  Match values are not
    *   supported by counter 2.
    *
    * enable:
    *   0 to disable, 1 to enable
    *
    * Return:
    *   None
    *
    * Side Effects:
    *   WDT counter counterNum should be disabled. Otherwise this function call will
    *   have no effect. If the specified counter is enabled, call CySysWdtDisable()
    *   function with the corresponding parameter to disable specified counter and
    *   wait for it to stop. This may take up to three LFCLK cycles.
    *
    *******************************************************************************/
    void CySysWdtWriteClearOnMatch(uint32 counterNum, uint32 enable)
    {
        uint32 configRegValue;

        CYASSERT((counterNum == CY_SYS_WDT_COUNTER0) ||
                 (counterNum == CY_SYS_WDT_COUNTER1));

        if(0u == CySysWdtReadEnabledStatus(counterNum))
        {
            configRegValue = CY_SYS_WDT_CONFIG_REG &
                (uint32)~((uint32)((uint32)1u << ((counterNum * CY_SYS_WDT_CNT_SHIFT) + CY_SYS_WDT_CNT_MATCH_CLR_SHIFT)));

            configRegValue
                |= (uint32)(enable << ((counterNum * CY_SYS_WDT_CNT_SHIFT) + CY_SYS_WDT_CNT_MATCH_CLR_SHIFT));

            CY_SYS_WDT_CONFIG_REG = configRegValue;
        }
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadClearOnMatch
    ********************************************************************************
    *
    * Summary:
    *  Reads the clear on match setting for the specified counter.
    *
    * Parameters:
    *   counterNum: Valid range [0-1].  Number of the WDT counter.  Match values are
    *               not supported by counter 2.
    *
    * Return:
    *   Clear on Match status: 1 if enabled, 0 if disabled
    *
    *******************************************************************************/
    uint32 CySysWdtReadClearOnMatch(uint32 counterNum)
    {
        CYASSERT((counterNum == CY_SYS_WDT_COUNTER0) ||
                 (counterNum == CY_SYS_WDT_COUNTER1));

        return (uint32)((CY_SYS_WDT_CONFIG_REG >>
                        ((counterNum * CY_SYS_WDT_CNT_SHIFT) + CY_SYS_WDT_CNT_MATCH_CLR_SHIFT)) & 0x01u);
    }


    /*******************************************************************************
    * Function Name: CySysWdtEnable
    ********************************************************************************
    *
    * Summary:
    *  Enables the specified WDT counters.  All the counters specified in the mask are
    *  enabled.
    *
    * Parameters:
    *  counterMask: Mask of all counters to enable
    *       Define                       Counter
    *      CY_SYS_WDT_COUNTER0_MASK      0
    *      CY_SYS_WDT_COUNTER1_MASK      1
    *      CY_SYS_WDT_COUNTER2_MASK      2
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  Enabling or disabling a WDT requires 3 LFCLK cycles to come into effect.
    *  Therefore, the WDT enable state must not be changed more than once in
    *  that period.
    *
    *  After WDT is enabled, it is illegal to write WDT configuration (WDT_CONFIG)
    *  and control (WDT_CONTROL) registers. That means that all WDT functions that
    *  contain 'write' in the name (with the exception of CySysWdtWriteMatch()
    *  function) are illegal to call once WDT enabled.
    *
    *  PSoC 4100 / PSoC 4200:
    *  This function enables the ILO.
    *
    *  PSoC 4100-BL / PSoC 4200-BL:
    *  The LFLCK should be configured before calling this function. The desired
    *  source should be enabled and configured to be the source for the LFCLK.
    *
    *******************************************************************************/
    void CySysWdtEnable(uint32 counterMask)
    {
        #if (!CY_IP_WCO)
            CySysClkIloStart();
        #endif /* (!CY_IP_WCO) */

        CY_SYS_WDT_CONTROL_REG |= counterMask;

        if(0u != (counterMask & CY_SYS_WDT_COUNTER0_MASK))
        {
            while (0u == CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER0))
            {
                /* Wait for the changes to come into effect */
            }
        }

        if(0u != (counterMask & CY_SYS_WDT_COUNTER1_MASK))
        {
            while (0u == CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER1))
            {
                /* Wait for the changes to come into effect */
            }
        }

        if(0u != (counterMask & CY_SYS_WDT_COUNTER2_MASK))
        {
            while (0u == CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER2))
            {
                /* Wait for the changes to come into effect */
            }
        }
    }


    /*******************************************************************************
    * Function Name: CySysWdtDisable
    ********************************************************************************
    *
    * Summary:
    *  Disables the specified WDT counters. All the counters specified in the mask
    *  are disabled. The function waits for the changes to come into effect.
    *
    * Parameters:
    *  uint32 counterMask: Mask of all counters to disable
    *    Define                       Counter
    *   CY_SYS_WDT_COUNTER0_MASK      0
    *   CY_SYS_WDT_COUNTER1_MASK      1
    *   CY_SYS_WDT_COUNTER2_MASK      2
    *
    * Return:
    *   None
    *
    *******************************************************************************/
    void CySysWdtDisable(uint32 counterMask)
    {
        CY_SYS_WDT_CONTROL_REG &= ~counterMask;

        if(0u != (counterMask & CY_SYS_WDT_COUNTER0_MASK))
        {
            while (0u != CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER0))
            {
                /* Wait for the changes to come into effect */
            }
        }

        if(0u != (counterMask & CY_SYS_WDT_COUNTER1_MASK))
        {
            while (0u != CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER1))
            {
                /* Wait for the changes to come into effect */
            }
        }

        if(0u != (counterMask & CY_SYS_WDT_COUNTER2_MASK))
        {
            while (0u != CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER2))
            {
                /* Wait for the changes to come into effect */
            }
        }
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteCascade
    ********************************************************************************
    *
    * Summary:
    *   Writes the two WDT cascade values based on the combination of mask values
    *   specified.
    *
    * Parameters:
    *   cascadeMask: Mask value used to set or clear the cascade values.
    *   Define                     Cascade
    *   CY_SYS_WDT_CASCADE_NONE    Neither
    *   CY_SYS_WDT_CASCADE_01      Cascade 01
    *   CY_SYS_WDT_CASCADE_12      Cascade 12
    *
    *   To set both cascade modes, two defines should be ORed:
    *   (CY_SYS_WDT_CASCADE_01 | CY_SYS_WDT_CASCADE_12)
    *
    * Return:
    *   None
    *
    * Side effects:
    *   If only one cascade mask is specified, the second cascade is disabled.
    *   To set both cascade modes two defines should be ORed:
    *    (CY_SYS_WDT_CASCADE_01 | CY_SYS_WDT_CASCADE_12).
    *
    *   WDT counters that are part of the specified cascade, should be disabled.
    *   Otherwise this function call will have no effect. If the specified
    *   counter is enabled, call CySysWdtDisable() function with the corresponding
    *   parameter to disable specified counter and wait for it to stop. This may
    *   take up to 3 LFCLK cycles.
    *
    *******************************************************************************/
    void CySysWdtWriteCascade(uint32 cascadeMask)
    {
        uint32 configRegValue;
        uint32 countersEnableStatus;

        countersEnableStatus = CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER0) |
                            CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER1) |
                            CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER2);

        if (0u == countersEnableStatus)
        {
            configRegValue = CY_SYS_WDT_CONFIG_REG;
            configRegValue &= ((uint32)(~(CY_SYS_WDT_CASCADE_01|CY_SYS_WDT_CASCADE_12)));
            configRegValue |= cascadeMask;
            CY_SYS_WDT_CONFIG_REG = configRegValue;
        }
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadCascade
    ********************************************************************************
    *
    * Summary:
    *   Reads the two WDT cascade values returning a mask of the bits set.
    *
    * Parameters:
    *   None
    *
    * Return:
    *   Mask of cascade values set.
    *   Define                     Cascade
    *   CY_SYS_WDT_CASCADE_NONE    Neither
    *   CY_SYS_WDT_CASCADE_01      Cascade 01
    *   CY_SYS_WDT_CASCADE_12      Cascade 12
    *
    *******************************************************************************/
    uint32 CySysWdtReadCascade(void)
    {
        return (CY_SYS_WDT_CONFIG_REG & (CY_SYS_WDT_CASCADE_01 | CY_SYS_WDT_CASCADE_12));
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteMatch
    ********************************************************************************
    *
    * Summary:
    *  Configures the WDT counter match comparison value.
    *
    * Parameters:
    *  counterNum:
    *   Valid range [0-1]. Number of the WDT counter. Match values are not
    *   supported by counter 2.
    *
    *  match:
    *   Valid range [0-65535]. Value to be used to match against the counter.
    *
    * Return:
    *   None
    *
    *******************************************************************************/
    void CySysWdtWriteMatch(uint32 counterNum, uint32 match)
    {
        uint32 regValue;

        CYASSERT((counterNum == CY_SYS_WDT_COUNTER0) ||
                 (counterNum == CY_SYS_WDT_COUNTER1));

        /* Wait for the previous changes to come into effect */
        CyDelayUs(CY_SYS_WDT_3LFCLK_DELAY_US);

        regValue = CY_SYS_WDT_MATCH_REG;
        regValue &= (uint32)~((uint32)(CY_SYS_WDT_LOWER_16BITS_MASK << (counterNum * CY_SYS_WDT_CNT_MATCH_SHIFT)));
        CY_SYS_WDT_MATCH_REG = (regValue | (match << (counterNum * CY_SYS_WDT_CNT_MATCH_SHIFT)));

        /* Make sure the match synchronization started */
        CyDelayUs(CY_SYS_WDT_1LFCLK_DELAY_US);
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteToggleBit
    ********************************************************************************
    *
    * Summary:
    *  Configures which bit in the WDT counter 2 to monitor for a toggle. When that
    *  bit toggles, an interrupt is generated if the mode for counter 2 has
    *  interrupts enabled.
    *
    * Parameters:
    *  bit:
    *   Valid range [0-31].  Counter 2 bit to monitor for a toggle.
    *
    * Return:
    *   None
    *
    * Side effects:
    *   WDT Counter 2 should be disabled. Otherwise this function call will have no
    *   effect.
    *
    *   If the specified counter is enabled, call CySysWdtDisable() function with
    *   the corresponding parameter to disable specified counter and wait for it to
    *   stop. This may take up to 3 LFCLK cycles.
    *
    *******************************************************************************/
    void CySysWdtWriteToggleBit(uint32 bits)
    {
        uint32 configRegValue;

        if (0u == CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER2))
        {
            configRegValue = CY_SYS_WDT_CONFIG_REG;
            configRegValue &= (uint32)(~((uint32)(CY_SYS_WDT_CONFIG_BITS2_MASK << CY_SYS_WDT_CONFIG_BITS2_POS)));
            configRegValue |= ((bits & CY_SYS_WDT_CONFIG_BITS2_MASK) << CY_SYS_WDT_CONFIG_BITS2_POS);
            CY_SYS_WDT_CONFIG_REG = configRegValue;
        }
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadToggleBit
    ********************************************************************************
    *
    * Summary:
    *  Reads which bit in the WDT counter 2 is monitored for a toggle.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The bit that is monitored (range of 0 to 31)
    *
    *******************************************************************************/
    uint32 CySysWdtReadToggleBit(void)
    {
        return ((CY_SYS_WDT_CONFIG_REG >> CY_SYS_WDT_CONFIG_BITS2_POS) & CY_SYS_WDT_CONFIG_BITS2_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadMatch
    ********************************************************************************
    *
    * Summary:
    *  Reads the WDT counter match comparison value.
    *
    * Parameters:
    *  counterNum: Valid range [0-1].  Number of the WDT counter.  Match values
    *               are not supported by counter 2.
    *
    * Return:
    *  16-bit match value.
    *
    *******************************************************************************/
    uint32 CySysWdtReadMatch(uint32 counterNum)
    {
        CYASSERT((counterNum == CY_SYS_WDT_COUNTER0) ||
                 (counterNum == CY_SYS_WDT_COUNTER1));

        return ((uint32)(CY_SYS_WDT_MATCH_REG >> (counterNum * CY_SYS_WDT_CNT_MATCH_SHIFT)) & CY_SYS_WDT_LOWER_16BITS_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadCount
    ********************************************************************************
    *
    * Summary:
    *  Reads the current WDT counter value.
    *
    * Parameters:
    *  counterNum: Valid range [0-2].  Number of the WDT counter.
    *
    * Return:
    *  Live counter value.  Counter 0 and 1 are 16 bit counters and counter
    *  2 is a 32 bit counter.
    *
    *******************************************************************************/
    uint32 CySysWdtReadCount(uint32 counterNum)
    {
        uint32 regValue = 0u;

        switch(counterNum)
        {
            /* WDT Counter 0 */
            case 0u:
                regValue = CY_SYS_WDT_CTRLOW_REG & CY_SYS_WDT_LOWER_16BITS_MASK;
            break;

            /* WDT Counter 1 */
            case 1u:
                regValue = (CY_SYS_WDT_CTRLOW_REG >> CY_SYS_WDT_CNT_MATCH_SHIFT) & CY_SYS_WDT_LOWER_16BITS_MASK;
            break;

            /* WDT Counter 2 */
            case 2u:
                regValue = CY_SYS_WDT_CTRHIGH_REG;
            break;

            default:
                CYASSERT(0u != 0u);
            break;
        }

        return (regValue);
    }


    /*******************************************************************************
    * Function Name: CySysWdtGetInterruptSource
    ********************************************************************************
    *
    * Summary:
    *   Reads a mask containing all the WDT counters interrupts that are currently
    *   set by the hardware, if a corresponding mode is selected.
    *
    * Parameters:
    *   None
    *
    * Return:
    *   Mask of interrupts set.
    *   Define                    Counter
    *   CY_SYS_WDT_COUNTER0_INT    0
    *   CY_SYS_WDT_COUNTER1_INT    1
    *   CY_SYS_WDT_COUNTER2_INT    2
    *
    *******************************************************************************/
    uint32 CySysWdtGetInterruptSource(void)
    {
        return (CY_SYS_WDT_CONTROL_REG & (CY_SYS_WDT_COUNTER0_INT | CY_SYS_WDT_COUNTER1_INT | CY_SYS_WDT_COUNTER2_INT));
    }


    /*******************************************************************************
    * Function Name: CySysWdtClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *   Clears all the WDT counter interrupts set in the mask. Calling this function
    *   also prevents Reset from happening when the counter mode is set to generate
    *   3 interrupts and then reset the device.
    *
    *   All the WDT interrupts are to be cleared by the firmware, otherwise interrupts
    *   will be generated continuously.
    *
    * Parameters:
    *   counterMask: Mask of all counters to enable
    *   Define                    Counter
    *   CY_SYS_WDT_COUNTER0_INT    0
    *   CY_SYS_WDT_COUNTER1_INT    1
    *   CY_SYS_WDT_COUNTER2_INT    2
    *
    * Return:
    *   None
    *
    * Side effects:
    *   This function temporary removes the watchdog lock, if it was set, and
    *   restores the lock state, after cleaning the WDT interrupts, that are set in
    *   a mask.
    *
    *******************************************************************************/
    void CySysWdtClearInterrupt(uint32 counterMask)
    {
        uint8 interruptState;
        uint32 wdtLockState;

        interruptState = CyEnterCriticalSection();

        if (0u != CySysWdtLocked())
        {
            wdtLockState = 1u;
            CySysWdtUnlock();
        }
        else
        {
            wdtLockState = 0u;
        }

        /* Set new WDT control register value */
        counterMask &= (CY_SYS_WDT_COUNTER0_INT |
                        CY_SYS_WDT_COUNTER1_INT |
                        CY_SYS_WDT_COUNTER2_INT);

        CY_SYS_WDT_CONTROL_REG = counterMask | (CY_SYS_WDT_CONTROL_REG & ~(CY_SYS_WDT_COUNTER0_INT |
                                                                           CY_SYS_WDT_COUNTER1_INT |
                                                                           CY_SYS_WDT_COUNTER2_INT));

        if (1u == wdtLockState)
        {
            CySysWdtLock();
        }

        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CySysWdtResetCounters
    ********************************************************************************
    *
    * Summary:
    *  Resets all WDT counters set in the mask.
    *
    * Parameters:
    *  counterMask: Mask of all counters to reset.
    *   Define                       Counter
    *   CY_SYS_WDT_COUNTER0_RESET    0
    *   CY_SYS_WDT_COUNTER1_RESET    1
    *   CY_SYS_WDT_COUNTER2_RESET    2
    *
    * Return:
    *   None
    *
    * Side effects:
    *   This function will not reset counter values if the Watchdog is locked.
    *
    *******************************************************************************/
    void CySysWdtResetCounters(uint32 countersMask)
    {
        /* Set new WDT reset value */
        CY_SYS_WDT_CONTROL_REG |= (countersMask & (CY_SYS_WDT_COUNTER0_RESET |
            CY_SYS_WDT_COUNTER1_RESET | CY_SYS_WDT_COUNTER2_RESET));
    }


    /*******************************************************************************
    * Function Name: CySysClkLfclkPosedgeCatch
    ********************************************************************************
    *
    * Summary:
    *  Returns once LFCLK positive edge occurred.
    *
    *  The CySysClkLfclkPosedgeRestore() should be called after this function in
    *  order to restore WDT configuration.
    *
    *  The pair of the CySysClkLfclkPosedgeCatch() and CySysClkLfclkPosedgeRestore()
    *  functions expected to be called inside of a critical section.
    *
    *  To ensure that the WDT counter value is read until it changes. The enabled
    *  WDT counter is used for that purpose. If no counters are enabled, the counter
    *  0 is enabled. And after LFCLK source is switched, the counter 0 configuration
    *  is restored back.
    *
    *  Not applicable for the PSoC 4000 devices.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function will have no effect if the WDT is locked (CySysWdtLock() is
    *  called). Call CySysWdtUnlock() to unlock the WDT.
    *
    *******************************************************************************/
    static void CySysClkLfclkPosedgeCatch(void)
    {
        uint32 firstCount;

        if (0u != CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER0))
        {
            lfclkPosedgeEnabledWdtCounter = CY_SYS_WDT_COUNTER0;
        }
        else if (0u != CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER1))
        {
            lfclkPosedgeEnabledWdtCounter = CY_SYS_WDT_COUNTER1;
        }
        else if (0u != CySysWdtReadEnabledStatus(CY_SYS_WDT_COUNTER2))
        {
            lfclkPosedgeEnabledWdtCounter = CY_SYS_WDT_COUNTER2;
        }
        else /* All WDT counters are disabled */
        {
            /* Configure WDT counter # 0 */
            lfclkPosedgeWdtCounter0Enabled = 1u;
            lfclkPosedgeEnabledWdtCounter = CY_SYS_WDT_COUNTER0;

            lfclkPosedgeWdtCounter0Mode = CySysWdtReadMode(CY_SYS_WDT_COUNTER0);
            CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_NONE);
            CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
        }

        firstCount = CySysWdtReadCount(lfclkPosedgeEnabledWdtCounter);
        while (CySysWdtReadCount(lfclkPosedgeEnabledWdtCounter) == firstCount)
        {
            /* Wait for counter to increment */
        }
    }


    /*******************************************************************************
    * Function Name: CySysClkLfclkPosedgeRestore
    ********************************************************************************
    *
    * Summary:
    *  Restores WDT configuration after CySysClkLfclkPosedgeCatch() call.
    *
    *  The pair of the CySysClkLfclkPosedgeCatch() and CySysClkLfclkPosedgeRestore()
    *  functions expected to be called inside of a critical section.
    *
    *  Not applicable for the PSoC 4000 devices.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function will have no effect if the WDT is locked (CySysWdtLock() is
    *  called). Call CySysWdtUnlock() to unlock the WDT.
    *
    *******************************************************************************/
    static void CySysClkLfclkPosedgeRestore(void)
    {
        if (lfclkPosedgeWdtCounter0Enabled != 0u)
        {
            /* Restore counter # 0 configuration and force it's shutdown */
            CY_SYS_WDT_CONTROL_REG &= (uint32)(~CY_SYS_WDT_COUNTER0_MASK);
            CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, lfclkPosedgeWdtCounter0Mode);
            lfclkPosedgeWdtCounter0Enabled = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: CySysWdtReadEnabledStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the enabled status WDT counter.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Status of WDT counter:
    *   0 - counter is disabled
    *   1 - counter is enabled
    *
    *******************************************************************************/
    uint32 CySysWdtReadEnabledStatus(void)
    {
        return ((CY_SYS_WDT_DISABLE_KEY_REG == CY_SYS_WDT_KEY) ? (uint32) 0u : (uint32) 1u);
    }


    /*******************************************************************************
    * Function Name: CySysWdtEnable
    ********************************************************************************
    *
    * Summary:
    *  Enables the watchdog timer reset generation. The CySysWdtClearInterrupt()
    *  feeds the watch dog. Two unserviced interrupts will lead to a system reset
    *  (i.e. at the third match).
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The ILO is enabled by hardware once WDT is started.
    *
    *******************************************************************************/
    void CySysWdtEnable(void)
    {
        CY_SYS_WDT_DISABLE_KEY_REG = 0u;
    }


    /*******************************************************************************
    * Function Name: CySysWdtDisable
    ********************************************************************************
    *
    * Summary:
    *  Disables the WDT reset generation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysWdtDisable(void)
    {
        CY_SYS_WDT_DISABLE_KEY_REG = CY_SYS_WDT_KEY;
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteMatch
    ********************************************************************************
    *
    * Summary:
    *  Configures the WDT counter match comparison value.
    *
    * Parameters:
    *  match:
    *   Valid range [0-65535]. Value to be used to match against the counter.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysWdtWriteMatch(uint32 match)
    {
        CY_SYS_WDT_MATCH_REG = (CY_SYS_WDT_MATCH_REG & (uint32)(~ (uint32)CY_SYS_WDT_MATCH_MASK)) | match;
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadMatch
    ********************************************************************************
    *
    * Summary:
    *  Reads the WDT counter match comparison value.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Counter match value.
    *
    *******************************************************************************/
    uint32 CySysWdtReadMatch(void)
    {
        return (CY_SYS_WDT_MATCH_REG & CY_SYS_WDT_MATCH_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadCount
    ********************************************************************************
    *
    * Summary:
    *  Reads the current WDT counter value.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Live counter value
    *
    *******************************************************************************/
    uint32 CySysWdtReadCount(void)
    {
        return ((uint32)CY_SYS_WDT_COUNTER_REG);
    }


    /*******************************************************************************
    * Function Name: CySysWdtWriteIgnoreBits
    ********************************************************************************
    *
    * Summary:
    *  Configures the number of the MSB bits of the watchdog timer that are not
    *  checked against the match.
    *
    * Parameters:
    *  bitsNum:
    *   Valid range [0-15]. Number of MSB bits.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The value of bitsNum provides control over the time-to-reset of the watchdog
    *  (which happens after 3 successive matches).
    *
    *******************************************************************************/
    void CySysWdtWriteIgnoreBits(uint32 bitsNum)
    {
        bitsNum = ((uint32)(bitsNum << CY_SYS_WDT_IGNORE_BITS_SHIFT) & CY_SYS_WDT_IGNORE_BITS_MASK);
        CY_SYS_WDT_MATCH_REG = (CY_SYS_WDT_MATCH_REG & (uint32)(~CY_SYS_WDT_IGNORE_BITS_MASK)) | bitsNum;
    }


    /*******************************************************************************
    * Function Name: CySysWdtReadIgnoreBits
    ********************************************************************************
    *
    * Summary:
    *  Reads the number of the  MSB bits of the watchdog timer that are not
    *  checked against the match.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of MSB bits.
    *
    *******************************************************************************/
    uint32 CySysWdtReadIgnoreBits(void)
    {
        return((uint32)((CY_SYS_WDT_MATCH_REG & CY_SYS_WDT_IGNORE_BITS_MASK) >> CY_SYS_WDT_IGNORE_BITS_SHIFT));
    }


    /*******************************************************************************
    * Function Name: CySysWdtClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Feeds the watch dog. Cleans the WDT match flag, which is set every time the
    *  WDT counter reaches a WDT match value.  Two unserviced interrupts will lead
    *  to a system reset (i.e. at the third match).
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysWdtClearInterrupt(void)
    {
        CY_SYS_SRSS_INTR_REG |= CY_SYS_WDT_LOWER_BIT_MASK;
    }


    /*******************************************************************************
    * Function Name: CySysWdtMaskInterrupt
    ********************************************************************************
    *
    * Summary:
    *  After masking interrupts from the WDT, they are not passed to the CPU.
    *  This function does not disable the WDT reset generation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysWdtMaskInterrupt(void)
    {
        CY_SYS_SRSS_INTR_MASK_REG &= (uint32)(~ (uint32)CY_SYS_WDT_LOWER_BIT_MASK);
    }


    /*******************************************************************************
    * Function Name: CySysWdtUnmaskInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Unmasks WDT interrupts.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysWdtUnmaskInterrupt(void)
    {
        CY_SYS_SRSS_INTR_MASK_REG |= CY_SYS_WDT_LOWER_BIT_MASK;
    }

#endif /* (CY_IP_SRSSV2) */


#if(CY_IP_SRSSV2)

    /*******************************************************************************
    * Function Name: CySysLvdEnable
    ********************************************************************************
    *
    * Summary:
    *  Enables the output of the low-voltage monitor when Vddd is at or below the
    *  trip point, configures the device to generate an interrupt, and sets the voltage
    *  trip level.
    *
    * Parameters:
    *  threshold: Threshold selection for Low Voltage Detect circuit.
    *  Threshold variation is +/- 2.5% from these typical voltage choices.
    *         Define                     Voltage threshold
    *    CY_LVD_THRESHOLD_1_75_V       1.7500 V
    *    CY_LVD_THRESHOLD_1_80_V       1.8000 V
    *    CY_LVD_THRESHOLD_1_90_V       1.9000 V
    *    CY_LVD_THRESHOLD_2_00_V       2.0000 V
    *    CY_LVD_THRESHOLD_2_10_V       2.1000 V
    *    CY_LVD_THRESHOLD_2_20_V       2.2000 V
    *    CY_LVD_THRESHOLD_2_30_V       2.3000 V
    *    CY_LVD_THRESHOLD_2_40_V       2.4000 V
    *    CY_LVD_THRESHOLD_2_50_V       2.5000 V
    *    CY_LVD_THRESHOLD_2_60_V       2.6000 V
    *    CY_LVD_THRESHOLD_2_70_V       2.7000 V
    *    CY_LVD_THRESHOLD_2_80_V       2.8000 V
    *    CY_LVD_THRESHOLD_2_90_V       2.9000 V
    *    CY_LVD_THRESHOLD_3_00_V       3.0000 V
    *    CY_LVD_THRESHOLD_3_20_V       3.2000 V
    *    CY_LVD_THRESHOLD_4_50_V       4.5000 V
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysLvdEnable(uint32 threshold)
    {
        /* Set threshold */
        CY_LVD_PWR_VMON_CONFIG_REG = (CY_LVD_PWR_VMON_CONFIG_REG & ~CY_LVD_PWR_VMON_CONFIG_LVD_SEL_MASK) |
                ((threshold << CY_LVD_PWR_VMON_CONFIG_LVD_SEL_SHIFT) & CY_LVD_PWR_VMON_CONFIG_LVD_SEL_MASK);

        CY_LVD_PWR_VMON_CONFIG_REG |= CY_LVD_PWR_VMON_CONFIG_LVD_EN;
        CY_LVD_PWR_INTR_MASK_REG   |= CY_LVD_PROPAGATE_INT_TO_CPU;
    }


    /*******************************************************************************
    * Function Name: CySysLvdDisable
    ********************************************************************************
    *
    * Summary:
    *  Disables the low voltage detection. A low voltage interrupt is disabled.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysLvdDisable(void)
    {
        CY_LVD_PWR_INTR_MASK_REG   &= ~CY_LVD_PROPAGATE_INT_TO_CPU;
        CY_LVD_PWR_VMON_CONFIG_REG &= ~CY_LVD_PWR_VMON_CONFIG_LVD_EN;
    }


    /*******************************************************************************
    * Function Name: CySysLvdGetInterruptSource
    ********************************************************************************
    *
    * Summary:
    *  Gets the low voltage detection interrupt status (without clearing).
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Interrupt request value:
    *    CY_SYS_LVD_INT - Indicates an Low Voltage Detect interrupt
    *
    *******************************************************************************/
    uint32 CySysLvdGetInterruptSource(void)
    {
        return (CY_LVD_PWR_INTR_REG & CY_SYS_LVD_INT);
    }


    /*******************************************************************************
    * Function Name: CySysLvdClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears the low voltage detection interrupt status.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysLvdClearInterrupt(void)
    {
        CY_LVD_PWR_INTR_REG = CY_SYS_LVD_INT;
    }

#endif /* (CY_IP_SRSSV2) */


/*******************************************************************************
* Function Name: CySysGetResetReason
********************************************************************************
*
* Summary:
*  Reports the cause for the latest reset(s) that occurred in the system. All the
*  bits in the RES_CAUSE register assert when the corresponding reset cause
*  occurs and must be cleared by the firmware. These bits are cleared by the hardware
*  only during XRES, POR, or a detected brown-out.
*
* Parameters:
*  reason: bits in the RES_CAUSE register to clear.
*   CY_SYS_RESET_WDT       - WDT caused a reset
*   CY_SYS_RESET_PROTFAULT - A protection violation occurred that requires a RESET
*   CY_SYS_RESET_SW        - Cortex-M0 requested a system reset.
*
* Return:
*  Status. Same enumerated bit values as used for the reason parameter.
*
*******************************************************************************/
uint32 CySysGetResetReason(uint32 reason)
{
    uint32 status;

    reason &= (CY_SYS_RESET_WDT | CY_SYS_RESET_PROTFAULT | CY_SYS_RESET_SW);
    status = CY_SYS_RES_CAUSE_REG &
             (CY_SYS_RESET_WDT | CY_SYS_RESET_PROTFAULT | CY_SYS_RESET_SW);
    CY_SYS_RES_CAUSE_REG = reason;

    return (status);
}


/*******************************************************************************
* Function Name: CyDisableInts
********************************************************************************
*
* Summary:
*  Disables all interrupts.
*
* Parameters:
*  None
*
* Return:
*  32 bit mask of previously enabled interrupts.
*
*******************************************************************************/
uint32 CyDisableInts(void)
{
    uint32 intState;

    /* Get current interrupt state. */
    intState = CY_INT_CLEAR_REG;

    /* Disable all interrupts. */
    CY_INT_CLEAR_REG = CY_INT_CLEAR_DISABLE_ALL;

    return (intState);
}


/*******************************************************************************
* Function Name: CyEnableInts
********************************************************************************
*
* Summary:
*  Enables interrupts to a given state.
*
* Parameters:
*   mask, 32 bit mask of interrupts to enable.
*
* Return:
*  None
*
*******************************************************************************/
void CyEnableInts(uint32 mask)
{
    CY_INT_ENABLE_REG = mask;
}


/*******************************************************************************
* Function Name: CyIntSetSysVector
********************************************************************************
*
* Summary:
*  Sets the interrupt vector of the specified system interrupt number. These
*  interrupts are for SysTick, PendSV and others.
*
* Parameters:
*  number: System interrupt number:
*    CY_INT_NMI_IRQN        - Non Maskable Interrupt
*    CY_INT_HARD_FAULT_IRQN - Hard Fault Interrupt
*    CY_INT_SVCALL_IRQN     - SV Call Interrupt
*    CY_INT_PEND_SV_IRQN    - Pend SV Interrupt
*    CY_INT_SYSTICK_IRQN    - System Tick Interrupt
*
*  address: Pointer to an interrupt service routine.
*
* Return:
*  The old ISR vector at this location.
*
*******************************************************************************/
cyisraddress CyIntSetSysVector(uint8 number, cyisraddress address)
{
    cyisraddress oldIsr;
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_INT_IRQ_BASE);

    /* Save old Interrupt service routine. */
    oldIsr = ramVectorTable[number];

    /* Set new Interrupt service routine. */
    ramVectorTable[number] = address;

    return(oldIsr);
}


/*******************************************************************************
* Function Name: CyIntGetSysVector
********************************************************************************
*
* Summary:
*   Gets the interrupt vector of the specified system interrupt number. These
*   interrupts are for SysTick, PendSV and others.
*
* Parameters:
*  number: System interrupt number:
*    CY_INT_NMI_IRQN        - Non Maskable Interrupt
*    CY_INT_HARD_FAULT_IRQN - Hard Fault Interrupt
*    CY_INT_SVCALL_IRQN     - SV Call Interrupt
*    CY_INT_PEND_SV_IRQN    - Pend SV Interrupt
*    CY_INT_SYSTICK_IRQN    - System Tick Interrupt
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress CyIntGetSysVector(uint8 number)
{
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_INT_IRQ_BASE);

    return(ramVectorTable[number]);
}


/*******************************************************************************
* Function Name: CyIntSetVector
********************************************************************************
*
* Summary:
*  Sets the interrupt vector of the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number
*  address: Pointer to an interrupt service routine
*
* Return:
*   Previous interrupt vector value.
*
*******************************************************************************/
cyisraddress CyIntSetVector(uint8 number, cyisraddress address)
{
    cyisraddress oldIsr;
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_NUM_INTERRUPTS);

    /* Save old Interrupt service routine. */
    oldIsr = ramVectorTable[CY_INT_IRQ_BASE + number];

    /* Set new Interrupt service routine. */
    ramVectorTable[CY_INT_IRQ_BASE + number] = address;

    return(oldIsr);
}


/*******************************************************************************
* Function Name: CyIntGetVector
********************************************************************************
*
* Summary:
*  Gets the interrupt vector of the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number
*
* Return:
*  Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress CyIntGetVector(uint8 number)
{
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_NUM_INTERRUPTS);

    return (ramVectorTable[CY_INT_IRQ_BASE + number]);
}

/*******************************************************************************
* Function Name: CyIntSetPriority
********************************************************************************
*
* Summary:
*  Sets the priority of the interrupt.
*
* Parameters:
*  priority: Priority of the interrupt. 0 - 3, 0 being the highest.
*  number: The number of the interrupt, 0 - 31.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntSetPriority(uint8 number, uint8 priority)
{
    uint8 interruptState;
    uint32 shift;
    uint32 value;

    CYASSERT(priority <= CY_MIN_PRIORITY);
    CYASSERT(number < CY_NUM_INTERRUPTS);

    shift = CY_INT_PRIORITY_SHIFT(number);

    interruptState = CyEnterCriticalSection();

    value = CY_INT_PRIORITY_REG(number);
    value &= (uint32)(~((uint32)(CY_INT_PRIORITY_MASK << shift)));
    value |= ((uint32)priority << shift);
    CY_INT_PRIORITY_REG(number) = value;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyIntGetPriority
********************************************************************************
*
* Summary:
*  Gets the priority of the interrupt.
*
* Parameters:
*  number: The number of the interrupt, 0 - 31.
*
* Return:
*  Priority of the interrupt. 0 - 3, 0 being the highest.
*
*******************************************************************************/
uint8 CyIntGetPriority(uint8 number)
{
    uint8 priority;

    CYASSERT(number < CY_NUM_INTERRUPTS);

    priority = (uint8) (CY_INT_PRIORITY_REG(number) >> CY_INT_PRIORITY_SHIFT(number));

    return (priority & (uint8) CY_INT_PRIORITY_MASK);
}


/*******************************************************************************
* Function Name: CyIntEnable
********************************************************************************
*
* Summary:
*  Enables the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number
*
* Return:
*  None
*
*******************************************************************************/
void CyIntEnable(uint8 number)
{
    CY_INT_ENABLE_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}


/*******************************************************************************
* Function Name: CyIntGetState
********************************************************************************
*
* Summary:
*  Gets the enable state of the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number.
*
* Return:
*  Enable status: 1 if enabled, 0 if disabled
*
*******************************************************************************/
uint8 CyIntGetState(uint8 number)
{
    /* Get state of interrupt. */
    return ((0u != (CY_INT_ENABLE_REG & ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number)))) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: CyIntDisable
********************************************************************************
*
* Summary:
*   Disables the specified interrupt number.
*
* Parameters:
*   number: Valid range [0-31].  Interrupt number.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntDisable(uint8 number)
{
    CY_INT_CLEAR_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}

/*******************************************************************************
* Function Name: CyIntSetPending
********************************************************************************
*
* Summary:
*  Forces the specified interrupt number to be pending.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntSetPending(uint8 number)
{
    CY_INT_SET_PEND_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}


/*******************************************************************************
* Function Name: CyIntClearPending
********************************************************************************
*
* Summary:
*  Clears any pending interrupt for the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntClearPending(uint8 number)
{
    CY_INT_CLR_PEND_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}


/*******************************************************************************
* Function Name: CyHalt
********************************************************************************
*
* Summary:
*  Halts the CPU.
*
* Parameters:
*  reason: Value to be used during debugging.
*
* Return:
*  None
*
*******************************************************************************/
void CyHalt(uint8 reason)
{
    if(0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }

    #if defined (__ARMCC_VERSION)
        __breakpoint(0x0);
    #elif defined(__GNUC__) || defined (__ICCARM__)
        __asm("    bkpt    1");
    #elif defined(__C51__)
        CYDEV_HALT_CPU;
    #endif  /* (__ARMCC_VERSION) */
}


/*******************************************************************************
* Function Name: CySoftwareReset
********************************************************************************
*
* Summary:
*  Forces a software reset of the device.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySoftwareReset(void)
{
    /***************************************************************************
    * Setting the system reset request bit. The vector key value must be written
    * to the register, otherwise the register write is unpredictable.
    ***************************************************************************/
    CY_SYS_CM0_AIRCR_REG = (CY_SYS_CM0_AIRCR_REG & (uint32)(~CY_SYS_CM0_AIRCR_VECTKEY_MASK)) |
                            CY_SYS_CM0_AIRCR_VECTKEY | CY_SYS_CM0_AIRCR_SYSRESETREQ;
}


/*******************************************************************************
* Function Name: CyDelay
********************************************************************************
*
* Summary:
*  Blocks for milliseconds.
*
* Parameters:
*  milliseconds: number of milliseconds to delay.
*
* Return:
*  None
*
*******************************************************************************/
void CyDelay(uint32 milliseconds)
{
    while (milliseconds > CY_DELAY_MS_OVERFLOW)
    {
        /* This loop prevents overflow.
         * At 100MHz, milliseconds * cydelayFreqKhz overflows at about 42 seconds
         */
        CyDelayCycles(cydelay32kMs);
        milliseconds -= CY_DELAY_MS_OVERFLOW;
    }

    CyDelayCycles(milliseconds * cydelayFreqKhz);
}


/*******************************************************************************
* Function Name: CyDelayUs
********************************************************************************
* Summary:
*  Blocks for microseconds.
*
* Parameters:
*  microseconds: number of microseconds to delay.
*
* Return:
*  None
*
*******************************************************************************/
void CyDelayUs(uint16 microseconds)
{
    CyDelayCycles((uint32)microseconds * cydelayFreqMhz);
}


/*******************************************************************************
* Function Name: CyDelayFreq
********************************************************************************
* Summary:
*  Sets clock frequency for CyDelay.
*
* Parameters:
*  freq: Frequency of bus clock in Hertz.
*
* Return:
*  None
*
*******************************************************************************/
void CyDelayFreq(uint32 freq)
{
    if (freq != 0u)
    {
        cydelayFreqHz = freq;
    }
    else
    {
        cydelayFreqHz = CYDEV_BCLK__SYSCLK__HZ;
    }

    cydelayFreqMhz = (uint8)((cydelayFreqHz + CY_DELAY_1M_MINUS_1_THRESHOLD) / CY_DELAY_1M_THRESHOLD);
    cydelayFreqKhz = (cydelayFreqHz + CY_DELAY_1K_MINUS_1_THRESHOLD) / CY_DELAY_1K_THRESHOLD;
    cydelay32kMs   = CY_DELAY_MS_OVERFLOW * cydelayFreqKhz;
}


/*******************************************************************************
* Function Name: CySysTick_Start
********************************************************************************
*
* Summary:
*  Starts the system timer (SysTick): configures SysTick to generate interrupt
*  every 1 ms and enables the interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set.
*
*******************************************************************************/
void CySysTickStart(void)
{
    if (0u == CySysTickInitVar)
    {
        CySysTickInit();
        CySysTickInitVar = 1u;
    }

    CySysTickEnable();
}


/*******************************************************************************
* Function Name: CySysTickInit
********************************************************************************
*
* Summary:
*  Sets CySysTickServiceCallbacks() as an ISR for the SysTick exception.
*  The clock source for the SysTick counter is set to the System Clock.
*  The SysTick reload value is configured for the timer to generate interrupt
*  every 1 ms. The System clock value is on the global variables cydelayFreqHz.
*  User is responsible for calling CyDelayFreq() function.
*  Initializes CySysTickCallbacks array.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickInit(void)
{
    uint32 i;

    for (i = 0u; i<CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
    {
        CySysTickCallbacks[i] = (void *) 0;
    }

    (void) CyIntSetSysVector(CY_INT_SYSTICK_IRQN, &CySysTickServiceCallbacks);

    #if(CY_PSOC4_4100BL || CY_PSOC4_4200BL || CY_PSOC5)
        CySysTickSetClockSource(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK);
    #endif /* (CY_PSOC4_4100BL || CY_PSOC4_4200BL || CY_PSOC5) */

    CySysTickSetReload(cydelayFreqHz/1000u);
    CySysTickClear();
    CyIntEnable(CY_INT_SYSTICK_IRQN);
}


/*******************************************************************************
* Function Name: CySysTickEnable
********************************************************************************
*
* Summary:
*  Enables the SysTick timer and its interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickEnable(void)
{
    CySysTickEnableInterrupt();
    CY_SYS_SYST_CSR_REG |= CY_SYS_SYST_CSR_ENABLE;
}


/*******************************************************************************
* Function Name: CySysTickStop
********************************************************************************
*
* Summary:
*  Stops the system timer (SysTick).
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickStop(void)
{
    CY_SYS_SYST_CSR_REG &= ((uint32) ~(CY_SYS_SYST_CSR_ENABLE));
}


/*******************************************************************************
* Function Name: CySysTickEnableInterrupt
********************************************************************************
*
* Summary:
*  Enables the SysTick interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickEnableInterrupt(void)
{
    CY_SYS_SYST_CSR_REG |= CY_SYS_SYST_CSR_ENABLE_INT;
}


/*******************************************************************************
* Function Name: CySysTickDisableInterrupt
********************************************************************************
*
* Summary:
*  Disables the SysTick interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickDisableInterrupt(void)
{
    CY_SYS_SYST_CSR_REG &= ((uint32) ~(CY_SYS_SYST_CSR_ENABLE_INT));
}


/*******************************************************************************
* Function Name: CySysTickSetReload
********************************************************************************
*
* Summary:
*  Sets value the counter is set to on startup and after it reaches zero. This
*  function do not change or reset current sysTick counter value, so it should
*  be cleared using CySysTickClear() API.
*
* Parameters:
*  value: Valid range [0x0-0x00FFFFFF]. Counter reset value
*
* Return:
*  None
*
*******************************************************************************/
void CySysTickSetReload(uint32 value)
{
    CY_SYS_SYST_RVR_REG = (value & CY_SYS_SYST_RVR_CNT_MASK);
}


/*******************************************************************************
* Function Name: CySysTickGetReload
********************************************************************************
*
* Summary:
*  Sets value the counter is set to on startup and after it reaches zero.
*
* Parameters:
*  None
*
* Return:
*  Counter reset value.
*
*******************************************************************************/
uint32 CySysTickGetReload(void)
{
    return(CY_SYS_SYST_RVR_REG & CY_SYS_SYST_RVR_CNT_MASK);
}


/*******************************************************************************
* Function Name: CySysTickGetValue
********************************************************************************
*
* Summary:
*  Gets current SysTick counter value.
*
* Parameters:
*  None
*
* Return:
*  Current SysTick counter value
*
*******************************************************************************/
uint32 CySysTickGetValue(void)
{
    return(CY_SYS_SYST_RVR_REG & CY_SYS_SYST_CVR_REG);
}


#if(CY_PSOC4_4100BL || CY_PSOC4_4200BL || CY_PSOC5)
    /*******************************************************************************
    * Function Name: CySysTickSetClockSource
    ********************************************************************************
    *
    * Summary:
    *  Sets the clock source for the SysTick counter.
    *
    * Parameters:
    *  clockSource: Clock source for SysTick counter
    *         Define                     Clock Source
    *   CY_SYS_SYST_CSR_CLK_SRC_SYSCLK     SysTick is clocked by CPU clock.
    *   CY_SYS_SYST_CSR_CLK_SRC_LFCLK      SysTick is clocked by the low frequency clock
    *                                 (ILO 100 KHz for PSoC 5LP, LFCLK for PSoC 4).
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  Clears SysTick count flag if it was set. If clock source is not ready this
    *  function call will have no effect. After changing clock source to the low frequency
    *  clock the counter and reload register values will remain unchanged so time to
    *  the interrupt will be significantly bigger and vice versa.
    *
    *******************************************************************************/
    void CySysTickSetClockSource(uint32 clockSource)
    {
        if (clockSource == CY_SYS_SYST_CSR_CLK_SRC_SYSCLK)
        {
            CY_SYS_SYST_CSR_REG |= (uint32)(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK << CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT);
        }
        else
        {
            CY_SYS_SYST_CSR_REG &= ((uint32) ~((uint32)(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK << CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT)));
        }
    }
#endif /* (CY_PSOC4_4100BL || CY_PSOC4_4200BL || CY_PSOC5) */


/*******************************************************************************
* Function Name: CySysTickGetCountFlag
********************************************************************************
*
* Summary:
*  The count flag is set once SysTick counter reaches zero.
*   The flag cleared on read.
*
* Parameters:
*  None
*
* Return:
*  Returns non-zero value is counter is set, otherwise zero is returned.
*
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
uint32 CySysTickGetCountFlag(void)
{
    return ((CY_SYS_SYST_CSR_REG>>CY_SYS_SYST_CSR_COUNTFLAG_SHIFT) & 0x01u);
}


/*******************************************************************************
* Function Name: CySysTickClear
********************************************************************************
*
* Summary:
*  Clears the SysTick counter for well-defined startup.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysTickClear(void)
{
    CY_SYS_SYST_CVR_REG = 0u;
}


/*******************************************************************************
* Function Name: CySysTickSetCallback
********************************************************************************
*
* Summary:
*  The function set the pointers to the functions that will be called on
*  SysTick interrupt.
*
* Parameters:
*  number:  The number of callback function address to be set.
*           The valid range is from 0 to 4.
*  CallbackFunction: Function address.
*
* Return:
*  Returns the address of the previous callback function.
*  The NULL is returned if the specified address in not set.
*
*******************************************************************************/
cySysTickCallback CySysTickSetCallback(uint32 number, cySysTickCallback function)
{
    cySysTickCallback retVal;

    retVal = CySysTickCallbacks[number];
    CySysTickCallbacks[number] = function;
    return (retVal);
}


/*******************************************************************************
* Function Name: CySysTickGetCallback
********************************************************************************
*
* Summary:
*  The function get the specified callback pointer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
cySysTickCallback CySysTickGetCallback(uint32 number)
{
    return ((cySysTickCallback) CySysTickCallbacks[number]);
}


/*******************************************************************************
* Function Name: CySysTickServiceCallbacks
********************************************************************************
*
* Summary:
*  System Tick timer interrupt routine
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void CySysTickServiceCallbacks(void)
{
    uint32 i;

    /* Verify that tick timer flag was set */
    if (1u == CySysTickGetCountFlag())
    {
        for (i=0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
        {
            if (CySysTickCallbacks[i] != (void *) 0)
            {
                (void)(CySysTickCallbacks[i])();
            }
        }
    }
}

/* [] END OF FILE */
