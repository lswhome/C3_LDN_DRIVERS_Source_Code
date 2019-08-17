//---------------------------------------------------------------------------

#include <vcl.h>
#include "inifiles.hpp"
#pragma hdrstop
#include "inifiles.hpp"
#include "Unit1.h"
#include "FIFO_Class_Manager.h"
#include "FuncSelect.h"
#include "MyListView.h" //列表控件
#include "NumSelect.h"  //数字选择
#include "TimeSelect.h" //时间选择
#include "LedSelect.h"  //灯珠选择
#include <System.SysUtils.hpp>
#include <System.IOUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Vcl.WinXPanels"
#pragma resource "*.dfm"

TMainForm *MainForm; //主窗口
MyRwHid *MyHid; //USB通信
MyListView *FnKeyEditer; //fn键编辑器
MyListView *ComboKeysEditer; //组合键编辑器句柄
MyListView *nHitKeysEditer; //连击键编辑器
MyListView *two1KeysEditer; //二合一键的编辑器
MyListView *HdKeyEditer; //保持键编辑器

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent *Owner) : TForm(Owner)
{
	//让窗口居中
	ThemesInit(); //皮肤初始化
	MyHid = new MyRwHid(MainForm->Handle); //启动HID驱动
	IsCloseApplication = 0; //1=将要关闭程序
	MainForm->Caption = "C3-LDN KeyBoard Config Tools V1.0(Disconnect)";
	Application->Title = "C3-LDN KeyBoard Config Tools V1.0";
	MainForm->Left = (Screen->Width - MainForm->Width) / 2;
	MainForm->Top = (Screen->Height - MainForm->Height) / 2;

	memset(&CpRun, 0, sizeof(CpRun));
	memset(&PhyCfg, 0, sizeof(PhyCfg));

	LoadLanguageFiles(); //语言初始化

	LoadMediaKeyCode(); //加载多媒体按键的键码列表
	LoadNormalKeyCode(); //加载普通键帽的列表

	InitNormalLayer(); //常规层数据初始化

	//触发层编辑器
	FnDispEditer(); //FN键编辑器
	ComboKeyDispEditer(); //组合键编辑器
	nHitKeyDispEditer(); //连击键编辑器
	two1KeyDispEditer(); //创建二合一键的编辑器
	HoldDownKeyDispEditer(); //保持键
	//功能层管理
	FlayerSetPoint(); //设置关联性
	MacroInitEditer(); //初始化宏的编辑器
	KeyLockInitEditer(); //锁定键编辑器

	//WS2812配置
	CpRun.BklConfig.WSLedRun.img[0] = Image4;
	CpRun.BklConfig.WSLedRun.img[1] = Image5;
	CpRun.BklConfig.WSLedRun.img[2] = Image6;
	CpRun.BklConfig.WSLedRun.img[3] = Image7;

	CpRun.BklConfig.WSLedRun.rbtn[0] = RadioButton23;
	CpRun.BklConfig.WSLedRun.rbtn[1] = RadioButton19;
	CpRun.BklConfig.WSLedRun.rbtn[2] = RadioButton20;
	CpRun.BklConfig.WSLedRun.rbtn[3] = RadioButton21;
	CpRun.BklConfig.WSLedRun.rbtn[4] = RadioButton22;

	Timer1->Interval = 1000;
	Timer1->Enabled = true; //启动定时器
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
	//定时器
	if(MyHid->GetConnectType() != 0)//如果HID已连接，则直接返回
	{
		return;
	}

	//出现错误，不再连接
	//if(CpRun.DeviceVerFlag.IsFail != 0) return;

	if(MyHid->QueryMyHidDevice() == true)
	{
		//如果连接正常
		Timer1->Enabled = false; //停止定时器
		switch(MyHid->GetConnectType())
		{
			case _DEF_USB_CONNECT_TYPE_KEYBOARD_FW:
			{
				//MainForm->Caption = "C3-LDN 键盘配置工具 V1.0(KeyBoard 已连接)";
				MyHid->StartRecvThread(); //启动接收线程
				SendCmd(USB_DEF_CMD_GET_HW_FW_INFO, 0x00, 0x00);
			} break;
			case _DEF_USB_CONNECT_TYPE_BOOTLOADER: //BootLoader固件
			{
				//MainForm->Caption = "C3-LDN 键盘配置工具 V1.0(BootLoader 已连接)";
				MyHid->StartRecvThread(); //启动接收线程
				SendCmd(_DEF_USB_RECV_NEED_HW_INFO, 0x00, 0x00);
			} break;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::On_HidDisConnect(Messages::TMessage &Message)
{
	//HID连接被断开
	MyHid->SetDisConnect(); //设置为断开连接方式

	MainForm->Caption = "C3-LDN KeyBoard Config Tools V1.0(Disconnect)";
	Timer1->Enabled = true;
	TForm::Dispatch(&Message);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::On_GetRecvData(Messages::TMessage &Message)
	//收到了数据接收完成的消息
{

	union CP_UNION_ONE_PACK_64BYTE RecvPacks; //当前数据包缓冲区
	byte RePortID;

	if(IsCloseApplication == 1)//如果程序要关闭，则什么也不做了
	{
		TForm::Dispatch(&Message);
		return;
	}

	MyHid->GetRecvDataFromFifoBuff(&RePortID, &RecvPacks);

	if(MyHid->GetConnectType() == _DEF_USB_CONNECT_TYPE_BOOTLOADER)//BootLoader固件
	{
		switch(RecvPacks.Cmd.Cmd)
		{
			case _DEF_USB_SEND_HW_INFO: //设备信息
			{
				GetBootLoaderInfo(&RecvPacks);
			} break;
			case _DEF_USB_SEND_ERASE_FLASH_DONE: //FLASH擦除完毕
			{
				Label26->Caption = LanguageGetStr(200);
			} break;
			case _DEF_USB_SEND_ERASE_W25Q_DONE: //W25Q擦除完毕
			{
				Label26->Caption = LanguageGetStr(201);
			} break;
			case _DEF_USB_SEND_ERASE_FLASH_ERR: //FLASH擦除失败
			{
				Label26->Caption = LanguageGetStr(202);
			} break;
			case _DEF_USB_SEND_FW_PROGRAM_DONE: //固件编程完毕
			{
				Label26->Caption = LanguageGetStr(203);
			} break;
			case _DEF_USB_SEND_FW_PROGRAM_ERR: //固件编程失败
			{
				Label26->Caption = LanguageGetStr(204);
			} break;
			case _DEF_USB_SEND_W25Q_ERASE_POS: //W25Q擦除进度
			{
				ProgressBar1->Max = RecvPacks.Bl_Erase_W25q_Pos.Max;
				ProgressBar1->Position = RecvPacks.Bl_Erase_W25q_Pos.Pos;
				if(RecvPacks.Bl_Erase_W25q_Pos.Max == RecvPacks.Bl_Erase_W25q_Pos.Pos)//如果刷写完毕
				{
					Button47->Enabled = true;
					Button44->Enabled = true;
					Button45->Enabled = true;
					Label26->Caption = LanguageGetStr(205);
					if(CpRun.Bl.FwFile.hFwFile != NULL)
					{
						Button46->Enabled = true;
					}
				}
			} break;

		}
	}

	//键盘固件通信
	if(MyHid->GetConnectType() == _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)//键盘固件
	{
		switch(RecvPacks.Cmd.Cmd)
		{
			case USB_DEF_CMD_GET_HW_FW_INFO: //设备信息
			{
				GetDeviceInfo(&RecvPacks);
			} break;
			case USB_DEF_CMD_PHY_CFG_KEY_SCAN_GET: //扫描配置
			{
				GetKeyScanCfgFormDevice(&RecvPacks);
			} break;
			case USB_DEF_CMD_PHY_CFG_LED_GET: //LED配置
			{
				GetLedCfgFormDevice(&RecvPacks);
			} break;
			case USB_DEF_CMD_FL_GET_DATA: //设备发来功能层的数据
			{
				FLGetData(&RecvPacks);
			} break;
			case USB_DEF_CMD_TL_GET_DATA: //设备发来触发层的数据
			{
				TLGetData(&RecvPacks);
			} break;
			case USB_DEF_CMD_LAYER_GET_DATA: //设备发来层配置
			{
				FLayerGetDataFromDevice(&RecvPacks);
			} break;
			case USB_DEF_CMD_MACRO_GET_DATA: //设备发来宏的配置
			{
				MacroGetDataFromDevice(&RecvPacks);
			} break;
			case USB_DEF_CMD_KEY_LOCK_GET_DATA: //设备发来锁定键的配置
			{
				KeyLockGetDataFromDevice(&RecvPacks);
			} break;
			case USB_DEF_CMD_LED_CFG_GET_DATA: //设备发来LED的配置
			{
				LedCfgGetDataFromDevice(&RecvPacks);
			} break;
			case USB_DEF_CMD_CUSTOM_LED_GET_PIC: //设备发来一张图片
			{
				CustomGetPicFromDevice(&RecvPacks);
			} break;
		}
	}
	TForm::Dispatch(&Message);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::GetBootLoaderInfo(void *pData)//获取BL的信息
{
	String str;
	memcpy(&CpRun.Bl.BlInfo, pData, sizeof(CpRun.Bl.BlInfo));

	str = "C3-LDN KeyBoard Config Tools V1.0(BootLoader Connected)";
	str += String().sprintf(L"   HwVer:%d.%d", CpRun.Bl.BlInfo.Hw_Ver >> 4, CpRun.Bl.BlInfo.Hw_Ver & 0x0f);
	str += String().sprintf(L"   BootLoaderVer:%d.%d", CpRun.Bl.BlInfo.Bl_Ver >> 4, CpRun.Bl.BlInfo.Bl_Ver & 0x0f);
	MainForm->Caption = str;

	Button38->Enabled = false;
	Button44->Enabled = true;
	Button45->Enabled = true;
	Button46->Enabled = false;
	Button47->Enabled = true;
	Button48->Enabled = false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::On_SendDataSuccess(Messages::TMessage &Message)
	//数据发送完成
{

	if(Message.WParam == ON_SEND_DATA_PARA_ERROR)//数据发送失败
	{
		TForm::Dispatch(&Message);
		ShowMessage(LanguageGetStr(210));
		return;
	}

	//BootLoader的通信
	if(MyHid->GetConnectType() == _DEF_USB_CONNECT_TYPE_BOOTLOADER)//BL固件
	{
		switch(Message.WParam)
		{
			case ON_SEND_DATA_PARA_DATA_PROGRESS: //发送进度
			{
				if(CpRun.Bl.IsSendFwData)//如果正在发送FW的数据，则显示进度
				{
					ProgressBar1->Position = Message.LParam;
				}
			} break;
			case ON_SEND_DATA_PARA_ERROR: //数据发送失败
			{
				ShowMessage(LanguageGetStr(210));
			} break;
		}
	}

	TForm::Dispatch(&Message);
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	//窗口被关闭，程序将要退出
	IsCloseApplication = 1;
	MyHid->StopRecvThread();
	MyHid->StopSendThread();
	delete MyHid;
	delete ComboKeysEditer;
	delete nHitKeysEditer; //连击键编辑器
	delete two1KeysEditer; //二合一键的编辑器
	delete HdKeyEditer; //保持键编辑器
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SendCmd(byte Cmd, byte Para1, byte Para2)
{

	union CP_UNION_ONE_PACK_64BYTE Pack;
	Pack.Cmd.Cmd = Cmd;
	Pack.Cmd.Pa1.HL.u8_1 = Para1;
	Pack.Cmd.Pa1.HL.u8_2 = Para2;
	MyHid->SendDataByFifo(0, &Pack);
	MyHid->StartSendThread(); //启动发送线程

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::SendRawData(byte ReportID, void *Data)
{

	union CP_UNION_ONE_PACK_64BYTE *Pack = (union CP_UNION_ONE_PACK_64BYTE *) Data;
	MyHid->SendDataByFifo(0, Pack);
	MyHid->StartSendThread(); //启动发送线程

}

//---------------------------------------------------------------------------
ULONG __fastcall TMainForm::SendDataByPack(byte ReportID, byte Cmd, uint16_t ID, void *Data, ULONG Size)//把数据分包发送
{

	union CP_UNION_ONE_PACK_64BYTE Pack; //分包后的数据
	byte *pData = (byte *)Data;
	ULONG SendSize = 0;
	ULONG PackCount = 0;

	Pack.LongDataPack.Cmd = Cmd;
	Pack.LongDataPack.TotalPackSize = Size;
	Pack.LongDataPack.id = ID;

	while(SendSize < Size)
	{
		if((Size - SendSize) > _DEF_RW_DATA_PACK_SIZE)
		{
			//如果不是最后一包数据，则为满包数据(58Byte)
			Pack.LongDataPack.CurrPackSize = CP_DEF_MAX_CARRY_DATA_SIZE;
		}
		else
		{
			//否则为剩下的数据
			Pack.LongDataPack.CurrPackSize = Size - SendSize;
		}
		//复制数据到缓冲区
		memcpy(&Pack.LongDataPack.PackData, pData, Pack.LongDataPack.CurrPackSize);
		//累计已发送的
		SendSize += Pack.LongDataPack.CurrPackSize;
		pData += Pack.LongDataPack.CurrPackSize;
		//把数据放入缓冲区中，待发送
		MyHid->SendDataByFifo(0, &Pack);
		PackCount++ ;
	}
	return (PackCount);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::GetDeviceInfo(void *pData)//获取设备的基本信息
{
	String str;
	memcpy(&CpRun.DeviceInfo, pData, sizeof(CpRun.DeviceInfo));

	//检查版本是否匹配
	switch(CpRun.DeviceInfo.HwVer)
	{
		case SYS_DEF_HW_VER_M3://STM32F103
		case SYS_DEF_HW_VER_M4://stm32F405/407
		{
			CpRun.DeviceVerFlag.Bit.Hw = 0;
		} break;
		default: //Other = ERROR
		{
			CpRun.DeviceVerFlag.Bit.Hw = 1;
		}
	}

	if(CpRun.DeviceInfo.FwVer != SYS_DEF_FW_VER)
	{
		CpRun.DeviceVerFlag.Bit.Fw = 1;
	}
	else
	{
		CpRun.DeviceVerFlag.Bit.Fw = 0;
	}
	if(CpRun.DeviceInfo.BlVer != SYS_DEF_BL_VER)
	{
		CpRun.DeviceVerFlag.Bit.Bl = 1;
	}
	else
	{
		CpRun.DeviceVerFlag.Bit.Bl = 0;
	}

	if(CpRun.DeviceVerFlag.IsFail != 0)//版本验证错误，直接让设备进入BL状态
	{
		//切换到BL
		/*
		 if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
		 {
		 ShowMessage(LanguageGetStr(211));
		 return;
		 } */
		PageControl1->ActivePage = TabSheet13; //切换到系统页
		MainForm->Height = 530;
		Button38->Enabled = false;
		Button48->Enabled = false;
		ShowMessage(LanguageGetStr(170));
		SendCmd(USB_DEF_CMD_RESET_TO_BOOTLOADER, 0, 0);
	}
	else
	{
		str = "C3-LDN KeyBoard Config Tools V1.0(KeyBoard Connected)";
		str += String().sprintf(L"   HwVer:%d.%d", CpRun.DeviceInfo.HwVer >> 4, CpRun.DeviceInfo.HwVer & 0x0f);
		str += String().sprintf(L"   FwVer:%d.%d", CpRun.DeviceInfo.FwVer >> 4, CpRun.DeviceInfo.FwVer & 0x0f);
		str += String().sprintf(L"   BootLoaderVer:%d.%d", CpRun.DeviceInfo.BlVer >> 4, CpRun.DeviceInfo.BlVer & 0x0f);
		MainForm->Caption = str;

		Button38->Enabled = true;
		Button44->Enabled = false;
		Button45->Enabled = false;
		Button46->Enabled = false;
		Button47->Enabled = false;
		Button48->Enabled = true;
		SendCmd(USB_DEF_CMD_PHY_CFG_NEED_KEY_SCAN_CFG, 0x00, 0x00);
	}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::GetKeyScanCfgFormDevice(void *pData)//从设备获取配置
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *) pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[4096]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(PhyCfg.KeyScanCfg))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&PhyCfg.KeyScanCfg, RecvDataBuff, sizeof(PhyCfg.KeyScanCfg));
		//显示内容
		SendCmd(USB_DEF_CMD_PHY_CFG_NEED_LED_CFG, 0x00, 0x00);
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::GetLedCfgFormDevice(void *pData)//从设备获取LED的扫描配置
{
	//获取配置
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *) pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[4096]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(PhyCfg.LedCfg))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&PhyCfg.LedCfg, RecvDataBuff, sizeof(PhyCfg.LedCfg));
		//显示内容

		SendCmd(USB_DEF_CMD_LAYER_NEED_DATA, 0, 0); //要求获取层的配置

		DispNormalLayer(CpRun.NormalLayer.CurrSelectLabel); //显示功能层的数据
		FnDispFLayer(Panel10); //显示触发层-FN键的选择框
		InitCustomEdit(); //显示自定义背光编辑器
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Img0DblClick(TObject *Sender)
{
	//被单击
	int i;
	CpRun.NormalLayer.CurrSelectLabel = dynamic_cast <TImage *> (Sender)->Tag;
	for(i = 0; i < LAYER_DEF_FLAYER_MAX_NUM; i++)
	{
		if(CpRun.NormalLayer.Icon[i]->Tag == CpRun.NormalLayer.CurrSelectLabel)
		{
			//被选中
			FLayerSetIconSt(CpRun.NormalLayer.Icon[i], 1);
		}
		else
		{
			//没有被选中的
			FLayerSetIconSt(CpRun.NormalLayer.Icon[i], 0);
		}
	}
	DispNormalLayer(CpRun.NormalLayer.CurrSelectLabel); //显示当前层的数据
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FLayerSetIconSt(TImage *img, bool Seleced)//画出图标的样式
{
	//画出当前选择的哪个层
	int fw, fh, fx;

	if(Seleced)
	{
		img->Canvas->Brush->Color = TColor(0x004080FF);
		img->Canvas->Font->Color = TColor(RGB(255, 255, 255));
	}
	else
	{
		img->Canvas->Brush->Color = TColor(0x00FFA54A);
		img->Canvas->Font->Color = TColor(RGB(0, 0, 0));
	}

	img->Canvas->Pen->Color = TColor(0x0000ECEC);
	img->Canvas->Rectangle(0, 0, img->Width, img->Height);

	img->Canvas->Pen->Color = TColor(0x00977326);
	img->Canvas->Rectangle(0 + 1, 0 + 1, img->Width - 1, img->Height - 1);

	img->Canvas->Font->Style = TFontStyles() << fsBold; //粗体

	fw = Canvas->TextWidth("Layer" + String(img->Tag));
	fh = Canvas->TextHeight("Layer" + String(img->Tag));

	fx = (img->Width / 2) - (fw / 2);
	fh = (img->Height / 2) - (fh / 2);

	img->Canvas->TextOut(fx, fh, "Layer" + String(img->Tag));
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::InitNormalLayer(void)//初始化常规层的功能
{
	//
	CpRun.NormalLayer.Icon[0] = Img0;
	CpRun.NormalLayer.Icon[1] = Img1;
	CpRun.NormalLayer.Icon[2] = Img2;
	CpRun.NormalLayer.Icon[3] = Img3;
	CpRun.NormalLayer.Icon[4] = Img4;
	CpRun.NormalLayer.Icon[5] = Img5;
	CpRun.NormalLayer.Icon[6] = Img6;
	CpRun.NormalLayer.Icon[7] = Img7;

	Img0DblClick(Img0); //默认显示0层
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void __fastcall TMainForm::FLGetData(void *pData)//获取功能层数据
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *) pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[4096]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[0]))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[PackData->LongDataPack.id], RecvDataBuff,
			sizeof(CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[0]));
		//显示内容
		DispNormalLayer(PackData->LongDataPack.id);
		Button1->Enabled = true;
		FnDispFLayer(Panel10);
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::OnFLIconClick(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)//功能层的ICON被单击
{
	//
	TFuncSel *FSel;

	if(Button == mbRight)//右键
	{
		if(Y < (dynamic_cast <TImage *> (Sender)->Height / 2))//上半部分
		{
			memset(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].RFunc[dynamic_cast <TImage *> (Sender)->Tag],
				0, sizeof(union FUNC_UNION_COMMAND_LIST));
		}
		else //下半部分
		{
			memset(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].PFunc[dynamic_cast <TImage *> (Sender)->Tag],
				0, sizeof(union FUNC_UNION_COMMAND_LIST));
		}
		FlDispOneIconInfo(dynamic_cast <TImage *> (Sender), CpRun.NormalLayer.CurrSelectLabel);
	}

	if(Button == mbMiddle)//中键禁用
	{
		if(Y < (dynamic_cast <TImage *> (Sender)->Height / 2))//上半部分
		{
			CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].RFunc[dynamic_cast <TImage *> (Sender)->Tag]
				.NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_DISABLE;
		}
		else //下半部分
		{
			CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].PFunc[dynamic_cast <TImage *> (Sender)->Tag]
				.NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_DISABLE;
		}
		FlDispOneIconInfo(dynamic_cast <TImage *> (Sender), CpRun.NormalLayer.CurrSelectLabel);
	}

	if(Button == mbLeft)//左键
	{
		//如果点击在上半部分，设置释放功能，
		if(Y < (dynamic_cast <TImage *> (Sender)->Height / 2))
		{
			FSel = new TFuncSel(this, MainForm, &CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].RFunc
				[dynamic_cast <TImage *> (Sender)->Tag], 0); //funcid = 0，功能层
			FSel->ShowModal();
		}
		else //否则就是下半部分，设置按下功能
		{
			FSel = new TFuncSel(this, MainForm, &CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].PFunc
				[dynamic_cast <TImage *> (Sender)->Tag], 0); //funcid = 0，功能层
			FSel->ShowModal();
			if((CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].PFunc[dynamic_cast <TImage *> (Sender)->Tag]
				.NormalKey.FuncID == FUNC_DEF_CMD_NORMAL_KEY_CODE) ||
				(CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].PFunc[dynamic_cast <TImage *> (Sender)->Tag]
				.NormalKey.FuncID == FUNC_DEF_CMD_MEDIA_KEY_CODE))
			{
				memcpy(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].RFunc[dynamic_cast <TImage *> (Sender)
					->Tag], &CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel].PFunc[dynamic_cast <TImage *>
					(Sender)->Tag], sizeof(FUNC_UNION_COMMAND_LIST));
			}
		}
		delete FSel;
		FlDispOneIconInfo(dynamic_cast <TImage *> (Sender), CpRun.NormalLayer.CurrSelectLabel);
	}
	//显示

}

//---------------------------------------------------------------------------
//----------------------------------------------------------------------------
void __fastcall TMainForm::DispNormalLayer(uint8_t LayerID)//显示当前层的数据
{
	//显示当前层的数据
	int x, y;
	int index = 0;

	//清空显示的数据
	for(index = 0; index < KeyEdit_MAX_KEYS; index++)
	{
		if(CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon == NULL)
		{
			continue;
		}
		delete CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon;
	}
	memset(&CpRun.NormalLayer.LayerEdit.KeyLabel, 0x00, sizeof(CpRun.NormalLayer.LayerEdit.KeyLabel));

	//重建显示数据
	index = 0;
	for(x = 0; x < PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{
			if(PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Enable == 0)
				//如果被禁用，则不再创建
			{
				continue;
			}
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon = new TImage(this);
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->Parent = Panel9; //在PANEL9上生成
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->Tag = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid; //按键ID
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->Top = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_y;
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->Left = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_x;
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->Width = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_w;
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->Height = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_h;
			CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon->OnMouseDown = OnFLIconClick; //事件处理

			FlDispOneIconInfo(CpRun.NormalLayer.LayerEdit.KeyLabel[index].Icon, CpRun.NormalLayer.CurrSelectLabel);
		}
	}
}

//-----------------------------------------------------------------------------
void __fastcall TMainForm::FlDispOneIconInfo(TImage *Sender, uint8_t LayerID)
	//常规显示单个ICON的内容
{
	//常规层显示单个ICON的内容
	TImage *icon = Sender;
	String Str;
	int fx;
	//画出外形
	icon->Canvas->Brush->Color = TColor(0x00FFB56A);
	icon->Canvas->Pen->Color = TColor(0x00977326);
	icon->Canvas->Pen->Style = psSolid;
	icon->Canvas->Rectangle(0, 0, icon->Width, icon->Height);
	icon->Canvas->Pen->Style = psDot;
	icon->Canvas->MoveTo(5, icon->Height / 2);
	icon->Canvas->LineTo(icon->Width - 5, icon->Height / 2);

	//画出按键编号
	icon->Canvas->Font->Color = TColor(0x00977326);
	icon->Canvas->Font->Style = TFontStyles(); //标准风格
	Str = icon->Tag;
	icon->Canvas->TextOut(icon->Width - Canvas->TextWidth(Str) - 4, (icon->Height / 2 - 6), Str);

	//画出文字
	icon->Canvas->Font->Color = clWhite; //白色
	icon->Canvas->Font->Size = 8; //字体大小，8
	icon->Canvas->Font->Style = TFontStyles() << fsBold; //粗体

	//上半部分
	//取得字符串
	Str = GetFuncStr(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[LayerID].RFunc[icon->Tag], 0);
	fx = (icon->Width / 2) - (icon->Canvas->TextWidth(Str) / 2) - 1;
	icon->Canvas->TextOutA(fx, 5, Str);

	//生成提示字符串
	icon->Hint = LanguageGetStr(220) + GetAllFunctionHintStr(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[LayerID].RFunc[icon->Tag], 0) + "\r\n";

	//下半部分
	Str = GetFuncStr(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[LayerID].PFunc[icon->Tag], 0);
	fx = (icon->Width / 2) - (icon->Canvas->TextWidth(Str) / 2) - 1;
	icon->Canvas->TextOutA(fx, icon->Height / 2 + 5, Str);
	icon->Hint = icon->Hint + LanguageGetStr(221) + GetAllFunctionHintStr
		(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[LayerID].PFunc[icon->Tag], 0);

	icon->ShowHint = true;
}

