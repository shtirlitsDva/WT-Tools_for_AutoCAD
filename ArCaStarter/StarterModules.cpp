#include "StdAfx.h"
#include "StarterModules.h"

StarterModules::StarterModules(void)
{
}

StarterModules::~StarterModules(void)
{
}

void StarterModules::LoadModules()
	{
		//		// Set app Path
		//CString appFileName = acedGetAppName();
		//TCHAR dir[_MAX_DIR], drive[_MAX_DRIVE], path[_MAX_PATH];
		//_tsplitpath(appFileName, drive, dir, NULL, NULL);
		//_tmakepath(path, drive, dir, NULL, NULL);
		//m_appPath.Format(_T("%s"),path);	

		//acutPrintf(_T("\n\n\nStarting up modules loading...\n\n"));

		//CString name;
		////Load Modules
		////Commands managed
		///*name = _T("ArCaManaged.dll");
		//if (!LoadModule(name))
		//	return AcRx::kRetError;*/

		////Custon objects
		//name = _T("ArCaArCaObjects.dbx");
		//LoadModule(name);
		//	//return AcRx::kRetError;

		////Property Palette
		//name = _T("ArCaArCaPropertyPalette.dbx");
		//LoadModule(name);
		//	//return AcRx::kRetError;

		////Wrapper for managed
		//name = _T("ArCaArCaWrapper.dll");
		//LoadModule(name);
		//	//return AcRx::kRetError;

		//acutPrintf(_T("\n\n\nAll modules are loaded successful!\n\n"));
	}

// ---------------------------------------------------------------------------
	bool LoadModule(CString& module, BOOL bForceDynamicLinkerLoad = false)
	{
//		CString sFilePath;
//		TCHAR szError[12];
//		module.MakeLower();
//
//		//Load managed module
//		if ((!bForceDynamicLinkerLoad) && (module.Find(_T(".dll"),0) != -1))
//		{
//			sFilePath.Format(_T("%s%s"),m_appPath,module);
//
//			HMODULE hAcMgd = ::GetModuleHandle(_T("ACDBMGD.DLL"));
//			LPCSTR loadDLL = (LPCSTR)_T("LoadManagedDll");
//			funcPtr pLoadMgd = (funcPtr)::GetProcAddress(hAcMgd,"LoadManagedDll");
//			if( pLoadMgd )
//			{
//				Acad::ErrorStatus es = (*pLoadMgd)(sFilePath);
//				if( es != Acad::eOk )
//				{
//// 					wsprintf(szError, _T("0x%08lx"), GetLastError());
//// 					::MessageBox(NULL,szError,module,MB_ICONSTOP);
//					acutPrintf(_T("\n*** Module %s can not be loaded! ***"),module);
//					return false;
//				}
//				else
//				{
//					CString temp = _T("[") + module + _T("]");
//					acutPrintf(_T("\n -> %s loaded!"),temp);
//				}
//			}
//			else
//			{
//				acutPrintf(_T("\nError in getting the function pointer to LoadManagedDll()!"));
//			}			
//		}
//		//Load Unmanaged module
//		else 
//		{
//			sFilePath.Format(_T("%s%s"),m_appPath,module);
//			if (!acrxDynamicLinker->loadModule(sFilePath,true)) {
//				acutPrintf(_T("\n*** Module %s can not be loaded! ***"),module);
//				return false;
//			}
//			else {
//				CString temp = _T("[") + module + _T("]");
//				acutPrintf(_T("\n -> %s loaded!"),temp);
//			}
//		}
		return true;
	}
