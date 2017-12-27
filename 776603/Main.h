//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <dbcgrids.hpp>
#include <Mask.hpp>
#include <QuickRpt.hpp>
#include <QRCtrls.hpp>
//---------------------------------------------------------------------------
class TFMain : public TForm
{
__published:	// IDE-managed Components
	TSpeedButton *cmdExit;
	TButton *cmdBack;
	TLabel *lblBack;
	TADOConnection *adoConn;
	TDataSource *dataSource;
	TADOQuery *adoPracticeQuery;
	TADOTable *adoTable;
	TPanel *pnlSubForm;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *cmdStaff;
	TButton *cmdDir;
	TButton *cmdReports;
	TADOQuery *adoPersRanks;
	TDataSource *srcPersRanks;
	TADOQuery *adoPersPractice;
	TDataSource *srcPersPractice;
	TADOQuery *adoPersSubj;
	TDataSource *srcPersSubj;
	TADOTable *tbl;
	TDataSource *src;
	TQuickRep *qrPayments;
	TQRBand *QRBand1;
	TQRDBText *QRDBText9;
	TQRDBText *txtPrem;
	TQRDBText *txtPaymID;
	TQRDBText *txtPremContract;
	TQRDBText *txtPremCouncil;
	TQRDBText *txtPremSubj;
	TQRDBText *txtPremRank;
	TQRDBText *txtPremPractice;
	TQRDBText *txtPremTotal;
	TQRBand *ColumnHeaderBand2;
	TQRLabel *QRLabel8;
	TQRLabel *QRLabel9;
	TQRLabel *QRLabel10;
	TQRLabel *QRLabel11;
	TQRLabel *QRLabel12;
	TQRLabel *QRLabel13;
	TQRLabel *QRLabel14;
	TQRLabel *QRLabel15;
	TQRLabel *QRLabel16;
	TQRLabel *QRLabel17;
	TQRLabel *QRLabel19;
	TQRDBText *QRDBText8;
	TQRBand *SummaryBand1;
	TQRExpr *QRExpr2;
	TQRLabel *QRLabel1;
	TQRExpr *QRExpr1;
	TQRLabel *QRLabel2;
	TQRExpr *QRExpr3;
	TQRLabel *QRLabel3;
	TQuickRep *qrStaff;
	TQRBand *DetailBand1;
	TQRDBText *QRDBText1;
	TQRDBText *QRDBText2;
	TQRDBText *QRDBText3;
	TQRDBText *QRDBText4;
	TQRDBText *QRDBText5;
	TQRDBText *QRDBText6;
	TQRDBText *QRDBText7;
	TQRBand *ColumnHeaderBand1;
	TQRLabel *QRLabel4;
	TQRLabel *QRLabel5;
	TQRLabel *QRLabel6;
	TQRLabel *QRLabel7;
	TQRLabel *QRLabel18;
	TQRLabel *QRLabel20;
	TQRLabel *QRLabel21;
	TQRBand *SummaryBand2;
	TQRExpr *QRExpr4;
	TQRLabel *QRLabel22;
	TQRExpr *QRExpr5;
	TQRExpr *QRExpr6;
	TQRLabel *QRLabel23;
	TQRLabel *QRLabel24;
	void __fastcall cmdExitClick(TObject *Sender);
	void __fastcall cmdStaffClick(TObject *Sender);
	void __fastcall cmdDirClick(TObject *Sender);
	void __fastcall cmdReportsClick(TObject *Sender);
	void __fastcall cmdBackClick(TObject *Sender);
private:	// User declarations
	static const int FrameHeight = 350;
	// стек подчиненных форм
	typedef std::stack<TFrame*> ActiveFrames;
	ActiveFrames activeFrames;
	//TFrame* activeFrame;
public:		// User declarations
	__fastcall TFMain(TComponent* Owner);
	// добавить подчиненную форму верхнего уровня
	void __fastcall AddFrame(TFrame *frame);
	// убрать подчиненную форму с вершины стека
	void __fastcall DelFrame();
};
//---------------------------------------------------------------------------
extern PACKAGE TFMain *FMain;
//---------------------------------------------------------------------------
#endif
