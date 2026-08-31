/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for CM0+ in the Secure boot
*              Application for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2026-YEAR Infineon Technologies AG $
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "mtb_hal.h"
#include "cybsp.h"
#include "cy_si_config.h"

/*******************************************************************************
* Macros
********************************************************************************/


/*******************************************************************************
* Global Variables
********************************************************************************/
/* CM4 application address: location info provided from linker */
extern const char __si_cm4_app_address[];

/*******************************************************************************
* Function Prototypes
********************************************************************************/
/* CM4 application verification */
static bool verify_cm4_app(void);


/*******************************************************************************
* Function Definitions
********************************************************************************/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM0+ CPU.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    bool verify_cm4_is_failed = false;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Verify CM4 application*/
    if (verify_cm4_app())
    {
        verify_cm4_is_failed = false;
        
        /* Enable CM4. CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
        Cy_SysEnableCM4((uint32_t)(__si_cm4_app_address) + 1024);

        /* Verification successful: turn off the LED2 */
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
    }
    else
    {
        verify_cm4_is_failed = true;
    }

    /* Enable global interrupts */
    __enable_irq();

    for (;;)
    {
        if(verify_cm4_is_failed)
        {
            /* Quickly blink LED2 to indicate that verification failed */
            Cy_GPIO_Inv(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
            Cy_SysLib_Delay(100);
        }
        else
        {
            Cy_SysPm_CpuEnterDeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
        }
    }
}

/*******************************************************************************
*   Function Name: verify_cm4_app
********************************************************************************
* Summary:
* The function is used to verify the CM4 application using the secure signature.
*
* Parameters:
* void
* 
* Return:
* bool - true if verification is successful, false otherwise.
*
*******************************************************************************/
static bool verify_cm4_app(void)
{
    uint32_t topAddress = (uint32_t)(__si_cm4_app_address);
    uint32_t objectSize = *(uint32_t *)topAddress;
    uint32_t sigAddress = topAddress + objectSize;

    /* Check if the object size exceeds the combined size of large and small secure boot manager regions */
    if(objectSize > (CY_FLASH_LG_SBM_SIZE + CY_FLASH_SM_SBM_SIZE))
    {
        return false;
    }
    /* Verify the application */
    if(1 == Cy_FB_VerifyApplication(topAddress, objectSize, sigAddress, 
                                    (cy_stc_crypto_rsa_pub_key_t*)CY_SI_PUBLIC_KEY_ADDR))
    {
        /* Verification successful */
        return true;
    }
    else
    {
        /* Verification failed */
        return false;
    }
}

/* [] END OF FILE */
