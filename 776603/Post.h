//---------------------------------------------------------------------------


#ifndef PostH
#define PostH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFPost : public TFrame
{
__published:	// IDE-managed Components
	TDBGrid *tblMain;
	TDBNavigator *dbnMain;
private:	// User declarations
public:		// User declarations
	__fastcall TFPost(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFPost *FPost;
//---------------------------------------------------------------------------
#endif
