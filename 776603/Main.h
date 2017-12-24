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
