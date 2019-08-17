//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "RecvAndSendData.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <ButtonGroup.hpp>
#include <XPMan.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
#include <GIFImg.hpp>
#include <jpeg.hpp>
#include <CustomizeDlg.hpp>
#include "Vcl.WinXPanels.hpp"


//程序适配的固件版本号
#define	SYS_DEF_HW_VER_M3	   0x01//硬件版本 stm32F103ret6
#define SYS_DEF_HW_VER_M4      0x02//硬件版本 stm32F405/407
#define	SYS_DEF_FW_VER		   0x02//固件版本
#define	SYS_DEF_BL_VER		   0x01//BL版本

//背光灯效配置
#define	EFF_DEF_BKL_MAX_EFFECT													15			//一共有多少个固定灯效
#define EFF_DEF_BKL_CUSTOM_MAX_EFFECT											8			//一共有多少个自定义灯效
#define EFF_DEF_ERR_MAX_SPEED													64			//速度值，最大
#define EFF_DEF_EFF_STEP_MAX													24			//灯效的衰减幅度
#define	EFF_DEF_WS_GROUP_MAX													4			//最多可以设置4组灯带的灯效
#define EFF_DEF_WS_SET_COLOR_MAX												4			//每组最多可设置4个固定颜色
#define EFF_DEF_WS_EFF_MAX                                                      5           //几种灯效


//STM32 GPIO地址定义
#define	CP_DEF_STM32_F103_GPIOA		0x40010800
#define	CP_DEF_STM32_F103_GPIOB		0x40010C00
#define	CP_DEF_STM32_F103_GPIOC		0x40011000
#define	CP_DEF_STM32_F103_GPIOD		0x40011400
#define	CP_DEF_STM32_F103_GPIOE		0x40011800
#define	CP_DEF_STM32_F103_GPIOF		0x40011C00
#define	CP_DEF_STM32_F103_GPIOG		0x40012000

//PIN
#define	CP_DEF_STM32_F103_PIN0		((uint16_t)0x0001)
#define	CP_DEF_STM32_F103_PIN1		((uint16_t)0x0002)
#define	CP_DEF_STM32_F103_PIN2		((uint16_t)0x0004)
#define	CP_DEF_STM32_F103_PIN3		((uint16_t)0x0008)
#define	CP_DEF_STM32_F103_PIN4		((uint16_t)0x0010)
#define	CP_DEF_STM32_F103_PIN5		((uint16_t)0x0020)
#define	CP_DEF_STM32_F103_PIN6		((uint16_t)0x0040)
#define	CP_DEF_STM32_F103_PIN7		((uint16_t)0x0080)
#define	CP_DEF_STM32_F103_PIN8		((uint16_t)0x0100)
#define	CP_DEF_STM32_F103_PIN9		((uint16_t)0x0200)
#define	CP_DEF_STM32_F103_PIN10		((uint16_t)0x0400)
#define	CP_DEF_STM32_F103_PIN11		((uint16_t)0x0800)
#define	CP_DEF_STM32_F103_PIN12		((uint16_t)0x1000)
#define	CP_DEF_STM32_F103_PIN13		((uint16_t)0x2000)
#define	CP_DEF_STM32_F103_PIN14		((uint16_t)0x4000)
#define	CP_DEF_STM32_F103_PIN15		((uint16_t)0x8000)
#define	CP_DEF_STM32_F103_PIN_ALL	((uint16_t)0xFFFF)

//stm32 GPIO结构
typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;
} GPIO_TypeDef;

//GPIO结构
const struct CP_STRUCT_DEF_GPIO
{
	uint32_t GPIO[8];
	String Str[8];

} STM32_GPIO =
{
	{
		CP_DEF_STM32_F103_GPIOA,
		CP_DEF_STM32_F103_GPIOB,
		CP_DEF_STM32_F103_GPIOC,
		CP_DEF_STM32_F103_GPIOD,
		CP_DEF_STM32_F103_GPIOE,
		CP_DEF_STM32_F103_GPIOF,
		CP_DEF_STM32_F103_GPIOG,
		0x00000000,
	},
	{
		"GPIOA",
		"GPIOB",
		"GPIOC",
		"GPIOD",
		"GPIOE",
		"GPIOF",
		"GPIOG",
		"未配置",
	}
};

const struct CP_STRUCT_DEF_STM32_PIN
{
	uint16_t PIN[17];
	String Str[17];

} STM32_PIN =
{
	{
		CP_DEF_STM32_F103_PIN0,
		CP_DEF_STM32_F103_PIN1,
		CP_DEF_STM32_F103_PIN2,
		CP_DEF_STM32_F103_PIN3,
		CP_DEF_STM32_F103_PIN4,
		CP_DEF_STM32_F103_PIN5,
		CP_DEF_STM32_F103_PIN6,
		CP_DEF_STM32_F103_PIN7,
		CP_DEF_STM32_F103_PIN8,
		CP_DEF_STM32_F103_PIN9,
		CP_DEF_STM32_F103_PIN10,
		CP_DEF_STM32_F103_PIN11,
		CP_DEF_STM32_F103_PIN12,
		CP_DEF_STM32_F103_PIN13,
		CP_DEF_STM32_F103_PIN14,
		CP_DEF_STM32_F103_PIN15,
		0x0000
	},
	{
		"PIN_0",
		"PIN_1",
		"PIN_2",
		"PIN_3",
		"PIN_4",
		"PIN_5",
		"PIN_6",
		"PIN_7",
		"PIN_8",
		"PIN_9",
		"PIN_10",
		"PIN_11",
		"PIN_12",
		"PIN_13",
		"PIN_14",
		"PIN_15",
		"未配置",
	}
};

//------------------------------------------------------------------------------
#define	CP_DEF_MAX_RAW_PACK_SIZE												64		//一个原始的数据包的大小
#define	CP_DEF_MAX_CARRY_DATA_SIZE												56		//数据有效携带量
#define	CP_DEF_MAX_SEND_DATA_BUFF_BLOCK_COUNT									64		//发送数据的FIFO缓冲区最大可缓冲多少个数据包
#define	CP_DEF_MAX_TEMP_DATA_BUFF												4096	//4K的临时缓冲区

#define	CP_DEF_KEYSCAN_CFG_FILE_HEAD											0x0483	//键盘扫描配置文件的文件头
#define	CP_DEF_LED_CFG_FILE_HEAD												0x8601	//LED的配置文件的文件头
#define	CP_DEF_PHY_ALL_CFG_FILE_HEAD											0x8604	//所有硬件配置文件的文件头
#define CP_DEF_FLAYER_CFG_FILE_HEAD												0x8610	//功能层的配置文件文件头
#define	CP_DEF_TLAYER_CFG_FILE_HEAD												0x8611	//触发层的配置文件文件头
#define CP_DEF_MACRO_CFG_FILE_HEAD                                              0x8612  //宏的数据配置
#define CP_DEF_KEYLOCK_CFG_FILE_HEAD                                            0x8613  //锁定键的配置

#define		LAYER_DEF_FLAYER_MAX_NUM											8				//功能层最多8个层
#define		LAYER_DEF_TLAYER_MAX_NUM											8				//触发层最多8个层

#define		LAYER_DEF_TLAYER_FN_KEY_MAX_NUM										8				//一个触发层，最多可设置8个FN键
#define		LAYER_DEF_TLAYER_COMBO_GROUP_MAX_NUM								48				//一个触发层，最多可设置48组组合键
#define		LAYER_DEF_TLAYER_nHIT_KEY_MAX_NUM									64				//一个触发层，最多可设置64个连击键
#define		LAYER_DEF_TLAYER_2IN1_KEY_MAX_NUM									64				//一个触发层，最多可设置64个二合一键
#define		LAYER_DEF_TLAYER_HOLD_DOWN_MAX_NUM									64				//一个触发层，最多可设置64个按下保持键
#define		LAYER_DEF_KEY_LOCK_GROUP_MAX_KEYS									8				//锁定键，一组最多锁定8个按键
#define		LAYER_DEF_KEY_LOCK_MAX_GROUP										64				//锁定键，最多有64组
#define		LAYER_DEF_MACRO_ONE_GROUP_MAX_STEP									512				//每个宏组，可以设置最多512步的数据
#define		LAYER_DEF_MACRO_MAX_GROUP											100				//最多可以定义100组宏

//USB通信命令码
#define	USB_DEF_CMD_NONE														0			//空命令
#define	USB_DEF_CMD_GET_HW_FW_INFO												1			//获取固件和硬件的版本
#define	USB_DEF_CMD_PHY_CFG_KEY_SCAN_GET										2			//主机发来按键扫描的硬件配置，设备发给主机，也用相同的命令
#define	USB_DEF_CMD_PHY_CFG_LED_GET												3			//主机发来LED背光的硬件配置，设备发给主机，命令相同
#define	USB_DEF_CMD_PHY_CFG_NEED_KEY_SCAN_CFG									4			//主机要求获取按键扫描配置
#define	USB_DEF_CMD_PHY_CFG_NEED_LED_CFG										5			//主机要求获取LED的配置
#define USB_DEF_CMD_PHY_CFG_SET_ALL_CFG											6			//主机发来所有硬件配置的数据

#define	USB_DEF_CMD_FL_NEED_DATA												10			//主机要求获取功能层的数据
#define	USB_DEF_CMD_FL_GET_DATA													11			//主机发来功能层的数据

#define USB_DEF_CMD_TL_NEED_DATA												12		//主机要求获取触发层的数据
#define	USB_DEF_CMD_TL_GET_DATA													13		//主机发来触发层的数据

#define USB_DEF_CMD_LAYER_NEED_DATA												20		//主机要求获取层配置
#define USB_DEF_CMD_LAYER_GET_DATA												21		//主机发来层配置

#define USB_DEF_CMD_MACRO_NEED_DATA												25		//主机要求获取宏的数据
#define USB_DEF_CMD_MACRO_GET_DATA												26		//主机发来宏的数据

#define USB_DEF_CMD_KEY_LOCK_NEED_DATA											27		//主机要求获取锁定键的数据
#define USB_DEF_CMD_KEY_LOCK_GET_DATA											28		//主机发来锁定键的数据

