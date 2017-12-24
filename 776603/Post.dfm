object FPost: TFPost
  Left = 0
  Top = 0
  Width = 825
  Height = 432
  Align = alClient
  TabOrder = 0
  object tblMain: TDBGrid
    Left = 0
    Top = 0
    Width = 825
    Height = 407
    Align = alClient
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = #1044#1086#1083#1078#1085#1086#1089#1090#1100
        Width = 100
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1054#1082#1083#1072#1076
        Visible = True
      end>
  end
  object dbnMain: TDBNavigator
    Left = 0
    Top = 407
    Width = 825
    Height = 25
    Align = alBottom
    TabOrder = 1
  end
end
