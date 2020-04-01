#include "system.h"
//////////////////////////////////////////user identifier////////////////////////////////////

static struct netif server_netif;
int sedflg=0;
int s=0;
int sendnum=0;

#define Sndbuf_Size 32768
int TCPSendLength=0;
int TCPSendPos=0;
int BufSendNum=0;

/////////////////////////////////////////////////////////////////////////////////////////////

int transfer_data() {
	return 0;
}
void OrderAnalyse(unsigned char order,int orderdata);
void print_app_header()
{
#if (LWIP_IPV6==0)
	xil_printf("\n\r\n\r-----lwIP TCP echo server ------\n\r");
#else
	xil_printf("\n\r\n\r-----lwIPv6 TCP echo server ------\n\r");
#endif
	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	/* echo back the payload */
	/* in this case, we assume that the payload is < TCP_SND_BUF */
	if (tcp_sndbuf(tpcb) > p->len) {
		//err = tcp_write(tpcb, p->payload, p->len, 1);
		//LinTcpSend(0,1048576);
		//SysFlag=1;
		//PCNeedDataFlag=1-PCNeedDataFlag;
		memcpy(LwipRevBuf+LwipRevNum,p->payload,p->len);
		LwipRevNum+=p->len;
		///////////////////////////////////
		Hgpio_axi_int_init();//配置数据采集完成中断信号
		Hak5394_PL_init();//配置ak5394工作模式
		///////////////////////////////////
		while(1)
		{
			if(LwipRevIndex>=LwipRevNum-7)
			{
				LwipRevIndex=0;
				LwipRevNum=0;
				memset(LwipRevBuf,'\0',sizeof(LwipRevBuf));
				break;
			}


			if((LwipRevBuf[LwipRevIndex]=='A')&&(LwipRevBuf[LwipRevIndex+1]=='A')&&(LwipRevBuf[LwipRevIndex+2]=='A')&&(LwipRevBuf[LwipRevIndex+3]=='A'))
			{
				unsigned char order = LwipRevBuf[LwipRevIndex+4];
				int orderdata = *((int*)(LwipRevBuf+LwipRevIndex+5));
				//printf("%d\r\n",orderdata);

				OrderAnalyse(order,orderdata);

			}
			LwipRevIndex++;
		}


		//LinTcpSend(1024);

	} else
		xil_printf("no space in tcp_sndbuf\n\r");

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	if(LwipBusyFlag==1)
	{
		if(TCPSendLength>0)
		{
			if (tpcb->snd_buf>=Sndbuf_Size)
			{
				if(TCPSendLength==TCPSendPos)
				{
					TCPSendLength=0;
					TCPSendPos=0;
					LwipBusyFlag=0;
				}
				if((TCPSendLength-TCPSendPos<=Sndbuf_Size)&&(TCPSendLength!=TCPSendPos))
				{
					tcp_write(tpcb, Lwipram+TCPSendPos, TCPSendLength-TCPSendPos, 1);
					TCPSendPos+=TCPSendLength-TCPSendPos;
				}

				if(TCPSendLength-TCPSendPos>Sndbuf_Size)
				{
					tcp_write(tpcb, Lwipram+TCPSendPos, Sndbuf_Size, 1);
					TCPSendPos+=Sndbuf_Size;
				}


			}
		}

	}
	if(LwipBusyFlag==2)
	{
		if(TCPSendLength>0)
		{
			if (tpcb->snd_buf>=Sndbuf_Size)
			{
				if(TCPSendLength==TCPSendPos)
				{
					TCPSendLength=0;
					TCPSendPos=0;
					LwipBusyFlag=0;
				}
				if((TCPSendLength-TCPSendPos<=Sndbuf_Size)&&(TCPSendLength!=TCPSendPos))
				{
					tcp_write(tpcb, LwipramBack+TCPSendPos, TCPSendLength-TCPSendPos, 1);
					TCPSendPos+=TCPSendLength-TCPSendPos;
				}

				if(TCPSendLength-TCPSendPos>Sndbuf_Size)
				{
					tcp_write(tpcb, LwipramBack+TCPSendPos, Sndbuf_Size, 1);
					TCPSendPos+=Sndbuf_Size;
				}


			}
		}

	}

	if(TCPSendLength==0)LwipBusyFlag=0;

	return ERR_OK;
}
//void tcp_error_callback(void *arg, err_t err)
//{
//	ForwordNeedSendFlag=0;
//	BackwordNeedSendFlag=0;
//	LwipBusyFlag=0;
//	LwipForwordNum=0;
//	LwipBackwordNum=0;
//	PCNeedDataFlag=0;
//	//xil_printf("qeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeermiaoshazhi\n\r");
//
//}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	tcp_sent(newpcb, tcp_server_sent);

//	tcp_err(newpcb, tcp_error_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	Mypcb=newpcb;

	ForwordNeedSendFlag=0;
	BackwordNeedSendFlag=0;
	LwipBusyFlag=0;
	LwipForwordNum=0;
	LwipBackwordNum=0;
	PCNeedDataFlag=0;
	ForwordSendPermission=0;
	BackwordSendPermission=1;

	//xil_printf("accepted!\n\r");

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}
void print_ip(char *msg, ip_addr_t *ip)
{
	printf(msg);
	xil_printf("%d.%d.%d.%d\r\n", ip4_addr1(ip), ip4_addr2(ip),ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{
	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}

int Hlwip_init(void){
	ip_addr_t ipaddr, netmask, gw;
	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] =
	{ 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };
	echo_netif = &server_netif;
	//init_platform();
	/* initliaze IP addresses to be used */
	IP4_ADDR(&ipaddr,  192, 168,   1, 10);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      192, 168,   1,  1);
	print_app_header();
	lwip_init();
	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(echo_netif, &ipaddr, &netmask,
						&gw, mac_ethernet_address,
						PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return -1;
	}
	netif_set_default(echo_netif);
	/* now enable interrupts */
	platform_enable_interrupts();
	/* specify that the network if is up */
	netif_set_up(echo_netif);
	print_ip_settings(&ipaddr, &netmask, &gw);
	return 0;
}


