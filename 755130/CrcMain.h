//---------------------------------------------------------------------------

#ifndef CrcMainH
#define CrcMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class CrcBase;
//---------------------------------------------------------------------------
class TfrmCrcMain : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *openDlg;
	TButton *cmdProtect;
	TButton *cmdVerify;
	TEdit *txtFileName;
	TButton *cmdOpenFile;
	TMemo *Log;
	TButton *cmdShowContent;
	TButton *cmdCalcCRC;
	TRadioGroup *rgCrcWidth;
	void __fastcall cmdOpenFileClick(TObject *Sender);
	void __fastcall cmdProtectClick(TObject *Sender);
	void __fastcall cmdVerifyClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cmdShowContentClick(TObject *Sender);
	void __fastcall cmdCalcCRCClick(TObject *Sender);
private:	// User declarations
	std::vector<BYTE> input;
	String exePath;
	String iniFileName;
public:		// User declarations
	__fastcall TfrmCrcMain(TComponent* Owner);
	CrcBase* GetCrcCalc(int index);
	void ReadFromFile(std::string& content, const std::string& fileName);
	void ShowContent(const std::string& content, size_t padding = 0);
	void AppendCRC(const std::string& fileName, int crc, int width) {
		std::ofstream file;
		file.open(fileName.c_str(), std::fstream::app);
		if (file) {
			file.write((const char*)&crc, width);
		}
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCrcMain *frmCrcMain;
//---------------------------------------------------------------------------
#endif
