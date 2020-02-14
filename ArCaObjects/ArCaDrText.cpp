#include "StdAfx.h"
#include "ArCaDrEntity.h"
#include "ArxDbgUtils.h"

ArCaDrText::ArCaDrText(void)
{
	InitializeBase();
	SetInitialStyle();
}
ArCaDrText::ArCaDrText(AcGePoint3d p3dPosition, AcGeVector3d VecDirection, CString sText)
{
	InitializeBase();
	SetInitialStyle();
	_p3dPosition = p3dPosition;
	_VecDirection = VecDirection;
	_sText = sText;
}

ArCaDrText::~ArCaDrText(void)
{
}

void ArCaDrText::DoDraw(AcGiWorldDraw *mode)
{
	SetEnvironment(mode);

	AcGiTextStyle style;
	style.setFileName(_T("Romans"));
	style.setTextSize(0.0023);
	style.loadStyleRec();

	mode->geometry().text(_p3dPosition,AcGeVector3d::kZAxis,_VecDirection,_sText,_sText.GetLength(),Adesk::kFalse,style);
}
void ArCaDrText::DoExplode(AcDbVoidPtrArray & entitySet)const
{
	AcDbText* pText = new AcDbText();
	pText->setTextString(_sText);
	pText->setTextStyle(GetTextStyle());
	pText->setHeight(_stSyle.textSize());
	pText->setPosition(_p3dPosition);
	pText->setLayer(_idLayer);
	pText->setColorIndex(_iColor);

	AcGeVector3d vx = AcGeVector3d::kXAxis;
	double ang = vx.angleTo(_VecDirection,AcGeVector3d::kZAxis);	
	pText->setRotation(ang);

	entitySet.append(pText);
}

void ArCaDrText::SetInitialStyle()
{	
	_stSyle.setFileName(_T("Romans"));
	_stSyle.setTextSize(0.0023);
	_stSyle.loadStyleRec();
	_sStyleName = "ArCaTextStyle1";
}

AcDbObjectId ArCaDrText::GetTextStyle()const
{
	//############ Variables ############
	AcDbObjectId idStyle = NULL;
	AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

	//############ TextStyle ############
	AcDbTextStyleTable* textTbl;
	pDB->getTextStyleTable(textTbl, AcDb::kForWrite);

	//If not exist Create TextStyle
	ArxDbgUtils::addNewTextStyle(textTbl, _sStyleName, _stSyle.styleName(), _stSyle.textSize());
	textTbl->close();
	//Get TextStyle
	AcDbTextStyleTableRecord *objTextStyleTabRec  = (AcDbTextStyleTableRecord*)ArxDbgUtils::openSymbolTableRecord(AcDbTextStyleTableRecord::desc(), _T("ArCaTextStyle1"), AcDb::kForWrite ,pDB);
	idStyle = objTextStyleTabRec->objectId();
	objTextStyleTabRec->close();

	return idStyle;
}

void ArCaDrText::GetOsnapPoints(
								AcDb::OsnapMode osnapMode,
								int gsSelectionMark,
								const AcGePoint3d &pickPoint,
								const AcGePoint3d &lastPoint,
								const AcGeMatrix3d &viewXform,
								AcGePoint3dArray &snapPoints,
								AcDbIntArray &geomIds) const
{

	switch (osnapMode) {

case AcDb::kOsModeEnd:
	snapPoints.append(_p3dPosition);
	break;
	}
}