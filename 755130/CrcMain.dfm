object frmCrcMain: TfrmCrcMain
  Left = 161
  Top = 163
  BorderStyle = bsDialog
  Caption = 'CRC'
  ClientHeight = 366
  ClientWidth = 456
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object cmdProtect: TButton
    Left = 8
    Top = 80
    Width = 105
    Height = 25
    Caption = #1047#1072#1087#1080#1089#1072#1090#1100' CRC'
    TabOrder = 0
    OnClick = cmdProtectClick
  end
  object cmdVerify: TButton
    Left = 120
    Top = 80
    Width = 105
    Height = 25
    Caption = #1055#1088#1086#1074#1077#1088#1080#1090#1100' CRC'
    TabOrder = 1
    OnClick = cmdVerifyClick
  end
  object txtFileName: TEdit
    Left = 8
    Top = 8
    Width = 393
    Height = 21
    TabOrder = 2
  end
  object cmdOpenFile: TButton
    Left = 408
    Top = 8
    Width = 25
    Height = 25
    Caption = '...'
    TabOrder = 3
    OnClick = cmdOpenFileClick
  end
  object Log: TMemo
    Left = 0
    Top = 120
    Width = 456
    Height = 246
    Align = alBottom
    TabOrder = 4
  end
  object cmdShowContent: TButton
    Left = 8
    Top = 40
    Width = 105
    Height = 25
    Caption = #1057#1086#1076#1077#1088#1078#1080#1084#1086#1077
    TabOrder = 5
    OnClick = cmdShowContentClick
  end
  object cmdCalcCRC: TButton
    Left = 120
    Top = 40
    Width = 105
    Height = 25
    Caption = #1055#1086#1089#1095#1080#1090#1072#1090#1100' CRC'
    TabOrder = 6
    OnClick = cmdCalcCRCClick
  end
  object rgCrcWidth: TRadioGroup
    Left = 248
    Top = 30
    Width = 185
    Height = 81
    Caption = 'CRC'
    ItemIndex = 0
    Items.Strings = (
      'CRC-8'
      'CRC-16')
    TabOrder = 7
  end
  object openDlg: TOpenDialog
    Left = 360
    Top = 65528
  end
end