//------------------------------------------------------------------------------
String __fastcall TMainForm::GetFuncStr(union FUNC_UNION_COMMAND_LIST *Func, uint8_t Model)//返回功能对应的字符串
{
	//不同的功能模块，返回的字符是有所不同的
	//Model = 0,功能层
	//Model = 1,触发层
	//Model = 2,宏
	//Model = 3,锁定键

	switch(Func->NormalKey.FuncID)
	{
		case FUNC_DEF_CMD_NONE: //透明
		{
			switch(Model)
			{
				case 0: //功能层，返回空
				{
					return ("");
				}
				default: //不匹配，返回空
				{
					return ("");
				}
			}
		};
		case FUNC_DEF_CMD_NORMAL_KEY_DISABLE: //按键被禁用
		{
			return ("DisEn"); //禁用字符
		};
		case FUNC_DEF_CMD_NORMAL_KEY_CODE: //普通键码
		{
			return (GetKeyCodeStr(Func->NormalKey.KeyCode)); //取得普通键码的数据
		};
		case FUNC_DEF_CMD_MEDIA_KEY_CODE: //多媒体键码
		{
			return (GetMediaKeyCodeStr(Func->MediaKey.MediaKeyCode));
		}
		case FUNC_DEF_CMD_ENABLE_FLAYER: //功能层开关
		{
			return (Func->FLayerEnable.Enable ? "FE:E" : "FE:D");
		}
		case FUNC_DEF_CMD_SET_FLAYER_TOP_PRIORITY: //功能层最高优先级
		{
			return ("FT:" + String(Func->FLayerSetTopPriority.LayerID));
		}
		case FUNC_DEF_CMD_ACTIVE_TLAYER: //激活触发层
		{
			return ("TA:" + String(Func->TLayerActive.TlayerID));
		}
		case FUNC_DEF_CMD_FN_FUNC_ENABLE: //FN功能开关
		{
			return (Func->FnFuncEnable.Enable ? "Fnf:E" : "FnF:D");
		}
		case FUNC_DEF_CMD_FN_KEY_ENABLE: //fn功能分开关
		{
			return ((Func->FnKeyEnable.Enable ? "Fnk:E" : "Fnk:D") + String(Func->FnKeyEnable.Keyid));
		}
		case FUNC_DEF_CMD_COMBO_FUNC_ENABLE: //组合键总开关
		{
			return (Func->ComboFuncEnable.Enable ? "CF:E" : "CF:D");
		}
		case FUNC_DEF_CMD_COMBO_KEY_ENABLE: //组合键分开关
		{
			return ((Func->ComboGroupEnable.Enable ? "Ck:E" : "Ck:D") + String(Func->ComboGroupEnable.GroupID));
		}
		case FUNC_DEF_CMD_NHIT_FUNC_ENABLE: //连击键总开关
		{
			return (Func->nHitFuncEnable.Enable ? "nF:E" : "nF:D");
		}
		case FUNC_DEF_CMD_NHIT_KEY_ENABLE: //连击键分开关
		{
			return ((Func->nHitKeyEnable.Enable ? "nK:E" : "nK:D") + String(Func->nHitKeyEnable.Keyid));
		}
		case FUNC_DEF_CMD_2IN1_FUNC_ENABLE: //二合一键总开关
		{
			return (Func->Two1FuncEnable.Enable ? "21F:E" : "21F:D");
		}
		case FUNC_DEF_CMD_2IN1_KEY_ENABLE: //二合一键分开关
		{
			return ((Func->Two1KeyEnable.Enable ? "21KE" : "21KD") + String(Func->Two1KeyEnable.Keyid));
		}
		case FUNC_DEF_CMD_HOLDDOWN_FUNC_ENABLE: //保持键
		{
			return (Func->HoldDownFuncEnable.Enable ? "Hd:E" : "Hd:D");
		}
		case FUNC_DEF_CMD_HOLDDOWN_KEY_ENABLE: //保持键分开关
		{
			return ((Func->HoldDownKeyEnable.Enable ? "HdkE" : "HdkD") + String(Func->HoldDownKeyEnable.Keyid));
		}
		case FUNC_DEF_CMD_NORMAL_KEY_ENABLE: //物理按键触发开关
		{
			return ((Func->NormalKeyEnable.Enable ? "NE" : "ND") + String(Func->NormalKeyEnable.Keyid));
		}
		case FUNC_DEF_CMD_RUN_KEY_LOCK: //运行一组锁定键
		{
			return ("RLK" + String(Func->RunKeyLockGroup.GroupID));
		}
		case FUNC_DEF_CMD_RUN_MACRO: //运行一组宏
		{
			return ("RM");
		}
		case FUNC_DEF_CMD_MACRO_CURR_JMP: //宏组内跳转
		{
			return ("MJ");
		}
		case FUNC_DEF_CMD_MACRO_GROUP_JMP: //跨组跳转
		{
			return ("GJ");
		}
		case FUNC_DEF_CMD_MACRO_DELAY: //宏延时
		{
			return ("MD");
		}
		case FUNC_DEF_CMD_MACRO_P_NORMAL_KEY: //宏按下普通键
		{
			return ("MNP");
		}
		case FUNC_DEF_CMD_MACRO_R_NORMAL_KEY: //宏释放普通键
		{
			return ("MNR");
		}
		case FUNC_DEF_CMD_MACRO_P_MEDIA_KEY:
		{
			return ("MMP");
		}
		case FUNC_DEF_CMD_MACRO_R_MEDIA_KEY:
		{
			return ("MMR");
		}
		case FUNC_DEF_CMD_MACRO_STOP: //停止宏
		{
			return ("MSP");
		}
		case FUNC_DEF_CMD_BKL_LAST_EFF: //背光上一个灯效
		{
			return ("BKLU");
		}
		case FUNC_DEF_CMD_BKL_NEXT_EFF: //背光下一个灯效
		{
			return ("BKLD");
		}
		case FUNC_DEF_CMD_BKL_SET_EFF: //指定背光灯效
		{
			return ("BKLS");
		}
		case FUNC_DEF_CMD_BKL_EFF_FAST: //背光速度增加
		{
			return ("BKLF");
		}
		case FUNC_DEF_CMD_BKL_EFF_SLOW: //背光更慢
		{
			return ("BKLL");
		}
		case FUNC_DEF_CMD_BKL_EFF_SPEED_SET: //指定背光速度
		{
			return ("BKLSS");
		}
		case FUNC_DEF_CMD_BKL_LOW_ALL_LED_LIG_LEVEL: //降低背光亮度
		{
			return ("BKLLD");
		}
		case FUNC_DEF_CMD_BKL_HIGH_ALL_LED_LIG_LEVEL: //提高背光亮度
		{
			return ("BKLLH");
		}
		case FUNC_DEF_CMD_WS_ENABLE_GROUP_FUNC: //开启或者关闭WS灯效
		{
			return ("WSSW");
		}
		case FUNC_DEF_CMD_WS_SET_GROUP_LAST_EFF: //WS上一个灯效
		{
			return ("WSEU");
		}
		case FUNC_DEF_CMD_WS_SET_GROUP_NEXT_EFF: //WS下一个灯效
		{
			return ("WSED");
		}
		case FUNC_DEF_CMD_WS_SET_GROUP_EFFECT: //指定WS灯效
		{
			return ("WSES");
		}
		case FUNC_DEF_CMD_ST_LED_ENABLE_CAPS: //兼容状态灯开关
		{
			return ("CPE");
		}
		case FUNC_DEF_CMD_ST_LED_ENABLE_SCROLL: //
		{
			return ("SLE");
		}
		case FUNC_DEF_CMD_ST_LED_ENABLE_NUM:
		{
			return ("NME");
		}
		case FUNC_DEF_CMD_LED_ACTION_CAPS_ENABLE: //指示灯动作
		{
			return ("ACPE");
		}
		case FUNC_DEF_CMD_LED_ACTION_SCROLL_ENABLE:
		{
			return ("ASLE");
		}
		case FUNC_DEF_CMD_LED_ACTION_NUM_ENABLE:
		{
			return ("ANME");
		}
		case FUNC_DEF_CMD_BKL_ENABLE: //背光灯效开关
		{
			return ("BKLSW");
		}

	}
	return ("");
}

//------------------------------------------------------------------------------
String __fastcall TMainForm::GetMediaKeyCodeStr(uint16_t Code)//返回多媒体键码的字符串
{
	switch(Code)
	{
		case 233:
		return ("Vol +");
		case 234:
		return ("Vol -");
		case 226:
		return ("Mute");
		case 176:
		return ("Play");
		case 177:
		return ("Pause");
		case 181:
		return ("Next");
		case 182:
		return ("Prev");
		case 180:
		return ("Rewind");
		case 179:
		return ("forw");
		case 183:
		return ("stop");
		case 404:
		return ("MyPc");
		case 402:
		return ("Calc");
		case 545:
		return ("Find");
		case 547:
		return ("Home");
		case 548:
		return ("Back");
		case 549:
		return ("Forw");
		case 551:
		return ("Refre");
		case 550:
		return ("STOP");
		case 554:
		return ("Label");
		default:
		{
			return ("MK...");
		}
	}
}

//------------------------------------------------------------------------------
String __fastcall TMainForm::GetKeyCodeStr(uint8_t Code)//返回键码对应的字符串
{
	switch(Code)
	{
		case 0:
		return ("");
		case 1:
		return ("");
		case 2:
		return ("");
		case 3:
		return ("");
		case 4:
		return ("A");
		case 5:
		return ("B");
		case 6:
		return ("C");
		case 7:
		return ("D");
		case 8:
		return ("E");
		case 9:
		return ("F"); //9

		case 10:
		return ("G");
		case 11:
		return ("H");
		case 12:
		return ("I");
		case 13:
		return ("J");
		case 14:
		return ("K");
		case 15:
		return ("L");
		case 16:
		return ("M");
		case 17:
		return ("N");
		case 18:
		return ("O");
		case 19:
		return ("P"); //19

		case 20:
		return ("Q");
		case 21:
		return ("R");
		case 22:
		return ("S");
		case 23:
		return ("T");
		case 24:
		return ("U");
		case 25:
		return ("V");
		case 26:
		return ("W");
		case 27:
		return ("X");
		case 28:
		return ("Y");
		case 29:
		return ("Z"); //29

		case 30:
		return ("1  !");
		case 31:
		return ("2  @");
		case 32:
		return ("3  #");
		case 33:
		return ("4  $");
		case 34:
		return ("5  %");
		case 35:
		return ("6  ^");
		case 36:
		return ("7  &");
		case 37:
		return ("8  *");
		case 38:
		return ("9  (");
		case 39:
		return (")  0"); //39

		case 40:
		return ("Enter");
		case 41:
		return ("Esc");
		case 42:
		return ("BKSP");
		case 43:
		return ("Tab");
		case 44:
		return ("Space");
		case 45:
		return ("-  _");
		case 46:
		return ("=  +");
		case 47:
		return ("[  {");
		case 48:
		return ("]  }");
		case 49:
		return ("\\  |"); //49

		case 50:
		return ("Europ1");
		case 51:
		return (";  :");
		case 52:
		return ("'  \"");
		case 53:
		return ("`  ~");
		case 54:
		return (",  <");
		case 55:
		return (">  .");
		case 56:
		return ("/  ?");
		case 57:
		return ("CAPS");
		case 58:
		return ("F1");
		case 59:
		return ("F2"); //59

		case 60:
		return ("F3");
		case 61:
		return ("F4");
		case 62:
		return ("F5");
		case 63:
		return ("F6");
		case 64:
		return ("F7");
		case 65:
		return ("F8");
		case 66:
		return ("F9");
		case 67:
		return ("F10");
		case 68:
		return ("F11");
		case 69:
		return ("F12"); //69

		case 70:
		return ("PS");
		case 71:
		return ("SL");
		case 72:
		return ("PB");
		case 73:
		return ("Insert");
		case 74:
		return ("Home");
		case 75:
		return ("PgUp");
		case 76:
		return ("Delete");
		case 77:
		return ("End");
		case 78:
		return ("PgDn");
		case 79:
		return ("Right"); //79

		case 80:
		return ("Left");
		case 81:
		return ("Down");
		case 82:
		return ("Up");
		case 83:
		return ("Num");
		case 84:
		return ("/");
		case 85:
		return ("*");
		case 86:
		return ("-");
		case 87:
		return ("+");
		case 88:
		return ("Enter");
		case 89:
		return ("1"); //89

		case 90:
		return ("2");
		case 91:
		return ("3");
		case 92:
		return ("4");
		case 93:
		return ("5");
		case 94:
		return ("6");
		case 95:
		return ("7");
		case 96:
		return ("8");
		case 97:
		return ("9");
		case 98:
		return ("0");
		case 99:
		return ("."); //99

		case 100:
		return ("Europ2");
		case 101:
		return ("App");
		case 102:
		return ("Pwr");
		case 103:
		return ("Pad =");
		case 104:
		return ("F13");
		case 105:
		return ("F14");
		case 106:
		return ("F15");
		case 107:
		return ("F16");
		case 108:
		return ("F17");
		case 109:
		return ("F18"); //109

		case 110:
		return ("F19");
		case 111:
		return ("F20");
		case 112:
		return ("F21");
		case 113:
		return ("F22");
		case 114:
		return ("F23");
		case 115:
		return ("F24");
		case 116:
		return ("Execut");
		case 117:
		return ("Help");
		case 118:
		return ("Menu");
		case 119:
		return ("Select"); //119

		case 120:
		return ("Stop");
		case 121:
		return ("Again");
		case 122:
		return ("Undo");
		case 123:
		return ("Cut");
		case 124:
		return ("Copy");
		case 125:
		return ("Paste");
		case 126:
		return ("Find");
		case 127:
		return ("mute");
		case 128:
		return ("VolUp");
		case 129:
		return ("VolDn"); //129

		case 130:
		return ("LCaps");
		case 131:
		return ("LNum");
		case 132:
		return ("LScr");
		case 133:
		return ("Comma");
		case 134:
		return ("Equal");
		case 135:
		return ("inte1");
		case 136:
		return ("inte2");
		case 137:
		return ("inte3");
		case 138:
		return ("inte4");
		case 139:
		return ("inte5"); //139

		case 140:
		return ("inte6");
		case 141:
		return ("inte7");
		case 142:
		return ("inte8");
		case 143:
		return ("inte9");
		case 144:
		return ("Lang1");
		case 145:
		return ("Lang2");
		case 146:
		return ("Lang3");
		case 147:
		return ("Lang4");
		case 148:
		return ("Lang5");
		case 149:
		return ("Lang6"); //149

		case 150:
		return ("Lang7");
		case 151:
		return ("Lang8");
		case 152:
		return ("Lang9");
		case 153:
		return ("AlteErs");
		case 154:
		return ("SysReq");
		case 155:
		return ("Cancel");
		case 156:
		return ("Clear");
		case 157:
		return ("Prior");
		case 158:
		return ("Return");
		case 159:
		return ("Separ"); //159

		case 160:
		return ("Out");
		case 161:
		return ("Oper");
		case 162:
		return ("C/A");
		case 163:
		return ("CS/Pr");
		case 164:
		return ("ExSel");

		case 176:
		return ("Pad00");
		case 177:
		return ("Pad000");
		case 178:
		return ("T.S");
		case 179:
		return ("D.S"); //179

		case 180:
		return ("C.Unit");
		case 181:
		return ("C.Sunit");
		case 182:
		return ("Pad (");
		case 183:
		return ("Pad )");
		case 184:
		return ("Pad {");
		case 185:
		return ("Pad }");
		case 186:
		return ("Pad Tab");
		case 187:
		return ("PadBKSP");
		case 188:
		return ("Pad A");
		case 189:
		return ("Pad B"); //189

		case 190:
		return ("Pad C");
		case 191:
		return ("Pad D");
		case 192:
		return ("Pad E");
		case 193:
		return ("Pad F");
		case 194:
		return ("PadXor");
		case 195:
		return ("Pad ^");
		case 196:
		return ("Pad %");
		case 197:
		return ("Pad <");
		case 198:
		return ("Pad >");
		case 199:
		return ("Pad &"); //199

		case 200:
		return ("Pad &&");
		case 201:
		return ("Pad |");
		case 202:
		return ("Pad ||");
		case 203:
		return ("Pad :");
		case 204:
		return ("Pad #");
		case 205:
		return ("Pad Sp");
		case 206:
		return ("Pad @");
		case 207:
		return ("Pad !");
		case 208:
		return ("Pad MS");
		case 209:
		return ("Pad MR"); //209

		case 210:
		return ("Pad MC");
		case 211:
		return ("Pad MA");
		case 212:
		return ("Pad Ms");
		case 213:
		return ("Pad MM");
		case 214:
		return ("Pad MD");
		case 215:
		return ("Pad +/-");
		case 216:
		return ("Pad Cl");
		case 217:
		return ("Pad C.E");
		case 218:
		return ("Pad Bin");
		case 219:
		return ("Pad Oct"); //219

		case 220:
		return ("Pad Dec");
		case 221:
		return ("Pad Hex");

		case 224:
		return ("L_Ctrl");
		case 225:
		return ("L_Shift");
		case 226:
		return ("L_Alt");
		case 227:
		return ("L_Win");
		case 228:
		return ("R_Ctrl");
		case 229:
		return ("R_Shift"); //229

		case 230:
		return ("R_Alt");
		case 231:
		return ("R_Win");
	}
	return ("ERR!");
}

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	//下载功能层的数据
	if(MyHid->GetConnectType() == _DEF_USB_CONNECT_TYPE_NONE)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button1->Enabled = false;
	SendCmd(USB_DEF_CMD_FL_NEED_DATA, CpRun.NormalLayer.CurrSelectLabel, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button3Click(TObject *Sender)
{
	//清空本层的数据
	int x, y;
	if(MessageBox(this->Handle, AnsiString(LanguageGetStr(222)).c_str(), "Warning", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}
	memset(&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel], 0, sizeof(struct LAYER_STRUCT_ONE_FLAYER));
	//重新显示
	DispNormalLayer(CpRun.NormalLayer.CurrSelectLabel);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender)
{
	//上传配置
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button2->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_FL_GET_DATA, CpRun.NormalLayer.CurrSelectLabel,
		&CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel], sizeof(struct LAYER_STRUCT_ONE_FLAYER));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button2->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button5Click(TObject *Sender)
{
	//保存功能层数据
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "FuncLayerData(*.FLCfg)\0*.FLCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Set a File Name...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "FLCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetSaveFileName(&ofn) == 0)
	{
		return;
	}

	HANDLE hHandle = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(223)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	DWORD WriteFileSize;
	uint16_t tmp = CP_DEF_FLAYER_CFG_FILE_HEAD;
	WriteFile(hHandle, &tmp, sizeof(tmp), &WriteFileSize, NULL);
	WriteFile(hHandle, &CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel],
		sizeof(CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel]), &WriteFileSize, NULL);
	CloseHandle(hHandle);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button4Click(TObject *Sender)
{
	//读取配置
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "FuncLayerData(*.FLCfg)\0*.FLCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Select a File...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "FLCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(225)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	uint16_t tmp;
	DWORD ReadSize;
	ReadFile(hHandle, &tmp, sizeof(tmp), &ReadSize, 0);
	if(tmp != CP_DEF_FLAYER_CFG_FILE_HEAD)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(226)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		CloseHandle(hHandle);
		return;
	}
	ReadFile(hHandle, &CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel],
		sizeof(CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel]), &ReadSize, NULL);
	CloseHandle(hHandle);
	if(ReadSize != sizeof(CpRun.NormalLayer.LayerEdit.LayerData.FLayerData[CpRun.NormalLayer.CurrSelectLabel]))
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(227)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	DispNormalLayer(CpRun.NormalLayer.CurrSelectLabel); //显示当前层的数据
	FnDispFLayer(Panel10); //显示参考
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadMediaKeyCode(void)//加载多媒体键码的配置文件
{
	int i;
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "MediaKeyCode.ini");
	CpRun.MediaKeyCode.Code = new TStringList();
	CpRun.MediaKeyCode.Str = new TStringList();

	ini->ReadSection("ConsumerKeyCode", CpRun.MediaKeyCode.Code);
	for(i = 0; i < CpRun.MediaKeyCode.Code->Count; i++)
	{
		CpRun.MediaKeyCode.Str->Add(ini->ReadString("ConsumerKeyCode", CpRun.MediaKeyCode.Code->Strings[i], ""));
	}
	delete ini;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadNormalKeyCode(void)//加载普通键码的配置文件
{
	int i;
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "KeyBoardCode.ini");
	CpRun.NormalKeyCode.Code = new TStringList();
	CpRun.NormalKeyCode.Str = new TStringList();

	ini->ReadSection("KeyBoardCode", CpRun.NormalKeyCode.Code);
	for(i = 0; i < CpRun.NormalKeyCode.Code->Count; i++)
	{
		CpRun.NormalKeyCode.Str->Add(ini->ReadString("KeyBoardCode", CpRun.NormalKeyCode.Code->Strings[i], ""));
	}
	delete ini;
}

//---------------------------------------------------------------------------
String __fastcall TMainForm::GetNormalKeyCodeForStr(uint8_t KeyCode)
	//获取一个普通按键的键码字符串
{
	int i;
	for(i = 0; i < CpRun.NormalKeyCode.Code->Count; i++)
	{
		if(KeyCode == CpRun.NormalKeyCode.Code->Strings[i].ToIntDef(0))
			//找到了，返回
		{
			return (CpRun.NormalKeyCode.Str->Strings[i]);
		}
	}
	return ("Err!");
}

//---------------------------------------------------------------------------
String __fastcall TMainForm::GetMediaKeyCodeForStr(uint16_t KeyCode)
	//获取一个多媒体键码的字符串
{
	int i;
	for(i = 0; i < CpRun.MediaKeyCode.Code->Count; i++)
	{
		if(KeyCode == CpRun.MediaKeyCode.Code->Strings[i].ToIntDef(0))
			//找到了，返回
		{
			return (CpRun.MediaKeyCode.Str->Strings[i]);
		}
	}
	return ("Err!");
}

