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
//----- ArCaCota.cpp : Implementation of ArCaCota
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaCota.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ArCaCota::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS_WITH_PROPERTIES (
	ArCaCota, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kAllAllowedBits, ARCACOTA,
ARCAARCAOBJECTSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address,
	ArCaCota::makeMembers
)

//-----------------------------------------------------------------------------
ArCaCota::ArCaCota () : AcDbEntity () {
	_dRadiosCircle = 0.0024;
	dCrossLine = 0.0041;
	_enType = CotaType::kArrow;
	_dCota = 100.00;
	_dScaleFactor = 100;
	_bDynamicQuote = true;
}

ArCaCota::~ArCaCota () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
//################################## O U T ######################################
Acad::ErrorStatus ArCaCota::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ArCaCota::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;

	pFiler->writePoint3d(_p3dText) ;
	pFiler->writeDouble(_dCota) ;
	pFiler->writeDouble (_dScaleFactor) ;
	pFiler->writePoint3d (_p3dInsertion) ;
	pFiler->writePoint3d(_p3dLeader) ;
	pFiler->writeInt16(_enType);

	return (pFiler->filerStatus ()) ;
}

//################################## I N ######################################
Acad::ErrorStatus ArCaCota::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ArCaCota::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ArCaCota::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;

	pFiler->readPoint3d(&_p3dText) ;
	pFiler->readDouble (&_dCota) ;
	pFiler->readDouble (&_dScaleFactor) ;
	pFiler->readPoint3d(&_p3dInsertion);
	pFiler->readPoint3d(&_p3dLeader);
	Adesk::Int16 iType;
	pFiler->readInt16(&iType);
	_enType = (CotaType)iType;

	return (pFiler->filerStatus ()) ;
}

//################################## W O R L D D R A W ######################################
Adesk::Boolean ArCaCota::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled ();

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,this->layer(),1);
	SetDrawer(drDrawer);
	drDrawer->DoDraw(mode);

	return Adesk::kTrue;
}
//################################ BOOOOOOM!!! ###################################
Acad::ErrorStatus ArCaCota::subExplode (AcDbVoidPtrArray & entitySet) const
{
	assertReadEnabled();

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,this->layer(),1);
	SetDrawer(drDrawer);
	drDrawer->DoExplode(entitySet);

	return (Acad::eOk);
}
//################################ T H E   D R A W E R ###################################
void ArCaCota::SetDrawer( ArCaDrDrawer *drDrawer ) const
{
	//Circle
	ArCaDrCircle *drCircle = new ArCaDrCircle(_p3dInsertion,_dRadiosCircle);
	drDrawer->Append(drCircle);

	//Horizontal Line
	ArCaDrLine *drHorLine = new ArCaDrLine();
	drHorLine->_p3dStart.set(_p3dInsertion.x ,_p3dInsertion.y - dCrossLine,_p3dInsertion.z);
	drHorLine->_p3dEnd.set(_p3dInsertion.x ,_p3dInsertion.y + dCrossLine,_p3dInsertion.z);
	drDrawer->Append(drHorLine);

	//Arcs
	double dDifArcs = _dRadiosCircle/9;
	for (int i = 0; i < 9; i++)
	{	
		ArCaDrArc *drArc = new ArCaDrArc();
		drArc->_p3dCenter = _p3dInsertion;
		drArc->_dRadios = dDifArcs*i;
		drArc->_VecStart = AcGeVector3d::kYAxis;
		drArc->_dAngle = 1.5708;
		drArc->_iColor = 6;
		drDrawer->Append(drArc);

		ArCaDrArc *drArc2 = new ArCaDrArc();
		drArc2->_p3dCenter = _p3dInsertion;
		drArc2->_dRadios = dDifArcs*i;
		drArc2->_VecStart = -AcGeVector3d::kYAxis;
		drArc2->_dAngle = 1.5708;
		drArc2->_iColor = 6;
		drDrawer->Append(drArc2);
	}

	//Leader and Text
	ArCaDrPolyLine *drLeader = new ArCaDrPolyLine();
	ArCaDrText *drText = new ArCaDrText();
	drText->_iColor = 221;
	CString TxtCota;
	TxtCota.Format(_T("i=%.2f"), _dCota);
	drText->_sText = TxtCota;
	drText->_VecDirection = AcGeVector3d::kXAxis;
	drText->_stSyle.loadStyleRec();
	double dLengthUper = drText->_sText.GetLength() * +0.002;

	if (_p3dText.x > _p3dInsertion.x)
	{
		drLeader->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x - dCrossLine,_p3dInsertion.y,_p3dInsertion.z));
		drLeader->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x + _dRadiosCircle ,_p3dInsertion.y,_p3dInsertion.z));
		drLeader->_p3dVertices.append(_p3dText);
		drLeader->_p3dVertices.append(*new AcGePoint3d(_p3dText.x  + dLengthUper,_p3dText.y,_p3dText.z));
		drText->_p3dPosition = *new AcGePoint3d(_p3dText.x ,_p3dText.y+0.0005,_p3dText.z);
	}
	else
	{
		drLeader->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x + dCrossLine,_p3dInsertion.y,_p3dInsertion.z));
		drLeader->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x - _dRadiosCircle ,_p3dInsertion.y,_p3dInsertion.z));
		drLeader->_p3dVertices.append(_p3dText);
		drLeader->_p3dVertices.append(*new AcGePoint3d(_p3dText.x - dLengthUper,_p3dText.y,_p3dText.z));
		drText->_p3dPosition = *new AcGePoint3d(_p3dText.x - dLengthUper,_p3dText.y+0.0005,_p3dText.z);
	}

	ArCaDrPolyLine *drExtraLine = new ArCaDrPolyLine();
	switch (_enType)
	{
	case CotaType::kPlane:
		break;
	case CotaType::kArrow:
		//Arrow Line
		drExtraLine->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x ,_p3dInsertion.y - dCrossLine,_p3dInsertion.z));
		drExtraLine->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x - 0.001,_p3dInsertion.y - dCrossLine + 0.001,_p3dInsertion.z));
		drExtraLine->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x + 0.001,_p3dInsertion.y - dCrossLine + 0.001,_p3dInsertion.z));
		break;
	case CotaType::kLeader:
		if (_p3dText.x > _p3dInsertion.x)
			drExtraLine->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x - dCrossLine,_p3dInsertion.y,_p3dInsertion.z));
		else
			drExtraLine->_p3dVertices.append(*new AcGePoint3d(_p3dInsertion.x + dCrossLine,_p3dInsertion.y,_p3dInsertion.z));

		drExtraLine->_p3dVertices.append(_p3dLeader);
		break;
	}

	drDrawer->Append(drExtraLine);
	drDrawer->Append(drLeader);
	drDrawer->Append(drText);
}
//################################ OTHER METHODS ###################################
Adesk::UInt32 ArCaCota::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

