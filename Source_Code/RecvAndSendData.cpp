//---------------------------------------------------------------------------

#pragma hdrstop

#include "RecvAndSendData.h"

extern "C"
{
#include "setupapi.h"
#include "hiddll.h"
}
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
MyRwHid::MyRwHid(HWND hFormHandle)//构造函数
{
	memset(&RecvDataBuff, 0x00, sizeof(RecvDataBuff)); //清空缓冲区
	memset(&SendDataBuff, 0x00, sizeof(SendDataBuff));
	memset(&ConnectType, 0x00, sizeof(ConnectType));
	hMainFormHandle = hFormHandle; //主窗口的句柄

	//接收线程基本参数
	InitializeCriticalSection(&RecvDataBuff.hCs); //初始化互斥体
	RecvDataBuff.hOl.hEvent = CreateEvent(0, false, false, NULL); //创建一个事件
	RecvDataBuff.hMainFormHandle = hMainFormHandle;
	//创建一个接收队列缓冲区
	RecvDataBuff.RecvFifoBuff = new MyFifoBuff(_DEF_USB_RECV_DATA_FIFO_BUFF_BLOCK_COUNT, _DEF_RECV_DATA_PACK_SIZE);

	//发送线程基本参数
	InitializeCriticalSection(&SendDataBuff.hCs); //初始化互斥体
	SendDataBuff.hOl.hEvent = CreateEvent(0, false, false, NULL); //创建一个事件
	SendDataBuff.hMainFormHandle = hMainFormHandle;
	SendDataBuff.SendFifoBuff = new MyFifoBuff(_DEF_USB_RECV_DATA_FIFO_BUFF_BLOCK_COUNT, _DEF_RECV_DATA_PACK_SIZE);
}

//---------------------------------------------------------------------------
MyRwHid::~MyRwHid(void)//析构函数
{
	/*
	 if (WaitForSingleObject(hRecvThread, 200) != WAIT_FAILED) // 等待接收数据线程退出 INFINITE
	 {
	 CloseHandle(hRecvThread);
	 }
	 if (WaitForSingleObject(hSendThread, 200) != WAIT_FAILED) // 等待发送数据线程退出
	 {
	 CloseHandle(hSendThread);
	 }
	 */
	DeleteCriticalSection(&SendDataBuff.hCs);
	DeleteCriticalSection(&RecvDataBuff.hCs);
	delete RecvDataBuff.RecvFifoBuff;
}

//---------------------------------------------------------------------------
void MyRwHid::StartRecvThread(void)//启动接收线程
{

	if(ConnectType.ConnectType == _DEF_USB_CONNECT_TYPE_NONE)
	{
		return;
	}//如果未连接，则不会启动线程

	if(WaitForSingleObject(hRecvThread, INFINITE) != WAIT_FAILED)
	{
		CloseHandle(hRecvThread);
	}
	RecvDataBuff.hHidHandle = ConnectType.hHidHandle; //句柄
	hRecvThread = CreateThread(NULL, 0, USB_Thread_RecvData, (VOID *) &RecvDataBuff, 0, NULL);

}

//---------------------------------------------------------------------------
void MyRwHid::GetRecvDataFromFifoBuff(byte *ReportID, void *Data)
	//读取收到的数据，从FIFO缓冲区
{
	struct HID_DEF_DATA_PACK_65BYTE Pack;

	RecvDataBuff.RecvFifoBuff->GetData(&Pack); //从缓冲区中取出数据

	*ReportID = Pack.ReportID; //返回报告ID
	memcpy(Data, Pack.PackData, sizeof(Pack.PackData)); //复制数据出来
}

