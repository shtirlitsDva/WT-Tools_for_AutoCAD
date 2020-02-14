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
//----- ArCaCOMGrating.cpp : Implementation of CArCaCOMGrating
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaCOMGrating.h"
#include "..\ArCaObjects\ArCaGrating.h"

//----- CArCaCOMGrating
//-----------------------------------------------------------------------------
STDMETHODIMP CArCaCOMGrating::InterfaceSupportsErrorInfo (REFIID riid) {
	static const IID * arr [] ={
		&IID_IArCaCOMGrating
	} ;

	for ( int i =0 ; i < sizeof (arr) / sizeof (arr [0]) ; i++ ) {
		if ( InlineIsEqualGUID (*arr [i], riid) )
			return (S_OK) ;
	}
	return (S_FALSE) ;
}

//-----------------------------------------------------------------------------
//IAcadBaseObjectImpl
HRESULT CArCaCOMGrating::CreateNewObject (AcDbObjectId &objId, AcDbObjectId &ownerId, TCHAR *keyName) {
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
STDMETHODIMP CArCaCOMGrating::ForceDbResident (VARIANT_BOOL *forceDbResident) {
	if ( forceDbResident == NULL )
		return (E_POINTER) ;
	//----- Return ACAX_VARIANT_TRUE if the object must be database resident to be functional
	*forceDbResident =ACAX_VARIANT_FALSE ;
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::CreateObject (AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/) {
	try {
		Acad::ErrorStatus es ;
		AcDbObjectPointer<ArCaGrating> obj ;
		if ( (es =obj.create ()) != Acad::eOk )
			throw es ;

		obj->setDatabaseDefaults (ownerId.database ()) ;

		ArCaGrating *pTmp =NULL ;
		obj.release (pTmp) ;

		SetObject ((AcDbObject *&)pTmp) ;
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to create ArCaGrating", IID_IArCaCOMGrating, E_FAIL)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::AddToDb (AcDbObjectId &objId, AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/) {
	try {
		AcAxDocLock docLock (ownerId) ;

		Acad::ErrorStatus es ;
		AcAxObjectRefPtr<ArCaGrating> obj (&m_objRef, AcDb::kForRead) ;

		//----- Non-entities cannot be owned by block table record, but are
		//----- usually owned by dictionaries. If this is not your case, please
		//----- change the code below.
		AcDbDictionaryPointer pDictionary (ownerId, AcDb::kForWrite, true) ;
		if ( (es =pDictionary.openStatus ()) != Acad::eOk )
			throw es ;

		if ( (es =pDictionary->setAt (keyName, obj.object (), objId)) != Acad::eOk )
			throw es ;
	} catch( const Acad::ErrorStatus ) {
		return (Error(L"Failed to add ArCaGrating to database", IID_IArCaCOMGrating, E_FAIL)) ;
	}
	return (SetObjectId (objId)) ;
}

STDMETHODIMP CArCaCOMGrating::get_dOpening(double* pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		*pVal =pEntity->get__dOpening();
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::put_dOpening(double newpVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		pEntity->put__dOpening(newpVal);
		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::get_dGridVerThickness(double* pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		*pVal =pEntity->get__dGridVerThickness();
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::put_dGridVerThickness(double newpVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		pEntity->put__dGridVerThickness(newpVal);
		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::get_dGridHorThickness(double* pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		*pVal =pEntity->get__dGridHorThickness();
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::put_dGridHorThickness(double newpVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		pEntity->put__dGridHorThickness(newpVal);
		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}


STDMETHODIMP CArCaCOMGrating::get_iGridHorQuantiy(int* pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		*pVal =pEntity->get__iGridHorQuantiy();
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::put_iGridHorQuantiy(int newpVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		pEntity->put__iGridHorQuantiy(newpVal);
		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}


STDMETHODIMP CArCaCOMGrating::get_TypeTipTop(BSTR * pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
			
		if (pEntity->get__kTipTypeTop() == ArCaGrating::TipType::kPlane)
				*pVal = ::SysAllocString(L"Plane");
		else if (pEntity->get__kTipTypeTop() == ArCaGrating::TipType::kArrow)
				*pVal = ::SysAllocString(L"Arrow");
		else if (pEntity->get__kTipTypeTop() == ArCaGrating::TipType::kPointed)
				*pVal = ::SysAllocString(L"Pointed");
		else if (pEntity->get__kTipTypeTop() == ArCaGrating::TipType::kRound)
				*pVal = ::SysAllocString(L"Round");

	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::put_TypeTipTop(BSTR newVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		if (wcscmp(newVal, L"Plane") == 0)
				pEntity->put__kTipTypeTop(ArCaGrating::TipType::kPlane);
		else if (wcscmp(newVal, L"Arrow") == 0)
				pEntity->put__kTipTypeTop(ArCaGrating::TipType::kArrow);
		else if (wcscmp(newVal, L"Pointed") == 0)
			pEntity->put__kTipTypeTop(ArCaGrating::TipType::kPointed);
		else if (wcscmp(newVal, L"Round") == 0)
			pEntity->put__kTipTypeTop(ArCaGrating::TipType::kRound);

		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}


STDMETHODIMP CArCaCOMGrating::get_TypeTipLower(BSTR * pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
			
		if (pEntity->get__kTipTypeLower() == ArCaGrating::TipType::kPlane)
				*pVal = ::SysAllocString(L"Plane");
		else if (pEntity->get__kTipTypeLower() == ArCaGrating::TipType::kArrow)
				*pVal = ::SysAllocString(L"Arrow");
		else if (pEntity->get__kTipTypeLower() == ArCaGrating::TipType::kPointed)
				*pVal = ::SysAllocString(L"Pointed");
		else if (pEntity->get__kTipTypeLower() == ArCaGrating::TipType::kRound)
				*pVal = ::SysAllocString(L"Round");

	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMGrating::put_TypeTipLower(BSTR newVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaGrating> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED

		if (wcscmp(newVal, L"Plane") == 0)
				pEntity->put__kTipTypeLower(ArCaGrating::TipType::kPlane);
		else if (wcscmp(newVal, L"Arrow") == 0)
				pEntity->put__kTipTypeLower(ArCaGrating::TipType::kArrow);
		else if (wcscmp(newVal, L"Pointed") == 0)
			pEntity->put__kTipTypeLower(ArCaGrating::TipType::kPointed);
		else if (wcscmp(newVal, L"Round") == 0)
			pEntity->put__kTipTypeLower(ArCaGrating::TipType::kRound);

		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMGrating, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMGrating, hr)) ;
	}
	return (S_OK) ;
}

// OPM calls this function for each property to obtain a list of strings and cookies if they are available.
// This function is declared on the IPerPropertyBrowsing interface. Our IOPMPropertyExtensionImpl
// class implements this member by reading the values in the OPM property map.
STDMETHODIMP CArCaCOMGrating::GetPredefinedStrings(DISPID dispID, CALPOLESTR *pCaStringsOut, CADWORD *pCaCookiesOut)
{
    if (dispID == DISPID_TYPETOP || dispID == DISPID_TYPELOWER)
	{   
		pCaStringsOut->pElems = (LPOLESTR *)::CoTaskMemAlloc(sizeof(LPOLESTR) * 4);
		pCaCookiesOut->pElems = (DWORD *)::CoTaskMemAlloc(sizeof(DWORD) * 4);

		pCaStringsOut->pElems[0] = ::SysAllocString(L"Plane");
		pCaCookiesOut->pElems[0] = 1;
		pCaStringsOut->pElems[1] = ::SysAllocString(L"Arrow");
		pCaCookiesOut->pElems[1] = 2;
		pCaStringsOut->pElems[2] = ::SysAllocString(L"Pointed");
		pCaCookiesOut->pElems[2] = 3;
		pCaStringsOut->pElems[3] = ::SysAllocString(L"Round");
		pCaCookiesOut->pElems[3] = 4;

		pCaStringsOut->cElems = 4;
		pCaCookiesOut->cElems = 4;
	}
	else
		return  IOPMPropertyExtensionImpl<CArCaCOMGrating>::GetPredefinedStrings(dispID,pCaStringsOut,pCaCookiesOut);
    
    return S_OK;
}

// OPM calls this function when the user selects an element from a drop down list. OPM provides
// the cookie that we associated with the element in the GetPredefinedStrings function. We are
// responsible for mapping this cookie back to a value that the properties corresponding put_ function
// can understand. 
// In this particular case all we need to do is to provide the name of the text style as
// the put_TextStyle method needs that.
STDMETHODIMP CArCaCOMGrating::GetPredefinedValue(DISPID dispID, DWORD dwCookie, VARIANT *pVarOut)
{
	if (dispID == DISPID_TYPETOP || dispID == DISPID_TYPELOWER)
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
			::VariantCopy(pVarOut,&CComVariant(L"Pointed"));
			break;
		case 4:
			::VariantCopy(pVarOut,&CComVariant(L"Round"));
			break;
		default:
			return S_FALSE;
		}
	}
	else
		return  IOPMPropertyExtensionImpl<CArCaCOMGrating>::GetPredefinedValue(dispID,dwCookie, pVarOut);
}

STDMETHODIMP CArCaCOMGrating::GetDisplayName(DISPID dispID, BSTR *propName)
{
	if (propName==NULL)
		return E_POINTER;
	switch ( dispID ) {
		case DISPID_OPENING:
			*propName =::SysAllocString (L"Opening") ;
			break ;
		case DISPID_THICKNESS_VERT:
		case DISPID_THICKNESS_HORIZ:
			*propName =::SysAllocString (L"Thickness") ;
			break ;
		case DISPID_QUANTITY_HORIZ:
			*propName =::SysAllocString (L"Quantity") ;
			break ;
		case DISPID_TYPETOP:
			*propName =::SysAllocString (L"Top tip") ;
			break ;
		case DISPID_TYPELOWER:
			*propName =::SysAllocString (L"Lower tip") ;
			break ;
		default:
			return (E_NOTIMPL) ;
	}
	return S_OK; 
}

STDMETHODIMP CArCaCOMGrating::MapPropertyToCategory( 
	/* [in]  */ DISPID dispid,
	/* [out] */ PROPCAT* pPropCat) 
{
	if (pPropCat==NULL)
		return E_POINTER;
	switch ( dispid ) {
		case DISPID_OPENING:
		case DISPID_THICKNESS_VERT:
		case DISPID_TYPETOP:
		case DISPID_TYPELOWER:
			*pPropCat =1 ;
			break ;
		case DISPID_THICKNESS_HORIZ:
		case DISPID_QUANTITY_HORIZ:
			*pPropCat =2 ;
			break ;
		default:
			return (E_NOTIMPL) ;
	}
	return S_OK; 
}
// Get the name for the category ID
STDMETHODIMP CArCaCOMGrating::GetCategoryName(
	/* [in]  */ PROPCAT propcat, 
	/* [in]  */ LCID lcid,
	/* [out] */ BSTR* pBstrName) 
{
	if (pBstrName==NULL)
		return E_POINTER;

	if ( propcat == 1 ) { //- Arbitrary number but unique
		*pBstrName =::SysAllocString (L"Vertical Grids") ;
		return (S_OK) ;
	} else if ( propcat == 2 ) { //- Arbitrary number but unique
		*pBstrName =::SysAllocString (L"Horizontal Grids") ;
		return (S_OK) ;
	}
	return (E_NOTIMPL);
}
