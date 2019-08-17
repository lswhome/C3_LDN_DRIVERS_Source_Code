//---------------------------------------------------------------------------

#ifndef LedSelectH
#define LedSelectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Unit1.h"

struct SEL_STRUCT_ICON_DATA
{
    int px;
    int py;
    int ux;
    int uy;
};


//---------------------------------------------------------------------------
class TLedSel : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

    void __fastcall IconClicks(TObject *Sender);//图标被单击

private:	// User declarations
public:		// User declarations

//用户传输的地址
struct
{
    int *px;//硬件地址
    int *py;
    int *ux;//用户地址
    int *uy;

    TImage *icon[128];
    struct SEL_STRUCT_ICON_DATA icondata[128];
}ParaData;






void __fastcall TLedSel::DispLedInfo(void);//显示LED选择框



	__fastcall TLedSel(TComponent* Owner,TMainForm *Mf,int *px,int *py,int *ux,int *uy);
};
//---------------------------------------------------------------------------
extern PACKAGE TLedSel *LedSel;
//---------------------------------------------------------------------------
#endif
