object OKRightDlg: TOKRightDlg
  Left = 227
  Top = 108
  BorderStyle = bsDialog
  Caption = #12480#12452#12450#12525#12464
  ClientHeight = 314
  ClientWidth = 416
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 304
    Height = 265
    Shape = bsFrame
  end
  object OKBtn: TButton
    Left = 325
    Top = 9
    Width = 81
    Height = 27
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 325
    Top = 41
    Width = 81
    Height = 27
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 1
    OnClick = CancelBtnClick
  end
  object CheckBox1: TCheckBox
    Left = 128
    Top = 40
    Width = 97
    Height = 17
    Caption = 'color_gray'
    TabOrder = 2
  end
  object CheckBox2: TCheckBox
    Left = 128
    Top = 72
    Width = 97
    Height = 17
    Caption = 'clipping'
    TabOrder = 3
  end
  object CheckBox3: TCheckBox
    Left = 128
    Top = 95
    Width = 97
    Height = 17
    Caption = 'eblarge'
    TabOrder = 4
  end
  object CheckBox4: TCheckBox
    Left = 128
    Top = 118
    Width = 97
    Height = 17
    Caption = 'R/L invert'
    TabOrder = 5
  end
  object CheckBox5: TCheckBox
    Left = 128
    Top = 141
    Width = 97
    Height = 17
    Caption = 'U/D invert'
    TabOrder = 6
  end
  object CheckBox6: TCheckBox
    Left = 128
    Top = 164
    Width = 97
    Height = 17
    Caption = 'bright'
    TabOrder = 7
  end
  object CheckBox7: TCheckBox
    Left = 128
    Top = 187
    Width = 97
    Height = 17
    Caption = 'Sobel'#39's routine'
    TabOrder = 8
  end
end
