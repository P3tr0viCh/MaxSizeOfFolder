// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <AboutFrm.h>
#include <UtilsLog.h>
#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsFileIni.h>

#include <System.DateUtils.hpp>

#include "MaxSizeOfFolderMain.h"
#include "MaxSizeOfFolderStrings.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMain * Main;

const MB = 1048576;

// ---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miCloseClick(TObject *Sender) {
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miAboutClick(TObject *Sender) {
	miAbout->Enabled = false;

	WriteToLog(IDS_LOG_ABOUT);

	ShowAbout(14, MAXBYTE, MAXBYTE, MAXBYTE, NULL, NULL, NULL, NULL, NULL,
		LoadStr(IDS_COPYRIGHT));

	miAbout->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender) {
	TrayIcon->Visible = true;

	Timer->Interval = 60 * 60 * 1000; // 1 час

	WriteToLog(Format(IDS_LOG_START_PROGRAM, GetFileVer(Application->ExeName)));

	MainFunction();

	Timer->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TrayIconClick(TObject *Sender) {
	Application->BringToFront();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::FormDestroy(TObject *Sender) {
	WriteToLog(IDS_LOG_STOP_PROGRAM);
}

// ---------------------------------------------------------------------------
void __fastcall TMain::TimerTimer(TObject *Sender) {
	MainFunction();
}

// ---------------------------------------------------------------------------
void __fastcall TMain::miStartCheckClick(TObject *Sender) {
	MainFunction();
}

// ---------------------------------------------------------------------------
class TFileSizeAndDate : public TObject {
public:
	__int64 Size;
	TDateTime DateTime;
};

// ---------------------------------------------------------------------------
__int64 FindFiles(String StartFolder, TStringList * Files) {
	TSearchRec SearchRec;
	__int64 Result = 0;

	StartFolder = IncludeTrailingPathDelimiter(StartFolder);

	if (FindFirst(StartFolder + "*.*", faAnyFile, SearchRec) == 0) {
		do {
			if (SearchRec.Name != "." & SearchRec.Name != "..") {
				if (IsValueInWord(SearchRec.Attr, faDirectory)) {
					Result += FindFiles(StartFolder + SearchRec.Name, Files);
				}
				else {
					TFileSizeAndDate * FileSizeAndDate = new TFileSizeAndDate();
					FileSizeAndDate->Size = SearchRec.Size;
					FileSizeAndDate->DateTime = SearchRec.TimeStamp;

					Files->AddObject(StartFolder + SearchRec.Name,
						FileSizeAndDate);

					Result += SearchRec.Size;
				}
			}
		}
		while (FindNext(SearchRec) == 0);

		FindClose(SearchRec);
	}

	return Result;
}

// ---------------------------------------------------------------------------
int __fastcall SortByDateTime(TStringList * List, int Index1, int Index2) {
	return CompareDateTime(((TFileSizeAndDate*) List->Objects[Index1])
		->DateTime, ((TFileSizeAndDate*) List->Objects[Index2])->DateTime);
}

// ---------------------------------------------------------------------------
void TMain::MainFunction() {
	DWORD FirstTickMain = StartTimer();

	WriteToLog(IDS_LOG_START_MAIN_FUNCTION);

	TFileIni * FileIni = TFileIni::GetNewInstance();

	String Folder = FileIni->ReadString("Options", "Folder", "");
	__int64 MaxFolderSize = FileIni->ReadInteger("Options", "MaxSize", 0);
	MaxFolderSize = MaxFolderSize * MB;

	FileIni->Free();

	if (Folder != "" & MaxFolderSize > 0) {

		TStringList * Files = new TStringList();

		DWORD FirstTick = StartTimer();

		// ------------------------------------------------
		__int64 FolderSize = FindFiles(Folder, Files);
		// ------------------------------------------------

		WriteToLog(Format(IDS_LOG_FIND_FILES,
			ARRAYOFCONST((Files->Count, FolderSize / MB,
			StopTimer(FirstTick)))));

		if (FolderSize > MaxFolderSize) {
			__int64 NeedDelete = FolderSize - MaxFolderSize;

			WriteToLog(Format(IDS_LOG_NEED_DELETE,
				ARRAYOFCONST((NeedDelete / MB))));

			FirstTick = StartTimer();

			Files->CustomSort(SortByDateTime);

			WriteToLog(Format(IDS_LOG_SORT_FILES, StopTimer(FirstTick)));

			__int64 DeletedFilesSize = 0;
			__int64 FileSize;
			int DeletedCount = 0;
			String FileName = "";

			FirstTick = StartTimer();

			for (int i = 0; i < Files->Count; i++) {
				FileName = Files[0][i];
				FileSize = ((TFileSizeAndDate*) Files->Objects[i])->Size;

				if (DeleteFile(FileName)) {
					WriteToLog(Format(IDS_LOG_DELETE_OK,
						ARRAYOFCONST((FileName, FileSize / MB))));

					DeletedFilesSize += FileSize;
					DeletedCount++;
				}
				else {
					WriteToLog(Format(IDS_LOG_DELETE_FAIL,
						ARRAYOFCONST((FileName, FileSize / MB))));
				}

				if (DeletedFilesSize >= NeedDelete) {
					break;
				}
			}

			WriteToLog(Format(IDS_LOG_DONE_DELETE,
				ARRAYOFCONST((DeletedCount, DeletedFilesSize / MB,
				StopTimer(FirstTick)))));
		}
		else {
			WriteToLog(IDS_LOG_GOOD_FOLDER_SIZE);
		}

		Files->Free();
	}

	WriteToLog(Format(IDS_LOG_DONE_MAIN_FUNCTION, StopTimer(FirstTickMain)));
}
// ---------------------------------------------------------------------------
