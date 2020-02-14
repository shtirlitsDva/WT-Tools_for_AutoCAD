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
//----- ArCaCOMCota.cpp : Implementation of CArCaCOMCota
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaCOMCota.h"
#include "..\ArCaObjects\ArCaCota.h"

//----- CArCaCOMCota
//-----------------------------------------------------------------------------
STDMETHODIMP CArCaCOMCota::InterfaceSupportsErrorInfo (REFIID riid) {
	static const IID * arr [] ={
		&IID_IArCaCOMCota
	} ;

	for ( int i =0 ; i < sizeof (arr) / sizeof (arr [0]) ; i++ ) {
		if ( InlineIsEqualGUID (*arr [i], riid) )
			return (S_OK) ;
	}
	return (S_FALSE) ;
}

//-----------------------------------------------------------------------------
//IAcadBaseObjectImpl
HRESULT CArCaCOMCota::CreateNewObject (AcDbObjectId &objId, AcDbObjectId &ownerId, TCHAR *keyName) {
	try {
		HRESULT hr ;
		if ( FAILED(hr =CreateObject (ownerId, keyName)) )
			throw hr ;
		if ( FAILED(hr =AddToDb (objId, ownerId, keyName)) )
			throw hr ;
	} catch( HRESULT hr ) {
		return (hr) ;
	}
	return (S_OK) ;
}

