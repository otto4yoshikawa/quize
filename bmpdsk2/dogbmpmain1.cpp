//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "dogbmpmain1.h"
#include "bitmaphlp.h"
#include "oprationdlg.h"
#pragma package(smart_init)
#include <Jpeg.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
FILE *logfile;

int DX, DY, DX1;
int BASEX, BASEY;
bool OK;
bool checklist[6];
void DoPrintf(char *szFormat, ...) {
	va_list vaArgs;
	static char szBuf[256];
	if (!logfile)
		logfile = fopen("debug.txt", "w");
	va_start(vaArgs, szFormat);
	vsprintf(szBuf, szFormat, vaArgs);
	va_end(vaArgs);
	Form1->Memo1->Lines->Add(szBuf);
	fprintf(logfile, "%s", szBuf);
	if (szBuf[0] != ' ') {
		fprintf(logfile, "\n");
		fflush(logfile);
	}

}
HANDLE Spawn(char *commandline, DWORD ggg, bool waitmode) {
	STARTUPINFO lpStartInfo;
	bool fSuccess;
	HANDLE hProcess;
	PROCESS_INFORMATION lpProcessInfo;
	int rc;
	ZeroMemory(&lpStartInfo, sizeof(lpStartInfo));
	lpStartInfo.cb = sizeof(lpStartInfo);
	lpStartInfo.dwFlags = STARTF_USESHOWWINDOW;
	lpStartInfo.wShowWindow = ggg;
	fSuccess = CreateProcess(NULL, commandline, NULL, NULL, TRUE, 0, NULL,
		NULL, &lpStartInfo, &lpProcessInfo);
	if (fSuccess) {
		hProcess = lpProcessInfo.hProcess;

		// 不要なスレッドハンドルをすぐクローズ
		CloseHandle(lpProcessInfo.hThread);
		if (waitmode) {
			// プロセスが終了するまで待つ

			rc = WaitForSingleObject(hProcess, INFINITE);
			if (rc == WAIT_FAILED) {
				DoPrintf("wait ertror");
				return 0;
			}
		}
	}
	else {
		DoPrintf("fsuceess error");
		return 0;
	}

	return hProcess;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TForm1::converttoColor1Click(TObject *Sender)
{
  DoPrintf("convert gray");

}
AnsiString A;
//---------------------------------------------------------------------------
void __fastcall TForm1::clipping1Click(TObject *Sender)
{
	 DoPrintf("clipping");
	 	char w[80], str[80], fname[80],GNUDIR[80];
	sprintf(w, "c:/puzzle/bmpdsk/BmpDeform.exe x");
HANDLE 	h = Spawn(w, SW_SHOW, true);
	if (!h)
		return ;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ellp1Click(TObject *Sender)
{
	 DoPrintf("help ver 1.0");

 TOKHelpBottomDlg *p=new TOKHelpBottomDlg(this);p->ShowModal();}

//---------------------------------------------------------------------------

void __fastcall TForm1::open1Click(TObject *Sender)
{
OpenDialog1->Filter="bmp files (*,bmp)|*.bmp";
	if(!OpenDialog1->Execute()) return ;
	AnsiString A=OpenDialog1->FileName;char *q=A.c_str();
	DoPrintf("%s",A.c_str());
	Image1->
	Picture ->LoadFromFile( A);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::operation1Click(TObject *Sender)
{
DoPrintf("operation");
	 TOKRightDlg *p= new TOKRightDlg(this);
	 p->ShowModal() ;
	 DoPrintf("rc=%d",OK);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::save1Click(TObject *Sender)
{
	SaveDialog1->Execute();
	AnsiString A=SaveDialog1->FileName;char *q=A.c_str();
	DoPrintf("%s",A.c_str());
	Image1->
	Picture ->SaveToFile( A);

}
//---------------------------------------------------------------------------

