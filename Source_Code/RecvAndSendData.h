// ---------------------------------------------------------------------------

#ifndef RecvAndSendDataH
#define RecvAndSendDataH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>

#include "FIFO_Class_Manager.h"

// USB设备号定义
#define		_DEF_USB_DEVICE_VID													0x0483		//设备ID
#define		_DEF_USB_DEVICE_PID_KEYBOARD_FW										0x8601		//键盘的PID
#define		_DEF_USB_DEVICE_PID_BOOTLOADER_FW									0x8602		//BootLoader的PID
#define		_DEF_USB_DEVICE_KEYBOARD_FW_STRING									"vid_0483&pid_8601&mi_02"//键盘的字符串
#define     _DEF_USB_DEVICE_BOOTLOADER_FW_STRING                                "vid_0483&pid_8602"  //BootLoader的字符串

// 设备类型定义
#define		_DEF_USB_CONNECT_TYPE_NONE											0x00		//未连接
#define		_DEF_USB_CONNECT_TYPE_BOOTLOADER									0x01		//连接到了BootLoader
#define		_DEF_USB_CONNECT_TYPE_KEYBOARD_FW									0x02		//连接了键盘固件

// 消息
#define  	ON_GET_RECV_DATA  (WM_USER+103) 												//收据收到消息
#define  	ON_HID_DISCONNECT (WM_USER+104) 												//hid连接被断开
#define		ON_SEND_DATA_SUCCESS (WM_USER+105)												//线程数据发送完成

// 消息返回的代码WParam
#define		ON_SEND_DATA_PARA_SUCCESS											0			//发送数据成功
#define		ON_SEND_DATA_PARA_ERROR												1			//发送数据失败
#define		ON_SEND_DATA_PARA_DATA_SEND_SUCCESS									2			//数据发送完成
#define		ON_SEND_DATA_PARA_DATA_PROGRESS										3			//数据发送的进度

// 宏定义
#define   	_DEF_RECV_DATA_PACK_SIZE                                            65      	//每个数据包最大64+1字节
#define		_DEF_RECV_DATA_SIZE													64			//有效数据
#define   	_DEF_RW_DATA_PACK_SIZE 					                            56      	//每个数据包最大有效数据搭载量

// FIFO缓冲区大小
#define		_DEF_USB_RECV_DATA_FIFO_BUFF_BLOCK_COUNT							2048	//数据接收线程的FIFO缓冲区大小
#define		_DEF_USB_SEND_DATA_FIFO_BUFF_BLOCK_COUNT							2048	//数据发送线程的FIFO缓冲区大小

// 使用的数据结构
#pragma pack(1)           //1字节对齐

struct HID_DEF_DATA_PACK_65BYTE
{
	uint8_t ReportID; // USB报告ID
	uint8_t PackData[_DEF_RECV_DATA_SIZE]; // 数据包的数据
};

// 收发数据使用的数据结构
struct _USB_THREAD_RECV_DATA_STRUCT // 接收数据线程使用的
{
	HANDLE hHidHandle; // HID设备句柄
	HWND hMainFormHandle; // 主窗口的句柄
	bool IsEndRun; // 线程是否需要结束，1=是，0=否
	CRITICAL_SECTION hCs; // 线程互斥
	OVERLAPPED hOl; // 异步事件
	MyFifoBuff *RecvFifoBuff; // FIFO数据管理类
};

struct _USB_THREAD_SEND_DATA_STRUCT // 发送数据线程使用的
{
	HANDLE hHidHandle; // HID设备句柄
	HWND hMainFormHandle; // 主窗口句柄
	bool IsEndRun; // 线程是否需要结束，1=是，0=否
	CRITICAL_SECTION hCs; // 线程互斥
	OVERLAPPED hOl; // 重叠IO事件
	bool IsSend; // 正在发送=true，发送完毕=false
	MyFifoBuff *SendFifoBuff; // FIFO数据管理类
};

#pragma pack()

// 线程函数
// 读数据
DWORD WINAPI USB_Thread_RecvData(void*lpParam); // 读取数据线程
DWORD WINAPI USB_Thread_SendData(void*lpParam); // 写入数据线程

class MyRwHid
{
private:
	// 数据收发缓冲区
	HWND hMainFormHandle; // 主窗口句柄
	HANDLE hRecvThread; // 读线程句柄
	HANDLE hSendThread; // 发送数据线程

	// 写线程句柄
	struct
	{
		// 连接相关的结构体
		byte ConnectType; // 连接类型0=未连接，1=Bootloader，2=KeyBoard_Fw
		HANDLE hHidHandle; // 连接句柄
	}

	ConnectType;

	struct _USB_THREAD_RECV_DATA_STRUCT RecvDataBuff;
	struct _USB_THREAD_SEND_DATA_STRUCT SendDataBuff;

public:

	MyRwHid(HWND hFormHandle); // 构造函数
	~MyRwHid(void); // 析构函数

	// 数据接收管理
	void StartRecvThread(void); // 启动接收线程
	void GetRecvDataFromFifoBuff(byte *ReportID, void *Data);
	// 读取收到的数据，从FIFO缓冲区
	DWORD GetRecvDataLen(void); // 获取缓冲区中的数据数量，0=缓冲区为空

	// 发送管理
	void StartSendThread(void); // 启动发送线程
	void SendDataByFifo(byte ReportID, void *Data); // 插入要发送的数据，必须是64字节
	byte GetSendStatus(void); // 获取发送状态1=正在发送，0=发送完毕

	// 线程管理
	void StopRecvThread(void); // 停止读线程
	void StopSendThread(void); // 停止写线程

	// 查询以及连接HID设备
	bool QueryMyHidDevice(void); // 查询HID设备
	byte GetConnectType(void); // 获取连接类型
	void SetDisConnect(void); // 设置为断开连接

private:

};

#endif
