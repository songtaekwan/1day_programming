#include "IfxAsclin_Asc.h"
#include "IfxCpu_Irq.h"
#include "Bsp.h"
#include "IfxPort.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define UART_BAUDRATE           115200                                  /* UART baud rate in bit/s                  */

#define UART_PIN_RX             IfxAsclin0_RXB_P15_3_IN                 /* UART receive port pin                    */
#define UART_PIN_TX             IfxAsclin0_TX_P15_2_OUT                 /* UART transmit port pin                   */

/* Definition of the interrupt priorities */
#define INTPRIO_ASCLIN0_RX      18
#define INTPRIO_ASCLIN0_TX      19

#define UART_RX_BUFFER_SIZE     64                                      /* Definition of the receive buffer size    */
#define UART_TX_BUFFER_SIZE     64                                      /* Definition of the transmit buffer size   */
#define SIZE                    30                                      /* Size of the string                       */
#define MAX_RCV_BUFFER_SIZE     20                                      /* Definition of the transmit buffer size   */

/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
/* Declaration of the ASC handle */
static IfxAsclin_Asc g_ascHandle;

/* Declaration of the FIFOs parameters */
static uint8 g_ascTxBuffer[UART_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 g_ascRxBuffer[UART_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/* Definition of txData and rxData */
uint8 g_txData[] = "Hello World!\n";
uint8 g_rxData[SIZE] = {''};
uint8 g_rxRcvBuffer[MAX_RCV_BUFFER_SIZE] = {''};
uint8 send_buff[9] = {''};

/* Size of the message */
Ifx_SizeT g_count = sizeof(g_txData);

int cnt_rcv = 0;

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Adding of the interrupt service routines */
IFX_INTERRUPT(asclin0TxISR, 0, INTPRIO_ASCLIN0_TX);

void asclin0TxISR(void)
{
    IfxAsclin_Asc_isrTransmit(&g_ascHandle);
}

IFX_INTERRUPT(asclin0RxISR, 0, INTPRIO_ASCLIN0_RX);

void asclin0RxISR(void)
{
    int size_of_read = 1;
    IfxAsclin_Asc_isrReceive(&g_ascHandle);
    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &size_of_read, TIME_INFINITE);

    for(int i = 0; i < MAX_RCV_BUFFER_SIZE-1; i++)
    {
        g_rxRcvBuffer[i] = g_rxRcvBuffer[i+1];
    }
    g_rxRcvBuffer[MAX_RCV_BUFFER_SIZE-1] = g_rxData[0];

    cnt_rcv++;
}

/* This function initializes the ASCLIN UART module */
void init_ASCLIN_UART(void)
{
    /* Initialize an instance of IfxAsclin_Asc_Config with default values */
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

    /* Set the desired baud rate */
    ascConfig.baudrate.baudrate = UART_BAUDRATE;

    /* ISR priorities and interrupt target */
    ascConfig.interrupt.txPriority = INTPRIO_ASCLIN0_TX;
    ascConfig.interrupt.rxPriority = INTPRIO_ASCLIN0_RX;
    ascConfig.interrupt.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

    /* FIFO configuration */
    ascConfig.txBuffer = &g_ascTxBuffer;
    ascConfig.txBufferSize = UART_TX_BUFFER_SIZE;
    ascConfig.rxBuffer = &g_ascRxBuffer;
    ascConfig.rxBufferSize = UART_RX_BUFFER_SIZE;

    /* Pin configuration */
    const IfxAsclin_Asc_Pins pins =
    {
        NULL_PTR,       IfxPort_InputMode_pullUp,     /* CTS pin not used */
        &UART_PIN_RX,   IfxPort_InputMode_pullUp,     /* RX pin           */
        NULL_PTR,       IfxPort_OutputMode_pushPull,  /* RTS pin not used */
        &UART_PIN_TX,   IfxPort_OutputMode_pushPull,  /* TX pin           */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    IfxAsclin_Asc_initModule(&g_ascHandle, &ascConfig); /* Initialize module with above parameters */
}

void send_sonar_data(int sonar_data)
{
        Ifx_SizeT number_of_byte = 9;
        union
        {
            int data;
            char bytedata[4];
        } sonar_value;

        union
        {
            sint16 data;
            char bytedata[2];
        } m_crc_data;

        sonar_value.data = sonar_data;

        send_buff[0] = '#';

        send_buff[1] = 'I';
        send_buff[2] = sonar_value.bytedata[3];
        send_buff[3] = sonar_value.bytedata[2];
        send_buff[4] = sonar_value.bytedata[1];
        send_buff[5] = sonar_value.bytedata[0];

        m_crc_data.data = 0;
        for(int i = 1; i<6; i++)
            m_crc_data.data += send_buff[i];

        send_buff[6] = m_crc_data.bytedata[1];
        send_buff[7] = m_crc_data.bytedata[0];
        send_buff[8] = '*';

        IfxAsclin_Asc_write(&g_ascHandle, send_buff, &number_of_byte, TIME_INFINITE);
}

//send hello world
void send_receive_ASCLIN_UART_message(void)
{
    IfxAsclin_Asc_write(&g_ascHandle, g_txData, &g_count, TIME_INFINITE);
    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &g_count, TIME_INFINITE);
}
/*
void send_ASCLIN_UART_message(void)
{
    IfxAsclin_Asc_write(&g_ascHandle, g_txData, &g_count, TIME_INFINITE);
}

void receive_ASCLIN_UART_message(sint32 cnt)
{
    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &cnt, TIME_INFINITE);
sint32 receive_buff_count(void)
{
    return  IfxAsclin_Asc_getReadCount(&g_ascHandle);
}
*/
