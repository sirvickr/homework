object FMain: TFMain
  Left = 306
  Top = 100
  BorderStyle = bsDialog
  Caption = #1043#1083#1072#1074#1085#1072#1103' '#1082#1085#1086#1087#1086#1095#1085#1072#1103' '#1092#1086#1088#1084#1072
  ClientHeight = 601
  ClientWidth = 1216
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object cmdExit: TSpeedButton
    Left = 64
    Top = 488
    Width = 57
    Height = 57
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00330000000000
      03333377777777777F333301BBBBBBBB033333773F3333337F3333011BBBBBBB
      0333337F73F333337F33330111BBBBBB0333337F373F33337F333301110BBBBB
      0333337F337F33337F333301110BBBBB0333337F337F33337F333301110BBBBB
      0333337F337F33337F333301110BBBBB0333337F337F33337F333301110BBBBB
      0333337F337F33337F333301110BBBBB0333337F337FF3337F33330111B0BBBB
      0333337F337733337F333301110BBBBB0333337F337F33337F333301110BBBBB
      0333337F3F7F33337F333301E10BBBBB0333337F7F7F33337F333301EE0BBBBB
      0333337F777FFFFF7F3333000000000003333377777777777333}
    NumGlyphs = 2
    OnClick = cmdExitClick
  end
  object lblBack: TLabel
    Left = 96
    Top = 448
    Width = 32
    Height = 13
    Caption = #1053#1072#1079#1072#1076
    Enabled = False
  end
  object cmdBack: TButton
    Left = 64
    Top = 448
    Width = 25
    Height = 25
    Enabled = False
    TabOrder = 0
    OnClick = cmdBackClick
  end
  object pnlSubForm: TPanel
    Left = 0
    Top = 0
    Width = 1216
    Height = 433
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object Label1: TLabel
      Left = 96
      Top = 52
      Width = 59
      Height = 13
      Caption = #1057#1086#1090#1088#1091#1076#1085#1080#1082#1080
    end
    object Label2: TLabel
      Left = 96
      Top = 88
      Width = 66
      Height = 13
      Caption = #1057#1087#1088#1072#1074#1086#1095#1085#1080#1082#1080
    end
    object Label3: TLabel
      Left = 96
      Top = 128
      Width = 37
      Height = 13
      Caption = #1054#1090#1095#1077#1090#1099
    end
    object cmdStaff: TButton
      Left = 64
      Top = 48
      Width = 25
      Height = 25
      TabOrder = 0
      OnClick = cmdStaffClick
    end
    object cmdDir: TButton
      Left = 64
      Top = 88
      Width = 25
      Height = 25
      TabOrder = 1
      OnClick = cmdDirClick
    end
    object cmdReports: TButton
      Left = 64
      Top = 128
      Width = 25
      Height = 25
      TabOrder = 2
      OnClick = cmdReportsClick
    end
  end
  object adoConn: TADOConnection
    Connected = True
    ConnectionString = 
      'Provider=Microsoft.ACE.OLEDB.12.0;Data Source=main.accdb;Persist' +
      ' Security Info=False'
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'Microsoft.ACE.OLEDB.12.0'
    Left = 248
    Top = 536
  end
  object dataSource: TDataSource
    DataSet = adoTable
    Left = 248
    Top = 456
  end
  object adoPracticeQuery: TADOQuery
    Active = True
    Connection = adoConn
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      
        'SELECT ['#1057#1086#1090#1088#1091#1076#1085#1080#1082'].'#1060#1072#1084#1080#1083#1080#1103' + '#39' '#39' + ['#1057#1086#1090#1088#1091#1076#1085#1080#1082'].'#1048#1084#1103' + '#39' '#39' + ['#1057#1086#1090#1088 +
        #1091#1076#1085#1080#1082'].'#1054#1090#1095#1077#1089#1090#1074#1086' AS '#1057#1086#1090#1088#1091#1076#1085#1080#1082', ['#1042#1080#1076' '#1087#1088#1072#1082#1090#1080#1082#1080'].'#1042#1080#1076#1055#1088#1072#1082#1090' AS ['#1055#1088#1072#1082#1090#1080 +
        #1082#1072'], ['#1056#1091#1082#1086#1074#1086#1076#1089#1090#1074#1086' '#1087#1088#1072#1082#1090#1080#1082#1086#1081'].'#1063#1080#1089#1083#1086#1057#1090#1091#1076#1077#1085#1090#1086#1074
      
        'FROM ['#1042#1080#1076' '#1087#1088#1072#1082#1090#1080#1082#1080'] INNER JOIN ('#1057#1086#1090#1088#1091#1076#1085#1080#1082' INNER JOIN ['#1056#1091#1082#1086#1074#1086#1076#1089#1090#1074 +
        #1086' '#1087#1088#1072#1082#1090#1080#1082#1086#1081'] ON '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082' = ['#1056#1091#1082#1086#1074#1086#1076#1089#1090#1074#1086' '#1087#1088#1072#1082#1090#1080#1082#1086#1081']' +
        '.'#1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082') ON ['#1042#1080#1076' '#1087#1088#1072#1082#1090#1080#1082#1080'].'#1050#1086#1076#1042#1055' = ['#1056#1091#1082#1086#1074#1086#1076#1089#1090#1074#1086' '#1087#1088#1072#1082#1090#1080#1082#1086#1081']' +
        '.'#1050#1086#1076#1042#1055)
    Left = 320
    Top = 472
  end
  object adoTable: TADOTable
    Connection = adoConn
    CursorType = ctStatic
    TableName = #1050#1072#1076#1088#1099
    Left = 248
    Top = 496
  end
  object adoPersRanks: TADOQuery
    Connection = adoConn
    CursorType = ctStatic
    Parameters = <>
    Left = 360
    Top = 472
  end
  object srcPersRanks: TDataSource
    DataSet = adoPersRanks
    Left = 400
    Top = 472
  end
  object adoPersPractice: TADOQuery
    Connection = adoConn
    CursorType = ctStatic
    Parameters = <>
    Left = 360
    Top = 512
  end
  object srcPersPractice: TDataSource
    DataSet = adoPersPractice
    Left = 400
    Top = 512
  end
  object adoPersSubj: TADOQuery
    Connection = adoConn
    CursorType = ctStatic
    Parameters = <>
    Left = 360
    Top = 552
  end
  object srcPersSubj: TDataSource
    DataSet = adoPersSubj
    Left = 400
    Top = 552
  end
  object tbl: TADOTable
    Active = True
    Connection = adoConn
    CursorType = ctStatic
    TableName = #1050#1072#1076#1088#1099
    Left = 504
    Top = 464
  end
  object src: TDataSource
    DataSet = tbl
    Left = 544
    Top = 464
  end
end
