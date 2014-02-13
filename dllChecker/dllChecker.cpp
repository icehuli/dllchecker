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
using namespace System::IO;
using namespace System::Security::AccessControl;
using namespace System::Security;
using namespace System::Security::Permissions;
using namespace System::Security::Principal;
using namespace System::Windows::Forms;


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

VALUE DllChecker::IsDataFolderWritable(VALUE sodDllDir)
{
    char* words = StringValueCStr(sodDllDir);

    String^ folder = System::IO::Path::GetFullPath(gcnew String(words));

    // Create a new DirectoryInfo object.
    DirectoryInfo^ dInfo = gcnew DirectoryInfo(folder);
    // Get a DirectorySecurity object that represents the
    // current security settings.
    DirectorySecurity^ dSecurity = dInfo->GetAccessControl();
    //FileSystemAccessRule(@"Everyone", FileSystemRights.FullControl, AccessControlType.Allow
    //"S-1-5-32-545" == "Users"
    //FileSystemAccessRule^ administratorRule = gcnew FileSystemAccessRule("S-1-5-32-545", FileSystemRights::FullControl, AccessControlType::Allow);
    SecurityIdentifier^ sid = gcnew SecurityIdentifier("S-1-5-32-545");
    /* Interestingly, SDDL aliases work here. */
        //(NTAccount)sidIf.Translate(typeof(NTAccount));
    
    AuthorizationRuleCollection^ accessRules = dSecurity->GetAccessRules(true, true, Principal::SecurityIdentifier::typeid);
    bool canCreateDirectories = false;
    bool canCreateFiles = false;
    bool canModify = false;

    int isDataFolderWritableRights = 0;
    for (int x = 0; x < accessRules->Count; x++)
    {
        FileSystemAccessRule^ currentRule = (FileSystemAccessRule^)(accessRules[x]);

        SecurityIdentifier^ currentSid = (SecurityIdentifier^)(currentRule->IdentityReference->Translate(SecurityIdentifier::typeid));
        if (currentSid == sid && currentRule->AccessControlType == AccessControlType::Allow)
        {

            if ((currentRule->FileSystemRights & FileSystemRights::CreateDirectories) == FileSystemRights::CreateDirectories)
            {
                canCreateDirectories = true;
            }

            if ((currentRule->FileSystemRights & FileSystemRights::CreateFiles) == FileSystemRights::CreateFiles)
            {
                canCreateFiles = true;
            }

            if ((currentRule->FileSystemRights & FileSystemRights::Modify) == FileSystemRights::Modify)
            {
                canModify = true;
            }
            
        }
        else if (currentSid == sid && currentRule->AccessControlType == AccessControlType::Deny)
        {
            if ((currentRule->FileSystemRights & FileSystemRights::CreateDirectories) == FileSystemRights::CreateDirectories
                || (currentRule->FileSystemRights & FileSystemRights::CreateFiles) == FileSystemRights::CreateFiles
                || (currentRule->FileSystemRights & FileSystemRights::Modify) == FileSystemRights::Modify
                )
            {
                return Qfalse;
            }
        }
    }

    return (
        canCreateDirectories && 
        canCreateFiles && 
        canModify) ? Qtrue : Qfalse;

}

DllChecker::DllChecker()
{
}
DllChecker::~DllChecker()
{
}


