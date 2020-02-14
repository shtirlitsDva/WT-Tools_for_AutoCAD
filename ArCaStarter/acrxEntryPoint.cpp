// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "..\ArCaObjects\ArCaWindow.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ArCa")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArCaStarterApp : public AcRxArxApp {
protected:
	CString m_appPath;

public:
	CArCaStarterApp () : AcRxArxApp () {}

	//Loadator for "LoadManagedDll()"
	typedef Acad::ErrorStatus (__stdcall* funcPtr)(const TCHAR* fname);

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {

		// Set app Path
		CString appFileName = acedGetAppName();
		TCHAR dir[_MAX_DIR], drive[_MAX_DRIVE], path[_MAX_PATH];
		_tsplitpath(appFileName, drive, dir, NULL, NULL);
		_tmakepath(path, drive, dir, NULL, NULL);
		m_appPath.Format(_T("%s"),path);	

		acutPrintf(_T("\n\n\nStarting up modules loading...\n\n"));

		CString name;
		//Load Modules
		//Commands managed
		// Carrega a Ribbon mas não registra os commandos .NET.
		//name = _T("WT-CAD.dll");
		//if (!LoadModule(name))
		//	return AcRx::kRetError;

		//Custon objects
		name = _T("ArCaArCaObjects.dbx");
		if (!LoadModule(name))
			return AcRx::kRetError;

		//Property Palette
		name = _T("ArCaArCaPropertyPalette.dbx");
		if (!LoadModule(name))
			return AcRx::kRetError;

		//Wrapper for managed
		name = _T("ArCaArCaWrapper.dll");
		if (!LoadModule(name))
			return AcRx::kRetError;

		acutPrintf(_T("\n\n\nAll modules are loaded successful!\n\n"));

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	// ---------------------------------------------------------------------------
	bool LoadModule(CString& module, BOOL bForceDynamicLinkerLoad = false)
	{
		CString sFilePath;
		TCHAR szError[12];
		module.MakeLower();

		//Load managed module
		if ((!bForceDynamicLinkerLoad) && (module.Find(_T(".dll"),0) != -1))
		{
			sFilePath.Format(_T("%s%s"),m_appPath,module);

			HMODULE hAcMgd = ::GetModuleHandle(_T("ACDBMGD.DLL"));
			LPCSTR loadDLL = (LPCSTR)_T("LoadManagedDll");
			funcPtr pLoadMgd = (funcPtr)::GetProcAddress(hAcMgd,"LoadManagedDll");
			if( pLoadMgd )
			{
				Acad::ErrorStatus es = (*pLoadMgd)(sFilePath);
				if( es != Acad::eOk )
				{
// 					wsprintf(szError, _T("0x%08lx"), GetLastError());
// 					::MessageBox(NULL,szError,module,MB_ICONSTOP);
					acutPrintf(_T("\n*** Module %s can not be loaded! ***"),module);
					return false;
				}
				else
				{
					CString temp = _T("[") + module + _T("]");
					acutPrintf(_T("\n -> %s loaded!"),temp);
				}
			}
			else
			{
				acutPrintf(_T("\nError in getting the function pointer to LoadManagedDll()!"));
			}			
		}
		//Load Unmanaged module
		else 
		{
			sFilePath.Format(_T("%s%s"),m_appPath,module);
			if (!acrxDynamicLinker->loadModule(sFilePath,true)) {
				acutPrintf(_T("\n*** Module %s can not be loaded! ***"),module);
				return false;
			}
			else {
				CString temp = _T("[") + module + _T("]");
				acutPrintf(_T("\n -> %s loaded!"),temp);
			}
		}
		return true;
	}

	virtual void RegisterServerComponents () {
	}


	// - ArCaArCaStarter.test1 command (do not rename)
	static void ArCaArCaStartertest1(void)
	{
 		// Input information
 		ads_point pt1, pt2;

 		if (acedGetPoint(NULL,_T("Set the first point:\n"),pt1) != RTNORM)
 			return;
		 if (acedGetPoint(NULL,_T("Set the second point:\n"),pt2) != RTNORM)
 			return;

		ArCaWindow *pWin = new ArCaWindow();
		pWin->put__p3dLeftLower(asPnt3d(pt1));
		pWin->put__p3dRightLower(asPnt3d(pt2));

 		// Post to Database
 		AcDbBlockTable *pBlockTable;
 		acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable,
 			AcDb::kForRead);

 		AcDbBlockTableRecord *pBlockTableRecord;
 		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,AcDb::kForWrite);
 		pBlockTable->close();
 
 		AcDbObjectId retId = AcDbObjectId::kNull;
 		pBlockTableRecord->appendAcDbEntity(retId, pWin);
 		pBlockTableRecord->close();
 		pWin->close();
	}

	public:

	// ----- AsdkArxProject2._test command
	static void AsdkArxProject2_test(void)
	{
				// Add your code for command AsdkArxProject2._cmd1 here
		AcDbDatabase *p =new AcDbDatabase (false, false) ;
		{
			p->readDwgFile (_T("C:\\drawing1.dwg")) ;
			p->setRetainOriginalThumbnailBitmap (true) ;
			AcDbObjectPointer<AcDbLine> pLine ;
			pLine.create () ;
			pLine->setEndPoint (AcGePoint3d (10, 10, 0)) ;
			AcDbBlockTableRecordPointer pMs (ACDB_MODEL_SPACE, p, AcDb::kForWrite) ;
			pMs->appendAcDbEntity (pLine.object ()) ;
		}
		::DeleteFile (_T("C:\\testout0.dwg")) ;
		p->saveAs (_T("C:\\testout0.dwg")) ;
		delete p ;
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArCaStarterApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CArCaStarterApp, ArCaArCaStarter, test1, test1, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArCaStarterApp, AsdkArxProject2, _test, test, ACRX_CMD_TRANSPARENT, NULL)
