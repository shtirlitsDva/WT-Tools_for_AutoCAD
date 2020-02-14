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
//----- ArCaRampa.cpp : Implementation of ArCaRampa
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaRampa.h"
#include "ArxDbgUtils.h"
#include "ArCaDrEntity.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ArCaRampa::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS_WITH_PROPERTIES (
	ArCaRampa, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kAllAllowedBits, ARCARAMPA,
ARCAARCAOBJECTSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address,
ArCaRampa::makeMembers
)

//-----------------------------------------------------------------------------
ArCaRampa::ArCaRampa () : AcDbEntity () {
	_dHeight = 1;
	_dScale = 100;
}

ArCaRampa::~ArCaRampa () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
//################################## O U T ######################################
Acad::ErrorStatus ArCaRampa::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ArCaRampa::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	//_sDeclive
	pFiler->writeString (static_cast<const TCHAR*>(_sDeclive)) ;
	//_p3dDeclive
	pFiler->writePoint3d(_p3dDeclive) ;
	//_p3dStart
	pFiler->writePoint3d (_p3dStart) ;
	//_p3dEnd
	pFiler->writePoint3d (_p3dEnd) ;
	//_dHeight
	pFiler->writeDouble(_dHeight) ;
	//_dScale
	pFiler->writeDouble (_dScale) ;

	return (pFiler->filerStatus ()) ;
}

//################################## I N ######################################
Acad::ErrorStatus ArCaRampa::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ArCaRampa::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ArCaRampa::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	//_sDeclive
	if ( version >= 1 /*&& version <= endVersion*/ ) {
		TCHAR* temp = NULL;
		pFiler->readString(&temp);
		_sDeclive.Format(_T("%s"),temp);
		acutDelString(temp);
	}
	//_p3dDeclive
	if ( version >= 1 /*&& version <= endVersion*/ ) pFiler->readPoint3d (&_p3dDeclive) ;
	//_p3dStart
	if ( version >= 1 /*&& version <= endVersion*/ ) pFiler->readPoint3d (&_p3dStart) ;
	//_p3dEnd
	if ( version >= 1 /*&& version <= endVersion*/ ) pFiler->readPoint3d (&_p3dEnd) ;
	//_dHeight
	if ( version >= 1 /*&& version <= endVersion*/ ) pFiler->readDouble (&_dHeight) ;
	//_dScale
	if ( version >= 1 /*&& version <= endVersion*/ ) pFiler->readDouble (&_dScale) ;

	_LCS.SetAlignSys(_p3dStart, _p3dEnd);

	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
