#include "StdAfx.h"
#include "ArCaDrEntity.h"
#include "ArxDbgUtils.h"

ArCaDrDrawer::ArCaDrDrawer(void)
{
	_DrawArray = *new AcArray<ArCaDrEntity*>();
	_iSelectionMarker = -1;

	//Get idLayer by string name
	CString sLayer = "0";
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	ArxDbgUtils::GetLayerIdFromString(sLayer, _idLayer, TRUE, pDB);
	
	_iColor = 7;
	_iSelMarkerNum = 1;
}
ArCaDrDrawer::ArCaDrDrawer(int iSelectionMarker, CString sLayer, int iColor)
{
	_DrawArray = *new AcArray<ArCaDrEntity*>();
	_iSelectionMarker = iSelectionMarker;

	//Get idLayer by string name
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
	ArxDbgUtils::GetLayerIdFromString(sLayer, _idLayer, TRUE, pDB);

	_iColor = iColor;
	_iSelMarkerNum = 1;
}
ArCaDrDrawer::ArCaDrDrawer(int iSelectionMarker, AcDbObjectId idLayer, int iColor)
{
	_DrawArray = *new AcArray<ArCaDrEntity*>();
	_iSelectionMarker = iSelectionMarker;
	_idLayer = idLayer;
	_iColor = iColor;
	_iSelMarkerNum = 1;
}
ArCaDrDrawer::~ArCaDrDrawer(void)
{
}
void ArCaDrDrawer::Append(ArCaDrEntity* Entity)
{

	if (Entity->_iSelectionMarker == -1)
		//if _iSelectionMarker == -1 then selectionMarker progressive
		if (this->_iSelectionMarker == -1)
		{
			Entity->_iSelectionMarker = this->_iSelMarkerNum;
			this->_iSelMarkerNum += 1;
		}
		else
			Entity->_iSelectionMarker = this->_iSelectionMarker;

	if (Entity->_idLayer == NULL)
		Entity->_idLayer = this->_idLayer;
	if (Entity->_iColor == -1)
		Entity->_iColor = this->_iColor;

	_DrawArray.append(Entity);
}
void ArCaDrDrawer::DoDraw(AcGiWorldDraw *mode)
{
	for (int i=0; i<_DrawArray.length(); i++)
	{
		_DrawArray.at(i)->DoDraw(mode);
	}
}
void ArCaDrDrawer::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	for (int i=0; i<_DrawArray.length(); i++)
	{
		_DrawArray.at(i)->DoExplode(entitySet);
	}
}


void ArCaDrDrawer::GetOsnapPoints(
							   AcDb::OsnapMode osnapMode,
							   int gsSelectionMark,
							   const AcGePoint3d &pickPoint,
							   const AcGePoint3d &lastPoint,
							   const AcGeMatrix3d &viewXform,
							   AcGePoint3dArray &snapPoints,
							   AcDbIntArray &geomIds) const
{
	switch (osnapMode) 
	{

	case AcDb::kOsModeEnd:
		for (int i=0; i<_DrawArray.length(); i++)
			if (_DrawArray.at(i)->_bOsModeEnd)
				_DrawArray.at(i)->GetOsnapPoints(osnapMode,gsSelectionMark,pickPoint,lastPoint,viewXform,snapPoints,geomIds);
		break;

	case AcDb::kOsModeNear:
		for (int i=0; i<_DrawArray.length(); i++)
			if (_DrawArray.at(i)->_bOsModeNear)
				_DrawArray.at(i)->GetOsnapPoints(osnapMode,gsSelectionMark,pickPoint,lastPoint,viewXform,snapPoints,geomIds);
				break;

	case AcDb::kOsModeMid:
		for (int i=0; i<_DrawArray.length(); i++)
			if (_DrawArray.at(i)->_bOsModeMid)
				_DrawArray.at(i)->GetOsnapPoints(osnapMode,gsSelectionMark,pickPoint,lastPoint,viewXform,snapPoints,geomIds);
				break;

	case AcDb::kOsModeCen:
		for (int i=0; i<_DrawArray.length(); i++)
			if (_DrawArray.at(i)->_bOsModeCen)
				_DrawArray.at(i)->GetOsnapPoints(osnapMode,gsSelectionMark,pickPoint,lastPoint,viewXform,snapPoints,geomIds);
				break;
	}
}