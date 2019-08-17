// ---------------------------------------------------------------------------

#ifndef FIFO_Class_ManagerH
#define FIFO_Class_ManagerH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// #include <Controls.hpp>
// #include <StdCtrls.hpp>
// #include <Forms.hpp>
// #include <ExtCtrls.hpp>
// #include <Buttons.hpp>
// #include <ComCtrls.hpp>

#define		MY_FIFO_DEF_SUCCESS						                           	0x00			//操作成功
#define		MY_FIFO_DEF_BUFF_NULL						                        0x01			//缓冲区为空
#define		MY_FIFO_DEF_BUFF_FULL						                        0x02			//缓冲区已满
#define		MY_FIFO_DEF_BUFF_ERROR						                        0x03			//缓冲区设置错误

struct MY_FIFO_DATA_MANAGER_STRUCT
{
	DWORD SET_UserBuffBlockCount; // 设置用户的缓冲区有多少数据块
	DWORD SET_UserDataBlockSize; // 设置用户的数据块的大小(字节)
	byte *pSET_UserDataBuff; // 指针，指向用户的数据缓冲区
	DWORD *pData; // 临时使用的
	DWORD ErrorCode; // 保存上一次操作后的错误代码

	struct
	{
		DWORD Head; // 数据头的指针
		DWORD End; // 数据尾的指针
		DWORD DataLen; // 缓冲区内数据组的个数，这个长度*用户每一组数据的大小，才是真正的字节数
	}

	DataManager; // 数据管理器
};

class MyFifoBuff
{
private:

	struct MY_FIFO_DATA_MANAGER_STRUCT hHandle;
	CRITICAL_SECTION hCs; // 线程互斥

public:
	MyFifoBuff::MyFifoBuff(DWORD BlockCount, DWORD BlockSize); // 构造函数
	MyFifoBuff::~MyFifoBuff(void); // 析构函数

	void MyFifoBuff::InserData(void *Data); // 插入数据
	void MyFifoBuff::GetData(void *Data); // 取得数据
	DWORD MyFifoBuff::GetDataLen(void); // 取得已存入的数据块数量
	void MyFifoBuff::ClearData(void); // 清空存入的数据
};

#endif
