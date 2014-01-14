/* File : example.c */

#include "dllChecker.h"

/* Move the shape to a new location */
#include <Windows.h>
#include <cstdlib>

#include <direct.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>

//#include <iostream>


#define GetCurrentDir _getcwd

using namespace std;


#include <msclr/marshal.h>
using namespace msclr::interop;
using namespace System;
using namespace System::Runtime::InteropServices;
VALUE SystemString2RString(System::String^ systemString)
{
	char* cstr = (char*)Marshal::StringToHGlobalAnsi(systemString).ToPointer();
	VALUE rstr = rb_str_new2(cstr);
	Marshal::FreeHGlobal((IntPtr)cstr);
	return rstr;
}

System::String^ RString2SystemString(VALUE rstr)
{
	return gcnew System::String(StringValueCStr(rstr));
}

void OpenCopyDllsWindows(WCHAR* sodDllDirStr, WCHAR* pszParseName)
{
	SHELLEXECUTEINFO ShExecInfo;
	//ppenum->Release();
	WCHAR fullDllDirPath[MAX_PATH] = L"\"";
	wcscat_s(fullDllDirPath, sodDllDirStr);
	wcscat_s(fullDllDirPath, L"\"");
	//if(fWPF)
	{
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = L"runas";
		ShExecInfo.lpFile = pszParseName;
		ShExecInfo.lpParameters = fullDllDirPath;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = NULL;
		
		//System::Windows::Forms::MessageBox::Show(gcnew System::String(ShExecInfo.lpParameters));
		ShellExecuteEx(&ShExecInfo);
		PostQuitMessage(NULL);
	}
}


void DllChecker::launchDllsInstaller(VALUE sodDllDir, VALUE exeName)
{
	VALUE str = StringValue(sodDllDir);
	char* words = StringValueCStr(sodDllDir);
	long  len   = RSTRING(str)->len;
	size_t cSize = len;
	WCHAR sodDllDirStr[MAX_PATH] = L"";
	size_t cSizeCoverted = cSize;
	mbstowcs_s (&cSizeCoverted, sodDllDirStr, words, MAX_PATH);
	
	VALUE exeNamestr = StringValue(exeName);
	char* exeNamewords = StringValueCStr(exeName);
	long  exeNamelen   = RSTRING(exeNamestr)->len;
	size_t exeNameSize = exeNamelen;
	WCHAR wExeName[MAX_PATH] = L"";
	size_t exeNameSizeCoverted = exeNameSize;
	mbstowcs_s (&exeNameSizeCoverted, wExeName, exeNamewords, MAX_PATH);
	
	WCHAR wFullExeName[MAX_PATH] = L"";
	wcscat_s(wFullExeName, sodDllDirStr);
	wcscat_s(wFullExeName, wExeName);
	OpenCopyDllsWindows(sodDllDirStr, wFullExeName);
}

DllChecker::DllChecker()
{
}
DllChecker::~DllChecker()
{
}


