object FPayments: TFPayments
  Left = 0
  Top = 0
  Width = 861
  Height = 771
  TabOrder = 0
  object qrMain: TQuickRep
    Left = 8
    Top = 152
    Width = 794
    Height = 1123
    Frame.Color = clBlack
    Frame.DrawTop = False
    Frame.DrawBottom = False
    Frame.DrawLeft = False
    Frame.DrawRight = False
    DataSet = FMain.adoTable
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    Functions.Strings = (
      'PAGENUMBER'
      'COLUMNNUMBER'
      'REPORTTITLE')
    Functions.DATA = (
      '0'
      '0'
      #39#39)
    Options = [FirstPageHeader, LastPageFooter]
    Page.Columns = 1
    Page.Orientation = poPortrait
    Page.PaperSize = Default
    Page.Values = (
      100
      2970
      100
      2100
      100
      100
      0)
    PrinterSettings.Copies = 1
    PrinterSettings.Duplex = False
    PrinterSettings.FirstPage = 0
    PrinterSettings.LastPage = 0
    PrinterSettings.OutputBin = Auto
    PrintIfEmpty = True
    SnapToGrid = True
    Units = MM
    Zoom = 100
    object DetailBand1: TQRBand
      Left = 38
      Top = 38
      Width = 718
      Height = 40
      Frame.Color = clBlack
      Frame.DrawTop = False
      Frame.DrawBottom = False
      Frame.DrawLeft = False
      Frame.DrawRight = False
      AlignToBottom = False
      Color = clWhite
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ForceNewColumn = False
      ForceNewPage = False
      ParentFont = False
      Size.Values = (
        105.833333333333
        1899.70833333333)
      BandType = rbDetail
      object QRDBText1: TQRDBText
        Left = 0
        Top = 8
        Width = 26
        Height = 17
        Frame.Color = clBlack
        Frame.DrawTop = False
        Frame.DrawBottom = False
        Frame.DrawLeft = False
        Frame.DrawRight = False
        Size.Values = (
          44.9791666666667
          0
          21.1666666666667
          68.7916666666667)
        Alignment = taLeftJustify
        AlignToBand = False
        AutoSize = True
        AutoStretch = False
        Color = clWhite
        DataSet = FMain.adoTable
        DataField = #1060#1048#1054
        Transparent = False
        WordWrap = True
        FontSize = 8
      end
      object QRDBText2: TQRDBText
        Left = 64
        Top = 8
        Width = 32
        Height = 15
        Frame.Color = clBlack
        Frame.DrawTop = False
        Frame.DrawBottom = False
        Frame.DrawLeft = False
        Frame.DrawRight = False
        Size.Values = (
          39.6875
          169.333333333333
          21.1666666666667
          84.6666666666667)
        Alignment = taLeftJustify
        AlignToBand = False
        AutoSize = True
        AutoStretch = False
        Color = clWhite
        DataSet = FMain.adoTable
        DataField = #1054#1082#1083#1072#1076
        Transparent = False
        WordWrap = True
        FontSize = 8
      end
      object QRDBText3: TQRDBText
        Left = 104
        Top = 8
        Width = 71
        Height = 15
        Frame.Color = clBlack
        Frame.DrawTop = False
        Frame.DrawBottom = False
        Frame.DrawLeft = False
        Frame.DrawRight = False
        Size.Values = (
          39.6875
          275.166666666667
          21.1666666666667
          187.854166666667)
        Alignment = taLeftJustify
        AlignToBand = False
        AutoSize = True
        AutoStretch = False
        Color = clWhite
        DataSet = FMain.adoTable
        DataField = #1053#1072#1076#1073#1072#1074#1082#1072' '#1088#1091#1073
        Transparent = False
        WordWrap = True
        FontSize = 8
      end
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 0
    Width = 844
    Height = 145
    Align = alTop
    DataSource = src
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
        Width = 28
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
        FieldName = #1054#1082#1083#1072#1076
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072' '#1088#1091#1073
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072' '#1087#1086' '#1090#1080#1087#1091' '#1082#1086#1085#1090#1088#1072#1082#1090#1072
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072' '#1087#1086' '#1091#1088#1086#1074#1085#1102' '#1089#1086#1074#1077#1090#1072
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072' '#1087#1086' '#1087#1088#1072#1082#1090#1080#1082#1077
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072' '#1087#1086' '#1076#1080#1089#1094#1080#1087#1083#1080#1085#1072#1084
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1053#1072#1076#1073#1072#1074#1082#1072' '#1087#1086' '#1088#1072#1079#1088#1103#1076#1072#1084
        Visible = True
      end
      item
        Expanded = False
        FieldName = #1048#1090#1086#1075#1086
        Visible = True
      end>
  end
  object src: TDataSource
    DataSet = FMain.adoTable
    Left = 16
    Top = 16
  end
end
