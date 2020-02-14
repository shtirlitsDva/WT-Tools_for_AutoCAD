#include "StdAfx.h"
#include "ArCaDrEntity.h"

ArCaDrPolygon::ArCaDrPolygon(void)
{
	InitializeBase();
}
ArCaDrPolygon::ArCaDrPolygon(AcGePoint3dArray p3dVertices, bool bIsFilled)
{
	InitializeBase();
	_p3dVertices = p3dVertices;
	_bIsFilled = bIsFilled;
}

ArCaDrPolygon::~ArCaDrPolygon(void)
{
}

void ArCaDrPolygon::DoDraw(AcGiWorldDraw *mode)
{
	SetEnvironment(mode);
	if (_bIsFilled)
		mode->subEntityTraits().setFillType(AcGiFillType::kAcGiFillAlways);

	mode->geometry().polygon(_p3dVertices.length(),_p3dVertices.asArrayPtr());
}
void ArCaDrPolygon::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	AcDbPolyline* pPolyLine = new AcDbPolyline();
	for (int i=0; i<_p3dVertices.length(); i++)
		pPolyLine->addVertexAt(i,AcGePoint2d(_p3dVertices.at(i).x,_p3dVertices.at(i).y));

	pPolyLine->setClosed(TRUE);
	pPolyLine->setLayer(_idLayer);
	pPolyLine->setColorIndex(_iColor);
	entitySet.append(pPolyLine);

	// Is Filled?
	if (_bIsFilled)
	{
		AcDbHatch* pHatch = GetHatch(_p3dVertices);
		pHatch->setLayer(_idLayer);
		pHatch->setColorIndex(_iColor);
		entitySet.append(pHatch);
	}
}

void ArCaDrPolygon::GetOsnapPoints(
								AcDb::OsnapMode osnapMode,
								int gsSelectionMark,
								const AcGePoint3d &pickPoint,
								const AcGePoint3d &lastPoint,
								const AcGeMatrix3d &viewXform,
								AcGePoint3dArray &snapPoints,
								AcDbIntArray &geomIds) const
{
	AcGeLineSeg3d *line;
	AcGePoint3dArray p3dPoligon;
	AcGePolyline3d *pPolyline;

	switch (osnapMode) 
	{
	case AcDb::kOsModeEnd:
		for(int i=0; i<_p3dVertices.length(); i++) {
			snapPoints.append(_p3dVertices.at(i));
		}
		break;

	case AcDb::kOsModeNear:
		p3dPoligon = _p3dVertices;
		p3dPoligon.append(_p3dVertices.first());
		pPolyline = new AcGePolyline3d(p3dPoligon);
		snapPoints.append(pPolyline->closestPointTo(pickPoint));
		break;

	case AcDb::kOsModeMid:		
		for(int i=0; i<_p3dVertices.length(); i++) 
		{
			if (_p3dVertices.at(i) != _p3dVertices.last())		
				line = new AcGeLineSeg3d(_p3dVertices.at(i),_p3dVertices.at(i+1));		
			else		
				line = new AcGeLineSeg3d(_p3dVertices.at(i),_p3dVertices.first());
			snapPoints.append(line->midPoint());		
		}
		break;
	}
}
