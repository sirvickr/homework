#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Subj.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFSubj *FSubj;
//---------------------------------------------------------------------------
__fastcall TFSubj::TFSubj(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
