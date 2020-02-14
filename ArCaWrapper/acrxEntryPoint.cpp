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
#include "ArCaWrpDynamicArea.h"
#include "ArCaWrpRampa.h"
#include "ArCaWrpGrating.h"
#include "ArCaWrpCota.h"

static AcMgObjectFactoryBase __nogc * __nogc *g_PEs = NULL;

//-----------------------------------------------------------------------------
#define szRDS _RXST("ArCa")
AC_DECLARE_EXTENSION_MODULE(ArCaWrapperDLL) ;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArCaWrapperApp : public AcRxDbxApp {

public:
	CArCaWrapperApp () : AcRxDbxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here
		// Save critical data pointers before running the constructors (see afxdllx.h for details)
		AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
		pModuleState->m_pClassInit = pModuleState->m_classList;
		pModuleState->m_pFactoryInit = pModuleState->m_factoryList;
		pModuleState->m_classList.m_pHead = NULL;
		pModuleState->m_factoryList.m_pHead = NULL;

		ArCaWrapperDLL.AttachInstance (_hdllInstance) ;

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kInitAppMsg (pkt) ;
		
		// create a new object factory array
		static AcMgObjectFactoryBase* g_PEs[] = 
		{
			new AcMgObjectFactory< ArCa::Wrapper::ArCaWrpDynamicArea, ArCaDynamicArea >(), 
			new AcMgObjectFactory< ArCa::Wrapper::ArCaWrpRampa, ArCaRampa >(), 
			new AcMgObjectFactory< ArCa::Wrapper::ArCaWrpGrating, ArCaGrating >(), 
			new AcMgObjectFactory< ArCa::Wrapper::ArCaWrpCota, ArCaCota >(), 
			// end the array with a NULL
			NULL
		};


		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// clean up
		int i=0;
		while (g_PEs[i]!=NULL) {
			delete g_PEs[i++];
		}

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		ArCaWrapperDLL.DetachInstance () ;

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArCaWrapperApp)

