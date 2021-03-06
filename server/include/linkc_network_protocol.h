/*
 * Author           ： Junfeng Zhang <564691478@qq.com>
 * Last-Change      ： April 5 , 2014
 */

#ifndef LINKC_NETWORK_PROTOCOL_H
#define LINKC_NETWORK_PROTOCOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "def.h"
#include "linkc_types.h"

#define LINKC_STATUS
#define LINKC_SUCCESS   0
#define LINKC_FAILURE   -1
#define LINKC_LIMITED   -2

/* 系统 */
#define MESSAGE_POOL_SIZE       15      // 发送消息池的大小
#define STD_PACKAGE_SIZE        512     // 标准包最大大小
#define STD_BUFFER_SIZE         1024    // 标准缓冲区大小
#define MAX_BUFFER_SIZE         10240   // 最大缓冲区大小
#define END_OF_LINKC_MESSAGE    0X011B  // Esc键 汗!
#define LINKC_MESSAGE_VERSION   1       // 协议版本
#define ALL_FRIENDS             0       // 表示所有好友
#define HEART_BEATS_TIME        5       // 心跳时间[感觉有歧义]

/* Error_Code 定义区 [Check_Message里]*/
#define DIFF_VERSION            -3      // 协议版本不一致
#define EXCEED_THE_LIMIT        -2      // 大小超出限制
#define NOT_MESSAGE             -1      // 非消息
#define MESSAGE_INCOMPLETE      1       // 数据不完整
#define OVER_RECV               2       // 收到的数据大于一个包，并且前面的是一个完整的包


/* 消息头[LinkC_Message_Header] */
#define HEART_BEATS             0       // 心跳包
#define EXIT                    1       // 退出
#define CONNECTION              2       // 连接
#define SYS_MESSAGE_PUSHING     3       // 系统消息推送
#define SYS_ACTION_STATUS       4       // 系统操作状态
#define SYS_FRIEND_DATA         5       // 好友资料
#define SYS_FREIDNS_DATA        6
#define SYS_USER_INFO           7       // 用户自己的资料
#define LOGIN                   8       // 登录
//-------分割--------//
#define USER_REQUEST            9       // 用户请求 [扩展中]
#define USER_MESSAGE            10      // 好友之间的信息[扩展中]
#define USER_CHAT_MESSAGE       11      //

/* 用户请求[LinkC_User_Request] */
#define USER_LOGOUT             1       // 登出
#define USER_STATUS_HIDDEN      2       // 状态-隐身
#define USER_STATUS_ONLINE      3       // 状态-在线
#define USER_FRIEND_DATA        4       // 请求好友数据
#define USER_FRIENDS_DATA       5
#define USER_CHAT_REQUEST       6       // 
#define USER_INFO_REQUEST       7       // 用户请求自身的数据

/* 用户消息[LinkC_User_Message]*/
#define USER_CHAT               1       // 聊天
#define USER_ACCEPT             2       // 接受
#define USER_REFUSE             3       // 拒绝
#define USER_ADD_FRIEND         4       // 添加好友
#define USER_DEL_FRIEND         5       // 删除好友
#define USER_CONNECT_READY      6       // P2P连接准备完毕

struct LinkC_Message_Header_t
{
    uint8_t  Version;                   // 协议版本
    uint8_t  Totle;                     // 总包数
    uint8_t  Current;                   // 当前包标记
    uint8_t  MessageHeader;             // 服务类型
    uint16_t MessageLength;             // 数据总长度
    time_t   Time;                      // 时间戳
};
struct LinkC_User_Request_t{
    uint16_t Action;
    uint16_t Flag;                      // 补充说明操作情况，省略值为0
    uint32_t UID;                       // 若是自己则UID为0
};
struct LinkC_Sys_Status_t{
    uint16_t Action;
    int16_t  Status;
};
struct LinkC_Msg_Buf_t{
    char buffer[1024];
    int type;                           // 数据类型
    int byte;                           // 接收到的数据长度
    int flag;                           // flag = 1 时表示还有数据，= 0时表示无数据
    int offset;
};
struct LinkC_User_Message_t{
    uint32_t SrcUID;
    uint16_t Action;
};

typedef struct LinkC_Message_Header_t   LinkC_Message_Header;
typedef struct LinkC_User_Request_t     LinkC_User_Request;
typedef struct LinkC_User_Message_t     LinkC_User_Message;
typedef struct login_data               LinkC_User_Login;
typedef struct LinkC_Sys_Status_t       LinkC_Sys_Status;
typedef struct LinkC_Friend_Data_t      LinkC_Friend_Data;
typedef struct user_info                LinkC_User_Info;

#define MESSAGE_HEADER_LENGTH       sizeof(struct LinkC_Message_Header_t)
#define USER_REQUEST_LENGTH         sizeof(struct LinkC_User_Request_t)
#define USER_MESSAGE_LENGTH         sizeof(struct LinkC_User_Message_t)
#define USER_LOGIN_LENGTH           sizeof(struct login_data)
#define SYS_STATUS_LENGTH           sizeof(struct LinkC_Sys_Status_t)
#define SYS_FRIEND_DATA_LENGTH      sizeof(struct friend_data)
#define SYS_USER_INFO_LENGTH        sizeof(struct user_info)

// 缩写一下，啊哈哈哈哈哈
#define LMH     LinkC_Message_Header
#define LUR     LinkC_User_Request
#define LUM     LinkC_User_Message
#define LUL     LinkC_User_Login
#define LSS     LinkC_Sys_Status
#define LFD     LinkC_Friend_Data
#define LUI     LinkC_User_Info

#define LMH_L   MESSAGE_HEADER_LENGTH
#define LUR_L   USER_REQUEST_LENGTH
#define LUM_L   USER_MESSAGE_LENGTH
#define LUL_L   USER_LOGIN_LENGTH
#define LSS_L   SYS_STATUS_LENGTH
#define LFD_L   SYS_FRIEND_DATA_LENGTH
#define LUI_L   SYS_USER_INFO_LENGTH
// 缩写 End

int16_t check_message           (void *Message,uint16_t Recv_Length);
int16_t pack_message            (uint16_t Header, const void *Data, uint16_t Length, void *Out);
int16_t pack_m_message          (uint16_t Header,void *Data,uint16_t Length,void *Out,uint16_t Totle,uint16_t Current);
int16_t unpack_message          (void *Message,void *Out);
int16_t std_m_message_send      (void *Message,int sockfd,uint16_t Length);
int16_t non_std_m_message_send  (void *Message,int sockfd,uint16_t Memb,uint16_t Each_Length,uint16_t Header,int Flag);
int16_t non_std_m_message_recv  (int Sockfd,int Echo_Size,void *Out);
int16_t get_message_header      (void *Message);
int16_t TCP_Send(int sockfd, void *buf, int buf_size, int flag);
int16_t TCP_Recv(int sockfd, void *out, int out_size, int flag);


#endif
