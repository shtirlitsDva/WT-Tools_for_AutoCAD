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
//----- ArCaCOMCota.h : Declaration of the CArCaCOMCota
//-----------------------------------------------------------------------------
#pragma once
#include "resource.h"

#include "ArCaPropertyPalette.h"

// DISPIDs for resources
#define DISPID_TYPE			  0x00000001
#define DISPID_VALUE		  0x00000002
#define DISPID_SCALE_FACTOR   0x00000003
#define DISPID_DYNAMIC_QUOTE  0x00000004


//----- CArCaCOMCota
class ATL_NO_VTABLE CArCaCOMCota : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CArCaCOMCota, &CLSID_ArCaCOMCota>,
	public ISupportErrorInfo,
	public IOPMPropertyExtensionImpl<CArCaCOMCota>,
	public IAcadEntityDispatchImpl<CArCaCOMCota, &CLSID_ArCaCOMCota, IArCaCOMCota, &IID_IArCaCOMCota, &LIBID_ArCaArCaPropertyPaletteLib>
	//- Please do not remove the following line. It is here to make the Visual Studio ATL Wizards
	//- running properly. The class will not compile but is required by Visual Studio to recognize
	//-	this class as being an ATL class
	//- Uncomment the following line when you want to use the ATL Wizards to add a member or method
	//- to its interface.
	//,public IDispatchImpl<Ighfghgfh, &IID_IIghfghgfh, &LIBID_adskArxProject1Lib>

{
public:
	CArCaCOMCota () {
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ARCACOMCOTA)

	BEGIN_COM_MAP(CArCaCOMCota)
		COM_INTERFACE_ENTRY(IArCaCOMCota)
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
		    OPMPROP_ENTRY(0, DISPID_TYPE, 1, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
			OPMPROP_ENTRY(0, DISPID_VALUE, 1, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
			OPMPROP_ENTRY(0, DISPID_SCALE_FACTOR, 1, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
			OPMPROP_ENTRY(0, DISPID_DYNAMIC_QUOTE, 1, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
	END_OPMPROP_MAP()
	//IOPMPropertyExtensionImpl
	virtual HINSTANCE GetResourceInstance () { return (_hdllInstance) ; }
	//- IAcPiCategorizeProperties
	STDMETHOD(GetCategoryName) (PROPCAT propcat, LCID lcid, BSTR *pbstrName) ;
	//Properties name
	STDMETHOD(GetDisplayName) (DISPID dispID, BSTR *propName);
	//Combo Box
	STDMETHOD(GetPredefinedStrings)(
        DISPID dispID,	// [in]
        CALPOLESTR *pCaStringsOut,	// [out]
        CADWORD *pCaCookiesOut);	// [out]

	STDMETHOD(GetPredefinedValue)(
        DISPID dispID,	// [in] 
        DWORD dwCookie,	// [out] 
        VARIANT *pVarOut);	// [out]
public:
	//IArCaCOMCota
	STDMETHOD(get_Type)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Type)(/*[in]*/ BSTR newVal);

	STDMETHOD(get_Value)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_Value)(/*[in]*/ double newVal);

	STDMETHOD(get_ScaleFactor)(/*[out, retval]*/ double *pVal);
	STDMETHOD(put_ScaleFactor)(/*[in]*/ double newVal);

	STDMETHOD(get_DynamicQuote)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_DynamicQuote)(/*[in]*/ BSTR newVal);
} ;

OBJECT_ENTRY_AUTO(__uuidof(ArCaCOMCota), CArCaCOMCota)
