#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"
#include "Bsp.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_GPT12_T2_INT   10              /* Define the GPT12 Timer 2 interrupt priority                  */
#define ISR_PRIORITY_GPT12_T3_INT   11              /* Define the GPT12 Timer 2 interrupt priority                  */
#define MAX_VAL_16_BIT              0xFFFF          /* Used for calculation of timer value with overflows           */

#define PWM_PIN                     &MODULE_P00,8   /* Pin which is used to generate a simple PWM signal            */
#define T3_OUT_PIN                  &MODULE_P02,0
#define Test_PIN                    &MODULE_P02,1
/*----------------sonar pin----------------------*/
#define TRIG_PIN                    &MODULE_P02,3
#define ECHO_PIN                    &MODULE_P00,7

#define PWM_DUTY                    0.8             /* Duty cycle of generated PWM signal, value between 0 and 1    */
#define FACTOR_SEC_TO_USEC          1000000         /* Factor to convert seconds to microseconds                    */
#define WAIT_TIME                   1
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
float32 g_generatedPwmFreq_Hz   = 50.0; /* Global variable for frequency of generated PWM signal                    */
float32 g_measuredPwmFreq_Hz    = 0.0;  /* Global variable for frequency calculation of PWM signal                  */
uint32  g_cntOverflow           = 0;    /* Global counter variable for the timer overflow between two edges         */
uint32  g_cntOverflow_rising    = 0;    /* Global counter variable for the timer overflow between two edges         */
uint32  g_cntOverflow_falling   = 0;    /* Global counter variable for the timer overflow between two edges         */
uint32  g_previousCntVal        = 0;    /* Global variable which stores the timer value of the previous interrupt   */
uint32  currentCntVal_r         = 0;
uint32  currentCntVal_f         = 0;
/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define the Interrupt Service Routine */
extern uint32 finalCntVal_main;
extern float32 T3_frequency_main;
extern uint32 g_cntOverflow_main;
int type_edge = 1;
extern uint32 elaps_cnt_val;
extern uint32 sonar_time;
extern float32 distance;

IFX_INTERRUPT(GPT12_T2_Int0_Handler, 0, ISR_PRIORITY_GPT12_T2_INT);

/* Interrupt Service Routine of timer T2, gets triggered by rising edge on input pin of timer T2 */
void GPT12_T2_Int0_Handler(void)
{
    uint32 currentCntVal = IfxGpt12_T2_getTimerValue(&MODULE_GPT120); /* Get timer value of timer T2 */
    uint32 finalCntVal = 0; /* Variable to calculate final timer counter value */

    IfxPort_setPinState(Test_PIN,IfxPort_State_high);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));
    IfxPort_setPinState(Test_PIN,IfxPort_State_low);

    if(g_cntOverflow == 0)
    {
        /* If no overflow detected */
        finalCntVal = currentCntVal - g_previousCntVal; /* Total counter value calculation */
    }
    else
    {
        /* One or more overflows detected */
        /* Add to the current counter value, the amount of counter ticks which passed before the first overflow,
         * plus 65525 (0xFFFF) for each additional overflow since the previous rising edge.
         */
        finalCntVal = (uint32)(currentCntVal + (MAX_VAL_16_BIT - g_previousCntVal) + ((g_cntOverflow - 1) * MAX_VAL_16_BIT));
    }

    /* Calculation of the PWM frequency by dividing the frequency of timer T3 through the final total counter value */
    T3_frequency_main = IfxGpt12_T3_getFrequency(&MODULE_GPT120);

    g_measuredPwmFreq_Hz = IfxGpt12_T3_getFrequency(&MODULE_GPT120) / finalCntVal;

    finalCntVal_main = finalCntVal;
    g_previousCntVal = currentCntVal;    /* Set g_previousCntVal to currentCntVal for the next calculation */
    g_cntOverflow_main = g_cntOverflow;

    if(type_edge == 1)
    {
        IfxGpt12_T2_setCaptureInputMode(&MODULE_GPT120, IfxGpt12_CaptureInputMode_fallingEdgeTxIN);
        type_edge = 0;
        g_cntOverflow = 0;
        g_cntOverflow_rising = g_cntOverflow;
        currentCntVal_r = currentCntVal;
    }
    else
    {
        IfxGpt12_T2_setCaptureInputMode(&MODULE_GPT120, IfxGpt12_CaptureInputMode_risingEdgeTxIN);
        type_edge = 1;
        g_cntOverflow_falling = g_cntOverflow;
        currentCntVal_f = currentCntVal;

        elaps_cnt_val = (uint32)(currentCntVal_f + (MAX_VAL_16_BIT - currentCntVal_r) + ((g_cntOverflow - 1) * MAX_VAL_16_BIT));
        //elaps_cnt_val = (uint32)(currentCntVal_f + (MAX_VAL_16_BIT - currentCntVal_r) + ((g_cntOverflow_falling - g_cntOverflow_rising -1) * MAX_VAL_16_BIT));
        sonar_time = elaps_cnt_val*40;
        distance = (340.0*sonar_time)/(1000000.0*2.0);
    }
}

