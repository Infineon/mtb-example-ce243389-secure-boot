/******************************************************************************
* File Name:   cy_si_config.h
*
* Description: Definitions and function prototypes for Secure Image.
*
* Related Document: See README.md
*
*
*******************************************************************************
* $ Copyright 2026-YEAR Infineon Technologies AG $
*******************************************************************************/

#ifndef CY_SI_CONFIG_H
#define CY_SI_CONFIG_H

/*******************************************************************************
* Header Files
*******************************************************************************/
#include <string.h>
#include "cy_pdl.h"
#include "cy_si_keystorage.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Macros
*******************************************************************************/
/* Secure Image Version: Macros to define the secure image version and ID */
#define SI_APP_VERSION(major, minor)        (((major) << 24u) | ((minor) << 16u))

/* Secure Image begin Flash address */
#define CY_SI_SECURE_FLASH_BEGIN            (0x10000000UL)

/* Size (in Bytes) of the digital signature */
#define CY_SI_SECURE_DIGSIG_SIZE            (256u)

/* Flash Boot clock selection: Clock selection for Flash boot execution */
#define CY_SI_FLASHBOOT_CLK_25MHZ           (0x00UL)        /**< 25MHz clock selection for Flashboot */
#define CY_SI_FLASHBOOT_CLK_8MHZ            (0x01UL)        /**< 8MHz clock selection for Flashboot */
#define CY_SI_FLASHBOOT_CLK_50MHZ           (0x02UL)        /**< 50MHz clock selection for Flashboot */
#define CY_SI_FLASHBOOT_CLK_ROM_CONFIG      (0x03UL)        /**< Use ROM boot clocks configuration (default) */


/* Flash Boot wait window: Debugger wait window selection for Flash boot execution */
#define CY_SI_FLASHBOOT_WAIT_20MS           (0x00UL)        /**< 20ms debugger wait window for Flashboot */
#define CY_SI_FLASHBOOT_WAIT_10MS           (0x01UL)        /**< 10ms debugger wait window for Flashboot */
#define CY_SI_FLASHBOOT_WAIT_1MS            (0x02UL)        /**< 1ms debugger wait window for Flashboot */
#define CY_SI_FLASHBOOT_WAIT_0MS            (0x03UL)        /**< 0ms debugger wait window for Flashboot */
#define CY_SI_FLASHBOOT_WAIT_100MS          (0x04UL)        /**< 100ms debugger wait window for Flashboot */

/*  Debugger pin configuration. */
#define CY_SI_FLASHBOOT_SWJ_DISABLE         (1UL)           /**< Do not enable SWJ pins in Flash boot. Listen window is skipped */
#define CY_SI_FLASHBOOT_SWJ_ENABLE          (2UL)           /**< Enable SWJ pins in Flash boot  */

/* Flash Boot validation: Flash boot validation selection in chip NORMAL mode */
#define CY_SI_FLASHBOOT_VALIDATE_DISABLE    (1UL)           /**< Do not validate app1 in NORMAL mode */
#define CY_SI_FLASHBOOT_VALIDATE_ENABLE     (2UL)           /**< Validate app1 in NORMAL mode */

/* Flash boot loader configuration. */
#define CY_SI_FLASHBOOT_FBLOADER_ENABLE     (1UL)           /**< Internal bootloader is launched if the other bootloader conditions are met  */
#define CY_SI_FLASHBOOT_FBLOADER_DISABLE    (2UL)           /**< Internal bootloader is disabled */

/* Application format: Application format selection for secure boot */
#define CY_SI_APP_FORMAT_BASIC              (0UL)           /**< Basic application format (no header) */
#define CY_SI_APP_FORMAT_CYPRESS            (1UL)           /**< Cypress application format (Cypress header) */

/* Application type: Application type selection for secure boot */
#define CY_SI_APP_ID_FLASHBOOT              (0x8001UL)      /**< Flash boot ID Type */
#define CY_SI_APP_ID_SECUREIMG              (0x8002UL)      /**< Secure image ID Type */
#define CY_SI_APP_ID_BOOTLOADER             (0x8003UL)      /**< Bootloader ID Type */


