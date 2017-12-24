//---------------------------------------------------------------------------


#ifndef RanksH
#define RanksH
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
class TFRanks : public TFrame
{
__published:	// IDE-managed Components
	TDBNavigator *dbnMain;
	TDBGrid *tblMain;
private:	// User declarations
public:		// User declarations
	__fastcall TFRanks(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRanks *FRanks;
//---------------------------------------------------------------------------
#endif
