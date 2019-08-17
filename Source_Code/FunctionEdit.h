//---------------------------------------------------------------------------

#ifndef FunctionEditH
#define FunctionEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "Unit1.h"
//---------------------------------------------------------------------------
class TFuncEdit : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TTabSheet *TabSheet3;
    TTabSheet *TabSheet4;
    TTabSheet *TabSheet5;
    TListView *LV_NormalKeyCodeSelect;
    TListView *LV_MediaKeyCodeSelect;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TComboBox *ComboBox_LayerEnable_LayerSelect;
    TLabel *Label2;
    TRadioButton *RadioButton_LayerClose;
    TRadioButton *RadioButton_LayerOpen;
    TButton *Button_LayerEnable_Add;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TComboBox *ComboBox_LayerTopPrio_LayerSelect;
    TButton *Button_LayerSetTopPrio_Add;
    TGroupBox *GroupBox3;
    TLabel *Label5;
    TComboBox *ComboBox_LayerPrioMove_LayerSelect;
    TLabel *Label6;
    TRadioButton *RadioButton_TopPrio;
    TRadioButton *RadioButton_DownPrio;
    TButton *Button_LayerPrioMove_Add;
    TGroupBox *GroupBox4;
    TRadioButton *RadioButton1;
    TRadioButton *RadioButton2;
    TButton *Button1;
    TGroupBox *GroupBox5;
    TRadioButton *RadioButton3;
    TRadioButton *RadioButton4;
    TButton *Button2;
    TGroupBox *GroupBox6;
    TRadioButton *RadioButton5;
    TRadioButton *RadioButton6;
    TButton *Button3;
    TGroupBox *GroupBox7;
    TRadioButton *RadioButton7;
    TRadioButton *RadioButton8;
    TButton *Button4;
    TGroupBox *GroupBox8;
    TRadioButton *RadioButton9;
    TRadioButton *RadioButton10;
    TButton *Button5;
    TGroupBox *GroupBox9;
    TLabel *Label4;
    TComboBox *ComboBox_MacroLoad_Select;
    TButton *Button6;
    TButton *Button7;
    TGroupBox *GroupBox10;
    TLabel *Label7;
    TComboBox *ComboBox_KeyLock_Select;
    TButton *Button8;
    TLabel *Label8;
    TComboBox *ComboBox_MacroLoad_Select_Step;
    TTabSheet *TabSheet6;
    TGroupBox *GroupBox11;
    TLabel *Label9;
    TComboBox *ComboBox_MacroJmp;
    TButton *Button_MacroJmpAdd;
    TGroupBox *GroupBox12;
    TLabel *Label10;
    TComboBox *ComboBox_MacroGroupJmp_GroupSelect;
    TButton *Button_MacroGroupJmp_ADD;
    TLabel *Label11;
    TComboBox *ComboBox_MacroGroupJmp_StepSelect;
    TGroupBox *GroupBox13;
    TRadioButton *RadioButton11;
    TRadioButton *RadioButton12;
    TRadioButton *RadioButton13;
    TRadioButton *RadioButton14;
    TRadioButton *RadioButton15;
    TRadioButton *RadioButton16;
    TRadioButton *RadioButton17;
    TRadioButton *RadioButton_Macro_InputTime;
    TLabel *Label12;
    TEdit *Edit1;
    TButton *Button_Macro_Delay_Add;
    TRadioButton *RadioButton_Macro_Normal_Down;
    TRadioButton *RadioButton_Macro_Normal_Up;
    TRadioButton *RadioButton_Macro_Media_Down;
    TRadioButton *RadioButton_Macro_Media_Up;
    TRadioButton *RadioButton_Macro_NormalKey;
    TRadioButton *RadioButton_Macro_Media_NormalOut;
    TTabSheet *TabSheet7;
    TGroupBox *GroupBox14;
    TButton *Button_EffectIndexUp;
    TButton *Button_EffectIndexDown;
    TButton *Button_EffectSpeedUp;
    TButton *Button_EffectSpeedDown;
    void __fastcall LV_NormalKeyCodeSelectDblClick(TObject *Sender);
    void __fastcall LV_MediaKeyCodeSelectDblClick(TObject *Sender);
    void __fastcall Button_LayerEnable_AddClick(TObject *Sender);
    void __fastcall Button_LayerSetTopPrio_AddClick(TObject *Sender);
    void __fastcall Button_LayerPrioMove_AddClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall Button7Click(TObject *Sender);
    void __fastcall Button8Click(TObject *Sender);
    void __fastcall Button_MacroJmpAddClick(TObject *Sender);
    void __fastcall Button_MacroGroupJmp_ADDClick(TObject *Sender);
    void __fastcall Edit1KeyPress(TObject *Sender, wchar_t &Key);
    void __fastcall Button_Macro_Delay_AddClick(TObject *Sender);
    void __fastcall Button_EffectIndexUpClick(TObject *Sender);
    void __fastcall Button_EffectIndexDownClick(TObject *Sender);
    void __fastcall Button_EffectSpeedUpClick(TObject *Sender);
    void __fastcall Button_EffectSpeedDownClick(TObject *Sender);
private:	// User declarations

    struct
    {
        int EditPage;		//当前所在的页
        int LayerID;		//当前编辑的层
        int EditFunc;		//当前编辑的功能
        int FuncIndex;		//当前编辑的第几个项的功能
        int EditEvent;		//如果有多个子功能，这个参数指定第几个功能
        union _KEYSCAN_ONE_FUNC_MAP_UNION *KeyMap;//当前功能的映射表
        WORD  *KeyLockCode;	//锁定键的键码
    } EditPara;






public:		// User declarations

    void	__fastcall TFuncEdit::GetFuncMap(void);//获取当前要编辑的功能映射
    void	__fastcall TFuncEdit::DisableFuncs(void);//取消一些功能

    void	__fastcall TFuncEdit::InitUiLanguage(void);//初始化界面的语言



	//在哪个页，    什么功能，第几个功能，如果有多个功能，这个参数指定是第几个功能 TComponent
    __fastcall TFuncEdit(TComponent* Owner, TMainForm *MFm, int EditPage, int LayerID, int EditFunc, int FuncIndex, int EditEvent);
};
//---------------------------------------------------------------------------
extern PACKAGE TFuncEdit *FuncEdit;
//---------------------------------------------------------------------------
#endif
