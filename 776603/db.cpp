#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Main.cpp", FMain);
USEFORM("Directories.cpp", FDirectories); /* TFrame: File Type */
USEFORM("Reports.cpp", FReports); /* TFrame: File Type */
USEFORM("Staff.cpp", FStaff); /* TFrame: File Type */
USEFORM("Post.cpp", FPost); /* TFrame: File Type */
USEFORM("Contract.cpp", FContract); /* TFrame: File Type */
USEFORM("Council.cpp", FCouncil); /* TFrame: File Type */
USEFORM("Practice.cpp", FPractice); /* TFrame: File Type */
USEFORM("Ranks.cpp", FRanks); /* TFrame: File Type */
USEFORM("Subj.cpp", FSubj); /* TFrame: File Type */
USEFORM("StaffReport.cpp", FStaffReport); /* TFrame: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TFMain), &FMain);
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
