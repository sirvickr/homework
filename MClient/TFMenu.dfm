object FMainMenu: TFMainMenu
  Left = 197
  Top = 129
  Width = 789
  Height = 97
  Caption = #1057#1083#1091#1078#1073#1072' '#1090#1077#1093#1085#1080#1095#1077#1089#1082#1086#1081' '#1087#1086#1076#1076#1077#1088#1078#1082#1080' '#1048#1085#1090#1077#1088#1085#1077#1090
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SbrMain: TStatusBar
    Left = 0
    Top = 32
    Width = 781
    Height = 19
    Panels = <
      item
        Width = 200
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object PbrMain: TProgressBar
    Left = 92
    Top = 8
    Width = 150
    Height = 16
    Min = 0
    Max = 100
    Smooth = True
    Step = 1
    TabOrder = 1
  end
  object MainMenu: TMainMenu
    Top = 65520
    object MnuClients: TMenuItem
      Caption = #1050#1083#1080#1077#1085#1090#1099
      object MnuClientsInfo: TMenuItem
        Caption = #1057#1074#1077#1076#1077#1085#1080#1103' '#1086' '#1082#1083#1080#1077#1085#1090#1077
        OnClick = MnuClientsInfoClick
      end
      object MnuClientsSeries: TMenuItem
        Caption = #1057#1086#1079#1076#1072#1085#1080#1077' '#1085#1086#1074#1099#1093' '#1089#1077#1088#1080#1081
        OnClick = MnuClientsSeriesClick
      end
      object MnuClientsStore: TMenuItem
        Caption = #1055#1077#1088#1077#1076#1072#1095#1072' '#1089#1077#1088#1080#1081' '#1085#1072' '#1089#1082#1083#1072#1076
      end
      object MnuClientsSep1: TMenuItem
        Caption = '-'
      end
      object MnuClientsExit: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = MnuClientsExitClick
      end
    end
    object MnuDealers: TMenuItem
      Caption = #1044#1080#1083#1077#1088#1099
    end
    object N3: TMenuItem
      Caption = #1057#1087#1088#1072#1074#1082#1080
    end
    object N4: TMenuItem
      Caption = #1057#1090#1072#1090#1080#1089#1090#1080#1082#1072
    end
    object N5: TMenuItem
      Caption = #1057#1087#1088#1072#1074#1086#1095#1085#1080#1082#1080
    end
    object N6: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072
    end
  end
end
