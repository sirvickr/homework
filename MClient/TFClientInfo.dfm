object FClientInfo: TFClientInfo
  Left = 220
  Top = 129
  Width = 547
  Height = 509
  Caption = 'FClientInfo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GBUserID: TGroupBox
    Left = 2
    Top = 1
    Width = 535
    Height = 64
    TabOrder = 0
    object Label1: TLabel
      Left = 9
      Top = 12
      Width = 92
      Height = 13
      Caption = #1053#1086#1084#1077#1088' '#1082#1083#1080#1077#1085#1090#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 132
      Top = 12
      Width = 26
      Height = 13
      Caption = #1048#1084#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label3: TLabel
      Left = 268
      Top = 12
      Width = 45
      Height = 13
      Caption = #1055#1072#1088#1086#1083#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object CboUserID: TComboBox
      Left = 8
      Top = 28
      Width = 105
      Height = 27
      AutoComplete = False
      AutoDropDown = True
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ItemHeight = 19
      ParentFont = False
      TabOrder = 0
      OnClick = CboUserIDClick
      OnKeyPress = CboUserIDKeyPress
    end
    object EdName: TEdit
      Left = 132
      Top = 28
      Width = 121
      Height = 27
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
    object EdPwd: TEdit
      Left = 268
      Top = 28
      Width = 121
      Height = 27
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
    end
    object ChkLocked: TCheckBox
      Left = 400
      Top = 28
      Width = 133
      Height = 17
      Caption = #1047#1072#1073#1083#1086#1082#1080#1088#1086#1074#1072#1085
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
    end
  end
  object GBUserProperties: TGroupBox
    Left = 2
    Top = 65
    Width = 535
    Height = 176
    TabOrder = 1
    object Label4: TLabel
      Left = 10
      Top = 16
      Width = 126
      Height = 13
      Caption = #1044#1072#1090#1072' '#1080#1085#1080#1094#1080#1072#1083#1080#1079#1072#1094#1080#1080
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label5: TLabel
      Left = 10
      Top = 48
      Width = 94
      Height = 13
      Caption = #1058#1072#1088#1080#1092#1085#1099#1081' '#1087#1083#1072#1085
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label6: TLabel
      Left = 10
      Top = 80
      Width = 159
      Height = 13
      Caption = #1054#1089#1090#1072#1090#1086#1082' '#1074#1088#1077#1084#1077#1085#1080' '#1085#1072' '#1089#1095#1077#1090#1091
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label7: TLabel
      Left = 10
      Top = 112
      Width = 92
      Height = 13
      Caption = #1058#1077#1082#1091#1097#1080#1081' '#1089#1077#1072#1085#1089
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label8: TLabel
      Left = 10
      Top = 145
      Width = 104
      Height = 13
      Caption = #1044#1077#1081#1089#1090#1074#1080#1090#1077#1083#1077#1085' '#1076#1086
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object EdDateInit: TEdit
      Left = 187
      Top = 16
      Width = 341
      Height = 23
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object EdTarif: TEdit
      Left = 187
      Top = 48
      Width = 341
      Height = 23
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
    object EdTimeNowOnline: TEdit
      Left = 187
      Top = 80
      Width = 341
      Height = 23
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
    end
    object EdCurrentSession: TEdit
      Left = 187
      Top = 112
      Width = 341
      Height = 23
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
    end
    object EdEndByInit: TEdit
      Left = 187
      Top = 144
      Width = 341
      Height = 23
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
    end
  end
end
