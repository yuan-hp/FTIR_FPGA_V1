
/*
 * Huart.c
 * 串口控制器
 *  Created on: 2019年6月13日
 *      Author: hpy
 */
#include "Huart.h"
#include "xparameters.h"
#include "xscugic.h"
/**
 * 初始化串口,接收采用查询方式，本机作为串口主机，FIFO=64
 * 参数1：uart 串口对象
 * 参数2：DeviceId  设备ID
 * 参数3：BaudRate 波特率
 * 操作成功返回0
 */
#define UART_DEVICE_ID		XPAR_XUARTPS_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define UART_INT_IRQ_ID		XPAR_XUARTPS_1_INTR

XScuGic uart_int;

int Huart_init_s(XUartPs *uart,u16 DeviceId,u32 BaudRate)
{
		int Status;
		XUartPs_Config *Config;

		/*
		 * Initialize the UART driver so that it's ready to use
		 * Look up the configuration in the config table and then initialize it.
		 */
		Config = XUartPs_LookupConfig(DeviceId);
		if (NULL == Config) {
			return XST_FAILURE;
		}

		Status = XUartPs_CfgInitialize(uart, Config, Config->BaseAddress);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
		XUartPs_SetBaudRate(uart, BaudRate);//设置波特率
		XUartPs_SetRecvTimeout(uart, 8);
		return 0;
}

/*****************************************************************************/
/**
*
* This function sets up the interrupt system so interrupts can occur for the
* Uart. This function is application-specific. The user should modify this
* function to fit the application.
*
* @param	IntcInstancePtr is a pointer to the instance of the INTC.
* @param	UartInstancePtr contains a pointer to the instance of the UART
*		driver which is going to be connected to the interrupt
*		controller.
* @param	UartIntrId is the interrupt Id and is typically
*		XPAR_<UARTPS_instance>_INTR value from xparameters.h.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
****************************************************************************/
static int SetupInterruptSystem(XScuGic *IntcInstancePtr,
				XUartPs *UartInstancePtr,
				u16 UartIntrId)
{
	int Status;

#ifdef XPAR_INTC_0_DEVICE_ID
#ifndef TESTAPP_GEN
	/*
	 * Initialize the interrupt controller driver so that it's ready to
	 * use.
	 */
	Status = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
	/*
	 * Connect the handler that will be called when an interrupt
	 * for the device occurs, the handler defined above performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(IntcInstancePtr, UartIntrId,
		(XInterruptHandler) XUartPs_InterruptHandler, UartInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#ifndef TESTAPP_GEN
	/*
	 * Start the interrupt controller so interrupts are enabled for all
	 * devices that cause interrupts.
	 */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif
	/*
	 * Enable the interrupt for uart
	 */
	XIntc_Enable(IntcInstancePtr, UartIntrId);

	#ifndef TESTAPP_GEN
	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler) XIntc_InterruptHandler,
				IntcInstancePtr);
	#endif
#else
#ifndef TESTAPP_GEN
	XScuGic_Config *IntcConfig; /* Config for interrupt controller */

	/* Initialize the interrupt controller driver */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the
	 * hardware interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler) XScuGic_InterruptHandler,
				IntcInstancePtr);
#endif

	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(IntcInstancePtr, UartIntrId,
				  (Xil_ExceptionHandler) XUartPs_InterruptHandler,
				  (void *) UartInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Enable the interrupt for the device */
	XScuGic_Enable(IntcInstancePtr, UartIntrId);

#endif
#ifndef TESTAPP_GEN
	/* Enable interrupts */
	 Xil_ExceptionEnable();
#endif

	return XST_SUCCESS;
}