//---------------------------------------------------------------------------
String __fastcall TMainForm::GetAllFunctionHintStr(union FUNC_UNION_COMMAND_LIST *Func, uint8_t Model)//获取所有功能的提示字符串
{
	//获取所有功能的提示字符串
	switch(Func->NormalKey.FuncID)
	{
		case FUNC_DEF_CMD_NONE: //对于功能层，作为透明处理，对于触发层，则什么也不做，对于宏，会停止执行命令
		{
			switch(Model)
			{
				case 0: //功能层
				{
					return (LanguageGetStr(230));
				}
				case 1: //触发层
				{
					return ("");
				}
				case 2: //宏
				{
					return ("");
				}
				default:
				{
					return ("");
				}
			}
		}
		case FUNC_DEF_CMD_NORMAL_KEY_DISABLE: //对于功能层，是禁用，对于触发层，什么也不做，对于宏，忽略
		{
			switch(Model)
			{
				case 0: //功能层
				{
					return (LanguageGetStr(231));
				}
				case 1: //触发层
				{
					return ("");
				}
				case 2: //宏
				{
					return ("");
				}
				default: //其他
				{
					return ("");
				}
			}
		}
		case FUNC_DEF_CMD_NORMAL_KEY_CODE: //普通键码
		{
			return (LanguageGetStr(232) + GetNormalKeyCodeForStr(Func->NormalKey.KeyCode) + "](" + String().sprintf(L"0x%02x",
				Func->NormalKey.KeyCode) + ")");
		}
		case FUNC_DEF_CMD_MEDIA_KEY_CODE: //多媒体键码
		{
			return (LanguageGetStr(233) + GetMediaKeyCodeForStr(Func->MediaKey.MediaKeyCode) + "](" + String().sprintf(L"0x%04x",
				Func->MediaKey.MediaKeyCode) + ")");
		}
		case FUNC_DEF_CMD_ENABLE_FLAYER: //功能层开关
		{
			return ((Func->FLayerEnable.Enable ? LanguageGetStr(234) : LanguageGetStr(235)) + String(Func->FLayerEnable.LayerID));
		}
		case FUNC_DEF_CMD_SET_FLAYER_TOP_PRIORITY: //功能层最高优先级
		{
			return (LanguageGetStr(236) + String(Func->FLayerSetTopPriority.LayerID) + LanguageGetStr(237));
		}
		case FUNC_DEF_CMD_ACTIVE_TLAYER: //激活触发层
		{
			return (LanguageGetStr(238) + String(Func->TLayerActive.TlayerID));
		}
		case FUNC_DEF_CMD_FN_FUNC_ENABLE: //FN功能开关
		{
			return (Func->FnFuncEnable.Enable ? LanguageGetStr(239) : LanguageGetStr(240));
		}
		case FUNC_DEF_CMD_FN_KEY_ENABLE: //fn功能分开关
		{
			return ((Func->FnKeyEnable.Enable ? LanguageGetStr(241) : LanguageGetStr(242)) + String(Func->FnKeyEnable.Keyid) + LanguageGetStr(243));
		}
		case FUNC_DEF_CMD_COMBO_FUNC_ENABLE: //组合键总开关
		{
			return (Func->ComboFuncEnable.Enable ? LanguageGetStr(244) : LanguageGetStr(245));
		}
		case FUNC_DEF_CMD_COMBO_KEY_ENABLE: //组合键分开关
		{
			return ((Func->ComboGroupEnable.Enable ? LanguageGetStr(246) : LanguageGetStr(247)) + String(Func->ComboGroupEnable.GroupID) +
				LanguageGetStr(248));
		}
		case FUNC_DEF_CMD_NHIT_FUNC_ENABLE: //连击键总开关
		{
			return (Func->nHitFuncEnable.Enable ? LanguageGetStr(249) : LanguageGetStr(250));
		}
		case FUNC_DEF_CMD_NHIT_KEY_ENABLE: //连击键分开关
		{
			return ((Func->nHitKeyEnable.Enable ? LanguageGetStr(251) : LanguageGetStr(252)) + String(Func->nHitKeyEnable.Keyid) +
				LanguageGetStr(253));
		}
		case FUNC_DEF_CMD_2IN1_FUNC_ENABLE: //二合一键总开关
		{
			return (Func->Two1FuncEnable.Enable ? LanguageGetStr(254) : LanguageGetStr(255));
		}
		case FUNC_DEF_CMD_2IN1_KEY_ENABLE: //二合一键分开关
		{
			return ((Func->Two1KeyEnable.Enable ? LanguageGetStr(256) : LanguageGetStr(257)) + String(Func->Two1KeyEnable.Keyid) +
				LanguageGetStr(258));
		}
		case FUNC_DEF_CMD_HOLDDOWN_FUNC_ENABLE: //保持键
		{
			return (Func->HoldDownFuncEnable.Enable ? LanguageGetStr(259) : LanguageGetStr(260));
		}
		case FUNC_DEF_CMD_HOLDDOWN_KEY_ENABLE: //保持键分开关
		{
			return ((Func->HoldDownKeyEnable.Enable ? LanguageGetStr(261) : LanguageGetStr(262)) + String(Func->HoldDownKeyEnable.Keyid) +
				LanguageGetStr(263));
		}
		case FUNC_DEF_CMD_NORMAL_KEY_ENABLE: //物理按键触发开关
		{
			return ((Func->NormalKeyEnable.Enable ? LanguageGetStr(264) : LanguageGetStr(265)) + String(Func->NormalKeyEnable.Keyid) +
				LanguageGetStr(266));
		}
		case FUNC_DEF_CMD_RUN_KEY_LOCK: //运行一组锁定键
		{
			return (LanguageGetStr(267) + String(Func->RunKeyLockGroup.GroupID) + LanguageGetStr(268));
		}
		case FUNC_DEF_CMD_RUN_MACRO: //运行一组宏
		{
			return (LanguageGetStr(269) + String(Func->RunMacroGroup.GroupID) + LanguageGetStr(270) + String(Func->RunMacroGroup.StepID) +
				LanguageGetStr(271));
		}
		case FUNC_DEF_CMD_MACRO_CURR_JMP: //宏组内跳转
		{
			return (LanguageGetStr(272) + String(Func->MacroJmp.JmpStepID) + LanguageGetStr(273));
		}
		case FUNC_DEF_CMD_MACRO_GROUP_JMP: //跨组跳转
		{
			return (LanguageGetStr(274) + String(Func->MacroGroupJmp.GroupID) + LanguageGetStr(275) + String(Func->MacroGroupJmp.JmpStepID) +
				LanguageGetStr(276));
		}
		case FUNC_DEF_CMD_MACRO_DELAY: //宏延时
		{
			return (LanguageGetStr(277) + String(Func->MacroDelay.Timems) + LanguageGetStr(278));
		}
		case FUNC_DEF_CMD_MACRO_P_NORMAL_KEY: //宏按下普通键
		{
			return (LanguageGetStr(279) + GetNormalKeyCodeForStr(Func->MacroPressNormalKey.KeyCode) + "](" + String().sprintf(L"0x%02x",
				Func->MacroPressNormalKey.KeyCode) + ")");
		}
		case FUNC_DEF_CMD_MACRO_R_NORMAL_KEY: //宏释放普通键
		{
			return (LanguageGetStr(280) + GetNormalKeyCodeForStr(Func->MacroReleaseNormalKey.KeyCode) + "](" + String().sprintf(L"0x%02x",
				Func->MacroReleaseNormalKey.KeyCode) + ")");
		}
		case FUNC_DEF_CMD_MACRO_P_MEDIA_KEY:
		{
			return (LanguageGetStr(281) + GetMediaKeyCodeForStr(Func->MacroPressMediaKey.MediaCode) + "](" + String().sprintf(L"0x%04x",
				Func->MacroPressMediaKey.MediaCode) + ")");
		}
		case FUNC_DEF_CMD_MACRO_R_MEDIA_KEY:
		{
			return (LanguageGetStr(282) + GetMediaKeyCodeForStr(Func->MacroReleaseMediaKey.MediaCode) + "](" + String().sprintf(L"0x%04x",
				Func->MacroReleaseMediaKey.MediaCode) + ")");
		}
		case FUNC_DEF_CMD_MACRO_STOP: //停止宏
		{
			return (LanguageGetStr(283));
		}
		case FUNC_DEF_CMD_BKL_LAST_EFF: //背光上一个灯效
		{
			return (LanguageGetStr(284));
		}
		case FUNC_DEF_CMD_BKL_NEXT_EFF: //背光下一个灯效
		{
			return (LanguageGetStr(285));
		}
		case FUNC_DEF_CMD_BKL_SET_EFF: //指定背光灯效
		{
			return (LanguageGetStr(286) + String(Func->BklSetEff.EffIndex));
		}
		case FUNC_DEF_CMD_BKL_EFF_FAST: //背光速度增加
		{
			return (LanguageGetStr(287));
		}
		case FUNC_DEF_CMD_BKL_EFF_SLOW: //背光更慢
		{
			return (LanguageGetStr(288));
		}
		case FUNC_DEF_CMD_BKL_EFF_SPEED_SET: //指定背光速度
		{
			return (LanguageGetStr(289) + String(Func->BklSpeedSet.Speed));
		}
		case FUNC_DEF_CMD_BKL_LOW_ALL_LED_LIG_LEVEL: //降低背光亮度
		{
			return (LanguageGetStr(290));
		}
		case FUNC_DEF_CMD_BKL_HIGH_ALL_LED_LIG_LEVEL: //提高背光亮度
		{
			return (LanguageGetStr(291));
		}
		case FUNC_DEF_CMD_WS_ENABLE_GROUP_FUNC: //开启或者关闭WS灯效
		{
			return ((Func->BklWsEnableGroup.Enable ? LanguageGetStr(292) : LanguageGetStr(293)) + String(Func->BklWsEnableGroup.GroupID) +
				LanguageGetStr(294));
		}
		case FUNC_DEF_CMD_WS_SET_GROUP_LAST_EFF: //WS上一个灯效
		{
			return (LanguageGetStr(295) + String(Func->BklWsSetGroupUDEff.GroupID) + LanguageGetStr(296));
		}
		case FUNC_DEF_CMD_WS_SET_GROUP_NEXT_EFF: //WS下一个灯效
		{
			return (LanguageGetStr(297) + String(Func->BklWsSetGroupUDEff.GroupID) + LanguageGetStr(298));
		}
		case FUNC_DEF_CMD_WS_SET_GROUP_EFFECT: //指定WS灯效
		{
			return (LanguageGetStr(299) + String(Func->BklWsSetGroupEffect.GroupID) + LanguageGetStr(300) +
				String(Func->BklWsSetGroupEffect.EffIndex));
		}
		case FUNC_DEF_CMD_ST_LED_ENABLE_CAPS: //兼容状态灯开关
		{
			return ((Func->BklSetStLedEnable.Enable ? LanguageGetStr(301) : LanguageGetStr(302)));
		}
		case FUNC_DEF_CMD_ST_LED_ENABLE_SCROLL: //
		{
			return ((Func->BklSetStLedEnable.Enable ? LanguageGetStr(303) : LanguageGetStr(304)));
		}
		case FUNC_DEF_CMD_ST_LED_ENABLE_NUM:
		{
			return ((Func->BklSetStLedEnable.Enable ? LanguageGetStr(305) : LanguageGetStr(306)));
		}
		case FUNC_DEF_CMD_LED_ACTION_CAPS_ENABLE: //指示灯动作
		{
			return ((Func->BklSetLedActionEnable.Enable ? LanguageGetStr(307) : LanguageGetStr(308)));
		}
		case FUNC_DEF_CMD_LED_ACTION_SCROLL_ENABLE:
		{
			return ((Func->BklSetLedActionEnable.Enable ? LanguageGetStr(309) : LanguageGetStr(310)));
		}
		case FUNC_DEF_CMD_LED_ACTION_NUM_ENABLE:
		{
			return ((Func->BklSetLedActionEnable.Enable ? LanguageGetStr(311) : LanguageGetStr(312)));
		}
		case FUNC_DEF_CMD_BKL_ENABLE: //背光灯效开关
		{
			return (LanguageGetStr(313));
		}
	};

	return ("");
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::PageControl1Change(TObject *Sender)
{
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet1)//功能层
	{
		MainForm->Height = 530;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet2)//触发层
	{
		MainForm->Height = 740;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet9)//宏配置
	{
		MainForm->Height = 530;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet8)//锁定键
	{
		MainForm->Height = 530;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet10)//锁定键
	{
		MainForm->Height = 530;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet11)//锁定键
	{
		MainForm->Height = 530;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet12)//锁定键
	{
		MainForm->Height = 740;
	}
	if(dynamic_cast <TPageControl *> (Sender)->ActivePage == TabSheet13)//锁定键
	{
		MainForm->Height = 530;
	}

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FnDispFLayer(TObject *Sender)//显示FN键的参考层
{
	//显示当前层的数据
	int x, y;
	int index = 0;

	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
		//如果未连接，则不显示
	{
		return;
	}

	//清空显示的数据
	for(index = 0; index < KeyEdit_MAX_KEYS; index++)
	{
		if(CpRun.TLayerEdit.FnKeys.icon[index] == NULL)
		{
			continue;
		}
		delete CpRun.TLayerEdit.FnKeys.icon[index];
	}
	memset(&CpRun.TLayerEdit.FnKeys.icon, 0x00, sizeof(CpRun.TLayerEdit.FnKeys.icon));

	//重建显示数据
	index = 0;
	for(x = 0; x < PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{
			if(PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Enable == 0)
				//如果被禁用，则不再创建
			{
				continue;
			}
			CpRun.TLayerEdit.FnKeys.icon[index] = new TImage(this);
			CpRun.TLayerEdit.FnKeys.icon[index]->Parent = (TWinControl *)Sender; //Panel10
			//在PANEL10上生成
			CpRun.TLayerEdit.FnKeys.icon[index]->Tag = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid; //按键ID
			CpRun.TLayerEdit.FnKeys.icon[index]->Top = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_y;
			CpRun.TLayerEdit.FnKeys.icon[index]->Left = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_x;
			CpRun.TLayerEdit.FnKeys.icon[index]->Width = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_w;
			CpRun.TLayerEdit.FnKeys.icon[index]->Height = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_h;
			CpRun.TLayerEdit.FnKeys.icon[index]->OnClick = TLayerKeyListClick; //单击事件

			FlDispOneIconInfo(CpRun.TLayerEdit.FnKeys.icon[index], ComboBox1->ItemIndex);
			index++ ;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBox1Select(TObject *Sender)
{
	FnDispFLayer(Panel10);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button6Click(TObject *Sender)
{
	//触发层，下载配置
	if(MyHid->GetConnectType() == _DEF_USB_CONNECT_TYPE_NONE)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button6->Enabled = false;
	SendCmd(USB_DEF_CMD_TL_NEED_DATA, ComboBox2->ItemIndex, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FnDispEditer(void)//显示EDIT的数据
{
	FnKeyEditer = new MyListView(this, TabSheet3, MainForm, 0, 0, TabSheet3->Width, TabSheet3->Height);
	FnKeyEditer->SetColNum(6);
	FnKeyEditer->SetRowNum(9);
	FnKeyEditer->SetColWidth(0, 80); //index列宽度
	FnKeyEditer->SetColWidth(1, 80); //Enable列宽度
	FnKeyEditer->SetColWidth(2, 80); //KeyID
	FnKeyEditer->SetColWidth(3, 338); //抬起触发1
	FnKeyEditer->SetColWidth(4, 338); //抬起触发1
	FnKeyEditer->SetColWidth(5, 339); //抬起触发2
	FnKeyEditer->SetHeadHeight(22); //表头高度
	FnKeyEditer->SetHeadFontSize(8); //表头字体大小
	FnKeyEditer->SetHeadBkColor(TColor(0x00FFDAB5)); //表头背景颜色
	FnKeyEditer->SetHeadFontColor(TColor(0)); //表头字体颜色
	FnKeyEditer->SetHeadPanColor(TColor(0x00977326)); //表头画笔颜色
	FnKeyEditer->SetRowHeight(-1, 27); //设置表格内容的每一行的高度
	FnKeyEditer->SetRowHeight(0, 30); //表头高一点

	FnKeyEditer->SetTableFontSize(8); //设置表格内容的字体
	FnKeyEditer->SetTableBkColor(TColor(0x00FFBB77)); //设置表格内容的背景颜色
	FnKeyEditer->SetTableFontColor(TColor(0x00FFFFFF)); //设置表格内容的字体颜色
	FnKeyEditer->SetTablePanColor(TColor(0x00977326)); //设置表格内容的画笔颜色
	FnKeyEditer->SetTableSelectBkColor(TColor(0x004080FF)); //选择的背景颜色
	FnKeyEditer->SetTableSelectFontColor(TColor(0x00FFFFFF)); //选择的字体颜色
	FnKeyEditer->SetTableSelectPenColor(TColor(0x00977326)); //选择的画笔颜色
	FnKeyEditer->SetOnMouseDown(FnKeyEditerMouseDown); //鼠标按下事件
	FnKeyEditer->Create();

	//生成表头的行
	FnKeyEditer->PaintDrawCell(0, 0, 0, 1, "Index");
	FnKeyEditer->PaintDrawCell(1, 0, 0, 1, "Enable");
	FnKeyEditer->PaintDrawCell(2, 0, 0, 1, "FnKeyID");
	FnKeyEditer->PaintDrawCell(3, 0, 0, 0, LanguageGetStr(40));
	FnKeyEditer->PaintDrawCell(4, 0, 0, 0, LanguageGetStr(48));
	FnKeyEditer->PaintDrawCell(5, 0, 0, 0, LanguageGetStr(49));

	//生成索引列
	for(int i = 0; i < LAYER_DEF_TLAYER_FN_KEY_MAX_NUM + 1; i++)
	{
		FnKeyEditer->PaintDrawCell(0, i + 1, 0, 1, i);
	}
	//显示当前层的数据
	FnKeyDispAllData(ComboBox2->ItemIndex);
}

//-------------------------------------------------------------------------
void __fastcall TMainForm::FnKeyDispAllData(int LayerID)//显示当前FN键的数据
{
	int row;
	String str;
	//清空当前选择的单元格
	CpRun.TLayerEdit.FnKeys.CurrItemRow = 0;
	CpRun.TLayerEdit.FnKeys.CurrItemCol = 0;

	for(row = 0; row < LAYER_DEF_TLAYER_FN_KEY_MAX_NUM; row++)
	{

		//Enable列
		if(MainForm->CpRun.TLayerEdit.TLayerData[LayerID].FnKey.FnKeyData[row].Enable)
		{
			FnKeyEditer->PaintDrawCell(1, row + 1, 1, 1, "ON");
		}
		else
		{
			FnKeyEditer->PaintDrawCell(1, row + 1, 1, 1, "OFF");
		}
		//键码列
		FnKeyEditer->PaintDrawCell(2, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].FnKey.FnKeyData[row].KeyID));

		//按下触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].FnKey.FnKeyData[row].PFunc, 1);
		FnKeyEditer->PaintDrawCell(3, row + 1, 1, 0, str); //左对齐

		//释放触发1
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].FnKey.FnKeyData[row].RFunc, 1);
		FnKeyEditer->PaintDrawCell(4, row + 1, 1, 0, str); //左对齐

		//释放触发2
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].FnKey.FnKeyData[row].SFunc, 1);
		FnKeyEditer->PaintDrawCell(5, row + 1, 1, 0, str); //左对齐
	}
}

//-------------------------------------------------------------------------
void __fastcall TMainForm::FnKeyEditerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	MainForm->FnKeyEditMouseDown_Class(Sender, Button, Shift, X, Y, nCol, nRow);
}

//-------------------------------------------------------------------------
void __fastcall TMainForm::FnKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	TFuncSel *FSel;
	String str;

	if(Button == mbLeft)//左键单击
	{
		if(nCol == 0 && nRow >= 0)
		{
			return;
		}
		if(nRow == 0 && nCol >= 0)
		{
			return;
		}
		if(nRow == 0)
		{
			return;
		}
		switch(nCol)
		{
			case 1:
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].Enable =
					!CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].Enable;

				if(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].Enable)
				{
					FnKeyEditer->PaintDrawCell(1, nRow, 2, 1, "ON");
				}
				else
				{
					FnKeyEditer->PaintDrawCell(1, nRow, 2, 1, "OFF");
				}
				CpRun.TLayerEdit.FnKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.FnKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.FnKeys.CurFuncCol = 0;
				CpRun.TLayerEdit.FnKeys.CurFuncRow = 0;
				CpRun.TLayerEdit.CurrEditFunction = 0; //保持键的标志位
			} break;
			case 2: //选择一个按键
			{
				CpRun.TLayerEdit.FnKeys.CurrItemRow = nRow; //记录本次点击的行列，便于编辑
				CpRun.TLayerEdit.FnKeys.CurrItemCol = nCol;
				CpRun.TLayerEdit.FnKeys.CurFuncCol = 0;
				CpRun.TLayerEdit.FnKeys.CurFuncRow = 0;
				CpRun.TLayerEdit.CurrEditFunction = 0; //保持键的标志位
			} break;
			case 3: //选择功能
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].PFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].PFunc, 1);
				FnKeyEditer->PaintDrawCell(3, nRow, 2, 0, str);

				CpRun.TLayerEdit.FnKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.FnKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.FnKeys.CurFuncCol = nCol;
				CpRun.TLayerEdit.FnKeys.CurFuncRow = nRow;
				CpRun.TLayerEdit.CurrEditFunction = 0; //保持键的标志位
			} break;
			case 4:
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].RFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].RFunc, 1);
				FnKeyEditer->PaintDrawCell(4, nRow, 2, 0, str);

				CpRun.TLayerEdit.FnKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.FnKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.FnKeys.CurFuncCol = nCol;
				CpRun.TLayerEdit.FnKeys.CurFuncRow = nRow;
				CpRun.TLayerEdit.CurrEditFunction = 0; //保持键的标志位
			} break;
			case 5:
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].SFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].SFunc, 1);
				FnKeyEditer->PaintDrawCell(5, nRow, 2, 0, str);

				CpRun.TLayerEdit.FnKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.FnKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.FnKeys.CurFuncCol = nCol;
				CpRun.TLayerEdit.FnKeys.CurFuncRow = nRow;
				CpRun.TLayerEdit.CurrEditFunction = 0; //保持键的标志位
			} break;

		}
	}
	else
	{
		//右键单击
		if(Button == mbRight)
		{
			if(nCol == 0 && nRow >= 0)
			{
				return;
			}
			if(nRow == 0 && nCol >= 0)
			{
				return;
			}
			if(nRow == 0)
			{
				return;
			}
			switch(nCol)
			{
				case 2: //fnKeyid
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].KeyID = 0;
					if((nRow == CpRun.TLayerEdit.FnKeys.CurrItemRow) && (nCol == CpRun.TLayerEdit.FnKeys.CurrItemCol))
					{
						//当前被选择
						FnKeyEditer->PaintDrawCell(2, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].KeyID));
					}
					else
					{
						FnKeyEditer->PaintDrawCell(2, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].KeyID));
					}
				} break;
				case 3: //按下事件
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].PFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].PFunc));
					if((nRow == CpRun.TLayerEdit.FnKeys.CurFuncRow) && (nCol == CpRun.TLayerEdit.FnKeys.CurFuncCol))
					{
						//当前被选择
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].PFunc, 1);
						FnKeyEditer->PaintDrawCell(3, nRow, 2, 0, str);
					}
					else
					{
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].PFunc, 1);
						FnKeyEditer->PaintDrawCell(3, nRow, 1, 0, str);
					}
				} break;
				case 4: //释放事件1
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].RFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].RFunc));
					if((nRow == CpRun.TLayerEdit.FnKeys.CurFuncRow) && (nCol == CpRun.TLayerEdit.FnKeys.CurFuncCol))
					{
						//当前被选择
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].RFunc, 1);
						FnKeyEditer->PaintDrawCell(4, nRow, 2, 0, str);
					}
					else
					{
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].RFunc, 1);
						FnKeyEditer->PaintDrawCell(4, nRow, 1, 0, str);
					}
				} break;
				case 5: //释放事件2
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].SFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].SFunc));
					if((nRow == CpRun.TLayerEdit.FnKeys.CurFuncRow) && (nCol == CpRun.TLayerEdit.FnKeys.CurFuncCol))
					{
						//当前被选择
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].SFunc, 1);
						FnKeyEditer->PaintDrawCell(5, nRow, 2, 0, str);
					}
					else
					{
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[nRow - 1].SFunc, 1);
						FnKeyEditer->PaintDrawCell(5, nRow, 1, 0, str);
					}
				} break;
			}
		}
	}
}

//-------------------------------------------------------------------------
void __fastcall TMainForm::TLayerKeyListClick(TObject *Sender)//触发层的参考键位被单击
{
	//
	TImage *img = dynamic_cast <TImage *> (Sender);
	switch(CpRun.TLayerEdit.CurrEditFunction)
	{
		case 0: //编辑fn键
		{
			if(CpRun.TLayerEdit.FnKeys.CurrItemRow < 1 || CpRun.TLayerEdit.FnKeys.CurrItemCol < 2)
			{
				return;
			}
			switch(CpRun.TLayerEdit.FnKeys.CurrItemCol)
			{
				case 2: //fnKeyID
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].FnKey.FnKeyData[CpRun.TLayerEdit.FnKeys.CurrItemRow - 1].KeyID = img->Tag;
					FnKeyEditer->PaintDrawCell(2, CpRun.TLayerEdit.FnKeys.CurrItemRow, 2, 1, String(img->Tag));
				} break;

			}

		} break;
		case 1: //编辑组合键
		{
			if(CpRun.TLayerEdit.ComboKeys.CurrItemRow < 1 || CpRun.TLayerEdit.ComboKeys.CurrItemCol < 2)
			{
				return;
			}

			switch(CpRun.TLayerEdit.ComboKeys.CurrItemCol)
			{
				case 2: //fnKeyID
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[CpRun.TLayerEdit.ComboKeys.CurrItemRow - 1].Config.FnKeyID =
						img->Tag;
					ComboKeysEditer->PaintDrawCell(2, CpRun.TLayerEdit.ComboKeys.CurrItemRow, 2, 1, String(img->Tag));
				} break;
				case 4: //TKeyID0
				case 5: //TKeyID1
				case 6: //TKeyID2
				case 7: //TKeyID3
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[CpRun.TLayerEdit.ComboKeys.CurrItemRow - 1].Config.ComboKeyid
						[CpRun.TLayerEdit.ComboKeys.CurrItemCol - 4] = img->Tag;
					ComboKeysEditer->PaintDrawCell(CpRun.TLayerEdit.ComboKeys.CurrItemCol, CpRun.TLayerEdit.ComboKeys.CurrItemRow, 2, 1,
						String(img->Tag));
				} break;
			}
		} break;
		case 2: //编辑连击键
		{
			if(CpRun.TLayerEdit.nHitKeys.cRow < 1 || CpRun.TLayerEdit.nHitKeys.cCol < 2)
			{
				return;
			}
			if(CpRun.TLayerEdit.nHitKeys.cCol == 2)
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[CpRun.TLayerEdit.nHitKeys.cRow - 1].KeyID = img->Tag;
				nHitKeysEditer->PaintDrawCell(CpRun.TLayerEdit.nHitKeys.cCol, CpRun.TLayerEdit.nHitKeys.cRow, 2, 1, String(img->Tag));
			}

		} break;
		case 3: //编辑2合一键
		{
			if(CpRun.TLayerEdit.two1Keys.cRow < 1 || CpRun.TLayerEdit.two1Keys.cCol < 2)
			{
				return;
			}
			if(CpRun.TLayerEdit.two1Keys.cCol == 2)
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[CpRun.TLayerEdit.two1Keys.cRow - 1].KeyID = img->Tag;
				two1KeysEditer->PaintDrawCell(CpRun.TLayerEdit.two1Keys.cCol, CpRun.TLayerEdit.two1Keys.cRow, 2, 1, String(img->Tag));
			}

		} break;
		case 4: //编辑保持键
		{
			if(CpRun.TLayerEdit.HdKeys.cRow < 1 || CpRun.TLayerEdit.HdKeys.cCol < 2)
			{
				return;
			}
			if(CpRun.TLayerEdit.HdKeys.cCol == 2)
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[CpRun.TLayerEdit.HdKeys.cRow - 1].KeyID = img->Tag;
				HdKeyEditer->PaintDrawCell(CpRun.TLayerEdit.HdKeys.cCol, CpRun.TLayerEdit.HdKeys.cRow, 2, 1, String(img->Tag));
			}
		} break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBox2Select(TObject *Sender)
{
	//触发层被选择
	FnKeyDispAllData(ComboBox2->ItemIndex); //显示当前层FN键的数据
	ComboKeyDispAllData(ComboBox2->ItemIndex); //重画组合键编辑器
	nHitKeyDispAllData(ComboBox2->ItemIndex); //连击键
	two1KeyDispAllData(ComboBox2->ItemIndex); //二合一键
	HoldDownKeyDispAllData(ComboBox2->ItemIndex); //保持键
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::TLGetData(void *pData)//获取触发层的数据
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *)pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[4096]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.TLayerEdit.TLayerData[0]))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&CpRun.TLayerEdit.TLayerData[PackData->LongDataPack.id], RecvDataBuff, sizeof(CpRun.TLayerEdit.TLayerData[0]));
		Button6->Enabled = true;
		//显示内容
		FnKeyDispAllData(ComboBox2->ItemIndex); //显示当前层FN键的数据
		ComboKeyDispAllData(PackData->LongDataPack.id); //重画组合键编辑器
		nHitKeyDispAllData(PackData->LongDataPack.id); //连击键
		two1KeyDispAllData(PackData->LongDataPack.id); //二合一键
		HoldDownKeyDispAllData(PackData->LongDataPack.id); //保持键
	}
}