//---------------------------------------------------------------------------
DWORD MyRwHid::GetRecvDataLen(void)//获取缓冲区中的数据数量，0=缓冲区为空
{
	DWORD DataLen;
	DataLen = RecvDataBuff.RecvFifoBuff->GetDataLen();
	return (DataLen);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void MyRwHid::StartSendThread(void)//启动发送线程
{

	if(ConnectType.ConnectType == _DEF_USB_CONNECT_TYPE_NONE)
	{
		return;
	}//如果未连接，则不会启动线程


	if(WaitForSingleObject(hSendThread, 500) != WAIT_FAILED)		//等待上次发送的线程结束 INFINITE
	{
		CloseHandle(hSendThread); //关闭线程的句柄
	}

	SendDataBuff.hHidHandle = ConnectType.hHidHandle; //设置HID_Device的句柄
	SendDataBuff.IsSend = 1; //设置为正在发送状态
	hSendThread = CreateThread(NULL, 0, USB_Thread_SendData, (VOID *) &SendDataBuff, 0, NULL);

}

//---------------------------------------------------------------------------
void MyRwHid::SendDataByFifo(byte ReportID, void *Data)//插入要发送的数据，必须是64字节
{
	struct HID_DEF_DATA_PACK_65BYTE Pack;

	if(ConnectType.ConnectType == _DEF_USB_CONNECT_TYPE_NONE)
	{
		return;
	}//如果未连接，则不会把数据放入缓冲区，数据被丢失

	Pack.ReportID = ReportID; //设置报告ID
	memcpy(&Pack.PackData, Data, sizeof(Pack.PackData)); //复制要发送的数据到缓冲区
	SendDataBuff.SendFifoBuff->InserData(&Pack); //把数据插入缓冲区
}

//---------------------------------------------------------------------------
byte MyRwHid::GetSendStatus(void)//获取发送状态1=正在发送，0=发送完毕
{
	byte status;
	EnterCriticalSection(&SendDataBuff.hCs); //进入线程互斥
	status = SendDataBuff.IsSend;
	LeaveCriticalSection(&SendDataBuff.hCs); //离开线程互斥
	return (status);
}

//---------------------------------------------------------------------------
void MyRwHid::StopRecvThread(void)//停止读写线程
{
	EnterCriticalSection(&RecvDataBuff.hCs); //进入互斥
	RecvDataBuff.IsEndRun = 1; //立即结束线程的运行
	LeaveCriticalSection(&RecvDataBuff.hCs); //离开互斥
	SetEvent(RecvDataBuff.hOl.hEvent); //人工设置为有信号，迫使阻塞函数停止阻塞
	Sleep(50);

}

//---------------------------------------------------------------------------
void MyRwHid::StopSendThread(void)//停止写线程
{
	EnterCriticalSection(&SendDataBuff.hCs); //进入互斥
	SendDataBuff.IsEndRun = 1; //立即结束线程的运行
	LeaveCriticalSection(&SendDataBuff.hCs); //离开互斥
	SetEvent(SendDataBuff.hOl.hEvent); //人工设置为有信号，迫使阻塞函数停止阻塞
	Sleep(50);

}

//---------------------------------------------------------------------------
DWORD WINAPI USB_Thread_RecvData(void *lpParam)//读取数据线程
{
	struct _USB_THREAD_RECV_DATA_STRUCT *RecvBuff;
	struct HID_DEF_DATA_PACK_65BYTE Pack;
	DWORD ReadFileRecvSize;
	DWORD LASTERROR;
	DWORD errs;

	RecvBuff = (struct _USB_THREAD_RECV_DATA_STRUCT *)lpParam; //参数

	while(1)
	{
		memset(&Pack, 0x00, sizeof(Pack)); //清空接收缓冲区

		//使用重叠IO读取数据
		ReadFile(RecvBuff->hHidHandle, (byte *) &Pack, sizeof(Pack), &ReadFileRecvSize, &RecvBuff->hOl);
		//等待操作完成
		LASTERROR = GetLastError();

		if(LASTERROR == ERROR_IO_PENDING)
		{
			WaitForSingleObject(RecvBuff->hOl.hEvent, INFINITE); //等待操作完成
		}
		else //句柄已经失效,应该退出线程
		{
			CloseHandle(RecvBuff->hHidHandle); //关闭HID句柄
			//应该通知主线程，连接断开了
			PostMessage(RecvBuff->hMainFormHandle, ON_HID_DISCONNECT, 0, 0);
			//通知主线程，连接已经断开
			break;
		}
		//把收到的数据插入缓冲区
		EnterCriticalSection(&RecvBuff->hCs); //进入互斥
		if(RecvBuff->IsEndRun == 1)//如果要结束接收线程
		{
			LeaveCriticalSection(&RecvBuff->hCs); //离开互斥
			break;
		}
		LeaveCriticalSection(&RecvBuff->hCs); //离开互斥
		RecvBuff->RecvFifoBuff->InserData(&Pack);
		PostMessage(RecvBuff->hMainFormHandle, ON_GET_RECV_DATA, 0, 0);
	}
	return (0);
}

//---------------------------------------------------------------------------
DWORD WINAPI USB_Thread_SendData(void *lpParam)//写入数据线程
{

	struct _USB_THREAD_SEND_DATA_STRUCT *SendBuff = (struct _USB_THREAD_SEND_DATA_STRUCT *)lpParam;
	DWORD WriteStatus; //WriteFile函数返回的数据
	DWORD WriteFileSize; //WriteFile函数写入的字节数
	DWORD SendPack = 0; //已经发送的数据大小
	struct HID_DEF_DATA_PACK_65BYTE Pack; //要发送的数据包

	//发送数据包
	while(1)//直到发送完毕
	{

		if(SendBuff->IsEndRun == 1)//如果要结束线程的发送
		{
			EnterCriticalSection(&SendBuff->hCs); //进入互斥
			SendBuff->IsSend = 0; //发送完毕
			LeaveCriticalSection(&SendBuff->hCs); //离开互斥
			break;
		}

		if(SendBuff->SendFifoBuff->GetDataLen() == 0)//如果数据发送完毕
		{
			EnterCriticalSection(&SendBuff->hCs); //进入互斥
			SendBuff->IsSend = 0; //发送完毕
			LeaveCriticalSection(&SendBuff->hCs); //离开互斥
			break;
		}

		SendBuff->SendFifoBuff->GetData(&Pack); //取出数据
		//发送数据，通过重叠IO

		WriteStatus = WriteFile(SendBuff->hHidHandle, //句柄
			(byte *) &Pack, //数据缓冲区
			sizeof(Pack), //数据大小
			&WriteFileSize, //已发送大小
			&SendBuff->hOl); //使用重叠IO

		if((WriteStatus == false) && (GetLastError() == ERROR_IO_PENDING))
			//检查是否出错
		{
			WaitForSingleObject(SendBuff->hOl.hEvent, INFINITE); //等待数据发送完成
		}
		else //如果句柄已经变的不可用，则立即退出线程
		{
			EnterCriticalSection(&SendBuff->hCs); //进入互斥
			SendBuff->IsSend = 0; //发送完毕
			LeaveCriticalSection(&SendBuff->hCs); //离开互斥
			CloseHandle(SendBuff->hHidHandle); //关闭HID句柄
			//应该通知主线程，连接断开了
			PostMessage(SendBuff->hMainFormHandle, ON_HID_DISCONNECT, 0, 0);
			//通知主线程，连接已经断开
			PostMessage(SendBuff->hMainFormHandle, ON_SEND_DATA_SUCCESS, ON_SEND_DATA_PARA_ERROR, 0); //数据发送失败通知
			return (0); //结束线程
		}
		//发送当前的进度给主线程
		PostMessage(SendBuff->hMainFormHandle, ON_SEND_DATA_SUCCESS, ON_SEND_DATA_PARA_DATA_PROGRESS, ++SendPack);
	}

	PostMessage(SendBuff->hMainFormHandle, ON_SEND_DATA_SUCCESS, ON_SEND_DATA_PARA_DATA_SEND_SUCCESS, 0); //数据发送完成通知
	EnterCriticalSection(&SendBuff->hCs); //进入互斥
	SendBuff->IsSend = 0; //发送完毕
	LeaveCriticalSection(&SendBuff->hCs); //离开互斥
	return (0);
}

//---------------------------------------------------------------------------
bool MyRwHid::QueryMyHidDevice(void)//查询HID设备
{
	GUID hidGuid;
	String DeviceString;
	HDEVINFO hDevInfo;
	ULONG requiredLength = 0;
	int deviceNo = 0;

	if(ConnectType.ConnectType != 0)//如果已经连接，则不再连接
	{
		return (false);
	}
	HidD_GetHidGuid(&hidGuid);
	hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

	if(hDevInfo == INVALID_HANDLE_VALUE)//查询失败
	{
		return (false);
	}
	SP_DEVICE_INTERFACE_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	deviceNo = 0;
	SetLastError(0);
	while(GetLastError() != ERROR_NO_MORE_ITEMS)//如果没有更多的HID设备，则退出循环
	{
		if(SetupDiEnumInterfaceDevice(hDevInfo, 0, &hidGuid, deviceNo, &devInfoData))//枚举系统内的USB HID设备
		{
			requiredLength = 0;
			SetupDiGetInterfaceDeviceDetail(hDevInfo, &devInfoData, NULL, 0, &requiredLength, NULL); //获取需要的数据空间大小
			PSP_INTERFACE_DEVICE_DETAIL_DATA devDetail = (SP_INTERFACE_DEVICE_DETAIL_DATA *)malloc(requiredLength);
			//动态分配内存
			devDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
			if(!SetupDiGetInterfaceDeviceDetail(hDevInfo, &devInfoData, devDetail, requiredLength, NULL, NULL))//再次调用，以获取有用的数据
			{
				free(devDetail); //失败了就释放分配的内存
				SetupDiDestroyDeviceInfoList(hDevInfo);
				return (false); //返回
			}
			DeviceString = devDetail->DevicePath; //成功了，获取设备的绝对路径
			if((DeviceString.Pos(_DEF_USB_DEVICE_KEYBOARD_FW_STRING) != 0) || (DeviceString.Pos(_DEF_USB_DEVICE_BOOTLOADER_FW_STRING) != 0))
				//查找设备ID，如果不对，就继续下一个。。。
			{
				ConnectType.hHidHandle = CreateFile(devDetail->DevicePath, //重叠IO模式打开设备
					GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			}
			else
			{
				++deviceNo; //如果没有需要的设备，则继续下一个
				continue;
			}

			free(devDetail); //释放设备信息分配的内存
			if(ConnectType.hHidHandle == INVALID_HANDLE_VALUE)//如果设备打开失败
			{
				SetupDiDestroyDeviceInfoList(hDevInfo);
				free(devDetail);
				return (false);
			}
			_HIDD_ATTRIBUTES hidAttributes;
			if(!HidD_GetAttributes(ConnectType.hHidHandle, &hidAttributes))
				//获取设备的属性
			{
				CloseHandle(ConnectType.hHidHandle);
				SetupDiDestroyDeviceInfoList(hDevInfo);
				return (false);
			}
			if((_DEF_USB_DEVICE_VID == hidAttributes.VendorID) && (_DEF_USB_DEVICE_PID_KEYBOARD_FW == hidAttributes.ProductID))//判断是KB_FW
			{
				ConnectType.ConnectType = _DEF_USB_CONNECT_TYPE_KEYBOARD_FW;
				//已连接键盘固件
				SetupDiDestroyDeviceInfoList(hDevInfo);
				//如果是需要的，那么释放保存设备信息所分配的内存
				return true; //返回正确
			}
			if((_DEF_USB_DEVICE_VID == hidAttributes.VendorID) && (_DEF_USB_DEVICE_PID_BOOTLOADER_FW == hidAttributes.ProductID))//判断是BootLoader_FW
			{
				ConnectType.ConnectType = _DEF_USB_CONNECT_TYPE_BOOTLOADER;
				//已连接键盘固件
				SetupDiDestroyDeviceInfoList(hDevInfo);
				//如果是需要的，那么释放保存设备信息所分配的内存
				return true; //返回正确
			}

			//都不是，则继续下一个
			CloseHandle(ConnectType.hHidHandle);
			//否则关闭打开后的句柄，继续查询下一个USB HID设备的信息
			++deviceNo;
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfo); //如果没有找到任何需要的USB HID设备句柄，就释放分配的内存
	return false; //返回无效的句柄
}

//---------------------------------------------------------------------------
byte MyRwHid::GetConnectType(void)//获取连接类型
{
	return (ConnectType.ConnectType);
}

//---------------------------------------------------------------------------
void MyRwHid::SetDisConnect(void)//设置为断开连接
{
	ConnectType.ConnectType = _DEF_USB_CONNECT_TYPE_NONE;
	ConnectType.hHidHandle = NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
