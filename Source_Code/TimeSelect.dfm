object TSel: TTSel
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Input a number for time'
  ClientHeight = 80
  ClientWidth = 284
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 151
    Top = 45
    Width = 13
    Height = 13
    Caption = 'ms'
  end
  object Label2: TLabel
    Left = 24
    Top = 15
    Width = 25
    Height = 13
    Caption = 'Limit:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Button1: TButton
    Left = 186
    Top = 40
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 24
    Top = 42
    Width = 121
    Height = 21
    TabOrder = 0
    Text = '0'
    OnChange = Edit1Change
    OnKeyPress = Edit1KeyPress
  end
end
