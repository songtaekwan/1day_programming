#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ADC_Single_Channel.h"
#include "Bsp.h"
#include "Motor_control_pwm.h"
#include "GPT12_PWM_Generation.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define WAIT_TIME   1000             /* Number of milliseconds to wait between each conversion                       */

IfxCpu_syncEvent g_cpuSyncEvent = 0;
int adc_value;
int pre_val = 0;
int val = 0;
int duty;
float alpha = 0.2;
int pwm;

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

    /* Initialize UART */

    L298_Pin_init();
    initGpt12PWM();
    runGpt12PWM();

    while(1)
    {
        /*-------filter--------*/
        adc_value = indicateConversionValue();
        val = alpha * pre_val + (1 - alpha) * adc_value;
        pre_val = val;

        /*------duty, pwm-----*/
        duty = (val*100)/4095;
        pwm = (val*255)/4095;

        Set_GptPWM_Duty(duty);
        motor_control(pwm);
        //wait(ticksFor100ms);
    }
    return (1);
}
