/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */
#include "linkc_network_protocol.h"
#include "Protocol/LinkC_NetWorkProtocol.h"
#include "Csocket.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int16_t check_message(void *Message,uint16_t Recv_Length)
{
    if(Message == NULL || Recv_Length == 0)	return LINKC_FAILURE;
    return ((LMH*)Message)->MessageHeader;
}

int16_t get_message_header(void *Message)
{
    if(Message != NULL)
        return ((LMH*)Message)->MessageHeader;
    else
        printf("Argument is NULL!\n");
    return -1;
}

int16_t pack_message(uint16_t Header,const void *Data,uint16_t Length,void *Out)
{
    ((LMH*)Out)->Version = LINKC_MESSAGE_VERSION;
    ((LMH*)Out)->MessageHeader = Header;
    time(&(((LMH*)Out)->Time));
    ((LMH*)Out)->Totle = 1;
    ((LMH*)Out)->Current = 1;
    if(Length == 0)
    {
        ((LMH*)Out)->MessageLength  = LMH_L;
        return LMH_L;
    }
    ((LMH*)Out)->MessageLength  = Length + LMH_L;
    memcpy((char *)Out+LMH_L,Data,Length);
    return LMH_L+Length;
}

int16_t pack_m_message(uint16_t Header,void *Data,uint16_t Length,void *Out,uint16_t Totle,uint16_t Current)
{
    if(Data == NULL)	return -1;
    ((LMH*)Out)->Version = LINKC_MESSAGE_VERSION;
    ((LMH*)Out)->MessageHeader = Header;
    ((LMH*)Out)->MessageLength  = Length + LMH_L;
    time(&(((LMH*)Out)->Time));
    ((LMH*)Out)->Totle = Totle;
    ((LMH*)Out)->Current = Current;
    memcpy((char *)Out+LMH_L,Data,Length);
    return LMH_L+Length;
}


int16_t unpack_message(void *Message,void *Out)
{
    memcpy(Out,(char *)Message + LMH_L,((LMH*)Message)->MessageLength - LMH_L);
    return 0;
}

int16_t std_m_message_send(void *Message,int sockfd,uint16_t Length)
{
    uint16_t Totle,i;
    if(Length < 8 || Message == NULL)	return -1;
    if(Length <= STD_PACKAGE_SIZE)
    {
        if(send(sockfd,Message,Length,MSG_DONTWAIT) < 0)
            return -1;
    }
    else
    {
        Totle = Length / STD_PACKAGE_SIZE;
        for(i=1;i<=Totle;i++)
            send(sockfd,(char *)Message+(i-1)*STD_PACKAGE_SIZE,STD_PACKAGE_SIZE,MSG_DONTWAIT);
    }
    return LINKC_SUCCESS;
}

int16_t non_std_m_message_send(void *Message,int sockfd,uint16_t Memb,uint16_t Each_Length,uint16_t Header,int Flag)
{
    void *data = new char(Each_Length + Memb + LMH_L);
    uint16_t Totle,i,length;
    Totle =  Memb;
    for(i=1;i<=Totle;i++)
    {
        length = pack_m_message(Header,(char *)Message+(i-1)*Each_Length,Each_Length,data,Totle,i);
        send(sockfd,data,length,Flag);
    }
    delete (char *)data;
    return LINKC_SUCCESS;
}

int16_t non_std_m_message_recv(int Sockfd,int Echo_Size,void *Out){
    void *buff = new char[STD_PACKAGE_SIZE];
    void *data = new char[STD_PACKAGE_SIZE];
    int i,totle,current;
    int length;
    length = recv(Sockfd,buff,STD_PACKAGE_SIZE,0);
    if(check_message(buff,length) < 0){
        delete (char *)buff;
        delete (char *)data;
        return LINKC_FAILURE;
    }
    totle   = ((LinkC_Message_Header*)buff)->Totle;
    current = ((LinkC_Message_Header*)buff)->Current;
    unpack_message(buff,data);
    memcpy((char *)Out+(current-1)*Echo_Size,data,Echo_Size);
    for(i=1;i<totle;i++){
        length = recv(Sockfd,buff,STD_PACKAGE_SIZE,0);
        if(check_message(buff,length) < 0){
            delete (char *)buff;
            delete (char *)data;
            return LINKC_FAILURE;
        }
        current = ((LinkC_Message_Header*)buff)->Current;
        unpack_message(buff,data);
        memcpy((char *)Out+(current-1)*Echo_Size,data,Echo_Size);
    }
    delete (char *)buff;
    delete (char *)data;
    return totle;
}
