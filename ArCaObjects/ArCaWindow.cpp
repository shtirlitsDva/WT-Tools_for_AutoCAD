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
//----- ArCaWindow.cpp : Implementation of ArCaWindow
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaWindow.h"


//-----------------------------------------------------------------------------
Adesk::UInt32 ArCaWindow::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	ArCaWindow, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, ARCAWINDOW,
ARCAARCAOBJECTSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
ArCaWindow::ArCaWindow () : AcDbEntity () 
{
	_dThickness = 10;
}

ArCaWindow::~ArCaWindow () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
//################################## O U T ######################################
Acad::ErrorStatus ArCaWindow::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ArCaWindow::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	return (pFiler->filerStatus ()) ;
}

//################################## I N ######################################
Acad::ErrorStatus ArCaWindow::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ArCaWindow::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ArCaWindow::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	return (pFiler->filerStatus ()) ;
}

//################################## W O R L D D R A W ######################################
Adesk::Boolean ArCaWindow::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,"A.Anno-Patt",151);
	SetDrawer(drDrawer);
	drDrawer->DoDraw(mode);

	return (AcDbEntity::subWorldDraw (mode)) ;
}

//################################ B O O O M ! ###################################
Acad::ErrorStatus ArCaWindow::subExplode (AcDbVoidPtrArray & entitySet) const
{
	assertReadEnabled();

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,"A.Anno-Patt",151);
	SetDrawer(drDrawer);
	drDrawer->DoExplode(entitySet);

	return (Acad::eOk);
}

//################################ T H E   D R A W E R ###################################
void ArCaWindow::SetDrawer( ArCaDrDrawer *drDrawer  ) const
{
	// ***** Boundary *****
	ArCaDrPolygon *drRectangle = new ArCaDrPolygon();

	drRectangle->_bIsFilled = FALSE;
	drRectangle->_p3dVertices.append(_p3dLeftLower);//Left Lower
	drRectangle->_p3dVertices.append(_p3dRightLower);//Right Lower
	drRectangle->_p3dVertices.append(_p3dRightTop);//Right Top
	drRectangle->_p3dVertices.append(_p3dLeftTop);//Left Top

	drDrawer->Append(drRectangle);
}

//################################ OTHER METHODS ###################################
Adesk::UInt32 ArCaWindow::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

void ArCaWindow::RedefinePoints() 
{
	assertWriteEnabled () ;

	_dWidth = _p3dLeftLower.distanceTo(_p3dRightLower);

	_LCS.SetAlignSys(_p3dLeftLower, _p3dRightLower);

	//Top
	_p3dLeftTop.set(0,_dThickness,0);
	_p3dRightTop.set(_dWidth,_dThickness,0);

	//Transform
	_p3dLeftTop.transformBy(_LCS._matAlignSys);
	_p3dRightTop.transformBy(_LCS._matAlignSys);
}

//################################## O S N A P ######################################
//--------------------- Osnap points protocol -------------------
Acad::ErrorStatus ArCaWindow::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	int gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}

//################################## G R I P S ######################################
//--------------------- Get Grips -------------------
Acad::ErrorStatus ArCaWindow::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;

	gripPoints.append(_p3dLeftLower);
	gripPoints.append(_p3dRightLower);

	return (Acad::eOk) ;
}
//--------------------- Move Grips -------------------
Acad::ErrorStatus ArCaWindow::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;

	for(int i=0; i<indices.length(); i++)
	{
		int idx = indices.at(i);

		if (idx==0)
		{
			_p3dLeftLower += offset;
			RedefinePoints();
		}
		if (idx==1)
		{
			_p3dRightLower += offset;
			RedefinePoints();
		}
	}

	return (Acad::eOk) ;
}

//################################## the GET and PUT from life! ######################################
AcGePoint3d ArCaWindow::get__p3dLeftLower(void) const
{
	assertReadEnabled ();	
	return (_p3dLeftLower) ;	
}

Acad::ErrorStatus ArCaWindow::put__p3dLeftLower(AcGePoint3d newp3d)
{
	assertWriteEnabled ();
	_p3dLeftLower = newp3d;
	RedefinePoints();
	return (Acad::eOk) ;
}

AcGePoint3d ArCaWindow::get__p3dRightLower(void) const
{
	assertReadEnabled ();
	return (_p3dRightLower);
}

Acad::ErrorStatus ArCaWindow::put__p3dRightLower(AcGePoint3d newp3d)
{
	assertWriteEnabled ();
	_p3dRightLower = newp3d;
	RedefinePoints();
	return (Acad::eOk);
}
