// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
// ---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("NumSelect.cpp", nSelect);
USEFORM("TimeSelect.cpp", TSel);
USEFORM("Unit1.cpp", MainForm);
USEFORM("FuncSelect.cpp", FuncSel);
USEFORM("LedSelect.cpp", LedSel);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->Run();
	} catch (Exception &exception)
	{
		Application->ShowException(&exception);
	} catch (...)
	{
		try
		{
			throw Exception("");
		} catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
