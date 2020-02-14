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
//----- ArCaDynamicArea.cpp : Implementation of ArCaDynamicArea
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaDynamicArea.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ArCaDynamicArea::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	ArCaDynamicArea, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kAllAllowedBits, ARCADYNAMICAREA,
ARCAARCAOBJECTSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
ArCaDynamicArea::ArCaDynamicArea () : AcDbEntity () {
	_drDrawer = new ArCaDrDrawer(-1,"Area",1);
}

ArCaDynamicArea::~ArCaDynamicArea () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
//################################## O U T ######################################
Acad::ErrorStatus ArCaDynamicArea::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ArCaDynamicArea::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeInt16(_aPolyLPoint3D.length());
	for(int i=0; i< _aPolyLPoint3D.length(); i++) {
		pFiler->writePoint3d(_aPolyLPoint3D.at(i)) ;
	}
	pFiler->writePoint3d (_p3dArea) ;
	pFiler->writeString (static_cast<const TCHAR*>(_sArea)) ;

	return (pFiler->filerStatus ()) ;
}
//################################## I N ######################################
Acad::ErrorStatus ArCaDynamicArea::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ArCaDynamicArea::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ArCaDynamicArea::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	AcGePoint3d ipt;
	Adesk::Int16 loop;
	pFiler->readInt16(&loop);
	_aPolyLPoint3D.removeAll();
	for(int i=0; i< loop; i++) {
		if ( version >= 1 /*&& version <= endVersion*/ )
		{
			pFiler->readPoint3d(&ipt) ;
			_aPolyLPoint3D.append(ipt);
		}
	}
	if ( version >= 1 /*&& version <= endVersion*/ ) pFiler->readPoint3d(&_p3dArea) ;
	if ( version >= 1 /*&& version <= endVersion*/ ) {
		TCHAR* temp = NULL;
		pFiler->readString(&temp);
		_sArea.Format(_T("%s"),temp);
		acutDelString(temp);
	}
	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
//################################## W O R L D D R A W ######################################
Adesk::Boolean ArCaDynamicArea::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;

	SetDrawer(_drDrawer);
	_drDrawer->DoDraw(mode);

	return Adesk::kTrue;
}
//################################ BOOOOOOM!!! ###################################
Acad::ErrorStatus ArCaDynamicArea::subExplode (AcDbVoidPtrArray & entitySet) const
{
	assertReadEnabled();

	SetDrawer(_drDrawer);
	_drDrawer->DoExplode(entitySet);

	return (Acad::eOk);
}
//################################ T H E   D R A W E R ###################################
void ArCaDynamicArea::SetDrawer( ArCaDrDrawer *drDrawer  ) const
{
	ArCaDrPolygon * drPoligon = new ArCaDrPolygon(_aPolyLPoint3D,TRUE);
	drPoligon->_bOsModeEnd = TRUE;
	drPoligon->_bOsModeNear = TRUE;
	drPoligon->_bOsModeMid = TRUE;
	drDrawer->Append(drPoligon);	

	//Construct a polyline only to area.
	AcDbPolyline *poly1 = new AcDbPolyline();

	for (int i=0; i<_aPolyLPoint3D.length(); i++)
		poly1->addVertexAt(i,*new AcGePoint2d(_aPolyLPoint3D.at(i).x, _aPolyLPoint3D.at(i).y));

	double area1;
	poly1->getArea(area1);
	delete poly1;
	CString sArea;
	sArea.Format(_T("Área: %.2f"),(area1*100));

	//ageita o texto pra fica no meio do grip
	AcGePoint3d pts3;
	pts3.set(_p3dArea.x-((sArea.GetLength()/2) * +0.001),_p3dArea.y+0.002,_p3dArea.z);

	ArCaDrText *drText = new ArCaDrText(pts3,AcGeVector3d::kXAxis,sArea);
	drText->_iColor = 256;
	drDrawer->Append(drText);
}

