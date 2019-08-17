// ---------------------------------------------------------------------------

#ifndef NumSelectH
#define NumSelectH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#define SEL_DEF_BUTTON_W	32
#define SEL_DEF_BUTTON_H	32

// ---------------------------------------------------------------------------
class TnSelect : public TForm
{
__published: // IDE-managed Components
	void __fastcall ButtonClick(TObject *Sender);

private: // User declarations
public: // User declarations

		struct
	{
		int Col;
		int Row;
		int Min;
		int Max;
		int *SelectNum;
	}

	selrun;

	void __fastcall TnSelect::CreateButton(void);

	__fastcall TnSelect(TComponent* Owner, int Col, int Row, int Min, int Max, int *SelectNum);
};

// ---------------------------------------------------------------------------
extern PACKAGE TnSelect *nSelect;
// ---------------------------------------------------------------------------
#endif