/* Macro to define the Interrupt Service Routine. */
IFX_INTERRUPT(GPT12_T3_Int0_Handler, 0, ISR_PRIORITY_GPT12_T3_INT);

/* Interrupt Service Routine of timer T3, gets triggered after T3 timer overflow */
void GPT12_T3_Int0_Handler(void)
{
    g_cntOverflow++; /* Increase overflow counter */

    IfxPort_setPinState(T3_OUT_PIN,IfxPort_State_high);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));
    IfxPort_setPinState(T3_OUT_PIN,IfxPort_State_low);
}

/* This function initializes timer T2 and T3 of the block GPT1 of module GPT12 to capture PWM signals. */
void init_GPT12_module(void)
{
    /* Enable GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120);
    /* Select 4 as prescaler for the GPT1 module -> fastest clock frequency for best accuracy */
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_4);
    /* Set core timer T3 in timer mode */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    /* Set auxiliary timer T2 in capture mode */
    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_capture);
    /* Select input pin A of timer T2 which is P00.7 (the James Bond pin) */
    IfxGpt12_T2_setInput(&MODULE_GPT120, IfxGpt12_Input_A);
    /* Select rising edge as capture event */
    IfxGpt12_T2_setCaptureInputMode(&MODULE_GPT120, IfxGpt12_CaptureInputMode_risingEdgeTxIN);

    /* Service request configuration */
    /* Get source pointer of timer T2, initialize and enable */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T2_getSrc(&MODULE_GPT120);
    IfxSrc_init(src, IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_T2_INT);
    IfxSrc_enable(src);
    /* Get source pointer of timer T3, initialize and enable */
    src = IfxGpt12_T3_getSrc(&MODULE_GPT120);
    IfxSrc_init(src, IfxSrc_Tos_cpu0, ISR_PRIORITY_GPT12_T3_INT);
    IfxSrc_enable(src);
    /*
    IfxPort_setPinMode(T3_OUT_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(T3_OUT_PIN, IfxPort_State_low);

    IfxPort_setPinMode(Test_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinState(Test_PIN, IfxPort_State_low);
    */
    /* Initialize TRIG_PIN port pin */
    IfxPort_setPinMode(TRIG_PIN, IfxPort_Mode_outputPushPullGeneral);
    //IfxPort_setPinState(TRIG_PIN, IfxPort_State_low);

    /* Start timer T3*/
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);
}

void Trig_Signal(void)
{
    IfxPort_setPinModeOutput(TRIG_PIN, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    IfxPort_setPinHigh(TRIG_PIN);
    wait(100000);

    IfxPort_setPinLow(TRIG_PIN);
    wait(10000000);
}
