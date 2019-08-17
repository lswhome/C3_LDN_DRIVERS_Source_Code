//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NumSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TnSelect *nSelect;

//---------------------------------------------------------------------------
__fastcall TnSelect::TnSelect(TComponent *Owner, int Col, int Row, int Min, int Max, int *SelectNum) : TForm(Owner)
{
	nSelect = this;
	nSelect->Left = Mouse->CursorPos.x;
	nSelect->Top = Mouse->CursorPos.y;
	nSelect->Width = Col * SEL_DEF_BUTTON_W + 48;
	nSelect->Height = Row * SEL_DEF_BUTTON_H + 70;
	selrun.Col = Col;
	selrun.Row = Row;
	selrun.Max = Max;
	selrun.Min = Min;
	selrun.SelectNum = SelectNum;
	CreateButton();
}

//---------------------------------------------------------------------------
void __fastcall TnSelect::CreateButton(void)
{

	TButton *btn;
	int x, y;
	int idx = selrun.Min;
	for(y = 0; y < selrun.Row; y++)
	{
		for(x = 0; x < selrun.Col; x++)
		{
			btn = new TButton(this);
			btn->Left = SEL_DEF_BUTTON_W * x + 22;
			btn->Top = SEL_DEF_BUTTON_H * y + 22;
			btn->Width = SEL_DEF_BUTTON_W;
			btn->Height = SEL_DEF_BUTTON_H;
			btn->Parent = nSelect;
			btn->Caption = idx;
			btn->Tag = idx;
			btn->OnClick = ButtonClick;
			if(idx == selrun.Max)
			{
				return;
			}
			idx++ ;
		}
	}
}

void __fastcall TnSelect::ButtonClick(TObject *Sender)
{
	*selrun.SelectNum = dynamic_cast <TButton *> (Sender)->Tag;
	Close();
}
//---------------------------------------------------------------------------
