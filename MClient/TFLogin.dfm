object FLogin: TFLogin
  Left = 192
  Top = 107
  BorderStyle = bsDialog
  Caption = #1044#1086#1089#1090#1091#1087
  ClientHeight = 243
  ClientWidth = 178
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
  object GBAccessCode: TGroupBox
    Left = 2
    Top = 2
    Width = 173
    Height = 72
    Caption = #1042#1074#1077#1076#1080#1090#1077' '#1082#1086#1076' '#1076#1086#1089#1090#1091#1087#1072
    TabOrder = 0
    object EdtCode: TEdit
      Left = 8
      Top = 16
      Width = 157
      Height = 21
      MaxLength = 1
      PasswordChar = '*'
      TabOrder = 0
      Text = 'z'
      OnKeyPress = EdtCodeKeyPress
    end
    object BNewCode: TButton
      Left = 8
      Top = 42
      Width = 75
      Height = 22
      Caption = #1053#1086#1074#1099#1081' '#1082#1086#1076
      TabOrder = 1
      Visible = False
    end
    object BContinue: TButton
      Left = 92
      Top = 42
      Width = 75
      Height = 22
      Caption = #1055#1088#1086#1076#1086#1083#1078#1080#1090#1100
      TabOrder = 2
      Visible = False
      OnClick = BContinueClick
    end
  end
  object GBDBAccess: TGroupBox
    Left = 2
    Top = 80
    Width = 173
    Height = 133
    Caption = #1044#1086#1089#1090#1091#1087' '#1082' '#1073#1072#1079#1077' '#1076#1072#1085#1085#1099#1093
    TabOrder = 1
    object Label1: TLabel
      Left = 7
      Top = 20
      Width = 37
      Height = 13
      Caption = #1057#1077#1088#1074#1077#1088
    end
    object Label2: TLabel
      Left = 7
      Top = 46
      Width = 31
      Height = 13
      Caption = #1051#1086#1075#1080#1085
    end
    object Label3: TLabel
      Left = 7
      Top = 76
      Width = 38
      Height = 13
      Caption = #1055#1072#1088#1086#1083#1100
    end
    object EdServerName: TEdit
      Left = 51
      Top = 19
      Width = 114
      Height = 22
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PasswordChar = '@'
      TabOrder = 0
      Text = 'COUNTER'
    end
    object EdLogin: TEdit
      Left = 51
      Top = 47
      Width = 114
      Height = 21
      TabOrder = 1
      Text = 'sysdba'
    end
    object EdPwd: TEdit
      Left = 51
      Top = 75
      Width = 114
      Height = 21
      PasswordChar = '*'
      TabOrder = 2
      Text = 'LoAD76V'
    end
    object BConnect: TButton
      Left = 7
      Top = 101
      Width = 159
      Height = 25
      Caption = #1057#1086#1077#1076#1080#1085#1077#1085#1080#1077
      TabOrder = 3
      OnClick = BConnectClick
    end
  end
  object BExit: TButton
    Left = 100
    Top = 217
    Width = 75
    Height = 23
    Cancel = True
    Caption = #1042#1099#1093#1086#1076
    TabOrder = 2
    OnClick = BExitClick
  end
end
