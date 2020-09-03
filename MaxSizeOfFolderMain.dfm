object Main: TMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Max Size Of Folder'
  ClientHeight = 211
  ClientWidth = 457
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = PopupMenu
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PopupMenu: TPopupMenu
    Left = 128
    Top = 72
    object miStartCheck: TMenuItem
      Caption = #1047#1072#1087#1091#1089#1082' '#1086#1095#1080#1089#1090#1082#1080
      Default = True
      OnClick = miStartCheckClick
    end
    object miSeparator01: TMenuItem
      Caption = '-'
    end
    object miAbout: TMenuItem
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077'...'
      OnClick = miAboutClick
    end
    object miSeparator02: TMenuItem
      Caption = '-'
    end
    object miClose: TMenuItem
      Caption = #1047#1072#1082#1088#1099#1090#1100
      OnClick = miCloseClick
    end
  end
  object TrayIcon: TTrayIcon
    PopupMenu = PopupMenu
    OnClick = TrayIconClick
    OnDblClick = miStartCheckClick
    Left = 200
    Top = 112
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 224
    Top = 32
  end
end
