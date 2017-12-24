object FRanks: TFRanks
  Left = 0
  Top = 0
  Width = 507
  Height = 240
  TabOrder = 0
  object dbnMain: TDBNavigator
    Left = 0
    Top = 215
    Width = 507
    Height = 25
    Align = alBottom
    TabOrder = 0
  end
  object tblMain: TDBGrid
    Left = 0
    Top = 0
    Width = 507
    Height = 215
    Align = alClient
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = #1056#1072#1079#1088#1103#1076
        Width = 200
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072'1'#1088
        Width = 75
        Visible = True
      end>
  end
end
