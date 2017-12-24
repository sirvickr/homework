//---------------------------------------------------------------------------


#ifndef StaffH
#define StaffH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFStaff : public TFrame
{
__published:	// IDE-managed Components
	TPanel *pnlMain;
	TDBNavigator *dbnMain;
	TDBGrid *tblMain;
	TPanel *pnlDetails;
	TGroupBox *GroupBox1;
	TDBGrid *tblPersRanks;
	TGroupBox *GroupBox2;
	TDBEdit *txtID;
	TDBText *txtPerson;
	TDBGrid *tblPersPractice;
	TGroupBox *GroupBox3;
	TDBGrid *tblPersSubj;
	void __fastcall txtIDChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFStaff(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFStaff *FStaff;
//---------------------------------------------------------------------------
#endif
