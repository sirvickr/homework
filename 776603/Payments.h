//---------------------------------------------------------------------------


#ifndef PaymentsH
#define PaymentsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <QuickRpt.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <QRCtrls.hpp>
//---------------------------------------------------------------------------
class TFPayments : public TFrame
{
__published:	// IDE-managed Components
	TQuickRep *qrMain;
	TDataSource *src;
	TDBGrid *DBGrid1;
	TQRBand *DetailBand1;
	TQRDBText *QRDBText1;
	TQRDBText *QRDBText2;
	TQRDBText *QRDBText3;
private:	// User declarations
public:		// User declarations
	__fastcall TFPayments(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFPayments *FPayments;
//---------------------------------------------------------------------------
#endif
