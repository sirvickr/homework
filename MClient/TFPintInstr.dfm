object FPintInstr: TFPintInstr
  Left = 450
  Top = 215
  BorderStyle = bsDialog
  Caption = 'FPintInstr'
  ClientHeight = 100
  ClientWidth = 333
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object RGDocType: TRadioGroup
    Left = 4
    Top = 4
    Width = 109
    Height = 64
    Caption = #1058#1080#1087' '#1076#1086#1082#1091#1084#1077#1085#1090#1072
    TabOrder = 0
  end
  object RbInstruction: TRadioButton
    Left = 12
    Top = 19
    Width = 94
    Height = 17
    Caption = #1048#1085#1089#1090#1088#1091#1082#1094#1080#1103
    TabOrder = 1
  end
  object RbEnvelope: TRadioButton
    Left = 12
    Top = 39
    Width = 94
    Height = 17
    Caption = #1050#1086#1085#1074#1077#1088#1090
    TabOrder = 2
  end
  object GBNumbers: TGroupBox
    Left = 122
    Top = 4
    Width = 207
    Height = 64
    Caption = #1053#1086#1084#1077#1088#1072' '#1082' '#1087#1077#1095#1072#1090#1080
    TabOrder = 3
    object ChkMin: TCheckBox
      Left = 8
      Top = 15
      Width = 85
      Height = 17
      Caption = #1053#1072#1095#1072#1083#1100#1085#1099#1081
      TabOrder = 0
    end
    object ChkMax: TCheckBox
      Left = 8
      Top = 37
      Width = 85
      Height = 17
      Caption = #1050#1086#1085#1077#1095#1085#1099#1081
      TabOrder = 1
    end
    object CboMin: TComboBox
      Left = 104
      Top = 12
      Width = 96
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 2
    end
    object CboMax: TComboBox
      Left = 104
      Top = 36
      Width = 97
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 3
    end
  end
  object BTemplate: TButton
    Left = 4
    Top = 72
    Width = 109
    Height = 25
    Caption = #1064#1072#1073#1083#1086#1085' '#1082#1086#1085#1074#1077#1088#1090#1072'...'
    TabOrder = 4
  end
  object BPrint: TButton
    Left = 252
    Top = 72
    Width = 75
    Height = 25
    Caption = #1055#1077#1095#1072#1090#1100
    TabOrder = 5
    OnClick = BPrintClick
  end
end
