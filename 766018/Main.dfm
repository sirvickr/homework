object FMain: TFMain
  Left = 187
  Top = 174
  Width = 297
  Height = 577
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
    Width = 108
    Height = 13
    Caption = #1059#1075#1086#1083' '#1074#1093#1086#1078#1076#1077#1085#1080#1103' '#1083#1091#1095#1072
  end
  object Label1: TLabel
    Left = 8
    Top = 514
    Width = 90
    Height = 13
    Caption = #1059#1075#1086#1083' '#1074#1099#1093#1086#1076#1072' '#1083#1091#1095#1072
  end
  object Label3: TLabel
    Left = 157
    Top = 514
    Width = 69
    Height = 13
    Caption = #1057#1084#1077#1097#1077#1085#1080#1077' (L)'
  end
  object Label4: TLabel
    Left = 24
    Top = 24
    Width = 32
    Height = 13
    Caption = 'Label4'
  end
  object pnlConfig: TPanel
    Left = 8
    Top = 48
    Width = 265
    Height = 441
    TabOrder = 0
  end
  object txtAlpha: TEdit
    Left = 136
    Top = 8
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '30.5'
    OnChange = txtAlphaChange
  end
  object cmdView: TButton
    Left = 192
    Top = 8
    Width = 75
    Height = 33
    Caption = #1055#1088#1086#1089#1084#1086#1090#1088
    TabOrder = 2
    OnClick = cmdViewClick
  end
  object txtBeta: TEdit
    Left = 104
    Top = 512
    Width = 41
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 3
    Text = '0.0'
  end
  object txtL: TEdit
    Left = 232
    Top = 512
    Width = 41
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 4
    Text = '0.0'
  end
end
