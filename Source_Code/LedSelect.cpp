//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LedSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLedSel *LedSel;
TMainForm *sMform; //主窗体指针

//---------------------------------------------------------------------------
__fastcall TLedSel::TLedSel(TComponent *Owner, TMainForm *Mf, int *px, int *py, int *ux, int *uy) : TForm(Owner)
{
	sMform = Mf;
	LedSel = this;
	ParaData.px = px;
	ParaData.py = py;
	ParaData.ux = ux;
	ParaData.uy = uy;
	memset(ParaData.icon, 0, sizeof(ParaData.icon));
	DispLedInfo();
}

//---------------------------------------------------------------------------
void __fastcall TLedSel::DispLedInfo(void)//显示LED选择框
{
	int index = 0;
	int x, y;
	for(x = 0; x < sMform->PhyCfg.LedCfg.LEDMatrixCfg.LED_X; x++)
	{
		for(y = 0; y < sMform->PhyCfg.LedCfg.LEDMatrixCfg.LED_Y; y++)
		{
			if(sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Enable == 0)
				//如果被禁用，则不再创建
			{
				continue;
			}
			ParaData.icon[index] = new TImage(this);
			ParaData.icon[index]->Parent = Panel1; //(TWinControl*)Sender; // Panel10
			//在PANEL10上生成
			ParaData.icon[index]->Tag = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid; //按键ID
			ParaData.icon[index]->Top = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_y;
			ParaData.icon[index]->Left = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_x;
			ParaData.icon[index]->Width = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_w;
			ParaData.icon[index]->Height = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Button_h;
			ParaData.icondata[sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid].px = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Phy_x;
			ParaData.icondata[sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid].py = sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Phy_y;
			ParaData.icondata[sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid].ux = x;
			ParaData.icondata[sMform->PhyCfg.LedCfg.LEDMatrixCfg.MatrixMap[x][y].Keyid].uy = y;

			ParaData.icon[index]->OnClick = IconClicks; //单击事件

			sMform->FlDispOneIconInfo(ParaData.icon[index], 0);
			index++ ;
		}
	}

}

void __fastcall TLedSel::FormClose(TObject *Sender, TCloseAction &Action)
{
	//关闭窗口
	for(int i = 0; i < 128; i++)
	{
		if(ParaData.icon[i] == NULL)
		{
			continue;
		}
		delete ParaData.icon[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TLedSel::IconClicks(TObject *Sender)//图标被单击
{
	int index = dynamic_cast <TImage *> (Sender)->Tag;

	*ParaData.px = ParaData.icondata[index].px;
	*ParaData.py = ParaData.icondata[index].py;
	*ParaData.ux = ParaData.icondata[index].ux;
	*ParaData.uy = ParaData.icondata[index].uy;
	Close();
}