//-----------------------------------------------------------------------------
//IAcadBaseObject2Impl
STDMETHODIMP CArCaCOMCota::ForceDbResident (VARIANT_BOOL *forceDbResident) {
	if ( forceDbResident == NULL )
		return (E_POINTER) ;
	//----- Return ACAX_VARIANT_TRUE if the object must be database resident to be functional
	*forceDbResident =ACAX_VARIANT_FALSE ;
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::CreateObject (AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/) {
	try {
		Acad::ErrorStatus es ;
		AcDbObjectPointer<ArCaCota> obj ;
		if ( (es =obj.create ()) != Acad::eOk )
			throw es ;

		obj->setDatabaseDefaults (ownerId.database ()) ;

		ArCaCota *pTmp =NULL ;
		obj.release (pTmp) ;

		SetObject ((AcDbObject *&)pTmp) ;
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to create ArCaCota", IID_IArCaCOMCota, E_FAIL)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::AddToDb (AcDbObjectId &objId, AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/) {
	try {
		AcAxDocLock docLock (ownerId) ;

		Acad::ErrorStatus es ;
		AcAxObjectRefPtr<ArCaCota> obj (&m_objRef, AcDb::kForRead) ;

		//----- Non-entities cannot be owned by block table record, but are
		//----- usually owned by dictionaries. If this is not your case, please
		//----- change the code below.
		AcDbDictionaryPointer pDictionary (ownerId, AcDb::kForWrite, true) ;
		if ( (es =pDictionary.openStatus ()) != Acad::eOk )
			throw es ;

		if ( (es =pDictionary->setAt (keyName, obj.object (), objId)) != Acad::eOk )
			throw es ;
	} catch( const Acad::ErrorStatus ) {
		return (Error(L"Failed to add ArCaCota to database", IID_IArCaCOMCota, E_FAIL)) ;
	}
	return (SetObjectId (objId)) ;
}

STDMETHODIMP CArCaCOMCota::get_Type(BSTR * pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
			
		if (pEntity->get__kType() == ArCaCota::CotaType::kPlane)
				*pVal = ::SysAllocString(L"Plane");
		else if (pEntity->get__kType() == ArCaCota::CotaType::kArrow)
				*pVal = ::SysAllocString(L"Arrow");
		else if (pEntity->get__kType() == ArCaCota::CotaType::kLeader)
				*pVal = ::SysAllocString(L"Leader");

	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::put_Type(BSTR newVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		if (wcscmp(newVal, L"Plane") == 0)
				pEntity->put__kType(ArCaCota::CotaType::kPlane);
		else if (wcscmp(newVal, L"Arrow") == 0)
				pEntity->put__kType(ArCaCota::CotaType::kArrow);
		else if (wcscmp(newVal, L"Leader") == 0)
				pEntity->put__kType(ArCaCota::CotaType::kLeader);

		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}


STDMETHODIMP CArCaCOMCota::get_Value(double * pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
			
		*pVal = pEntity->get__dCota();

	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::put_Value(double newVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		pEntity->put__dCota(newVal);

		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::get_ScaleFactor(double * pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
			
		*pVal = pEntity->get__dScaleFactor();

	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::put_ScaleFactor(double newVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		pEntity->put__dScaleFactor(newVal);

		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::get_DynamicQuote(BSTR * pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		if (pEntity->get__bDynamicQuote())
			*pVal = ::SysAllocString(L"True");
		else
			*pVal = ::SysAllocString(L"False");

	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::put_DynamicQuote(BSTR newVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaCota> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		if (wcscmp(newVal, L"True") == 0)
			pEntity->put__bDynamicQuote(true);
		else
			pEntity->put__bDynamicQuote(false);

		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMCota, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMCota, hr)) ;
	}
	return (S_OK) ;
}
// OPM calls this function for each property to obtain a list of strings and cookies if they are available.
// This function is declared on the IPerPropertyBrowsing interface. Our IOPMPropertyExtensionImpl
// class implements this member by reading the values in the OPM property map.
STDMETHODIMP CArCaCOMCota::GetPredefinedStrings(DISPID dispID, CALPOLESTR *pCaStringsOut, CADWORD *pCaCookiesOut)
{
    if (dispID == DISPID_TYPE)
	{   
		pCaStringsOut->pElems = (LPOLESTR *)::CoTaskMemAlloc(sizeof(LPOLESTR) * 3);
		pCaCookiesOut->pElems = (DWORD *)::CoTaskMemAlloc(sizeof(DWORD) * 3);

		pCaStringsOut->pElems[0] = ::SysAllocString(L"Plane");
		pCaCookiesOut->pElems[0] = 1;
		pCaStringsOut->pElems[1] = ::SysAllocString(L"Arrow");
		pCaCookiesOut->pElems[1] = 2;
		pCaStringsOut->pElems[2] = ::SysAllocString(L"Leader");
		pCaCookiesOut->pElems[2] = 3;

		pCaStringsOut->cElems = 3;
		pCaCookiesOut->cElems = 3;
	}
	else if (dispID == DISPID_DYNAMIC_QUOTE)
	{
		pCaStringsOut->pElems = (LPOLESTR *)::CoTaskMemAlloc(sizeof(LPOLESTR) * 2);
		pCaCookiesOut->pElems = (DWORD *)::CoTaskMemAlloc(sizeof(DWORD) * 2);

		pCaStringsOut->pElems[0] = ::SysAllocString(L"True");
		pCaCookiesOut->pElems[0] = 1;
		pCaStringsOut->pElems[1] = ::SysAllocString(L"False");
		pCaCookiesOut->pElems[1] = 2;

		pCaStringsOut->cElems = 2;
		pCaCookiesOut->cElems = 2;
	}
	else
		return  IOPMPropertyExtensionImpl<CArCaCOMCota>::GetPredefinedStrings(dispID,pCaStringsOut,pCaCookiesOut);
    
    return S_OK;
}

// OPM calls this function when the user selects an element from a drop down list. OPM provides
// the cookie that we associated with the element in the GetPredefinedStrings function. We are
// responsible for mapping this cookie back to a value that the properties corresponding put_ function
// can understand. 
// In this particular case all we need to do is to provide the name of the text style as
// the put_TextStyle method needs that.
STDMETHODIMP CArCaCOMCota::GetPredefinedValue(DISPID dispID, DWORD dwCookie, VARIANT *pVarOut)
{
	if (dispID == DISPID_TYPE)
	{   
		switch ( dwCookie )
		{
		case 1:
			::VariantCopy(pVarOut,&CComVariant(L"Plane"));
			break;
		case 2:
			::VariantCopy(pVarOut,&CComVariant(L"Arrow"));
			break;
		case 3:
			::VariantCopy(pVarOut,&CComVariant(L"Leader"));
			break;
		default:
			return S_FALSE;
		}
	}
	else if (dispID == DISPID_DYNAMIC_QUOTE)
	{
		switch ( dwCookie )
		{
		case 1:
			::VariantCopy(pVarOut,&CComVariant(L"True"));
			break;
		case 2:
			::VariantCopy(pVarOut,&CComVariant(L"False"));
			break;
		default:
			return S_FALSE;
		}
	}
	else
		return  IOPMPropertyExtensionImpl<CArCaCOMCota>::GetPredefinedValue(dispID,dwCookie, pVarOut);
}

// Get the name for the category ID
STDMETHODIMP CArCaCOMCota::GetCategoryName(
	/* [in]  */ PROPCAT propcat, 
	/* [in]  */ LCID lcid,
	/* [out] */ BSTR* pBstrName) 
{
	if (pBstrName==NULL)
		return E_POINTER;

	*pBstrName =::SysAllocString (L"Custon Property") ;
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMCota::GetDisplayName(DISPID dispID, BSTR *propName)
{
	if (propName==NULL)
		return E_POINTER;
	switch ( dispID ) {
		case DISPID_TYPE:
			*propName =::SysAllocString (L"Type Quota:") ;
			break ;
		case DISPID_VALUE:
			*propName =::SysAllocString (L"Value:") ;
			break ;
		case DISPID_SCALE_FACTOR:
			*propName =::SysAllocString (L"Scale:") ;
			break ;
		case DISPID_DYNAMIC_QUOTE:
			*propName =::SysAllocString (L"Dynamic Quote:") ;
			break ;
		default:
			return (E_NOTIMPL) ;
	}
	return S_OK; 
}