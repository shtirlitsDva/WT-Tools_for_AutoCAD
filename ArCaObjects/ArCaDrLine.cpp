#include "StdAfx.h"
#include "ArCaDrEntity.h"

ArCaDrLine::ArCaDrLine(void)
{
	InitializeBase();
}
ArCaDrLine::ArCaDrLine(AcGePoint3d p3dStart, AcGePoint3d p3dEnd)
{
	InitializeBase();
	_p3dStart = p3dStart;
	_p3dEnd = p3dEnd;
}
ArCaDrLine::~ArCaDrLine(void)
{
}

void ArCaDrLine::DoDraw(AcGiWorldDraw *mode)
{
	SetEnvironment(mode);
	AcGePoint3d ptLine[2];
	ptLine[0] = _p3dStart;
	ptLine[1] = _p3dEnd;
	mode->geometry().worldLine(ptLine);
}

void ArCaDrLine::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	AcDbLine* pLine = new AcDbLine();	
	pLine->setStartPoint(_p3dStart);
	pLine->setEndPoint(_p3dEnd);
	pLine->setLayer(_idLayer);
	pLine->setColorIndex(_iColor);
	entitySet.append(pLine);
}

void ArCaDrLine::GetOsnapPoints(
						   AcDb::OsnapMode osnapMode,
						   int gsSelectionMark,
						   const AcGePoint3d &pickPoint,
						   const AcGePoint3d &lastPoint,
						   const AcGeMatrix3d &viewXform,
						   AcGePoint3dArray &snapPoints,
						   AcDbIntArray &geomIds) const
{
	AcGeLineSeg3d *line;
	switch (osnapMode) {

case AcDb::kOsModeEnd:
	snapPoints.append(_p3dStart);
	snapPoints.append(_p3dEnd);
	break;

case AcDb::kOsModeNear:
	line = new AcGeLineSeg3d(_p3dStart,_p3dEnd);
	snapPoints.append(line->closestPointTo(pickPoint));
	break;

case AcDb::kOsModeMid:
	line = new AcGeLineSeg3d(_p3dStart,_p3dEnd);
	snapPoints.append(line->midPoint());
	break;
	}
}