void __fastcall TMainForm::Button7Click(TObject *Sender)
{
	//上传触发层的配置
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button7->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_TL_GET_DATA, ComboBox2->ItemIndex, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex],
		sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex]));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button7->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button8Click(TObject *Sender)
{
	//清空本层设置
	if(MessageBox(this->Handle, AnsiString(LanguageGetStr(222)).c_str(), "Warning", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}
	memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex], 0, sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex]));
	FnKeyDispAllData(ComboBox2->ItemIndex); //显示当前层FN键的数据
	ComboKeyDispAllData(ComboBox2->ItemIndex); //重画组合键编辑器
	nHitKeyDispAllData(ComboBox2->ItemIndex); //连击键
	two1KeyDispAllData(ComboBox2->ItemIndex); //二合一键
	HoldDownKeyDispAllData(ComboBox2->ItemIndex); //保持键
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button9Click(TObject *Sender)
{
	//读取配置
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "TrigLayerData(*.TLCfg)\0*.TLCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Select a File...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "TLCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(225)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	uint16_t tmp;
	DWORD ReadSize;
	ReadFile(hHandle, &tmp, sizeof(tmp), &ReadSize, 0);
	if(tmp != CP_DEF_TLAYER_CFG_FILE_HEAD)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(226)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		CloseHandle(hHandle);
		return;
	}
	ReadFile(hHandle, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex], sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex]), &ReadSize, NULL);
	CloseHandle(hHandle);
	if(ReadSize != sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex]))
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(227)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	FnKeyDispAllData(ComboBox2->ItemIndex); //显示当前层FN键的数据
	ComboKeyDispAllData(ComboBox2->ItemIndex); //重画组合键编辑器
	nHitKeyDispAllData(ComboBox2->ItemIndex); //连击键
	two1KeyDispAllData(ComboBox2->ItemIndex); //二合一键
	HoldDownKeyDispAllData(ComboBox2->ItemIndex); //保持键
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button10Click(TObject *Sender)
{
	//保存触发层数据
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "TrigLayerData(*.TLCfg)\0*.TLCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Set a File Name...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "TLCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetSaveFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(223)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	DWORD WriteFileSize;
	uint16_t tmp = CP_DEF_TLAYER_CFG_FILE_HEAD;
	WriteFile(hHandle, &tmp, sizeof(tmp), &WriteFileSize, NULL);
	WriteFile(hHandle, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex], sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex]),
		&WriteFileSize, NULL);
	CloseHandle(hHandle);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::PageControl2Change(TObject *Sender)
{
	//当前编辑第几个页面功能
	if(PageControl2->ActivePage == TabSheet3)
	{
		CpRun.TLayerEdit.CurrEditFunction = 0;
	}
	if(PageControl2->ActivePage == TabSheet4)
	{
		CpRun.TLayerEdit.CurrEditFunction = 1;
	}
	if(PageControl2->ActivePage == TabSheet5)
	{
		CpRun.TLayerEdit.CurrEditFunction = 2;
	}
	if(PageControl2->ActivePage == TabSheet6)
	{
		CpRun.TLayerEdit.CurrEditFunction = 3;
	}
	if(PageControl2->ActivePage == TabSheet7)
	{
		CpRun.TLayerEdit.CurrEditFunction = 4;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboKeyDispEditer(void)//显示组合键的编辑器
{
	ComboKeysEditer = new MyListView(this, TabSheet4, MainForm, 0, 0, TabSheet4->Width, TabSheet4->Height);
	ComboKeysEditer->SetColNum(10);
	ComboKeysEditer->SetRowNum(49);
	ComboKeysEditer->SetColWidth(0, 70); //index列宽度
	ComboKeysEditer->SetColWidth(1, 70); //Enable列宽度
	ComboKeysEditer->SetColWidth(2, 70); //TkeyID
	ComboKeysEditer->SetColWidth(3, 70); //TKeyNum
	ComboKeysEditer->SetColWidth(4, 70); //TkeyID0
	ComboKeysEditer->SetColWidth(5, 70); //TkeyID1
	ComboKeysEditer->SetColWidth(6, 70); //TkeyID2
	ComboKeysEditer->SetColWidth(7, 70); //TkeyID3
	ComboKeysEditer->SetColWidth(8, 347); //按下触发
	ComboKeysEditer->SetColWidth(9, 348); //抬起触发
	ComboKeysEditer->SetHeadHeight(22); //表头高度
	ComboKeysEditer->SetHeadFontSize(8); //表头字体大小
	ComboKeysEditer->SetHeadBkColor(TColor(0x00FFDAB5)); //表头背景颜色
	ComboKeysEditer->SetHeadFontColor(TColor(0)); //表头字体颜色
	ComboKeysEditer->SetHeadPanColor(TColor(0x00977326)); //表头画笔颜色
	ComboKeysEditer->SetRowHeight(-1, 22); //设置表格内容的每一行的高度

	ComboKeysEditer->SetTableFontSize(8); //设置表格内容的字体
	ComboKeysEditer->SetTableBkColor(TColor(0x00FFBB77)); //设置表格内容的背景颜色
	ComboKeysEditer->SetTableFontColor(TColor(0x00FFFFFF)); //设置表格内容的字体颜色
	ComboKeysEditer->SetTablePanColor(TColor(0x00977326)); //设置表格内容的画笔颜色
	ComboKeysEditer->SetTableSelectBkColor(TColor(0x004080FF)); //选择的背景颜色
	ComboKeysEditer->SetTableSelectFontColor(TColor(0x00FFFFFF)); //选择的字体颜色
	ComboKeysEditer->SetTableSelectPenColor(TColor(0x00977326)); //选择的画笔颜色
	ComboKeysEditer->SetOnMouseDown(ComboKeyEditerMouseDown); //鼠标按下事件
	ComboKeysEditer->Create();

	//生成表头的行
	ComboKeysEditer->PaintDrawCell(0, 0, 0, 1, "Index");
	ComboKeysEditer->PaintDrawCell(1, 0, 0, 1, "Enable");
	ComboKeysEditer->PaintDrawCell(2, 0, 0, 1, "FnKeyID");
	ComboKeysEditer->PaintDrawCell(3, 0, 0, 1, "TKeyNum");
	ComboKeysEditer->PaintDrawCell(4, 0, 0, 1, "TKeyID0");
	ComboKeysEditer->PaintDrawCell(5, 0, 0, 1, "TKeyID1");
	ComboKeysEditer->PaintDrawCell(6, 0, 0, 1, "TKeyID2");
	ComboKeysEditer->PaintDrawCell(7, 0, 0, 1, "TKeyID3");
	ComboKeysEditer->PaintDrawCell(8, 0, 0, 0, LanguageGetStr(40));
	ComboKeysEditer->PaintDrawCell(9, 0, 0, 0, LanguageGetStr(41));

	//生成索引列
	for(int i = 0; i < LAYER_DEF_TLAYER_COMBO_GROUP_MAX_NUM + 1; i++)
	{
		ComboKeysEditer->PaintDrawCell(0, i + 1, 0, 1, i);
	}

	ComboKeyDispAllData(ComboBox2->ItemIndex); //显示当前层里的数据
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboKeyEditerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	MainForm->ComboKeyEditMouseDown_Class(Sender, Button, Shift, X, Y, nCol, nRow);
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::ComboKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	TFuncSel *FSel;
	String str;
	int SelectNum = 0;

	if(Button == mbLeft)//左键单击
	{
		if(nCol == 0 && nRow >= 0)
		{
			return;
		}
		if(nRow == 0 && nCol >= 0)
		{
			return;
		}
		if(nRow == 0)
		{
			return;
		}
		switch(nCol)
		{
			case 1:
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.Enable =
					!CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.Enable;
				if(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.Enable)
				{
					ComboKeysEditer->PaintDrawCell(1, nRow, 2, 1, "ON");
				}
				else
				{
					ComboKeysEditer->PaintDrawCell(1, nRow, 2, 1, "OFF");
				}
				CpRun.TLayerEdit.ComboKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.ComboKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.ComboKeys.CurFuncCol = 0;
				CpRun.TLayerEdit.ComboKeys.CurFuncRow = 0;
			} break;
			case 3: //按键数量选择
			{
				TnSelect *nSel;
				SelectNum = CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.MaxKeyNum;
				nSel = new TnSelect(this, 4, 1, 1, 4, &SelectNum);
				nSel->ShowModal();
				delete nSel;
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.MaxKeyNum = SelectNum;
				ComboKeysEditer->PaintDrawCell(3, nRow, 2, 1,
					String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.MaxKeyNum));

				CpRun.TLayerEdit.ComboKeys.CurrItemRow = nRow; //记录本次点击的行列，便于编辑
				CpRun.TLayerEdit.ComboKeys.CurrItemCol = nCol;
				CpRun.TLayerEdit.ComboKeys.CurFuncCol = 0;
				CpRun.TLayerEdit.ComboKeys.CurFuncRow = 0;
				CpRun.TLayerEdit.CurrEditFunction = 1; //编辑组合键的标志位

			} break;
			case 2:
			case 4:
			case 5:
			case 6:
			case 7:
			{
				CpRun.TLayerEdit.ComboKeys.CurrItemRow = nRow; //记录本次点击的行列，便于编辑
				CpRun.TLayerEdit.ComboKeys.CurrItemCol = nCol;
				CpRun.TLayerEdit.ComboKeys.CurFuncCol = 0;
				CpRun.TLayerEdit.ComboKeys.CurFuncRow = 0;
				CpRun.TLayerEdit.CurrEditFunction = 1; //编辑组合键的标志位
			} break;
			case 8: //按下事件
			{

				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].PFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].PFunc, 1);
				ComboKeysEditer->PaintDrawCell(8, nRow, 2, 0, str);

				CpRun.TLayerEdit.ComboKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.ComboKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.ComboKeys.CurFuncCol = nCol;
				CpRun.TLayerEdit.ComboKeys.CurFuncRow = nRow;
			} break;
			case 9: //释放事件
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].RFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].RFunc, 1);
				ComboKeysEditer->PaintDrawCell(9, nRow, 2, 0, str);
				CpRun.TLayerEdit.ComboKeys.CurrItemRow = 0;
				CpRun.TLayerEdit.ComboKeys.CurrItemCol = 0;
				CpRun.TLayerEdit.ComboKeys.CurFuncCol = nCol;
				CpRun.TLayerEdit.ComboKeys.CurFuncRow = nRow;
			} break;
		}
	}
	else
	{
		//右键单击
		if(Button == mbRight)
		{
			if(nCol == 0 && nRow >= 0)
			{
				return;
			}
			if(nRow == 0 && nCol >= 0)
			{
				return;
			}
			if(nRow == 0)
			{
				return;
			}
			switch(nCol)
			{
				case 2: //fnKeyid
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.FnKeyID = 0;
					if((nRow == CpRun.TLayerEdit.ComboKeys.CurrItemRow) && (nCol == CpRun.TLayerEdit.ComboKeys.CurrItemCol))
					{
						//当前被选择
						ComboKeysEditer->PaintDrawCell(2, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.FnKeyID));
					}
					else
					{
						ComboKeysEditer->PaintDrawCell(2, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.FnKeyID));
					}
				} break;
				case 3: //TKeyNum按键数量
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.MaxKeyNum = 0;
					if((nRow == CpRun.TLayerEdit.ComboKeys.CurrItemRow) && (nCol == CpRun.TLayerEdit.ComboKeys.CurrItemCol))
					{
						//当前被选择
						ComboKeysEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.MaxKeyNum));
					}
					else
					{
						ComboKeysEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.MaxKeyNum));
					}
				} break;
				case 4:
				case 5:
				case 6:
				case 7:
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.ComboKeyid[nCol - 3] = 0;
					if((nRow == CpRun.TLayerEdit.ComboKeys.CurrItemRow) && (nCol == CpRun.TLayerEdit.ComboKeys.CurrItemCol))
					{
						//当前被选择
						ComboKeysEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.ComboKeyid[nCol - 3]));
					}
					else
					{
						ComboKeysEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].Config.ComboKeyid[nCol - 3]));
					}
				} break;
				case 8: //按下
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].PFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].PFunc));
					if((nRow == CpRun.TLayerEdit.ComboKeys.CurFuncRow) && (nCol == CpRun.TLayerEdit.ComboKeys.CurFuncCol))
					{
						//当前被选择
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].PFunc, 1);
						ComboKeysEditer->PaintDrawCell(8, nRow, 2, 0, str);
					}
					else
					{
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].PFunc, 1);
						ComboKeysEditer->PaintDrawCell(8, nRow, 1, 0, str);
					}

				} break;
				case 9: //释放
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].RFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].RFunc));
					if((nRow == CpRun.TLayerEdit.ComboKeys.CurFuncRow) && (nCol == CpRun.TLayerEdit.ComboKeys.CurFuncCol))
					{
						//当前被选择
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].RFunc, 1);
						ComboKeysEditer->PaintDrawCell(9, nRow, 2, 0, str);
					}
					else
					{
						str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].ComboKey.GroupData[nRow - 1].RFunc, 1);
						ComboKeysEditer->PaintDrawCell(9, nRow, 1, 0, str);
					}
				} break;

			}
		}
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::ComboKeyDispAllData(int LayerID)//在编辑器内显示当前层的数据
{
	int row;
	String str;
	//清空当前选择的单元格
	CpRun.TLayerEdit.ComboKeys.CurrItemRow = 0;
	CpRun.TLayerEdit.ComboKeys.CurrItemCol = 0;

	for(row = 0; row < LAYER_DEF_TLAYER_COMBO_GROUP_MAX_NUM; row++)
	{

		//Enable列
		if(MainForm->CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.Enable)
		{
			ComboKeysEditer->PaintDrawCell(1, row + 1, 1, 1, "ON");
		}
		else
		{
			ComboKeysEditer->PaintDrawCell(1, row + 1, 1, 1, "OFF");
		}

		//键码列
		ComboKeysEditer->PaintDrawCell(2, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.FnKeyID));
		//按键数量选择
		ComboKeysEditer->PaintDrawCell(3, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.MaxKeyNum));
		//K1
		ComboKeysEditer->PaintDrawCell(4, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.ComboKeyid[0]));
		ComboKeysEditer->PaintDrawCell(5, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.ComboKeyid[1]));
		ComboKeysEditer->PaintDrawCell(6, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.ComboKeyid[2]));
		ComboKeysEditer->PaintDrawCell(7, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].Config.ComboKeyid[3]));

		//按下触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].PFunc, 1);
		ComboKeysEditer->PaintDrawCell(8, row + 1, 1, 0, str); //左对齐

		//释放触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].ComboKey.GroupData[row].RFunc, 1);
		ComboKeysEditer->PaintDrawCell(9, row + 1, 1, 0, str); //左对齐
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::nHitKeyDispEditer(void)//创建连击键的编辑器
{

	nHitKeysEditer = new MyListView(this, TabSheet5, MainForm, 0, 0, TabSheet5->Width, TabSheet5->Height);
	nHitKeysEditer->SetColNum(5);
	nHitKeysEditer->SetRowNum(LAYER_DEF_TLAYER_nHIT_KEY_MAX_NUM + 1); //65

	nHitKeysEditer->SetColWidth(0, 80); //index列宽度
	nHitKeysEditer->SetColWidth(1, 100); //Enable列宽度
	nHitKeysEditer->SetColWidth(2, 100); //KEYID
	nHitKeysEditer->SetColWidth(3, 100); //触发次数
	nHitKeysEditer->SetColWidth(4, 873); //按下触发

	nHitKeysEditer->SetHeadHeight(22); //表头高度
	nHitKeysEditer->SetHeadFontSize(8); //表头字体大小
	nHitKeysEditer->SetHeadBkColor(TColor(0x00FFDAB5)); //表头背景颜色
	nHitKeysEditer->SetHeadFontColor(TColor(0)); //表头字体颜色
	nHitKeysEditer->SetHeadPanColor(TColor(0x00977326)); //表头画笔颜色
	nHitKeysEditer->SetRowHeight(-1, 22); //设置表格内容的每一行的高度

	nHitKeysEditer->SetTableFontSize(8); //设置表格内容的字体
	nHitKeysEditer->SetTableBkColor(TColor(0x00FFBB77)); //设置表格内容的背景颜色
	nHitKeysEditer->SetTableFontColor(TColor(0x00FFFFFF)); //设置表格内容的字体颜色
	nHitKeysEditer->SetTablePanColor(TColor(0x00977326)); //设置表格内容的画笔颜色
	nHitKeysEditer->SetTableSelectBkColor(TColor(0x004080FF)); //选择的背景颜色
	nHitKeysEditer->SetTableSelectFontColor(TColor(0x00FFFFFF)); //选择的字体颜色
	nHitKeysEditer->SetTableSelectPenColor(TColor(0x00977326)); //选择的画笔颜色
	nHitKeysEditer->SetOnMouseDown(nHitKeyEditMouseDown); //鼠标按下事件
	nHitKeysEditer->Create();

	//生成表头的行
	nHitKeysEditer->PaintDrawCell(0, 0, 0, 1, "Index");
	nHitKeysEditer->PaintDrawCell(1, 0, 0, 1, "Enable");
	nHitKeysEditer->PaintDrawCell(2, 0, 0, 1, "KeyID");
	nHitKeysEditer->PaintDrawCell(3, 0, 0, 1, LanguageGetStr(42));
	nHitKeysEditer->PaintDrawCell(4, 0, 0, 0, LanguageGetStr(43));

	//生成索引列
	for(int i = 0; i < LAYER_DEF_TLAYER_nHIT_KEY_MAX_NUM + 1; i++)
	{
		nHitKeysEditer->PaintDrawCell(0, i + 1, 0, 1, i);
	}
	nHitKeyDispAllData(ComboBox2->ItemIndex);
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::nHitKeyDispAllData(int LayerID)
{
	int row;
	String str;
	CpRun.TLayerEdit.nHitKeys.cRow = 0;
	CpRun.TLayerEdit.nHitKeys.cCol = 0;
	CpRun.TLayerEdit.nHitKeys.cFCol = 0;
	CpRun.TLayerEdit.nHitKeys.cFRow = 0;
	for(row = 0; row < LAYER_DEF_TLAYER_nHIT_KEY_MAX_NUM; row++)
	{

		//Enable列
		if(CpRun.TLayerEdit.TLayerData[LayerID].nHitKey.KeyData[row].Enable)
		{
			nHitKeysEditer->PaintDrawCell(1, row + 1, 1, 1, "ON");
		}
		else
		{
			nHitKeysEditer->PaintDrawCell(1, row + 1, 1, 1, "OFF");
		}
		//KeyID列
		nHitKeysEditer->PaintDrawCell(2, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].nHitKey.KeyData[row].KeyID));
		//触发次数列
		nHitKeysEditer->PaintDrawCell(3, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].nHitKey.KeyData[row].TrigCount));
		//按下触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].nHitKey.KeyData[row].Func, 1);
		nHitKeysEditer->PaintDrawCell(4, row + 1, 1, 0, str); //左对齐
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::nHitKeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	MainForm->nHitKeyEditMouseDown_Class(Sender, Button, Shift, X, Y, nCol, nRow);
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::nHitKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	TFuncSel *FSel;
	String str;
	int SelectNum = 0;

	if(Button == mbLeft)//左键单击
	{
		if(nCol == 0 && nRow >= 0)
		{
			return;
		}
		if(nRow == 0 && nCol >= 0)
		{
			return;
		}
		if(nRow == 0)
		{
			return;
		}
		switch(nCol)
		{
			case 1:
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Enable =
					!CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Enable;
				if(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Enable)
				{
					nHitKeysEditer->PaintDrawCell(1, nRow, 2, 1, "ON");
				}
				else
				{
					nHitKeysEditer->PaintDrawCell(1, nRow, 2, 1, "OFF");
				}
				CpRun.TLayerEdit.nHitKeys.cRow = 0;
				CpRun.TLayerEdit.nHitKeys.cCol = 0;
				CpRun.TLayerEdit.nHitKeys.cFRow = 0;
				CpRun.TLayerEdit.nHitKeys.cFCol = 0;
			} break;
			case 2: //键码ID
			{
				CpRun.TLayerEdit.nHitKeys.cRow = nRow;
				CpRun.TLayerEdit.nHitKeys.cCol = nCol;
				CpRun.TLayerEdit.CurrEditFunction = 2; //连击键的标志位
				CpRun.TLayerEdit.nHitKeys.cFRow = 0;
				CpRun.TLayerEdit.nHitKeys.cFCol = 0;
			} break;
			case 3: //触发次数
			{
				TnSelect *nSel;
				SelectNum = CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].TrigCount;
				nSel = new TnSelect(this, 13, 10, 0, 127, &SelectNum);
				nSel->ShowModal();
				delete nSel;
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].TrigCount = SelectNum;
				nHitKeysEditer->PaintDrawCell(3, nRow, 2, 1,
					String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].TrigCount));
				CpRun.TLayerEdit.nHitKeys.cRow = nRow;
				CpRun.TLayerEdit.nHitKeys.cCol = nCol;
				CpRun.TLayerEdit.nHitKeys.cFRow = 0;
				CpRun.TLayerEdit.nHitKeys.cFCol = 0;
			} break;
			case 4: //功能选择
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Func, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Func, 1);
				nHitKeysEditer->PaintDrawCell(4, nRow, 2, 0, str);
				CpRun.TLayerEdit.nHitKeys.cRow = 0;
				CpRun.TLayerEdit.nHitKeys.cCol = 0;
				CpRun.TLayerEdit.nHitKeys.cFRow = nRow;
				CpRun.TLayerEdit.nHitKeys.cFCol = nCol;
			} break;
		}
	}
	else
	{
		if(Button == mbRight)//右键
		{
			if(nCol == 0 && nRow >= 0)
			{
				return;
			}
			if(nRow == 0 && nCol >= 0)
			{
				return;
			}
			if(nRow == 0)
			{
				return;
			}
			switch(nCol)
			{
				case 2: //键码ID
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].KeyID = 0;
					if(nRow == CpRun.TLayerEdit.nHitKeys.cRow && nCol == CpRun.TLayerEdit.nHitKeys.cCol)
					{
						//如果被选择
						nHitKeysEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].KeyID));
					}
					else
					{
						nHitKeysEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].KeyID));
					}

				} break;
				case 3: //按键次数
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].TrigCount = 0;
					if(nRow == CpRun.TLayerEdit.nHitKeys.cRow && nCol == CpRun.TLayerEdit.nHitKeys.cCol)
					{
						//如果被选择
						nHitKeysEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].TrigCount));
					}
					else
					{
						nHitKeysEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].TrigCount));
					}
				} break;
				case 4: //触发功能
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Func, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Func));
					str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].nHitKey.KeyData[nRow - 1].Func, 1);
					if(nRow == CpRun.TLayerEdit.nHitKeys.cFRow && nCol == CpRun.TLayerEdit.nHitKeys.cFCol)
					{
						nHitKeysEditer->PaintDrawCell(4, nRow, 2, 0, str);
					}
					else
					{
						nHitKeysEditer->PaintDrawCell(4, nRow, 1, 0, str);
					}
				} break;
			}
		}
	}

}

//------------------------------------------------------------------------------
//二合一键
void __fastcall TMainForm::two1KeyDispEditer(void)//创建编辑器
{
	two1KeysEditer = new MyListView(this, TabSheet6, MainForm, 0, 0, TabSheet6->Width, TabSheet6->Height);
	two1KeysEditer->SetColNum(7);
	two1KeysEditer->SetRowNum(LAYER_DEF_TLAYER_2IN1_KEY_MAX_NUM + 1); //65

	two1KeysEditer->SetColWidth(0, 70); //index列宽度
	two1KeysEditer->SetColWidth(1, 70); //Enable列宽度
	two1KeysEditer->SetColWidth(2, 70); //KEYID
	two1KeysEditer->SetColWidth(3, 70); //触发时间
	two1KeysEditer->SetColWidth(4, 325); //短按触发
	two1KeysEditer->SetColWidth(5, 325); //长按触发
	two1KeysEditer->SetColWidth(6, 325); //长按释放触发

	two1KeysEditer->SetHeadHeight(22); //表头高度
	two1KeysEditer->SetHeadFontSize(8); //表头字体大小
	two1KeysEditer->SetHeadBkColor(TColor(0x00FFDAB5)); //表头背景颜色
	two1KeysEditer->SetHeadFontColor(TColor(0)); //表头字体颜色
	two1KeysEditer->SetHeadPanColor(TColor(0x00977326)); //表头画笔颜色
	two1KeysEditer->SetRowHeight(-1, 22); //设置表格内容的每一行的高度

	two1KeysEditer->SetTableFontSize(8); //设置表格内容的字体
	two1KeysEditer->SetTableBkColor(TColor(0x00FFBB77)); //设置表格内容的背景颜色
	two1KeysEditer->SetTableFontColor(TColor(0x00FFFFFF)); //设置表格内容的字体颜色
	two1KeysEditer->SetTablePanColor(TColor(0x00977326)); //设置表格内容的画笔颜色
	two1KeysEditer->SetTableSelectBkColor(TColor(0x004080FF)); //选择的背景颜色
	two1KeysEditer->SetTableSelectFontColor(TColor(0x00FFFFFF)); //选择的字体颜色
	two1KeysEditer->SetTableSelectPenColor(TColor(0x00977326)); //选择的画笔颜色
	two1KeysEditer->SetOnMouseDown(two1KeyEditMouseDown); //鼠标按下事件
	two1KeysEditer->Create();

	//生成表头的行
	two1KeysEditer->PaintDrawCell(0, 0, 0, 1, "Index");
	two1KeysEditer->PaintDrawCell(1, 0, 0, 1, "Enable");
	two1KeysEditer->PaintDrawCell(2, 0, 0, 1, "KeyID");
	two1KeysEditer->PaintDrawCell(3, 0, 0, 1, LanguageGetStr(44));
	two1KeysEditer->PaintDrawCell(4, 0, 0, 0, LanguageGetStr(45));
	two1KeysEditer->PaintDrawCell(5, 0, 0, 0, LanguageGetStr(46));
	two1KeysEditer->PaintDrawCell(6, 0, 0, 0, LanguageGetStr(47));

	//生成索引列
	for(int i = 0; i < LAYER_DEF_TLAYER_2IN1_KEY_MAX_NUM + 1; i++)
	{
		two1KeysEditer->PaintDrawCell(0, i + 1, 0, 1, i);
	}
	two1KeyDispAllData(ComboBox2->ItemIndex);
}

