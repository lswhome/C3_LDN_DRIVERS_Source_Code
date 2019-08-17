// ---------------------------------------------------------------------------

#ifndef FuncSelectH
#define FuncSelectH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Unit1.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ButtonGroup.hpp>
// ---------------------------------------------------------------------------
#define CP_DEF_NORMAL_KB_EX_CODE_MAX											109	//扩展代码的数量

const struct CP_DEF_STRUCT_NORMAL_KB_EX_CODE
{
	uint8_t Code[CP_DEF_NORMAL_KB_EX_CODE_MAX];
} KB_EX_CODE =
{ {102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
			131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
			158, 159, 160, 161, 162, 163, 164, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
			196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, }};

class TFuncSel : public TForm
{
__published: // IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TButton *Button1;
	TButton *Button3;
	TButton *Button6;
	TButton *Button7;
	TButton *Button8;
	TButton *Button2;
	TButton *Button4;
	TButton *Button5;
	TButton *Button9;
	TButton *Button14;
	TButton *Button15;
	TButton *Button16;
	TButton *Button17;
	TButton *Button18;
	TButton *Button19;
	TButton *Button20;
	TButton *Button21;
	TButton *Button24;
	TButton *Button25;
	TButton *Button22;
	TButton *Button23;
	TButton *Button26;
	TButton *Button27;
	TButton *Button28;
	TButton *Button29;
	TButton *Button30;
	TButton *Button31;
	TButton *Button32;
	TButton *Button33;
	TButton *Button34;
	TButton *Button35;
	TButton *Button36;
	TButton *Button37;
	TButton *Button38;
	TButton *Button39;
	TButton *Button40;
	TButton *Button41;
	TButton *Button42;
	TButton *Button43;
	TButton *Button46;
	TButton *Button47;
	TButton *Button44;
	TButton *Button45;
	TButton *Button48;
	TButton *Button49;
	TButton *Button50;
	TButton *Button51;
	TButton *Button52;
	TButton *Button53;
	TButton *Button54;
	TButton *Button55;
	TButton *Button56;
	TButton *Button57;
	TButton *Button61;
	TButton *Button58;
	TButton *Button59;
	TButton *Button60;
	TButton *Button62;
	TButton *Button63;
	TButton *Button64;
	TButton *Button65;
	TButton *Button66;
	TButton *Button67;
	TButton *Button68;
	TButton *Button69;
	TButton *Button70;
	TButton *Button71;
	TButton *Button72;
	TButton *Button73;
	TButton *Button74;
	TButton *Button75;
	TButton *Button76;
	TButton *Button77;
	TButton *Button78;
	TButton *Button79;
	TButton *Button80;
	TButton *Button81;
	TButton *Button82;
	TButton *Button83;
	TButton *Button84;
	TButton *Button85;
	TButton *Button86;
	TButton *Button87;
	TButton *Button88;
	TButton *Button89;
	TButton *Button90;
	TButton *Button91;
	TButton *Button92;
	TButton *Button93;
	TButton *Button94;
	TButton *Button95;
	TButton *Button96;
	TButton *Button97;
	TButton *Button98;
	TButton *Button99;
	TButton *Button100;
	TButton *Button101;
	TButton *Button102;
	TButton *Button103;
	TButton *Button105;
	TButton *Button106;
	TButton *Button107;
	TButton *Button108;
	TButton *Button109;
	TButton *Button111;
	TButton *Button113;
	TButtonGroup *ButtonGroup1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TRadioButton *RadioButton3;
	TButton *Button10;
	TPanel *Panel1;
	TButton *Button11;
	TButton *Button112;
	TButton *Button124;
	TButton *Button12;
	TButton *Button114;
	TButton *Button126;
	TButton *Button125;
	TButton *Button13;
	TButton *Button104;
	TButton *Button110;
	TButton *Button116;
	TButton *Button115;
	TButton *Button123;
	TButton *Button117;
	TButton *Button118;
	TButton *Button119;
	TButton *Button120;
	TButton *Button122;
	TButton *Button121;
	TButtonGroup *ButtonGroup2;
	TRadioButton *RadioButton4;
	TRadioButton *RadioButton5;
	TRadioButton *RadioButton6;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *ComboBox2;
	TComboBox *ComboBox1;
	TButton *Button127;
	TButton *Button129;
	TButton *Button128;
	TGroupBox *GroupBox2;
	TComboBox *ComboBox3;
	TButton *Button130;
	TLabel *Label3;
	TGroupBox *GroupBox3;
	TLabel *Label4;
	TButton *Button131;
	TButton *Button132;
	TLabel *Label5;
	TComboBox *ComboBox4;
	TButton *Button133;
	TButton *Button134;
	TGroupBox *GroupBox4;
	TLabel *Label6;
	TLabel *Label7;
	TButton *Button135;
	TButton *Button136;
	TComboBox *ComboBox5;
	TButton *Button143;
	TButton *Button144;
	TGroupBox *GroupBox5;
	TLabel *Label8;
	TButton *Button137;
	TButton *Button138;
	TLabel *Label9;
	TComboBox *ComboBox6;
	TButton *Button145;
	TButton *Button146;
	TGroupBox *GroupBox6;
	TLabel *Label10;
	TButton *Button139;
	TButton *Button140;
	TLabel *Label11;
	TComboBox *ComboBox7;
	TButton *Button147;
	TButton *Button148;
	TGroupBox *GroupBox7;
	TLabel *Label12;
	TButton *Button141;
	TButton *Button142;
	TLabel *Label13;
	TComboBox *ComboBox8;
	TButton *Button149;
	TButton *Button150;
	TGroupBox *GroupBox8;
	TLabel *Label15;
	TComboBox *ComboBox10;
	TButton *Button153;
	TLabel *Label16;
	TButton *Button154;
	TGroupBox *GroupBox9;
	TLabel *Label17;
	TComboBox *ComboBox11;
	TButton *Button155;
	TGroupBox *GroupBox10;
	TLabel *Label14;
	TComboBox *ComboBox9;
	TButton *Button151;
	TButton *Button152;
	TComboBox *ComboBox12;
	TGroupBox *GroupBox11;
	TLabel *Label18;
	TComboBox *ComboBox13;
	TButton *Button156;
	TGroupBox *GroupBox12;
	TLabel *Label19;
	TLabel *Label20;
	TComboBox *ComboBox15;
	TButton *Button157;
	TComboBox *ComboBox14;
	TGroupBox *GroupBox13;
	TButton *Button158;
	TButton *Button159;
	TButton *Button160;
	TButton *Button161;
	TButton *Button162;
	TButton *Button163;
	TButton *Button164;
	TButton *Button165;
	TButton *Button166;
	TButton *Button167;
	TLabel *Label21;
	TEdit *Edit1;
	TButton *Button168;
	TLabel *Label22;
	TGroupBox *GroupBox14;
	TButton *Button169;
	TButton *Button170;
	TLabel *Label23;
	TComboBox *ComboBox16;
	TButton *Button171;
	TButton *Button172;
	TButton *Button173;
	TLabel *Label24;
	TComboBox *ComboBox17;
	TButton *Button174;
	TButton *Button175;
	TButton *Button176;
	TGroupBox *GroupBox15;
	TLabel *Label25;
	TComboBox *ComboBox18;
	TButton *Button178;
	TLabel *Label26;
	TComboBox *ComboBox19;
	TButton *Button179;
	TLabel *Label27;
	TComboBox *ComboBox20;
	TButton *Button180;
	TLabel *Label28;
	TComboBox *ComboBox21;
	TComboBox *ComboBox22;
	TButton *Button181;
	TGroupBox *GroupBox16;
	TButton *Button182;
	TButton *Button183;
	TButton *Button184;
	TButton *Button185;
	TButton *Button186;
	TButton *Button187;
	TButton *Button188;
	TGroupBox *GroupBox17;
	TButton *Button189;
	TButton *Button190;
	TButton *Button191;
	TButton *Button192;
	TButton *Button193;
	TButton *Button194;
	TButton *Button177;

