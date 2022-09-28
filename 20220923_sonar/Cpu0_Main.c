#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ASCLIN_UART.h"
#include "Bsp.h"
#include "IfxPort.h"
#include "GPT12_PWM_Capture.h"

uint32 finalCntVal_main = 0;
float32 T3_frequency_main = 0;
uint32 g_cntOverflow_main = 0;
uint32 elaps_cnt_val;
uint32 sonar_time;
float32 distance;
uint32 int_distance;

IfxCpu_syncEvent g_cpuSyncEvent = 0;

int core0_main(void)
{
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    
    init_ASCLIN_UART();
    init_GPT12_module();

    while(1)
    {
        finalCntVal_main;
        T3_frequency_main;
        g_cntOverflow_main;
        elaps_cnt_val;

        Trig_Signal();
        distance;
        int_distance=(int)distance;
        send_sonar_data(int_distance);
    }
    return (1);
}
