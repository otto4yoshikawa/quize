object PagesDlg: TPagesDlg
  Left = 195
  Top = 108
  BorderStyle = bsDialog
  Caption = #12479#12502#20184#12365#12506#12540#12472
  ClientHeight = 325
  ClientWidth = 463
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 463
    Height = 288
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    ParentColor = True
    TabOrder = 0
    object PageControl1: TPageControl
      Left = 5
      Top = 5
      Width = 453
      Height = 278
      ActivePage = TabSheet1
      Align = alClient
      TabOrder = 0
      ExplicitWidth = 452
      object TabSheet1: TTabSheet
        Caption = 'TabSheet1'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
      end
      object TabSheet2: TTabSheet
        Caption = 'TabSheet2'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
      end
      object TabSheet3: TTabSheet
        Caption = 'TabSheet3'
        ExplicitLeft = 0
        ExplicitTop = 0
        ExplicitWidth = 0
        ExplicitHeight = 0
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 288
    Width = 463
    Height = 37
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 1
    object OKBtn: TButton
      Left = 203
      Top = 2
      Width = 81
      Height = 27
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object CancelBtn: TButton
      Left = 289
      Top = 2
      Width = 82
      Height = 27
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object HelpBtn: TButton
      Left = 376
      Top = 2
      Width = 81
      Height = 27
      Caption = #12504#12523#12503'(&H)'
      TabOrder = 2
    end
  end
end
