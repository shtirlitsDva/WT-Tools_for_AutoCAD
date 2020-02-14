#include "StdAfx.h"
#include "ArCaDrEntity.h"

ArCaDrArc::ArCaDrArc(void)
{
	InitializeBase();
}
ArCaDrArc::ArCaDrArc(AcGePoint3d p3dCenter, AcGeVector3d VecStart, double dRadios, double dAngle)
{
	InitializeBase();
	_p3dCenter = p3dCenter;
	_VecStart = VecStart;
	_dRadios = dRadios;
	_dAngle = dAngle;
}
ArCaDrArc::~ArCaDrArc(void)
{
}

void ArCaDrArc::DoDraw(AcGiWorldDraw *mode)
{
	SetEnvironment(mode);
	mode->geometry().circularArc(_p3dCenter,_dRadios,AcGeVector3d::kZAxis,_VecStart,_dAngle);
}
void ArCaDrArc::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	AcDbArc* pArc = new AcDbArc();
	pArc->setCenter(_p3dCenter);
	pArc->setRadius(_dRadios);
	pArc->setNormal(AcGeVector3d::kZAxis);	

	AcGeVector3d vx = AcGeVector3d::kXAxis;
	double ang = vx.angleTo(_VecStart,AcGeVector3d::kZAxis);
	pArc->setStartAngle(ang);
	pArc->setEndAngle(ang+_dAngle);
	
	pArc->setLayer(_idLayer);
	pArc->setColorIndex(_iColor);
	entitySet.append(pArc);
}

void ArCaDrArc::GetOsnapPoints(
								  AcDb::OsnapMode osnapMode,
								  int gsSelectionMark,
								  const AcGePoint3d &pickPoint,
								  const AcGePoint3d &lastPoint,
								  const AcGeMatrix3d &viewXform,
								  AcGePoint3dArray &snapPoints,
								  AcDbIntArray &geomIds) const
{
	AcGeCircArc3d *pCircle;
	AcGePoint3d pt;
	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd:
		pCircle = new AcGeCircArc3d(_p3dCenter,AcGeVector3d::kZAxis,_VecStart,_dRadios,0,_dAngle);
		snapPoints.append(pCircle->startPoint());
		snapPoints.append(pCircle->endPoint());
		break;

	case AcDb::kOsModeNear:
		pCircle = new AcGeCircArc3d(_p3dCenter,AcGeVector3d::kZAxis,_VecStart,_dRadios,0,_dAngle);
		pt = pCircle->closestPointTo(pickPoint);
		snapPoints.append(pt);
		break;

	case AcDb::kOsModeMid:
		pCircle = new AcGeCircArc3d(_p3dCenter,AcGeVector3d::kZAxis,_VecStart,_dRadios,0,(_dAngle/2));
		snapPoints.append(pCircle->endPoint());
		break;

	case AcDb::kOsModeCen:
		snapPoints.append(_p3dCenter);
		break;
	}
}