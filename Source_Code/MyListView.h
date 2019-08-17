// ---------------------------------------------------------------------------

#ifndef MyListViewH
#define MyListViewH
// ---------------------------------------------------------------------------
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
#include "Unit1.h"

#define LST_DEF_SCROLL_PX       35          //滚动的像素

// 滚动框容器
struct LST_STRUCT_SCROLL_BOX_INFO
{
	TScrollBox *SBox; // 滚动框

	Classes::TComponent* AOwner; // 父窗口句柄

	int Top;
	int Left;
	int Width;
	int Height;
};

// 表头的结构
struct LST_STRUCT_MY_LIST_VIEW_TABLE_HEAD
{
	int Height; // 高
	int FontSize; // 字体的大小
	TColor BkColor; // 背景颜色
	TColor FontColor; // 字体颜色
	TColor PenColor; // 画笔颜色
};

struct LST_STRUCT_MY_LIST_VIEW_TABLE_ONE_ROW
{
	TStringList *Caption; // 每一行的标题
	int Height; // 每一行的高度
};

struct LST_STRUCT_MY_LIST_VIEW_TABLE_ONE_ROW_STYLE
{
	int *ColFontStyle; // 每个列的字体风格
};

// 整个表的数据
struct LST_STRUCT_MY_LIST_VIEW_TABLE_ALL_ROW
{
	TStringList **RowCaption; // 每一行的数据，最多256行
	int FontSize; // 字体的大小

	// 正常显示的颜色
	struct
	{
		TColor BkColor; // 背景颜色
		TColor FontColor; // 字体颜色
		TColor PenColor; // 画笔颜色
	}

	NormalColor;

	// 选中后的颜色
	struct
	{
		TColor BkColor; // 背景颜色
		TColor FontColor; // 字体颜色
		TColor PenColor; // 画笔颜色
	}

	SelectColor;

	struct
	{
		int **RowStyle; // 字体的风格
		int LastSelectnCol; // 上次选择的列坐标
		int LastSelectnRow; // 上次选择的行坐标
		int LastSelectFontStyle; // 上次选择的行坐标风格
	}

	CellStyle;
};

struct LST_STRUCT_MY_LIST_VIEW_ALL_DATA
{
	int RowNum; // 有多少行
	int ColNum; // 有多少列
	int *ColWidth; // 每个列有多宽,动态的new出来，关闭的时候要delete
	int *RowHeight; // 每个行的高度
	TImage *Image; // 作图的画板
	TImage *VScroll; // 横向滚动条
	TImage *HScroll; // 纵向滚动条
	struct LST_STRUCT_SCROLL_BOX_INFO ScrollBox; // 滚动框的数据
	struct LST_STRUCT_MY_LIST_VIEW_TABLE_HEAD TableHead; // 表头
	struct LST_STRUCT_MY_LIST_VIEW_TABLE_ALL_ROW Table; // 表的数据

	struct
	{
		void __fastcall(*OnMouseDown)(TObject * Sender, TMouseButton Button, TShiftState Shift, int X, int Y, int nCol, int nRow);
	}TableEvent;
    bool IsFirstRun;

};

class MyListView
{

private:
	void __fastcall ImageOnMouseLeave(TObject *Sender); // 鼠标离开IMAGE的事件
	void __fastcall ImageOnMouseEnter(TObject *Sender); // 鼠标进入Image的事件
	void __fastcall ImageOnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SBoxMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, const TPoint &MousePos, bool &Handled); // 滚动框的鼠标滚动事件

public:
	MyListView::MyListView(Classes::TComponent* AOwner, TWinControl *Parent, TMainForm *Mf, int Left, int Top, int Width, int Height); // 构造函数
	MyListView::~MyListView(void); // 析构函数

	struct LST_STRUCT_MY_LIST_VIEW_ALL_DATA ListData; // 数据

	// 表头设置
	void __fastcall MyListView::SetColNum(int Num); // 设置有多少列
	void __fastcall MyListView::SetRowNum(int Num); // 设置有多少行

	void __fastcall MyListView::SetHeadHeight(int Height); // 设置表头有多高
	void __fastcall MyListView::SetHeadFontSize(int Size); // 表头的字体大小
	void __fastcall MyListView::SetHeadBkColor(TColor Color); // 设置表格内的背景颜色
	void __fastcall MyListView::SetHeadFontColor(TColor Color); // 设置表格内的字体颜色
	void __fastcall MyListView::SetHeadPanColor(TColor Color); // 设置表格内的画笔颜色

	void __fastcall MyListView::SetColWidth(int nCol, int Width);
	// 设置第几列的宽度,nCol = -1，表示设置所有COL的宽度相同
	void __fastcall MyListView::SetRowHeight(int nRow, int Height);
	// 设置第几行的高度,nRow = -1,表示所有的ROW的高度相同

	void __fastcall MyListView::SetTableFontSize(int Size); // 表格内的字体大小
	// 常规表格内的颜色
	void __fastcall MyListView::SetTableBkColor(TColor Color); // 设置表格内的背景颜色
	void __fastcall MyListView::SetTableFontColor(TColor Color); // 设置表格内的字体颜色
	void __fastcall MyListView::SetTablePanColor(TColor Color); // 设置表格内的画笔颜色

	// 选中的框的颜色
	void __fastcall MyListView::SetTableSelectBkColor(TColor Color);
	// 设置表格内选中的背景颜色
	void __fastcall MyListView::SetTableSelectFontColor(TColor Color);
	// 设置表格选中内的字体颜色
	void __fastcall MyListView::SetTableSelectPenColor(TColor Color);
	// 设置表格内选中的画笔颜色

	// 设置事件
	void __fastcall MyListView::SetOnMouseDown(void __fastcall(*OnMouseDown)(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y,
		int nCol, int nRow)); // 设置鼠标按下的事件处理句柄

	void __fastcall MyListView::Create(void); // 根据设置，创建出控件
	void __fastcall MyListView::PaintBox(int x, int y, int width, int height, TColor PenColor); // 画一个方框
	void __fastcall MyListView::PaintRectangle(int x, int y, int width, int height, TColor BkColor, TColor PenColor); // 画一个填充的方框
	void __fastcall MyListView::PaintDrawCell(int nCol, int nRow, int CellStyle, int StrStyle, String Str); // 画一个单元格
	void __fastcall MyListView::GetCellPoint(int *nCol, int *nRow, int x, int y); // 通过鼠标的坐标，返回表格的坐标
	String __fastcall MyListView::GetCellStr(int nCol, int nRow);
	// 获取某单元格的字符串数据

};

#endif
