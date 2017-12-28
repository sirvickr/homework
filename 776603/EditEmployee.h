//---------------------------------------------------------------------------

#ifndef EditEmployeeH
#define EditEmployeeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
//---------------------------------------------------------------------------
class TFEditEmployee : public TForm
{
__published:	// IDE-managed Components
	TEdit *txtSurname;
	TLabel *Label1;
	TEdit *txtName;
	TLabel *Label2;
	TEdit *txtPatronimic;
	TLabel *Label3;
	TEdit *txtBirthDate;
	TLabel *Label4;
	TEdit *txtHireDate;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TListView *lvwRanks;
	TListView *lvwPractice;
	TListView *ListView1;
	TButton *cmdOK;
	TButton *cmdCancel;
	TADOQuery *adoEmployee;
	TDataSource *srcEmployee;
	TADOTable *adoContract;
	TADOTable *adoPost;
	TADOTable *adoCouncil;
	TDataSource *srcCouncil;
	TDataSource *srcPost;
	TDataSource *srcContract;
	TDBLookupComboBox *cbxPost;
	TDBLookupComboBox *cbxContract;
	TDBLookupComboBox *cbxCouncil;
private:	// User declarations
public:		// User declarations
	__fastcall TFEditEmployee(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFEditEmployee *FEditEmployee;
//---------------------------------------------------------------------------
#endif