void LinTcpSend(u32 length)
{
	if(length<=Sndbuf_Size)
		tcp_write(Mypcb,Lwipram,length,1);//8192
	else
	{
		LwipBusyFlag = 1;//正在发送正向数据中
		tcp_write(Mypcb,Lwipram,Sndbuf_Size,1);//8192
		TCPSendPos=Sndbuf_Size;
		TCPSendLength=length;
	}
}

void LinTcpSendBack(u32 length)
{
	if(length<=Sndbuf_Size)
		tcp_write(Mypcb,LwipramBack,length,1);//8192
	else
	{
		LwipBusyFlag = 2;//正在发送反向数据中
		tcp_write(Mypcb,LwipramBack,Sndbuf_Size,1);//8192
		TCPSendPos=Sndbuf_Size;
		TCPSendLength=length;
	}
}

void OrderAnalyse(unsigned char order,int orderdata)
{
	char str[20];
	if(order=='a')
	{
		PCNeedDataFlag=1;
	}
	if(order=='b')
	{
		PCNeedDataFlag=0;
	}
	if(order=='c')
	{
		switch(orderdata)
		{
			case 3:
				sprintf(str,"R00003");
				break;
			case 4:
				sprintf(str,"R00004");
				break;
			case 5:
				sprintf(str,"R00005");
				break;
			case 10:
				sprintf(str,"R00010");
			break;
			case 20:
				sprintf(str,"R00020");
				break;
			case 40:
				sprintf(str,"R00040");
				break;
			case 80:
				sprintf(str,"R00080");
				break;
			case 160:
				sprintf(str,"R00160");
				break;
		}
		//Huart_Send(&Uart,(u8*)str,6);

	}
	if(order=='d')
	{
		switch(orderdata)
		{
			case 2:
				sprintf(str,"S00002");
				break;
			case 3:
				sprintf(str,"S00003");
				break;
			case 4:
				sprintf(str,"S00004");
				break;
			case 5:
				sprintf(str,"S00005");
				break;
		}
		//Huart_Send(&Uart,(u8*)str,6);

	}
//	if(order=='e')
//	{
//		int temp = abs(orderdata);
//		if(orderdata>=0)
//		{
//			if(temp>=10000)printf("+%d",temp);
//			else if(temp>=1000)printf("+0%d",temp);
//			else if(temp>=100)printf("+00%d",temp);
//			else if(temp>=10)printf("+000%d",temp);
//			else if(temp>=0)printf("+0000%d",temp);
//		}
//		else
//		{
//			if(temp>=10000)printf("-%d",temp);
//			else if(temp>=1000)printf("-0%d",temp);
//			else if(temp>=100)printf("-00%d",temp);
//			else if(temp>=10)printf("-000%d",temp);
//			else if(temp>=0)printf("-0000%d",temp);
//		}
//	}
	if(order=='e')
	{
		int temp = abs(orderdata);

		if(orderdata>=0)
		{
			if(temp>=10000)
				sprintf(str,"+%d",temp);
			else if(temp>=1000)
				sprintf(str,"+0%d",temp);
			else if(temp>=100)
				sprintf(str,"+00%d",temp);
			else if(temp>=10)
				sprintf(str,"+000%d",temp);
			else if(temp>=0)
				sprintf(str,"+0000%d",temp);
		}
		else
		{
			if(temp>=10000)
				sprintf(str,"-%d",temp);
			else if(temp>=1000)
				sprintf(str,"-0%d",temp);
			else if(temp>=100)
				sprintf(str,"-00%d",temp);
			else if(temp>=10)
				sprintf(str,"-000%d",temp);
			else if(temp>=0)
				sprintf(str,"-0000%d",temp);
		}
		//Huart_Send(&Uart,(u8*)str,6);
	}
	if(order=='f')
	{

	}
	if(order=='g')
	{

	}
	if(order=='h')
	{

	}
	if(order=='i')
	{

	}
	if(order=='j')
	{

	}
	if(order=='k')
	{

	}
	if(order=='l')
	{

	}
	if(order=='m')
	{

	}
	if(order=='n')
	{
		if(orderdata==0)
		{
			ForwordSendPermission=1;
			BackwordSendPermission=1;
		}
		if(orderdata==1)
		{
			ForwordSendPermission=1;
			BackwordSendPermission=0;
		}
		if(orderdata==2)
		{
			ForwordSendPermission=0;
			BackwordSendPermission=1;
		}
	}
	if(order=='o')
	{
		if(orderdata==0)
		{
			sprintf(str,"G00000");
		}
		if(orderdata==1)
		{
			sprintf(str,"G00001");
		}
		if(orderdata==2)
		{
			sprintf(str,"G00002");
		}
		//Huart_Send(&Uart,(u8*)str,6);
	}
	if(order=='p')
	{
		if(orderdata==0)
		{
			sprintf(str,"000000");
		}
		if(orderdata==1)
		{
			sprintf(str,"100000");
		}
		//Huart_Send(&Uart,(u8*)str,6);
	}
	if(order=='q')
	{
		if(orderdata==0)
		{
			sprintf(str,"J00000");
		}
		if(orderdata==1)
		{
			sprintf(str,"J00001");
		}
		//Huart_Send(&Uart,(u8*)str,6);
	}
}


/******************************************************************/

