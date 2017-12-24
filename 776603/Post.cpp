#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "Post.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFPost *FPost;
//---------------------------------------------------------------------------
__fastcall TFPost::TFPost(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