/*******************************************************************************
* Constants
*******************************************************************************/
#define CY_SI_TOC_FLAGS_CLOCKS_MASK     (0x00000003UL)      /**< Mask for Flashboot clock selection */
#define CY_SI_TOC_FLAGS_CLOCKS_POS      (0UL)               /**< Bit position of Flashboot clock selection */
#define CY_SI_TOC_FLAGS_DELAY_MASK      (0x0000001CUL)      /**< Mask for Flashboot wait window selection */
#define CY_SI_TOC_FLAGS_DELAY_POS       (2UL)               /**< Bit position of Flashboot wait window selection */
#define CY_SI_TOC_FLAGS_SWJEN_POS       (5UL)               /**< Bit position of SWJ pin configuration */
#define CY_SI_TOC_FLAGS_APP_VERIFY_MASK (0x80000000UL)      /**< Mask for Flashboot NORMAL mode app1 validation */
#define CY_SI_TOC_FLAGS_APP_VERIFY_POS  (7UL)               /**< Bit position of Flashboot NORMAL mode app1 validation */

#define CY_SI_TOC_FLAGS_FBLOADER_ENABLE_POS (9UL)           /**< Bit position of Flashboot Loader Enable */

#define CY_SI_TOC2_OBJECTSIZE           (0x000001FCUL)      /**< Number of TOC2 object */
#define CY_SI_SWPU_BEGIN                (0x17007600UL)      /**< Address of SWPU configuration */
#define CY_SI_TOC2_BEGIN                (0x17007C00UL)      /**< Address of TOC2 */

#define CY_SI_IMGVAL_VERIFYAPP_ADDR     ((volatile uint32_t *)0x17002040UL)         /**< Flash boot verify app function address */
#define CY_SI_IMGVAL_VERIFYAPP_REG      (*(uint32_t *)CY_SI_IMGVAL_VERIFYAPP_ADDR)  /**< Flash boot verify app function register */

#define CY_SI_IMGVAL_VALIDKEY_ADDR      ((volatile uint32_t *)0x17002044UL)         /**< Flash boot validate key function address */
#define CY_SI_IMGVAL_VALIDKEY_REG       (*(uint32_t *)CY_SI_IMGVAL_VALIDKEY_ADDR)   /**< Flash boot validate key function register */

#define CY_SI_TOC2_MAGICNUMBER          (0x01211220UL)      /**< TOC2 identifier */

#define CY_SI_PUBLIC_KEY                (0x17006400UL)      /**< PUBLIC KEY address in SFlash */

#define CY_SI_PUBLIC_KEY_ADDR           (0x17006408)        /**< PUBLIC KEY data address in SFlash */


/*******************************************************************************
* ROM/SFlash function typedefs
*******************************************************************************/
typedef uint32_t (*sflash_verifyapp_func_t)(uint32_t param0, uint32_t param1, uint32_t param2, cy_stc_crypto_rsa_pub_key_t *param3);
typedef uint32_t (*sflash_validkey_func_t)(uint32_t param0, cy_stc_crypto_rsa_pub_key_t *param1);


/*******************************************************************************
* Structs
*******************************************************************************/
/* Table of Content structure */
typedef struct
{
    volatile uint32_t objSize;       /**< Object size (Bytes) */
    volatile uint32_t magicNum;      /**< TOC ID (magic number) */

    volatile uint32_t smifCfgAddr;   /**< SMIF configuration structure */

    volatile uint32_t appAddr1;      /**< First user application object address */
    volatile uint32_t appFormat1;    /**< First user application format */
    volatile uint32_t appAddr2;      /**< Second user application object address */
    volatile uint32_t appFormat2;    /**< Second user application format */

    volatile uint32_t cmx1appAddr1;  /**< Address of First CM4 or CM7 core1 User Application Object */
    volatile uint32_t cmx1appAddr2;  /**< Address of Second CM4 or CM7 core1 User Application Object */
    volatile uint32_t cmx2appAddr1;  /**< Address of First CM4 or CM7 core2 User Application Object */
    volatile uint32_t cmx2appAddr2;  /**< Address of Second CM4 or CM7 core2 User Application Object */

    volatile uint32_t reserved1[52];

    volatile uint32_t securityUpdateMaker; /**< Marker for Security Updates */

    volatile uint32_t shashObj;      /**< Number of additional objects to be verified (S-HASH) */
    volatile uint32_t sigKeyAddr;    /**< Signature verification key address */
    volatile uint32_t appProtectionAddr; /**< Address of Application Protection */

    volatile uint32_t reserved2[58];

    volatile uint32_t tocRevision;  /**< TOC2 Revision */
    volatile uint32_t tocFlags;      /**< Flags in TOC to control Flash boot options */
}cy_stc_si_toc_t;

