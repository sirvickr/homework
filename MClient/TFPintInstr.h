//---------------------------------------------------------------------------

#ifndef TFPintInstrH
#define TFPintInstrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFPintInstr : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGDocType;
	TRadioButton *RbInstruction;
	TRadioButton *RbEnvelope;
	TGroupBox *GBNumbers;
	TCheckBox *ChkMin;
	TCheckBox *ChkMax;
	TComboBox *CboMin;
	TComboBox *CboMax;
	TButton *BTemplate;
	TButton *BPrint;
	void __fastcall BPrintClick(TObject *Sender);
private:	// User declarations
	AnsiString min_id;
	AnsiString max_id;
	AnsiString usr_id;
public:		// User declarations
	void Print_Instruction( bool fNight,
					const AnsiString& strTime1, const AnsiString& strTime2,
					AnsiString &login, AnsiString &pw, AnsiString &id,
					AnsiString &per, AnsiString &traf );
	void Print_Envelope(AnsiString &s, AnsiString &id);

	__fastcall TFPintInstr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFPintInstr *FPintInstr;
//---------------------------------------------------------------------------
#endif
