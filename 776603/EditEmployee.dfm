object FEditEmployee: TFEditEmployee
  Left = 1137
  Top = 677
  Width = 627
  Height = 351
  Caption = #1044#1086#1073#1072#1074#1083#1077#1085#1080#1077' '#1089#1086#1090#1088#1091#1076#1085#1080#1082#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 49
    Height = 13
    Caption = #1060#1072#1084#1080#1083#1080#1103
  end
  object Label2: TLabel
    Left = 16
    Top = 48
    Width = 22
    Height = 13
    Caption = #1048#1084#1103
  end
  object Label3: TLabel
    Left = 16
    Top = 80
    Width = 47
    Height = 13
    Caption = #1054#1090#1095#1077#1089#1090#1074#1086
  end
  object Label4: TLabel
    Left = 16
    Top = 112
    Width = 79
    Height = 13
    Caption = #1044#1072#1090#1072' '#1088#1086#1078#1076#1077#1085#1080#1103
  end
  object Label5: TLabel
    Left = 16
    Top = 144
    Width = 61
    Height = 13
    Caption = #1044#1072#1090#1072' '#1085#1072#1081#1084#1072
  end
  object Label6: TLabel
    Left = 16
    Top = 176
    Width = 74
    Height = 13
    Caption = #1058#1080#1087' '#1082#1086#1085#1090#1088#1072#1082#1090#1072
  end
  object Label7: TLabel
    Left = 16
    Top = 208
    Width = 58
    Height = 13
    Caption = #1044#1086#1083#1078#1085#1086#1089#1090#1100
  end
  object Label8: TLabel
    Left = 16
    Top = 240
    Width = 82
    Height = 13
    Caption = #1059#1088#1086#1074#1077#1085#1100' '#1089#1086#1074#1077#1090#1072
  end
  object txtSurname: TEdit
    Left = 112
    Top = 16
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object txtName: TEdit
    Left = 112
    Top = 48
    Width = 121
    Height = 21
    TabOrder = 1
  end
  object txtPatronimic: TEdit
    Left = 112
    Top = 80
    Width = 121
    Height = 21
    TabOrder = 2
  end
  object txtBirthDate: TEdit
    Left = 112
    Top = 112
    Width = 121
    Height = 21
    TabOrder = 3
  end
  object txtHireDate: TEdit
    Left = 112
    Top = 144
    Width = 121
    Height = 21
    TabOrder = 4
  end
  object lvwRanks: TListView
    Left = 264
    Top = 16
    Width = 305
    Height = 65
    Columns = <
      item
        Caption = #1050#1086#1076
        Width = 34
      end
      item
        Caption = #1056#1072#1079#1088#1103#1076
        Width = 120
      end
      item
        Caption = #1053#1072#1076#1073#1072#1074#1082#1072
        Width = 70
      end>
    GridLines = True
    RowSelect = True
    TabOrder = 5
    ViewStyle = vsReport
  end
  object lvwPractice: TListView
    Left = 264
    Top = 104
    Width = 305
    Height = 65
    Columns = <
      item
        Caption = #1050#1086#1076
        Width = 34
      end
      item
        Caption = #1057#1090#1091#1076#1077#1085#1090#1099
        Width = 70
      end
      item
        Caption = #1042#1080#1076' '#1087#1088#1072#1082#1090#1080#1082#1080
        Width = 100
      end
      item
        Caption = #1053#1072#1076#1073#1072#1074#1082#1072
        Width = 70
      end>
    GridLines = True
    RowSelect = True
    TabOrder = 6
    ViewStyle = vsReport
  end
  object ListView1: TListView
    Left = 264
    Top = 192
    Width = 305
    Height = 65
    Columns = <
      item
        Caption = #1050#1086#1076
        Width = 34
      end
      item
        Caption = #1057#1090#1091#1076#1077#1085#1090#1099
        Width = 70
      end
      item
        Caption = #1044#1080#1089#1094#1080#1087#1083#1080#1085#1072
        Width = 100
      end
      item
        Caption = #1053#1072#1076#1073#1072#1074#1082#1072
        Width = 70
      end>
    GridLines = True
    RowSelect = True
    TabOrder = 7
    ViewStyle = vsReport
  end
  object cmdOK: TButton
    Left = 440
    Top = 280
    Width = 75
    Height = 25
    Caption = #1055#1088#1080#1085#1103#1090#1100
    Default = True
    ModalResult = 1
    TabOrder = 8
  end
  object cmdCancel: TButton
    Left = 528
    Top = 280
    Width = 75
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1080#1090#1100
    ModalResult = 2
    TabOrder = 9
  end
  object cbxPost: TDBLookupComboBox
    Left = 112
    Top = 208
    Width = 145
    Height = 21
    KeyField = #1050#1086#1076#1044#1086#1083#1078#1085#1086#1089#1090#1100
    ListField = #1044#1086#1083#1078#1085#1086#1089#1090#1100
    ListSource = srcPost
    TabOrder = 10
  end
  object cbxContract: TDBLookupComboBox
    Left = 112
    Top = 176
    Width = 145
    Height = 21
    KeyField = #1050#1086#1076#1050#1086#1085#1090#1088#1072#1082#1090#1072
    ListField = #1053#1072#1079#1074#1072#1085#1080#1077
    ListSource = srcContract
    TabOrder = 11
  end
  object cbxCouncil: TDBLookupComboBox
    Left = 112
    Top = 232
    Width = 145
    Height = 21
    KeyField = #1050#1086#1076#1059#1052#1057
    ListField = #1052#1077#1090#1086#1076#1080#1095#1077#1089#1082#1080#1081' '#1089#1086#1074#1077#1090
    ListSource = srcCouncil
    TabOrder = 12
  end
  object adoEmployee: TADOQuery
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      
        'SELECT '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1060#1072#1084#1080#1083#1080#1103', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1048#1084#1103',' +
        ' '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1054#1090#1095#1077#1089#1090#1074#1086', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1044#1072#1090#1072#1056#1086#1078#1076#1077#1085#1080#1103', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1044#1072#1090#1072#1053#1072#1081#1084#1072 +
        ', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1050#1086#1085#1090#1088#1072#1082#1090#1072', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1044#1086#1083#1078#1085#1086#1089#1090#1080', '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1059 +
        #1052#1057', ['#1058#1080#1087' '#1082#1086#1085#1090#1088#1072#1082#1090#1072'].'#1053#1072#1079#1074#1072#1085#1080#1077', '#1044#1086#1083#1078#1085#1086#1089#1090#1100'.'#1044#1086#1083#1078#1085#1086#1089#1090#1100', ['#1059#1088#1086#1074#1077#1085#1100' '#1089#1086#1074#1077 +
        #1090#1072'].['#1052#1077#1090#1086#1076#1080#1095#1077#1089#1082#1080#1081' '#1089#1086#1074#1077#1090']'
      
        'FROM ['#1059#1088#1086#1074#1077#1085#1100' '#1089#1086#1074#1077#1090#1072'] INNER JOIN (['#1058#1080#1087' '#1082#1086#1085#1090#1088#1072#1082#1090#1072'] INNER JOIN ('#1044#1086 +
        #1083#1078#1085#1086#1089#1090#1100' INNER JOIN '#1057#1086#1090#1088#1091#1076#1085#1080#1082' ON '#1044#1086#1083#1078#1085#1086#1089#1090#1100'.'#1050#1086#1076#1044#1086#1083#1078#1085#1086#1089#1090#1100' = '#1057#1086#1090#1088#1091#1076#1085 +
        #1080#1082'.'#1050#1086#1076#1044#1086#1083#1078#1085#1086#1089#1090#1080') ON ['#1058#1080#1087' '#1082#1086#1085#1090#1088#1072#1082#1090#1072'].'#1050#1086#1076#1050#1086#1085#1090#1088#1072#1082#1090#1072' = '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076 +
        #1050#1086#1085#1090#1088#1072#1082#1090#1072') ON ['#1059#1088#1086#1074#1077#1085#1100' '#1089#1086#1074#1077#1090#1072'].'#1050#1086#1076#1059#1052#1057' = '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1059#1052#1057
      'WHERE '#1057#1086#1090#1088#1091#1076#1085#1080#1082'.'#1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082' = 4')
    Left = 192
    Top = 752
  end
  object srcEmployee: TDataSource
    DataSet = adoEmployee
    Left = 192
    Top = 712
  end
  object adoContract: TADOTable
    Active = True
    Connection = FMain.adoConn
    CursorType = ctStatic
    TableName = #1058#1080#1087' '#1082#1086#1085#1090#1088#1072#1082#1090#1072
    Left = 16
    Top = 264
  end
  object adoPost: TADOTable
    Active = True
    Connection = FMain.adoConn
    CursorType = ctStatic
    TableName = #1044#1086#1083#1078#1085#1086#1089#1090#1100
    Left = 56
    Top = 264
  end
  object adoCouncil: TADOTable
    Active = True
    Connection = FMain.adoConn
    CursorType = ctStatic
    TableName = #1059#1088#1086#1074#1077#1085#1100' '#1089#1086#1074#1077#1090#1072
    Left = 96
    Top = 264
  end
  object srcCouncil: TDataSource
    DataSet = adoCouncil
    Left = 240
    Top = 264
  end
  object srcPost: TDataSource
    DataSet = adoPost
    Left = 200
    Top = 264
  end
  object srcContract: TDataSource
    DataSet = adoContract
    Left = 160
    Top = 264
  end
end
