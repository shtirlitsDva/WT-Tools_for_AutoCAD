#pragma once

//############ Entity ############
class ArCaDrEntity
{
public:
	ArCaDrEntity(){};
	virtual ~ArCaDrEntity(void);

	int _iSelectionMarker;
	AcDbObjectId _idLayer;
	int _iColor;

	//Osnap
	bool _bOsModeEnd;
	bool _bOsModeNear;
	bool _bOsModeMid;
	bool _bOsModeCen;

	void InitializeBase();

	//Virtual
	virtual void DoDraw(AcGiWorldDraw *mode) = 0;
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const = 0;
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const = 0;
	//Base
	virtual void SetEnvironment(AcGiWorldDraw *mode);
	virtual void SetLayerByString(CString sLayer);
	virtual AcDbHatch* GetHatch(AcGePoint3dArray p3dVertices)const;

};

//############ Drawer ############
class ArCaDrDrawer
{
public:
	ArCaDrDrawer();
	ArCaDrDrawer(int iSelectionMarker, CString sLayer, int iColor);
	ArCaDrDrawer(int iSelectionMarker, AcDbObjectId idLayer, int iColor);
	~ArCaDrDrawer(void);

	//Draw array
	AcArray<ArCaDrEntity*> _DrawArray;

	//-1 == Selection Marker Progressive
	int _iSelectionMarker;
	AcDbObjectId _idLayer;
	int _iColor;

	void Append(ArCaDrEntity* Entity);
	void DoDraw(AcGiWorldDraw *mode);
	void DoExplode(AcDbVoidPtrArray & entitySet) const;
	void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const;
protected:
	int _iSelMarkerNum;
};

//############ Line ############
class ArCaDrLine :	public ArCaDrEntity
{
public:
	ArCaDrLine(void);
	ArCaDrLine(AcGePoint3d p3dStart, AcGePoint3d p3dEnd);
	~ArCaDrLine(void);

	AcGePoint3d _p3dStart;
	AcGePoint3d _p3dEnd;
	
	virtual void DoDraw(AcGiWorldDraw *mode);
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const;
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
};

//############ Circle ############
class ArCaDrCircle : public ArCaDrEntity
{
public:
	ArCaDrCircle(void);
	ArCaDrCircle(AcGePoint3d p3dCenter, double dRadios);
	~ArCaDrCircle(void);

	AcGePoint3d _p3dCenter;
	double _dRadios;

	virtual void DoDraw(AcGiWorldDraw *mode);
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const;
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
};

//############ PlyLine ############
class ArCaDrPolyLine :	public ArCaDrEntity
{
public:
	ArCaDrPolyLine(void);
	ArCaDrPolyLine(AcGePoint3dArray p3dVertices);
	~ArCaDrPolyLine(void);

	AcGePoint3dArray _p3dVertices;

	virtual void DoDraw(AcGiWorldDraw *mode);
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const;
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
};

//############ Polygon ############
class ArCaDrPolygon :	public ArCaDrEntity
{
public:
	ArCaDrPolygon(void);
	ArCaDrPolygon(AcGePoint3dArray p3dVertices, bool bIsFilled);
	~ArCaDrPolygon(void);

	AcGePoint3dArray _p3dVertices;
	bool _bIsFilled;

	virtual void DoDraw(AcGiWorldDraw *mode);
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const;	
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
};

//############ Arc ############
class ArCaDrArc : public ArCaDrEntity
{
public:
	ArCaDrArc(void);
	ArCaDrArc(AcGePoint3d p3dCenter, AcGeVector3d VecStart, double dRadios, double dAngle);
	~ArCaDrArc(void);

	AcGePoint3d _p3dCenter;
	AcGeVector3d _VecStart;
	double _dRadios;
	double _dAngle;

	virtual void DoDraw(AcGiWorldDraw *mode);
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const;
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
};

//############ Text ############
class ArCaDrText : public ArCaDrEntity
{
public:
	ArCaDrText(void);
	ArCaDrText(AcGePoint3d p3dPosition, AcGeVector3d VecDirection, CString sText);
	~ArCaDrText(void);

	AcGePoint3d _p3dPosition;
	AcGeVector3d _VecDirection;
	AcGiTextStyle _stSyle;
	CString _sStyleName;
	CString _sText;

	virtual void DoDraw(AcGiWorldDraw *mode);
	virtual void DoExplode(AcDbVoidPtrArray & entitySet) const;
	virtual void GetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	void SetInitialStyle();
	AcDbObjectId GetTextStyle()const;

};