#define USB_DEF_CMD_LED_CFG_NEED_DATA											30		//主机要求获取LED配置
#define USB_DEF_CMD_LED_CFG_GET_DATA											31		//主机发来LED的配置

#define USB_DEF_CMD_LED_CFG_NEXT_BKL_EFF										32		//下一个灯效
#define USB_DEF_CMD_LED_CFG_LAST_BKL_EFF										33		//上一个灯效
#define USB_DEF_CMD_LED_CFG_SET_BKL_EFF											34		//指定灯效
#define USB_DEF_CMD_LED_CFG_SET_SPEED											35		//指定当前灯效的速度
#define USB_DEF_CMD_LED_CFG_SET_RGB_CH_LEVEL									36		//设置RGB颜色通道的亮度等级

#define USB_DEF_CMD_LED_CFG_SET_INDICATOR_CFG									37		//设置兼容状态灯的配置
#define USB_DEF_CMD_LED_CFG_SET_STEP											38		//设置灯效衰减的幅度
#define USB_DEF_CMD_LED_CFG_SET_USB_DISCONN_FUNC                                39      //设置USB断开后，是否关闭背光

#define USB_DEF_CMD_LED_CFG_SET_WS_LED											40		//设置WS2812的灯效
#define USB_DEF_CMD_LED_CFG_SET_ST_LED_ACTION									41		//设置指示器

#define USB_DEF_CMD_CUSTOM_LED_SET_CFG											50		//设置自定义背光的配置
#define USB_DEF_CMD_CUSTOM_LED_NEED_PIC											51		//主机要求获取一个图片的数据
#define USB_DEF_CMD_CUSTOM_LED_GET_PIC											52		//主机发来一个图片

#define USB_DEF_CMD_RESET_TO_BOOTLOADER											60		//复位到BootLoader



//与BootLoader的通信命令
//PC发来的命令
#define	_DEF_USB_RECV_NONE										        		0		//空命令
#define	_DEF_USB_RECV_FW_HEAD									        		1		//主机发来固件的数据结构
#define	_DEF_USB_RECV_FW_DATA									        		2		//主机发来固件的数据
#define	_DEF_USB_RECV_NEED_HW_INFO								        		3		//主机要获取硬件的信息
#define	_DEF_USB_RECV_RUN_APP									        		4		//主机要求启动APP
#define	_DEF_USB_RECV_CLEAR_FLASH								        		5		//主机要求清空外置存储器

//发给PC的命令
#define	_DEF_USB_SEND_NONE										        		0		//空命令
#define	_DEF_USB_SEND_ERASE_FLASH_DONE							        		1		//片内FLASH擦除成功
#define	_DEF_USB_SEND_ERASE_W25Q_DONE							        		2		//外置存储器擦除完毕
#define	_DEF_USB_SEND_HW_INFO									        		3		//发送硬件信息给主机
#define	_DEF_USB_SEND_ERASE_FLASH_ERR							        		4		//擦除片内FLASH失败
#define	_DEF_USB_SEND_FW_PROGRAM_DONE							        		5		//固件编程完毕
#define	_DEF_USB_SEND_FW_PROGRAM_ERR							        		6		//固件编程失败
#define	_DEF_USB_SEND_W25Q_ERASE_POS							        		7		//显示W25Q的擦除进度

//程序使用的定义
#define		_DEF_PACK_FILE_HEAD_1				0x82
#define		_DEF_PACK_FILE_HEAD_2				0x81

//---------------------------BootLoader使用的数据结构------------------------
#pragma pack(1)

//灯珠像素的数据结构
struct LED_STRUCT_ONE_LED_RGB
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

struct LED_STRUCT_ONE_LED_RGB_MAP
{
	uint8_t *R;
	uint8_t *G;
	uint8_t *B;
};

//兼容状态灯
struct EFF_STRUCT_INDICATOR_LED_CFG
{
	uint8_t Enable : 1; //是否允许
	uint8_t WorkMode : 2; //工作模式
	uint8_t UseBklEffect : 1; //是否跟随轴灯灯效，0=不，1=是，同时本身的灯效关闭
	uint8_t Effect : 4; //灯效索引

	struct LED_STRUCT_ONE_LED_RGB FixColor; //固定的颜色

	struct//如果使用WS2812LED
	{
		uint8_t WsLedPoint; //WS2812的内存地址
	}

	WsLed;

	struct//如果使用背光LED
	{//记录本轴灯使用的轴灯坐标数据
		uint8_t SrcBkLed_x; //原始的背光轴灯，所使用的物理坐标
		uint8_t SrcBkLed_y; //原始的背光轴灯，所使用的物理坐标
		uint8_t UserLed_x; //背光的逻辑坐标
		uint8_t UserLed_y; //背光的逻辑坐标
	}

	BkLed;
};

//数据包格式
//发送设备的硬件和软件信息给主机
union CP_UNION_HL_BIT
{
	struct
	{
		uint8_t u8_1;
		uint8_t u8_2;
	}

	HL;
	uint16_t u16_1;
};

struct CP_STRUCT_PACK_CMD
{
	uint8_t Cmd;
	union CP_UNION_HL_BIT Pa1;
	union CP_UNION_HL_BIT pa2;
	union CP_UNION_HL_BIT pa3;
};

struct CP_STRUCT_PACK_HW_FW_INFO
{
	uint8_t Cmd; //命令
	uint8_t HwVer; //硬件版本
	uint8_t FwVer; //固件版本
	uint8_t BlVer; //BootLoader固件版本
};

struct CP_STRUCT_PACK_BL_HW_INFO
{
	uint8_t Cmd;
	uint8_t Hw_Ver;
	uint8_t Bl_Ver;
};

//设置背光的亮度等级
struct CP_STRUCT_PACK_BKL_LIG_LEVEL
{
	uint8_t Cmd;
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

//设置当前展示第几个灯效
struct CP_STRUCT_PACK_SET_EFF_INDEX
{
	uint8_t Cmd;
	uint8_t SetFunc; //设置什么的灯效：0=背光灯效
	uint8_t EffIndex;
};

//切换灯效
struct CP_STRUCT_PACK_SW_EFFECT
{
	uint8_t Cmd;
	uint8_t SetFunc; //设置功能：0=背光灯效
	uint8_t SetMode; //切换方向：0=上一个灯效，1=下一个灯效
};

//设置灯效的速度
struct CP_STRUCT_PACK_SET_BKL_SPEED
{
	uint8_t Cmd;
	uint8_t SetFunc; //设置哪个灯效：0=背光...
	uint8_t SetID; //设置第几个灯效
	uint16_t Speed; //设置的速度
};

//获取某些数据（功能层，触发层，宏等等）
struct CP_STRUCT_GET_DATA
{
	uint8_t Cmd;
	uint16_t ID;
};

//多包数据的分包数据格式
struct CP_STRUCT_PACK_64BYTE
{
	uint8_t Cmd; //命令
	uint8_t CurrPackSize; //当前数据包携带大小
	uint16_t id; //某层或者某组的ID编号
	uint32_t TotalPackSize; //总数据包大小
	uint8_t PackData[CP_DEF_MAX_CARRY_DATA_SIZE]; //有效携带的数据内容
};

//设置兼容LED的数据
struct CP_STRUCT_SET_INDICATOR_LED_DATA
{
	uint8_t Cmd;
	uint8_t Leds; //设置哪个LED
	struct EFF_STRUCT_INDICATOR_LED_CFG Data;
};

struct CP_STRUCT_BL_W25Q_ERASE_POS//擦除W25Q的进度通知
{
	uint8_t Cmd;
	uint32_t Pos;
	uint32_t Max;
};

union CP_UNION_ONE_PACK_64BYTE
{
	struct CP_STRUCT_PACK_CMD Cmd; //专用于发送各种命令
	struct CP_STRUCT_PACK_HW_FW_INFO HwFwInfo; //软硬件信息
	struct CP_STRUCT_PACK_BL_HW_INFO BlInfo; //BootLoader的信息
	struct CP_STRUCT_BL_W25Q_ERASE_POS Bl_Erase_W25q_Pos; //擦除W25Q的进度通知

	struct CP_STRUCT_PACK_BKL_LIG_LEVEL Bkl_LigLevel; //背光亮度
	struct CP_STRUCT_PACK_SET_EFF_INDEX SetEffIndex; //设置背光展示第几个灯效
	struct CP_STRUCT_PACK_SW_EFFECT SwEffect; //切换灯效
	struct CP_STRUCT_PACK_SET_BKL_SPEED SetEffectSpeed; //设置灯效的速度
	struct CP_STRUCT_SET_INDICATOR_LED_DATA SetIndicatorLed; //设置兼容LED的数据
	struct CP_STRUCT_GET_DATA Get_x_Data; //主机要求获取某数据

	struct CP_STRUCT_PACK_64BYTE LongDataPack; //长数据包，分包发送的数据格式（超过56字节的）
	uint8_t RawData[CP_DEF_MAX_RAW_PACK_SIZE]; //原始的数据包内容
};

#pragma pack()           //1字节对齐结束

//按键扫描结构

//扫描线IO映射表
struct KEYSCAN_STRUCT_PHY_SCAN_IO_MAP
{
	GPIO_TypeDef *GPIOx; //IO所在的端口
	uint16_t Pin; //端口的pin
};

//行列扫描结构
#define		KEYSCAN_DEF_PHY_SCAN_ROW_LINE_MAX					              	22	//扫描IO最多可以有22行
#define 	KEYSCAN_DEF_PHY_SCAN_COL_LINE_MAX					              	8	//扫描IO最多可以有8列
#define		KEYSCAN_DEF_PHY_SCAN_MAX_MATRIX										(KEYSCAN_DEF_PHY_SCAN_COL_LINE_MAX * KEYSCAN_DEF_PHY_SCAN_ROW_LINE_MAX) //最多可以有多少个按键

struct KEYSCAN_STRUCT_PHY_SCAN_LINE
{
	uint8_t UseRowLine; //实际使用了多少行线
	uint8_t UseColLine; //实际使用了多少列线
	struct KEYSCAN_STRUCT_PHY_SCAN_IO_MAP RowLine[KEYSCAN_DEF_PHY_SCAN_ROW_LINE_MAX]; //最多可以有22行扫描
	struct KEYSCAN_STRUCT_PHY_SCAN_IO_MAP ColLine[KEYSCAN_DEF_PHY_SCAN_COL_LINE_MAX]; //最多可以有8列扫描线
};

struct KEYSCAN_STRUCT_PHY_IO_MAP_TABLE
{
	struct KEYSCAN_STRUCT_PHY_SCAN_LINE IoMap; //IO映射表
	uint8_t KeyStatusMap[KEYSCAN_DEF_PHY_SCAN_MAX_MATRIX]; //硬件状态映射表
};

//配置信息
struct KEYSCAN_STRUCT_CONFIG
{
	struct KEYSCAN_STRUCT_PHY_IO_MAP_TABLE IoMapTable; //IO映射表

