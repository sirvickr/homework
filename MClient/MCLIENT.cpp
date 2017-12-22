//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("TFLogin.cpp", FLogin);
USEFORM("TFMenu.cpp", FMainMenu);
USEFORM("TFDB.cpp", FDB); /* TDataModule: File Type */
USEFORM("TFClientInfo.cpp", FClientInfo);
USEFORM("TFSeries.cpp", FSeries);
USEFORM("TFPintInstr.cpp", FPintInstr);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFLogin), &FLogin);
		Application->CreateForm(__classid(TFMainMenu), &FMainMenu);
		Application->CreateForm(__classid(TFDB), &FDB);
		Application->CreateForm(__classid(TFClientInfo), &FClientInfo);
		Application->CreateForm(__classid(TFSeries), &FSeries);
		Application->CreateForm(__classid(TFPintInstr), &FPintInstr);
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
