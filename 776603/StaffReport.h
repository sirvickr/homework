//---------------------------------------------------------------------------


#ifndef StaffReportH
#define StaffReportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <QuickRpt.hpp>
//---------------------------------------------------------------------------
class TFStaffReport : public TFrame
{
__published:	// IDE-managed Components
	TQuickRep *QuickRep1;
private:	// User declarations
public:		// User declarations
	__fastcall TFStaffReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFStaffReport *FStaffReport;
//---------------------------------------------------------------------------
#endif
