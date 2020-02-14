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
//----- ArCaRampa.h : Declaration of the ArCaRampa
//-----------------------------------------------------------------------------
#pragma once

#ifdef ARCAOBJECTS_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include "ArCaDrEntity.h"
#include "ArCaLCS.h"

//Pallet Propertyes
#include "rxmember.h"
#include "rxvaluetype.h"
#include "rxattrib.h"
#include "rxprop.h"
#include "rxcategory.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP ArCaRampa : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ArCaRampa) ;
	
	static void makeMembers( AcRxMemberCollectionBuilder& collectionBuilder, void* customData);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ArCaRampa () ;
	virtual ~ArCaRampa () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;
	void SetDrawer( ArCaDrDrawer *drDrawer  ) const;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	CString _sDeclive;
	AcGePoint3d _p3dDeclive;
	AcGePoint3d _p3dStart;
	AcGePoint3d _p3dEnd;
	double _dHeight;
	double _dScale;
	ArCaLCS _LCS;

public:
	//- Automation support
	virtual Acad::ErrorStatus subExplode (AcDbVoidPtrArray & entitySet) const ;

	AcGePoint3d get__p3dStart(void) const;
	Acad::ErrorStatus put__p3dStart(AcGePoint3d newp3d);

	AcGePoint3d get__p3dEnd(void) const;
	Acad::ErrorStatus put__p3dEnd(AcGePoint3d newp3d);

	Acad::ErrorStatus Set_p3dDeclive(void);	

	double get__dHeight(void) const;
	Acad::ErrorStatus put__dHeight(double newVal);

	double get__dScale(void) const;
	Acad::ErrorStatus put__dScale(double newVal);
};

#ifdef ARCAOBJECTS_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArCaRampa)
#endif

	/////////////////////////////////////////////////////////////////////
// HeightProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class HeightProp : public AcRxProperty 
{
public:
	static AcRxObject * makeHeightProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	HeightProp();
	~HeightProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// ScaleProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class RampScaleProp : public AcRxProperty 
{
public:
	static AcRxObject * makeScaleProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	RampScaleProp();
	~RampScaleProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};