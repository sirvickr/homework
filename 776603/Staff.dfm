object FStaff: TFStaff
  Left = 0
  Top = 0
  Width = 925
  Height = 685
  Align = alClient
  TabOrder = 0
  object pnlMain: TPanel
    Left = 0
    Top = 0
    Width = 632
    Height = 685
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object dbnMain: TDBNavigator
      Left = 0
      Top = 660
      Width = 632
      Height = 25
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbDelete, nbRefresh]
      Align = alBottom
      TabOrder = 0
    end
    object tblMain: TDBGrid
      Left = 0
      Top = 0
      Width = 632
      Height = 660
      Align = alClient
      ReadOnly = True
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = #1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082
          Width = 45
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1060#1048#1054
          Width = 120
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1044#1086#1083#1078#1085#1086#1089#1090#1100
          Width = 120
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1044#1072#1090#1072#1056#1086#1078#1076#1077#1085#1080#1103
          Width = 85
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1042#1086#1079#1088#1072#1089#1090
          Width = 50
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1044#1072#1090#1072#1053#1072#1081#1084#1072
          Width = 75
          Visible = True
        end
        item
          Expanded = False
          FieldName = #1057#1090#1072#1078
          Width = 40
          Visible = True
        end>
    end
  end
  object pnlDetails: TPanel
    Left = 632
    Top = 0
    Width = 293
    Height = 685
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 1
    object txtPerson: TDBText
      Left = 8
      Top = 8
      Width = 225
      Height = 25
      DataField = #1060#1048#1054
      DataSource = FMain.dataSource
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clTeal
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object GroupBox1: TGroupBox
      Left = 0
      Top = 41
      Width = 293
      Height = 112
      Caption = #1055#1086#1076#1075#1086#1090#1086#1074#1082#1072' '#1088#1072#1079#1088#1103#1076#1085#1080#1082#1086#1074
      TabOrder = 0
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
      Left = 0
      Top = 160
      Width = 293
      Height = 145
      Caption = #1056#1091#1082#1086#1074#1086#1076#1089#1090#1074#1086' '#1087#1088#1072#1082#1090#1080#1082#1086#1081
      TabOrder = 1
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
    object txtID: TDBEdit
      Left = 248
      Top = 8
      Width = 33
      Height = 21
      DataField = #1050#1086#1076#1057#1086#1090#1088#1091#1076#1085#1080#1082
      DataSource = FMain.dataSource
      TabOrder = 2
      Visible = False
      OnChange = txtIDChange
    end
    object GroupBox3: TGroupBox
      Left = 0
      Top = 312
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
  end
end