	struct
	{
		//基本参数配置
		uint8_t KeyDebounceTime; //按键消抖的时间ms
		uint8_t KeyMax; //最多有多少个按键
		uint16_t KeyUp_TimeOut; //抬起按键后，到达这个时间，则会自动归零按下次数计数器ms
		uint16_t KeyScanFreq; //按键的扫描频率，不应低于250hz，500hz以上最好
		uint16_t HoldDownTime; //按住保持触发键的检测时间，按住达到这个时间，就触发
		uint8_t ColScanTime; //列扫描间隔时间，13.88*4 = 纳秒的倍数
	}

	Config;
};

//LED配置

#define	LED_DEF_MBI5042_ROW_SCAN_MAX_LINE											8					//行扫描最多8行

struct LED_STRUCT_ONE_IO_MAP
{
	GPIO_TypeDef *GPIOx; //IO所在的端口
	uint16_t Pin; //端口的pin
};

struct LED_STRUCT_MBI5042_IO_MAP
{
	struct LED_STRUCT_ONE_IO_MAP GCLK; //PWM时钟
	struct LED_STRUCT_ONE_IO_MAP LE; //命令线
	struct LED_STRUCT_ONE_IO_MAP DCLK; //数据时钟
	struct LED_STRUCT_ONE_IO_MAP R_DATA; //红色通道数据
	struct LED_STRUCT_ONE_IO_MAP G_DATA; //绿色通道数据
	struct LED_STRUCT_ONE_IO_MAP B_DATA; //蓝色通道数据
};

//行扫描IO映射表
struct LED_STRUCT_ROW_SCAN_IO_MAP
{
	uint8_t RowScanMaxLine; //目前行扫描的行数(最多8行)
	struct LED_STRUCT_ONE_IO_MAP RowMap[LED_DEF_MBI5042_ROW_SCAN_MAX_LINE];
};

struct LED_WS2812_IO_MAP
{
	uint8_t Timx; //使用哪个端口
	uint8_t Timx_Ch; //使用哪个通道
	struct LED_STRUCT_ONE_IO_MAP DATA_OUT; //数据输出端口
};

#define	LED_DEF_LED_MATRIX_POINT_CAPTION_MAX															5			//每个按钮的文字

//用户显示矩阵的映射表，坐标对应一个物理显示矩阵的坐标
struct LED_STRUCT_USER_MATRIX_ONE_MAP
{
	uint8_t Phy_x; //其中任何一个值为0xff，则映射为空

	uint8_t Phy_y : 7; //其中任何一个值>=8，则映射为空
	uint8_t Enable : 1; //是否启用，0=不启用，1=启用，不启用的会被映射为空

	uint16_t Button_x; //对应按钮的坐标
	uint16_t Button_y; //对应按钮的坐标

	uint16_t Button_w : 9; //按钮的宽
	uint16_t Button_h : 7; //按钮的高

	uint8_t Keyid; //对应的按键编号
	uint8_t Caption[LED_DEF_LED_MATRIX_POINT_CAPTION_MAX]; //按钮的文字

};

#define	LED_DEF_USER_MATRIX_X_MAX												22			//用户显示矩阵的X方向最大坐标
#define	LED_DEF_USER_MATRIX_Y_MAX												6			//用户显示矩阵的Y方向最大坐标

struct LED_STRUCT_PHY_CONFIG
{
	//IO映射表
	struct
	{
		struct LED_STRUCT_MBI5042_IO_MAP MBI5042; //MBI5042IO映射
		struct LED_WS2812_IO_MAP WS2812; //WS2812数据输出IO映射
		struct LED_STRUCT_ROW_SCAN_IO_MAP RowScan; //行扫描输出IO映射
		uint16_t RowScanFreq; //行扫描频率
		uint16_t RowScanHideTime; //行扫描消隐时间（10us的倍数）
	}

	IoMap;

	//LED用户显示矩阵映射表
	struct
	{
		uint8_t LED_X; //用户显示矩阵的X方向有多少列
		uint8_t LED_Y; //用户显示矩阵的Y方向有多少行
		struct LED_STRUCT_USER_MATRIX_ONE_MAP MatrixMap[LED_DEF_USER_MATRIX_X_MAX][LED_DEF_USER_MATRIX_Y_MAX]; //显示矩阵映射表
	}