//################################## G R I P S ######################################
//--------------------- Get Grips -------------------
Acad::ErrorStatus ArCaCota::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	gripPoints.append(_p3dText);
	if (_enType == CotaType::kArrow)
	{
		AcGePoint3d ArrowPt = *new AcGePoint3d(_p3dInsertion.x,_p3dInsertion.y - dCrossLine,_p3dInsertion.z);
		gripPoints.append(ArrowPt);
	}
	else
	{
		gripPoints.append(_p3dInsertion);
	}

	if (_enType == CotaType::kLeader)
		gripPoints.append(_p3dLeader);


	return (Acad::eOk) ;
}
//--------------------- Move Grips -------------------
Acad::ErrorStatus ArCaCota::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	for(int i=0; i<indices.length(); i++)
	{
		int idx = indices.at(i);

		if (idx==0) _p3dText += offset;
		if (idx==1)
		{
			_p3dText += offset;
			_p3dInsertion += offset;			
			if (_enType != CotaType::kLeader)
			{
				_p3dLeader += offset;
				if (_bDynamicQuote && _enType == CotaType::kArrow)
					_dCota += (offset.y * _dScaleFactor);
			}
		}
		if (idx==2)
		{
			_p3dLeader += offset;
			if (_bDynamicQuote)
				_dCota += (offset.y * _dScaleFactor);
		}
	}
	return (Acad::eOk) ;
}

//################################## the GET and PUT from life! ######################################
AcGePoint3d ArCaCota::get__p3dInsertion(void) const
{
	assertReadEnabled ();	
	return (_p3dInsertion) ;	
}
Acad::ErrorStatus ArCaCota::put__p3dInsertion(AcGePoint3d newp3d)
{
	assertWriteEnabled ();
	if (_enType == CotaType::kArrow)
		_p3dInsertion = *new AcGePoint3d(newp3d.x,newp3d.y + dCrossLine,newp3d.z);
	else if (_enType == CotaType::kLeader)
		_p3dInsertion = *new AcGePoint3d(newp3d.x + (dCrossLine * 2),newp3d.y + dCrossLine, newp3d.z);
	else
		_p3dInsertion = newp3d;

	_p3dText = *new AcGePoint3d(_p3dInsertion.x + _dRadiosCircle,_p3dInsertion.y,_p3dInsertion.z);
	_p3dLeader = *new AcGePoint3d(_p3dInsertion.x - (dCrossLine * 2),_p3dInsertion.y - dCrossLine,_p3dInsertion.z);
	return (Acad::eOk) ;
}
ArCaCota::CotaType ArCaCota::get__kType(void) const
{
	assertReadEnabled ();	
	return (_enType) ;	
}
Acad::ErrorStatus ArCaCota::put__kType(CotaType newType)
{
	assertWriteEnabled ();
	_enType = newType;
	return (Acad::eOk) ;
}
Acad::ErrorStatus ArCaCota::put__kTypeByString(int iType)
{
	assertWriteEnabled ();
	_enType = (CotaType)iType;
	return (Acad::eOk) ;
}

