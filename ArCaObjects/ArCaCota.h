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
//----- ArCaCota.h : Declaration of the ArCaCota
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
#include "ArxDbgUtils.h"

//Pallet Propertyes
#include "rxmember.h"
#include "rxvaluetype.h"
#include "rxattrib.h"
#include "rxprop.h"
#include "rxcategory.h"

//-----------------------------------------------------------------------------
class DLLIMPEXP ArCaCota : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(ArCaCota) ;

	    static void makeMembers(
            AcRxMemberCollectionBuilder& collectionBuilder,
            void* customData);

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	ArCaCota () ;
	virtual ~ArCaCota () ;

	enum CotaType {		
		kArrow	= 0,
		kLeader	= 1,
		kPlane	= 2,
	};

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
	//virtual Acad::ErrorStatus subGetClassID (CLSID *pClsid) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	AcGePoint3d _p3dInsertion;
	AcGePoint3d _p3dText;
	AcGePoint3d _p3dLeader;
	CotaType _enType;
	double _dRadiosCircle;
	double dCrossLine;
	double _dCota;
	double _dScaleFactor;
	bool _bDynamicQuote;

public:
	AcGePoint3d get__p3dInsertion(void) const;
	Acad::ErrorStatus put__p3dInsertion(AcGePoint3d newp3d);

	CotaType get__kType(void) const;
	Acad::ErrorStatus put__kType(CotaType newType);
	Acad::ErrorStatus put__kTypeByString(int iType);

	double get__dCota(void) const;
	Acad::ErrorStatus put__dCota(double newVal);

	double get__dScaleFactor(void) const;
	Acad::ErrorStatus put__dScaleFactor(double newVal);

	bool get__bDynamicQuote(void) const;
	Acad::ErrorStatus put__bDynamicQuote(bool newVal);
} ;

#ifdef ARCAOBJECTS_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ArCaCota)
#endif

/////////////////////////////////////////////////////////////////////
// DynamicQuoteProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class DynamicQuoteProp : public AcRxProperty 
{
public:
	static AcRxObject * makeDynamicQuoteProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	DynamicQuoteProp();
	~DynamicQuoteProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// ValueProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class ValueProp : public AcRxProperty 
{
public:
	static AcRxObject * makeValueProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	ValueProp();
	~ValueProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// ScaleProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class ScaleProp : public AcRxProperty 
{
public:
	static AcRxObject * makeScaleProp();

	static const ACHAR * kMyAppName; 
	static const ACHAR * kCategoryName;
	static AcRxCategory * category;

	ScaleProp();
	~ScaleProp()
	{}

    virtual Acad::ErrorStatus subGetValue(const AcRxObject* pO, AcRxValue& value) const;
	virtual Acad::ErrorStatus subSetValue(AcRxObject* pO, const AcRxValue& value) const;
};

/////////////////////////////////////////////////////////////////////
// TypeProp ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
 
 
template<typename ValueType>
class CotaValueTypeTemplate : public AcRxValueType
{
public:
  CotaValueTypeTemplate(const ACHAR* name,
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
class TypePropTemplate :
  public CotaValueTypeTemplate<ValueType>, public IAcRxEnumeration
{
  AcArray<const AcRxEnumTag*> m_tags;
 
public:
  TypePropTemplate(const ACHAR* name,
    AcRxMemberCollectionConstructorPtr memberConstruct,
    void* userData = NULL):
  CotaValueTypeTemplate<ValueType>(
    name,*this, memberConstruct, userData) {}
 
  ~TypePropTemplate()
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
 
// TypeProp 
 
class TypeProp : public AcRxProperty 
{
public:
  static AcRxObject * makeTypeProp();
 
  static const ACHAR * kCategoryName;
  static AcRxCategory * category;
 
  TypeProp();
  virtual ~TypeProp();
 
  virtual Acad::ErrorStatus subGetValue(
    const AcRxObject* pO, AcRxValue& value) const;
 
  virtual Acad::ErrorStatus subSetValue(
    AcRxObject* pO, const AcRxValue& value) const;
};
 