//################################## W O R L D D R A W ######################################
Adesk::Boolean ArCaRampa::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,this->layerId(),30);
	SetDrawer(drDrawer);
	drDrawer->DoDraw(mode);

	return Adesk::kTrue;
}
//################################ B O O O M ! ###################################
Acad::ErrorStatus ArCaRampa::subExplode (AcDbVoidPtrArray & entitySet) const
{
	assertReadEnabled();

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,this->layerId(),30);
	SetDrawer(drDrawer);
	drDrawer->DoExplode(entitySet);
	
	return (Acad::eOk);
}
//################################ T H E   D R A W E R ###################################
void ArCaRampa::SetDrawer( ArCaDrDrawer *drDrawer  ) const
{
	//############ Variables ############
	//Distance
	double Dist = _p3dStart.distanceTo(_p3dEnd);

	//############ Body Line ############
	ArCaDrLine *drLine = new ArCaDrLine(_p3dStart, _p3dEnd);
	drDrawer->Append(drLine);

	//############ Arrowhead ############
	AcGePoint3dArray ptArrowhead;
	ptArrowhead.append(*new AcGePoint3d(Dist,0,0));
	ptArrowhead.append(*new AcGePoint3d(Dist-0.0017,-0.0003,0));
	ptArrowhead.append(*new AcGePoint3d(Dist-0.0017,0.0003,0));

	//Set Matrix
	ptArrowhead.at(0).transformBy(_LCS._matAlignSys);
	ptArrowhead.at(1).transformBy(_LCS._matAlignSys);
	ptArrowhead.at(2).transformBy(_LCS._matAlignSys);

	ArCaDrPolygon *drPolygon = new ArCaDrPolygon(ptArrowhead,TRUE);
	drDrawer->Append(drPolygon);

	//############ Circle ############
	ArCaDrCircle *pCircle = new ArCaDrCircle(_p3dStart, 0.0008);
	drDrawer->Append(pCircle);

	//############ Text ############
	//Variables
	CString TxtDeclive;
	double text = (_dHeight / Dist) / (_dScale/100);
	TxtDeclive.Format(_T("i=%.2f"), text);
	TxtDeclive += "%";
	CString TxtRampa;
	TxtRampa = "RAMPA";
	double dLengthUper;
	double dHeightUper;
	double dLengthLower;
	double dHeightLower;

	AcGePoint3d p3dDecliveUper;
	AcGePoint3d p3dDecliveLower;

	//Direction
	AcGeVector3d vDir;
	if (_p3dStart.x < _p3dEnd.x)
	{
		p3dDecliveUper.x = (TxtDeclive.GetLength()/2) * -0.002;
		p3dDecliveUper.y = 0.001;
		p3dDecliveUper.z = 0;

		p3dDecliveLower.x = (TxtRampa.GetLength()/2) * -0.0025;
		p3dDecliveLower.y = -0.001 - 0.0023;
		p3dDecliveLower.z = 0;

		vDir = _p3dEnd - _p3dStart;
	}
	else
	{
		p3dDecliveUper.x = (TxtDeclive.GetLength()/2) * +0.002;
		p3dDecliveUper.y = -0.001;
		p3dDecliveUper.z = 0;

		p3dDecliveLower.x = (TxtRampa.GetLength()/2) * +0.0025;
		p3dDecliveLower.y = 0.001 + 0.0023;
		p3dDecliveLower.z = 0;

		vDir = _p3dStart - _p3dEnd;
	}

	//Create Text upper
	p3dDecliveUper.transformBy(_LCS.GetAlignSysByPoint(_p3dDeclive));

	ArCaDrText * drTopText = new ArCaDrText(p3dDecliveUper,vDir,TxtDeclive);
	drTopText->_stSyle.setTextSize(0.0023);
	drTopText->_iColor = 223;
	drDrawer->Append(drTopText);

 	//Create Text lower
 	p3dDecliveLower.transformBy(_LCS.GetAlignSysByPoint(_p3dDeclive));

	ArCaDrText * drLowerText = new ArCaDrText(p3dDecliveLower,vDir,TxtRampa);
	drLowerText->_stSyle.setTextSize(0.0023);
	drLowerText->_iColor = 223;
	drDrawer->Append(drLowerText);
}
//################################ OTHER METODES ###################################

Adesk::UInt32 ArCaRampa::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

//################################## G R I P S ######################################
//--------------------- Get Grips -------------------
Acad::ErrorStatus ArCaRampa::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	gripPoints.append(_p3dDeclive);
	gripPoints.append(_p3dStart);
	gripPoints.append(_p3dEnd);

	return (Acad::eOk) ;
}
//--------------------- Move Grips -------------------
Acad::ErrorStatus ArCaRampa::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	for(int i=0; i<indices.length(); i++)
	{
		int idx = indices.at(i);

		if (idx==0) _p3dDeclive += offset;
		if (idx==1)
		{
			_p3dStart += offset;
			_LCS.SetAlignSys(_p3dStart, _p3dEnd);
			Set_p3dDeclive();
		}
		if (idx==2)
		{
			_p3dEnd += offset;
			_LCS.SetAlignSys(_p3dStart, _p3dEnd);
			Set_p3dDeclive();
		}
	}
	return (Acad::eOk) ;
}

//################################## the GET and PUT from life! ######################################
AcGePoint3d ArCaRampa::get__p3dStart(void) const
{
	assertReadEnabled ();	
	return (_p3dStart) ;	
}

Acad::ErrorStatus ArCaRampa::put__p3dStart(AcGePoint3d newp3d)
{
	assertWriteEnabled ();
	_p3dStart = newp3d;
	return (Acad::eOk) ;
}

AcGePoint3d ArCaRampa::get__p3dEnd(void) const
{
	assertReadEnabled ();
	return (_p3dEnd);
}

Acad::ErrorStatus ArCaRampa::put__p3dEnd(AcGePoint3d newp3d)
{
	assertWriteEnabled ();
	_p3dEnd = newp3d;
	_LCS.SetAlignSys(_p3dStart, _p3dEnd);
	Set_p3dDeclive();
	return (Acad::eOk);
}

Acad::ErrorStatus ArCaRampa::Set_p3dDeclive()
{
	assertWriteEnabled ();
	_p3dDeclive.x = (_p3dStart.x+_p3dEnd.x)/2.0;
	_p3dDeclive.y = (_p3dStart.y+_p3dEnd.y)/2.0;
	_p3dDeclive.z = (_p3dStart.z+_p3dEnd.z)/2.0;
	return (Acad::eOk);
}

