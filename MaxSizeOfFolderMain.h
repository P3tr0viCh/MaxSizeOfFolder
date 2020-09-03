// ---------------------------------------------------------------------------

#ifndef MaxSizeOfFolderMainH
#define MaxSizeOfFolderMainH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TMain : public TForm {
__published: // IDE-managed Components
	TPopupMenu *PopupMenu;
	TMenuItem *miAbout;
	TMenuItem *miClose;
	TMenuItem *miSeparator01;
	TTrayIcon *TrayIcon;
	TTimer *Timer;
	TMenuItem *miStartCheck;
	TMenuItem *miSeparator02;

	void __fastcall miCloseClick(TObject *Sender);
	void __fastcall miAboutClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TrayIconClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall miStartCheckClick(TObject *Sender);

private: // User declarations
	void MainFunction();

public: // User declarations
	__fastcall TMain(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
// ---------------------------------------------------------------------------
#endif
