//---------------------------------------------------------------------------

#include <fmx.h>
#include <System.UITypes.hpp>
#pragma hdrstop
#include "IniFiles.hpp"

#include "Unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource ("*.Windows.fmx", _PLAT_MSWINDOWS)
#pragma resource ("*.Macintosh.fmx", _PLAT_MACOS)
#pragma resource ("*.iPhone.fmx", _PLAT_IOS)
#pragma resource ("*.iPhone4in.fmx", _PLAT_IOS)
#pragma resource ("*.iPhone55in.fmx", _PLAT_IOS)
#pragma resource ("*.iPhone47in.fmx", _PLAT_IOS)
#pragma resource ("*.LgXhdpiPh.fmx", _PLAT_ANDROID)
#pragma resource ("*.iPad.fmx", _PLAT_IOS)

TMainForm *MainForm;

TIniFile* Settings;
TStringList * Sheet = new TStringList();
String Send, Temp;
int i, kol;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormShow(TObject *Sender)
{
	//Load settings
	Settings = new TIniFile(GetCurrentDir()+"\\settings.ini");
	AdressEdit->Text=Settings->ReadString("Auth","Adress","");
	LoginEdit->Text=Settings->ReadString("Auth","Login","");
	PassEdit->Text=Settings->ReadString("Auth","Pass","");
	MinEdit->Text=Settings->ReadString("Options","UpdateTime","5");
	KolEdit->Text=Settings->ReadString("Options","QueuesCount","0");
	FileEdit->Text=Settings->ReadString("Options","Sound",GetCurrentDir()+"\\notice.mp3");
	if(FileExists(FileEdit->Text))
		Player->FileName=FileEdit->Text;

	PlayButton->Enabled=true;
	delete Settings;

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartButtonClick(TObject *Sender)
{
	//Disable some settings
	AdressEdit->Enabled=false;
	LoginEdit->Enabled=false;
	PassEdit->Enabled=false;
	ShowPassButton->Enabled=false;
	MinEdit->Enabled=false;
	StartButton->Enabled=false;

	//Save settings
	Settings = new TIniFile(GetCurrentDir()+"\\settings.ini");
	Settings->WriteString("Auth","Adress",AdressEdit->Text);
	Settings->WriteString("Auth","Login",LoginEdit->Text);
	Settings->WriteString("Auth","Pass",PassEdit->Text);
	Settings->WriteString("Options","UpdateTime",MinEdit->Text);
	Timer->Interval=StrToInt(MinEdit->Text)*60000;
    Settings->WriteString("Options","QueuesCount",KolEdit->Text);
	Settings->WriteString("Options","Sound",FileEdit->Text);
	Player->FileName=FileEdit->Text;
	delete Settings;

	//Log-In
	Send=AdressEdit->Text+"?Action=Login&RequestedURL=?Action=AgentPreferences&Lang=en&TimeOffset=-180&User="+LoginEdit->Text+"&Password="+PassEdit->Text;
	Sheet->Text=Web->Get(Send);

	//Get ChallengeToken
	for (int i = 0; i < Sheet->Count; i++)
	{
		if(Sheet->Strings[i].Pos("ChallengeToken="))
		{
			Temp=Sheet->Strings[i].SubString(Sheet->Strings[i].Pos("ChallengeToken=")+15,Sheet->Strings[i].Pos(";\"")-2);
			Temp=Temp.SubString(0,Temp.Pos(";\"")-1);
		}
	}

	//Set OTRS language = English
	Send=AdressEdit->Text+"?ChallengeToken="+Temp+"&Action=AgentPreferences&Subaction=Update&Group=Language&UserLanguage=en: undefined";
	Web->Get(Send);

	Send=AdressEdit->Text+"?Action=AgentTicketQueue";
	Sheet->Text=Web->Get(Send);

	if(Sheet->Text.Pos("My Queues ("))
	{
		for (int i = 0; i < Sheet->Count; i++)
		{
			if(Sheet->Strings[i].Pos("My Queues ("))
			{
				Temp=Sheet->Strings[i].SubString(Sheet->Strings[i].Pos("My Queues (")+11,Sheet->Strings[i].Pos(")</a>")-4);
				Temp=Temp.SubString(0,Temp.Pos(")</a>")-1);
				TicketLabel->Enabled=true;
				KolLabel->Enabled=true;
				KolLabel->Text=Temp;
				Timer->Enabled=true;
				return;
			}
		}
	}
	else
	{
		ShowMessage("Authorization error or incorrect OTRS URL!");
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::TimerTimer(TObject *Sender)
{
	Sheet->Text=Web->Get(AdressEdit->Text+"?Action=AgentTicketQueue");
	if(Sheet->Text.Pos("My Queues ("))
	{
		for (int i = 0; i < Sheet->Count; i++)
		{
			if(Sheet->Strings[i].Pos("My Queues ("))
			{
				Temp=Sheet->Strings[i].SubString(Sheet->Strings[i].Pos("My Queues (")+11,Sheet->Strings[i].Pos(")</a>")-4);
				Temp=Temp.SubString(0,Temp.Pos(")</a>")-1);
				KolLabel->Text=Temp;
				Temp=Temp.SubString(0,Temp.Pos("/")-1);
				kol=StrToInt(Temp);

				if(kol>StrToInt(KolEdit->Text))
				{
					KolLabel->FontColor=claRed;
                    Player->FileName=FileEdit->Text;
					Player->Play();
					StopButton->Enabled=true;
				}
				return;
			}
		}
	}
	else
	{
		ShowMessage("Authorization error or incorrect OTRS URL!");
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::OpenButtonClick(TObject *Sender)
{
	if(OpenDialog->Execute())
	{
		FileEdit->Text=OpenDialog->FileName;
        Player->FileName=FileEdit->Text;
		PlayButton->Enabled=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PlayButtonClick(TObject *Sender)
{
	Player->Play();
	PlayButton->Enabled=false;
	StopButton->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopButtonClick(TObject *Sender)
{
	Player->Stop();
	StopButton->Enabled=false;
	PlayButton->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ShowPassButtonClick(TObject *Sender)
{
	if(PassEdit->Password==true)
		PassEdit->Password=false;
	else
		PassEdit->Password=true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MenuButtonClick(TObject *Sender)
{
	if(ListBox->Visible==true)
	{
        ListBox->ItemIndex=-1;
		ListBox->Visible=false;
	}
	else
	{
		ListBox->ItemIndex=-1;
		ListBox->Visible=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ForumItemClick(TObject *Sender)
{
	ListBox->Visible=false;
	ShellExecute(NULL, NULL, L"https://alexell.ru/", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::AboutItemClick(TObject *Sender)
{
	ListBox->Visible=false;
	ShowMessage("About OTRS Queues Notifier\n\nVersion: 1.0 (27.04.2016)\nCreator: Alexell Production\n\nAlexell.Ru");

}
//---------------------------------------------------------------------------