	LEDMatrixCfg;
};

//用户显示矩阵编辑器的数据
#define	CP_DEF_USER_MATRIX_PAN_MAX_WIDTH 										48		//默认的pan尺寸
#define	CP_DEF_USER_MATRIX_PAN_MAX_HEIGHT										48		//默认的pan尺寸
#define	CP_DEF_USER_MATRIX_FIRST_PAN_TOP										25		//默认生成的pan的位置
#define	CP_DEF_USER_MATRIX_FIRST_PAN_LEFT										18		//默认生成的pan的位置

struct CP_STRUCT_USER_DISP_MATRIX_MAP
{
	TPanel *pan;
	TLabel *str;
	int User_x;
	int User_y;
	int Keyidx;
	int Phy_x;
	int Phy_y;
	AnsiString Caption;
	int Enable;
};

//常规层编辑器
//键盘的执行命令
#define	FUNC_DEF_CMD_NONE														0					//对于常规触发，这相当于透明键，宏执行器碰到本命令，就会停止，其他的功能，会忽略
#define	FUNC_DEF_CMD_NORMAL_KEY_DISABLE											1					//常规触发，本按键被禁用
#define	FUNC_DEF_CMD_NORMAL_KEY_CODE											2
#define	FUNC_DEF_CMD_MEDIA_KEY_CODE												3
#define	FUNC_DEF_CMD_ENABLE_FLAYER												4
#define	FUNC_DEF_CMD_SET_FLAYER_TOP_PRIORITY									5
#define	FUNC_DEF_CMD_ACTIVE_TLAYER												6
#define	FUNC_DEF_CMD_FN_FUNC_ENABLE												7
#define	FUNC_DEF_CMD_FN_KEY_ENABLE												8
#define	FUNC_DEF_CMD_COMBO_FUNC_ENABLE											9
#define	FUNC_DEF_CMD_COMBO_KEY_ENABLE											10
#define	FUNC_DEF_CMD_NHIT_FUNC_ENABLE											11
#define	FUNC_DEF_CMD_NHIT_KEY_ENABLE											12
#define	FUNC_DEF_CMD_2IN1_FUNC_ENABLE											13
#define	FUNC_DEF_CMD_2IN1_KEY_ENABLE											14
#define	FUNC_DEF_CMD_HOLDDOWN_FUNC_ENABLE										15
#define	FUNC_DEF_CMD_HOLDDOWN_KEY_ENABLE										16
#define	FUNC_DEF_CMD_NORMAL_KEY_ENABLE											17
#define	FUNC_DEF_CMD_RUN_KEY_LOCK												18
#define	FUNC_DEF_CMD_RUN_MACRO													19

#define	FUNC_DEF_CMD_MACRO_CURR_JMP												20
#define	FUNC_DEF_CMD_MACRO_GROUP_JMP											21
#define	FUNC_DEF_CMD_MACRO_DELAY												22
#define	FUNC_DEF_CMD_MACRO_P_NORMAL_KEY											23
#define	FUNC_DEF_CMD_MACRO_R_NORMAL_KEY											24
#define	FUNC_DEF_CMD_MACRO_P_MEDIA_KEY											25
#define	FUNC_DEF_CMD_MACRO_R_MEDIA_KEY											26

#define	FUNC_DEF_CMD_MACRO_STOP													27

#define	FUNC_DEF_CMD_BKL_LAST_EFF												28
#define	FUNC_DEF_CMD_BKL_NEXT_EFF												29
#define	FUNC_DEF_CMD_BKL_SET_EFF												30
#define	FUNC_DEF_CMD_BKL_EFF_FAST												31
#define	FUNC_DEF_CMD_BKL_EFF_SLOW												32
#define	FUNC_DEF_CMD_BKL_EFF_SPEED_SET											33

//背光亮度
#define FUNC_DEF_CMD_BKL_LOW_ALL_LED_LIG_LEVEL									34	//降低LED的背光亮度，整体的
#define FUNC_DEF_CMD_BKL_HIGH_ALL_LED_LIG_LEVEL									35	//提高LED的背光亮度，整体的

#define FUNC_DEF_CMD_WS_ENABLE_GROUP_FUNC										36	//开启或者关闭WS2812的灯效
#define FUNC_DEF_CMD_WS_SET_GROUP_LAST_EFF										37	//切换WS的灯效到上一个
#define FUNC_DEF_CMD_WS_SET_GROUP_NEXT_EFF										38	//切换WS的灯效到下一个
#define FUNC_DEF_CMD_WS_SET_GROUP_EFFECT										39	//指定某一组WS使用哪个灯效

#define FUNC_DEF_CMD_ST_LED_ENABLE_CAPS											40	//开启或者关闭CAPS兼容指示灯的功能
#define FUNC_DEF_CMD_ST_LED_ENABLE_SCROLL										41	//开启或者关闭Scroll兼容指示灯的功能
#define FUNC_DEF_CMD_ST_LED_ENABLE_NUM											42	//开启或者关闭Num兼容指示灯的功能

#define FUNC_DEF_CMD_LED_ACTION_CAPS_ENABLE										43	//开启或者关闭CAPS的动作触发功能
#define FUNC_DEF_CMD_LED_ACTION_SCROLL_ENABLE									44	//开启或者关闭SCROLL的动作触发功能
#define FUNC_DEF_CMD_LED_ACTION_NUM_ENABLE										45	//开启或者关闭NUM的动作触发功能

#define FUNC_DEF_CMD_BKL_ENABLE													46	//开启或者关闭背光

#pragma pack(1) //1字节对齐

//功能码定义

//1、普通键码
struct FUNC_STRUCT_NORMAL_KEY_FUNC
{
	uint8_t FuncID; //功能ID
	uint8_t KeyCode; //键码
	uint8_t Status; //状态，0=抬起，1=按下
	uint8_t PressCount; //按下次数，此处不使用
};

//2、多媒体
struct FUNC_STRUCT_MEDIA_KEY_FUNC
{
	uint8_t FuncID;
	uint8_t Status;
	uint16_t MediaKeyCode;
};

//3、开关功能层
struct FUNC_STRUCT_ENABLE_FLAYER
{
	uint8_t FuncID;
	uint8_t LayerID; //层ID编号
	uint8_t Enable; //打开还是关闭，0=关闭，1=打开
	uint8_t None; //不使用
};

//5、把某功能层设置为最高优先级（最先扫描的层）
struct FUNC_STRUCT_SET_PRIORITY
{
	uint8_t FuncID;
	uint8_t LayerID;
	uint16_t None; //不使用
};

//6、激活某触发层
struct FUNC_STRUCT_ACTIVE_TLAYER
{
	uint8_t FuncID;
	uint8_t TlayerID;
	uint16_t None; //不使用
};

//7、打开或者关闭当前层所有FN键的触发
struct FUNC_STRUCT_FN_FUNC_ENABLE
{
	uint8_t FuncID;
	uint8_t Enable;
	uint16_t None;
};

//8、打开或者关闭本层单个FN键的触发
struct FUNC_STRUCT_FN_ONE_KEY_ENABLE
{
	uint8_t FuncID;
	uint8_t Keyid;
	uint8_t Enable;
	uint8_t None;
};

//9、打开或者关闭组合键功能
struct FUNC_STRUCT_COMBO_KEY_ENABLE
{
	uint8_t FuncID;
	uint8_t Enable;
	uint16_t None;
};

//10、打开或者关闭某个组合键组的功能
struct FUNC_STRUCT_COMBO_KEY_GROUP_ENABLE
{
	uint8_t FuncID;
	uint8_t GroupID;
	uint8_t Enable;
	uint8_t None;
};

//11、打开或者关闭连击键的功能
struct FUNC_STRUCT_nHIT_FUNC_ENABLE
{
	uint8_t FuncID;
	uint8_t Enable;
	uint16_t None;
};

//12、打开或者关闭某个连击键的开关
struct FUNC_STRUCT_nHIT_KEY_ENABLE
{
	uint8_t FuncID;
	uint8_t Keyid;
	uint8_t Enable;
	uint8_t None;
};

//13、打开或者关闭二合一键的触发功能
struct FUNC_STRUCT_TWO1_FUNC_ENABLE
{
	uint8_t FuncID;
	uint8_t Enable;
	uint16_t None;
};

//14、打开或者关闭某个二合一键的触发功能
struct FUNC_STRUCT_TWO1_KEY_ENABLE
{
	uint8_t FuncID;
	uint8_t Keyid;
	uint8_t Enable;
	uint8_t None;
};

//15、打开或者关闭按下保持键的触发功能
struct FUNC_STRUCT_HOLDDOWN_FUNC_ENABLE
{
	uint8_t FuncID;
	uint8_t Enable;
	uint16_t None;
};

//16、打开或者关闭某个按下保持键的触发功能
struct FUNC_STRUCT_HOLDDOWN_KEY_ENABLE
{
	uint8_t FuncID;
	uint8_t Keyid;
	uint8_t Enable;
	uint8_t None;
};

//17、打开或者关闭某个常规触发键的触发功能
struct FUNC_STRUCT_NORMAL_TRIG_KEY_ENABLE
{
	uint8_t FuncID;
	uint8_t Keyid;
	uint8_t Enable;
	uint8_t None;
};

//18、执行一个锁定键功能
struct FUNC_STRUCT_RUN_KEY_LOCK
{
	uint8_t FuncID;
	uint16_t GroupID;
	uint8_t None;
};

//19、执行一组宏
struct FUNC_STRUCT_RUN_MACRO
{
	uint8_t FuncID;
	uint8_t GroupID; //宏组ID
	uint16_t StepID; //运行第几步
};

//20、宏当前组内跳转
struct FUNC_STRUCT_MACRO_CURR_JMP
{
	uint8_t FuncID;
	uint8_t None;
	uint16_t JmpStepID; //跳转到第几步
};

//21、宏跨组跳转
struct FUNC_STRUCT_MACRO_GROUP_JMP
{
	uint8_t FuncID;
	uint8_t GroupID; //跳转到哪个组
	uint16_t JmpStepID; //从哪一步开始执行
};

//22、宏延迟
struct FUNC_STRUCT_MACRO_DELAY
{
	uint32_t FuncID : 8; //8比特指令
	uint32_t Timems : 24; //24比特时间值，毫秒
};

//23、按下一个普通键码
struct FUNC_STRUCT_MACRO_PRESS_NORMAL_KEY
{
	uint8_t FuncID;
	uint8_t KeyCode;
	uint16_t None;
};

//24、释放一个普通键码
struct FUNC_STRUCT_MACRO_RELEASE_NORMAL_KEY
{
	uint8_t FuncID;
	uint8_t KeyCode;
	uint16_t None;
};

//25、按下一个多媒体键码
struct FUNC_STRUCT_MACRO_PRESS_MEDIA_KEY
{
	uint8_t FuncID;
	uint16_t MediaCode;
	uint8_t None;
};

//26、释放一个多媒体键码
struct FUNC_STRUCT_MACRO_RELEASE_MEDIA_KEY
{
	uint8_t FuncID;
	uint16_t MediaCode;
	uint8_t None;
};

//27、停止一个宏的执行
struct FUNC_STRUCT_MACRO_STOP_RUN
{
	uint8_t FuncID;
	uint8_t None1;
	uint16_t None2;
};

//28、切换上一个背光灯效
struct FUNC_STRUCT_BKL_LAST_EFF
{
	uint8_t FuncID;
	uint8_t None1;
	uint16_t None2;
};

//29、切换下一个灯效
struct FUNC_STRUCT_BKL_NEXT_EFF
{
	uint8_t FuncID;
	uint8_t None1;
	uint16_t None2;
};

//30、指定切换到某个灯效
struct FUNC_STRUCT_BKL_DISP_EFF
{
	uint8_t FuncID;
	uint8_t EffIndex;
	uint16_t None;
};

//31、提高背光灯效运行速度
struct FUNC_STRUCT_BKL_FAST
{
	uint8_t FuncID;
	uint8_t EffIndex;
	uint16_t None;
};

//32、降低背光灯效运行速度
struct FUNC_STRUCT_BKL_SLOW
{
	uint8_t FuncID;
	uint8_t EffIndex;
	uint16_t None;
};

//33、指定背光灯效运行速度
struct FUNC_STRUCT_BKL_SET_SPEED
{
	uint8_t FuncID;
	uint8_t EffIndex;
	uint16_t Speed;
};

//34降低或者提高背光亮度
struct FUNC_STRUCT_BKL_LH_BKL_LEVEL
{
	uint8_t FuncID;
	uint8_t None1;
	uint16_t None2;
};

//35开启或者关闭WS的某个组
struct FUNC_STRUCT_WS_ENABLE_GROUP
{
	uint8_t FuncID;
	uint8_t GroupID;
	uint8_t Enable;
	uint8_t None;
};

//36切换WS某个组的灯效
struct FUNC_STRUCT_WS_UD_EFFECT
{
	uint8_t FuncID;
	uint8_t GroupID;
	uint8_t Mode;
	uint8_t None;
};

//37设置某个WS组使用哪个灯效
struct FUNC_STRUCT_WS_SET_EFFECT
{
	uint8_t FuncID;
	uint8_t GroupID;
	uint8_t EffIndex;
	uint8_t None;
};

//38设置三个兼容指示灯的状态
struct FUNC_STRUCT_ST_LED_ENABLE
{
	uint8_t FuncID;
	uint8_t Leds; //设置哪个，0=CAPS，1=SCROLL，2=num
	uint8_t Enable; //1=开启，0=关闭
	uint8_t None;
};

//39开启或者关闭LED指示灯的动作
struct FUNC_STRUCT_LED_ACTION_ENABLE
{
	uint8_t FuncID;
	uint8_t Leds; //设置哪个，0=CAPS，1=SCROLL，2=num
	uint8_t Enable; //1=开启，0=关闭
	uint8_t None;
};

//40开启或者关闭背光
struct FUNC_STRUCT_BKL_ENABLE
{
	uint8_t FuncID;
	uint8_t None1;
	uint16_t None2;
};

//功能列表
union FUNC_UNION_COMMAND_LIST
{
	//数据结构集合(内存共用)
	struct FUNC_STRUCT_NORMAL_KEY_FUNC NormalKey; //普通键码
	struct FUNC_STRUCT_MEDIA_KEY_FUNC MediaKey; //多媒体键码

	struct FUNC_STRUCT_ENABLE_FLAYER FLayerEnable; //功能层开关
	struct FUNC_STRUCT_SET_PRIORITY FLayerSetTopPriority; //功能层最高优先级

	struct FUNC_STRUCT_ACTIVE_TLAYER TLayerActive; //激活某触发层

	struct FUNC_STRUCT_FN_FUNC_ENABLE FnFuncEnable; //禁止或者打开所有FN键的触发
	struct FUNC_STRUCT_FN_ONE_KEY_ENABLE FnKeyEnable; //禁止或者打开单独的FN键的触发

