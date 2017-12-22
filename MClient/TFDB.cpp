//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFDB *FDB;
//---------------------------------------------------------------------------
__fastcall TFDB::TFDB(TComponent* Owner)
	: TDataModule(Owner)
{
	exe_ini = ChangeFileExt(Application->ExeName, ".INI" );
	ini = new TIniFile(exe_ini);
	strcpy(znak,";");
	dataReport = "01.04.02";
	TipDB = "radusers";

	//считывание имени провайдера
	Prov = ini->ReadString("Провайдер", "Имя", "");
	//считывание парвметров для печати конвертов
	buf[0]='\0';
	strcpy(buf,ini->ReadString("Конверт", "Тип_1строка", "").c_str());
	if (strlen(buf))
	{
		Kon_Left1 = atof(strtok(buf,znak));
		Kon_Top1 = atof(strtok(NULL,znak));
	}
	buf[0]='\0';
	strcpy(buf,ini->ReadString("Конверт", "Тип_2строка", "").c_str());
	if (strlen(buf))
	{
		Kon_Left2 = atof(strtok(buf,znak));
		Kon_Top2 = atof(strtok(NULL,znak));
	}
	buf[0]='\0';
	strcpy(buf,ini->ReadString("Конверт", "Номер", "").c_str());
	if (strlen(buf))
	{
		Kon_Left3 = atof(strtok(buf,znak));
		Kon_Top3 = atof(strtok(NULL,znak));
	}
}
//---------------------------------------------------------------------------
void TFDB::SaveTemplate(void)
{
	// сохранение координат строк конверта
	buf[0]='\0';
	sprintf(buf, "%6.2f;%6.2f", Kon_Left1, Kon_Top1);
	ini->WriteString("Конверт", "Тип_1строка", buf);
	buf[0]='\0';
	sprintf(buf, "%6.2f;%6.2f", Kon_Left2, Kon_Top2);
	ini->WriteString("Конверт", "Тип_2строка", buf);
	buf[0]='\0';
	sprintf(buf, "%6.2f;%6.2f", Kon_Left3, Kon_Top3);
	ini->WriteString("Конверт", "Номер", buf);
}
//---------------------------------------------------------------------------
void TFDB::GetIniFile(TForm* F)
{
	int c, Con, Cont, j, ncol;
	char ss[5];
	char *fieldname, *work;
	AnsiString NameClass, NameDS;
	TdxDBGrid* G;
	TDataSet* D;
	buf[0]='\0';
	strcpy(buf,ini->ReadString(F->Caption, "Позиция формы", "").c_str());
	if (strlen(buf))
	{
		// восстановление координат формы: left, top, width, heght
		work = strtok(buf,znak);   F->Left   = atoi(work);
		work = strtok(NULL,znak);  F->Top    = atoi(work);
		work = strtok(NULL,znak);  F->Width  = atoi(work);
		work = strtok(NULL,znak);  F->Height = atoi(work);
		Cont = F->ControlCount;
		for (c=0; c<Cont; c++)
		{
			NameClass =  F->Controls[c]->ClassName();
			if ( NameClass == "TdxDBGrid")
			{
				G =  (TdxDBGrid*) F->Controls[c];
				D = G->DataSource->DataSet;
				NameDS = G->DataSource->DataSet->Name;
				ncol=  ini->ReadInteger(F->Caption + " " + NameDS,"Число полей", 0);
				if (ncol)
				{
					for (j=0; j<ncol; j++)
					{
						buf[0]='\0';
						itoa(j, ss, 10);
						strcpy(buf,ini->ReadString(F->Caption + " " + NameDS, ss, "").c_str());
						fieldname = strtok(buf,znak);
						D->Fields->FieldByName(fieldname)->Index = j;
						work = strtok(NULL,znak);
						G->Columns[j]->Width = atoi(work);
						//D->Fields->FieldByName(fieldname)->Size = atoi(work);
						work = strtok(NULL,znak);
						//D->Fields->FieldByName(fieldname)->DisplayLabel = work;
						G->Columns[j]->Caption = work;
						work = strtok(NULL,znak);  // тип данных
						//D->Fields->FieldByName(fieldname)->DataType = atoi(work);
						work = strtok(NULL,znak);
						if (work[0] == '1')
						//D->Fields->FieldByName(fieldname)->Visible = true;
							G->Columns[j]->Visible = true;
						else
						//D->Fields->FieldByName(fieldname)->Visible = false;
							G->Columns[j]->Visible = false;
					} // for (j=0; j<ncol; j++)
				}  // if (ncol)
			} // if ( NameClass == "TdxDBGrid")
		} // for (c=0; c<Cont; c++)
	} //if (strlen(buf))
}
//---------------------------------------------------------------------------
void TFDB::PutIniFile(TForm* F)
{
	int j, c, Cont, ncol;
	char ss[5];
	AnsiString NameClass, NameDS;
	TdxDBGrid* G;
	TDataSet* D;
	// сохранение координат формы: left, top, width, heght
	buf[0]='\0';
	itoa(F->Left, ss, 10);   strcat(buf,ss); strcat(buf,znak);
	itoa(F->Top, ss, 10);    strcat(buf,ss); strcat(buf,znak);
	itoa(F->Width, ss, 10);  strcat(buf,ss); strcat(buf,znak);
	itoa(F->Height, ss, 10); strcat(buf,ss); strcat(buf,znak);
	ini->WriteString(F->Caption,"Позиция формы", buf);
	buf[0]='\0';
	G = NULL;
	Cont = F->ControlCount;
	for (c = 0; c < Cont; c++)
	{
		NameClass =  F->Controls[c]->ClassName();
		if ( NameClass == "TdxDBGrid")
		{
			G =  (TdxDBGrid*) F->Controls[c];
			D = G->DataSource->DataSet;
			NameDS = G->DataSource->DataSet->Name;
			ncol = D->FieldCount;
			ini->WriteInteger(F->Caption + " " + NameDS,"Число полей", ncol);
			for (j=0; j<ncol; j++)
			{
				buf[0]='\0';
				strcpy(buf,D->Fields->Fields[j]->FieldName.c_str());
				strcat(buf,znak);
				itoa(G->Columns[j]->Width, ss, 10);
				// itoa(D->Fields->Fields[j]->DisplayWidth, ss, 10);
				strcat(buf,ss);
				strcat(buf,znak);
				strcat(buf,G->Columns[j]->Caption.c_str());
				// strcat(buf, D->Fields->Fields[j]->DisplayLabel.c_str());
				strcat(buf,znak);
				itoa(D->Fields->Fields[j]->DataType,ss, 10);
				strcat(buf,ss);
				strcat(buf,znak);
				if  (G->Columns[j]->Visible == true)
				//(D->Fields->Fields[j]->Visible == true)
					strcat(buf, "1");
				else
					strcat(buf, "0");
				strcat(buf, znak);
				itoa(j, ss, 10);
				ini->WriteString(F->Caption + " " + NameDS, ss, buf);
			}   //for ( j=0; j < ncol; j++ )
		}  //if ( NameClass == "TdxDBGrid" )
	}  //for ( c=0; c<Cont; c++ )
	if ( G )
		G->DestroyColumns();
}
//---------------------------------------------------------------------------
