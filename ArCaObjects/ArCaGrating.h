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
//----- ArCaGrating.h : Declaration of the ArCaGrating
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

//Pallet Propertyes
#include "rxmember.h"
#include "rxvaluetype.h"
#include "rxattrib.h"
#include "rxprop.h"
#include "rxcategory.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP ArCaGrating : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ArCaGrating) ;

	static void makeMembers( AcRxMemberCollectionBuilder& collectionBuilder, void* customData);

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ArCaGrating () ;
	virtual ~ArCaGrating () ;

		enum TipType {		
		kPlane	 = 0,
		kArrow	 = 1,
		kPointed = 2,
		kRound	 = 3,
	};

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;

	void SetDrawer( ArCaDrDrawer *drDrawer  ) const;
	void GetVerticalGrid(double dPosition, ArCaDrDrawer *drDrawer) const;
	void GetHorizontalGrid(double dXDist, int iYPos, double dleght, ArCaDrDrawer *drDrawer) const;
	void RedefineHorGridLeftPositions();
	void RedefineHorGridRightPositions();
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	virtual Acad::ErrorStatus subExplode (AcDbVoidPtrArray & entitySet) const ;

	AcGePoint3d _p3dTopRight;
	AcGePoint3d _p3dLowerRight;
	AcGePoint3d _p3dTopLeft;
	AcGePoint3d _p3dLowerLeft;
	double _dGridVerThickness;
	double _dGridHorThickness;
	double _dOpening;
	AcArray<double> _lstLeftGrid;
	AcArray<double> _lstRightGrid;
	TipType _TopTipGrid;
	TipType _LowerTipGrid;

public:

	TipType get__kTipTypeTop(void) const;
	Acad::ErrorStatus put__kTipTypeTop(TipType newType);
	Acad::ErrorStatus put__kTipTypeTopByString(int iType);

	TipType get__kTipTypeLower(void) const;
	Acad::ErrorStatus put__kTipTypeLower(TipType newType);
	Acad::ErrorStatus put__kTipTypeLowerByString(int iType);

	Acad::ErrorStatus put__p3dLowerLeft(AcGePoint3d newp3d);
	Acad::ErrorStatus put__p3dTopRight(AcGePoint3d newp3d);

	Acad::ErrorStatus put__dOpening(double newVal);
	double get__dOpening() const;

	Acad::ErrorStatus put__dGridVerThickness(double newVal);
	double get__dGridVerThickness() const;

	Acad::ErrorStatus put__dGridHorThickness(double newVal);
	double get__dGridHorThickness() const;

	Acad::ErrorStatus put__iGridHorQuantiy(int newVal);
	int get__iGridHorQuantiy() const;

	//- Osnap points protocol
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
} ;

#ifdef ARCAOBJECTS_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArCaGrating)
#endif

/////////////////////////////////////////////////////////////////////
// OpeningProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class OpeningProp : public AcRxProperty 
{
public:
	static AcRxObject * makeProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	OpeningProp();
	~OpeningProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// VertThicknessProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class VertThicknessProp : public AcRxProperty 
{
public:
	static AcRxObject * makeProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	VertThicknessProp();
	~VertThicknessProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// TopTipTypeProp ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
 
 
template<typename ValueType>
class TipTypeValueTypeTemplate : public AcRxValueType
{
public:
  TipTypeValueTypeTemplate(const ACHAR* name,
    const IAcRxEnumeration& pEnum,
    AcRxMemberCollectionConstructorPtr memberConstruct,
    void* userData = NULL):
  AcRxValueType(name,pEnum, sizeof(ValueType),
    memberConstruct, userData) {}
  virtual int subToString(const void* instance, ACHAR* buffer,
    size_t sizeInACHARs, AcRxValueType::StringFormat format)
    const ADESK_OVERRIDE;
  virtual bool subEqualTo(const void* a, const void* b)
    const ADESK_OVERRIDE;
};
 
template<typename ValueType>
class TopTipTypePropTemplate :
  public TipTypeValueTypeTemplate<ValueType>, public IAcRxEnumeration
{
  AcArray<const AcRxEnumTag*> m_tags;
 
public:
  TopTipTypePropTemplate(const ACHAR* name,
    AcRxMemberCollectionConstructorPtr memberConstruct,
    void* userData = NULL):
  TipTypeValueTypeTemplate<ValueType>(
    name,*this, memberConstruct, userData) {}
 
  ~TopTipTypePropTemplate()
  {
    for (int i=m_tags.length()-1;i>=0;i--)
      AcRxMember::deleteMember(m_tags[i]);
  }
 
  virtual int count() const ADESK_OVERRIDE
  {
    return m_tags.length();
  }
 
  virtual const AcRxEnumTag& getAt(int i) const ADESK_OVERRIDE
  {
    return *m_tags[i];
  }
 
  void append(AcRxEnumTag& tag)
  {
    m_tags.append(&tag);
    void acdbImpSetOwnerForEnumTag(
      const AcRxClass*, AcRxEnumTag*);
    acdbImpSetOwnerForEnumTag(this, &tag);
  }
};
 
// TopTipTypeProp 
 
class TopTipTypeProp : public AcRxProperty 
{
public:
  static AcRxObject * makeProp();
 
  static const ACHAR * kCategoryName;
  static AcRxCategory * category;
 
  TopTipTypeProp();
  virtual ~TopTipTypeProp();
 
  virtual Acad::ErrorStatus subGetValue(
    const AcRxObject* pO, AcRxValue& value) const;
 
  virtual Acad::ErrorStatus subSetValue(
    AcRxObject* pO, const AcRxValue& value) const;
};
 
// LowerTipTypeProp 
 
class LowerTipTypeProp : public AcRxProperty 
{
public:
  static AcRxObject * makeProp();
 
  static const ACHAR * kCategoryName;
  static AcRxCategory * category;
 
  LowerTipTypeProp();
  virtual ~LowerTipTypeProp();
 
  virtual Acad::ErrorStatus subGetValue(
    const AcRxObject* pO, AcRxValue& value) const;
 
  virtual Acad::ErrorStatus subSetValue(
    AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// QuantityProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class QuantityProp : public AcRxProperty 
{
public:
	static AcRxObject * makeProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	QuantityProp();
	~QuantityProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// HorizThicknessProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class HorizThicknessProp : public AcRxProperty 
{
public:
	static AcRxObject * makeProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	HorizThicknessProp();
	~HorizThicknessProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};