	struct FUNC_STRUCT_COMBO_KEY_ENABLE ComboFuncEnable; //组合键总开关
	struct FUNC_STRUCT_COMBO_KEY_GROUP_ENABLE ComboGroupEnable; //组合键组开关

	struct FUNC_STRUCT_nHIT_FUNC_ENABLE nHitFuncEnable; //连击键总开关
	struct FUNC_STRUCT_nHIT_KEY_ENABLE nHitKeyEnable; //连击键单独开关

	struct FUNC_STRUCT_TWO1_FUNC_ENABLE Two1FuncEnable; //二合一键总开关
	struct FUNC_STRUCT_TWO1_KEY_ENABLE Two1KeyEnable; //二合一键单独开关

	struct FUNC_STRUCT_HOLDDOWN_FUNC_ENABLE HoldDownFuncEnable; //按下保持键的总开关
	struct FUNC_STRUCT_HOLDDOWN_KEY_ENABLE HoldDownKeyEnable; //按下保持键的单独开关

	struct FUNC_STRUCT_NORMAL_TRIG_KEY_ENABLE NormalKeyEnable; //开启或者禁止某普通按键触发

	struct FUNC_STRUCT_RUN_KEY_LOCK RunKeyLockGroup; //锁定或者解锁一组锁定键

	struct FUNC_STRUCT_RUN_MACRO RunMacroGroup; //运行一组宏
	struct FUNC_STRUCT_MACRO_CURR_JMP MacroJmp; //宏组内跳转
	struct FUNC_STRUCT_MACRO_GROUP_JMP MacroGroupJmp; //跨组跳转
	struct FUNC_STRUCT_MACRO_DELAY MacroDelay; //宏延迟
	struct FUNC_STRUCT_MACRO_PRESS_NORMAL_KEY MacroPressNormalKey;
	//宏模拟按下一个普通按键
	struct FUNC_STRUCT_MACRO_RELEASE_NORMAL_KEY MacroReleaseNormalKey;
	//宏模拟释放一个普通按键
	struct FUNC_STRUCT_MACRO_PRESS_MEDIA_KEY MacroPressMediaKey; //宏模拟按下一个多媒体按键
	struct FUNC_STRUCT_MACRO_RELEASE_MEDIA_KEY MacroReleaseMediaKey;
	//宏模拟释放一个多媒体按键
	struct FUNC_STRUCT_MACRO_STOP_RUN MacroStopRun; //停止当前宏的运行

	struct FUNC_STRUCT_BKL_LAST_EFF BklLastEff; //背光顺序切换到上一个背光
	struct FUNC_STRUCT_BKL_NEXT_EFF BklNextEff; //背光顺序切换到下一个背光
	struct FUNC_STRUCT_BKL_DISP_EFF BklSetEff; //背光切换到指定的灯效
	struct FUNC_STRUCT_BKL_FAST BklSpeedFast; //提高背光灯效的运行速度
	struct FUNC_STRUCT_BKL_SLOW BklSpeedSlow; //降低背光灯效的运行速度
	struct FUNC_STRUCT_BKL_SET_SPEED BklSpeedSet; //指定背光灯效的运行速度

	struct FUNC_STRUCT_BKL_LH_BKL_LEVEL BklLedLigLevel; //设置背光的亮度
	struct FUNC_STRUCT_WS_ENABLE_GROUP BklWsEnableGroup; //设置WS的某个组是否启用
	struct FUNC_STRUCT_WS_UD_EFFECT BklWsSetGroupUDEff; //切换WS某个组的灯效
	struct FUNC_STRUCT_WS_SET_EFFECT BklWsSetGroupEffect; //指定WS某个组的灯效

	struct FUNC_STRUCT_ST_LED_ENABLE BklSetStLedEnable; //设置3个兼容指示灯是否启用
	struct FUNC_STRUCT_LED_ACTION_ENABLE BklSetLedActionEnable; //设置3个指示灯动作是否启用

	struct FUNC_STRUCT_BKL_ENABLE BklEnable; //开启或者关闭轴灯背光

};

struct CP_STRUCT_NORMAL_LAYER_EDIT_ONE_KEYS
{
	TImage *Icon;
	union FUNC_UNION_COMMAND_LIST Func;
	int KeyIndex;
};

//功能层，单个功能层的数据，一个层占用内存8*176=1408字节

#define 		LAYER_DEF_FLAYER_MAX_NUM                                        8		//功能层最多8个层
#define			KeyEdit_MAX_KEYS												128		//最多128个按键

//功能层的所有数据
struct LAYER_STRUCT_ONE_FLAYER
{
	union FUNC_UNION_COMMAND_LIST PFunc[KEYSCAN_DEF_PHY_SCAN_MAX_MATRIX]; //按下事件
	union FUNC_UNION_COMMAND_LIST RFunc[KEYSCAN_DEF_PHY_SCAN_MAX_MATRIX]; //抬起事件
};

struct LAYER_STRUCT_ALL_FLAYER
{
	struct LAYER_STRUCT_ONE_FLAYER FLayerData[LAYER_DEF_FLAYER_MAX_NUM];
	//所有功能层的数据
};

//触发层的数据
//FN键配置，一个键的配置
struct LAYER_STRUCT_TLAYER_ONE_FN_KEY
{
	uint8_t KeyID; //FN键对应的内部ID编号
	uint8_t Enable; //本按键是否开启，0=关闭，1=开启
	union FUNC_UNION_COMMAND_LIST PFunc; //按下触发的功能
	union FUNC_UNION_COMMAND_LIST RFunc; //释放触发的功能1
	union FUNC_UNION_COMMAND_LIST SFunc; //释放触发的功能2
};

//一个触发层的所有FN键(占用内存(2+4+4)=10 * 8 = 80 Byte
struct LAYER_STRUCT_TLAYER_FN_ALL_KEY
{
	struct LAYER_STRUCT_TLAYER_ONE_FN_KEY FnKeyData[LAYER_DEF_TLAYER_FN_KEY_MAX_NUM]; //一个触发层最多可设置8个FN键
};

//组合键的数据
#define	KEYSCAN_DEF_COMBO_KEY_MAX_NUM											4	//一组内最多可以配置8个按键参与触发
#define	KEYSCAN_DEF_COMBO_MAX_GROUP												48	//最多有多少组

struct LAYER_STRUCT_TLAYER_COMBO_ONE_GROUP
{
	struct
	{
		uint8_t Enable : 1; //是否开启
		uint8_t MaxKeyNum : 7; //配置了几个键
		uint8_t FnKeyID; //与之搭配的FN键内部键码
		uint8_t ComboKeyid[KEYSCAN_DEF_COMBO_KEY_MAX_NUM]; //本组参与触发的组合键键码
	}

	Config; //配置信息
	union FUNC_UNION_COMMAND_LIST PFunc; //按下触发的功能
	union FUNC_UNION_COMMAND_LIST RFunc; //释放触发的功能
};

//占用内存：一组18字节*48组=864 byte
struct LAYER_STRUCT_TLAYER_COMBO_ALL_GROUP
{
	struct LAYER_STRUCT_TLAYER_COMBO_ONE_GROUP GroupData[LAYER_DEF_TLAYER_COMBO_GROUP_MAX_NUM]; //一个触发层最多可设置48组组合键
};

//连击键一个键占用内存：6字节
struct LAYER_STRUCT_TLAYER_nHIT_ONE_KEY
{
	uint8_t KeyID; //在哪个键上注册这个功能

	uint8_t Enable : 1; //是否开启，0=关闭，1=开启
	uint8_t TrigCount : 7; //触发次数

	union FUNC_UNION_COMMAND_LIST Func; //触发后的功能
};

//占用内存:6*64=384 Byte
struct LAYER_STRUCT_TLAYER_nHIT_ALL_KEY
{
	struct LAYER_STRUCT_TLAYER_nHIT_ONE_KEY KeyData[LAYER_DEF_TLAYER_nHIT_KEY_MAX_NUM];
};

//二合一键
struct LAYER_STRUCT_TLAYER_2IN1_ONE_KEY
{
	uint8_t KeyID; //在哪个键上注册这个功能

	uint16_t Enable : 1; //是否开启本功能

	uint16_t LongPressTime; //长按的时间
	union FUNC_UNION_COMMAND_LIST SPFunc; //短按的功能
	union FUNC_UNION_COMMAND_LIST LPFunc; //长按的功能
	union FUNC_UNION_COMMAND_LIST LRFunc; //长按释放的功能
};

//占用空间:一个键15字节*64组=960 Byte
struct LAYER_STRUCT_TLAYER_2IN1_ALL_KEY
{
	struct LAYER_STRUCT_TLAYER_2IN1_ONE_KEY GroupData[LAYER_DEF_TLAYER_2IN1_KEY_MAX_NUM];
};

//按下保持键
struct LAYER_STRUCT_TLAYER_HOLD_DOWN_ONE_KEY
{
	uint8_t KeyID; //在哪个键上注册本功能

	uint8_t Enable : 1; //是否开启本功能
	uint8_t TrigCount : 7; //触发的次数

	union FUNC_UNION_COMMAND_LIST PFunc; //按下触发的功能
	union FUNC_UNION_COMMAND_LIST RFunc; //释放触发的功能
};

//占用空间：10*64=640 Byte
struct LAYER_STRUCT_TLAYER_HOLD_DOWN_ALL_KEY
{
	struct LAYER_STRUCT_TLAYER_HOLD_DOWN_ONE_KEY GroupData[LAYER_DEF_TLAYER_HOLD_DOWN_MAX_NUM];
};

//一个触发层的所有数据(一个触发层占用2834字节的内存)
struct LAYER_STRUCT_TLAYER_ONE_GROUP
{
	struct LAYER_STRUCT_TLAYER_FN_ALL_KEY FnKey;
	struct LAYER_STRUCT_TLAYER_COMBO_ALL_GROUP ComboKey;
	struct LAYER_STRUCT_TLAYER_nHIT_ALL_KEY nHitKey;
	struct LAYER_STRUCT_TLAYER_2IN1_ALL_KEY Two1Key;
	struct LAYER_STRUCT_TLAYER_HOLD_DOWN_ALL_KEY HoldDownKey;
};

//层的配置
struct LAYER_STRUCT_CONFIG
{
	struct
	{
		struct
		{
			uint8_t TopPriorityID; //最高优先级的层ID
			uint8_t LayerEnable[LAYER_DEF_FLAYER_MAX_NUM]; //每个层的开关，0=关闭，1=开启
		} FlayerCfg;
		//触发层的配置
		struct
		{
			uint8_t ActiveID; //当前激活的触发层ID
		} TlayerCfg;
	}

