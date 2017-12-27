//---------------------------------------------------------------------------


#ifndef ReportsH
#define ReportsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <QRCtrls.hpp>
#include <QuickRpt.hpp>
//---------------------------------------------------------------------------
class TFReports : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TButton *cmdStaff;
	TButton *cmdDir;
	void __fastcall cmdStaffClick(TObject *Sender);
	void __fastcall cmdDirClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFReports(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFReports *FReports;
//---------------------------------------------------------------------------
#endif
