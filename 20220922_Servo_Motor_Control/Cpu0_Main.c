#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ADC_Single_Channel.h"
#include "Bsp.h"
#include "GPT12_Timer_Interrupt.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define LED1                        &MODULE_P00, 5
#define TEST_PIN2                   &MODULE_P02, 1

IfxCpu_syncEvent g_cpuSyncEvent = 0;

int adc_value;
int pre_val = 0;
int val = 0;
float angle;
float alpha = 0.2;
float second;
float WAIT_TIME_adc;

union
{
    int data;
    char bytedata[4];
} m_adc_value;

int core0_main(void)
{
    IfxCpu_enableInterrupts();
    
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    vadcBackgroundScanInit();

    /* Start the background scan */
    vadcBackgroundScanRun();


    IfxPort_setPinModeOutput(TEST_PIN2, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    while(1)
    {
        /*-------filter--------*/
        adc_value = indicateConversionValue();
        val = alpha * pre_val + (1 - alpha) * adc_value;
        pre_val = val;

        /*------angle, second-----*/
        angle = (val*180.0)/4094.0;

        WAIT_TIME_adc = val/4094.0 + 1;

        if(WAIT_TIME_adc < 1)
        {
            WAIT_TIME_adc = 1;
        }
        IfxPort_setPinHigh(TEST_PIN2);
        wait(WAIT_TIME_adc*100000);

        IfxPort_setPinLow(TEST_PIN2);
        wait((20.0-WAIT_TIME_adc)*100000);
    }
    return (1);
}
