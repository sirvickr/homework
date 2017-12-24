//---------------------------------------------------------------------------


#ifndef PracticeH
#define PracticeH
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
class TFPractice : public TFrame
{
__published:	// IDE-managed Components
	TDBNavigator *dbnMain;
	TDBGrid *tblMain;
private:	// User declarations
public:		// User declarations
	__fastcall TFPractice(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFPractice *FPractice;
//---------------------------------------------------------------------------
#endif