	Cfg;

	struct
	{
		TImage *Layer[LAYER_DEF_FLAYER_MAX_NUM]; //容器
	}

	Panel;

};

//宏
struct LAYER_STRUCT_MACRO_ONE_GROUP
{
	union FUNC_UNION_COMMAND_LIST Step[LAYER_DEF_MACRO_ONE_GROUP_MAX_STEP]; //512步
};

//锁定键
struct LAYER_STRUCT_KEY_LOCK_ONE_GROUP
{
	uint16_t KeyCode[LAYER_DEF_KEY_LOCK_GROUP_MAX_KEYS]; //最高位1=多媒体键码，0=普通键码
};



//ws2812灯效配置
struct EFF_STRUCT_WS_LED_CFG
{
	uint16_t Speed; //运行速度
	uint8_t Enable; //是否启用
	uint8_t Effect; //灯效
	uint8_t xMin; //起始灯珠坐标
	uint8_t xMax; //结束灯珠坐标
	struct LED_STRUCT_ONE_LED_RGB Color[EFF_DEF_WS_SET_COLOR_MAX]; //设定的颜色，每一组最多可设置4种自定义颜色，用于渐变等功能
};

//led动作
struct EFF_STRUCT_STATUS_LED_ACTION
{
	uint8_t Enable;
	union FUNC_UNION_COMMAND_LIST OnFunc;
	union FUNC_UNION_COMMAND_LIST OffFunc;
};

//自定义灯效的配置
struct EFF_STRUCT_CUSTOM_CFG
{
	uint8_t EffectIdx; //本组的灯效模式
	uint16_t UsePicIdx; //使用哪个图片
	uint16_t Speed; //速度
};

//USB断开后，是否关闭背光
union EFF_STRUCT_BKL_OFF_EVENT_BY_USB
{
	struct
	{
		uint8_t			Enable:1;				//总开关
		uint8_t			BklOff:1;				//背光关闭
		uint8_t			IndicatorOff:1;			//指示灯关闭
		uint8_t			wsG0:1;					//WS2812组0

		uint8_t			wsG1:1;					//WS2812组0
		uint8_t			wsG2:1;					//WS2812组0
		uint8_t			wsG3:1;					//WS2812组0
		uint8_t			none:1;					//未使用
	}Bit;
	uint8_t			FunctionSw;				//功能配置
};

//灯效的配置
struct EFF_STRUCT_EFF_CONFIG
{
	//灯效配置
	uint8_t BklEffectMax; //背光灯效有几个

	struct
	{
		uint8_t EffectIndex; //灯效索引
		uint8_t LastEffIdx; //开关灯效的索引
		struct LED_STRUCT_ONE_LED_RGB BackLigLevel; //颜色通道的亮度
		uint16_t EffectSpeed[EFF_DEF_BKL_MAX_EFFECT]; //灯效的速度
		uint8_t LigDownLevelStep[EFF_DEF_BKL_MAX_EFFECT]; //降低亮度的步数
	}

	EffectCfg;

	//状态指示灯配置
	struct
	{
		struct EFF_STRUCT_INDICATOR_LED_CFG Caps;
		struct EFF_STRUCT_INDICATOR_LED_CFG Scroll;
		struct EFF_STRUCT_INDICATOR_LED_CFG Num;
	}

	IndicatorCfg;
	//自定义灯效的配置
	struct EFF_STRUCT_CUSTOM_CFG CustomLedCfg[EFF_DEF_BKL_CUSTOM_MAX_EFFECT];
	//ws2812灯珠配置
	struct EFF_STRUCT_WS_LED_CFG WsCfg[EFF_DEF_WS_GROUP_MAX]; //暂定，最多4组
	struct EFF_STRUCT_STATUS_LED_ACTION LedAction[3];
    //当USB断开连接之后，是否关闭背光
	union EFF_STRUCT_BKL_OFF_EVENT_BY_USB	UsbDisConnOffEff;
};

//自定义背光编辑器
struct CUSTOM_BKL_ONE_ICON_INFO
{
	String Caption; //标题
	int Keyid; //按键编号
};
//一张图片的尺寸

#define CUSTOM_BKL_IMAGE_MAX        100     //最多可定义800张图片

struct CUSTOM_BKL_ONE_IMAGE
{
	struct LED_STRUCT_ONE_LED_RGB rgb[LED_DEF_USER_MATRIX_X_MAX][LED_DEF_USER_MATRIX_Y_MAX];
};

//---------------BootLoader----------------------------------------------------

//固件的数据头
struct _DEF_PACK_FILE_HEAD_STRUCT//固件数据头
{
	uint8_t Head1; //文件头1=0x82
	uint8_t Head2; //文件头2=0x81
	uint8_t Hw_Ver; //适用的硬件版本BCD码
	uint8_t BL_Ver; //适用的BL版本BCD码

	//键盘固件数据
	uint8_t KB_Fw_Ver; //键盘固件版本
	uint8_t KB_ErasePage_Count; //要擦除多少页
	uint32_t KB_Fw_Size; //固件的大小

