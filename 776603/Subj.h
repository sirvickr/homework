//---------------------------------------------------------------------------


#ifndef SubjH
#define SubjH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFSubj : public TFrame
{
__published:	// IDE-managed Components
	TDBNavigator *dbnMain;
	TDBGrid *tblMain;
private:	// User declarations
public:		// User declarations
	__fastcall TFSubj(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFSubj *FSubj;
//---------------------------------------------------------------------------
#endif
