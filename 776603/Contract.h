//---------------------------------------------------------------------------


#ifndef ContractH
#define ContractH
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
class TFContract : public TFrame
{
__published:	// IDE-managed Components
	TDBGrid *tblMain;
	TDBNavigator *dbnMain;
private:	// User declarations
public:		// User declarations
	__fastcall TFContract(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFContract *FContract;
//---------------------------------------------------------------------------
#endif
