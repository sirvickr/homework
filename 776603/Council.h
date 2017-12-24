//---------------------------------------------------------------------------


#ifndef CouncilH
#define CouncilH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFCouncil : public TFrame
{
__published:	// IDE-managed Components
	TDBGrid *tblMain;
	TDBNavigator *dbnMain;
private:	// User declarations
public:		// User declarations
	__fastcall TFCouncil(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFCouncil *FCouncil;
//---------------------------------------------------------------------------
#endif