//-----------------------------------------------------------------------------
void __fastcall TMainForm::two1KeyDispAllData(int LayerID)
{
	int row;
	String str;
	CpRun.TLayerEdit.two1Keys.cRow = 0;
	CpRun.TLayerEdit.two1Keys.cCol = 0;
	CpRun.TLayerEdit.two1Keys.CFCol = 0;
	CpRun.TLayerEdit.two1Keys.CFRow = 0;
	for(row = 0; row < LAYER_DEF_TLAYER_2IN1_KEY_MAX_NUM; row++)
	{

		//Enable列
		if(CpRun.TLayerEdit.TLayerData[LayerID].Two1Key.GroupData[row].Enable)
		{
			two1KeysEditer->PaintDrawCell(1, row + 1, 1, 1, "ON");
		}
		else
		{
			two1KeysEditer->PaintDrawCell(1, row + 1, 1, 1, "OFF");
		}
		//KeyID列
		two1KeysEditer->PaintDrawCell(2, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].Two1Key.GroupData[row].KeyID));
		//触发时间
		two1KeysEditer->PaintDrawCell(3, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].Two1Key.GroupData[row].LongPressTime) + "ms");
		//短按触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].Two1Key.GroupData[row].SPFunc, 1);
		two1KeysEditer->PaintDrawCell(4, row + 1, 1, 0, str); //左对齐
		//长按触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].Two1Key.GroupData[row].LPFunc, 1);
		two1KeysEditer->PaintDrawCell(5, row + 1, 1, 0, str); //左对齐
		//长按释放触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].Two1Key.GroupData[row].LRFunc, 1);
		two1KeysEditer->PaintDrawCell(6, row + 1, 1, 0, str); //左对齐

	}
}

//-----------------------------------------------------------------------------
void __fastcall TMainForm::two1KeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	MainForm->tow1KeyEditMouseDown_Class(Sender, Button, Shift, X, Y, nCol, nRow);
}

//-----------------------------------------------------------------------------
void __fastcall TMainForm::tow1KeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	TFuncSel *FSel;
	String str;
	int SelectNum = 0;

	if(Button == mbLeft)//左键单击
	{
		if(nCol == 0 && nRow >= 0)
		{
			return;
		}
		if(nRow == 0 && nCol >= 0)
		{
			return;
		}
		if(nRow == 0)
		{
			return;
		}
		switch(nCol)
		{
			case 1: //开关
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].Enable =
					!CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].Enable;
				if(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].Enable)
				{
					two1KeysEditer->PaintDrawCell(1, nRow, 2, 1, "ON");
				}
				else
				{
					two1KeysEditer->PaintDrawCell(1, nRow, 2, 1, "OFF");
				}
				CpRun.TLayerEdit.two1Keys.cRow = 0;
				CpRun.TLayerEdit.two1Keys.cCol = 0;
				CpRun.TLayerEdit.two1Keys.CFRow = 0;
				CpRun.TLayerEdit.two1Keys.CFCol = 0;
			} break;
			case 2: //键码
			{
				CpRun.TLayerEdit.two1Keys.cRow = nRow;
				CpRun.TLayerEdit.two1Keys.cCol = nCol;
				CpRun.TLayerEdit.two1Keys.CFRow = 0;
				CpRun.TLayerEdit.two1Keys.CFCol = 0;
				CpRun.TLayerEdit.CurrEditFunction = 3; //二合一键的标志位
			} break;
			case 3: //触发时间
			{
				CpRun.TLayerEdit.two1Keys.cRow = nRow;
				CpRun.TLayerEdit.two1Keys.cCol = nCol;
				CpRun.TLayerEdit.two1Keys.CFRow = 0;
				CpRun.TLayerEdit.two1Keys.CFCol = 0;
				CpRun.TLayerEdit.CurrEditFunction = 3; //二合一键的标志位

				SelectNum = CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LongPressTime;
				TTSel *sel = new TTSel(this, 10, 1000, &SelectNum);
				sel->ShowModal();
				delete sel;

				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LongPressTime = SelectNum;
				two1KeysEditer->PaintDrawCell(nCol, nRow, 2, 1, String(SelectNum) + "ms");
			} break;
			case 4: //短按触发
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].SPFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].SPFunc, 1);
				two1KeysEditer->PaintDrawCell(4, nRow, 2, 0, str);

				CpRun.TLayerEdit.two1Keys.CFRow = nRow;
				CpRun.TLayerEdit.two1Keys.CFCol = nCol;
				CpRun.TLayerEdit.two1Keys.cRow = 0;
				CpRun.TLayerEdit.two1Keys.cCol = 0;
			} break;
			case 5: //长按触发
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LPFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LPFunc, 1);
				two1KeysEditer->PaintDrawCell(5, nRow, 2, 0, str);

				CpRun.TLayerEdit.two1Keys.CFRow = nRow;
				CpRun.TLayerEdit.two1Keys.CFCol = nCol;
				CpRun.TLayerEdit.two1Keys.cRow = 0;
				CpRun.TLayerEdit.two1Keys.cCol = 0;
			} break;
			case 6: //长按释放触发
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LRFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LRFunc, 1);
				two1KeysEditer->PaintDrawCell(6, nRow, 2, 0, str);

				CpRun.TLayerEdit.two1Keys.CFRow = nRow;
				CpRun.TLayerEdit.two1Keys.CFCol = nCol;
				CpRun.TLayerEdit.two1Keys.cRow = 0;
				CpRun.TLayerEdit.two1Keys.cCol = 0;
			} break;
		}
	}
	else
	{
		if(Button == mbRight)//右键
		{
			if(nCol == 0 && nRow >= 0)
			{
				return;
			}
			if(nRow == 0 && nCol >= 0)
			{
				return;
			}
			if(nRow == 0)
			{
				return;
			}
			switch(nCol)
			{
				case 2: //键码列
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].KeyID = 0;
					if(nCol == CpRun.TLayerEdit.two1Keys.cCol && nRow == CpRun.TLayerEdit.two1Keys.cRow)
					{
						//被选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].KeyID));
					}
					else
					{
						//未选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].KeyID));
					}
				} break;
				case 3: //触发时间
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LongPressTime = 0;
					if(nCol == CpRun.TLayerEdit.two1Keys.cCol && nRow == CpRun.TLayerEdit.two1Keys.cRow)
					{
						//被选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LongPressTime) + "ms");
					}
					else
					{
						//未选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LongPressTime) + "ms");
					}
				} break;
				case 4: //短按触发
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].SPFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].SPFunc));
					str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].SPFunc, 1);
					if(nRow == CpRun.TLayerEdit.two1Keys.CFRow && CpRun.TLayerEdit.two1Keys.CFCol == nCol)
					{
						//被选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 2, 0, str);
					}
					else
					{
						two1KeysEditer->PaintDrawCell(nCol, nRow, 1, 0, str);
					}
				} break;
				case 5: //长按触发
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LPFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LPFunc));
					str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LPFunc, 1);
					if(nRow == CpRun.TLayerEdit.two1Keys.CFRow && CpRun.TLayerEdit.two1Keys.CFCol == nCol)
					{
						//被选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 2, 0, str);
					}
					else
					{
						two1KeysEditer->PaintDrawCell(nCol, nRow, 1, 0, str);
					}
				} break;
				case 6: //长按 释放触发
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LRFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LRFunc));
					str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].Two1Key.GroupData[nRow - 1].LRFunc, 1);
					if(nRow == CpRun.TLayerEdit.two1Keys.CFRow && CpRun.TLayerEdit.two1Keys.CFCol == nCol)
					{
						//被选择
						two1KeysEditer->PaintDrawCell(nCol, nRow, 2, 0, str);
					}
					else
					{
						two1KeysEditer->PaintDrawCell(nCol, nRow, 1, 0, str);
					}
				} break;
			}
		}
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::HoldDownKeyDispEditer(void)//创建编辑器
{
	HdKeyEditer = new MyListView(this, TabSheet7, MainForm, 0, 0, TabSheet7->Width, TabSheet7->Height);
	HdKeyEditer->SetColNum(6);
	HdKeyEditer->SetRowNum(LAYER_DEF_TLAYER_HOLD_DOWN_MAX_NUM + 1); //65

	HdKeyEditer->SetColWidth(0, 80); //index列宽度
	HdKeyEditer->SetColWidth(1, 80); //Enable列宽度
	HdKeyEditer->SetColWidth(2, 80); //KEYID
	HdKeyEditer->SetColWidth(3, 80); //触发次数
	HdKeyEditer->SetColWidth(4, 467); //按下触发
	HdKeyEditer->SetColWidth(5, 468); //释放触发

	HdKeyEditer->SetHeadHeight(22); //表头高度
	HdKeyEditer->SetHeadFontSize(8); //表头字体大小
	HdKeyEditer->SetHeadBkColor(TColor(0x00FFDAB5)); //表头背景颜色
	HdKeyEditer->SetHeadFontColor(TColor(0)); //表头字体颜色
	HdKeyEditer->SetHeadPanColor(TColor(0x00977326)); //表头画笔颜色
	HdKeyEditer->SetRowHeight(-1, 22); //设置表格内容的每一行的高度

	HdKeyEditer->SetTableFontSize(8); //设置表格内容的字体
	HdKeyEditer->SetTableBkColor(TColor(0x00FFBB77)); //设置表格内容的背景颜色
	HdKeyEditer->SetTableFontColor(TColor(0x00FFFFFF)); //设置表格内容的字体颜色
	HdKeyEditer->SetTablePanColor(TColor(0x00977326)); //设置表格内容的画笔颜色
	HdKeyEditer->SetTableSelectBkColor(TColor(0x004080FF)); //选择的背景颜色
	HdKeyEditer->SetTableSelectFontColor(TColor(0x00FFFFFF)); //选择的字体颜色
	HdKeyEditer->SetTableSelectPenColor(TColor(0x00977326)); //选择的画笔颜色
	HdKeyEditer->SetOnMouseDown(HoldDownKeyEditMouseDown); //鼠标按下事件
	HdKeyEditer->Create();

	//生成表头的行
	HdKeyEditer->PaintDrawCell(0, 0, 0, 1, "Index");
	HdKeyEditer->PaintDrawCell(1, 0, 0, 1, "Enable");
	HdKeyEditer->PaintDrawCell(2, 0, 0, 1, "KeyID");
	HdKeyEditer->PaintDrawCell(3, 0, 0, 1, LanguageGetStr(42));
	HdKeyEditer->PaintDrawCell(4, 0, 0, 0, LanguageGetStr(40));
	HdKeyEditer->PaintDrawCell(5, 0, 0, 0, LanguageGetStr(41));

	//生成索引列
	for(int i = 0; i < LAYER_DEF_TLAYER_HOLD_DOWN_MAX_NUM + 1; i++)
	{
		HdKeyEditer->PaintDrawCell(0, i + 1, 0, 1, i);
	}
	HoldDownKeyDispAllData(ComboBox2->ItemIndex);
}
//------------------------------------------------------------------------------

void __fastcall TMainForm::HoldDownKeyDispAllData(int LayerID)
{
	int row;
	String str;
	CpRun.TLayerEdit.HdKeys.cRow = 0;
	CpRun.TLayerEdit.HdKeys.cCol = 0;
	CpRun.TLayerEdit.HdKeys.cFCol = 0;
	CpRun.TLayerEdit.HdKeys.cFRow = 0;
	for(row = 0; row < LAYER_DEF_TLAYER_2IN1_KEY_MAX_NUM; row++)
	{

		//Enable列
		if(CpRun.TLayerEdit.TLayerData[LayerID].HoldDownKey.GroupData[row].Enable)
		{
			HdKeyEditer->PaintDrawCell(1, row + 1, 1, 1, "ON");
		}
		else
		{
			HdKeyEditer->PaintDrawCell(1, row + 1, 1, 1, "OFF");
		}
		//KeyID列
		HdKeyEditer->PaintDrawCell(2, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].HoldDownKey.GroupData[row].KeyID));
		//触发次数
		HdKeyEditer->PaintDrawCell(3, row + 1, 1, 1, String(CpRun.TLayerEdit.TLayerData[LayerID].HoldDownKey.GroupData[row].TrigCount));
		//按下触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].HoldDownKey.GroupData[row].PFunc, 1);
		HdKeyEditer->PaintDrawCell(4, row + 1, 1, 0, str); //左对齐
		//释放触发
		str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[LayerID].HoldDownKey.GroupData[row].RFunc, 1);
		HdKeyEditer->PaintDrawCell(5, row + 1, 1, 0, str); //左对齐

	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::HoldDownKeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	MainForm->HoldDownKeyEditMouseDown_Class(Sender, Button, Shift, X, Y, nCol, nRow);
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::HoldDownKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow)
{
	TFuncSel *FSel;
	String str;
	int SelectNum = 0;

	if(Button == mbLeft)//左键单击
	{
		if(nCol == 0 && nRow >= 0)
		{
			return;
		}
		if(nRow == 0 && nCol >= 0)
		{
			return;
		}
		if(nRow == 0)
		{
			return;
		}
		switch(nCol)
		{
			case 1: //开关
			{
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].Enable =
					!CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].Enable;
				if(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].Enable)
				{
					HdKeyEditer->PaintDrawCell(1, nRow, 2, 1, "ON");
				}
				else
				{
					HdKeyEditer->PaintDrawCell(1, nRow, 2, 1, "OFF");
				}
				CpRun.TLayerEdit.HdKeys.cRow = 0;
				CpRun.TLayerEdit.HdKeys.cCol = 0;
				CpRun.TLayerEdit.HdKeys.cFRow = 0;
				CpRun.TLayerEdit.HdKeys.cFCol = 0;
			} break;
			case 2: //键码
			{
				CpRun.TLayerEdit.HdKeys.cRow = nRow;
				CpRun.TLayerEdit.HdKeys.cCol = nCol;
				CpRun.TLayerEdit.HdKeys.cFRow = 0;
				CpRun.TLayerEdit.HdKeys.cFCol = 0;
				CpRun.TLayerEdit.CurrEditFunction = 4; //保持键的标志位
			} break;
			case 3: //按下次数
			{
				TnSelect *nSel;
				SelectNum = CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].TrigCount;
				nSel = new TnSelect(this, 13, 10, 0, 127, &SelectNum);
				nSel->ShowModal();
				delete nSel;
				CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].TrigCount = SelectNum;
				HdKeyEditer->PaintDrawCell(3, nRow, 2, 1,
					String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].TrigCount));
				CpRun.TLayerEdit.HdKeys.cRow = nRow;
				CpRun.TLayerEdit.HdKeys.cCol = nCol;
				CpRun.TLayerEdit.HdKeys.cFRow = 0;
				CpRun.TLayerEdit.HdKeys.cFCol = 0;
				CpRun.TLayerEdit.CurrEditFunction = 4; //保持键的标志位
			} break;
			case 4: //按下触发
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].PFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].PFunc, 1);
				HdKeyEditer->PaintDrawCell(4, nRow, 2, 0, str);

				CpRun.TLayerEdit.HdKeys.cRow = 0;
				CpRun.TLayerEdit.HdKeys.cCol = 0;
				CpRun.TLayerEdit.HdKeys.cFRow = nRow;
				CpRun.TLayerEdit.HdKeys.cFCol = nCol;
				CpRun.TLayerEdit.CurrEditFunction = 4; //保持键的标志位
			} break;
			case 5: //释放触发
			{
				FSel = new TFuncSel(this, MainForm, &CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].RFunc, 1);
				FSel->ShowModal();
				delete FSel;

				str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].RFunc, 1);
				HdKeyEditer->PaintDrawCell(5, nRow, 2, 0, str);

				CpRun.TLayerEdit.HdKeys.cRow = 0;
				CpRun.TLayerEdit.HdKeys.cCol = 0;
				CpRun.TLayerEdit.HdKeys.cFRow = nRow;
				CpRun.TLayerEdit.HdKeys.cFCol = nCol;
				CpRun.TLayerEdit.CurrEditFunction = 4; //保持键的标志位
			} break;
		}
	}
	else
	{
		if(Button == mbRight)//右键
		{
			if(nCol == 0 && nRow >= 0)
			{
				return;
			}
			if(nRow == 0 && nCol >= 0)
			{
				return;
			}
			if(nRow == 0)
			{
				return;
			}
			switch(nCol)
			{
				case 2:
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].KeyID = 0;
					if(nCol == CpRun.TLayerEdit.HdKeys.cCol && nRow == CpRun.TLayerEdit.HdKeys.cRow)
					{
						//被选择
						HdKeyEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].KeyID));
					}
					else
					{
						//未选择
						HdKeyEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].KeyID));
					}
				} break;
				case 3:
				{
					CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].TrigCount = 0;
					if(nCol == CpRun.TLayerEdit.HdKeys.cCol && nRow == CpRun.TLayerEdit.HdKeys.cRow)
					{
						//被选择
						HdKeyEditer->PaintDrawCell(nCol, nRow, 2, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].TrigCount));
					}
					else
					{
						//未选择
						HdKeyEditer->PaintDrawCell(nCol, nRow, 1, 1,
							String(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].TrigCount));
					}
				} break;
				case 4: //按下触发
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].PFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].PFunc));
					str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].PFunc, 1);
					if(nRow == CpRun.TLayerEdit.HdKeys.cFRow && CpRun.TLayerEdit.HdKeys.cFCol == nCol)
					{
						//被选择
						HdKeyEditer->PaintDrawCell(nCol, nRow, 2, 0, str);
					}
					else
					{
						HdKeyEditer->PaintDrawCell(nCol, nRow, 1, 0, str);
					}
				} break;
				case 5: //释放触发
				{
					memset(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].RFunc, 0,
						sizeof(CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].RFunc));
					str = GetAllFunctionHintStr(&CpRun.TLayerEdit.TLayerData[ComboBox2->ItemIndex].HoldDownKey.GroupData[nRow - 1].RFunc, 1);
					if(nRow == CpRun.TLayerEdit.HdKeys.cFRow && CpRun.TLayerEdit.HdKeys.cFCol == nCol)
					{
						//被选择
						HdKeyEditer->PaintDrawCell(nCol, nRow, 2, 0, str);
					}
					else
					{
						HdKeyEditer->PaintDrawCell(nCol, nRow, 1, 0, str);
					}
				} break;
			}
		}
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::FLIcon0MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//层开关
	switch(Button)
	{
		case mbLeft: //左键开关层
		{
			FLayerEnable(dynamic_cast <TImage *> (Sender)->Tag);
		} break;
		case mbRight:
		{
			FLayerSetTopPriority(dynamic_cast <TImage *> (Sender)->Tag);
		} break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FLayerEnable(int LayerID)//开关某功能层，调用一次，开/关一次
{
	if(LayerID == CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID)//如果当前是最高优先级的 层，禁止关闭
	{
		CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[LayerID] = 1;
		return;
	}
	else
	{
		CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[LayerID] = !CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[LayerID];
		if(CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[LayerID])
		{
			CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Brush->Color = TColor(0x0099E173);

		}
		else
		{
			CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Brush->Color = TColor(0x00BBBBBB);
		}
	}

	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Pen->Color = TColor(0x0000ECEC);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Rectangle(0, 0, CpRun.LayerCfg.Panel.Layer[LayerID]->Width,
		CpRun.LayerCfg.Panel.Layer[LayerID]->Height);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Pen->Color = TColor(0x00977326);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Rectangle(1, 1, CpRun.LayerCfg.Panel.Layer[LayerID]->Width - 1,
		CpRun.LayerCfg.Panel.Layer[LayerID]->Height - 1);

	int fw, fh, fx;

	fw = Canvas->TextWidth("L" + String(CpRun.LayerCfg.Panel.Layer[LayerID]->Tag));
	fh = Canvas->TextHeight("L" + String(CpRun.LayerCfg.Panel.Layer[LayerID]->Tag));

	fx = (CpRun.LayerCfg.Panel.Layer[LayerID]->Width / 2) - (fw / 2);
	fh = (CpRun.LayerCfg.Panel.Layer[LayerID]->Height / 2) - (fh / 2);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->TextOut(fx, fh, "L" + String(CpRun.LayerCfg.Panel.Layer[LayerID]->Tag));
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FLayerSetTopPriority(int LayerID)//把某层设置为最高优先级
{
	//先取消所有的颜色显示
	int fw, fh, fx;
	for(int i = 0; i < LAYER_DEF_FLAYER_MAX_NUM; i++)
	{
		if(CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[i])
		{
			CpRun.LayerCfg.Panel.Layer[i]->Canvas->Brush->Color = TColor(0x0099E173); //开启状态
		}
		else
		{
			CpRun.LayerCfg.Panel.Layer[i]->Canvas->Brush->Color = TColor(0x00BBBBBB); //关闭状态
		}

		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Pen->Color = TColor(0x0000ECEC);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Rectangle(0, 0, CpRun.LayerCfg.Panel.Layer[i]->Width, CpRun.LayerCfg.Panel.Layer[i]->Height);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Pen->Color = TColor(0x00977326);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Rectangle(1, 1, CpRun.LayerCfg.Panel.Layer[i]->Width - 1, CpRun.LayerCfg.Panel.Layer[i]->Height - 1);

		fw = Canvas->TextWidth("L" + String(CpRun.LayerCfg.Panel.Layer[i]->Tag));
		fh = Canvas->TextHeight("L" + String(CpRun.LayerCfg.Panel.Layer[i]->Tag));

		fx = (CpRun.LayerCfg.Panel.Layer[i]->Width / 2) - (fw / 2);
		fh = (CpRun.LayerCfg.Panel.Layer[i]->Height / 2) - (fh / 2);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->TextOut(fx, fh, "L" + String(CpRun.LayerCfg.Panel.Layer[i]->Tag));
	}

	//最高优先级的层，设置后会自动打开

	CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID = LayerID;
	CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[LayerID] = 1;
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Brush->Color = TColor(0x004080FF); //最高优先级的层
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Pen->Color = TColor(0x0000ECEC);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Rectangle(0, 0, CpRun.LayerCfg.Panel.Layer[LayerID]->Width,
		CpRun.LayerCfg.Panel.Layer[LayerID]->Height);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Pen->Color = TColor(0x00977326);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Rectangle(1, 1, CpRun.LayerCfg.Panel.Layer[LayerID]->Width - 1,
		CpRun.LayerCfg.Panel.Layer[LayerID]->Height - 1);

	fw = Canvas->TextWidth("L" + String(CpRun.LayerCfg.Panel.Layer[LayerID]->Tag));
	fh = Canvas->TextHeight("L" + String(CpRun.LayerCfg.Panel.Layer[LayerID]->Tag));

	fx = (CpRun.LayerCfg.Panel.Layer[LayerID]->Width / 2) - (fw / 2);
	fh = (CpRun.LayerCfg.Panel.Layer[LayerID]->Height / 2) - (fh / 2);
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
	CpRun.LayerCfg.Panel.Layer[LayerID]->Canvas->TextOut(fx, fh, "L" + String(CpRun.LayerCfg.Panel.Layer[LayerID]->Tag));

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FlayerSetPoint(void)//设置关联性
{
	CpRun.LayerCfg.Panel.Layer[0] = FLIcon0;
	CpRun.LayerCfg.Panel.Layer[1] = FLIcon1;
	CpRun.LayerCfg.Panel.Layer[2] = FLIcon2;
	CpRun.LayerCfg.Panel.Layer[3] = FLIcon3;
	CpRun.LayerCfg.Panel.Layer[4] = FLIcon4;
	CpRun.LayerCfg.Panel.Layer[5] = FLIcon5;
	CpRun.LayerCfg.Panel.Layer[6] = FLIcon6;
	CpRun.LayerCfg.Panel.Layer[7] = FLIcon7;
	FlayerDispData();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FlayerDispData(void)//显示当前层的配置
{
	int fw, fh, fx;
	for(int i = 0; i < LAYER_DEF_FLAYER_MAX_NUM; i++)
	{
		if(CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[i])
		{
			CpRun.LayerCfg.Panel.Layer[i]->Canvas->Brush->Color = TColor(0x0099E173); //开启状态
		}
		else
		{
			CpRun.LayerCfg.Panel.Layer[i]->Canvas->Brush->Color = TColor(0x00BBBBBB); //关闭状态
		}
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Pen->Color = TColor(0x0000ECEC);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Rectangle(0, 0, CpRun.LayerCfg.Panel.Layer[i]->Width, CpRun.LayerCfg.Panel.Layer[i]->Height);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Pen->Color = TColor(0x00977326);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Rectangle(1, 1, CpRun.LayerCfg.Panel.Layer[i]->Width - 1, CpRun.LayerCfg.Panel.Layer[i]->Height - 1);

		fw = Canvas->TextWidth("L" + String(CpRun.LayerCfg.Panel.Layer[i]->Tag));
		fh = Canvas->TextHeight("L" + String(CpRun.LayerCfg.Panel.Layer[i]->Tag));

		fx = (CpRun.LayerCfg.Panel.Layer[i]->Width / 2) - (fw / 2);
		fh = (CpRun.LayerCfg.Panel.Layer[i]->Height / 2) - (fh / 2);
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
		CpRun.LayerCfg.Panel.Layer[i]->Canvas->TextOut(fx, fh, "L" + String(CpRun.LayerCfg.Panel.Layer[i]->Tag));

	}
	CpRun.LayerCfg.Cfg.FlayerCfg.LayerEnable[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID] = 1;
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Brush->Color = TColor(0x004080FF); //最高优先级的层

	//画外框
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Pen->Color = TColor(0x0000ECEC);
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Rectangle(0, 0,
		CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Width,
		CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Height);
	//画内框
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Pen->Color = TColor(0x00977326);
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Rectangle(1, 1,
		CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Width - 1,
		CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Height - 1);

	//画文字
	fw = Canvas->TextWidth("L" + String(CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Tag));
	fh = Canvas->TextHeight("L" + String(CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Tag));

	fx = (CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Width / 2) - (fw / 2);
	fh = (CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Height / 2) - (fh / 2);
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
	CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Canvas->TextOut(fx, fh,
		"L" + String(CpRun.LayerCfg.Panel.Layer[CpRun.LayerCfg.Cfg.FlayerCfg.TopPriorityID]->Tag));

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button11Click(TObject *Sender)
{
	//上传层配置
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	dynamic_cast <TButton *> (Sender)->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_LAYER_GET_DATA, 0, &CpRun.LayerCfg.Cfg, sizeof(CpRun.LayerCfg.Cfg));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	dynamic_cast <TButton *> (Sender)->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FLayerGetDataFromDevice(void *pData)//从设备获取数据
{
	//获取配置
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *) pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[512]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.LayerCfg.Cfg))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&CpRun.LayerCfg.Cfg, RecvDataBuff, sizeof(CpRun.LayerCfg.Cfg));
		//显示内容
		FlayerDispData();
		TLayerDispData();
		//要求获取LED的配置
		SendCmd(USB_DEF_CMD_LED_CFG_NEED_DATA, 0, 0);
	}
}