double ArCaRampa::get__dHeight(void) const
{
	assertReadEnabled ();	
	return (_dHeight) ;	
}

Acad::ErrorStatus ArCaRampa::put__dHeight(double newVal)
{
	assertWriteEnabled ();
	_dHeight = newVal;
	return (Acad::eOk) ;
}

double ArCaRampa::get__dScale(void) const
{
	assertReadEnabled ();	
	return (_dScale) ;	
}

Acad::ErrorStatus ArCaRampa::put__dScale(double newVal)
{
	assertWriteEnabled ();
	_dScale = newVal;
	return (Acad::eOk) ;
}
//################################ T R A N S F O R M B Y ###################################
Acad::ErrorStatus ArCaRampa::subTransformBy(const AcGeMatrix3d & xform)
{
	assertWriteEnabled();
	_p3dDeclive.transformBy(xform);
	_p3dStart.transformBy(xform);
	_p3dEnd.transformBy(xform);
	_LCS.SetAlignSys(_p3dStart, _p3dEnd);
	return (Acad::eOk);
}
//################################ PROPERTY PALLET ###################################
/// <summary>
/// Called to provide the available properties
/// </summary>
void ArCaRampa::makeMembers( AcRxMemberCollectionBuilder& collectionBuilder, void* customData)
{
	collectionBuilder.add((AcRxMember*)HeightProp::makeHeightProp());
	collectionBuilder.add((AcRxMember*)RampScaleProp::makeScaleProp());
}

/////////////////////////////////////////////////////////////////////
// Utility Fuctions /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/// <summary>
/// Registers our application name if it is not registered already
/// </summary>
static Acad::ErrorStatus createRegApp(AcDbDatabase * pDb, const ACHAR * pAppName)
{
	Acad::ErrorStatus es;

	if (pDb == NULL)
		return Acad::eInvalidInput; 

	AcDbRegAppTablePointer appTable(pDb->regAppTableId(), AcDb::kForWrite);

	if((es = appTable.openStatus()) != Acad::eOk)
		return es;
    
	if(appTable->has(pAppName))
		return Acad::eOk;
    
	AcDbRegAppTableRecordPointer appTableRecord;
	appTableRecord.create(); 
	appTableRecord->setName(pAppName);
    
	if((es = appTable->add(appTableRecord)) != Acad::eOk)
		return es;

	return Acad::eOk;
}

/////////////////////////////////////////////////////////////////////
// HeightProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * HeightProp::kMyAppName = _T("ValueApp"); 
const ACHAR * HeightProp::kCategoryName = _T("Custon Properties"); 

AcRxCategory * HeightProp::category = NULL;

HeightProp::HeightProp() : AcRxProperty(_T("Height"), AcRxValueType::Desc<double>::value())
{
	if (category == NULL)
	{
		AcRxCategory * parent =  AcRxCategory::rootCategory();
		category = parent->findDescendant(kCategoryName); 
		if (category == NULL)
			category = new AcRxCategory(kCategoryName, parent); 
	}

	attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
	attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}

/// <summary>
/// This is called by the system to get the property value for a specific object
/// </summary>
Acad::ErrorStatus HeightProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaRampa * ent = ArCaRampa::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dHeight();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus HeightProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaRampa * ent = ArCaRampa::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), HeightProp::kMyAppName);

	ent->put__dHeight(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * HeightProp::makeHeightProp() 
{ 
	return new HeightProp(); 
}

/////////////////////////////////////////////////////////////////////
// ScaleProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * RampScaleProp::kMyAppName = _T("RampScaleApp"); 
const ACHAR * RampScaleProp::kCategoryName = _T("Custon Properties"); 

AcRxCategory * RampScaleProp::category = NULL;

RampScaleProp::RampScaleProp() : AcRxProperty(_T("Scale"), AcRxValueType::Desc<double>::value())
{
	if (category == NULL)
	{
		AcRxCategory * parent =  AcRxCategory::rootCategory();
		category = parent->findDescendant(kCategoryName); 
		if (category == NULL)
			category = new AcRxCategory(kCategoryName, parent); 
	}

	attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
	attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}

/// <summary>
/// This is called by the system to get the property value for a specific object
/// </summary>
Acad::ErrorStatus RampScaleProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaRampa * ent = ArCaRampa::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dScale();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus RampScaleProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaRampa * ent = ArCaRampa::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), RampScaleProp::kMyAppName);

	ent->put__dScale(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * RampScaleProp::makeScaleProp() 
{ 
	return new RampScaleProp(); 
}