	uint16_t W25Qxx_Erase_StartSector; //W25Qxx存储器从哪个页开始擦除
	uint16_t W25Qxx_Erase_Count; //擦除多少页，这个数如果为0，则不执行擦除操作
};

#pragma pack() //1字节对齐

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published: //IDE-managed Components
	TTimer *Timer1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TPanel *Panel9;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TPageControl *PageControl2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button9;
	TButton *Button10;
	TComboBox *ComboBox2;
	TComboBox *ComboBox1;
	TPanel *Panel10;
	TTabSheet *TabSheet5;
	TLabel *Label1;
	TLabel *Label2;
	TTabSheet *TabSheet6;
	TTabSheet *TabSheet7;
	TButton *Button11;
	TComboBox *ComboBox3;
	TLabel *Label3;
	TButton *Button12;
	TTabSheet *TabSheet9;
	TTabSheet *TabSheet8;
	TGroupBox *GroupBox1;
	TLabel *Label4;
	TComboBox *ComboBox4;
	TButton *Button13;
	TButton *Button14;
	TButton *Button15;
	TButton *Button16;
	TListView *MacroEdit;
	TGroupBox *GroupBox2;
	TButton *Button17;
	TButton *Button18;
	TButton *Button19;
	TButton *Button20;
	TListView *LockKeyEdit;
	TPopupMenu *KeyLockPopMenu;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TPopupMenu *MacroEditPopMenu;
	TMenuItem *N5;
	TMenuItem *N6;
	TTabSheet *TabSheet10;
	TGroupBox *GroupBox3;
	TLabel *Label5;
	TTrackBar *TrackBar1;
	TTrackBar *TrackBar2;
	TLabel *Label6;
	TTrackBar *TrackBar3;
	TLabel *Label7;
	TButton *Button21;
	TGroupBox *GroupBox4;
	TLabel *Label8;
	TTrackBar *TrackBar4;
	TButton *Button22;
	TLabel *Label9;
	TTrackBar *TrackBar5;
	TButton *Button23;
	TLabel *Label10;
	TTrackBar *TrackBar6;
	TButton *Button24;
	TButton *Button25;
	TButton *Button26;
	TGroupBox *GroupBox5;
	TButton *Button27;
	TRadioButton *RadioButton3;
	TRadioButton *RadioButton4;
	TRadioButton *RadioButton5;
	TRadioButton *RadioButton6;
	TImage *Image1;
	TPanel *Panel20;
	TCheckBox *CheckBox1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TButton *Button30;
	TGroupBox *GroupBox6;
	TImage *Image2;
	TButton *Button28;
	TRadioButton *RadioButton7;
	TRadioButton *RadioButton8;
	TRadioButton *RadioButton9;
	TRadioButton *RadioButton10;
	TPanel *Panel21;
	TCheckBox *CheckBox2;
	TRadioButton *RadioButton11;
	TRadioButton *RadioButton12;
	TButton *Button29;
	TGroupBox *GroupBox7;
	TImage *Image3;
	TButton *Button31;
	TRadioButton *RadioButton13;
	TRadioButton *RadioButton14;
	TRadioButton *RadioButton15;
	TRadioButton *RadioButton16;
	TPanel *Panel22;
	TCheckBox *CheckBox3;
	TRadioButton *RadioButton17;
	TRadioButton *RadioButton18;
	TButton *Button32;
	TColorDialog *ColorDialog1;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TCheckBox *CheckBox4;
	TCheckBox *CheckBox5;
	TCheckBox *CheckBox6;
	TTabSheet *TabSheet11;
	TGroupBox *GroupBox8;
	TGroupBox *GroupBox9;
	TLabel *Label14;
	TComboBox *ComboBox5;
	TCheckBox *CheckBox7;
	TLabel *Label15;
	TComboBox *ComboBox6;
	TLabel *Label16;
	TComboBox *ComboBox7;
	TRadioButton *RadioButton19;
	TRadioButton *RadioButton20;
	TRadioButton *RadioButton21;
	TRadioButton *RadioButton22;
	TRadioButton *RadioButton23;
	TImage *Image4;
	TImage *Image5;
	TImage *Image6;
	TImage *Image7;
	TButton *Button33;
	TTrackBar *WS_Speed;
	TGroupBox *GroupBox10;
	TLabel *Label17;
	TPanel *Panel23;
	TLabel *Label18;
	TPanel *Panel24;
	TCheckBox *CheckBox8;
	TGroupBox *GroupBox11;
	TLabel *Label19;
	TLabel *Label20;
	TPanel *Panel25;
	TPanel *Panel26;
	TCheckBox *CheckBox9;
	TGroupBox *GroupBox12;
	TLabel *Label21;
	TLabel *Label22;
	TPanel *Panel27;
	TPanel *Panel28;
	TCheckBox *CheckBox10;
	TButton *Button34;
	TButton *Button35;
	TButton *Button36;
	TTabSheet *TabSheet12;
	TPanel *Panel29;
	TGroupBox *GroupBox13;
	TLabel *Label23;
	TComboBox *ComboBox8;
	TButton *Button37;
	TRadioButton *RadioButton24;
	TRadioButton *RadioButton25;
	TRadioButton *RadioButton26;
	TRadioButton *RadioButton27;
	TRadioButton *RadioButton28;
	TGroupBox *GroupBox14;
	TLabel *Label24;
	TComboBox *ComboBox9;
	TLabel *Label25;
	TComboBox *ComboBox10;
	TButton *Button39;
	TButton *Button40;
	TButton *Button41;
	TButton *Button42;
	TButton *Button43;
	TPanel *Panel30;
	TTrackBar *TrackBar7;
	TTabSheet *TabSheet13;
	TGroupBox *GroupBox15;
	TButton *Button38;
	TButton *Button44;
	TButton *Button45;
	TButton *Button46;
	TButton *Button47;
	TButton *Button48;
	TProgressBar *ProgressBar1;
	TLabel *Label26;
	TGroupBox *GroupBox16;
	TLabel *Label27;
	TComboBox *ComboBox11;
	TImage *Img0;
	TImage *Img1;
	TImage *Img2;
	TImage *Img3;
	TImage *Img4;
	TImage *Img5;
	TImage *Img6;
	TImage *Img7;
	TImage *FLIcon0;
	TImage *FLIcon1;
	TImage *FLIcon3;
	TImage *FLIcon2;
	TImage *FLIcon5;
	TImage *FLIcon4;
	TImage *FLIcon7;
	TImage *FLIcon6;
	TLabel *Label28;
	TComboBox *ComboBox12;
	TGroupBox *GroupBox17;
	TCheckBox *CheckBox11;
	TCheckBox *CheckBox12;
	TCheckBox *CheckBox13;
	TCheckBox *CheckBox14;
	TCheckBox *CheckBox15;
	TCheckBox *CheckBox16;
	TButton *Button49;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall OnFLIconClick(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall ComboBox1Select(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall TLayerKeyListClick(TObject *Sender);
	void __fastcall ComboBox2Select(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall PageControl2Change(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall ComboBox3Change(TObject *Sender);
	void __fastcall MacroEditDblClick(TObject *Sender);
	void __fastcall ComboBox4Change(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall Button14Click(TObject *Sender);
	void __fastcall Button16Click(TObject *Sender);
	void __fastcall Button15Click(TObject *Sender);
	void __fastcall LockKeyEditDblClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall LockKeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Button17Click(TObject *Sender);
	void __fastcall Button18Click(TObject *Sender);
	void __fastcall Button19Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
	void __fastcall MacroEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall Button25Click(TObject *Sender);
	void __fastcall Button26Click(TObject *Sender);
	void __fastcall Button21Click(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall Button24Click(TObject *Sender);
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall Button30Click(TObject *Sender);
	void __fastcall Button29Click(TObject *Sender);
	void __fastcall Button32Click(TObject *Sender);
	void __fastcall Button27Click(TObject *Sender);
	void __fastcall Button28Click(TObject *Sender);
	void __fastcall Button31Click(TObject *Sender);
	void __fastcall IndicatorModeSelectClick(TObject *Sender);
	void __fastcall CheckBox4Click(TObject *Sender);
	void __fastcall CheckBox5Click(TObject *Sender);
	void __fastcall CheckBox6Click(TObject *Sender);
	void __fastcall ComboBox5Change(TObject *Sender);
	void __fastcall WS_ImgClick(TObject *Sender);
	void __fastcall Button33Click(TObject *Sender);
	void __fastcall CheckBox10Click(TObject *Sender);
	void __fastcall Button34Click(TObject *Sender);
	void __fastcall Panel23MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Panel24MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y); //WS2812灯珠颜色选择
	void __fastcall CustomMatrixIconClick(TObject *Sender);
	void __fastcall CUstomKbIconClick(TObject *Sender);
	void __fastcall Button42Click(TObject *Sender);
	void __fastcall Button43Click(TObject *Sender);
	void __fastcall Button41Click(TObject *Sender);
	void __fastcall ComboBox10Change(TObject *Sender);
	void __fastcall Button40Click(TObject *Sender);
	void __fastcall Button39Click(TObject *Sender);
	void __fastcall RadioButton24Click(TObject *Sender);
	void __fastcall TrackBar7Change(TObject *Sender);
	void __fastcall ComboBox8Change(TObject *Sender);
	void __fastcall Button37Click(TObject *Sender);
	void __fastcall ComboBox9Change(TObject *Sender);
	void __fastcall Button44Click(TObject *Sender);
	void __fastcall Button47Click(TObject *Sender);
	void __fastcall Button48Click(TObject *Sender);
	void __fastcall Button38Click(TObject *Sender);
	void __fastcall Button45Click(TObject *Sender);
	void __fastcall Button46Click(TObject *Sender);
	void __fastcall ComboBox11Change(TObject *Sender);
	void __fastcall Img0DblClick(TObject *Sender);
	void __fastcall FLIcon0MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ComboBox12Change(TObject *Sender);
	void __fastcall CheckBox11Click(TObject *Sender);
	void __fastcall CheckBox12Click(TObject *Sender);
	void __fastcall Button49Click(TObject *Sender);

private: //User declarations

	bool IsCloseApplication;

public: //User declarations


		struct
	{
		struct KEYSCAN_STRUCT_CONFIG KeyScanCfg;
		struct LED_STRUCT_PHY_CONFIG LedCfg;
	}

	PhyCfg;

	struct
	{
		struct CP_STRUCT_PACK_HW_FW_INFO DeviceInfo; //设备的信息
        //设备版本是否匹配
        union
		{
			struct
			{
				uint8_t         Hw:1;
				uint8_t         Bl:1;
				uint8_t         Fw:1;
				uint8_t         None:5;
			}Bit;
            uint8_t             IsFail; //这个值如果非零，表示有错误！
		}DeviceVerFlag;

		struct
		{
			TImage *Icon[8]; //常规层的标签
			int CurrSelectLabel; //当前选择的那个标签
			//编辑器界面的数据
			struct
			{
				struct CP_STRUCT_NORMAL_LAYER_EDIT_ONE_KEYS KeyLabel[KeyEdit_MAX_KEYS]; //最多128键
				struct LAYER_STRUCT_ALL_FLAYER LayerData; //功能层的所有数据

			} LayerEdit;
		} NormalLayer;
		//多媒体键码的数据
		struct
		{
			TStringList *Code;
			TStringList *Str;
		} MediaKeyCode;
		struct
		{
			TStringList *Code;
			TStringList *Str;
		} NormalKeyCode;

		//触发层
		struct
		{
			//FN键编辑器
			struct
			{
				TImage *icon[KeyEdit_MAX_KEYS]; //可选择的图标，最多产生128个
				uint8_t CurrItemIndex; //0xff=无效的，其他值表示一个项目
                int CurrItemRow; //当前编辑的是第几个项目
				int CurrItemCol; //当前编辑的是第几列
				int CurFuncRow;
				int CurFuncCol;
			} FnKeys;
			//组合键编辑器
			struct
			{
				int CurrItemRow; //当前编辑的是第几个项目
				int CurrItemCol; //当前编辑的是第几列
				int CurFuncRow;
				int CurFuncCol;
			} ComboKeys;
			//连击键编辑器
			struct
			{
				int cRow;
				int cCol;
				int cFRow;
				int cFCol;
			} nHitKeys;
			//二合一键编辑器
			struct
			{
				int cRow;
				int cCol;
				int CFRow;
				int CFCol;
			} two1Keys;
			//保持键
			struct
			{
				int cRow;
				int cCol;
				int cFRow;
				int cFCol;
			} HdKeys;

			struct LAYER_STRUCT_TLAYER_ONE_GROUP TLayerData[LAYER_DEF_TLAYER_MAX_NUM]; //8个层的数据
			int CurrEditFunction; //当前正在编辑触发功能0=fn，1=组合键，2连击键，3=2合1键，4=保持键
		} TLayerEdit;
		//层配置
		struct LAYER_STRUCT_CONFIG LayerCfg; //层的配置数据
		struct LAYER_STRUCT_MACRO_ONE_GROUP MacroData[LAYER_DEF_MACRO_MAX_GROUP]; //最多100组
		struct
		{
			int nRow;
		} MacroEditer;

		struct LAYER_STRUCT_KEY_LOCK_ONE_GROUP KeyLockData[LAYER_DEF_KEY_LOCK_MAX_GROUP]; //锁定键，最多64组
		struct
		{
			int nCol;
			int nRow;
		} KeyLockEditer; //锁定键编辑器

		//背光
		struct
		{
			struct EFF_STRUCT_EFF_CONFIG LedCfg; //背光的配置
			//WS2812的灯珠灯效编辑器
			struct
			{
				TImage *img[EFF_DEF_WS_SET_COLOR_MAX]; //颜色控件
				TRadioButton *rbtn[EFF_DEF_WS_EFF_MAX]; //单选按钮
			} WSLedRun;

			//自定义背光的编辑器
			struct
			{
				TImage *SrcImg; //原始的图片，用来取像素用，不显示
				TImage *Kbicon[LED_DEF_USER_MATRIX_X_MAX][LED_DEF_USER_MATRIX_Y_MAX]; //键盘矩阵的坐标
				TImage *MatrixIcon[LED_DEF_USER_MATRIX_X_MAX][LED_DEF_USER_MATRIX_Y_MAX]; //用户显示矩阵的坐标
				int UserX; //当前用户尺寸
				int UserY;
				int IconCount; //一共有多少个图标
				struct CUSTOM_BKL_ONE_ICON_INFO IconInfo[LED_DEF_USER_MATRIX_X_MAX][LED_DEF_USER_MATRIX_Y_MAX];
				struct CUSTOM_BKL_ONE_IMAGE Pics[CUSTOM_BKL_IMAGE_MAX]; //最多可定义800张图片
			} CustEff;
		} BklConfig;

		//BootLoader
		struct
		{
			struct CP_STRUCT_PACK_BL_HW_INFO BlInfo;
			bool IsSendFwData; //是否正在发送数据
			struct
			{
				HANDLE hFwFile; //固件文件句柄
				ULONG FwFileSize;
				struct _DEF_PACK_FILE_HEAD_STRUCT FwHeadInfo;
				uint8_t FwData[1024000];
			} FwFile;

		} Bl;
		struct
		{
			struct
			{
				struct KEYSCAN_STRUCT_CONFIG KeyScanCfg;
				struct LED_STRUCT_PHY_CONFIG LedCfg;
			} PhyCfgData;
		} PhyCfgUpLoad;
		//语言支持
		struct
		{
			TStringList *Idx;
			TStringList *Str;
		} Language;

	}

	CpRun;

	//硬件信息
	void __fastcall TMainForm::GetDeviceInfo(void *pData); //获取设备的基本信息
	void __fastcall TMainForm::GetKeyScanCfgFormDevice(void *pData); //从设备获取键盘扫描配置
	void __fastcall TMainForm::GetLedCfgFormDevice(void *pData); //从设备获取LED的扫描配置

	//加载多媒体键码的配置文件
	void __fastcall TMainForm::LoadMediaKeyCode(void); //加载多媒体键码的配置文件
	String __fastcall TMainForm::GetMediaKeyCodeForStr(uint16_t KeyCode);
	//获取一个多媒体键码的字符串

	void __fastcall TMainForm::LoadNormalKeyCode(void); //加载普通键码的配置文件
	String __fastcall TMainForm::GetNormalKeyCodeForStr(uint8_t KeyCode);
	//获取一个普通按键的键码字符串

	String __fastcall TMainForm::GetAllFunctionHintStr(union FUNC_UNION_COMMAND_LIST *Func, uint8_t Model); //获取所有功能的提示字符串

	//功能码获取
	String __fastcall TMainForm::GetFuncStr(union FUNC_UNION_COMMAND_LIST *Func, uint8_t Model); //返回功能对应的字符串
	String __fastcall TMainForm::GetKeyCodeStr(uint8_t Code); //返回键码对应的字符串
	String __fastcall TMainForm::GetMediaKeyCodeStr(uint16_t Code);
	//返回多媒体键码的字符串

	//常规层
	void __fastcall TMainForm::FLGetData(void *pData); //获取功能层数据
	void __fastcall TMainForm::InitNormalLayer(void); //初始化常规层的功能
	void __fastcall TMainForm::DispNormalLayer(uint8_t LayerID); //显示当前层的数据
	void __fastcall TMainForm::FlDispOneIconInfo(TImage *Sender, uint8_t LayerID); //常规显示单个ICON的内容

	//触发层
	void __fastcall TMainForm::TLGetData(void *pData); //获取触发层的数据

	//fn键编辑器
	void __fastcall TMainForm::FnDispFLayer(TObject *Sender); //显示FN键的参考层
	void __fastcall TMainForm::FnDispEditer(void); //显示EDIT的数据
	void __fastcall TMainForm::FnKeyDispAllData(int LayerID);//显示当前FN键的数据
	static void __fastcall FnKeyEditerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	void __fastcall TMainForm::FnKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);


	//组合键编辑器
	void __fastcall TMainForm::ComboKeyDispEditer(void); //显示组合键的编辑器
	void __fastcall TMainForm::ComboKeyDispAllData(int LayerID);//在编辑器内显示当前层的数据
	static void __fastcall ComboKeyEditerMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	void __fastcall TMainForm::ComboKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);

	//连击键的编辑器
	void __fastcall TMainForm::nHitKeyDispEditer(void); //创建连击键的编辑器
	void __fastcall TMainForm::nHitKeyDispAllData(int LayerID);
	static void __fastcall nHitKeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	void __fastcall TMainForm::nHitKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);

	//二合一键
	void __fastcall TMainForm::two1KeyDispEditer(void); //创建编辑器
	void __fastcall TMainForm::two1KeyDispAllData(int LayerID);
	static void __fastcall two1KeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	void __fastcall TMainForm::tow1KeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);

	//保持键
	void __fastcall TMainForm::HoldDownKeyDispEditer(void); //创建编辑器
	void __fastcall TMainForm::HoldDownKeyDispAllData(int LayerID);
	static void __fastcall HoldDownKeyEditMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	void __fastcall TMainForm::HoldDownKeyEditMouseDown_Class(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol,
		int nRow);

	//功能层管理
	void __fastcall TMainForm::FlayerSetPoint(void); //设置关联性
	void __fastcall TMainForm::FlayerDispData(void); //显示当前层的配置
	void __fastcall TMainForm::FLayerEnable(int LayerID); //开关某功能层，调用一次，开/关一次
	void __fastcall TMainForm::FLayerSetTopPriority(int LayerID); //把某层设置为最高优先级
	void __fastcall TMainForm::FLayerGetDataFromDevice(void *pData); //从设备获取数据
	void __fastcall TMainForm::FLayerSetIconSt(TImage *img, bool Seleced); //画出图标的样式

	//触发层管理
	void __fastcall TMainForm::TLayerDispData(void); //显示触发层的配置

	//宏编辑器
	void __fastcall TMainForm::MacroInitEditer(void); //初始化宏的编辑器
	void __fastcall TMainForm::MacroDispAllData(int gid); //显示选择的组的所有数据
	void __fastcall TMainForm::MacroGetDataFromDevice(void *pData); //获取设备发来的数据

	//锁定键编辑器
	void __fastcall TMainForm::KeyLockInitEditer(void); //初始化锁定键编辑器
	void __fastcall TMainForm::KeyLockGetKeys(union FUNC_UNION_COMMAND_LIST *Func, int nCol, int nRow); //设置功能并显示
	void __fastcall TMainForm::KeyLockGetDataFromDevice(void *pData); //获取设备发来的数据
	void __fastcall TMainForm::KeyLockDispAllData(void);

	//LED配置
	void __fastcall TMainForm::LedCfgGetDataFromDevice(void *pData); //获取设备发来的LED配置信息
	void __fastcall TMainForm::LedCfgDispAllData(void); //显示当前的配置
	void __fastcall TMainForm::LedCfgDispIndicator(void); //显示兼容LED的状态
	void __fastcall TMainForm::LedCfgDispIndicatorPoint(int Leds); //显示LED当前选择的坐标

	//为控件设置tip提醒窗口
	void __fastcall TMainForm::ShowBalloonTip(TWinControl *Control, int nIcon, LPSTR lpTitle, LPSTR lpText, TColor clBack, TColor clFore);

	//WS2812配置
	void __fastcall TMainForm::WsCfgDispGroupInfo(void); //显示某一组的配置
	void __fastcall TMainForm::wsCfgGetGroupInfo(void); //获得当前组的配置

	//状态指示灯动作
	void __fastcall TMainForm::StLedDispAllCfg(void); //显示所有的配置

	//自定义背光编辑器
	void __fastcall TMainForm::InitCustomEdit(void); //创建自定义背光编辑器
	void __fastcall TMainForm::CustomDrawMatrixIcon(TImage *img, TColor Color, bool SetToPic); //画一个显示矩阵的图标
	void __fastcall TMainForm::CustomDrawKbIcon(TImage *img, TColor Color, bool SetToPic); //画一个键盘矩阵的图标
	void __fastcall TMainForm::CustomGetPointFromTag(int tag, int *x, int *y); //根据tag，得到坐标
	void __fastcall TMainForm::CustomDispKbData(void); //显示键盘发来的图片数据
	void __fastcall TMainForm::CustomGetPicFromDevice(void *pData); //从设备获取一张图片
	void __fastcall TMainForm::CustomDispCfg(void); //显示配置信息

	//BootLoader功能
	void __fastcall TMainForm::GetBootLoaderInfo(void *pData); //获取BL的信息

	//皮肤
	void __fastcall TMainForm::ThemesInit(void); //初始化皮肤相关的数据

	//多语言
	void __fastcall TMainForm::LoadLanguageFiles(void); //加载配置的语言文件
	void __fastcall TMainForm::LanguageLoadLngRes(String FileName); //加载指定的语言文件到内存
	void __fastcall TMainForm::LanguageSetLng(void); //设置当前的语言到UI上
	String __fastcall TMainForm::LanguageGetStr(int id); //获取资源对应的字符串

	//USB断开后，是否关闭背光
    void __fastcall TMainForm::UsbDisConn_ShowCfg(void);//显示配置的结果


	//公用的通信函数
	void __fastcall TMainForm::SendCmd(byte Cmd, byte Para1, byte Para2);
	void __fastcall TMainForm::SendRawData(byte ReportID, void *Data);
	ULONG __fastcall TMainForm::SendDataByPack(byte ReportID, byte Cmd, uint16_t ID, void *Data, ULONG Size); //把数据分包发送

	//消息处理函数
	void __fastcall On_GetRecvData(Messages::TMessage &Message); //处理TCP信息的函数
	void __fastcall On_HidDisConnect(Messages::TMessage &Message); //HID连接被断开
	void __fastcall On_SendDataSuccess(Messages::TMessage &Message); //数据发送完成
	//拦截系统的消息
	BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(ON_GET_RECV_DATA, TMessage, On_GetRecvData); //收到HID设备发来的数据
	MESSAGE_HANDLER(ON_HID_DISCONNECT, TMessage, On_HidDisConnect); //hid设备断开连接
	MESSAGE_HANDLER(ON_SEND_DATA_SUCCESS, TMessage, On_SendDataSuccess); //数据发送成功消息
	END_MESSAGE_MAP(TForm)

		__fastcall TMainForm(TComponent *Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
