/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for CM4 core in the Secure boot
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
#include "cy_retarget_io.h"


/*******************************************************************************
* Macros
********************************************************************************/

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Interrupt configuration */
const cy_stc_sysint_t IRQ_CFG =
{
    .intrSrc = (NvicMux3_IRQn << CY_SYSINT_INTRSRC_MUXIRQ_SHIFT) | TCPWM_TIMER_IRQ,
    .intrPriority = 7UL
};

bool timer_interrupt_flag = false;
bool led_blink_active_flag = true;

/* Variable for storing character read from terminal */
uint8_t uart_read_value = 0UL;

/* For the Retarget -IO (Debug UART) usage */
static cy_stc_scb_uart_context_t    UART_context;           /** UART context */
static mtb_hal_uart_t               UART_hal_obj;           /** Debug UART HAL object  */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void timer_init(void);
void isr_timer(void);

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It sets up a timer to trigger a periodic
* interrupt. The main while loop checks for the status of a flag set by the 
* interrupt and whether the toggles an LED at 1Hz to create an LED blinky. The
* while loop also checks 'Enter' key was pressed and stops/restarts LED blinking.
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

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Debug UART init */
    result = (cy_rslt_t)Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);

    /* UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_SCB_UART_Enable(UART_HW);

    /* Setup the HAL UART */
    result = mtb_hal_uart_setup(&UART_hal_obj, &UART_hal_config, &UART_context, NULL);

    /* HAL UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    result = cy_retarget_io_init(&UART_hal_obj);

    /* HAL retarget_io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "Secure Boot Application Example "
           "****************** \r\n\n");

    printf("Application is successfully launched\r\n\n");

    printf("Hello World!!!\r\n\n");

    printf("For more projects, "
           "visit our code examples repositories:\r\n\n");

    printf("https://github.com/Infineon/"
           "Code-Examples-for-ModusToolbox-Software\r\n\n");

    /* Initialize timer to toggle the LED */
    timer_init();

    printf("Press 'Enter' key to pause or "
           "resume blinking the user LED \r\n\r\n");

    for (;;)
    {
        /* Check if 'Enter' key was pressed */
        uart_read_value = Cy_SCB_UART_Get(UART_HW);
        if (uart_read_value == '\r')
        {
            /* Pause LED blinking by stopping the timer */
            if (led_blink_active_flag)
            {
                Cy_TCPWM_Counter_Disable(TCPWM_TIMER_HW, TCPWM_TIMER_NUM);

                printf("LED blinking paused \r\n");
            }
            else /* Resume LED blinking by starting the timer */
            {
                Cy_TCPWM_Counter_Enable(TCPWM_TIMER_HW, TCPWM_TIMER_NUM);
                Cy_TCPWM_TriggerStart_Single(TCPWM_TIMER_HW, TCPWM_TIMER_NUM);

                printf("LED blinking resumed\r\n");
            }

            /* Move cursor to previous line */
            printf("\x1b[1F");

            led_blink_active_flag ^= 1;
        }

        /* Check if timer elapsed (interrupt fired) and toggle the LED */
        if (timer_interrupt_flag)
        {
            /* Clear the flag */
            timer_interrupt_flag = false;

            /* Invert the USER LED state */
            Cy_GPIO_Inv(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
        }
    }
}

/*******************************************************************************
* Function Name: timer_init
********************************************************************************
* Summary:
* This function creates and configures a Timer object. The timer ticks
* continuously and produces a periodic interrupt on every terminal count
* event. The period is configured by Device Configurator. Without any changes,
* this application is designed to produce an interrupt every 1 second.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
 void timer_init(void)
 {
    cy_rslt_t result;

    /* Initialize TCPWM counter mode */
    result = Cy_TCPWM_Counter_Init(TCPWM_TIMER_HW, TCPWM_TIMER_NUM, &TCPWM_TIMER_config);

    if(result != CY_TCPWM_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Interrupt settings */
    Cy_SysInt_Init(&IRQ_CFG, &isr_timer);
    NVIC_ClearPendingIRQ(NvicMux3_IRQn);
    NVIC_EnableIRQ((IRQn_Type) NvicMux3_IRQn);

    /* Enable the initialized counter */
    Cy_TCPWM_Counter_Enable(TCPWM_TIMER_HW, TCPWM_TIMER_NUM);

    /* Start the timer with the configured settings */
    Cy_TCPWM_TriggerStart_Single(TCPWM_TIMER_HW, TCPWM_TIMER_NUM);
 }

/*******************************************************************************
* Function Name: isr_timer
********************************************************************************
* Summary:
* This is the interrupt handler function for the timer interrupt.
*
* Parameters:
*    callback_arg    Arguments passed to the interrupt callback
*    event            Timer/counter interrupt triggers
*
* Return:
*  void
*******************************************************************************/
void isr_timer(void)
{
    /* Get interrupt source */
    uint32_t intrMask = Cy_TCPWM_GetInterruptStatusMasked(TCPWM_TIMER_HW, TCPWM_TIMER_NUM);

    /* Clear interrupt source */
    Cy_TCPWM_ClearInterrupt(TCPWM_TIMER_HW, TCPWM_TIMER_NUM, intrMask);

    /* Set interrupt flag */
    timer_interrupt_flag = true;
}

/* [] END OF FILE */
