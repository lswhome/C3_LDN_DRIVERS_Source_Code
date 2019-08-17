//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TimeSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTSel *TSel;

//---------------------------------------------------------------------------
__fastcall TTSel::TTSel(TComponent *Owner, int min, int max, int *num) : TForm(Owner)
{
	TSel = this;
	memset(&tselrun, 0, sizeof(tselrun));

	TSel->Left = Mouse->CursorPos.x;
	TSel->Top = Mouse->CursorPos.y;

	tselrun.InputNum = num;
	tselrun.min = min;
	tselrun.max = max;
	Label2->Caption = "Limit:" + String(min) + " <= Value <=  " + String(max);
	Edit1->Text = min;

}

//---------------------------------------------------------------------------
void __fastcall TTSel::Edit1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if(!(((Key >= '0') && (Key <= '9')) || (Key == VK_BACK) || (Key == VK_DELETE)))
	{
		Key = 0;
	}
	else
	{
		if(!(Edit1->Text.Length() < 6 || Key == VK_BACK) || (Key == VK_DELETE))
		{
			Key = 0;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TTSel::Edit1Change(TObject *Sender)
{
	int tmp = Edit1->Text.ToIntDef(tselrun.min);

	if(tmp >= tselrun.min && tmp <= tselrun.max)
	{
		Label2->Font->Color = clGreen;
		Edit1->Font->Color = clGreen;
		tselrun.isSuccess = 1;
	}
	else
	{
		Label2->Font->Color = clRed;
		Edit1->Font->Color = clRed;
		tselrun.isSuccess = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TTSel::Button1Click(TObject *Sender)
{
	//х╥хо
	if(tselrun.isSuccess)
	{
		*tselrun.InputNum = Edit1->Text.ToIntDef(tselrun.min);
		TSel->Close();
	}
}
//---------------------------------------------------------------------------
