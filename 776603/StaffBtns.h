//---------------------------------------------------------------------------


#ifndef StaffBtnsH
#define StaffBtnsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFStaffBtns : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TButton *cmdForms;
	TButton *cmdReport;
	void __fastcall cmdFormsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFStaffBtns(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFStaffBtns *FStaffBtns;
//---------------------------------------------------------------------------
#endif
