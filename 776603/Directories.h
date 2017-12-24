//---------------------------------------------------------------------------


#ifndef DirectoriesH
#define DirectoriesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFDirectories : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label8;
	TButton *cmdPosts;
	TButton *cmdContract;
	TButton *cmdCouncil;
	TButton *cmdPractice;
	TButton *cmdRanks;
	TButton *cmdSubjects;
	void __fastcall cmdPostsClick(TObject *Sender);
	void __fastcall cmdContractClick(TObject *Sender);
	void __fastcall cmdCouncilClick(TObject *Sender);
	void __fastcall cmdPracticeClick(TObject *Sender);
	void __fastcall cmdRanksClick(TObject *Sender);
	void __fastcall cmdSubjectsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFDirectories(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFDirectories *FDirectories;
//---------------------------------------------------------------------------
#endif