//################################## O S N A P ######################################
//--------------------- Osnap points protocol -------------------
Acad::ErrorStatus ArCaDynamicArea::subGetOsnapPoints(
    AcDb::OsnapMode       osnapMode,
    Adesk::GsMarker       gsSelectionMark,
    const AcGePoint3d&    pickPoint,
    const AcGePoint3d&    lastPoint,
    const AcGeMatrix3d&   viewXform,
    AcGePoint3dArray&     snapPoints,
    AcDbIntArray&         geomIds) const
{
	assertReadEnabled () ;

	_drDrawer->GetOsnapPoints(osnapMode,gsSelectionMark,pickPoint,lastPoint,viewXform,snapPoints,geomIds);
	
	return (Acad::eOk);
}
//################################## G R I P S ######################################
//--------------------- Get Grips -------------------
Acad::ErrorStatus ArCaDynamicArea::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
	) const {
		assertReadEnabled () ;		
		for(int i=0; i<_aPolyLPoint3D.length(); i++) {
			gripPoints.append(_aPolyLPoint3D.at(i));			
		}
		gripPoints.append(_p3dArea);		
		return (Acad::eOk) ;
}
//--------------------- Move Grips -------------------
Acad::ErrorStatus ArCaDynamicArea::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled ();

	for(int i=0; i<indices.length(); i++) {
		int idx = indices.at(i);
		if (idx==_aPolyLPoint3D.length())
		{
			_p3dArea += offset;
		}
		else{
			_aPolyLPoint3D.at(idx) += offset;			
		}
		//set Area point
		put_p3dArea();
	}
	return (Acad::eOk);
}
//################################## the GET and PUT from life! ######################################
//--------------------- PolyLPoint3D -------------------
AcGePoint3d ArCaDynamicArea::get_FirstPolyLPoint3D(void) const
{
	assertReadEnabled ();
	return (_aPolyLPoint3D.at(0));
}
Acad::ErrorStatus ArCaDynamicArea::put_PolyLPoint3D(AcGePoint3d newVal)
{
	assertWriteEnabled ();

	//Add one point3D in the array
	_aPolyLPoint3D.append(newVal);
	//set Area point
	put_p3dArea();

	return (Acad::eOk) ;
}
Acad::ErrorStatus ArCaDynamicArea::move_LastPolyLPoint3D(AcGePoint3d newVal)
{
	assertWriteEnabled ();
	_aPolyLPoint3D.at(_aPolyLPoint3D.length()-1) = newVal;
	//set Area point
	put_p3dArea();
	return (Acad::eOk) ;
}
Acad::ErrorStatus ArCaDynamicArea::del_PolyLPoint3D_movel()
{
	assertWriteEnabled ();
	_aPolyLPoint3D.removeLast();
	//set Area point
	put_p3dArea();
	return (Acad::eOk) ;
}
//--------------------- p3dArea -------------------
AcGePoint3d ArCaDynamicArea::get_p3dArea(void) const
{
	assertReadEnabled ();
	return (_p3dArea) ;
}
Acad::ErrorStatus ArCaDynamicArea::put_p3dArea()
{
	assertWriteEnabled ();
	// text
	_p3dArea.x = 0;_p3dArea.y = 0;_p3dArea.z = 0;
	for(int i=0; i<_aPolyLPoint3D.length(); i++) {
		_p3dArea.x += _aPolyLPoint3D.at(i).x;
		_p3dArea.y += _aPolyLPoint3D.at(i).y;
		_p3dArea.z += _aPolyLPoint3D.at(i).z;
	}
	_p3dArea.x = _p3dArea.x/_aPolyLPoint3D.length();
	_p3dArea.y = _p3dArea.y/_aPolyLPoint3D.length();
	_p3dArea.z = _p3dArea.z/_aPolyLPoint3D.length();
	return (Acad::eOk) ;
}
//--------------------- Area -------------------
void ArCaDynamicArea::get_Area(CString& text) const
{
	assertReadEnabled ();
	text.Format(_T("%s"),_sArea) ;
}

Acad::ErrorStatus ArCaDynamicArea::put_Area(LPCTSTR text)
{
	assertWriteEnabled ();
	_sArea.Format(_T("%s"),text) ;
	return (Acad::eOk) ;
}
//--------------------- IsFinished -------------------
boolean ArCaDynamicArea::get_IsFinished(void) const
{
	assertReadEnabled ();
	return (_bIsFinished) ;
}
Acad::ErrorStatus ArCaDynamicArea::switch_IsFinished()
{
	assertWriteEnabled ();
	if(_bIsFinished == true)
	{
		_bIsFinished = false;
		//del Point movel
		del_PolyLPoint3D_movel();
	}
	else
		_bIsFinished = true;

	return (Acad::eOk) ;
}

//################################ T R A N S F O R M B Y ###################################
Acad::ErrorStatus ArCaDynamicArea::subTransformBy(const AcGeMatrix3d & xform)
{
	assertWriteEnabled();
	for(int i=0; i<_aPolyLPoint3D.length(); i++) {
		_aPolyLPoint3D.at(i).transformBy(xform);
	}	
	_p3dArea.transformBy(xform);
	return (Acad::eOk);
}

Adesk::UInt32 ArCaDynamicArea::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

