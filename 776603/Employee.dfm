object FEmployee: TFEmployee
  Left = 0
  Top = 0
  Width = 304
  Height = 595
  TabOrder = 0
  object txtID: TDBEdit
    Left = 256
    Top = 8
    Width = 33
    Height = 21
    DataField = #1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082
    DataSource = FMain.dataSource
    TabOrder = 0
    Visible = False
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 105
    Width = 293
    Height = 112
    Caption = #1055#1086#1076#1075#1086#1090#1086#1074#1082#1072' '#1088#1072#1079#1088#1103#1076#1085#1080#1082#1086#1074
    TabOrder = 1
    object tblPersRanks: TDBGrid
      Left = 9
      Top = 24
      Width = 272
      Height = 81
      DataSource = FMain.srcPersRanks
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = #1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082
          Width = 30
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1050#1086#1076#1056#1058
          Width = 30
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1056#1072#1079#1088#1103#1076
          Width = 80
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1053#1072#1076#1073#1072#1074#1082#1072'1'#1088
          Visible = True
        end>
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 224
    Width = 293
    Height = 145
    Caption = #1056#1091#1082#1086#1074#1086#1076#1089#1090#1074#1086' '#1087#1088#1072#1082#1090#1080#1082#1086#1081
    TabOrder = 2
    object tblPersPractice: TDBGrid
      Left = 13
      Top = 24
      Width = 268
      Height = 105
      DataSource = FMain.srcPersPractice
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = #1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082
          Width = 0
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1050#1086#1076#1042#1055
          Width = 0
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1057#1090#1091#1076#1077#1085#1090#1086#1074
          Width = 60
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1042#1080#1076#1055#1088#1072#1082#1090
          Width = 120
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1053#1072#1076#1073#1072#1074#1082#1072
          Width = 60
          Visible = True
        end>
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 376
    Width = 289
    Height = 137
    Caption = #1055#1088#1086#1074#1077#1088#1082#1072' '#1088#1072#1073#1086#1090
    TabOrder = 3
    object tblPersSubj: TDBGrid
      Left = 9
      Top = 25
      Width = 264
      Height = 96
      DataSource = FMain.srcPersSubj
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = #1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082
          Width = 0
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1050#1086#1076#1042#1047
          Width = 0
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1057#1090#1091#1076#1077#1085#1090#1099
          Width = 60
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1044#1080#1089#1094#1080#1087#1083#1080#1085#1072
          Width = 70
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1053#1072#1076#1073#1072#1074#1082#1072
          Width = 60
          Visible = True
        end>
    end
  end
  object txtPost: TDBEdit
    Left = 16
    Top = 80
    Width = 121
    Height = 21
    DataField = #1044#1086#1083#1078#1085#1086#1089#1090#1100
    DataSource = FMain.srcEmployee
    ReadOnly = True
    TabOrder = 4
  end
  object txtSurname: TDBEdit
    Left = 16
    Top = 8
    Width = 121
    Height = 21
    DataField = #1060#1072#1084#1080#1083#1080#1103
    DataSource = FMain.srcEmployee
    ReadOnly = True
    TabOrder = 5
  end
  object txtName: TDBEdit
    Left = 16
    Top = 32
    Width = 121
    Height = 21
    DataField = #1048#1084#1103
    DataSource = FMain.srcEmployee
    ReadOnly = True
    TabOrder = 6
  end
  object txtPatronimic: TDBEdit
    Left = 16
    Top = 56
    Width = 121
    Height = 21
    DataField = #1054#1090#1095#1077#1089#1090#1074#1086
    DataSource = FMain.srcEmployee
    ReadOnly = True
    TabOrder = 7
  end
  object txtBirthDate: TDBEdit
    Left = 144
    Top = 8
    Width = 121
    Height = 21
    DataField = #1044#1072#1090#1072#1056#1086#1078#1076#1077#1085#1080#1103
    DataSource = FMain.srcEmployee
    TabOrder = 8
  end
  object txtHireDate: TDBEdit
    Left = 144
    Top = 32
    Width = 121
    Height = 21
    DataField = #1044#1072#1090#1072#1053#1072#1081#1084#1072
    DataSource = FMain.srcEmployee
    TabOrder = 9
  end
  object txtContract: TDBEdit
    Left = 144
    Top = 56
    Width = 121
    Height = 21
    DataField = #1053#1072#1079#1074#1072#1085#1080#1077
    DataSource = FMain.srcEmployee
    TabOrder = 10
  end
  object txtCouncil: TDBEdit
    Left = 144
    Top = 80
    Width = 121
    Height = 21
    DataField = #1052#1077#1090#1086#1076#1080#1095#1077#1089#1082#1080#1081' '#1089#1086#1074#1077#1090
    DataSource = FMain.srcEmployee
    ReadOnly = True
    TabOrder = 11
  end
end
