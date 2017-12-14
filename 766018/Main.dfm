object FMain: TFMain
  Left = 76
  Top = 152
  Width = 211
  Height = 769
  Caption = #1055#1088#1077#1083#1086#1084#1083#1077#1085#1080#1077
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
  object Label2: TLabel
    Left = 16
    Top = 8
    Width = 83
    Height = 13
    Caption = #1059#1075#1086#1083' '#1074#1093#1086#1078#1076#1077#1085#1080#1103
  end
  object Label1: TLabel
    Left = 8
    Top = 632
    Width = 65
    Height = 13
    Caption = #1059#1075#1086#1083' '#1074#1099#1093#1086#1076#1072
  end
  object Label3: TLabel
    Left = 8
    Top = 656
    Width = 54
    Height = 13
    Caption = #1057#1084#1077#1097#1077#1085#1080#1077
  end
  object pnlConfig: TPanel
    Left = 8
    Top = 32
    Width = 177
    Height = 593
    TabOrder = 0
  end
  object txtAlpha: TEdit
    Left = 112
    Top = 8
    Width = 73
    Height = 21
    TabOrder = 1
    Text = '0.0'
  end
  object cmdView: TButton
    Left = 112
    Top = 696
    Width = 75
    Height = 25
    Caption = #1055#1088#1086#1089#1084#1086#1090#1088
    TabOrder = 2
    OnClick = cmdViewClick
  end
  object txtBeta: TEdit
    Left = 104
    Top = 632
    Width = 73
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 3
    Text = '0.0'
  end
  object txtL: TEdit
    Left = 104
    Top = 656
    Width = 73
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 4
    Text = '0.0'
  end
end
