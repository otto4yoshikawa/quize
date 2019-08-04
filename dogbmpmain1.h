//---------------------------------------------------------------------------

#ifndef dogbmpmain1H
#define dogbmpmain1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TMainMenu *MainMenu1;
	TMemo *Memo1;
	TMenuItem *file1;
	TMenuItem *open1;
	TMenuItem *save1;
	TMenuItem *edit1;
	TMenuItem *show1;
	TMenuItem *operation1;
	TMenuItem *converttoColor1;
	TMenuItem *clipping1;
	TMenuItem *enlal1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *ellp1;
	TMenuItem *Dogbmpdeform1;
	TStatusBar *StatusBar1;
	TImage *Image1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	void __fastcall converttoColor1Click(TObject *Sender);
	void __fastcall clipping1Click(TObject *Sender);
	void __fastcall ellp1Click(TObject *Sender);
	void __fastcall open1Click(TObject *Sender);
	void __fastcall operation1Click(TObject *Sender);
	void __fastcall save1Click(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