void UART_TRQ(void *CallBackRef, u32 Event, unsigned int EventData)
{
	/* All of the data has been sent */
	if (Event == XUARTPS_EVENT_SENT_DATA) {
		//TotalSentCount = EventData;
	}

	/* All of the data has been received */
	if (Event == XUARTPS_EVENT_RECV_DATA) {
		//TotalReceivedCount = EventData;
		//RecvBuf[0] = UartPs_ReceiveBuffer(&uart0);
		//XUartPs_Recv(&uart0, , 1);
	}

	/*
	 * Data was received, but not the expected number of bytes, a
	 * timeout just indicates the data stopped for 8 character times
	 */
	if (Event == XUARTPS_EVENT_RECV_TOUT) {
		//TotalReceivedCount = EventData;
	}

	/*
	 * Data was received with an error, keep the data but determine
	 * what kind of errors occurred
	 */
	if (Event == XUARTPS_EVENT_RECV_ERROR) {
		//TotalReceivedCount = EventData;
		//TotalErrorCount++;
	}

	/*
	 * Data was received with an parity or frame or break error, keep the data
	 * but determine what kind of errors occurred. Specific to Zynq Ultrascale+
	 * MP.
	 */
	if (Event == XUARTPS_EVENT_PARE_FRAME_BRKE) {
		//TotalReceivedCount = EventData;
		//TotalErrorCount++;
	}

	/*
	 * Data was received with an overrun error, keep the data but determine
	 * what kind of errors occurred. Specific to Zynq Ultrascale+ MP.
	 */
	if (Event == XUARTPS_EVENT_RECV_ORERR) {
		//TotalReceivedCount = EventData;
		//TotalErrorCount++;
	}
}

int Huart_init(XUartPs *uart,u16 DeviceId,u32 BaudRate)
{
		int Status;
		XUartPs_Config *Config;
		u32 IntrMask;
		/*
		 * Initialize the UART driver so that it's ready to use
		 * Look up the configuration in the config table and then initialize it.
		 */
		Config = XUartPs_LookupConfig(DeviceId);
		if (NULL == Config) {
			return XST_FAILURE;
		}

		Status = XUartPs_CfgInitialize(uart, Config, Config->BaseAddress);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}
		XUartPs_SetBaudRate(uart, BaudRate);//设置波特率

		Status = SetupInterruptSystem(&uart_int,uart, DeviceId);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		//XUartPs_SetHandler(&uart_int, (XUartPs_Handler)UART_TRQ, uart);//暂时屏蔽

		IntrMask =
				XUARTPS_IXR_TOUT | XUARTPS_IXR_PARITY | XUARTPS_IXR_FRAMING |
				XUARTPS_IXR_OVER | XUARTPS_IXR_TXEMPTY | XUARTPS_IXR_RXFULL |
				XUARTPS_IXR_RXOVR;

		if (uart->Platform == XPLAT_ZYNQ_ULTRA_MP) {
			IntrMask |= XUARTPS_IXR_RBRK;
		}

		XUartPs_SetInterruptMask(uart, IntrMask);
		XUartPs_SetOperMode(uart, XUARTPS_OPER_MODE_NORMAL);

		/*
		 * Set the receiver timeout. If it is not set, and the last few bytes
		 * of data do not trigger the over-water or full interrupt, the bytes
		 * will not be received. By default it is disabled.
		 *
		 * The setting of 8 will timeout after 8 x 4 = 32 character times.
		 * Increase the time out value if baud rate is high, decrease it if
		 * baud rate is low.
		 */
		XUartPs_SetRecvTimeout(uart, 8);
		XUartPs_EnableUart(uart);
		return 0;
}
/**
 * 发送字节
 * 参数1：串口对象
 * 参数2：发送缓冲区
 * 参数3：发送字节数
 * 操作成功返回0
 */
int Huart_Send(XUartPs *uart,u8 *SendBuf,u16 ByteCount)
{
	 if(0 == XUartPs_Send(uart,SendBuf,ByteCount) )return 1;
	return 0;
}
/**
 * 发送字节
 * 参数1：串口对象
 * 参数2：接收缓冲区
 * 参数3：接收字节数
 * 操作成功返回接受的个数
 *
 * 务必保证它机串口发送来的数据字节数和本机接收所设置的字节数一致
 */
int Huart_Recv(XUartPs *uart,u8 *RecvBuf,u16 ByteCount)
{
	 return XUartPs_Recv(uart,RecvBuf,ByteCount);
}


