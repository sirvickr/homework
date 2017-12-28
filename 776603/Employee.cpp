//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Employee.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFEmployee *FEmployee;
//---------------------------------------------------------------------------
__fastcall TFEmployee::TFEmployee(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
