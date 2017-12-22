object FSeries: TFSeries
  Left = 447
  Top = 234
  BorderStyle = bsDialog
  Caption = 'FSeries'
  ClientHeight = 147
  ClientWidth = 318
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 6
    Top = 4
    Width = 54
    Height = 13
    Caption = #1058#1080#1087' '#1082#1072#1088#1090
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 6
    Top = 56
    Width = 78
    Height = 13
    Caption = #1058#1088#1077#1073#1091#1077#1090#1089#1103' ('#1096#1090'.)'
  end
  object CboProfile: TDBLookupComboBox
    Left = 6
    Top = 24
    Width = 305
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    KeyField = 'profile_cod'
    ListField = 'profile_description'
    ListSource = DSProfile
    ParentFont = False
    TabOrder = 0
    OnCloseUp = CboProfileCloseUp
  end
  object EdQuant: TEdit
    Left = 90
    Top = 52
    Width = 45
    Height = 21
    TabOrder = 1
  end
  object BPrint: TButton
    Left = 224
    Top = 90
    Width = 87
    Height = 25
    Caption = #1055#1077#1095#1072#1090#1100'...'
    TabOrder = 2
  end
  object BGenerate: TButton
    Left = 224
    Top = 52
    Width = 87
    Height = 25
    Caption = #1057#1092#1086#1088#1084#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 3
    OnClick = BGenerateClick
  end
  object GroupBox1: TGroupBox
    Left = 6
    Top = 84
    Width = 209
    Height = 57
    Caption = #1053#1086#1074#1099#1077' '#1085#1086#1084#1077#1088#1072
    TabOrder = 4
    object EdMin: TEdit
      Left = 8
      Top = 24
      Width = 95
      Height = 21
      TabOrder = 0
    end
    object EdMax: TEdit
      Left = 106
      Top = 24
      Width = 95
      Height = 21
      TabOrder = 1
    end
  end
  object DSProfile: TDataSource
    DataSet = FDB.FSeries_CboProfile
    Left = 94
    Top = 65512
  end
end