/* Secure image application header in Cypress format */
typedef struct
{
    volatile uint32_t objSize;       /**< Object size (Bytes) */
    volatile uint32_t appId;         /**< Application ID/version */
    volatile uint32_t appAttributes; /**< Attributes (reserved for future use) */
    volatile uint32_t numCores;      /**< Number of cores */
    volatile uint32_t core0Vt;       /**< (CM0+)VT offset - offset to the vector table from that entry */
    volatile uint32_t core0Id;       /**< CM0+ core ID */
}cy_stc_si_appheader_t;

/* User application header in Cypress format */
typedef struct
{
    volatile uint32_t objSize;       /**< Object size (Bytes) */
    volatile uint32_t appId;         /**< Application ID/version */
    volatile uint32_t appAttributes; /**< Attributes (reserved for future use) */
    volatile uint32_t numCores;      /**< Number of cores */
    volatile uint32_t core0Vt;       /**< (CM0+)VT offset - offset to the vector table from that entry */
    volatile uint32_t core1Vt;       /**< (CM4)VT offset - offset to the vector table from that entry */
    volatile uint32_t core0Id;       /**< CM0+ core ID */
    volatile uint32_t core1Id;       /**< CM4 core ID */
}cy_stc_user_appheader_t;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_FB_VerifyApplication(uint32_t address, uint32_t length, uint32_t signature,
                                 cy_stc_crypto_rsa_pub_key_t *publicKey);
__STATIC_INLINE uint32_t Cy_FB_IsValidKey(uint32_t tocAddr, cy_stc_crypto_rsa_pub_key_t *publicKey);



/*******************************************************************************
* Function Name: Cy_FB_VerifyApplication
****************************************************************************//**
*
* \brief Verifies the secure application digital signature.
*
* This function relies on the assumption that the application digital signature
* was calculated in the following manner:
* 1. SHA-256 hash of the binary application image is calculated.
* 2. The hash (digital digest) is signed using a RSA-1024/2056 <b>private</b> 
*    key to generate the digital signature.
* 3. The digital signature is placed in the application object in Cypress format.
*
* The application verification is performed by performing the following
* operations:
* 1. SHA-256 hash of the binary application image is calculated.
* 2. The application digital signature is decrypted using the RSA-1024/2056
*    <b>public</b> key.
* 3. The hash and the decrypted digital signature are compared. If they are
*    equivalent, the image is valid.
*
* \note This is a direct branch to a function residing in SFlash.
*
* \param address      
* Staring address of the application area to be verified with secure signature.
*
* \param length
* The length of the area to be verified.
* 
* \param signature
* Starting address of the signature inside the application residing in Flash.
* 
* \param publicKey
* Pointer to the public key structure.
*
* \return
* - 1 if the digital secure signature verification succeeds.
* - 0 if the digital secure signature verification of the application fails.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_FB_VerifyApplication(uint32_t address, uint32_t length, 
                                        uint32_t signature, cy_stc_crypto_rsa_pub_key_t *publicKey)
{
    sflash_verifyapp_func_t fp = (sflash_verifyapp_func_t)CY_SI_IMGVAL_VERIFYAPP_REG;
    return ( fp(address, length, signature, publicKey) );
}

/*******************************************************************************
* Function Name: Cy_FB_IsValidKey
****************************************************************************//**
*
* \brief Checks whether the Public Key structure is valid.
*
* The public key structure must be as specified as in cy_si_stc_public_key_t.
* Supported signature schemes are:
*   0x00: RSASSA-PKCS1-v1_5-2048
*   0x01: RSASSA-PKCS1-v1_5-1024
*
* \note This is a direct branch to a function residing in SFlash.
*
* \return
* - 1 if Public Key has a valid format
* - 0 if Public Key has an invalid format
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_FB_IsValidKey(uint32_t tocAddr, cy_stc_crypto_rsa_pub_key_t *publicKey)
{
    sflash_validkey_func_t fp = (sflash_validkey_func_t)CY_SI_IMGVAL_VALIDKEY_REG;
    return ( fp(tocAddr, publicKey) );
}


#if defined(__cplusplus)
}
#endif

#endif /* CY_SI_CONFIG_H */

/* [] END OF FILE */