	void __fastcall KEYSelectClick(TObject *Sender); // 常规按键选择
	void __fastcall MediaSelectClick(TObject *Sender); // 多媒体按键选择

	void __fastcall ButtonGroup1ButtonClicked(TObject *Sender, int Index);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall ButtonGroup2ButtonClicked(TObject *Sender, int Index);
	void __fastcall Button127Click(TObject *Sender);
	void __fastcall Button128Click(TObject *Sender);
	void __fastcall Button129Click(TObject *Sender);
	void __fastcall Button130Click(TObject *Sender);
	void __fastcall Button131Click(TObject *Sender);
	void __fastcall Button132Click(TObject *Sender);
	void __fastcall Button133Click(TObject *Sender);
	void __fastcall Button134Click(TObject *Sender);
	void __fastcall Button135Click(TObject *Sender);
	void __fastcall Button136Click(TObject *Sender);
	void __fastcall Button143Click(TObject *Sender);
	void __fastcall Button144Click(TObject *Sender);
	void __fastcall Button137Click(TObject *Sender);
	void __fastcall Button138Click(TObject *Sender);
	void __fastcall Button145Click(TObject *Sender);
	void __fastcall Button146Click(TObject *Sender);
	void __fastcall Button139Click(TObject *Sender);
	void __fastcall Button140Click(TObject *Sender);
	void __fastcall Button147Click(TObject *Sender);
	void __fastcall Button148Click(TObject *Sender);
	void __fastcall Button141Click(TObject *Sender);
	void __fastcall Button142Click(TObject *Sender);
	void __fastcall Button149Click(TObject *Sender);
	void __fastcall Button150Click(TObject *Sender);
	void __fastcall ButtonGroup2MouseEnter(TObject *Sender);
	void __fastcall Button153Click(TObject *Sender);
	void __fastcall Button154Click(TObject *Sender);
	void __fastcall Button155Click(TObject *Sender);
	void __fastcall Button156Click(TObject *Sender);
	void __fastcall Button157Click(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Button168Click(TObject *Sender);
	void __fastcall Button158Click(TObject *Sender);
	void __fastcall Button169Click(TObject *Sender);
	void __fastcall Button170Click(TObject *Sender);
	void __fastcall Button172Click(TObject *Sender);
	void __fastcall Button173Click(TObject *Sender);
	void __fastcall Button175Click(TObject *Sender);
	void __fastcall Button176Click(TObject *Sender);
	void __fastcall Button188Click(TObject *Sender);
	void __fastcall Button174Click(TObject *Sender);
	void __fastcall Button171Click(TObject *Sender);
	void __fastcall Button177Click(TObject *Sender);
	void __fastcall Button178Click(TObject *Sender);
	void __fastcall Button179Click(TObject *Sender);
	void __fastcall Button180Click(TObject *Sender);
	void __fastcall Button181Click(TObject *Sender);
	void __fastcall Button182Click(TObject *Sender);
	void __fastcall Button183Click(TObject *Sender);
	void __fastcall Button184Click(TObject *Sender);
	void __fastcall Button185Click(TObject *Sender);
	void __fastcall Button186Click(TObject *Sender);
	void __fastcall Button187Click(TObject *Sender);
	void __fastcall Button189Click(TObject *Sender);
	void __fastcall Button190Click(TObject *Sender);
	void __fastcall Button191Click(TObject *Sender);
	void __fastcall Button192Click(TObject *Sender);
	void __fastcall Button193Click(TObject *Sender);
	void __fastcall Button194Click(TObject *Sender);

private: // User declarations
public: // User declarations union FUNC_UNION_COMMAND_LIST *Func,int FuncID

	union FUNC_UNION_COMMAND_LIST *SetFunc; // 需要设置的功能结构体
	int IsSetFuncID; // 当前设置什么功能

	uint8_t __fastcall TFuncSel::GetKbExCode(uint8_t idx); // 返回扩展键码
	void __fastcall TFuncSel::SetFuncForNormalKeyCode(uint8_t Code);
	// 设置普通键码为功能码
	void __fastcall TFuncSel::MediaKeyAddItems(void); // 添加多媒体键码页面的项目
    void __fastcall TFuncSel::LanguageSetStr(void);//设置语言

	__fastcall TFuncSel(TComponent* Owner, TMainForm *Mf, union FUNC_UNION_COMMAND_LIST *Func, int FuncID);
};

// ---------------------------------------------------------------------------
extern PACKAGE TFuncSel *FuncSel;
// ---------------------------------------------------------------------------
#endif
