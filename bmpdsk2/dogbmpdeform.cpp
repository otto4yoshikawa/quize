//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("dogbmpmain1.cpp", Form1);
USEFORM("c:\program files (x86)\embarcadero\rad studio\7.0\ObjRepos\Cpp\okcancl1.CPP", OKBottomDlg);
USEFORM("bitmaphlp.cpp", OKHelpBottomDlg);
USEFORM("oprationdlg.cpp", OKRightDlg);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TOKHelpBottomDlg), &OKHelpBottomDlg);
		Application->CreateForm(__classid(TOKRightDlg), &OKRightDlg);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
