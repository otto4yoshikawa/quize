//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "oprationdlg.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TOKRightDlg *OKRightDlg;
//--------------------------------------------------------------------- 
__fastcall TOKRightDlg::TOKRightDlg(TComponent* AOwner)
	: TForm(AOwner)
{

}
extern bool checklist[6],OK;
//---------------------------------------------------------------------
void __fastcall TOKRightDlg::OKBtnClick(TObject *Sender)
{
OK=true;
	if( CheckBox1->Enabled) checklist[0]=true;

}
//---------------------------------------------------------------------------

void __fastcall TOKRightDlg::CancelBtnClick(TObject *Sender)
{
	 OK=false;

}
//---------------------------------------------------------------------------

