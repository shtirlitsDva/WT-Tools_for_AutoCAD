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
//----- ArCaDynamicArea.h : Declaration of the ArCaDynamicArea
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

//-----------------------------------------------------------------------------
class DLLIMPEXP ArCaDynamicArea : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ArCaDynamicArea) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

	//########################### CONSTRUTOR AND DESTRUTOR #################################
public:
	ArCaDynamicArea () ;
	virtual ~ArCaDynamicArea () ;

	//################################ D E F A U L T #######################################
	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;
	Acad::ErrorStatus subExplode (AcDbVoidPtrArray & entitySet) const;
	void SetDrawer( ArCaDrDrawer *drDrawer  ) const;

	//- OSNAP points protocol
	virtual Acad::ErrorStatus   subGetOsnapPoints(
        AcDb::OsnapMode       osnapMode,
        Adesk::GsMarker       gsSelectionMark,
        const AcGePoint3d&    pickPoint,
        const AcGePoint3d&    lastPoint,
        const AcGeMatrix3d&   viewXform,
        AcGePoint3dArray&        snapPoints,
        AcDbIntArray&     geomIds) const;


	// GRIP points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	//################################ V A R I A B L E S ###################################
protected:	
	AcGePoint3dArray _aPolyLPoint3D;
	CString _sArea;
	AcGePoint3d _p3dArea;
	boolean _bIsFinished;

	ArCaDrDrawer *_drDrawer;

	//GET AND PUT
public:
	// PolyLPoint3D
	AcGePoint3d get_FirstPolyLPoint3D(void) const;
	Acad::ErrorStatus put_PolyLPoint3D(AcGePoint3d newVal);
	Acad::ErrorStatus move_LastPolyLPoint3D(AcGePoint3d newVal);
	Acad::ErrorStatus del_PolyLPoint3D_movel(void);
	// Area
	void get_Area(CString& text) const;
	Acad::ErrorStatus put_Area(LPCTSTR text);
	// p3dArea
	AcGePoint3d get_p3dArea(void) const;
	Acad::ErrorStatus put_p3dArea();
	// IsFinished
	boolean get_IsFinished(void) const;
	Acad::ErrorStatus switch_IsFinished();

	//################################ T R A N S F O R M B Y ###################################
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
} ;

#ifdef ARCAOBJECTS_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArCaDynamicArea)
#endif
