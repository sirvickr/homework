object FPractice: TFPractice
  Left = 0
  Top = 0
  Width = 832
  Height = 437
  Align = alTop
  TabOrder = 0
  object dbnMain: TDBNavigator
    Left = 0
    Top = 412
    Width = 832
    Height = 25
    Align = alBottom
    TabOrder = 0
  end
  object tblMain: TDBGrid
    Left = 0
    Top = 0
    Width = 832
    Height = 412
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
        FieldName = #1042#1080#1076#1055#1088#1072#1082#1090
        Width = 200
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072
        Width = 70
        Visible = True
      end>
  end
end
