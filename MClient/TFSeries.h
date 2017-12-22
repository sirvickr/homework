//---------------------------------------------------------------------------

#ifndef TFSeriesH
#define TFSeriesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TFSeries : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TDBLookupComboBox *CboProfile;
	TDataSource *DSProfile;
	TLabel *Label2;
	TEdit *EdQuant;
	TButton *BPrint;
	TButton *BGenerate;
	TGroupBox *GroupBox1;
	TEdit *EdMin;
	TEdit *EdMax;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CboProfileCloseUp(TObject *Sender);
	void __fastcall BGenerateClick(TObject *Sender);
private:	// User declarations
	AnsiString profile_cod;
public:		// User declarations
	__fastcall TFSeries(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFSeries *FSeries;
//---------------------------------------------------------------------------
#endif
