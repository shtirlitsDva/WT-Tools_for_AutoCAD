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
//----- ArCaCOMRampa.h : Declaration of the CArCaCOMRampa
//-----------------------------------------------------------------------------
#pragma once
#include "resource.h"

#include "ArCaPropertyPalette.h"



//----- CArCaCOMRampa
class ATL_NO_VTABLE CArCaCOMRampa : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CArCaCOMRampa, &CLSID_ArCaCOMRampa>,
	public ISupportErrorInfo,
	public IOPMPropertyExtensionImpl<CArCaCOMRampa>,
	public IAcadEntityDispatchImpl<CArCaCOMRampa, &CLSID_ArCaCOMRampa, IArCaCOMRampa, &IID_IArCaCOMRampa, &LIBID_ArCaArCaPropertyPaletteLib>
	//- Please do not remove the following line. It is here to make the Visual Studio ATL Wizards
	//- running properly. The class will not compile but is required by Visual Studio to recognize
	//-	this class as being an ATL class
	//- Uncomment the following line when you want to use the ATL Wizards to add a member or method
	//- to its interface.
	//,public IDispatchImpl<Ighfghgfh, &IID_IIghfghgfh, &LIBID_adskArxProject1Lib>
{
public:
	CArCaCOMRampa () {
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ArCaCOMRampa)

	BEGIN_COM_MAP(CArCaCOMRampa)
		COM_INTERFACE_ENTRY(IArCaCOMRampa)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IOPMPropertyExtension)
		COM_INTERFACE_ENTRY(ICategorizeProperties)
		COM_INTERFACE_ENTRY(IPerPropertyBrowsing)
		COM_INTERFACE_ENTRY(IAcadBaseObject)
		COM_INTERFACE_ENTRY(IAcadBaseObject2)
		COM_INTERFACE_ENTRY(IAcadObject)
		COM_INTERFACE_ENTRY(IAcadEntity)
		COM_INTERFACE_ENTRY(IRetrieveApplication)
	END_COM_MAP()

	//----- ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct () {
		return (S_OK) ;
	}
	
	void FinalRelease () {
	}

	//IAcadBaseObjectImpl
	virtual HRESULT CreateNewObject (AcDbObjectId &objId, AcDbObjectId &ownerId, TCHAR *keyName);
	//IAcadBaseObject2Impl
	STDMETHOD(ForceDbResident)(VARIANT_BOOL *forceDbResident) ;
	STDMETHOD(CreateObject)(AcDbObjectId ownerId =AcDbObjectId::kNull, TCHAR *keyName =NULL);
	STDMETHOD(AddToDb)(AcDbObjectId &objId, AcDbObjectId ownerId =AcDbObjectId::kNull, TCHAR *keyName =NULL);

	//IOPMPropertyExtension
	BEGIN_OPMPROP_MAP()
			OPMPROP_ENTRY(0, 1, 1, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
			OPMPROP_ENTRY(0, 2, 1, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
	END_OPMPROP_MAP()
	//IOPMPropertyExtensionImpl
	virtual HINSTANCE GetResourceInstance () { return (_hdllInstance) ; }

	//- IAcPiCategorizeProperties
	STDMETHOD(GetCategoryName) (PROPCAT propcat, LCID lcid, BSTR *pbstrName) ;
	STDMETHOD(GetParentCategory) (PROPCAT CatID, PROPCAT *pParentCatID) ;

public:
	//IArCaCOMRampa
	STDMETHOD(get_Height)(double* pVal);
	STDMETHOD(put_Height)(double newpVal);

	STDMETHOD(get_Scale)(double* pVal);
	STDMETHOD(put_Scale)(double newpVal);
} ;

OBJECT_ENTRY_AUTO(__uuidof(ArCaCOMRampa), CArCaCOMRampa)
