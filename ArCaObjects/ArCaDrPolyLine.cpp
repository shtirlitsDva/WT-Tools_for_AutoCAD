#include "StdAfx.h"
#include "ArCaDrEntity.h"

ArCaDrPolyLine::ArCaDrPolyLine(void)
{
	InitializeBase();
}
ArCaDrPolyLine::ArCaDrPolyLine(AcGePoint3dArray p3dVertices)
{
	InitializeBase();
	_p3dVertices = p3dVertices;
}

ArCaDrPolyLine::~ArCaDrPolyLine(void)
{
}

void ArCaDrPolyLine::DoDraw(AcGiWorldDraw *mode)
{
	SetEnvironment(mode);
	mode->geometry().polyline(_p3dVertices.length(),_p3dVertices.asArrayPtr());
}
void ArCaDrPolyLine::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	AcDbPolyline* pPolyLine = new AcDbPolyline();
	for (int i=0; i<_p3dVertices.length(); i++)
		pPolyLine->addVertexAt(i,AcGePoint2d(_p3dVertices.at(i).x,_p3dVertices.at(i).y));

	pPolyLine->setClosed(FALSE);
	pPolyLine->setLayer(_idLayer);
	pPolyLine->setColorIndex(_iColor);
	entitySet.append(pPolyLine);
}

void ArCaDrPolyLine::GetOsnapPoints(
								   AcDb::OsnapMode osnapMode,
								   int gsSelectionMark,
								   const AcGePoint3d &pickPoint,
								   const AcGePoint3d &lastPoint,
								   const AcGeMatrix3d &viewXform,
								   AcGePoint3dArray &snapPoints,
								   AcDbIntArray &geomIds) const
{
	AcGePolyline3d *pPolyline;

	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd:
		for(int i=0; i<_p3dVertices.length(); i++) {
			snapPoints.append(_p3dVertices.at(i));
		}
		break;

	case AcDb::kOsModeNear:
		pPolyline = new AcGePolyline3d(_p3dVertices);
		snapPoints.append(pPolyline->closestPointTo(pickPoint));
		break;

	case AcDb::kOsModeMid:
		AcGeLineSeg3d *line;
		for(int i=0; i<_p3dVertices.length(); i++) 
		{	
			line = new AcGeLineSeg3d(_p3dVertices.at(i),_p3dVertices.at(i+1));
			snapPoints.append(line->midPoint());		
		}
		break;
	}
}