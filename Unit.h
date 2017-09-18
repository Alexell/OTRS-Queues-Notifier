//---------------------------------------------------------------------------

#ifndef UnitH
#define UnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCookieManager.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <FMX.Media.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TIdCookieManager *WebCookie;
	TIdHTTP *Web;
	TTimer *Timer;
	TMediaPlayer *Player;
	TOpenDialog *OpenDialog;
	TGroupBox *AuthBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *AdressEdit;
	TEdit *LoginEdit;
	TEdit *PassEdit;
	TGroupBox *SettingBox;
	TEdit *MinEdit;
	TLabel *Label4;
	TLabel *Label6;
	TEdit *FileEdit;
	TButton *OpenButton;
	TButton *PlayButton;
	TButton *StopButton;
	TButton *StartButton;
	TLabel *TicketLabel;
	TLabel *KolLabel;
	TLabel *Label7;
	TLabel *Label5;
	TLabel *Label8;
	TEdit *KolEdit;
	TButton *ShowPassButton;
	TToolBar *ToolBar;
	TSpeedButton *MenuButton;
	TListBox *ListBox;
	TListBoxItem *ForumItem;
	TListBoxItem *AboutItem;
	void __fastcall StartButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall OpenButtonClick(TObject *Sender);
	void __fastcall StopButtonClick(TObject *Sender);
	void __fastcall PlayButtonClick(TObject *Sender);
	void __fastcall ShowPassButtonClick(TObject *Sender);
	void __fastcall MenuButtonClick(TObject *Sender);
	void __fastcall AboutItemClick(TObject *Sender);
	void __fastcall ForumItemClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
