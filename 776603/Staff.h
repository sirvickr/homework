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
class TFEmployee;
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
	TDBGrid *tblPersPractice;
	TGroupBox *GroupBox3;
	TDBGrid *tblPersSubj;
	TButton *txtAddEmployee;
	TDBEdit *txtPost;
	TDBEdit *txtCouncil;
	TDBEdit *txtPatronimic;
	TDBEdit *txtContract;
	TDBEdit *txtName;
	TDBEdit *txtHireDate;
	TDBEdit *txtSurname;
	TDBEdit *txtBirthDate;
	TButton *Button1;
	void __fastcall txtIDChange(TObject *Sender);
	void __fastcall txtAddEmployeeClick(TObject *Sender);
private:	// User declarations
	TFEmployee* frmEmployee;
	// коды из таблиц соответствующих справочников
	int idContract, idPost, idCouncil;
public:		// User declarations
	__fastcall TFStaff(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFStaff *FStaff;
//---------------------------------------------------------------------------
#endif
