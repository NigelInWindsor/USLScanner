
#pragma once

const int g_iMonitorCountMax=4;
const int g_iWindowCountMax=16;//only the last windows are saved.

class UTKERNEL_API CUTKernelLayoutFile
{
//This class is used to save the window position of setup dialog and windows.
//The file where the windows are saved in named "LayoutXXX.txt"
//this file is in the same folder than the executable
//"XXX" is the name of the exe file.
//In this file the monitor configuration is saved.
//Each time the monitor configuration is different than the one
//that has been saved in the file, the layout file is automatically deleted.
//Each window is registered with its name (window title), it is
//also possible to register the window with a different name.
//The window name "MainWnd" is reserved for the main window.
//Only a maximum of "g_iWindowCountMax" could be registered in the file.
//When "g_iWindowCountMax" have already been registered then
//the last window on which no activity has been notified
//(via "WritePosition") is released and the new window
//is registered at the same index.
//It is not possible to release the main window.
public:
	CUTKernelLayoutFile(const wchar_t *pName=NULL);
	~CUTKernelLayoutFile();

	bool RegisterMainWnd(int &iWindowIndex,bool &bRegistered,bool &bCreateNew);
					//see comment of "RegisterWindow"
					//the name of th emain window is predefined and reserved
					//this name is "MainWnd"
					//For the main window you call "WritePositionMainWnd" and "ReadPositionMainWnd"
					//without any title (name).
	bool RegisterWindow(const wchar_t *pName,int &iWindowIndex,bool &bRegistered,bool &bCreateNew);
					//output bRegistered :
					//	- true if a window with the same title has already been registered,
					//		in this case you can get extra information with function "ReadPosition".
					//	- false if a window with the same title has not been registered.
					//output bCreateNew :
					//	- true if a new indew has been created in the list.
					//	- false if the last indew has been released in the list.
					//		in this case it will not be possible to use "WritePosition"
					//		for this old window.
					//IMPORTANT : In fact the name for the main window is predefined "MainWnd"
					//this name is reserved, the main window canno't be overwritten
					//by another window even if its time is the last one.
	bool WritePositionMainWnd(int iWindowIndex,
					int iResHorz,int iResVert,
					const WINDOWPLACEMENT &wndPlacement,int iCustomData);
	bool ReadPositionMainWnd(int iWindowIndex,
					SYSTEMTIME &sysTime,
					int &iResHorz,int &iResVert,
					WINDOWPLACEMENT &wndPlacement,int &iCustomData);
	bool WritePositionWindow(int iWindowIndex,const wchar_t *pName,//name is not required for the main window (NULL).
					int iResHorz,int iResVert,
					const WINDOWPLACEMENT &wndPlacement,int iCustomData);
	bool ReadPositionWindow(int iWindowIndex,const wchar_t *pName,//name is not required for the main window (NULL).
					SYSTEMTIME &sysTime,
					int &iResHorz,int &iResVert,
					WINDOWPLACEMENT &wndPlacement,int &iCustomData);
	//to get main features of your window by calling operating system (Windows)
	bool GetOperatingSystemMainWnd(HWND hWnd,
					int &iResHorz,int &iResVert,
					WINDOWPLACEMENT &wndPlacement);//if your window didn't have a title then call this functino with "pTitle=NULL" in order to not have error.
	bool GetOperatingSystemWindow(HWND hWnd,int iSize,wchar_t *pTitle,
					int &iResHorz,int &iResVert,
					WINDOWPLACEMENT &wndPlacement);//if your window didn't have a title then call this functino with "pTitle=NULL" in order to not have error.
	//Section name of window in the window list file.
	void SetSectionName(wchar_t *pName);//default section name is application name.
	const wchar_t *GetSectionName();
	
	int GetWindowCount();
	bool GetWindowTitle(int iWindowIndex,int iBufferSize,wchar_t *pBufferTitle);

	static bool CheckPlacement(WINDOWPLACEMENT &wndPlacement);

};

UTKERNEL_API CUTKernelLayoutFile* UTKernel_LayoutFile();

