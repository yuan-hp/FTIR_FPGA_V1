#ifndef SRC_HLWIP_H_
#define SRC_HLWIP_H_


#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

#endif

void print_ip(char *msg, ip_addr_t *ip);
void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
int Hlwip_init(void);
void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
void LinTcpSend(u32 length);//默认source为int或float占4字节
void LinTcpSendBack(u32 length);











