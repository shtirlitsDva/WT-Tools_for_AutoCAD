#include "StdAfx.h"
#include "ArCaDrEntity.h"

ArCaDrCircle::ArCaDrCircle(void)
{
	InitializeBase();
}
ArCaDrCircle::ArCaDrCircle(AcGePoint3d p3dCenter, double dRadios)
{
	InitializeBase();
	_p3dCenter = p3dCenter;
	_dRadios = dRadios;
}
ArCaDrCircle::~ArCaDrCircle(void)
{
}

void ArCaDrCircle::DoDraw(AcGiWorldDraw *mode)
{
	SetEnvironment(mode);
	mode->geometry().circle(_p3dCenter,_dRadios,AcGeVector3d::kZAxis);
}
void ArCaDrCircle::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	AcDbCircle* pCircle = new AcDbCircle();
	pCircle->setCenter(_p3dCenter);
	pCircle->setRadius(_dRadios);
	pCircle->setLayer(_idLayer);
	pCircle->setColorIndex(_iColor);
	entitySet.append(pCircle);
}

void ArCaDrCircle::GetOsnapPoints(
									AcDb::OsnapMode osnapMode,
									int gsSelectionMark,
									const AcGePoint3d &pickPoint,
									const AcGePoint3d &lastPoint,
									const AcGeMatrix3d &viewXform,
									AcGePoint3dArray &snapPoints,
									AcDbIntArray &geomIds) const
{
	AcGeCircArc3d *pCircle;
	switch (osnapMode) 
	{
	case AcDb::kOsModeNear:
		pCircle = new AcGeCircArc3d(_p3dCenter,AcGeVector3d::kZAxis,_dRadios);
		snapPoints.append(pCircle->closestPointTo(pickPoint));
		break;

	case AcDb::kOsModeCen:
		snapPoints.append(_p3dCenter);
		break;
	}
}