void __fastcall TMainForm::TLayerDispData(void)//显示触发层的配置
{
	if(CpRun.LayerCfg.Cfg.TlayerCfg.ActiveID >= LAYER_DEF_TLAYER_MAX_NUM)
	{
		return;
	}

	ComboBox3->ItemIndex = CpRun.LayerCfg.Cfg.TlayerCfg.ActiveID;
}

void __fastcall TMainForm::ComboBox3Change(TObject *Sender)
{
	//激活触发层选择
	CpRun.LayerCfg.Cfg.TlayerCfg.ActiveID = ComboBox3->ItemIndex;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::MacroInitEditer(void)//初始化宏的编辑器
{

	int i;
	TListItem *item;
	for(i = 0; i < LAYER_DEF_MACRO_ONE_GROUP_MAX_STEP; i++)
	{
		item = MacroEdit->Items->Add();
		item->Caption = i;
		item->SubItems->Add("");
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::MacroEditDblClick(TObject *Sender)
{
	//被双击
	TFuncSel *FSel = new TFuncSel(this, MainForm, &CpRun.MacroData[ComboBox4->ItemIndex].Step[MacroEdit->Selected->Index], 2);
	FSel->ShowModal();
	delete FSel;
	MacroEdit->Selected->SubItems->Strings[0] = GetAllFunctionHintStr(&CpRun.MacroData[ComboBox4->ItemIndex].Step[MacroEdit->Selected->Index], 2);
	//取得字符串

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::MacroDispAllData(int gid)//显示选择的组的所有数据
{
	int i;
	for(i = 0; i < LAYER_DEF_MACRO_ONE_GROUP_MAX_STEP; i++)
	{
		MacroEdit->Items->Item[i]->SubItems->Strings[0] = GetAllFunctionHintStr(&CpRun.MacroData[gid].Step[i], 2);
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBox4Change(TObject *Sender)
{
	MacroDispAllData(ComboBox4->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button13Click(TObject *Sender)
{
	//下载宏数据
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button13->Enabled = false;
	SendCmd(USB_DEF_CMD_MACRO_NEED_DATA, ComboBox4->ItemIndex, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::MacroGetDataFromDevice(void *pData)//获取设备发来的数据
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *)pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[4096]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.MacroData[0]))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&CpRun.MacroData[PackData->LongDataPack.id], RecvDataBuff, sizeof(CpRun.MacroData[0]));
		Button13->Enabled = true;
		//显示内容
		MacroDispAllData(PackData->LongDataPack.id);
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button14Click(TObject *Sender)
{
	//上传
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button14->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_MACRO_GET_DATA, ComboBox4->ItemIndex, &CpRun.MacroData[ComboBox4->ItemIndex], sizeof(CpRun.MacroData[0]));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button14->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button16Click(TObject *Sender)
{
	//读取配置
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "MacroData(*.MCfg)\0*.MCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Select a File...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "MCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(225)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	uint16_t tmp;
	DWORD ReadSize;
	ReadFile(hHandle, &tmp, sizeof(tmp), &ReadSize, 0);
	if(tmp != CP_DEF_MACRO_CFG_FILE_HEAD)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(226)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		CloseHandle(hHandle);
		return;
	}
	ReadFile(hHandle, &CpRun.MacroData[ComboBox4->ItemIndex], sizeof(CpRun.MacroData[ComboBox4->ItemIndex]), &ReadSize, NULL);
	CloseHandle(hHandle);
	if(ReadSize != sizeof(CpRun.MacroData[ComboBox4->ItemIndex]))
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(227)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	MacroDispAllData(ComboBox4->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button15Click(TObject *Sender)
{
	//保存触发层数据
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "MacroData(*.MCfg)\0*.MCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Set a File Name...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "MCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetSaveFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(223)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	DWORD WriteFileSize;
	uint16_t tmp = CP_DEF_MACRO_CFG_FILE_HEAD;
	WriteFile(hHandle, &tmp, sizeof(tmp), &WriteFileSize, NULL);
	WriteFile(hHandle, &CpRun.MacroData[ComboBox4->ItemIndex], sizeof(CpRun.MacroData[ComboBox4->ItemIndex]), &WriteFileSize, NULL);
	CloseHandle(hHandle);
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::LockKeyEditDblClick(TObject *Sender)
{
	//锁定键编辑器被双击
	int nWidth, nHeight;
	int nX, nY;
	int nCount, nCol, nRow;
	TRect Rect;
	TPoint Pos;
	TListItem *Item;
	union FUNC_UNION_COMMAND_LIST Func;
	memset(&Func, 0x00, sizeof(Func));

	Pos = LockKeyEdit->ScreenToClient(Mouse->CursorPos);
	Item = LockKeyEdit->GetItemAt(Pos.x, Pos.y);
	if(Item == NULL)
	{
		return;
	}
	nCount = LockKeyEdit->Columns->Count;
	nX = -GetScrollPos(LockKeyEdit->Handle, SB_HORZ);
	for(nCol = 0; nCol < nCount; nX += nWidth, nCol++)
	{
		nWidth = ListView_GetColumnWidth(LockKeyEdit->Handle, nCol);
		if(Pos.x <= nX + nWidth)
		{
			break;
		}
	}
	if(nCol == nCount)
	{
		return;
	}
	if(nX < 0)
	{
		nWidth += nX;
		nX = 0;
	}
	nRow = LockKeyEdit->ItemIndex;

	switch(nCol)
	{
		case 0: //0不可被编辑
		{
			return;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		{
			TFuncSel *FSel = new TFuncSel(this, MainForm, &Func, 3);
			FSel->ShowModal();
			delete FSel;
			KeyLockGetKeys(&Func, nCol, nRow); //设置功能
		} break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::KeyLockInitEditer(void)//初始化锁定键编辑器
{
	int index;
	TListItem *item;

	for(index = 0; index < LAYER_DEF_KEY_LOCK_MAX_GROUP; index++)
	{
		item = LockKeyEdit->Items->Add();
		item->Caption = index;
		item->SubItems->Add("");
		item->SubItems->Add("");
		item->SubItems->Add("");
		item->SubItems->Add("");
		item->SubItems->Add("");
		item->SubItems->Add("");
		item->SubItems->Add("");
		item->SubItems->Add("");
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::KeyLockGetKeys(union FUNC_UNION_COMMAND_LIST *Func, int nCol, int nRow)//设置功能并显示
{
	uint16_t KeyCode;
	String str;
	switch(Func->NormalKey.FuncID)
	{
		case FUNC_DEF_CMD_NORMAL_KEY_CODE: //常规按键
		{
			KeyCode = Func->NormalKey.KeyCode;
		} break;
		case FUNC_DEF_CMD_MEDIA_KEY_CODE: //多媒体按键
		{
			KeyCode = Func->MediaKey.MediaKeyCode | 0x8000; //多媒体键码最高位是1
		} break;
	}
	str = GetAllFunctionHintStr(Func, 1);
	CpRun.KeyLockData[nRow].KeyCode[nCol - 1] = KeyCode;
	LockKeyEdit->Items->Item[nRow]->SubItems->Strings[nCol - 1] = str;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LockKeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//鼠标被按下

	int nWidth, nHeight;
	int nX, nY;
	int nCount, nCol, nRow;
	TRect Rect;
	TPoint Pos;
	TListItem *Item;
	union FUNC_UNION_COMMAND_LIST Func;

	if(Button != mbRight)
	{
		CpRun.KeyLockEditer.nCol = 0;
		return; //如果不是右键，则返回
	}

	memset(&Func, 0x00, sizeof(Func));

	Pos = LockKeyEdit->ScreenToClient(Mouse->CursorPos);
	Item = LockKeyEdit->GetItemAt(Pos.x, Pos.y);
	if(Item == NULL)
	{
		return;
	}
	nCount = LockKeyEdit->Columns->Count;
	nX = -GetScrollPos(LockKeyEdit->Handle, SB_HORZ);
	for(nCol = 0; nCol < nCount; nX += nWidth, nCol++)
	{
		nWidth = ListView_GetColumnWidth(LockKeyEdit->Handle, nCol);
		if(Pos.x <= nX + nWidth)
		{
			break;
		}
	}
	if(nCol == nCount)
	{
		return;
	}
	if(nX < 0)
	{
		nWidth += nX;
		nX = 0;
	}
	CpRun.KeyLockEditer.nRow = Item->Index;
	CpRun.KeyLockEditer.nCol = nCol;

	KeyLockPopMenu->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMainForm::N1Click(TObject *Sender)
{
	//
	TMenuItem *item = dynamic_cast <TMenuItem *> (Sender);
	//0=清除当前单元格
	//1=清除选择的所有组
	//3=清空
	switch(item->Tag)
	{
		case 0:
		{
			if(CpRun.KeyLockEditer.nCol == 0)
			{
				return;
			}
			CpRun.KeyLockData[CpRun.KeyLockEditer.nRow].KeyCode[CpRun.KeyLockEditer.nCol - 1] = 0;
			LockKeyEdit->Items->Item[CpRun.KeyLockEditer.nRow]->SubItems->Strings[CpRun.KeyLockEditer.nCol - 1] = "";
		} break;
		case 1:
		{
			memset(&CpRun.KeyLockData[CpRun.KeyLockEditer.nRow], 0, sizeof(CpRun.KeyLockData[CpRun.KeyLockEditer.nRow]));
			for(int i = 0; i < LAYER_DEF_KEY_LOCK_GROUP_MAX_KEYS; i++)
			{
				LockKeyEdit->Items->Item[CpRun.KeyLockEditer.nRow]->SubItems->Strings[i] = "";
			}
		} break;
		case 2:
		{
			for(int index = 0; index < LAYER_DEF_KEY_LOCK_MAX_GROUP; index++)
			{
				if(LockKeyEdit->Items->Item[index]->Selected == NULL)
				{
					continue;
				}
				memset(&CpRun.KeyLockData[index], 0, sizeof(CpRun.KeyLockData[index]));
				for(int i = 0; i < LAYER_DEF_KEY_LOCK_GROUP_MAX_KEYS; i++)
				{
					LockKeyEdit->Items->Item[index]->SubItems->Strings[i] = "";
				}
			}
		} break;
	}
}

void __fastcall TMainForm::Button17Click(TObject *Sender)
{
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button17->Enabled = false;
	SendCmd(USB_DEF_CMD_KEY_LOCK_NEED_DATA, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::KeyLockGetDataFromDevice(void *pData)//获取设备发来的数据
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *)pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[4096]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.KeyLockData))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(CpRun.KeyLockData, RecvDataBuff, sizeof(CpRun.KeyLockData));
		Button17->Enabled = true;
		//显示内容
		KeyLockDispAllData();
	}
}

//-------------------------------------------------------------------------------
void __fastcall TMainForm::KeyLockDispAllData(void)
{
	union FUNC_UNION_COMMAND_LIST Func;
	int nRow, nCol;
	for(nRow = 0; nRow < LAYER_DEF_KEY_LOCK_MAX_GROUP; nRow++)
	{
		for(nCol = 0; nCol < LAYER_DEF_KEY_LOCK_GROUP_MAX_KEYS; nCol++)
		{
			if(CpRun.KeyLockData[nRow].KeyCode[nCol] & 0x8000)//判断是否是多媒体按键
			{
				//是多媒体按键
				Func.MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE;
				Func.MediaKey.MediaKeyCode = CpRun.KeyLockData[nRow].KeyCode[nCol] & 0x7fff;
				if(Func.MediaKey.MediaKeyCode < 0x00b0)
				{
					LockKeyEdit->Items->Item[nRow]->SubItems->Strings[nCol] = "";
				}
				else
				{
					LockKeyEdit->Items->Item[nRow]->SubItems->Strings[nCol] = GetAllFunctionHintStr(&Func, 1);
				}
			}
			else //普通按键
			{
				Func.NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_CODE;
				Func.NormalKey.KeyCode = CpRun.KeyLockData[nRow].KeyCode[nCol];
				if(Func.NormalKey.KeyCode < 4)//键码是以4开启
				{
					LockKeyEdit->Items->Item[nRow]->SubItems->Strings[nCol] = "";
				}
				else
				{
					LockKeyEdit->Items->Item[nRow]->SubItems->Strings[nCol] = GetAllFunctionHintStr(&Func, 1);
				}
			}
		}
	}

}

void __fastcall TMainForm::Button18Click(TObject *Sender)
{
	//上传
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button18->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_KEY_LOCK_GET_DATA, 0, &CpRun.KeyLockData, sizeof(CpRun.KeyLockData));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button18->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button19Click(TObject *Sender)
{
	//保存触发层数据
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "KeyLockData(*.KCfg)\0*.KCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Set a File Name...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "KCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetSaveFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(223)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	DWORD WriteFileSize;
	uint16_t tmp = CP_DEF_KEYLOCK_CFG_FILE_HEAD;
	WriteFile(hHandle, &tmp, sizeof(tmp), &WriteFileSize, NULL);
	WriteFile(hHandle, &CpRun.KeyLockData, sizeof(CpRun.KeyLockData), &WriteFileSize, NULL);
	CloseHandle(hHandle);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button20Click(TObject *Sender)
{
	//读取配置
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "KeyLockData(*.KCfg)\0*.KCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Select a File...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "KCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(225)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	uint16_t tmp;
	DWORD ReadSize;
	ReadFile(hHandle, &tmp, sizeof(tmp), &ReadSize, 0);
	if(tmp != CP_DEF_KEYLOCK_CFG_FILE_HEAD)
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(226)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		CloseHandle(hHandle);
		return;
	}
	ReadFile(hHandle, &CpRun.KeyLockData, sizeof(CpRun.KeyLockData), &ReadSize, NULL);
	CloseHandle(hHandle);
	if(ReadSize != sizeof(CpRun.KeyLockData))
	{
		MessageBox(Application->Handle, AnsiString(LanguageGetStr(227)).c_str(), AnsiString(LanguageGetStr(224)).c_str(), MB_OK);
		return;
	}
	KeyLockDispAllData();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MacroEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//鼠标按下
	if(Button != mbRight)
	{
		return;
	}

	CpRun.MacroEditer.nRow = MacroEdit->Selected->Index;
	MacroEditPopMenu->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N5Click(TObject *Sender)
{
	//右键菜单
	TMenuItem *item = dynamic_cast <TMenuItem *> (Sender);

	switch(item->Tag)
	{
		case 0: //清除当前选择的行的数据
		{
			memset(&CpRun.MacroData[ComboBox4->ItemIndex].Step[CpRun.MacroEditer.nRow], 0,
				sizeof(CpRun.MacroData[ComboBox4->ItemIndex].Step[CpRun.MacroEditer.nRow]));
			MacroEdit->Items->Item[CpRun.MacroEditer.nRow]->SubItems->Strings[0] = "";
		} break;
		case 1: //清除当前选择的行
		{
			for(int index = 0; index < LAYER_DEF_MACRO_ONE_GROUP_MAX_STEP; index++)
			{
				if(MacroEdit->Items->Item[index]->Selected == NULL)
				{
					continue;
				}
				memset(&CpRun.MacroData[ComboBox4->ItemIndex].Step[index], 0, sizeof(CpRun.MacroData[ComboBox4->ItemIndex].Step[index]));
				MacroEdit->Items->Item[index]->SubItems->Strings[0] = "";
			}
		} break;
	}

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LedCfgGetDataFromDevice(void *pData)//获取设备发来的LED配置信息
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *)pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[512]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.BklConfig.LedCfg))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&CpRun.BklConfig.LedCfg, RecvDataBuff, sizeof(CpRun.BklConfig.LedCfg));
		//显示内容
		LedCfgDispAllData();
		WsCfgDispGroupInfo(); //显示WS2812的配置
		StLedDispAllCfg(); //显示LED的动作配置
		CustomDispCfg(); //显示自定义背光的配置
		UsbDisConn_ShowCfg(); //显示USB断开后是否关闭背光的配置
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button25Click(TObject *Sender)
{
	//上一个灯效
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	SendCmd(USB_DEF_CMD_LED_CFG_LAST_BKL_EFF, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button26Click(TObject *Sender)
{
	//下一个灯效
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	SendCmd(USB_DEF_CMD_LED_CFG_NEXT_BKL_EFF, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LedCfgDispAllData(void)//显示当前的配置
{
	//灯效索引
	TrackBar4->Min = 0;
	TrackBar4->Max = EFF_DEF_BKL_CUSTOM_MAX_EFFECT + CpRun.BklConfig.LedCfg.BklEffectMax - 1;
	TrackBar4->Position = CpRun.BklConfig.LedCfg.EffectCfg.EffectIndex;

	//运行速度
	if(CpRun.BklConfig.LedCfg.EffectCfg.EffectIndex < EFF_DEF_BKL_MAX_EFFECT)
	{
		TrackBar5->Position = CpRun.BklConfig.LedCfg.EffectCfg.EffectSpeed[CpRun.BklConfig.LedCfg.EffectCfg.EffectIndex];
	}
	else
	{
		TrackBar5->Position = CpRun.BklConfig.LedCfg.CustomLedCfg[CpRun.BklConfig.LedCfg.EffectCfg.EffectIndex - EFF_DEF_BKL_MAX_EFFECT].Speed;
	}
	//灯效参数
	if(CpRun.BklConfig.LedCfg.EffectCfg.EffectIndex < EFF_DEF_BKL_MAX_EFFECT)
	{
		TrackBar6->Enabled = true;
		Button24->Enabled = true;
		TrackBar6->Position = CpRun.BklConfig.LedCfg.EffectCfg.LigDownLevelStep[CpRun.BklConfig.LedCfg.EffectCfg.EffectIndex];
	}
	else
	{
		TrackBar6->Enabled = false;
		Button24->Enabled = false;
		TrackBar6->Position = 0;
	}
	//颜色通道亮度
	TrackBar1->Position = CpRun.BklConfig.LedCfg.EffectCfg.BackLigLevel.R;
	TrackBar2->Position = CpRun.BklConfig.LedCfg.EffectCfg.BackLigLevel.G;
	TrackBar3->Position = CpRun.BklConfig.LedCfg.EffectCfg.BackLigLevel.B;

	//显示兼容状态灯的数据
	LedCfgDispIndicator();

}

void __fastcall TMainForm::Button21Click(TObject *Sender)
{
	//设置亮度等级
	union CP_UNION_ONE_PACK_64BYTE Pack;
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button21->Enabled = false;
	Pack.Bkl_LigLevel.Cmd = USB_DEF_CMD_LED_CFG_SET_RGB_CH_LEVEL; //设置亮度等级
	Pack.Bkl_LigLevel.R = TrackBar1->Position;
	Pack.Bkl_LigLevel.G = TrackBar2->Position;
	Pack.Bkl_LigLevel.B = TrackBar3->Position;
	MyHid->SendDataByFifo(0, &Pack);
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button21->Enabled = true;

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button22Click(TObject *Sender)
{
	//设置指定的灯效
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	SendCmd(USB_DEF_CMD_LED_CFG_SET_BKL_EFF, TrackBar4->Position, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button23Click(TObject *Sender)
{
	//灯效速度
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	SendCmd(USB_DEF_CMD_LED_CFG_SET_SPEED, TrackBar5->Position, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button24Click(TObject *Sender)
{
	//灯效衰减的幅度
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	SendCmd(USB_DEF_CMD_LED_CFG_SET_STEP, TrackBar6->Position, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LedCfgDispIndicatorPoint(int Leds)//显示LED当前选择的坐标
{
	switch(Leds)
	{
		case 0:
		{
			if(RadioButton1->Checked)
			{
				Label11->Caption = LanguageGetStr(88) + String(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.UserLed_x) + ":" +
					String(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.UserLed_y);
			}
			else
			{
				Label11->Caption = LanguageGetStr(94) + String(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.WsLed.WsLedPoint);
			}
		} break;
		case 1:
		{
			if(RadioButton11->Checked)
			{
				Label12->Caption = LanguageGetStr(88) + String(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.UserLed_x) + ":" +
					String(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.UserLed_y);
			}
			else
			{
				Label12->Caption = LanguageGetStr(94) + String(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.WsLed.WsLedPoint);
			}
		} break;
		case 2:
		{
			if(RadioButton17->Checked)
			{
				Label13->Caption = LanguageGetStr(88) + String(CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.UserLed_x) + ":" +
					String(CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.UserLed_y);
			}
			else
			{
				Label13->Caption = LanguageGetStr(94) + String(CpRun.BklConfig.LedCfg.IndicatorCfg.Num.WsLed.WsLedPoint);
			}
		} break;
	}

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LedCfgDispIndicator(void)//显示兼容LED的状态
{
	//GetRValue(Image1->Canvas->Brush->Color);
	//CAPS
	CheckBox1->Checked = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Enable;
	if(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.WorkMode == 0)
	{
		RadioButton1->Checked = true;
	}
	else
	{
		RadioButton2->Checked = true;
	}
	LedCfgDispIndicatorPoint(0);

	//跟随状态
	CheckBox4->Checked = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.UseBklEffect;

	switch(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Effect)
	{
		case 0:
		{
			RadioButton3->Checked = true;
		} break;
		case 1:
		{
			RadioButton4->Checked = true;
		} break;
		case 2:
		{
			RadioButton5->Checked = true;
		} break;
		case 3:
		{
			RadioButton6->Checked = true;
		} break;
	}

	//图像
	Image1->Canvas->Brush->Color =
		TColor(RGB(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.FixColor.R, CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.FixColor.G,
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.FixColor.B));
	Image1->Canvas->Rectangle(0, 0, Image1->Width, Image1->Height);

	//SCROLL
	CheckBox2->Checked = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Enable;
	if(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.WorkMode == 0)
	{
		RadioButton11->Checked = true;
	}
	else
	{
		RadioButton12->Checked = true;
	}
	LedCfgDispIndicatorPoint(1);

	//跟随状态
	CheckBox5->Checked = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.UseBklEffect;

	switch(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Effect)
	{
		case 0:
		{
			RadioButton7->Checked = true;
		} break;
		case 1:
		{
			RadioButton8->Checked = true;
		} break;
		case 2:
		{
			RadioButton9->Checked = true;
		} break;
		case 3:
		{
			RadioButton10->Checked = true;
		} break;
	}

	//图像
	Image2->Canvas->Brush->Color =
		TColor(RGB(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.FixColor.R, CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.FixColor.G,
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.FixColor.B));
	Image2->Canvas->Rectangle(0, 0, Image2->Width, Image2->Height);

	//NUM
	CheckBox3->Checked = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Enable;
	if(CpRun.BklConfig.LedCfg.IndicatorCfg.Num.WorkMode == 0)
	{
		RadioButton17->Checked = true;
	}
	else
	{
		RadioButton18->Checked = true;
	}
	LedCfgDispIndicatorPoint(2);
	//跟随状态
	CheckBox6->Checked = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.UseBklEffect;

	switch(CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Effect)
	{
		case 0:
		{
			RadioButton13->Checked = true;
		} break;
		case 1:
		{
			RadioButton14->Checked = true;
		} break;
		case 2:
		{
			RadioButton15->Checked = true;
		} break;
		case 3:
		{
			RadioButton16->Checked = true;
		} break;
	}

	//图像
	Image3->Canvas->Brush->Color = TColor(RGB(CpRun.BklConfig.LedCfg.IndicatorCfg.Num.FixColor.R, CpRun.BklConfig.LedCfg.IndicatorCfg.Num.FixColor.G,
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.FixColor.B));
	Image3->Canvas->Rectangle(0, 0, Image3->Width, Image3->Height);

}

void __fastcall TMainForm::Image1Click(TObject *Sender)
{
	//选择颜色
	TImage *img = dynamic_cast <TImage *> (Sender);
	uint8_t R;
	uint8_t G;
	uint8_t B;

	ColorDialog1->Color = img->Canvas->Brush->Color;
	if(ColorDialog1->Execute(this->Handle))
	{
		img->Canvas->Brush->Color = ColorDialog1->Color;
		img->Canvas->Rectangle(0, 0, img->Width, img->Height);
		R = GetRValue(ColorDialog1->Color);
		G = GetGValue(ColorDialog1->Color);
		B = GetBValue(ColorDialog1->Color);
	}
	else
	{
		return;
	}

	switch(img->Tag)
	{
		case 0: //CAPS
		{
			CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.FixColor.R = R;
			CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.FixColor.G = G;
			CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.FixColor.B = B;
		} break;
		case 1: //SCROLL
		{
			CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.FixColor.R = R;
			CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.FixColor.G = G;
			CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.FixColor.B = B;
		} break;
		case 2: //num
		{
			CpRun.BklConfig.LedCfg.IndicatorCfg.Num.FixColor.R = R;
			CpRun.BklConfig.LedCfg.IndicatorCfg.Num.FixColor.G = G;
			CpRun.BklConfig.LedCfg.IndicatorCfg.Num.FixColor.B = B;
		} break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBox1Click(TObject *Sender)
{
	//开启关闭的状态
	TCheckBox *cb = dynamic_cast <TCheckBox *> (Sender);
	switch(cb->Tag)
	{
		case 0: //CAPS
		{
			CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Enable = cb->Checked;
		} break;
		case 1: //Scroll
		{
			CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Enable = cb->Checked;
		} break;
		case 2: //Num
		{
			CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Enable = cb->Checked;
		} break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button30Click(TObject *Sender)
{
	//CAPS应用
	union CP_UNION_ONE_PACK_64BYTE Pack;
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}

	CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.WorkMode = RadioButton2->Checked;
	if(RadioButton3->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Effect = 0;
	}
	if(RadioButton4->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Effect = 1;
	}
	if(RadioButton5->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Effect = 2;
	}
	if(RadioButton6->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.Effect = 3;
	}

	Pack.SetIndicatorLed.Cmd = USB_DEF_CMD_LED_CFG_SET_INDICATOR_CFG;
	Pack.SetIndicatorLed.Leds = 0; //CAPS
	memcpy(&Pack.SetIndicatorLed.Data, &CpRun.BklConfig.LedCfg.IndicatorCfg.Caps, sizeof(CpRun.BklConfig.LedCfg.IndicatorCfg.Caps));
	MyHid->SendDataByFifo(0, &Pack);
	MyHid->StartSendThread(); //启动发送线程
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button29Click(TObject *Sender)
{
	//SCROLL应用
	union CP_UNION_ONE_PACK_64BYTE Pack;
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}

	CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.WorkMode = RadioButton12->Checked;
	if(RadioButton7->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Effect = 0;
	}
	if(RadioButton8->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Effect = 1;
	}
	if(RadioButton9->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Effect = 2;
	}
	if(RadioButton10->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.Effect = 3;
	}

	Pack.SetIndicatorLed.Cmd = USB_DEF_CMD_LED_CFG_SET_INDICATOR_CFG;
	Pack.SetIndicatorLed.Leds = 1; //CAPS
	memcpy(&Pack.SetIndicatorLed.Data, &CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll, sizeof(CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll));
	MyHid->SendDataByFifo(0, &Pack);
	MyHid->StartSendThread(); //启动发送线程
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button32Click(TObject *Sender)
{
	//SCROLL应用
	union CP_UNION_ONE_PACK_64BYTE Pack;
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}

	CpRun.BklConfig.LedCfg.IndicatorCfg.Num.WorkMode = RadioButton18->Checked;
	if(RadioButton13->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Effect = 0;
	}
	if(RadioButton14->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Effect = 1;
	}
	if(RadioButton15->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Effect = 2;
	}
	if(RadioButton16->Checked)
	{
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.Effect = 3;
	}

	Pack.SetIndicatorLed.Cmd = USB_DEF_CMD_LED_CFG_SET_INDICATOR_CFG;
	Pack.SetIndicatorLed.Leds = 2; //CAPS
	memcpy(&Pack.SetIndicatorLed.Data, &CpRun.BklConfig.LedCfg.IndicatorCfg.Num, sizeof(CpRun.BklConfig.LedCfg.IndicatorCfg.Num));
	MyHid->SendDataByFifo(0, &Pack);
	MyHid->StartSendThread(); //启动发送线程
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button27Click(TObject *Sender)
{
	//选择灯珠
	struct SEL_STRUCT_ICON_DATA point;
	int x = 0;
	TLedSel *LedSel;

	if(RadioButton1->Checked)
	{
		//使用轴灯
		point.px = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.SrcBkLed_x;
		point.py = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.SrcBkLed_y;
		point.ux = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.UserLed_x;
		point.uy = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.UserLed_y;
		LedSel = new TLedSel(this, MainForm, &point.px, &point.py, &point.ux, &point.uy);
		LedSel->ShowModal();
		delete LedSel;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.SrcBkLed_x = point.px;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.SrcBkLed_y = point.py;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.UserLed_x = point.ux;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.BkLed.UserLed_y = point.uy;
	}
	else
	{
		//使用WS2812
		TnSelect *nSel;
		x = CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.WsLed.WsLedPoint;
		nSel = new TnSelect(this, 8, 4, 0, 31, &x);
		nSel->ShowModal();
		delete nSel;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.WsLed.WsLedPoint = x;
	}
	LedCfgDispIndicatorPoint(0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button28Click(TObject *Sender)
{
	//选择灯珠
	struct SEL_STRUCT_ICON_DATA point;
	int x = 0;
	TLedSel *LedSel;

	if(RadioButton11->Checked)
	{
		//使用轴灯
		point.px = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.SrcBkLed_x;
		point.py = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.SrcBkLed_y;
		point.ux = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.UserLed_x;
		point.uy = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.UserLed_y;

		LedSel = new TLedSel(this, MainForm, &point.px, &point.py, &point.ux, &point.uy);
		LedSel->ShowModal();
		delete LedSel;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.SrcBkLed_x = point.px;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.SrcBkLed_y = point.py;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.UserLed_x = point.ux;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.BkLed.UserLed_y = point.uy;
	}
	else
	{
		//使用WS2812
		TnSelect *nSel;
		x = CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.WsLed.WsLedPoint;
		nSel = new TnSelect(this, 8, 4, 0, 31, &x);
		nSel->ShowModal();
		delete nSel;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.WsLed.WsLedPoint = x;
	}
	LedCfgDispIndicatorPoint(1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button31Click(TObject *Sender)
{
	//选择灯珠
	struct SEL_STRUCT_ICON_DATA point;
	int x = 0;
	TLedSel *LedSel;

	if(RadioButton17->Checked)
	{
		//使用轴灯
		point.px = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.SrcBkLed_x;
		point.py = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.SrcBkLed_y;
		point.ux = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.UserLed_x;
		point.uy = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.UserLed_y;
		LedSel = new TLedSel(this, MainForm, &point.px, &point.py, &point.ux, &point.uy);
		LedSel->ShowModal();
		delete LedSel;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.SrcBkLed_x = point.px;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.SrcBkLed_y = point.py;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.UserLed_x = point.ux;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.BkLed.UserLed_y = point.uy;
	}
	else
	{
		//使用WS2812
		TnSelect *nSel;
		x = CpRun.BklConfig.LedCfg.IndicatorCfg.Num.WsLed.WsLedPoint;
		nSel = new TnSelect(this, 8, 4, 0, 31, &x);
		nSel->ShowModal();
		delete nSel;
		CpRun.BklConfig.LedCfg.IndicatorCfg.Num.WsLed.WsLedPoint = x;
	}
	LedCfgDispIndicatorPoint(2);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IndicatorModeSelectClick(TObject *Sender)
{
	//显示选择的坐标
	LedCfgDispIndicatorPoint(dynamic_cast <TRadioButton *> (Sender)->Tag);
}

void __fastcall TMainForm::CheckBox4Click(TObject *Sender)
{
	//跟随状态
	CpRun.BklConfig.LedCfg.IndicatorCfg.Caps.UseBklEffect = CheckBox4->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBox5Click(TObject *Sender)
{
	//跟随状态
	CpRun.BklConfig.LedCfg.IndicatorCfg.Scroll.UseBklEffect = CheckBox5->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBox6Click(TObject *Sender)
{
	//跟随状态
	CpRun.BklConfig.LedCfg.IndicatorCfg.Num.UseBklEffect = CheckBox6->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ShowBalloonTip(TWinControl *Control, int nIcon, LPSTR lpTitle, LPSTR lpText, TColor clBack, TColor clFore)
{
	HWND hWnd = Control->Handle;
	HWND hWndTip = CreateWindow(TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_BALLOON | TTS_ALWAYSTIP | TTS_CLOSE, 0, 0, 0, 0, hWnd, 0,
		HInstance, NULL);
	TOOLINFO ti;
	if(hWndTip != 0)
	{
		SetWindowPos(hWndTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		ti.cbSize = sizeof(ti);
		ti.uFlags = TTF_CENTERTIP | TTF_TRANSPARENT | TTF_SUBCLASS;
		ti.hwnd = hWnd;
		ti.lpszText = lpText;
		::GetClientRect(hWnd, &ti.rect);
		SendMessage(hWndTip, TTM_SETTIPBKCOLOR, clBack, 0);
		SendMessage(hWndTip, TTM_SETTIPTEXTCOLOR, clFore, 0);
		SendMessage(hWndTip, TTM_ADDTOOL, 1, LPARAM(&ti));
		SendMessage(hWndTip, TTM_SETTITLE, nIcon % 4, LPARAM(lpTitle));
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::WsCfgDispGroupInfo(void)//显示某一组的配置
{
	//显示某一组的配置
	uint8_t R, G, B;
	if(ComboBox5->ItemIndex >= EFF_DEF_WS_GROUP_MAX)
	{
		return;
	}
	CheckBox7->Checked = CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Enable;
	ComboBox6->ItemIndex = CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].xMin;
	ComboBox7->ItemIndex = CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].xMax;
	CpRun.BklConfig.WSLedRun.rbtn[CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Effect]->Checked = true;

	WS_Speed->Position = CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Speed;

	for(int i = 0; i < EFF_DEF_WS_SET_COLOR_MAX; i++)
	{
		//反色处理
		R = 0xff - CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[i].R;
		G = 0xff - CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[i].G;
		B = 0xff - CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[i].B;

		CpRun.BklConfig.WSLedRun.img[i]->Canvas->Brush->Color =
			TColor(RGB(CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[i].R, CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[i].G,
			CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[i].B));

		CpRun.BklConfig.WSLedRun.img[i]->Canvas->Rectangle(0, 0, CpRun.BklConfig.WSLedRun.img[i]->Width, CpRun.BklConfig.WSLedRun.img[i]->Height);
		CpRun.BklConfig.WSLedRun.img[i]->Canvas->Font->Color = TColor(RGB(R, G, B));
		CpRun.BklConfig.WSLedRun.img[i]->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
		CpRun.BklConfig.WSLedRun.img[i]->Canvas->TextOut(20, 20, i);
	}
}

void __fastcall TMainForm::ComboBox5Change(TObject *Sender)
{
	WsCfgDispGroupInfo();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::WS_ImgClick(TObject *Sender)//WS2812灯珠颜色选择
{
	//单击选择颜色
	uint8_t R, G, B;
	TImage *img = dynamic_cast <TImage *> (Sender);
	ColorDialog1->Color = img->Canvas->Brush->Color;
	if(ColorDialog1->Execute(this->Handle))
	{
		img->Canvas->Brush->Color = ColorDialog1->Color;
		img->Canvas->Rectangle(0, 0, img->Width, img->Height);

		R = 0xff - (uint8_t)(GetRValue(ColorDialog1->Color));
		G = 0xff - (uint8_t)(GetGValue(ColorDialog1->Color));
		B = 0xff - (uint8_t)(GetBValue(ColorDialog1->Color));

		img->Canvas->Font->Color = TColor(RGB(R, G, B));
		img->Canvas->Font->Style = TFontStyles() << fsBold; //粗体

		img->Canvas->TextOut(20, 20, img->Tag);
		CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[img->Tag].R = GetRValue(ColorDialog1->Color);
		CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[img->Tag].G = GetGValue(ColorDialog1->Color);
		CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Color[img->Tag].B = GetBValue(ColorDialog1->Color);
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::wsCfgGetGroupInfo(void)//获得当前组的配置
{
	CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].xMin = ComboBox6->ItemIndex;
	CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].xMax = ComboBox7->ItemIndex;
	CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Speed = WS_Speed->Position;
	CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Enable = CheckBox7->Checked;
	for(int i = 0; i < EFF_DEF_WS_EFF_MAX; i++)
	{
		if(CpRun.BklConfig.WSLedRun.rbtn[i]->Checked)
		{
			CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex].Effect = i;
			break;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button33Click(TObject *Sender)
{
	//上传
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	//结束坐标，必须比开始坐标大
	if(ComboBox6->ItemIndex > ComboBox7->ItemIndex)
	{
		ShowMessage(LanguageGetStr(320));
		return;
	}

	wsCfgGetGroupInfo();

	Button33->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_LED_CFG_SET_WS_LED, ComboBox5->ItemIndex, &CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex],
		sizeof(CpRun.BklConfig.LedCfg.WsCfg[ComboBox5->ItemIndex]));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button33->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::StLedDispAllCfg(void)//显示所有的配置
{
	//
	CheckBox8->Checked = CpRun.BklConfig.LedCfg.LedAction[0].Enable;
	CheckBox9->Checked = CpRun.BklConfig.LedCfg.LedAction[1].Enable;
	CheckBox10->Checked = CpRun.BklConfig.LedCfg.LedAction[2].Enable;

	Panel23->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[0].OnFunc, 1);
	Panel24->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[0].OffFunc, 1);

	Panel25->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[1].OnFunc, 1);
	Panel26->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[1].OffFunc, 1);

	Panel27->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[2].OnFunc, 1);
	Panel28->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[2].OffFunc, 1);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CheckBox10Click(TObject *Sender)
{
	//
	CpRun.BklConfig.LedCfg.LedAction[dynamic_cast <TCheckBox *> (Sender)->Tag].Enable = dynamic_cast <TCheckBox *> (Sender)->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button34Click(TObject *Sender)
{
	//上传
	TButton *btn = dynamic_cast <TButton *> (Sender);
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}

	btn->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_LED_CFG_SET_ST_LED_ACTION, btn->Tag, &CpRun.BklConfig.LedCfg.LedAction[btn->Tag],
		sizeof(CpRun.BklConfig.LedCfg.LedAction[btn->Tag]));
	MyHid->StartSendThread();
	//等待发送完毕
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages(); //等待的同时，还要响应其他消息
	}
	btn->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Panel23MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//
	TPanel *Pan = dynamic_cast <TPanel *> (Sender);
	TFuncSel *FSel;

	if(Button == mbLeft)
	{
		FSel = new TFuncSel(this, MainForm, &CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OnFunc, 1);
		FSel->ShowModal();
		delete FSel;
		Pan->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OnFunc, 1);
	}
	else
	{
		if(Button == mbRight)
		{
			memset(&CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OnFunc, 0, sizeof(CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OnFunc));
			Pan->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OnFunc, 1);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Panel24MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	//
	TPanel *Pan = dynamic_cast <TPanel *> (Sender);
	TFuncSel *FSel;

	if(Button == mbLeft)
	{
		FSel = new TFuncSel(this, MainForm, &CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OffFunc, 1);
		FSel->ShowModal();
		delete FSel;
		Pan->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OffFunc, 1);
	}
	else
	{
		if(Button == mbRight)
		{
			memset(&CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OffFunc, 0, sizeof(CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OffFunc));
			Pan->Caption = GetAllFunctionHintStr(&CpRun.BklConfig.LedCfg.LedAction[Pan->Tag].OffFunc, 1);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::InitCustomEdit(void)//创建自定义背光编辑器
{
	int x, y;
	int Matrix_x, Matrix_y;
	uint16_t tag;

	//如果坐标为0，则不创建编辑器
	if(PhyCfg.LedCfg.LEDMatrixCfg.LED_X == 0 || PhyCfg.LedCfg.LEDMatrixCfg.LED_Y == 0)
	{
		return;
	}

	for(x = 0; x < CUSTOM_BKL_IMAGE_MAX; x++)
	{
		ComboBox9->Items->Add(x);
		ComboBox10->Items->Add(x);
	}
	ComboBox10->ItemIndex = 0;

	//判断是否需要重建显示矩阵
	if((CpRun.BklConfig.CustEff.UserX != PhyCfg.LedCfg.LEDMatrixCfg.LED_X) || (CpRun.BklConfig.CustEff.UserY != PhyCfg.LedCfg.LEDMatrixCfg.LED_Y))
	{
		//如果任何一个不同，则需要重建显示矩阵，先销毁之前建立的显示矩阵
		if(CpRun.BklConfig.CustEff.SrcImg != NULL)
		{
			delete CpRun.BklConfig.CustEff.SrcImg;
			CpRun.BklConfig.CustEff.SrcImg = NULL;
		}
		//销毁图标信息
		memset(&CpRun.BklConfig.CustEff.IconInfo, 0, sizeof(CpRun.BklConfig.CustEff.IconInfo));
		//销毁图标
		for(x = 0; x < CpRun.BklConfig.CustEff.UserX; x++)
		{
			for(y = 0; y < CpRun.BklConfig.CustEff.UserY; y++)
			{
				if(CpRun.BklConfig.CustEff.Kbicon[x][y] != NULL)
				{
					delete CpRun.BklConfig.CustEff.Kbicon[x][y];
					CpRun.BklConfig.CustEff.Kbicon[x][y] = NULL;
				}
			}
		}
	}
	//完毕，建立新的显示矩阵
	CpRun.BklConfig.CustEff.UserX = PhyCfg.LedCfg.LEDMatrixCfg.LED_X;
	CpRun.BklConfig.CustEff.UserY = PhyCfg.LedCfg.LEDMatrixCfg.LED_Y;

	//申请内存

	//创建图像容器
	CpRun.BklConfig.CustEff.SrcImg = new TImage(this);
	CpRun.BklConfig.CustEff.SrcImg->Parent = Panel30; //在Pan上创建
	CpRun.BklConfig.CustEff.SrcImg->AutoSize = false; //禁止自动调整大小
	CpRun.BklConfig.CustEff.SrcImg->Left = 5;
	CpRun.BklConfig.CustEff.SrcImg->Top = 5;
	CpRun.BklConfig.CustEff.SrcImg->Width = CpRun.BklConfig.CustEff.UserX;
	CpRun.BklConfig.CustEff.SrcImg->Height = CpRun.BklConfig.CustEff.UserY;
	CpRun.BklConfig.CustEff.SrcImg->Stretch = false; //启用拉伸
	CpRun.BklConfig.CustEff.SrcImg->Canvas->Brush->Color = TColor(0); //默认黑色背景
	CpRun.BklConfig.CustEff.SrcImg->Canvas->Pen->Color = TColor(0); //默认黑色画笔
	CpRun.BklConfig.CustEff.SrcImg->Canvas->Font->Color = TColor(0); //默认字体颜色为黑色
	CpRun.BklConfig.CustEff.SrcImg->Canvas->Rectangle(0, 0, CpRun.BklConfig.CustEff.SrcImg->Width, CpRun.BklConfig.CustEff.SrcImg->Height); //画出黑框

	//计算显示矩阵的坐标
	Matrix_x = CpRun.BklConfig.CustEff.UserX * 16;
	Matrix_y = CpRun.BklConfig.CustEff.UserY * 16;
	Matrix_x = (Panel30->Width / 2) - (Matrix_x / 2);
	Matrix_y = (Panel30->Height / 2) - (Matrix_y / 2);

	//创建显示矩阵
	for(x = 0; x < CpRun.BklConfig.CustEff.UserX; x++)
	{
		for(y = 0; y < CpRun.BklConfig.CustEff.UserY; y++)
		{
			tag = (uint16_t)x << 8 | (uint16_t)y;

			//设置图标信息
			CpRun.BklConfig.CustEff.IconInfo[x][y].Keyid = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid; //按键ID
			CpRun.BklConfig.CustEff.IconInfo[x][y].Caption = AnsiString().sprintf("%s", PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Caption);

			//创建显示矩阵
			CpRun.BklConfig.CustEff.MatrixIcon[x][y] = new TImage(this);
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Parent = Panel30;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Tag = tag;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Left = (x * 16) + Matrix_x;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Top = (y * 16) + Matrix_y;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Width = 16;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Height = 16;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->Canvas->Pen->Color = TColor(0x00977326); //默认的边框颜色
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->ShowHint = true;
			CpRun.BklConfig.CustEff.MatrixIcon[x][y]->OnClick = CustomMatrixIconClick;
			CustomDrawMatrixIcon(CpRun.BklConfig.CustEff.MatrixIcon[x][y], TColor(0x00FFB56A), 1);

			//创建键盘矩阵

			if(PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Enable == 0)//如果被禁用，则不再创建
			{
				continue;
			}
			CpRun.BklConfig.CustEff.Kbicon[x][y] = new TImage(this);
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Parent = Panel29;
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Tag = tag;
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Left = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_x;
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Top = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_y;
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Width = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_w;
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Height = PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_h;
			CpRun.BklConfig.CustEff.Kbicon[x][y]->Canvas->Pen->Color = TColor(0x00977326); //默认的边框颜色
			CpRun.BklConfig.CustEff.Kbicon[x][y]->ShowHint = true; //显示提示字符串
			CpRun.BklConfig.CustEff.Kbicon[x][y]->OnClick = CUstomKbIconClick; //单击事件处理程序
			CustomDrawKbIcon(CpRun.BklConfig.CustEff.Kbicon[x][y], TColor(0x00FFB56A), 0);

		}
	}

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CustomDrawMatrixIcon(TImage *img, TColor Color, bool SetToPic)//画一个显示矩阵的图标
{
	int x, y;

	if(img == NULL)
	{
		return;
	}
	CustomGetPointFromTag(img->Tag, &x, &y);
	img->Canvas->Brush->Color = Color;
	img->Canvas->Rectangle(0, 0, img->Width, img->Height);

	//设置对应图标的颜色
	if(SetToPic)
	{
		CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].R = GetRValue(Color);
		CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].G = GetGValue(Color);
		CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].B = GetBValue(Color);
	}
	//设置小参考的颜色
	//CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = Color;

	if(PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Enable == 0)
	{
		img->Hint = LanguageGetStr(322);
	}
	else
	{
		img->Hint = LanguageGetStr(321);
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CustomGetPointFromTag(int tag, int *x, int *y)//根据tag，得到坐标
{
	*x = tag >> 8;
	*y = tag & 0x000000ff;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CustomDrawKbIcon(TImage *img, TColor Color, bool SetToPic)//画一个键盘矩阵的图标
{

	int fx, fy, fw, fh;
	int r, g, b;
	int x, y;
	if(img == NULL)
	{
		return;
	}
	img->Canvas->Brush->Color = Color;
	img->Canvas->Rectangle(0, 0, img->Width, img->Height);
	CustomGetPointFromTag(img->Tag, &x, &y); //取得坐标

	//设置对应图标的颜色
	if(SetToPic)
	{
		CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].R = GetRValue(Color);
		CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].G = GetGValue(Color);
		CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].B = GetBValue(Color);
	}
	//设置小参考的颜色
	CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = Color;

	//显示提示
	img->Hint = String().sprintf(LanguageGetStr(323).c_str(), CpRun.BklConfig.CustEff.IconInfo[x][y].Keyid);
	img->Hint += "\r\n" + String().sprintf(LanguageGetStr(324).c_str(), GetRValue(Color), GetGValue(Color), GetBValue(Color));
	//设置按键的显示文字
	img->Canvas->Font->Size = 8; //字体大小，8
	img->Canvas->Font->Style = TFontStyles() << fsBold; //粗体
	if((GetRValue(Color) > 220) && (GetGValue(Color) > 220) && (GetBValue(Color) > 220))
	{
		r = 0xff - GetRValue(Color);
		g = 0xff - GetGValue(Color);
		b = 0xff - GetBValue(Color);
		img->Canvas->Font->Color = TColor(RGB(r, g, b));
	}
	else
	{
		img->Canvas->Font->Color = TColor(RGB(0xff, 0xff, 0xff));
	}

	fw = Canvas->TextWidth(CpRun.BklConfig.CustEff.IconInfo[x][y].Caption);
	fh = Canvas->TextHeight(CpRun.BklConfig.CustEff.IconInfo[x][y].Caption);

	fx = (img->Width / 2) - (fw / 2);
	fh = (img->Height / 2) - (fh / 2);

	img->Canvas->TextOut(fx, fh, CpRun.BklConfig.CustEff.IconInfo[x][y].Caption);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CustomMatrixIconClick(TObject *Sender)
{
	//显示矩阵被点击
	TImage *img = dynamic_cast <TImage *> (Sender);
	int x, y;
	ColorDialog1->Color = img->Canvas->Brush->Color;
	if(!ColorDialog1->Execute(this->Handle))
	{
		return;
	}
	CustomGetPointFromTag(img->Tag, &x, &y); //取得坐标
	CustomDrawMatrixIcon(img, ColorDialog1->Color, 1); //画出显示矩阵的图标

	CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = ColorDialog1->Color;
	CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].R = GetRValue(ColorDialog1->Color);
	CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].G = GetGValue(ColorDialog1->Color);
	CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].B = GetBValue(ColorDialog1->Color);
	//键盘矩阵对应的图标，也应该显示对应的颜色
	if(PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Enable == 1)
	{
		//只有开启的，才会显示
		CustomDrawKbIcon(CpRun.BklConfig.CustEff.Kbicon[x][y], ColorDialog1->Color, 0);
	}

}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CUstomKbIconClick(TObject *Sender)
{
	//KB矩阵被点击
	TImage *img = dynamic_cast <TImage *> (Sender);
	int x, y;
	ColorDialog1->Color = img->Canvas->Brush->Color;
	if(!ColorDialog1->Execute(this->Handle))
	{
		return;
	}
	CustomGetPointFromTag(img->Tag, &x, &y); //取得坐标
	CustomDrawKbIcon(img, ColorDialog1->Color, 1); //画出显示矩阵的图标

	CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = ColorDialog1->Color;
	CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].R = GetRValue(ColorDialog1->Color);
	CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].G = GetGValue(ColorDialog1->Color);
	CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].B = GetBValue(ColorDialog1->Color);
	//显示参考矩阵的颜色
	CustomDrawMatrixIcon(CpRun.BklConfig.CustEff.MatrixIcon[x][y], ColorDialog1->Color, 0);
}

void __fastcall TMainForm::Button42Click(TObject *Sender)
{
	//随机颜色

	int x, y;
	byte r, g, b;

	Randomize(); //初始化随机数发生器

	for(x = 0; x < PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{
			r = random(256);
			g = random(256);
			b = random(256);
			CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = TColor(RGB(r, g, b));
			CustomDrawKbIcon(CpRun.BklConfig.CustEff.Kbicon[x][y], TColor(RGB(r, g, b)), 0);
			CustomDrawMatrixIcon(CpRun.BklConfig.CustEff.MatrixIcon[x][y], TColor(RGB(r, g, b)), 1);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button43Click(TObject *Sender)
{
	//设置为相同的颜色
	int x, y;

	if(!ColorDialog1->Execute(this->Handle))
	{
		return;
	}

	for(x = 0; x < PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{

			CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = ColorDialog1->Color;
			CustomDrawKbIcon(CpRun.BklConfig.CustEff.Kbicon[x][y], ColorDialog1->Color, 0);
			CustomDrawMatrixIcon(CpRun.BklConfig.CustEff.MatrixIcon[x][y], ColorDialog1->Color, 1);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button41Click(TObject *Sender)
{
	//载入一个BMP图片
	int x, y;
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "BMP Picture Files(*.bmp)\0*.bmp\0\0";
	//要选择的文件后缀
	ofn.lpstrFile = szFileName;
	ofn.lpstrTitle = "Select a Picture File...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(PhyCfg.LedCfg.LEDMatrixCfg.LED_X == 0 || PhyCfg.LedCfg.LEDMatrixCfg.LED_Y == 0)
	{
		return;
	}

	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}
	CpRun.BklConfig.CustEff.SrcImg->Picture->LoadFromFile(szFileName);

	x = CpRun.BklConfig.CustEff.SrcImg->Width;
	y = CpRun.BklConfig.CustEff.SrcImg->Height;

	for(x = 0; x < PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{
			CustomDrawKbIcon(CpRun.BklConfig.CustEff.Kbicon[x][y], CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y], 0);
			CustomDrawMatrixIcon(CpRun.BklConfig.CustEff.MatrixIcon[x][y], CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y], 1);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CustomDispKbData(void)//显示键盘发来的图片数据
{
	int x, y;
	int r, g, b;
	for(x = 0; x < PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{
			r = CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].R;
			g = CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].G;
			b = CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex].rgb[x][y].B;

			CpRun.BklConfig.CustEff.SrcImg->Canvas->Pixels[x][y] = TColor(RGB(r, g, b));
			CustomDrawKbIcon(CpRun.BklConfig.CustEff.Kbicon[x][y], TColor(RGB(r, g, b)), 0);
			CustomDrawMatrixIcon(CpRun.BklConfig.CustEff.MatrixIcon[x][y], TColor(RGB(r, g, b)), 0);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBox10Change(TObject *Sender)
{
	//显示当前层的数据
	CustomDispKbData();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button40Click(TObject *Sender)
{
	//下载一张图片
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button40->Enabled = false;
	SendCmd(USB_DEF_CMD_CUSTOM_LED_NEED_PIC, ComboBox10->ItemIndex, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CustomGetPicFromDevice(void *pData)//从设备获取一张图片
{
	union CP_UNION_ONE_PACK_64BYTE *PackData = (union CP_UNION_ONE_PACK_64BYTE *)pData; //临时数据包
	static uint16_t TotalSize = 0;
	static uint8_t RecvDataBuff[1024]; //2KB的数据接收缓冲区

	//复制收到的数据到接收缓冲区
	memcpy(RecvDataBuff + TotalSize, PackData->LongDataPack.PackData, PackData->LongDataPack.CurrPackSize);

	//累计收到的字节数
	TotalSize += PackData->LongDataPack.CurrPackSize;

	//判断数据是否接收完毕
	if(TotalSize >= sizeof(CpRun.BklConfig.CustEff.Pics[0]))
	{
		//如果接收完毕
		TotalSize = 0; //接收总数归零
		//复制完整的数据到层数据中
		memcpy(&CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex], RecvDataBuff, sizeof(CpRun.BklConfig.CustEff.Pics[0]));
		Button40->Enabled = true;
		//显示内容
		CustomDispKbData();
	}
}

void __fastcall TMainForm::Button39Click(TObject *Sender)
{
	//上传图片
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button39->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_CUSTOM_LED_GET_PIC, ComboBox10->ItemIndex, &CpRun.BklConfig.CustEff.Pics[ComboBox10->ItemIndex],
		sizeof(CpRun.BklConfig.CustEff.Pics[0]));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button39->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::RadioButton24Click(TObject *Sender)
{
	//灯效选择
	CpRun.BklConfig.LedCfg.CustomLedCfg[ComboBox8->ItemIndex].EffectIdx = dynamic_cast <TRadioButton *> (Sender)->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TrackBar7Change(TObject *Sender)
{
	CpRun.BklConfig.LedCfg.CustomLedCfg[ComboBox8->ItemIndex].Speed = TrackBar7->Position;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ComboBox8Change(TObject *Sender)
{
	//组被切换
	CustomDispCfg();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::CustomDispCfg(void)//显示配置信息
{
	ComboBox9->ItemIndex = CpRun.BklConfig.LedCfg.CustomLedCfg[ComboBox8->ItemIndex].UsePicIdx;

	switch(CpRun.BklConfig.LedCfg.CustomLedCfg[ComboBox8->ItemIndex].EffectIdx)
	{
		case 0:
		{
			RadioButton24->Checked = true;
		} break;
		case 1:
		{
			RadioButton25->Checked = true;
		} break;
		case 2:
		{
			RadioButton26->Checked = true;
		} break;
		case 3:
		{
			RadioButton27->Checked = true;
		} break;
		case 4:
		{
			RadioButton28->Checked = true;
		} break;
	}
	TrackBar7->Position = CpRun.BklConfig.LedCfg.CustomLedCfg[ComboBox8->ItemIndex].Speed;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button37Click(TObject *Sender)
{
	//上传 灯效配置
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button37->Enabled = false;
	SendDataByPack(0, USB_DEF_CMD_CUSTOM_LED_SET_CFG, 0, &CpRun.BklConfig.LedCfg.CustomLedCfg, sizeof(CpRun.BklConfig.LedCfg.CustomLedCfg));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button37->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ComboBox9Change(TObject *Sender)
{
	CpRun.BklConfig.LedCfg.CustomLedCfg[ComboBox8->ItemIndex].UsePicIdx = ComboBox9->ItemIndex;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button44Click(TObject *Sender)
{
	//启动键盘固件
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_BOOTLOADER)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button44->Enabled = false;
	SendCmd(_DEF_USB_RECV_RUN_APP, 0, 0); //要求启动app

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button47Click(TObject *Sender)
{
	//恢复默认值
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_BOOTLOADER)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	if(MessageBox(this->Handle, AnsiString(LanguageGetStr(330)).c_str(), AnsiString(LanguageGetStr(331)).c_str(), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}
	Label26->Caption = LanguageGetStr(332);
	Button47->Enabled = false;
	Button44->Enabled = false;
	Button45->Enabled = false;
	if(Button46->Enabled == true)
	{
		Button46->Enabled = false;
	}

	SendCmd(_DEF_USB_RECV_CLEAR_FLASH, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button48Click(TObject *Sender)
{
	//刷入硬件配置
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	if(MessageBox(this->Handle, AnsiString(LanguageGetStr(333)).c_str(), AnsiString(LanguageGetStr(331)).c_str(), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}

	//加载所有配置
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "PhyConfigFiles(*.PhyCfg)\0*.PhyCfg\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Select a HeardWare Config File";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.lpstrDefExt = "PhyCfg";
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}
	HANDLE hHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(hHandle == INVALID_HANDLE_VALUE)
	{
		Label26->Caption = LanguageGetStr(334);
		ShowMessage(LanguageGetStr(334));
		return;
	}
	uint16_t tmp;
	DWORD ReadSize;
	ReadFile(hHandle, &tmp, sizeof(tmp), &ReadSize, 0);
	if(tmp != CP_DEF_PHY_ALL_CFG_FILE_HEAD)
	{
		Label26->Caption = LanguageGetStr(335);
		ShowMessage(LanguageGetStr(335));
		CloseHandle(hHandle);
		return;
	}
	ReadFile(hHandle, &CpRun.PhyCfgUpLoad.PhyCfgData, sizeof(CpRun.PhyCfgUpLoad.PhyCfgData), &ReadSize, 0);
	CloseHandle(hHandle);
	if(ReadSize != sizeof(CpRun.PhyCfgUpLoad.PhyCfgData))
	{
		Label26->Caption = LanguageGetStr(336);
		ShowMessage(LanguageGetStr(336));
		return;
	}

	Button38->Enabled = false;
	Button48->Enabled = false;
	//上传
	SendDataByPack(0, USB_DEF_CMD_PHY_CFG_SET_ALL_CFG, 0, &CpRun.PhyCfgUpLoad.PhyCfgData, sizeof(CpRun.PhyCfgUpLoad.PhyCfgData));
	MyHid->StartSendThread();
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	Button38->Enabled = true;
	Button48->Enabled = true;
	Label26->Caption = LanguageGetStr(337);
	//要求获取按键扫描配置
	SendCmd(USB_DEF_CMD_PHY_CFG_NEED_KEY_SCAN_CFG, 0x00, 0x00);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button38Click(TObject *Sender)
{
	//切换到BL
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	Button38->Enabled = false;
	Button48->Enabled = false;
	SendCmd(USB_DEF_CMD_RESET_TO_BOOTLOADER, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button45Click(TObject *Sender)
{
	//选择固件文件
	ULONG ReadSize;
	TCHAR szFileName[MAX_PATH] =
	{
		0
	};
	OPENFILENAME ofn =
	{
		0
	};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Application->Handle;
	ofn.lpstrFilter = "Firmware File(*.ldn)\0*.ldn\0All file(*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName; //存放文件的缓冲区
	ofn.lpstrTitle = "Select a Ldn Firmware File...";
	ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//标志如果是多选要加上OFN_ALLOWMULTISELECT
	if(GetOpenFileName(&ofn) == 0)
	{
		return;
	}

	if(CpRun.Bl.FwFile.hFwFile != NULL)
	{
		CloseHandle(CpRun.Bl.FwFile.hFwFile);
		CpRun.Bl.FwFile.hFwFile = NULL;
	}

	//打开文件
	CpRun.Bl.FwFile.hFwFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if(CpRun.Bl.FwFile.hFwFile == INVALID_HANDLE_VALUE)
	{
		Label26->Caption = LanguageGetStr(340);
		ShowMessage(LanguageGetStr(340));
		CpRun.Bl.FwFile.hFwFile = NULL;
		return;
	}

	//取得文件大小
	CpRun.Bl.FwFile.FwFileSize = GetFileSize(CpRun.Bl.FwFile.hFwFile, NULL);
	//读取文件头
	ReadFile(CpRun.Bl.FwFile.hFwFile, &CpRun.Bl.FwFile.FwHeadInfo, sizeof(CpRun.Bl.FwFile.FwHeadInfo), &ReadSize, NULL);
	//判断文件是否合法
	if((CpRun.Bl.FwFile.FwHeadInfo.Head1 != _DEF_PACK_FILE_HEAD_1) || CpRun.Bl.FwFile.FwHeadInfo.Head2 != _DEF_PACK_FILE_HEAD_2)
	{
		CloseHandle(CpRun.Bl.FwFile.hFwFile);
		CpRun.Bl.FwFile.hFwFile = NULL;
		Button46->Enabled = false;
		Label26->Caption = LanguageGetStr(341);
		ShowMessage(LanguageGetStr(341));
		return;
	}
	if(CpRun.Bl.FwFile.FwHeadInfo.BL_Ver != CpRun.Bl.BlInfo.Bl_Ver)
	{
		CloseHandle(CpRun.Bl.FwFile.hFwFile);
		CpRun.Bl.FwFile.hFwFile = NULL;
		Button46->Enabled = false;
		Label26->Caption = LanguageGetStr(342);
		ShowMessage(LanguageGetStr(342));
		return;
	}
	if((CpRun.Bl.FwFile.FwHeadInfo.KB_Fw_Size +sizeof(CpRun.Bl.FwFile.FwHeadInfo)) != CpRun.Bl.FwFile.FwFileSize)
	{
		CloseHandle(CpRun.Bl.FwFile.hFwFile);
		CpRun.Bl.FwFile.hFwFile = NULL;
		Button46->Enabled = false;
		Label26->Caption = LanguageGetStr(343);
		ShowMessage(LanguageGetStr(343));
		return;
	}
	Button46->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button46Click(TObject *Sender)
{
	//开始刷写 固件
	ULONG ReadSize;
	ULONG SendSize;
	ULONG PackCount = 0;

	//禁用这些按钮
	Button38->Enabled = false;
	Button44->Enabled = false;
	Button45->Enabled = false;
	Button46->Enabled = false;
	Button47->Enabled = false;
	Button48->Enabled = false;

	//发送文件头给设备
	SendDataByPack(0, _DEF_USB_RECV_FW_HEAD, 0, &CpRun.Bl.FwFile.FwHeadInfo, sizeof(CpRun.Bl.FwFile.FwHeadInfo));

	//读取固件内容

	ReadFile(CpRun.Bl.FwFile.hFwFile, CpRun.Bl.FwFile.FwData, CpRun.Bl.FwFile.FwHeadInfo.KB_Fw_Size, &ReadSize, NULL); //读出数据
	CloseHandle(CpRun.Bl.FwFile.hFwFile); //关闭句柄
	CpRun.Bl.FwFile.hFwFile = NULL;

	//把数据分包放入缓冲区
	PackCount = SendDataByPack(0, _DEF_USB_RECV_FW_DATA, 0, CpRun.Bl.FwFile.FwData, ReadSize);
	ProgressBar1->Min = 0;
	ProgressBar1->Max = PackCount;
	ProgressBar1->Position = 0;

	CpRun.Bl.IsSendFwData = 1; //设置标志位，表示正在发送固件数据
	//准备完毕，启动发送线程
	MyHid->StartSendThread();
	Label26->Caption = LanguageGetStr(206);

	//等待发送完毕
	while(MyHid->GetSendStatus())
	{
		Application->ProcessMessages();
	}
	CpRun.Bl.IsSendFwData = 0; //发送完毕，归0
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ThemesInit(void)//初始化皮肤相关的数据
{
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "Skin.ini");
	int SkinID = ini->ReadInteger("SkinCfg", "SkinID", 0);
	delete ini;

	for(int i = 0; i < TStyleManager::StyleNames.Length; i++)
	{
		ComboBox11->Items->Add(TStyleManager::StyleNames[i]);
	}

	//ComboBox11->ItemIndex = ComboBox11->Items->IndexOf(TStyleManager::ActiveStyle->Name);
	ComboBox11->ItemIndex = SkinID;
	TStyleManager::SetStyle(ComboBox11->Text);
}

void __fastcall TMainForm::ComboBox11Change(TObject *Sender)
{
	//选择一个皮肤
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "Skin.ini");
	ini->WriteInteger("SkinCfg", "SkinID", ComboBox11->ItemIndex);
	delete ini;
	ShowMessage(LanguageGetStr(344));
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadLanguageFiles(void)//加载配置的语言文件
{
	//获取目录下所有的lng文件
	int i;
	String LngFileName;

	TStringDynArray sl = TDirectory::GetFiles(ExtractFilePath(Application->ExeName) + "Language\\", "*.ini");
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "Skin.ini");
	LngFileName = ini->ReadString("Language", "Name", "");
	delete ini;

	//取得文件名
	for(i = 0; i < sl.Length; i++)
	{
		ComboBox12->Items->Add(ChangeFileExt(ExtractFileName(sl[i]), ""));
	}
	ComboBox12->ItemIndex = ComboBox12->Items->IndexOf(ChangeFileExt(LngFileName, ""));
	//应用当前的语言配置
	LanguageLoadLngRes(ComboBox12->Text);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBox12Change(TObject *Sender)
{
	//选择一个语言
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "Skin.ini");
	ini->WriteString("Language", "Name", ComboBox12->Text + ".ini");
	delete ini;
	LanguageLoadLngRes(ComboBox12->Text);
	ShowMessage(LanguageGetStr(345));
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LanguageLoadLngRes(String FileName)//加载指定的语言文件到内存
{
	if(FileExists(ExtractFilePath(Application->ExeName) + "Language\\" + FileName + ".ini") == false)
	{
		return;
	}

	if(CpRun.Language.Idx != NULL)
	{
		delete CpRun.Language.Idx;
	}
	if(CpRun.Language.Str != NULL)
	{
		delete CpRun.Language.Str;
	}

	CpRun.Language.Idx = new TStringList(); //申请内存
	CpRun.Language.Str = new TStringList();
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) + "Language\\" + FileName + ".ini");
	ini->ReadSection("Language", CpRun.Language.Idx);
	for(int i = 0; i < CpRun.Language.Idx->Count; i++)
	{
		CpRun.Language.Str->Add(ini->ReadString("Language", CpRun.Language.Idx->Strings[i], ""));
	}
	delete ini;
	LanguageSetLng(); //应用语言资源数据
}

//---------------------------------------------------------------------------
String __fastcall TMainForm::LanguageGetStr(int id)//获取资源对应的字符串
{
	int i;
	String Str;
	Str.sprintf(L"%04u", id);
	i = CpRun.Language.Idx->IndexOf(Str);
	if(i < 0)
	{
		return ("Lng Error!"); //如果超出范围，则返回语言错误消息
	}
	else
	{
		return (CpRun.Language.Str->Strings[i]); //否则返回正确的字符串
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LanguageSetLng(void)//设置当前的语言到UI上
{
	//主标签页
	TabSheet1->Caption = LanguageGetStr(0);
	TabSheet2->Caption = LanguageGetStr(1);
	TabSheet9->Caption = LanguageGetStr(2);
	TabSheet8->Caption = LanguageGetStr(3);
	TabSheet10->Caption = LanguageGetStr(4);
	TabSheet11->Caption = LanguageGetStr(5);
	TabSheet12->Caption = LanguageGetStr(6);
	TabSheet13->Caption = LanguageGetStr(7);

	//功能层页面
	Button1->Caption = LanguageGetStr(10);
	Button2->Caption = LanguageGetStr(11);
	Button3->Caption = LanguageGetStr(12);
	Button4->Caption = LanguageGetStr(13);
	Button5->Caption = LanguageGetStr(14);
	Button11->Caption = LanguageGetStr(15);

	//触发层页面
	TabSheet3->Caption = LanguageGetStr(20);
	TabSheet4->Caption = LanguageGetStr(21);
	TabSheet5->Caption = LanguageGetStr(22);
	TabSheet6->Caption = LanguageGetStr(23);
	TabSheet7->Caption = LanguageGetStr(24);

	Label3->Caption = LanguageGetStr(25);
	Label1->Caption = LanguageGetStr(26);
	Label2->Caption = LanguageGetStr(27);

	Button6->Caption = LanguageGetStr(28);
	Button7->Caption = LanguageGetStr(29);
	Button8->Caption = LanguageGetStr(30);
	Button9->Caption = LanguageGetStr(31);
	Button10->Caption = LanguageGetStr(32);

	//宏
	GroupBox1->Caption = LanguageGetStr(50);
	Label4->Caption = LanguageGetStr(51);
	Button13->Caption = LanguageGetStr(52);
	Button14->Caption = LanguageGetStr(53);
	Button15->Caption = LanguageGetStr(54);
	Button16->Caption = LanguageGetStr(55);
	MacroEditPopMenu->Items->Items[0]->Caption = LanguageGetStr(56);
	MacroEditPopMenu->Items->Items[1]->Caption = LanguageGetStr(57);

	//锁定键
	GroupBox2->Caption = LanguageGetStr(60);
	Button17->Caption = LanguageGetStr(61);
	Button18->Caption = LanguageGetStr(62);
	Button19->Caption = LanguageGetStr(63);
	Button20->Caption = LanguageGetStr(64);
	KeyLockPopMenu->Items->Items[0]->Caption = LanguageGetStr(65);
	KeyLockPopMenu->Items->Items[1]->Caption = LanguageGetStr(66);
	KeyLockPopMenu->Items->Items[3]->Caption = LanguageGetStr(67);

	//轴灯灯效配置
	GroupBox3->Caption = LanguageGetStr(70);
	Button21->Caption = LanguageGetStr(71);
	GroupBox4->Caption = LanguageGetStr(72);
	Label8->Caption = LanguageGetStr(73);
	Label9->Caption = LanguageGetStr(74);
	Label10->Caption = LanguageGetStr(75);
	Button22->Caption = LanguageGetStr(76);
	Button23->Caption = LanguageGetStr(76);
	Button24->Caption = LanguageGetStr(76);
	Button25->Caption = LanguageGetStr(77);
	Button26->Caption = LanguageGetStr(78);

	GroupBox5->Caption = LanguageGetStr(80);
	GroupBox6->Caption = LanguageGetStr(81);
	GroupBox7->Caption = LanguageGetStr(82);

	CheckBox1->Caption = LanguageGetStr(83);
	CheckBox2->Caption = LanguageGetStr(83);
	CheckBox3->Caption = LanguageGetStr(83);

	RadioButton1->Caption = LanguageGetStr(84);
	RadioButton11->Caption = LanguageGetStr(84);
	RadioButton17->Caption = LanguageGetStr(84);

	RadioButton2->Caption = LanguageGetStr(85);
	RadioButton12->Caption = LanguageGetStr(85);
	RadioButton18->Caption = LanguageGetStr(85);

	Button27->Caption = LanguageGetStr(86);
	Button28->Caption = LanguageGetStr(86);
	Button31->Caption = LanguageGetStr(86);

	Button29->Caption = LanguageGetStr(87);
	Button30->Caption = LanguageGetStr(87);
	Button32->Caption = LanguageGetStr(87);

	CheckBox4->Caption = LanguageGetStr(89);
	CheckBox5->Caption = LanguageGetStr(89);
	CheckBox6->Caption = LanguageGetStr(89);

	RadioButton3->Caption = LanguageGetStr(90);
	RadioButton7->Caption = LanguageGetStr(90);
	RadioButton13->Caption = LanguageGetStr(90);

	RadioButton4->Caption = LanguageGetStr(91);
	RadioButton8->Caption = LanguageGetStr(91);
	RadioButton14->Caption = LanguageGetStr(91);

	RadioButton5->Caption = LanguageGetStr(92);
	RadioButton9->Caption = LanguageGetStr(92);
	RadioButton15->Caption = LanguageGetStr(92);

	RadioButton6->Caption = LanguageGetStr(93);
	RadioButton10->Caption = LanguageGetStr(93);
	RadioButton16->Caption = LanguageGetStr(93);

	Label11->Caption = LanguageGetStr(88);
	Label12->Caption = LanguageGetStr(88);
	Label13->Caption = LanguageGetStr(88);

	//WS2812和指示灯动作配置
	GroupBox9->Caption = LanguageGetStr(100);
	Label14->Caption = LanguageGetStr(101);
	GroupBox8->Caption = LanguageGetStr(102);
	CheckBox7->Caption = LanguageGetStr(103);
	Label15->Caption = LanguageGetStr(104);
	Label16->Caption = LanguageGetStr(105);
	Button33->Caption = LanguageGetStr(106);
	RadioButton23->Caption = LanguageGetStr(107);
	RadioButton19->Caption = LanguageGetStr(108);
	RadioButton20->Caption = LanguageGetStr(109);
	RadioButton21->Caption = LanguageGetStr(110);
	RadioButton22->Caption = LanguageGetStr(111);
	WS_Speed->Hint = LanguageGetStr(112);

	//指示灯动作
	GroupBox10->Caption = LanguageGetStr(120);
	GroupBox11->Caption = LanguageGetStr(121);
	GroupBox12->Caption = LanguageGetStr(122);

	CheckBox8->Caption = LanguageGetStr(123);
	CheckBox9->Caption = LanguageGetStr(123);
	CheckBox10->Caption = LanguageGetStr(123);

	Button34->Caption = LanguageGetStr(124);
	Button35->Caption = LanguageGetStr(124);
	Button36->Caption = LanguageGetStr(124);

	Label17->Caption = LanguageGetStr(125);
	Label18->Caption = LanguageGetStr(126);

	Label19->Caption = LanguageGetStr(127);
	Label20->Caption = LanguageGetStr(128);

	Label21->Caption = LanguageGetStr(129);
	Label22->Caption = LanguageGetStr(130);

	//自定义灯效
	GroupBox13->Caption = LanguageGetStr(140);
	Label23->Caption = LanguageGetStr(141);
	Button37->Caption = LanguageGetStr(142);
	Label24->Caption = LanguageGetStr(143);
	RadioButton24->Caption = LanguageGetStr(144);
	RadioButton25->Caption = LanguageGetStr(145);
	RadioButton26->Caption = LanguageGetStr(146);
	RadioButton27->Caption = LanguageGetStr(147);
	RadioButton28->Caption = LanguageGetStr(148);

	GroupBox14->Caption = LanguageGetStr(155);
	Label25->Caption = LanguageGetStr(149);
	Button40->Caption = LanguageGetStr(150);
	Button39->Caption = LanguageGetStr(151);

	Button41->Caption = LanguageGetStr(152);
	Button42->Caption = LanguageGetStr(153);
	Button43->Caption = LanguageGetStr(154);

	//系统设置页面
	GroupBox15->Caption = LanguageGetStr(160);
	Button38->Caption = LanguageGetStr(161);
	Button48->Caption = LanguageGetStr(162);
	Button47->Caption = LanguageGetStr(163);
	Button44->Caption = LanguageGetStr(164);
	Button45->Caption = LanguageGetStr(165);
	Button46->Caption = LanguageGetStr(166);

	GroupBox16->Caption = LanguageGetStr(169);
	Label27->Caption = LanguageGetStr(167);
	Label28->Caption = LanguageGetStr(168);

	//关机后关闭背光
	GroupBox17->Caption = LanguageGetStr(640);
	CheckBox11->Caption = LanguageGetStr(641);
	CheckBox12->Caption = LanguageGetStr(642);
	CheckBox13->Caption = LanguageGetStr(643);
	CheckBox14->Caption = LanguageGetStr(644);
	CheckBox15->Caption = LanguageGetStr(645);
	CheckBox16->Caption = LanguageGetStr(646);
	Button49->Caption = LanguageGetStr(647);

};

void __fastcall TMainForm::CheckBox11Click(TObject *Sender)
{
	//启用或者禁用
	if(CheckBox11->Checked)
	{
		CheckBox12->Enabled = true;
		CheckBox13->Enabled = true;
		CheckBox14->Enabled = true;
		CheckBox15->Enabled = true;
		CheckBox16->Enabled = true;
		CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.Enable = 1;
	}
	else
	{
		CheckBox12->Enabled = false;
		CheckBox13->Enabled = false;
		CheckBox14->Enabled = false;
		CheckBox15->Enabled = false;
		CheckBox16->Enabled = false;
		CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.Enable = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBox12Click(TObject *Sender)
{
	//
	TCheckBox *cbx = dynamic_cast <TCheckBox *> (Sender);

	switch(cbx->Tag)
	{
		case 0:
		{
			CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.BklOff = cbx->Checked;
		} break;
		case 1:
		{
			CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG0 = cbx->Checked;
		} break;
		case 2:
		{
			CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG1 = cbx->Checked;
		} break;
		case 3:
		{
			CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG2 = cbx->Checked;
		} break;
		case 4:
		{
			CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG3 = cbx->Checked;
		} break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button49Click(TObject *Sender)
{
	//上传配置给键盘
	if(MyHid->GetConnectType() != _DEF_USB_CONNECT_TYPE_KEYBOARD_FW)
	{
		ShowMessage(LanguageGetStr(211));
		return;
	}
	SendCmd(USB_DEF_CMD_LED_CFG_SET_USB_DISCONN_FUNC, CpRun.BklConfig.LedCfg.UsbDisConnOffEff.FunctionSw, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::UsbDisConn_ShowCfg(void)
{
	//显示配置的结果
	CheckBox11->Checked = CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.Enable;
	CheckBox12->Checked = CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.BklOff;
	CheckBox13->Checked = CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG0;
	CheckBox14->Checked = CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG1;
	CheckBox15->Checked = CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG2;
	CheckBox16->Checked = CpRun.BklConfig.LedCfg.UsbDisConnOffEff.Bit.wsG3;
	CheckBox11Click(NULL);
}
