object FView: TFView
  Left = 290
  Top = 157
  BorderStyle = bsToolWindow
  Caption = #1061#1086#1076' '#1083#1091#1095#1072
  ClientHeight = 602
  ClientWidth = 595
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
    Interval = 50
    OnTimer = tmrViewTimer
  end
end
