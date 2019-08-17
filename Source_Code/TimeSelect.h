//---------------------------------------------------------------------------

#ifndef TimeSelectH
#define TimeSelectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TTSel : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations

    struct
    {
    	int *InputNum;
    	int min;
        int max;
        bool isSuccess;
    }tselrun;


	__fastcall TTSel(TComponent* Owner,int min,int max,int *num);
};
//---------------------------------------------------------------------------
extern PACKAGE TTSel *TSel;
//---------------------------------------------------------------------------
#endif
