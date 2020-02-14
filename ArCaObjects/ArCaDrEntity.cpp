#include "StdAfx.h"
#include "ArCaDrEntity.h"
#include "ArxDbgUtils.h"


ArCaDrEntity::~ArCaDrEntity(void)
{
}

void ArCaDrEntity::InitializeBase()
{
	_iSelectionMarker = -1;
	_idLayer = NULL;
	_iColor = -1;

	//Osnap
	_bOsModeEnd = FALSE;
	_bOsModeNear = FALSE;
	_bOsModeMid = FALSE;
	_bOsModeCen = FALSE;
}

void ArCaDrEntity::SetEnvironment(AcGiWorldDraw *mode)
{
	if (_iSelectionMarker == -1)
		mode->subEntityTraits().setSelectionMarker(1);
	else
		mode->subEntityTraits().setSelectionMarker(_iSelectionMarker);
	if (_iColor == -1)
		mode->subEntityTraits().setColor(7);
	else
		mode->subEntityTraits().setColor(_iColor);


	mode->subEntityTraits().setFillType(AcGiFillType::kAcGiFillNever);
}

void ArCaDrEntity::SetLayerByString(CString sLayer)
{
	//AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	////Get or Create Body Layer
	//ArxDbgUtils::GetLayerIdFromString(sLayer, _idLayer, TRUE, pDB);
}

AcDbHatch* ArCaDrEntity::GetHatch(AcGePoint3dArray p3dVertices)const
{
	//############ Hatch ############
	AcDbHatch* pHtArrowhead = new AcDbHatch();
	// Sem padrão definido inicialmente
	pHtArrowhead->setNormal(AcGeVector3d::kZAxis);
	pHtArrowhead->setElevation(p3dVertices.first().z);
	pHtArrowhead->setAssociative(true);
	pHtArrowhead->setHatchStyle(AcDbHatch::kNormal);
	pHtArrowhead->setPattern(AcDbHatch::kPreDefined, _T("SOLID"));

	int l = pHtArrowhead->numLoops();
	// Remove previous loop
	for (int i = 0; i < l; i++)
		pHtArrowhead->removeLoopAt(i);

	// Insert the updated loop
	AcGePoint2dArray vertexPts;
	AcGeDoubleArray vertexBulges;

	//adiciona vertices
	for (int i=0; i<p3dVertices.length(); i++)
	{
		vertexPts.append(AcGePoint2d(p3dVertices.at(i).x, p3dVertices.at(i).y));
		vertexBulges.append(0);
	}

	// Close the loop
	vertexPts.append(vertexPts.first());
	vertexBulges.append(vertexBulges.first());
	pHtArrowhead->appendLoop(AcDbHatch::kDefault, vertexPts, vertexBulges);

	// Refresh hatch
	pHtArrowhead->evaluateHatch();

	return pHtArrowhead;
}