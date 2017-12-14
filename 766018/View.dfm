object FView: TFView
  Left = 594
  Top = 158
  Width = 1012
  Height = 640
  Caption = 'FView'
  Color = clBtnFace
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
