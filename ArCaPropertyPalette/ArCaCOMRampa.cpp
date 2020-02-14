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
//----- ArCaCOMRampa.cpp : Implementation of CArCaCOMRampa
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaCOMRampa.h"
#include "..\ArCaObjects\ArCaRampa.h"

//----- CArCaCOMRampa
//-----------------------------------------------------------------------------
STDMETHODIMP CArCaCOMRampa::InterfaceSupportsErrorInfo (REFIID riid) {
	static const IID * arr [] ={
		&IID_IArCaCOMRampa
	} ;

	for ( int i =0 ; i < sizeof (arr) / sizeof (arr [0]) ; i++ ) {
		if ( InlineIsEqualGUID (*arr [i], riid) )
			return (S_OK) ;
	}
	return (S_FALSE) ;
}

//-----------------------------------------------------------------------------
//IAcadBaseObjectImpl
HRESULT CArCaCOMRampa::CreateNewObject (AcDbObjectId &objId, AcDbObjectId &ownerId, TCHAR *keyName) {
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
STDMETHODIMP CArCaCOMRampa::ForceDbResident (VARIANT_BOOL *forceDbResident) {
	if ( forceDbResident == NULL )
		return (E_POINTER) ;
	//----- Return ACAX_VARIANT_TRUE if the object must be database resident to be functional
	*forceDbResident =ACAX_VARIANT_FALSE ;
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMRampa::CreateObject (AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/) {
	try {
		Acad::ErrorStatus es ;
		AcDbObjectPointer<ArCaRampa> obj ;
		if ( (es =obj.create ()) != Acad::eOk )
			throw es ;

		obj->setDatabaseDefaults (ownerId.database ()) ;

		ArCaRampa *pTmp =NULL ;
		obj.release (pTmp) ;

		SetObject ((AcDbObject *&)pTmp) ;
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to create ArCaRampa", IID_IArCaCOMRampa, E_FAIL)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMRampa::AddToDb (AcDbObjectId &objId, AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/) {
	try {
		AcAxDocLock docLock (ownerId) ;

		Acad::ErrorStatus es ;
		AcAxObjectRefPtr<ArCaRampa> obj (&m_objRef, AcDb::kForRead) ;

		//----- Non-entities cannot be owned by block table record, but are
		//----- usually owned by dictionaries. If this is not your case, please
		//----- change the code below.
		AcDbDictionaryPointer pDictionary (ownerId, AcDb::kForWrite, true) ;
		if ( (es =pDictionary.openStatus ()) != Acad::eOk )
			throw es ;

		if ( (es =pDictionary->setAt (keyName, obj.object (), objId)) != Acad::eOk )
			throw es ;
	} catch( const Acad::ErrorStatus ) {
		return (Error(L"Failed to add ArCaRampa to database", IID_IArCaCOMRampa, E_FAIL)) ;
	}
	return (SetObjectId (objId)) ;
}

STDMETHODIMP CArCaCOMRampa::get_Height(double* pVal)
{
	try {
			if ( pVal == NULL )
			return (E_POINTER) ;

			AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
				AcAxObjectRefPtr<ArCaRampa> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
				*pVal =pEntity->get__dHeight();
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMRampa, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMRampa, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMRampa::put_Height(double newpVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaRampa> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		pEntity->put__dHeight(newpVal);
		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMRampa, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMRampa, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMRampa::get_Scale(double* pVal)
{
	try {
		if ( pVal == NULL )
			return (E_POINTER) ;

		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if (docLock.lockStatus () != Acad::eOk
			&& docLock.lockStatus () != Acad::eNoDatabase
			)
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaRampa> pEntity (&m_objRef, AcDb::kForRead) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		*pVal =pEntity->get__dScale();
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to get Entity Property", IID_IArCaCOMRampa, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMRampa, hr)) ;
	}
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMRampa::put_Scale(double newpVal)
{
	try {
		AcAxDocLock docLock (m_objRef.objectId (), AcAxDocLock::kNormal) ;
		if ( docLock.lockStatus () != Acad::eOk && docLock.lockStatus () != Acad::eNoDatabase )
			throw docLock.lockStatus () ; //- or E_ACCESSDENIED
		AcAxObjectRefPtr<ArCaRampa> pEntity (&m_objRef, AcDb::kForWrite) ;
		if ( pEntity.openStatus () != Acad::eOk )
			throw pEntity.openStatus () ; //- or E_ACCESSDENIED
		pEntity->put__dScale(newpVal);
		//Fire_Notification (DISPID_NUMBER) ; //for put_
	} catch ( const Acad::ErrorStatus ) {
		return (Error (L"Failed to set Entity Property", IID_IArCaCOMRampa, E_FAIL)) ;
	} catch ( const HRESULT hr ) {
		return (Error (L"Invalid argument.", IID_IArCaCOMRampa, hr)) ;
	}
	return (S_OK) ;
}

// Get the name for the category ID
STDMETHODIMP CArCaCOMRampa::GetCategoryName(
	/* [in]  */ PROPCAT propcat, 
	/* [in]  */ LCID lcid,
	/* [out] */ BSTR* pBstrName) 
{
	if (pBstrName==NULL)
		return E_POINTER;

	*pBstrName =::SysAllocString (L"Custon Property") ;
	return (S_OK) ;
}

STDMETHODIMP CArCaCOMRampa::GetParentCategory (PROPCAT CatID, PROPCAT *pParentCatID) {
	if ( pParentCatID == NULL )
		return (E_FAIL) ;
	if ( CatID == 2 ) {
		*pParentCatID =1 ; //- Making Sub a child of Main
		return (S_OK) ;
	}
	return (E_NOTIMPL) ;
}