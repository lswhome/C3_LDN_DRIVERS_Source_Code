object FuncEdit: TFuncEdit
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  ClientHeight = 396
  ClientWidth = 685
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 8
    Top = 8
    Width = 670
    Height = 381
    ActivePage = TabSheet1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #26222#36890#38190#30721
      object LV_NormalKeyCodeSelect: TListView
        Left = 0
        Top = 32
        Width = 662
        Height = 321
        Align = alBottom
        BorderStyle = bsNone
        Columns = <
          item
            Caption = #25353#38190#32534#30721
            Width = 100
          end
          item
            Caption = #23545#24212#30340#21151#33021
            Width = 400
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        FlatScrollBars = True
        GridLines = True
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnDblClick = LV_NormalKeyCodeSelectDblClick
      end
      object RadioButton_Macro_Normal_Down: TRadioButton
        Left = 224
        Top = 9
        Width = 97
        Height = 17
        Caption = #25353#19979#36825#20010#38190
        TabOrder = 1
      end
      object RadioButton_Macro_Normal_Up: TRadioButton
        Left = 352
        Top = 9
        Width = 113
        Height = 17
        Caption = #25260#36215#36825#20010#38190
        TabOrder = 2
      end
      object RadioButton_Macro_NormalKey: TRadioButton
        Left = 80
        Top = 9
        Width = 113
        Height = 17
        Caption = #27491#24120#36755#20986#38190#30721
        Checked = True
        TabOrder = 3
        TabStop = True
      end
    end
    object TabSheet2: TTabSheet
      Caption = #22810#23186#20307#38190#30721
      ImageIndex = 1
      object LV_MediaKeyCodeSelect: TListView
        Left = 0
        Top = 32
        Width = 662
        Height = 321
        Align = alBottom
        BorderStyle = bsNone
        Columns = <
          item
            Caption = #22810#23186#20307#38190#30721
            Width = 100
          end
          item
            Caption = #23545#24212#30340#21151#33021
            Width = 400
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        FlatScrollBars = True
        GridLines = True
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnDblClick = LV_MediaKeyCodeSelectDblClick
      end
      object RadioButton_Macro_Media_Down: TRadioButton
        Left = 224
        Top = 9
        Width = 97
        Height = 17
        Caption = #25353#19979#36825#20010#38190
        TabOrder = 1
      end
      object RadioButton_Macro_Media_Up: TRadioButton
        Left = 352
        Top = 9
        Width = 113
        Height = 17
        Caption = #25260#36215#36825#20010#38190
        TabOrder = 2
      end
      object RadioButton_Macro_Media_NormalOut: TRadioButton
        Left = 80
        Top = 9
        Width = 113
        Height = 17
        Caption = #27491#24120#36755#20986#38190#30721
        Checked = True
        TabOrder = 3
        TabStop = True
      end
    end
    object TabSheet3: TTabSheet
      Caption = #35302#21457#23618#25511#21046
      ImageIndex = 2
      object GroupBox1: TGroupBox
        Left = 16
        Top = 10
        Width = 617
        Height = 89
        Caption = #23618#24320#20851
        TabOrder = 0
        object Label1: TLabel
          Left = 16
          Top = 40
          Width = 96
          Height = 13
          Alignment = taRightJustify
          Caption = #36873#25321#35201#25805#20316#30340#23618#65306
        end
        object Label2: TLabel
          Left = 232
          Top = 40
          Width = 72
          Height = 13
          Caption = #25191#34892#30340#25805#20316#65306
        end
        object ComboBox_LayerEnable_LayerSelect: TComboBox
          Left = 118
          Top = 37
          Width = 99
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          TabOrder = 0
        end
        object RadioButton_LayerClose: TRadioButton
          Left = 310
          Top = 39
          Width = 59
          Height = 17
          Caption = #20851#38381#23618
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object RadioButton_LayerOpen: TRadioButton
          Left = 375
          Top = 39
          Width = 58
          Height = 17
          Caption = #25171#24320#23618
          TabOrder = 2
        end
        object Button_LayerEnable_Add: TButton
          Left = 519
          Top = 35
          Width = 82
          Height = 25
          Caption = #30830#23450
          TabOrder = 3
          OnClick = Button_LayerEnable_AddClick
        end
      end
      object GroupBox2: TGroupBox
        Left = 16
        Top = 120
        Width = 617
        Height = 89
        Caption = #35302#21457#23618#35774#32622#20026#26368#39640#20248#20808#32423
        TabOrder = 1
        object Label3: TLabel
          Left = 16
          Top = 40
          Width = 192
          Height = 13
          Alignment = taRightJustify
          Caption = #25226#36873#23450#30340#35302#21457#23618#35774#32622#20026#26368#39640#20248#20808#32423#65306
        end
        object ComboBox_LayerTopPrio_LayerSelect: TComboBox
          Left = 214
          Top = 37
          Width = 99
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          TabOrder = 0
        end
        object Button_LayerSetTopPrio_Add: TButton
          Left = 519
          Top = 35
          Width = 82
          Height = 25
          Caption = #30830#23450
          TabOrder = 1
          OnClick = Button_LayerSetTopPrio_AddClick
        end
      end
      object GroupBox3: TGroupBox
        Left = 16
        Top = 232
        Width = 617
        Height = 89
        Caption = #31227#21160#35302#21457#23618#30340#20248#20808#32423
        TabOrder = 2
        object Label5: TLabel
          Left = 16
          Top = 43
          Width = 96
          Height = 13
          Alignment = taRightJustify
          Caption = #36873#25321#35201#25805#20316#30340#23618#65306
        end
        object Label6: TLabel
          Left = 232
          Top = 43
          Width = 72
          Height = 13
          Caption = #25191#34892#30340#25805#20316#65306
        end
        object ComboBox_LayerPrioMove_LayerSelect: TComboBox
          Left = 120
          Top = 39
          Width = 99
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          TabOrder = 0
        end
        object RadioButton_TopPrio: TRadioButton
          Left = 310
          Top = 41
          Width = 83
          Height = 17
          Caption = #25552#39640#20248#20808#32423
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object RadioButton_DownPrio: TRadioButton
          Left = 399
          Top = 41
          Width = 82
          Height = 17
          Caption = #38477#20302#20248#20808#32423
          TabOrder = 2
        end
        object Button_LayerPrioMove_Add: TButton
          Left = 519
          Top = 35
          Width = 82
          Height = 25
          Caption = #30830#23450
          TabOrder = 3
          OnClick = Button_LayerPrioMove_AddClick
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = #35302#21457#24320#20851
      ImageIndex = 3
      object GroupBox4: TGroupBox
        Left = 16
        Top = 16
        Width = 625
        Height = 57
        Caption = 'FN'#38190#24320#20851
        TabOrder = 0
        object RadioButton1: TRadioButton
          Left = 136
          Top = 24
          Width = 113
          Height = 17
          Caption = #20572#29992'FN'#38190
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton2: TRadioButton
          Left = 280
          Top = 24
          Width = 113
          Height = 17
          Caption = #21551#29992'FN'#38190
          TabOrder = 1
        end
        object Button1: TButton
          Left = 536
          Top = 20
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 2
          OnClick = Button1Click
        end
      end
      object GroupBox5: TGroupBox
        Left = 16
        Top = 79
        Width = 625
        Height = 57
        Caption = #24120#35268#35302#21457#24320#20851#65288#20851#38381#21518#20250#23548#33268#24120#35268#25353#38190#26080#27861#35302#21457#65289
        TabOrder = 1
        object RadioButton3: TRadioButton
          Left = 136
          Top = 24
          Width = 113
          Height = 17
          Caption = #20572#27490#35302#21457
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton4: TRadioButton
          Left = 280
          Top = 24
          Width = 113
          Height = 17
          Caption = #24320#21551#35302#21457
          TabOrder = 1
        end
        object Button2: TButton
          Left = 536
          Top = 20
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 2
          OnClick = Button2Click
        end
      end
      object GroupBox6: TGroupBox
        Left = 16
        Top = 142
        Width = 625
        Height = 57
        Caption = #32452#21512#38190#35302#21457#24320#20851
        TabOrder = 2
        object RadioButton5: TRadioButton
          Left = 136
          Top = 24
          Width = 113
          Height = 17
          Caption = #20572#27490#35302#21457
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton6: TRadioButton
          Left = 280
          Top = 24
          Width = 113
          Height = 17
          Caption = #24320#21551#35302#21457
          TabOrder = 1
        end
        object Button3: TButton
          Left = 536
          Top = 20
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 2
          OnClick = Button3Click
        end
      end
      object GroupBox7: TGroupBox
        Left = 16
        Top = 205
        Width = 625
        Height = 57
        Caption = #36830#20987#38190#35302#21457#24320#20851
        TabOrder = 3
        object RadioButton7: TRadioButton
          Left = 136
          Top = 24
          Width = 113
          Height = 17
          Caption = #20572#27490#35302#21457
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton8: TRadioButton
          Left = 280
          Top = 24
          Width = 113
          Height = 17
          Caption = #24320#21551#35302#21457
          TabOrder = 1
        end
        object Button4: TButton
          Left = 536
          Top = 20
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 2
          OnClick = Button4Click
        end
      end
      object GroupBox8: TGroupBox
        Left = 16
        Top = 268
        Width = 625
        Height = 57
        Caption = #22810#21151#33021#38190#35302#21457#24320#20851
        TabOrder = 4
        object RadioButton9: TRadioButton
          Left = 136
          Top = 24
          Width = 113
          Height = 17
          Caption = #20572#27490#35302#21457
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton10: TRadioButton
          Left = 280
          Top = 24
          Width = 113
          Height = 17
          Caption = #24320#21551#35302#21457
          TabOrder = 1
        end
        object Button5: TButton
          Left = 536
          Top = 20
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 2
          OnClick = Button5Click
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = #21151#33021#35843#29992
      ImageIndex = 4
      object GroupBox9: TGroupBox
        Left = 16
        Top = 16
        Width = 625
        Height = 110
        Caption = #23439#25805#20316
        TabOrder = 0
        object Label4: TLabel
          Left = 32
          Top = 32
          Width = 96
          Height = 13
          Alignment = taRightJustify
          Caption = #21551#21160#19968#20010#23439#33050#26412#65306
        end
        object Label8: TLabel
          Left = 20
          Top = 72
          Width = 108
          Height = 13
          Alignment = taRightJustify
          Caption = #20174#31532#20960#27493#24320#22987#36816#34892#65306
        end
        object ComboBox_MacroLoad_Select: TComboBox
          Left = 134
          Top = 29
          Width = 155
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          TabOrder = 0
        end
        object Button6: TButton
          Left = 520
          Top = 27
          Width = 91
          Height = 25
          Caption = #30830#23450
          TabOrder = 1
          OnClick = Button6Click
        end
        object Button7: TButton
          Left = 520
          Top = 64
          Width = 91
          Height = 25
          Caption = #20572#27490#23439#30340#36816#34892
          TabOrder = 2
          OnClick = Button7Click
        end
        object ComboBox_MacroLoad_Select_Step: TComboBox
          Left = 134
          Top = 69
          Width = 155
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          TabOrder = 3
        end
      end
      object GroupBox10: TGroupBox
        Left = 16
        Top = 144
        Width = 625
        Height = 89
        Caption = #38145#23450#38190#25805#20316
        TabOrder = 1
        object Label7: TLabel
          Left = 32
          Top = 40
          Width = 96
          Height = 13
          Alignment = taRightJustify
          Caption = #38145#23450#19968#32452#38145#23450#38190#65306
        end
        object ComboBox_KeyLock_Select: TComboBox
          Left = 134
          Top = 37
          Width = 155
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          TabOrder = 0
        end
        object Button8: TButton
          Left = 520
          Top = 35
          Width = 89
          Height = 25
          Caption = #30830#23450
          TabOrder = 1
          OnClick = Button8Click
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = #23439#21151#33021
      ImageIndex = 5
      object GroupBox11: TGroupBox
        Left = 16
        Top = 16
        Width = 625
        Height = 89
        Caption = #23439#25191#34892#36339#36716'-'#22312#24403#21069#32452#20869#36339#36716
        TabOrder = 0
        object Label9: TLabel
          Left = 32
          Top = 40
          Width = 108
          Height = 13
          Alignment = taRightJustify
          Caption = #36339#36716#21040#21738#19968#27493#25191#34892#65306
        end
        object ComboBox_MacroJmp: TComboBox
          Left = 144
          Top = 37
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 0
        end
        object Button_MacroJmpAdd: TButton
          Left = 520
          Top = 35
          Width = 91
          Height = 25
          Caption = #30830#23450
          TabOrder = 1
          OnClick = Button_MacroJmpAddClick
        end
      end
      object GroupBox12: TGroupBox
        Left = 16
        Top = 111
        Width = 625
        Height = 113
        Caption = #23439#25191#34892#36339#36716'-'
        TabOrder = 1
        object Label10: TLabel
          Left = 32
          Top = 40
          Width = 108
          Height = 13
          Alignment = taRightJustify
          Caption = #36339#36716#21040#21738#20010#32452#25191#34892#65306
        end
        object Label11: TLabel
          Left = 8
          Top = 72
          Width = 132
          Height = 13
          Alignment = taRightJustify
          Caption = #20174#36825#20010#32452#30340#21738#19968#27493#25191#34892#65306
        end
        object ComboBox_MacroGroupJmp_GroupSelect: TComboBox
          Left = 144
          Top = 37
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 0
        end
        object Button_MacroGroupJmp_ADD: TButton
          Left = 520
          Top = 49
          Width = 91
          Height = 25
          Caption = #30830#23450
          TabOrder = 1
          OnClick = Button_MacroGroupJmp_ADDClick
        end
        object ComboBox_MacroGroupJmp_StepSelect: TComboBox
          Left = 144
          Top = 69
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 2
        end
      end
      object GroupBox13: TGroupBox
        Left = 16
        Top = 230
        Width = 625
        Height = 90
        Caption = #23439#24310#26102
        TabOrder = 2
        object Label12: TLabel
          Left = 16
          Top = 56
          Width = 176
          Height = 13
          Caption = #33258#23450#20041#30340#26102#38388#65288'1~65535'#27627#31186#65289#65306
        end
        object RadioButton11: TRadioButton
          Left = 128
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'0.1s'
          TabOrder = 0
        end
        object RadioButton12: TRadioButton
          Left = 199
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'0.2s'
          TabOrder = 1
        end
        object RadioButton13: TRadioButton
          Left = 270
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'0.3s'
          TabOrder = 2
        end
        object RadioButton14: TRadioButton
          Left = 341
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'0.5s'
          TabOrder = 3
        end
        object RadioButton15: TRadioButton
          Left = 412
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'0.8s'
          TabOrder = 4
        end
        object RadioButton16: TRadioButton
          Left = 483
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'1s'
          TabOrder = 5
        end
        object RadioButton17: TRadioButton
          Left = 549
          Top = 24
          Width = 65
          Height = 17
          Caption = #24310#26102'1.5s'
          TabOrder = 6
        end
        object RadioButton_Macro_InputTime: TRadioButton
          Left = 8
          Top = 24
          Width = 114
          Height = 17
          Caption = #33258#23450#20041#24310#26102#26102#38388
          Checked = True
          TabOrder = 7
          TabStop = True
        end
        object Edit1: TEdit
          Left = 191
          Top = 53
          Width = 121
          Height = 21
          TabOrder = 8
          Text = '1000'
          OnKeyPress = Edit1KeyPress
        end
        object Button_Macro_Delay_Add: TButton
          Left = 520
          Top = 51
          Width = 91
          Height = 25
          Caption = #30830#23450
          TabOrder = 9
          OnClick = Button_Macro_Delay_AddClick
        end
      end
    end
    object TabSheet7: TTabSheet
      Caption = #32972#20809#25511#21046
      ImageIndex = 6
      object GroupBox14: TGroupBox
        Left = 16
        Top = 16
        Width = 633
        Height = 105
        Caption = #32972#20809#25511#21046
        TabOrder = 0
        object Button_EffectIndexUp: TButton
          Left = 24
          Top = 24
          Width = 105
          Height = 25
          Caption = #19978#19968#20010#28783#25928
          TabOrder = 0
          OnClick = Button_EffectIndexUpClick
        end
        object Button_EffectIndexDown: TButton
          Left = 24
          Top = 64
          Width = 105
          Height = 25
          Caption = #19979#19968#20010#28783#25928
          TabOrder = 1
          OnClick = Button_EffectIndexDownClick
        end
        object Button_EffectSpeedUp: TButton
          Left = 152
          Top = 24
          Width = 113
          Height = 25
          Caption = #28783#25928#26356#24555
          TabOrder = 2
          OnClick = Button_EffectSpeedUpClick
        end
        object Button_EffectSpeedDown: TButton
          Left = 152
          Top = 64
          Width = 113
          Height = 25
          Caption = #28783#25928#26356#24930
          TabOrder = 3
          OnClick = Button_EffectSpeedDownClick
        end
      end
    end
  end
end
