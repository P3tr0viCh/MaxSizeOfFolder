// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include <windows.h>

bool AlreadyRun() {
	return FindWindow(TEXT("TApplication"),
		TEXT("Максимальный размер папки")) != 0;
}

// ---------------------------------------------------------------------------
USEFORM("MaxSizeOfFolderMain.cpp", Main);

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	try {
		if (AlreadyRun()) {
			return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = false;
		Application->ShowMainForm = false;
		Application->Title = "Максимальный размер папки";
		Application->CreateForm(__classid(TMain), &Main);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
