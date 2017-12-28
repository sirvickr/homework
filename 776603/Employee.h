//---------------------------------------------------------------------------


#ifndef EmployeeH
#define EmployeeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFEmployee : public TFrame
{
__published:	// IDE-managed Components
	TDBEdit *txtID;
	TGroupBox *GroupBox1;
	TDBGrid *tblPersRanks;
	TGroupBox *GroupBox2;
	TDBGrid *tblPersPractice;
	TGroupBox *GroupBox3;
	TDBGrid *tblPersSubj;
	TDBEdit *txtPost;
	TDBEdit *txtSurname;
	TDBEdit *txtName;
	TDBEdit *txtPatronimic;
	TDBEdit *txtBirthDate;
	TDBEdit *txtHireDate;
	TDBEdit *txtContract;
	TDBEdit *txtCouncil;
private:	// User declarations
public:		// User declarations
	__fastcall TFEmployee(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFEmployee *FEmployee;
//---------------------------------------------------------------------------
#endif
