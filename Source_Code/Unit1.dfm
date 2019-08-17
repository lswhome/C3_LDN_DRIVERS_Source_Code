object MainForm: TMainForm
  Left = 549
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  ClientHeight = 502
  ClientWidth = 1271
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poDesigned
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1271
    Height = 502
    ActivePage = TabSheet10
    Align = alClient
    TabOrder = 0
    OnChange = PageControl1Change
    object TabSheet1: TTabSheet
      Caption = #21151#33021#23618#37197#32622
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Img0: TImage
        Left = 19
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img1: TImage
        Tag = 1
        Left = 123
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img2: TImage
        Tag = 2
        Left = 227
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img3: TImage
        Tag = 3
        Left = 331
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img4: TImage
        Tag = 4
        Left = 435
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img5: TImage
        Tag = 5
        Left = 539
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img6: TImage
        Tag = 6
        Left = 643
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object Img7: TImage
        Tag = 7
        Left = 747
        Top = 11
        Width = 86
        Height = 30
        OnClick = Img0DblClick
      end
      object FLIcon0: TImage
        Left = 19
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon1: TImage
        Tag = 1
        Left = 123
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon3: TImage
        Tag = 3
        Left = 331
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon2: TImage
        Tag = 2
        Left = 227
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon5: TImage
        Tag = 5
        Left = 539
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon4: TImage
        Tag = 4
        Left = 435
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon7: TImage
        Tag = 7
        Left = 747
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object FLIcon6: TImage
        Tag = 6
        Left = 643
        Top = 47
        Width = 86
        Height = 21
        OnMouseDown = FLIcon0MouseDown
      end
      object Panel9: TPanel
        Left = 19
        Top = 72
        Width = 1230
        Height = 383
        BevelKind = bkFlat
        BevelOuter = bvNone
        TabOrder = 0
      end
      object Button1: TButton
        Left = 848
        Top = 14
        Width = 75
        Height = 25
        Caption = #19979#36733#37197#32622
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 929
        Top = 14
        Width = 75
        Height = 25
        Caption = #19978#20256#37197#32622
        TabOrder = 2
        OnClick = Button2Click
      end
      object Button3: TButton
        Left = 1010
        Top = 14
        Width = 99
        Height = 25
        Caption = #28165#31354#26412#23618
        TabOrder = 3
        OnClick = Button3Click
      end
      object Button4: TButton
        Left = 1115
        Top = 14
        Width = 60
        Height = 25
        Caption = #21152#36733#37197#32622
        TabOrder = 4
        OnClick = Button4Click
      end
      object Button5: TButton
        Left = 1179
        Top = 14
        Width = 60
        Height = 25
        Caption = #20445#23384#37197#32622
        TabOrder = 5
        OnClick = Button5Click
      end
      object Button11: TButton
        Left = 848
        Top = 43
        Width = 75
        Height = 25
        Caption = #24212#29992#37197#32622
        TabOrder = 6
        OnClick = Button11Click
      end
    end
    object TabSheet2: TTabSheet
      Caption = #35302#21457#23618#37197#32622
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label1: TLabel
        Left = 588
        Top = 10
        Width = 78
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = #21442#32771#23618#65306
      end
      object Label2: TLabel
        Left = 730
        Top = 10
        Width = 75
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = #35302#21457#23618#65306
      end
      object Label3: TLabel
        Left = 408
        Top = 8
        Width = 78
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = #28608#27963#23618#65306
      end
      object PageControl2: TPageControl
        Left = 0
        Top = 0
        Width = 1263
        Height = 284
        ActivePage = TabSheet3
        Align = alTop
        MultiLine = True
        TabHeight = 27
        TabOrder = 0
        OnChange = PageControl2Change
        object TabSheet3: TTabSheet
          Caption = 'FN'#38190#37197#32622
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
        object TabSheet4: TTabSheet
          Caption = #32452#21512#38190#37197#32622
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
        object TabSheet5: TTabSheet
          Caption = #36830#20987#38190#37197#32622
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
        object TabSheet6: TTabSheet
          Caption = #20108#21512#19968#38190#37197#32622
          ImageIndex = 3
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
        object TabSheet7: TTabSheet
          Caption = #25353#19979#20445#25345#38190
          ImageIndex = 4
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
      end
      object Button6: TButton
        Left = 875
        Top = 3
        Width = 68
        Height = 24
        Caption = #19979#36733#37197#32622
        TabOrder = 1
        OnClick = Button6Click
      end
      object Button7: TButton
        Left = 949
        Top = 3
        Width = 68
        Height = 24
        Caption = #19978#20256#37197#32622
        TabOrder = 2
        OnClick = Button7Click
      end
      object Button8: TButton
        Left = 1023
        Top = 3
        Width = 91
        Height = 24
        Caption = #28165#31354#26412#23618#25968#25454
        TabOrder = 3
        OnClick = Button8Click
      end
      object Button9: TButton
        Left = 1120
        Top = 3
        Width = 68
        Height = 24
        Caption = #21152#36733#37197#32622
        TabOrder = 4
        OnClick = Button9Click
      end
      object Button10: TButton
        Left = 1193
        Top = 3
        Width = 68
        Height = 24
        Caption = #20445#23384#37197#32622
        TabOrder = 5
        OnClick = Button10Click
      end
      object ComboBox2: TComboBox
        Left = 808
        Top = 5
        Width = 60
        Height = 21
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 6
        Text = '0'
        OnSelect = ComboBox2Select
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object ComboBox1: TComboBox
        Left = 667
        Top = 5
        Width = 60
        Height = 21
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 7
        Text = '0'
        OnSelect = ComboBox1Select
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object Panel10: TPanel
        Left = 18
        Top = 293
        Width = 1230
        Height = 383
        BevelOuter = bvNone
        TabOrder = 8
      end
      object ComboBox3: TComboBox
        Left = 487
        Top = 5
        Width = 60
        Height = 21
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 9
        Text = '0'
        OnChange = ComboBox3Change
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object Button12: TButton
        Left = 550
        Top = 3
        Width = 33
        Height = 24
        Caption = 'OK'
        TabOrder = 10
        OnClick = Button11Click
      end
    end
    object TabSheet9: TTabSheet
      Caption = #23439#37197#32622
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox1: TGroupBox
        Left = 3
        Top = 3
        Width = 1257
        Height = 54
        Caption = #23439#21151#33021
        TabOrder = 0
        object Label4: TLabel
          Left = 24
          Top = 24
          Width = 73
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #35201#32534#36753#30340#32452#65306
        end
        object ComboBox4: TComboBox
          Left = 99
          Top = 21
          Width = 102
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          ItemIndex = 0
          TabOrder = 0
          Text = '0'
          OnChange = ComboBox4Change
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20'
            '21'
            '22'
            '23'
            '24'
            '25'
            '26'
            '27'
            '28'
            '29'
            '30'
            '31'
            '32'
            '33'
            '34'
            '35'
            '36'
            '37'
            '38'
            '39'
            '40'
            '41'
            '42'
            '43'
            '44'
            '45'
            '46'
            '47'
            '48'
            '49'
            '50'
            '51'
            '52'
            '53'
            '54'
            '55'
            '56'
            '57'
            '58'
            '59'
            '60'
            '61'
            '62'
            '63'
            '64'
            '65'
            '66'
            '67'
            '68'
            '69'
            '70'
            '71'
            '72'
            '73'
            '74'
            '75'
            '76'
            '77'
            '78'
            '79'
            '80'
            '81'
            '82'
            '83'
            '84'
            '85'
            '86'
            '87'
            '88'
            '89'
            '90'
            '91'
            '92'
            '93'
            '94'
            '95'
            '96'
            '97'
            '98'
            '99')
        end
        object Button13: TButton
          Left = 240
          Top = 19
          Width = 75
          Height = 25
          Caption = #19979#36733
          TabOrder = 1
          OnClick = Button13Click
        end
        object Button14: TButton
          Left = 336
          Top = 19
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 2
          OnClick = Button14Click
        end
        object Button15: TButton
          Left = 432
          Top = 19
          Width = 75
          Height = 25
          Caption = #20445#23384
          TabOrder = 3
          OnClick = Button15Click
        end
        object Button16: TButton
          Left = 528
          Top = 19
          Width = 75
          Height = 25
          Caption = #35835#21462
          TabOrder = 4
          OnClick = Button16Click
        end
      end
      object MacroEdit: TListView
        Left = 3
        Top = 63
        Width = 1257
        Height = 408
        Columns = <
          item
            Caption = 'Step ID'
            Width = 70
          end
          item
            Caption = 'Functions'
            Width = 600
          end>
        GridLines = True
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 1
        ViewStyle = vsReport
        OnDblClick = MacroEditDblClick
        OnMouseDown = MacroEditMouseDown
      end
    end
    object TabSheet8: TTabSheet
      Caption = #38145#23450#38190#37197#32622
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox2: TGroupBox
        Left = 3
        Top = 3
        Width = 1257
        Height = 54
        Caption = #38145#23450#38190#21151#33021
        TabOrder = 0
        object Button17: TButton
          Left = 11
          Top = 17
          Width = 75
          Height = 25
          Caption = #19979#36733
          TabOrder = 0
          OnClick = Button17Click
        end
        object Button18: TButton
          Left = 107
          Top = 17
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 1
          OnClick = Button18Click
        end
        object Button19: TButton
          Left = 203
          Top = 17
          Width = 75
          Height = 25
          Caption = #20445#23384
          TabOrder = 2
          OnClick = Button19Click
        end
        object Button20: TButton
          Left = 299
          Top = 17
          Width = 75
          Height = 25
          Caption = #35835#21462
          TabOrder = 3
          OnClick = Button20Click
        end
      end
      object LockKeyEdit: TListView
        Left = 3
        Top = 63
        Width = 1257
        Height = 408
        Columns = <
          item
            Caption = 'Group ID'
            Width = 70
          end
          item
            Caption = 'Key1'
            Width = 145
          end
          item
            Caption = 'Key2'
            Width = 145
          end
          item
            Caption = 'Key3'
            Width = 145
          end
          item
            Caption = 'Key4'
            Width = 145
          end
          item
            Caption = 'Key5'
            Width = 145
          end
          item
            Caption = 'Key6'
            Width = 145
          end
          item
            Caption = 'Key7'
            Width = 145
          end
          item
            Caption = 'Key8'
            Width = 145
          end>
        GridLines = True
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 1
        ViewStyle = vsReport
        OnDblClick = LockKeyEditDblClick
        OnMouseDown = LockKeyEditMouseDown
      end
    end
    object TabSheet10: TTabSheet
      Caption = #36724#28783#28783#25928#37197#32622
      ImageIndex = 4
      object GroupBox3: TGroupBox
        Left = 16
        Top = 16
        Width = 537
        Height = 193
        Caption = #36724#28783#39068#33394#36890#36947#35843#25972
        TabOrder = 0
        object Label5: TLabel
          Left = 24
          Top = 32
          Width = 7
          Height = 13
          Caption = 'R'
        end
        object Label6: TLabel
          Left = 24
          Top = 71
          Width = 7
          Height = 13
          Caption = 'G'
        end
        object Label7: TLabel
          Left = 24
          Top = 110
          Width = 6
          Height = 13
          Caption = 'B'
        end
        object TrackBar1: TTrackBar
          Left = 45
          Top = 29
          Width = 460
          Height = 33
          Max = 63
          Position = 63
          PositionToolTip = ptTop
          TabOrder = 0
        end
        object TrackBar2: TTrackBar
          Left = 45
          Top = 68
          Width = 460
          Height = 33
          Max = 63
          Position = 63
          PositionToolTip = ptTop
          TabOrder = 1
        end
        object TrackBar3: TTrackBar
          Left = 45
          Top = 107
          Width = 460
          Height = 33
          Max = 63
          Position = 63
          PositionToolTip = ptTop
          TabOrder = 2
        end
        object Button21: TButton
          Left = 430
          Top = 154
          Width = 75
          Height = 25
          Caption = #24212#29992
          TabOrder = 3
          OnClick = Button21Click
        end
      end
      object GroupBox4: TGroupBox
        Left = 576
        Top = 16
        Width = 665
        Height = 193
        Caption = #36724#28783#28783#25928#35843#25972
        TabOrder = 1
        object Label8: TLabel
          Left = 16
          Top = 32
          Width = 89
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #28783#25928#20999#25442#65306
        end
        object Label9: TLabel
          Left = 16
          Top = 74
          Width = 89
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #28783#25928#36895#24230#65306
        end
        object Label10: TLabel
          Left = 16
          Top = 113
          Width = 89
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #28783#25928#21442#25968#65306
        end
        object TrackBar4: TTrackBar
          Left = 111
          Top = 29
          Width = 458
          Height = 33
          Max = 20
          PositionToolTip = ptTop
          TabOrder = 0
        end
        object Button22: TButton
          Left = 575
          Top = 27
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 1
          OnClick = Button22Click
        end
        object TrackBar5: TTrackBar
          Left = 111
          Top = 71
          Width = 458
          Height = 33
          Max = 64
          Min = 1
          Position = 15
          PositionToolTip = ptTop
          TabOrder = 2
        end
        object Button23: TButton
          Left = 575
          Top = 69
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 3
          OnClick = Button23Click
        end
        object TrackBar6: TTrackBar
          Left = 111
          Top = 110
          Width = 458
          Height = 33
          Max = 24
          Min = 1
          Position = 1
          PositionToolTip = ptTop
          TabOrder = 4
        end
        object Button24: TButton
          Left = 575
          Top = 108
          Width = 75
          Height = 25
          Caption = #30830#23450
          TabOrder = 5
          OnClick = Button24Click
        end
        object Button25: TButton
          Left = 80
          Top = 154
          Width = 113
          Height = 25
          Caption = #19978#19968#20010#28783#25928
          TabOrder = 6
          OnClick = Button25Click
        end
        object Button26: TButton
          Left = 208
          Top = 154
          Width = 113
          Height = 25
          Caption = #19979#19968#20010#28783#25928
          TabOrder = 7
          OnClick = Button26Click
        end
      end
      object GroupBox5: TGroupBox
        Left = 16
        Top = 224
        Width = 393
        Height = 233
        Caption = 'CAPS'#25351#31034#28783#37197#32622
        TabOrder = 2
        object Image1: TImage
          Left = 243
          Top = 136
          Width = 105
          Height = 73
          OnClick = Image1Click
        end
        object Label11: TLabel
          Left = 16
          Top = 104
          Width = 145
          Height = 13
          AutoSize = False
          Caption = #36724#22352#26631#65306
        end
        object Button27: TButton
          Left = 16
          Top = 64
          Width = 177
          Height = 25
          Caption = #36873#25321#25351#31034#29992#30340#28783#29664
          TabOrder = 0
          OnClick = Button27Click
        end
        object RadioButton3: TRadioButton
          Left = 16
          Top = 128
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#38543#26426#38745#24577
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object RadioButton4: TRadioButton
          Left = 16
          Top = 151
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#19971#24425#21464#25442
          TabOrder = 2
        end
        object RadioButton5: TRadioButton
          Left = 16
          Top = 174
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#38543#26426#38378#28865
          TabOrder = 3
        end
        object RadioButton6: TRadioButton
          Left = 16
          Top = 197
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#25351#23450#39068#33394
          TabOrder = 4
        end
        object Panel20: TPanel
          Left = 16
          Top = 24
          Width = 361
          Height = 34
          BevelOuter = bvNone
          TabOrder = 5
          object CheckBox1: TCheckBox
            Left = 13
            Top = 9
            Width = 81
            Height = 17
            Caption = #24320#21551
            TabOrder = 0
            OnClick = CheckBox1Click
          end
          object RadioButton1: TRadioButton
            Left = 103
            Top = 9
            Width = 118
            Height = 17
            Caption = #20351#29992#36724#28783#25351#31034
            Checked = True
            TabOrder = 1
            TabStop = True
            OnClick = IndicatorModeSelectClick
          end
          object RadioButton2: TRadioButton
            Left = 227
            Top = 9
            Width = 126
            Height = 17
            Caption = #20351#29992'WS2812'#25351#31034
            TabOrder = 2
            OnClick = IndicatorModeSelectClick
          end
        end
        object Button30: TButton
          Left = 272
          Top = 64
          Width = 105
          Height = 25
          Caption = #24212#29992
          TabOrder = 6
          OnClick = Button30Click
        end
        object CheckBox4: TCheckBox
          Left = 243
          Top = 104
          Width = 126
          Height = 17
          Caption = #36319#38543#36724#28783#28783#25928
          TabOrder = 7
          OnClick = CheckBox4Click
        end
      end
      object GroupBox6: TGroupBox
        Left = 432
        Top = 224
        Width = 393
        Height = 233
        Caption = 'SCROLL'#25351#31034#28783#37197#32622
        TabOrder = 3
        object Image2: TImage
          Tag = 1
          Left = 243
          Top = 136
          Width = 105
          Height = 73
          OnClick = Image1Click
        end
        object Label12: TLabel
          Left = 16
          Top = 104
          Width = 145
          Height = 13
          AutoSize = False
          Caption = #36724#22352#26631#65306
        end
        object Button28: TButton
          Tag = 1
          Left = 16
          Top = 64
          Width = 177
          Height = 25
          Caption = #36873#25321#25351#31034#29992#30340#28783#29664
          TabOrder = 0
          OnClick = Button28Click
        end
        object RadioButton7: TRadioButton
          Left = 16
          Top = 128
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#38543#26426#38745#24577
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object RadioButton8: TRadioButton
          Left = 16
          Top = 151
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#19971#24425#21464#25442
          TabOrder = 2
        end
        object RadioButton9: TRadioButton
          Left = 16
          Top = 174
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#38543#26426#38378#28865
          TabOrder = 3
        end
        object RadioButton10: TRadioButton
          Left = 16
          Top = 197
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#25351#23450#39068#33394
          TabOrder = 4
        end
        object Panel21: TPanel
          Left = 16
          Top = 24
          Width = 361
          Height = 34
          BevelOuter = bvNone
          TabOrder = 5
          object CheckBox2: TCheckBox
            Tag = 1
            Left = 13
            Top = 9
            Width = 81
            Height = 17
            Caption = #24320#21551
            TabOrder = 0
            OnClick = CheckBox1Click
          end
          object RadioButton11: TRadioButton
            Tag = 1
            Left = 103
            Top = 9
            Width = 118
            Height = 17
            Caption = #20351#29992#36724#28783#25351#31034
            Checked = True
            TabOrder = 1
            TabStop = True
            OnClick = IndicatorModeSelectClick
          end
          object RadioButton12: TRadioButton
            Tag = 1
            Left = 227
            Top = 9
            Width = 126
            Height = 17
            Caption = #20351#29992'WS2812'#25351#31034
            TabOrder = 2
            OnClick = IndicatorModeSelectClick
          end
        end
        object Button29: TButton
          Tag = 1
          Left = 272
          Top = 64
          Width = 105
          Height = 25
          Caption = #24212#29992
          TabOrder = 6
          OnClick = Button29Click
        end
        object CheckBox5: TCheckBox
          Left = 243
          Top = 103
          Width = 126
          Height = 17
          Caption = #36319#38543#36724#28783#28783#25928
          TabOrder = 7
          OnClick = CheckBox5Click
        end
      end
      object GroupBox7: TGroupBox
        Left = 848
        Top = 224
        Width = 393
        Height = 233
        Caption = 'NUM'#25351#31034#28783#37197#32622
        TabOrder = 4
        object Image3: TImage
          Tag = 2
          Left = 243
          Top = 136
          Width = 105
          Height = 73
          OnClick = Image1Click
        end
        object Label13: TLabel
          Left = 16
          Top = 104
          Width = 145
          Height = 13
          AutoSize = False
          Caption = #36724#22352#26631#65306
        end
        object Button31: TButton
          Tag = 2
          Left = 16
          Top = 64
          Width = 177
          Height = 25
          Caption = #36873#25321#25351#31034#29992#30340#28783#29664
          TabOrder = 0
          OnClick = Button31Click
        end
        object RadioButton13: TRadioButton
          Left = 16
          Top = 128
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#38543#26426#38745#24577
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object RadioButton14: TRadioButton
          Left = 16
          Top = 151
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#19971#24425#21464#25442
          TabOrder = 2
        end
        object RadioButton15: TRadioButton
          Left = 16
          Top = 174
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#38543#26426#38378#28865
          TabOrder = 3
        end
        object RadioButton16: TRadioButton
          Left = 16
          Top = 197
          Width = 145
          Height = 17
          Caption = #28783#25928#65306#25351#23450#39068#33394
          TabOrder = 4
        end
        object Panel22: TPanel
          Left = 16
          Top = 24
          Width = 361
          Height = 34
          BevelOuter = bvNone
          TabOrder = 5
          object CheckBox3: TCheckBox
            Tag = 2
            Left = 13
            Top = 9
            Width = 81
            Height = 17
            Caption = #24320#21551
            TabOrder = 0
            OnClick = CheckBox1Click
          end
          object RadioButton17: TRadioButton
            Tag = 2
            Left = 103
            Top = 9
            Width = 118
            Height = 17
            Caption = #20351#29992#36724#28783#25351#31034
            Checked = True
            TabOrder = 1
            TabStop = True
            OnClick = IndicatorModeSelectClick
          end
          object RadioButton18: TRadioButton
            Tag = 2
            Left = 227
            Top = 9
            Width = 126
            Height = 17
            Caption = #20351#29992'WS2812'#25351#31034
            TabOrder = 2
            OnClick = IndicatorModeSelectClick
          end
        end
        object Button32: TButton
          Tag = 2
          Left = 272
          Top = 64
          Width = 105
          Height = 25
          Caption = #24212#29992
          TabOrder = 6
          OnClick = Button32Click
        end
        object CheckBox6: TCheckBox
          Left = 243
          Top = 104
          Width = 126
          Height = 17
          Caption = #36319#38543#36724#28783#28783#25928
          TabOrder = 7
          OnClick = CheckBox6Click
        end
      end
    end
    object TabSheet11: TTabSheet
      Caption = 'WS2812'#28783#29664#37197#32622#21644#25351#31034#28783#21160#20316#37197#32622
      ImageIndex = 5
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox8: TGroupBox
        Left = 27
        Top = 184
        Width = 574
        Height = 265
        Caption = #32452#37197#32622
        TabOrder = 0
        object Label15: TLabel
          Left = 135
          Top = 33
          Width = 96
          Height = 13
          Caption = #20351#29992#28783#29664#32534#21495#20174#65306
        end
        object Label16: TLabel
          Left = 311
          Top = 33
          Width = 18
          Height = 13
          Alignment = taCenter
          AutoSize = False
          Caption = #21040
        end
        object Image4: TImage
          Left = 264
          Top = 88
          Width = 49
          Height = 49
          OnClick = WS_ImgClick
        end
        object Image5: TImage
          Tag = 1
          Left = 319
          Top = 88
          Width = 49
          Height = 49
          OnClick = WS_ImgClick
        end
        object Image6: TImage
          Tag = 2
          Left = 374
          Top = 88
          Width = 49
          Height = 49
          OnClick = WS_ImgClick
        end
        object Image7: TImage
          Tag = 3
          Left = 429
          Top = 88
          Width = 49
          Height = 49
          OnClick = WS_ImgClick
        end
        object CheckBox7: TCheckBox
          Left = 24
          Top = 32
          Width = 97
          Height = 17
          Caption = #21551#29992
          TabOrder = 0
        end
        object ComboBox6: TComboBox
          Left = 237
          Top = 30
          Width = 68
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          ItemIndex = 0
          TabOrder = 1
          Text = '0'
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20'
            '21'
            '22'
            '23'
            '24'
            '25'
            '26'
            '27'
            '28'
            '29'
            '30'
            '31')
        end
        object ComboBox7: TComboBox
          Left = 335
          Top = 30
          Width = 68
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          ItemIndex = 0
          TabOrder = 2
          Text = '0'
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7'
            '8'
            '9'
            '10'
            '11'
            '12'
            '13'
            '14'
            '15'
            '16'
            '17'
            '18'
            '19'
            '20'
            '21'
            '22'
            '23'
            '24'
            '25'
            '26'
            '27'
            '28'
            '29'
            '30'
            '31')
        end
        object RadioButton19: TRadioButton
          Left = 24
          Top = 112
          Width = 113
          Height = 17
          Caption = #38543#26426#39068#33394#21628#21560
          TabOrder = 3
        end
        object RadioButton20: TRadioButton
          Left = 24
          Top = 143
          Width = 113
          Height = 17
          Caption = #35774#23450#30340#39068#33394#21628#21560
          TabOrder = 4
        end
        object RadioButton21: TRadioButton
          Left = 24
          Top = 176
          Width = 113
          Height = 17
          Caption = #35774#23450#30340#39068#33394#28176#21464
          TabOrder = 5
        end
        object RadioButton22: TRadioButton
          Left = 24
          Top = 208
          Width = 113
          Height = 17
          Caption = #38543#26426#39068#33394#28176#21464
          TabOrder = 6
        end
        object RadioButton23: TRadioButton
          Left = 24
          Top = 80
          Width = 113
          Height = 17
          Caption = #24425#33394#36305#39532#28783
          Checked = True
          TabOrder = 7
          TabStop = True
        end
        object Button33: TButton
          Left = 461
          Top = 28
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 8
          OnClick = Button33Click
        end
        object WS_Speed: TTrackBar
          Left = 192
          Top = 167
          Width = 360
          Height = 33
          Hint = #35774#32622#28783#25928#36816#34892#36895#24230
          Max = 64
          Min = 1
          ParentShowHint = False
          Position = 15
          PositionToolTip = ptTop
          ShowHint = True
          TabOrder = 9
        end
      end
      object GroupBox9: TGroupBox
        Left = 27
        Top = 16
        Width = 574
        Height = 153
        Caption = 'WS2812'#28783#29664#28783#25928#37197#32622
        TabOrder = 1
        object Label14: TLabel
          Left = 24
          Top = 32
          Width = 105
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403#21069#37197#32622#30340#32452#65306
        end
        object ComboBox5: TComboBox
          Left = 135
          Top = 29
          Width = 122
          Height = 21
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 0
          Text = '0'
          OnChange = ComboBox5Change
          Items.Strings = (
            '0'
            '1'
            '2'
            '3')
        end
      end
      object GroupBox10: TGroupBox
        Left = 624
        Top = 16
        Width = 625
        Height = 129
        Caption = #25351#31034#28783#21160#20316#65306'CAPS'
        TabOrder = 2
        object Label17: TLabel
          Left = 16
          Top = 64
          Width = 162
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403'CAPS'#28857#20142#26102#35302#21457#30340#21151#33021#65306
        end
        object Label18: TLabel
          Left = 16
          Top = 94
          Width = 162
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403'CAPS'#29060#28781#26102#35302#21457#30340#21151#33021#65306
        end
        object Panel23: TPanel
          Left = 184
          Top = 60
          Width = 417
          Height = 23
          Alignment = taLeftJustify
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 0
          OnMouseDown = Panel23MouseDown
        end
        object Panel24: TPanel
          Left = 184
          Top = 90
          Width = 417
          Height = 23
          Alignment = taLeftJustify
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 1
          OnMouseDown = Panel24MouseDown
        end
        object CheckBox8: TCheckBox
          Left = 32
          Top = 28
          Width = 97
          Height = 17
          Caption = #21551#29992#36825#20010#21151#33021
          TabOrder = 2
          OnClick = CheckBox10Click
        end
        object Button34: TButton
          Left = 184
          Top = 24
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 3
          OnClick = Button34Click
        end
      end
      object GroupBox11: TGroupBox
        Left = 624
        Top = 168
        Width = 625
        Height = 129
        Caption = #25351#31034#28783#21160#20316#65306'SCROLL'
        TabOrder = 3
        object Label19: TLabel
          Left = 16
          Top = 64
          Width = 162
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403'SCROLL'#28857#20142#26102#35302#21457#30340#21151#33021#65306
        end
        object Label20: TLabel
          Left = 16
          Top = 94
          Width = 162
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403'SCROLL'#29060#28781#26102#35302#21457#30340#21151#33021#65306
        end
        object Panel25: TPanel
          Tag = 1
          Left = 184
          Top = 60
          Width = 417
          Height = 23
          Alignment = taLeftJustify
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 0
          OnMouseDown = Panel23MouseDown
        end
        object Panel26: TPanel
          Tag = 1
          Left = 184
          Top = 90
          Width = 417
          Height = 23
          Alignment = taLeftJustify
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 1
          OnMouseDown = Panel24MouseDown
        end
        object CheckBox9: TCheckBox
          Tag = 1
          Left = 32
          Top = 28
          Width = 97
          Height = 17
          Caption = #21551#29992#36825#20010#21151#33021
          TabOrder = 2
          OnClick = CheckBox10Click
        end
        object Button35: TButton
          Tag = 1
          Left = 184
          Top = 24
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 3
          OnClick = Button34Click
        end
      end
      object GroupBox12: TGroupBox
        Left = 624
        Top = 320
        Width = 625
        Height = 129
        Caption = #25351#31034#28783#21160#20316#65306'NUM'
        TabOrder = 4
        object Label21: TLabel
          Left = 16
          Top = 64
          Width = 162
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403'NUM'#28857#20142#26102#35302#21457#30340#21151#33021#65306
        end
        object Label22: TLabel
          Left = 16
          Top = 94
          Width = 162
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #24403'NUM'#29060#28781#26102#35302#21457#30340#21151#33021#65306
        end
        object Panel27: TPanel
          Tag = 2
          Left = 184
          Top = 60
          Width = 417
          Height = 23
          Alignment = taLeftJustify
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 0
          OnMouseDown = Panel23MouseDown
        end
        object Panel28: TPanel
          Tag = 2
          Left = 184
          Top = 90
          Width = 417
          Height = 23
          Alignment = taLeftJustify
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 1
          OnMouseDown = Panel24MouseDown
        end
        object CheckBox10: TCheckBox
          Tag = 2
          Left = 32
          Top = 28
          Width = 97
          Height = 17
          Caption = #21551#29992#36825#20010#21151#33021
          TabOrder = 2
          OnClick = CheckBox10Click
        end
        object Button36: TButton
          Tag = 2
          Left = 184
          Top = 24
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 3
          OnClick = Button34Click
        end
      end
    end
    object TabSheet12: TTabSheet
      Caption = #33258#23450#20041#32972#20809#37197#32622
      ImageIndex = 6
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Panel29: TPanel
        Left = 16
        Top = 280
        Width = 1230
        Height = 383
        BevelKind = bkFlat
        BevelOuter = bvNone
        TabOrder = 0
      end
      object GroupBox13: TGroupBox
        Left = 16
        Top = 16
        Width = 593
        Height = 249
        Caption = #33258#23450#20041#28783#25928#32452#37197#32622
        TabOrder = 1
        object Label23: TLabel
          Left = 16
          Top = 32
          Width = 97
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #28783#25928#32534#32452#65306
        end
        object Label24: TLabel
          Left = 16
          Top = 64
          Width = 97
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #20351#29992#22270#29255#32534#21495#65306
        end
        object ComboBox8: TComboBox
          Left = 119
          Top = 29
          Width = 145
          Height = 21
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 0
          Text = '0'
          OnChange = ComboBox8Change
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '7')
        end
        object Button37: TButton
          Left = 288
          Top = 27
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 1
          OnClick = Button37Click
        end
        object RadioButton24: TRadioButton
          Left = 32
          Top = 152
          Width = 145
          Height = 17
          Caption = #38745#24577#23637#31034
          Checked = True
          TabOrder = 2
          TabStop = True
          OnClick = RadioButton24Click
        end
        object RadioButton25: TRadioButton
          Tag = 1
          Left = 32
          Top = 175
          Width = 145
          Height = 17
          Caption = #21521#24038#24490#29615#28378#21160
          TabOrder = 3
          OnClick = RadioButton24Click
        end
        object RadioButton26: TRadioButton
          Tag = 2
          Left = 32
          Top = 198
          Width = 145
          Height = 17
          Caption = #21521#21491#24490#29615#28378#21160
          TabOrder = 4
          OnClick = RadioButton24Click
        end
        object RadioButton27: TRadioButton
          Tag = 3
          Left = 264
          Top = 152
          Width = 145
          Height = 17
          Caption = #21628#21560#27169#24335
          TabOrder = 5
          OnClick = RadioButton24Click
        end
        object RadioButton28: TRadioButton
          Tag = 4
          Left = 264
          Top = 175
          Width = 145
          Height = 17
          Caption = #29190#38378#27169#24335
          TabOrder = 6
          OnClick = RadioButton24Click
        end
        object ComboBox9: TComboBox
          Left = 119
          Top = 61
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 7
          OnChange = ComboBox9Change
        end
        object TrackBar7: TTrackBar
          Left = 16
          Top = 99
          Width = 562
          Height = 33
          Max = 64
          Min = 1
          Position = 15
          PositionToolTip = ptTop
          TabOrder = 8
          OnChange = TrackBar7Change
        end
      end
      object GroupBox14: TGroupBox
        Left = 640
        Top = 16
        Width = 606
        Height = 249
        Caption = #22270#29255#31649#29702
        TabOrder = 2
        object Label25: TLabel
          Left = 24
          Top = 32
          Width = 97
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #22270#29255#32534#21495#65306
        end
        object ComboBox10: TComboBox
          Left = 127
          Top = 29
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 0
          OnChange = ComboBox10Change
        end
        object Button39: TButton
          Left = 408
          Top = 27
          Width = 75
          Height = 25
          Caption = #19978#20256
          TabOrder = 1
          OnClick = Button39Click
        end
        object Button40: TButton
          Left = 288
          Top = 27
          Width = 75
          Height = 25
          Caption = #19979#36733
          TabOrder = 2
          OnClick = Button40Click
        end
        object Button41: TButton
          Left = 40
          Top = 69
          Width = 145
          Height = 25
          Caption = #36733#20837#19968#20010'BMP'#22270#29255
          TabOrder = 3
          OnClick = Button41Click
        end
        object Button42: TButton
          Left = 224
          Top = 69
          Width = 145
          Height = 25
          Caption = #25152#26377#25353#38190#38543#26426#39068#33394
          TabOrder = 4
          OnClick = Button42Click
        end
        object Button43: TButton
          Left = 408
          Top = 69
          Width = 145
          Height = 25
          Caption = #35774#32622#20026#30456#21516#30340#39068#33394
          TabOrder = 5
          OnClick = Button43Click
        end
        object Panel30: TPanel
          Left = 40
          Top = 112
          Width = 529
          Height = 121
          BevelKind = bkFlat
          BevelOuter = bvNone
          TabOrder = 6
        end
      end
    end
    object TabSheet13: TTabSheet
      Caption = #31995#32479#21151#33021
      ImageIndex = 7
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox15: TGroupBox
        Left = 16
        Top = 16
        Width = 601
        Height = 177
        Caption = #22266#20214#21047#20889
        TabOrder = 0
        object Label26: TLabel
          Left = 19
          Top = 111
          Width = 566
          Height = 15
          Alignment = taCenter
          AutoSize = False
        end
        object Button38: TButton
          Left = 72
          Top = 32
          Width = 137
          Height = 25
          Caption = #20999#25442#21040'BootLoader'
          Enabled = False
          TabOrder = 0
          OnClick = Button38Click
        end
        object Button44: TButton
          Left = 72
          Top = 71
          Width = 137
          Height = 25
          Caption = #21551#21160#38190#30424#22266#20214
          Enabled = False
          TabOrder = 1
          OnClick = Button44Click
        end
        object Button45: TButton
          Left = 224
          Top = 71
          Width = 137
          Height = 25
          Caption = #36873#25321#22266#20214#25991#20214
          Enabled = False
          TabOrder = 2
          OnClick = Button45Click
        end
        object Button46: TButton
          Left = 376
          Top = 71
          Width = 137
          Height = 25
          Caption = #21551#21160#21047#20889#36807#31243
          Enabled = False
          TabOrder = 3
          OnClick = Button46Click
        end
        object Button47: TButton
          Left = 376
          Top = 32
          Width = 137
          Height = 25
          Caption = #24674#22797#40664#35748#20540
          Enabled = False
          TabOrder = 4
          OnClick = Button47Click
        end
        object Button48: TButton
          Left = 224
          Top = 32
          Width = 137
          Height = 25
          Caption = #21047#20837#30828#20214#37197#32622
          Enabled = False
          TabOrder = 5
          OnClick = Button48Click
        end
        object ProgressBar1: TProgressBar
          Left = 19
          Top = 136
          Width = 566
          Height = 25
          TabOrder = 6
        end
      end
      object GroupBox16: TGroupBox
        Left = 16
        Top = 199
        Width = 601
        Height = 82
        Caption = #30382#32932#21644#35821#35328
        TabOrder = 1
        object Label27: TLabel
          Left = 19
          Top = 40
          Width = 118
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #36873#25321#19968#20010#30382#32932#65306
        end
        object Label28: TLabel
          Left = 312
          Top = 40
          Width = 97
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = #36873#25321#35821#35328#65306
        end
        object ComboBox11: TComboBox
          Left = 141
          Top = 37
          Width = 145
          Height = 21
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 0
          OnChange = ComboBox11Change
        end
        object ComboBox12: TComboBox
          Left = 417
          Top = 37
          Width = 160
          Height = 21
          Style = csDropDownList
          TabOrder = 1
          OnChange = ComboBox12Change
        end
      end
      object GroupBox17: TGroupBox
        Left = 16
        Top = 287
        Width = 601
        Height = 170
        Caption = #20851#26426#21518#26159#21542#20851#38381#32972#20809
        TabOrder = 2
        object CheckBox11: TCheckBox
          Left = 19
          Top = 24
          Width = 97
          Height = 17
          Caption = #21551#29992#36825#20010#21151#33021
          TabOrder = 0
          OnClick = CheckBox11Click
        end
        object CheckBox12: TCheckBox
          Left = 260
          Top = 59
          Width = 133
          Height = 17
          Caption = #20851#38381#32972#20809
          Enabled = False
          TabOrder = 1
          OnClick = CheckBox12Click
        end
        object CheckBox13: TCheckBox
          Tag = 1
          Left = 56
          Top = 59
          Width = 185
          Height = 17
          Caption = #20851#38381'WS2812'#28783#25928#32452'0'
          Enabled = False
          TabOrder = 2
          OnClick = CheckBox12Click
        end
        object CheckBox14: TCheckBox
          Tag = 2
          Left = 55
          Top = 82
          Width = 186
          Height = 17
          Caption = #20851#38381'WS2812'#28783#25928#32452'1'
          Enabled = False
          TabOrder = 3
          OnClick = CheckBox12Click
        end
        object CheckBox15: TCheckBox
          Tag = 3
          Left = 55
          Top = 105
          Width = 186
          Height = 17
          Caption = #20851#38381'WS2812'#28783#25928#32452'2'
          Enabled = False
          TabOrder = 4
          OnClick = CheckBox12Click
        end
        object CheckBox16: TCheckBox
          Tag = 4
          Left = 55
          Top = 128
          Width = 186
          Height = 17
          Caption = #20851#38381'WS2812'#28783#25928#32452'3'
          Enabled = False
          TabOrder = 5
          OnClick = CheckBox12Click
        end
        object Button49: TButton
          Left = 496
          Top = 120
          Width = 75
          Height = 25
          Caption = #24212#29992
          TabOrder = 6
          OnClick = Button49Click
        end
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 1472
    Top = 192
  end
  object KeyLockPopMenu: TPopupMenu
    Left = 1079
    Top = 65523
    object N1: TMenuItem
      Caption = #28165#38500#24403#21069#21333#20803#26684
      OnClick = N1Click
    end
    object N2: TMenuItem
      Tag = 1
      Caption = #28165#38500#25972#32452#25968#25454
      OnClick = N1Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object N4: TMenuItem
      Tag = 2
      Caption = #28165#31354#25152#36873#32452#30340#25968#25454
      OnClick = N1Click
    end
  end
  object MacroEditPopMenu: TPopupMenu
    Left = 1120
    Top = 65520
    object N5: TMenuItem
      Caption = #28165#38500#24403#21069#27493#30340#25968#25454
      OnClick = N5Click
    end
    object N6: TMenuItem
      Tag = 1
      Caption = #28165#38500#25152#36873#30340#27493#25968#25454
      OnClick = N5Click
    end
  end
  object ColorDialog1: TColorDialog
    Left = 1432
    Top = 192
  end
end
