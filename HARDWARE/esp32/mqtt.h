#ifndef __MQTT_H
#define __MQTT_H

#define  BUFF_UNIT           512    //缓冲区长度
#define  R_NUM               8     //接收缓冲区个数
#define  T_NUM               8     //发送缓冲区个数  
#define  C_NUM               8     //命令缓冲区个数

#define  MQTT_TxData(x)       u2_TxData(x)                                  //串口2负责800C的数据发送

#define  PRODUCTKEY           "183.230.40.39"                                 
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                            //产品ID长度
#define  DEVICENAME           "927136809"                                        //设备名  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                            //设备名长度
#define  DEVICESECRE          "1e5b7dbbe0776aa1b3c29758f390e0a9"            //设备秘钥   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                           //设备秘钥长度
#define  S_TOPIC_NAME         "/sys/a1dGO9gVhzW/jieerzi001/thing/service/property/set"   //需要订阅的主题  
#define  P_TOPIC_NAME         "$dp"    //需要发布的主题     
#define  PRODUCTID 						"503697"
#define  AUTHENTICATION       "123456"
extern unsigned char  MQTT_RxDataBuf[R_NUM][BUFF_UNIT];        //外部变量声明，数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_RxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_RxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_RxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_TxDataBuf[T_NUM][BUFF_UNIT];        //外部变量声明，数据的发送缓冲区,所有发往服务器的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_TxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_TxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_TxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_CMDBuf[C_NUM][BUFF_UNIT];           //外部变量声明，命令数据的接收缓冲区
extern unsigned char *MQTT_CMDInPtr;                           //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_CMDOutPtr;                          //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_CMDEndPtr;                          //外部变量声明，指向缓冲区结束的位置

extern char ClientID[128];     //外部变量声明，存放客户端ID的缓冲区
extern int  ClientID_len;      //外部变量声明，存放客户端ID的长度
extern char Username[128];     //外部变量声明，存放用户名的缓冲区
extern int  Username_len;	   //外部变量声明，存放用户名的长度
extern char Passward[128];     //外部变量声明，存放密码的缓冲区
extern int  Passward_len;	   //外部变量声明，存放密码的长度
extern char ServerIP[128];     //外部变量声明，存放服务器IP或是域名
extern int  ServerPort;        //外部变量声明，存放服务器的端口号

extern char Ping_flag;         //外部变量声明，ping报文状态      0：正常状态，等待计时时间到，发送Ping报文
                               //外部变量声明，ping报文状态      1：Ping报文已发送，当收到 服务器回复报文的后 将1置为0
extern char Connect_flag;      //外部变量声明，同服务器连接状态  0：还没有连接服务器  1：连接上服务器了
extern char ReConnect_flag;    //外部变量声明，重连服务器状态    0：连接还存在  1：连接断开，重连
extern char ConnectPack_flag;  //外部变量声明，CONNECT报文状态   1：CONNECT报文成功
extern char SubcribePack_flag; //外部变量声明，订阅报文状态      1：订阅报文成功

void MQTT_Buff_Init(void);
void MQTT_Buff_ReInit(void);
void OneNetIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *, int);
void MQTT_PingREQ(void);
void MQTT_PublishQs0(char *, char *, int);
void MQTT_DealPushdata_Qs0(unsigned char *);	
void TxDataBuf_Deal(unsigned char *, int);
void CMDBuf_Deal(unsigned char *, int);

#endif
