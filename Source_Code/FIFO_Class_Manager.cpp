//---------------------------------------------------------------------------

//#pragma hdrstop

#include "FIFO_Class_Manager.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)
MyFifoBuff::MyFifoBuff(DWORD BlockCount, DWORD BlockSize)//构造函数
{
	DWORD memsize;
	memset(&hHandle, 0x00, sizeof(hHandle)); //清空内存
	hHandle.SET_UserBuffBlockCount = BlockCount; //数据块数量
	hHandle.SET_UserDataBlockSize = BlockSize; //每个数据块的大小

	if(((BlockCount * BlockSize) % 4) == 0)//计算能否被4整除
	{
		memsize = (BlockCount * BlockSize) / 4; //那么分配的内存就是4的倍数
	}
	else
	{
		memsize = ((BlockCount * BlockSize) / 4) + 1;
		//如果不能被4整除，那么增加4个字节，用于内存对齐
	}
	hHandle.pData = new DWORD[memsize]; //申请内存
	hHandle.pSET_UserDataBuff = (byte *)hHandle.pData;
	InitializeCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
MyFifoBuff::~MyFifoBuff(void)//析构函数
{
	delete[]hHandle.pData; //释放内存
	DeleteCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
DWORD MyFifoBuff::GetDataLen(void)//取得已存入的数据块数量
{
	DWORD temp;
	EnterCriticalSection(&hCs); //线程互斥
	temp = hHandle.DataManager.DataLen;
	LeaveCriticalSection(&hCs);
	return (temp);
}

//------------------------------------------------------------------------------
void MyFifoBuff::InserData(void *Data)//插入数据
{
	DWORD index;
	byte *pData = (byte *)Data;

	EnterCriticalSection(&hCs); //线程互斥
	if(hHandle.DataManager.DataLen >= hHandle.SET_UserBuffBlockCount)
		//如果缓冲区满，则丢弃数据
	{
		LeaveCriticalSection(&hCs);
		return;
	}

	if((hHandle.DataManager.End + 1) > hHandle.SET_UserBuffBlockCount)
		//如果存入的数据达到了数据尾
	{
		hHandle.DataManager.End = 0; //那么数据从头开始存入
	}

	//复制要插入的数据到缓冲区
	for(index = 0; index < hHandle.SET_UserDataBlockSize; index++)
	{
		hHandle.pSET_UserDataBuff[(hHandle.DataManager.End * hHandle.SET_UserDataBlockSize) + index] = *pData++ ;
	}
	hHandle.DataManager.End++ ;
	hHandle.DataManager.DataLen++ ;
	LeaveCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
void MyFifoBuff::GetData(void *Data)//取得数据
{
	DWORD index;
	byte *pData = (byte *)Data;

	EnterCriticalSection(&hCs); //线程互斥
	if(hHandle.DataManager.DataLen == 0)//数据长度为空，则返回错误信息
	{
		LeaveCriticalSection(&hCs); //离开
		return;
	}

	for(index = 0; index < hHandle.SET_UserDataBlockSize; index++)//取出用户数据
	{
		*pData++ = hHandle.pSET_UserDataBuff[(hHandle.DataManager.Head * hHandle.SET_UserDataBlockSize) + index];
	}

	if(hHandle.DataManager.Head < (hHandle.SET_UserBuffBlockCount - 1))
		//如果到了数据尾
	{
		hHandle.DataManager.Head++ ; //否则指针指向下一个逻辑地
	}
	else
	{
		hHandle.DataManager.Head = 0; //那么从头开始
	}
	hHandle.DataManager.DataLen-- ;
	LeaveCriticalSection(&hCs);
}

//------------------------------------------------------------------------------
void MyFifoBuff::ClearData(void)//清空存入的数据
{
	memset(&hHandle.DataManager, 0x00, sizeof(hHandle.DataManager));
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