double ArCaCota::get__dCota(void) const
{
	assertReadEnabled ();	
	return (_dCota) ;	
}
Acad::ErrorStatus ArCaCota::put__dCota(double newVal)
{
	assertWriteEnabled ();
	_dCota = newVal;
	return (Acad::eOk) ;
}

double ArCaCota::get__dScaleFactor(void) const
{
	assertReadEnabled ();	
	return (_dScaleFactor) ;	
}
Acad::ErrorStatus ArCaCota::put__dScaleFactor(double newVal)
{
	assertWriteEnabled ();
	_dScaleFactor = newVal;
	return (Acad::eOk) ;
}

bool ArCaCota::get__bDynamicQuote(void) const
{
	assertReadEnabled ();	
	return (_bDynamicQuote) ;	
}
Acad::ErrorStatus ArCaCota::put__bDynamicQuote(bool newVal)
{
	assertWriteEnabled ();
	_bDynamicQuote = newVal;
	return (Acad::eOk) ;
}
//################################ T R A N S F O R M B Y ###################################
Acad::ErrorStatus ArCaCota::subTransformBy(const AcGeMatrix3d & xform)
{
	assertWriteEnabled();

	_p3dText.transformBy(xform);
	_p3dLeader.transformBy(xform);

	AcGePoint3d p3dAux = _p3dInsertion;
	_p3dInsertion.transformBy(xform);
	double dAux = _p3dInsertion.y - p3dAux.y;
	if (_bDynamicQuote)
		_dCota += (dAux * _dScaleFactor);
	
	return (Acad::eOk);
}
//################################ PROPERTY PALLET ###################################
/// <summary>
/// Called to provide the available properties
/// </summary>
void ArCaCota::makeMembers( AcRxMemberCollectionBuilder& collectionBuilder, void* customData)
{
	collectionBuilder.add((AcRxMember*)ValueProp::makeValueProp());
	collectionBuilder.add((AcRxMember*)ScaleProp::makeScaleProp());
	collectionBuilder.add((AcRxMember*)TypeProp::makeTypeProp());
	collectionBuilder.add((AcRxMember*)DynamicQuoteProp::makeDynamicQuoteProp());
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
// DynamicQuoteProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * DynamicQuoteProp::kMyAppName = _T("DynamicQuoteApp"); 
const ACHAR * DynamicQuoteProp::kCategoryName = _T("Custon Properties"); 

AcRxCategory * DynamicQuoteProp::category = NULL;

DynamicQuoteProp::DynamicQuoteProp() : AcRxProperty(_T("Dynamic Quote"), AcRxValueType::Desc<bool>::value())
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
Acad::ErrorStatus DynamicQuoteProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaCota * ent = ArCaCota::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	bool val = false;
	val = ent->get__bDynamicQuote();

	value = AcRxValue(static_cast<bool>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus DynamicQuoteProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaCota * ent = ArCaCota::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const bool * val = rxvalue_cast<bool>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), DynamicQuoteProp::kMyAppName);

	ent->put__bDynamicQuote(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * DynamicQuoteProp::makeDynamicQuoteProp() 
{ 
	return new DynamicQuoteProp(); 
}

/////////////////////////////////////////////////////////////////////
// ValueProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * ValueProp::kMyAppName = _T("ValueApp"); 
const ACHAR * ValueProp::kCategoryName = _T("Custon Properties"); 

AcRxCategory * ValueProp::category = NULL;

ValueProp::ValueProp() : AcRxProperty(_T("Value"), AcRxValueType::Desc<double>::value())
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
Acad::ErrorStatus ValueProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaCota * ent = ArCaCota::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dCota();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus ValueProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaCota * ent = ArCaCota::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), ValueProp::kMyAppName);

	ent->put__dCota(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * ValueProp::makeValueProp() 
{ 
	return new ValueProp(); 
}


/////////////////////////////////////////////////////////////////////
// ScaleProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * ScaleProp::kMyAppName = _T("ScaleApp"); 
const ACHAR * ScaleProp::kCategoryName = _T("Custon Properties"); 

AcRxCategory * ScaleProp::category = NULL;

ScaleProp::ScaleProp() : AcRxProperty(_T("Scale"), AcRxValueType::Desc<double>::value())
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
Acad::ErrorStatus ScaleProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaCota * ent = ArCaCota::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dScaleFactor();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus ScaleProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaCota * ent = ArCaCota::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), ScaleProp::kMyAppName);

	ent->put__dScaleFactor(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * ScaleProp::makeScaleProp() 
{ 
	return new ScaleProp(); 
}

/////////////////////////////////////////////////////////////////////
// TypeProp ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
 
void makeTypePropTemplateProperties(class AcRxMemberCollectionBuilder & collectionBuilder, void*);
 
template<>
int TypePropTemplate<ArCaCota::CotaType>::subToString(const void *instance, ACHAR *buffer, size_t sizeInACHARs, AcRxValueType::StringFormat format) const
{
  const ACHAR* formatString = L"%d";
  ArCaCota::CotaType & value = *(ArCaCota::CotaType*)instance;
  if (buffer==NULL)
    return _scwprintf(formatString,value);
  return swprintf_s(buffer,sizeInACHARs,formatString,value);
}
 
template<>
bool TypePropTemplate<ArCaCota::CotaType>::subEqualTo( const void *a, const void* b) const
{
  ArCaCota::CotaType & v1 = *(ArCaCota::CotaType*)a;
  ArCaCota::CotaType & v2 = *(ArCaCota::CotaType*)b;
  return v1==v2;
}
 
template<>
struct AcRxValueType::Desc< ArCaCota::CotaType >
{
  __declspec(dllexport) static const AcRxValueType& value() throw();
  static void del();
};
 
TypePropTemplate<ArCaCota::CotaType>* s_pTypePropTemplate = NULL;
 
const AcRxValueType& AcRxValueType::Desc< ArCaCota::CotaType >::value() throw()
{
  if (s_pTypePropTemplate==NULL)
  {
    s_pTypePropTemplate = new TypePropTemplate<ArCaCota::CotaType>(L"MyEnumProperties",&makeTypePropTemplateProperties);

    AcRxEnumTag* pTag;
    pTag = new  AcRxEnumTag  (L"Arrow", (int)0);
    s_pTypePropTemplate->append(*pTag);
    pTag = new  AcRxEnumTag  (L"Leader", (int)1);
    s_pTypePropTemplate->append(*pTag);
    pTag = new  AcRxEnumTag  (L"Plane", (int)2);
    s_pTypePropTemplate->append(*pTag);
  }
  return *s_pTypePropTemplate;
};
// This should be called when the value type is not needed anymore
// Best call it when the module gets unloaded (On_kUnloadAppMsg)
void AcRxValueType::Desc< ArCaCota::CotaType >::del()
{
  if (s_pTypePropTemplate)
  {
    const ACHAR * name = s_pTypePropTemplate->name();
 
    if (acrxSysRegistry())
      acrxClassDictionary->remove(s_pTypePropTemplate->name());
 
    s_pTypePropTemplate = NULL;
  }
};
 
void makeTypePropTemplateProperties(
  class AcRxMemberCollectionBuilder & collectionBuilder, void*)
{
}
 
// TypeProp 
 
const ACHAR * TypeProp::kCategoryName = _T("Custon Properties");
 
AcRxCategory * TypeProp::category = NULL;
 
TypeProp::TypeProp() : AcRxProperty(_T("Quote Type"), AcRxValueType::Desc< ArCaCota::CotaType >::value())
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
 
TypeProp::~TypeProp()
{
}
 
/// <summary>
/// This is called by the system to get the property value for
/// a specific object
/// </summary>
Acad::ErrorStatus TypeProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
  AcDbEntity * ent = AcDbEntity::cast(pO);
  if (ent == NULL)
    return Acad::eNotThatKindOfClass;
 
  // Get the value from the custom entity
  // or wherever we stored the value
  ArCaCota * pCustonEnt = ArCaCota::cast(pO);
  value = AcRxValue(static_cast<ArCaCota::CotaType>(pCustonEnt->get__kType())); 
 
  return Acad::eOk;
}
 
/// <summary>
/// This is called by the system to retrieve the property value for
/// a specific object
/// </summary>
Acad::ErrorStatus TypeProp::subSetValue( AcRxObject* pO, const AcRxValue& value) const
{
  AcDbEntity * ent = AcDbEntity::cast(pO);
  if (ent == NULL)
    return Acad::eNotThatKindOfClass;
 
  const ArCaCota::CotaType * val = rxvalue_cast<ArCaCota::CotaType>(&value);
  if (val == NULL)
    return Acad::eInvalidInput;
 
  // Set the property of the custom entity
  // or store it wherever we want to
  ArCaCota * pCustonEnt = ArCaCota::cast(pO);
  pCustonEnt->put__kType(*val); 
 
  return Acad::eOk;
}
 
/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * TypeProp::makeTypeProp()
{
  return new TypeProp();
}