object FView: TFView
  Left = 290
  Top = 157
  BorderStyle = bsToolWindow
  Caption = #1054#1090#1086#1073#1088#1072#1078#1077#1085#1080#1077
  ClientHeight = 602
  ClientWidth = 890
  Color = clGray
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnDeactivate = FormDeactivate
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object tmrView: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmrViewTimer
  end
end
