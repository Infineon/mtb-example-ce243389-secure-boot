/******************************************************************************
* File Name:   secure_boot_config.c
*
* Description: This is the source code for Secure boot configuration of CM4 core
*              for ModusToolbox.
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
#include "cy_si_config.h"
#include "cy_si_keystorage.h"

/*******************************************************************************
* Macros
********************************************************************************/
/* App version */
#define SI_VERSION_MAJOR    (0UL) /* Major version */
#define SI_VERSION_MINOR    (0UL) /* Minor version */

/* The start address of secure image */
#define SI_ADDR             ((uint32_t)__si_addr_linker_symbol)

/* Secure image size */
#define SI_SIZE             ((uint32_t)__si_size_linker_symbol)

/* Vector table offset */
#define SI_VECTOR_OFFSET    ((uint32_t)__si_vector_offset_linker_symbol)

/* CM0+ VT Offset */
#define SI_VT_OFFSET        (SI_VECTOR_OFFSET) - offsetof(cy_stc_si_appheader_t, core0Vt)

/* CM4 ARM CPUID[15:4] Reg shifted to [31:20] */
#define SI_CPUID            (0xC2400000UL)

/* Index ID of the CM4 core */
#define SI_CORE_IDX         (0UL)

/*******************************************************************************
* Global Variables
********************************************************************************/
/* Location info provided from linker */
extern const char __si_addr_linker_symbol[];
extern const char __si_size_linker_symbol[];
extern const char __si_vector_offset_linker_symbol[];

/*******************************************************************************
* Application header of CM4 APP in Code Flash
********************************************************************************/
/* Secure Application header for the CodeFlash application */
CY_SECTION(".cy_app_header") __USED static const cy_stc_si_appheader_t cy_si_appHeader = 
{
    .objSize        = SI_SIZE,
    .appId          = (SI_APP_VERSION(SI_VERSION_MAJOR, SI_VERSION_MINOR) | CY_SI_APP_ID_SECUREIMG),
    .appAttributes  = 0UL,                      /* Reserved */
    .numCores       = 1UL,                      /* Only CM4 */
    .core0Vt        = SI_VT_OFFSET,             /* CM4 VT offset */
    .core0Id        = SI_CPUID | SI_CORE_IDX,   /* CM4 core ID */
};

/*******************************************************************************
* Application digital signature of CM4 APP in Code Flash
********************************************************************************/
/* Secure Image Digital signature (Populated by cymcuelftool) */
CY_SECTION(".cy_app_signature") __USED CY_ALIGN(4) 
static const uint8_t cy_si_appSignature[CY_SI_SECURE_DIGSIG_SIZE] = {0u};

/*******************************************************************************
* Function Prototypes
********************************************************************************/

/* [] END OF FILE */
