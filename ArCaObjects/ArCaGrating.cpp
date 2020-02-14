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
//----- ArCaGrating.cpp : Implementation of ArCaGrating
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ArCaGrating.h"
#include "ArxDbgUtils.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ArCaGrating::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS_WITH_PROPERTIES (
	ArCaGrating, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kAllAllowedBits, ARCAGRATING,
ARCAARCAOBJECTSAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address,
ArCaGrating::makeMembers
)

//-----------------------------------------------------------------------------
ArCaGrating::ArCaGrating () : AcDbEntity () {
	_dGridVerThickness = 0.025;
	_dGridHorThickness = 0.025;
	_dOpening = 0.065;
	_TopTipGrid = TipType::kPlane;
	_LowerTipGrid = TipType::kPlane;
}

ArCaGrating::~ArCaGrating () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
//################################## O U T ######################################
Acad::ErrorStatus ArCaGrating::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ArCaGrating::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	pFiler->writePoint3d (_p3dLowerRight) ;
	pFiler->writePoint3d (_p3dLowerLeft) ;
	pFiler->writePoint3d (_p3dTopLeft) ;
	pFiler->writePoint3d(_p3dTopRight) ;
	pFiler->writeDouble (_dGridVerThickness) ;
	pFiler->writeDouble (_dGridHorThickness) ;
	pFiler->writeDouble(_dOpening) ;
	pFiler->writeInt16(_TopTipGrid);
	pFiler->writeInt16(_LowerTipGrid);
	pFiler->writeInt16(_lstRightGrid.length());

	for (int i=0; i<_lstRightGrid.length(); i++)
	{
		pFiler->writeDouble (_lstLeftGrid.at(i)) ;
		pFiler->writeDouble (_lstRightGrid.at(i)) ;
	}

	return (pFiler->filerStatus ()) ;
}

//################################## I N ######################################
Acad::ErrorStatus ArCaGrating::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ArCaGrating::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ArCaGrating::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	if ( version >= 1 /*&& version <= endVersion*/ )
	{
		pFiler->readPoint3d(&_p3dLowerRight) ;
		pFiler->readPoint3d (&_p3dLowerLeft) ;
		pFiler->readPoint3d (&_p3dTopLeft) ;
		pFiler->readPoint3d (&_p3dTopRight) ;
		pFiler->readDouble (&_dGridVerThickness) ;
		pFiler->readDouble (&_dGridHorThickness) ;
		pFiler->readDouble (&_dOpening) ;
		Adesk::Int16 iType;
		pFiler->readInt16(&iType);
		_TopTipGrid = (TipType)iType;
		pFiler->readInt16(&iType);
		_LowerTipGrid = (TipType)iType;

		double dItem;
		Adesk::Int16 loop;
		pFiler->readInt16(&loop);
		_lstLeftGrid.removeAll();
		_lstRightGrid.removeAll();
		for (int i=0; i<loop; i++)
		{
			pFiler->readDouble(&dItem) ;
			_lstLeftGrid.append(dItem);
			pFiler->readDouble(&dItem) ;
			_lstRightGrid.append(dItem);
		}
	}

	return (pFiler->filerStatus ()) ;
}

//################################## W O R L D D R A W ######################################
Adesk::Boolean ArCaGrating::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,this->layerId(),this->colorIndex());
	SetDrawer(drDrawer);
	drDrawer->DoDraw(mode);

	return Adesk::kFalse;
}

//################################ B O O O M ! ###################################
Acad::ErrorStatus ArCaGrating::subExplode (AcDbVoidPtrArray & entitySet) const
{
	assertReadEnabled();

	ArCaDrDrawer *drDrawer = new ArCaDrDrawer(-1,this->layerId(),this->colorIndex());
	SetDrawer(drDrawer);
	drDrawer->DoExplode(entitySet);

	return (Acad::eOk);
}

//################################ T H E   D R A W E R ###################################
void ArCaGrating::SetDrawer( ArCaDrDrawer *drDrawer  ) const
{
	double dLegth = _p3dLowerRight.x - _p3dLowerLeft.x;
	int iColunm;
	double dRest;
	// check if is not inverted grating
	if(dLegth > 0)
	{
		iColunm = floor(dLegth/(_dOpening + _dGridVerThickness));
		dRest = ((dLegth - (iColunm * (_dOpening + _dGridVerThickness)))+_dOpening)/2;
	}
	else
	{
		double dLegthFixed = _p3dLowerLeft.x - _p3dLowerRight.x;
		iColunm = floor(dLegthFixed/(_dOpening + _dGridVerThickness));
		dRest = -(((dLegthFixed - (iColunm * (_dOpening + _dGridVerThickness)))+_dOpening)/2);
	}
	double dIterator = dRest;	

	bool bDoLoop = true;

	while(bDoLoop)
	{
		//***Vertical Grid***
		if(iColunm != 0)
			GetVerticalGrid(dIterator,drDrawer);					

		//***Horizontal Grid***
		for (int i=1; i<_lstLeftGrid.length(); i++)
		{
			//Check if there are no columns to insert
			if(iColunm == 0)
			{
				GetHorizontalGrid(0,i,dLegth,drDrawer);
				continue;//exit for
			}

			//----Initial Grid----
			if (dIterator == dRest)			
				GetHorizontalGrid(0,i,dRest,drDrawer);
			//----Middle Grid----
			bool MiddleGridIsDone = true;
			// check if is not inverted grating			
			if(dLegth > 0)
			{
				if ((dIterator + _dGridVerThickness + _dOpening) < (dLegth - dRest))
				{
					GetHorizontalGrid(dIterator + _dGridVerThickness,i,_dOpening,drDrawer);	
					MiddleGridIsDone = false;
				}
			}
			else
			{
				if ((dIterator - (_dGridVerThickness + _dOpening)) > (dLegth - dRest))
				{
					GetHorizontalGrid(dIterator - _dGridVerThickness,i,-(_dOpening),drDrawer);	
					MiddleGridIsDone = false;
				}
			}
		
			//----Final Grid----
			//If middle grid is done.
			if(MiddleGridIsDone)
			{
				// check if is not inverted grating
				if(dLegth > 0)
					GetHorizontalGrid(dIterator + _dGridVerThickness,i,dRest,drDrawer);	
				else
					GetHorizontalGrid(dIterator - _dGridVerThickness,i,dRest,drDrawer);	
			}
		}
		// check if is not inverted grating
		if(dLegth > 0)
		{
			dIterator += (_dOpening + _dGridVerThickness);
			bDoLoop = (dIterator < (dLegth - dRest));			
		}
		else
		{
			dIterator -= (_dOpening + _dGridVerThickness);
			bDoLoop = (dIterator > (dLegth - dRest));			
		}
	}
}

void ArCaGrating::GetVerticalGrid(double dPosition, ArCaDrDrawer *drDrawer) const
{
	//Calculate variables
	double dTotalLegth = _p3dLowerRight.x - _p3dLowerLeft.x;
	double dLowerDif = _p3dLowerRight.y - _p3dLowerLeft.y;
	double dTopDif = _p3dTopRight.y - _p3dTopLeft.y;

	//Y
	double dYLowerLeft = _p3dLowerLeft.y + ((dPosition * dLowerDif)/dTotalLegth);
	double dYLowerRight = _p3dLowerLeft.y + (((dPosition + _dGridVerThickness) * dLowerDif)/dTotalLegth);
	double dYTopLeft = _p3dTopLeft.y + ((dPosition * dTopDif)/dTotalLegth);
	double dYTopRight = _p3dTopLeft.y + (((dPosition + _dGridVerThickness) * dTopDif)/dTotalLegth);

	//Linearize grid extremity
	if(dYLowerLeft > dYLowerRight)
		dYLowerRight = dYLowerLeft;
	else
		dYLowerLeft = dYLowerRight;
	if(dYTopLeft < dYTopRight)
		dYTopRight = dYTopLeft;
	else
		dYTopLeft = dYTopRight;

	//X
	double dXLeft = _p3dLowerLeft.x + dPosition;
	double dXRight;
	// check if is not inverted grating
	if(dTotalLegth > 0)
		dXRight = _p3dLowerLeft.x + dPosition + _dGridVerThickness;	
	else
		dXRight = _p3dLowerLeft.x + dPosition - _dGridVerThickness;	

	//Extremit points
	AcGePoint3d pt3dTopLeft = *new AcGePoint3d(0,0,0);
	AcGePoint3d pt3dTopRight = *new AcGePoint3d(0,0,0);
	AcGePoint3d pt3dLowerLeft = *new AcGePoint3d(0,0,0);
	AcGePoint3d pt3dLowerRight = *new AcGePoint3d(0,0,0);

	//--- Top ---

		if(_TopTipGrid == TipType::kPlane)
		{
			pt3dTopRight = *new AcGePoint3d(dXRight,dYTopRight,0);
			pt3dTopLeft = *new AcGePoint3d(dXLeft,dYTopLeft,0);

			ArCaDrPolyLine *drLine = new ArCaDrPolyLine();
			drLine->_p3dVertices.append(pt3dTopRight);//Right Top
			drLine->_p3dVertices.append(pt3dTopLeft);//Left Top

			drDrawer->Append(drLine);
		}

		else if(_TopTipGrid == TipType::kArrow)
		{
			ArCaDrPolyLine *drRectangle = new ArCaDrPolyLine();
			double dXMiddle = (dXRight + dXLeft)/2;
			double dRetreatFinal;
			double dRetreatMiddle;
			double dRetreatX;
			if(dYTopRight > dYLowerRight)
			{
				dRetreatFinal = _dGridVerThickness*3;
				dRetreatMiddle = _dGridVerThickness*2;				
			}
			else
			{
				dRetreatFinal = -(_dGridVerThickness*3);
				dRetreatMiddle = -(_dGridVerThickness*2);				
			}

			if(dXRight > dXLeft)
				dRetreatX = _dGridVerThickness/2;
			else
				dRetreatX = -(_dGridVerThickness/2);

			pt3dTopRight = *new AcGePoint3d(dXRight,dYTopRight-dRetreatFinal,0);
			pt3dTopLeft = *new AcGePoint3d(dXLeft,dYTopLeft-dRetreatFinal,0);

			drRectangle->_p3dVertices.append(pt3dTopRight);//Left Top
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXRight+dRetreatX,dYTopLeft-dRetreatMiddle,0));//Right Lower middle
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXMiddle,dYTopRight,0));//Middle Top
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXLeft-dRetreatX,dYTopLeft-dRetreatMiddle,0));//Left Lower middle
			drRectangle->_p3dVertices.append(pt3dTopLeft);//Right Top

			drDrawer->Append(drRectangle);
		}

		else if(_TopTipGrid == TipType::kPointed)
		{
			ArCaDrPolyLine *drRectangle = new ArCaDrPolyLine();
			double dXMiddle = (dXRight + dXLeft)/2;
			double dRetreat;
			if(dYTopRight > dYLowerRight)
				dRetreat = _dGridVerThickness*3;
			else
				dRetreat = -(_dGridVerThickness*3);

			pt3dTopRight = *new AcGePoint3d(dXRight,dYTopRight-dRetreat,0);
			pt3dTopLeft = *new AcGePoint3d(dXLeft,dYTopLeft-dRetreat,0);

			drRectangle->_p3dVertices.append(pt3dTopRight);//Right Top
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXMiddle,dYTopRight,0));//Middle Top
			drRectangle->_p3dVertices.append(pt3dTopLeft);//Left Top

			drDrawer->Append(drRectangle);
		}

		else if(_TopTipGrid == TipType::kRound)
		{
			ArCaDrArc *drArc1 = new ArCaDrArc();			
			drArc1->_dRadios = abs(dXLeft - dXRight)/2;		

			double dRetreat;
			if(dYTopRight > dYLowerRight)
			{
				drArc1->_VecStart = AcGeVector3d::kXAxis;
				dRetreat = abs(dXLeft - dXRight)/2;
			}
			else
			{
				drArc1->_VecStart = -(AcGeVector3d::kXAxis);
				dRetreat = -(abs(dXLeft - dXRight)/2);
			}

			drArc1->_p3dCenter = *new AcGePoint3d((dXRight + dXLeft)/2,dYTopRight-dRetreat,0);
			drArc1->_dAngle = 3.1416;
			drDrawer->Append(drArc1);

			pt3dTopRight = *new AcGePoint3d(dXRight,dYTopRight-dRetreat,0);
			pt3dTopLeft = *new AcGePoint3d(dXLeft,dYTopLeft-dRetreat,0);
		}

		//--- Lower ---

		if(_LowerTipGrid == TipType::kPlane)
		{
			pt3dLowerLeft = *new AcGePoint3d(dXLeft,dYLowerLeft,0);
			pt3dLowerRight = *new AcGePoint3d(dXRight,dYLowerRight,0);

			ArCaDrPolyLine *drRectangle = new ArCaDrPolyLine();
			drRectangle->_p3dVertices.append(pt3dLowerLeft);//Left Lower
			drRectangle->_p3dVertices.append(pt3dLowerRight);//Right Lower

			drDrawer->Append(drRectangle);
		}

		else if(_LowerTipGrid == TipType::kArrow)
		{
			ArCaDrPolyLine *drRectangle = new ArCaDrPolyLine();
			double dXMiddle = (dXRight + dXLeft)/2;
			double dRetreatFinal;
			double dRetreatMiddle;
			double dRetreatX;
			if(dYTopRight > dYLowerRight)
			{
				dRetreatFinal = _dGridVerThickness*3;
				dRetreatMiddle = _dGridVerThickness*2;				
			}
			else
			{
				dRetreatFinal = -(_dGridVerThickness*3);
				dRetreatMiddle = -(_dGridVerThickness*2);				
			}

			if(dXRight > dXLeft)
				dRetreatX = _dGridVerThickness/2;
			else
				dRetreatX = -(_dGridVerThickness/2);

			pt3dLowerLeft = *new AcGePoint3d(dXLeft,dYLowerLeft+dRetreatFinal,0);
			pt3dLowerRight = *new AcGePoint3d(dXRight,dYLowerRight+dRetreatFinal,0);

			drRectangle->_p3dVertices.append(pt3dLowerLeft);//Left Lower
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXLeft-dRetreatX,dYLowerLeft+dRetreatMiddle,0));//Left Lower middle
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXMiddle,dYLowerLeft,0));//Middle Lower
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXRight+dRetreatX,dYLowerRight+dRetreatMiddle,0));//Left Lower middle
			drRectangle->_p3dVertices.append(pt3dLowerRight);//Right Lower

			drDrawer->Append(drRectangle);
		}

		else if(_LowerTipGrid == TipType::kPointed)
		{
			ArCaDrPolyLine *drRectangle = new ArCaDrPolyLine();
			double dXMiddle = (dXRight + dXLeft)/2;
			double dRetreat;
			if(dYTopRight > dYLowerRight)
				dRetreat = _dGridVerThickness*3;
			else
				dRetreat = -(_dGridVerThickness*3);

			pt3dLowerLeft = *new AcGePoint3d(dXLeft,dYLowerLeft+dRetreat,0);
			pt3dLowerRight = *new AcGePoint3d(dXRight,dYLowerRight+dRetreat,0);

			drRectangle->_p3dVertices.append(pt3dLowerLeft);//Left Lower
			drRectangle->_p3dVertices.append(*new AcGePoint3d(dXMiddle,dYLowerLeft,0));//Middle Lower
			drRectangle->_p3dVertices.append(pt3dLowerRight);//Right Lower

			drDrawer->Append(drRectangle);
		}

		else if(_LowerTipGrid == TipType::kRound)
		{
			ArCaDrArc *drArc1 = new ArCaDrArc();			
			drArc1->_dRadios = abs(dXLeft - dXRight)/2;		

			double dRetreat;
			if(dYTopRight < dYLowerRight)
			{
				drArc1->_VecStart = AcGeVector3d::kXAxis;
				dRetreat = abs(dXLeft - dXRight)/2;
			}
			else
			{
				drArc1->_VecStart = -(AcGeVector3d::kXAxis);
				dRetreat = -(abs(dXLeft - dXRight)/2);
			}

			drArc1->_p3dCenter = *new AcGePoint3d((dXRight + dXLeft)/2,dYLowerRight-dRetreat,0);
			drArc1->_dAngle = 3.1416;
			drDrawer->Append(drArc1);

			pt3dLowerRight = *new AcGePoint3d(dXRight,dYLowerRight-dRetreat,0);
			pt3dLowerLeft = *new AcGePoint3d(dXLeft,dYLowerLeft-dRetreat,0);
		}

		ArCaDrPolyLine *drLine1 = new ArCaDrPolyLine();
		ArCaDrPolyLine *drLine2 = new ArCaDrPolyLine();

		drLine1->_p3dVertices.append(pt3dTopRight);//Left Lower
		drLine1->_p3dVertices.append(pt3dLowerRight);//Left Top
		drDrawer->Append(drLine1);

		drLine2->_p3dVertices.append(pt3dTopLeft);//Right Lower
		drLine2->_p3dVertices.append(pt3dLowerLeft);//Right Top
		drDrawer->Append(drLine2);
}

void ArCaGrating::GetHorizontalGrid(double dXDist, int iYPos, double dleght, ArCaDrDrawer *drDrawer) const
{
	//Calculate variables
	double dTotalLegth = _p3dLowerRight.x - _p3dLowerLeft.x;
	double dGridDif = (_p3dLowerRight.y + _lstRightGrid.at(iYPos)) - (_p3dLowerLeft.y + _lstLeftGrid.at(iYPos));

	//Y
	double dYLowerLeft;
	double dYLowerRight;
	double dYTopLeft;
	double dYTopRight;
	double dYDist;

	dYDist = (_p3dLowerLeft.y + _lstLeftGrid.at(iYPos)) - (_dGridHorThickness/2);
	dYLowerLeft = dYDist + ((dXDist * dGridDif)/dTotalLegth);
	dYLowerRight = dYDist + (((dXDist + dleght) * dGridDif)/dTotalLegth);
	dYTopLeft = dYDist + ((dXDist * dGridDif)/dTotalLegth) + _dGridHorThickness;
	dYTopRight = dYDist + (((dXDist + dleght) * dGridDif)/dTotalLegth) + _dGridHorThickness;

	//X
	double dXLeft = _p3dLowerLeft.x + dXDist;
	double dXRight = _p3dLowerLeft.x + dXDist + dleght;


	ArCaDrLine *drLowerLine = new ArCaDrLine();
	drLowerLine->_p3dStart = *new AcGePoint3d(dXLeft,dYLowerLeft,0);//Left Lower
	drLowerLine->_p3dEnd = *new AcGePoint3d(dXRight,dYLowerRight,0);//Right Lower
	drDrawer->Append(drLowerLine);

	ArCaDrLine *drTopLine = new ArCaDrLine();
	drTopLine->_p3dStart = *new AcGePoint3d(dXRight,dYTopRight,0);//Left Lower
	drTopLine->_p3dEnd = *new AcGePoint3d(dXLeft,dYTopLeft,0);//Right Lower
	drDrawer->Append(drTopLine);
}

//################################ OTHER METODES ###################################
Adesk::UInt32 ArCaGrating::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

void ArCaGrating::RedefineHorGridLeftPositions() 
{
	double dOldHeight = _lstLeftGrid.first();//The first double of array is Old Height
	double dNewHeight = _p3dTopLeft.y - _p3dLowerLeft.y;
	_lstLeftGrid.first() = dNewHeight;//Update Height in array

	for (int i=1; i<_lstLeftGrid.length(); i++)	//Start on second element of array	
		_lstLeftGrid.at(i) = (_lstLeftGrid.at(i) * dNewHeight)/dOldHeight;
	
}
void ArCaGrating::RedefineHorGridRightPositions() 
{
	double dOldHeight = _lstRightGrid.first();//The first double of array is Old Height
	double dNewHeight = _p3dTopRight.y - _p3dLowerRight.y;
	_lstRightGrid.first() = dNewHeight;//Update Height in array

	for (int i=1; i<_lstRightGrid.length(); i++)//Start on second element of array			
		_lstRightGrid.at(i) = (_lstRightGrid.at(i) * dNewHeight)/dOldHeight;

}
//################################## O S N A P ######################################
//--------------------- Osnap points protocol -------------------
Acad::ErrorStatus ArCaGrating::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	int gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}

//################################## G R I P S ######################################
//--------------------- Get Grips -------------------
Acad::ErrorStatus ArCaGrating::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;

	//Corner grips
	gripPoints.append(_p3dLowerLeft);
	gripPoints.append(_p3dLowerRight);
	gripPoints.append(_p3dTopRight);
	gripPoints.append(_p3dTopLeft);
	
	//Lower Middle grip
	AcGePoint3d pt3dGrip;
	pt3dGrip.set((_p3dLowerLeft.x + _p3dLowerRight.x)/2,(_p3dLowerLeft.y + _p3dLowerRight.y)/2,(_p3dLowerLeft.z + _p3dLowerRight.z)/2);
	gripPoints.append(pt3dGrip);
	//Upper Middle grip
	pt3dGrip.set((_p3dTopLeft.x + _p3dTopRight.x)/2,(_p3dTopLeft.y + _p3dTopRight.y)/2,(_p3dTopLeft.z + _p3dTopRight.z)/2);
	gripPoints.append(pt3dGrip);

	//Left horizontal grid grip
	for (int i=1; i<_lstLeftGrid.length(); i++)
		gripPoints.append(*new AcGePoint3d(_p3dLowerLeft.x,_p3dLowerLeft.y+_lstLeftGrid[i],0));

	//Right horizontal grid grip
	for (int i=1; i<_lstRightGrid.length(); i++)
		gripPoints.append(*new AcGePoint3d(_p3dLowerRight.x,_p3dLowerRight.y+_lstRightGrid[i],0));

	//Middle horizontal grid grip
	for (int i=1; i<_lstRightGrid.length(); i++)
	{
		pt3dGrip.set((_p3dTopLeft.x + _p3dTopRight.x)/2,(_p3dLowerLeft.y+_lstLeftGrid[i] + _p3dLowerRight.y+_lstRightGrid[i])/2,(_p3dTopLeft.z + _p3dTopRight.z)/2);
		gripPoints.append(pt3dGrip);
	}

	return (Acad::eOk) ;
}
//--------------------- Move Grips -------------------
Acad::ErrorStatus ArCaGrating::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;

	for(int i=0; i<indices.length(); i++)
	{
		int idx = indices.at(i);
		//_p3dLowerLeft
		if (idx==0)
		{
			_p3dLowerLeft += offset;
			_p3dTopLeft.x = _p3dLowerLeft.x;
			RedefineHorGridLeftPositions();
		}
		//_p3dLowerRight
		if (idx==1)
		{
			_p3dLowerRight += offset;
			_p3dTopRight.x = _p3dLowerRight.x;
			RedefineHorGridRightPositions();
		}
		//_p3dTopRight
		if (idx==2)
		{
			_p3dTopRight += offset;
			_p3dLowerRight.x = _p3dTopRight.x;
			RedefineHorGridRightPositions();
		}
		//_p3dTopLeft
		if (idx==3)
		{
			_p3dTopLeft += offset;
			_p3dLowerLeft.x = _p3dTopLeft.x;
			RedefineHorGridLeftPositions();
		}
		//Lower Middle grip
		if (idx==4)
		{
			_p3dLowerLeft.y += offset.y;
			_p3dLowerRight.y += offset.y;
			RedefineHorGridLeftPositions();
			RedefineHorGridRightPositions();
		}
		//Upper Middle grip
		if (idx==5)
		{
			_p3dTopLeft.y += offset.y;
			_p3dTopRight.y += offset.y;
			RedefineHorGridLeftPositions();
			RedefineHorGridRightPositions();
		}

		//Left horizontal grid grip
		for (int i=1; i<_lstLeftGrid.length(); i++)		
			if (idx==(5+i))			
				_lstLeftGrid[i] += offset.y;

		//Right horizontal grid grip
		for (int i=1; i<_lstRightGrid.length(); i++)		
			if (idx==(5+_lstLeftGrid.length()-1+i))			
				_lstRightGrid[i] += offset.y;

		//Middle horizontal grid grip
		for (int i=1; i<_lstRightGrid.length(); i++)		
			if (idx==(5+_lstLeftGrid.length()+_lstRightGrid.length()-2+i))
			{
				_lstLeftGrid[i] += offset.y;
				_lstRightGrid[i] += offset.y;
			}
	}
	return (Acad::eOk) ;
}

//################################## the GET and PUT from life! ######################################
Acad::ErrorStatus ArCaGrating::put__p3dLowerLeft(AcGePoint3d newp3d)
{
	assertWriteEnabled ();
	_p3dLowerLeft = newp3d;
	return (Acad::eOk) ;
}
Acad::ErrorStatus ArCaGrating::put__p3dTopRight(AcGePoint3d newp3d)
{
	assertWriteEnabled ();

	_p3dTopRight = newp3d;
	_p3dTopLeft.set(_p3dLowerLeft.x, _p3dTopRight.y, 0);
	_p3dLowerRight.set(_p3dTopRight.x, _p3dLowerLeft.y, 0);

	if (_lstLeftGrid.isEmpty())
	{
		double sLeftDif = (_p3dTopLeft.y - _p3dLowerLeft.y)/3;		
		_lstLeftGrid.append(_p3dTopLeft.y - _p3dLowerLeft.y);//keeping Height
		_lstLeftGrid.append(sLeftDif);
		_lstLeftGrid.append(sLeftDif*2);

		double sRightDif = (_p3dTopRight.y - _p3dLowerRight.y)/3;
		_lstRightGrid.append(_p3dTopRight.y - _p3dLowerRight.y);//keeping Height
		_lstRightGrid.append(sRightDif);
		_lstRightGrid.append(sRightDif*2);
	}
	return (Acad::eOk) ;
}
//Get and Put in TOP
ArCaGrating::TipType ArCaGrating::get__kTipTypeTop(void) const
{
	assertReadEnabled ();	
	return (_TopTipGrid) ;	
}
Acad::ErrorStatus ArCaGrating::put__kTipTypeTop(TipType newType)
{
	assertWriteEnabled ();
	_TopTipGrid = newType;
	return (Acad::eOk) ;
}
Acad::ErrorStatus ArCaGrating::put__kTipTypeTopByString(int iType)
{
	assertWriteEnabled ();
	_TopTipGrid = (TipType)iType;
	return (Acad::eOk) ;
}

//Get and Put in LOWER
ArCaGrating::TipType ArCaGrating::get__kTipTypeLower(void) const
{
	assertReadEnabled ();	
	return (_LowerTipGrid) ;	
}
Acad::ErrorStatus ArCaGrating::put__kTipTypeLower(TipType newType)
{
	assertWriteEnabled ();
	_LowerTipGrid = newType;
	return (Acad::eOk) ;
}
Acad::ErrorStatus ArCaGrating::put__kTipTypeLowerByString(int iType)
{
	assertWriteEnabled ();
	_LowerTipGrid = (TipType)iType;
	return (Acad::eOk) ;
}

Acad::ErrorStatus ArCaGrating::put__dOpening(double newVal)
{
	assertWriteEnabled ();
	_dOpening = newVal;
	return (Acad::eOk) ;
}

double ArCaGrating::get__dOpening(void) const
{
	assertReadEnabled ();	
	return (_dOpening) ;	
}

Acad::ErrorStatus ArCaGrating::put__dGridVerThickness(double newVal)
{
	assertWriteEnabled ();
	_dGridVerThickness = newVal;
	return (Acad::eOk) ;
}

double ArCaGrating::get__dGridVerThickness(void) const
{
	assertReadEnabled ();	
	return (_dGridVerThickness) ;	
}

Acad::ErrorStatus ArCaGrating::put__dGridHorThickness(double newVal)
{
	assertWriteEnabled ();
	_dGridHorThickness = newVal;
	return (Acad::eOk) ;
}

double ArCaGrating::get__dGridHorThickness(void) const
{
	assertReadEnabled ();	
	return (_dGridHorThickness) ;	
}

Acad::ErrorStatus ArCaGrating::put__iGridHorQuantiy(int newVal)
{
	assertWriteEnabled ();	
	double dDif;

	dDif= (_p3dTopLeft.y - _p3dLowerLeft.y)/(newVal+1);	
	_lstLeftGrid.removeAll();
	_lstLeftGrid.append(_p3dTopLeft.y - _p3dLowerLeft.y);
	for (int i=1; i<=newVal; i++)	
		_lstLeftGrid.append(dDif*i);
	
	dDif= (_p3dTopRight.y - _p3dLowerRight.y)/(newVal+1);	
	_lstRightGrid.removeAll();
	_lstRightGrid.append(_p3dTopRight.y - _p3dLowerRight.y);
	for (int i=1; i<=newVal; i++)	
		_lstRightGrid.append(dDif*i);
	
	return (Acad::eOk) ;
}

int ArCaGrating::get__iGridHorQuantiy(void) const
{
	assertReadEnabled ();	
	return (_lstLeftGrid.length()-1) ;	
}

//################################ T R A N S F O R M B Y ###################################
Acad::ErrorStatus ArCaGrating::subTransformBy(const AcGeMatrix3d & xform)
{
	assertWriteEnabled();
	_p3dLowerLeft.transformBy(xform);
	_p3dLowerRight.transformBy(xform);
	_p3dTopRight.transformBy(xform);
	_p3dTopLeft.transformBy(xform);
	RedefineHorGridLeftPositions();
	RedefineHorGridRightPositions();
	return (Acad::eOk);
}
//################################ PROPERTY PALLET ###################################
/// <summary>
/// Called to provide the available properties
/// </summary>
void ArCaGrating::makeMembers( AcRxMemberCollectionBuilder& collectionBuilder, void* customData)
{
	//Vertical
	collectionBuilder.add((AcRxMember*)OpeningProp::makeProp());
	collectionBuilder.add((AcRxMember*)VertThicknessProp::makeProp());
	collectionBuilder.add((AcRxMember*)TopTipTypeProp::makeProp());
	collectionBuilder.add((AcRxMember*)LowerTipTypeProp::makeProp());
	//Horizontal
	collectionBuilder.add((AcRxMember*)QuantityProp::makeProp());
	collectionBuilder.add((AcRxMember*)HorizThicknessProp::makeProp());
}

/////////////////////////////////////////////////////////////////////
// Utility Fuctions /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

/// <summary>
/// Registers our application name if it is not registered already
/// </summary>
static Acad::ErrorStatus createRegApp(AcDbDatabase * pDb, const ACHAR * pAppName)
{
	Acad::ErrorStatus es;

	if (pDb == NULL)
		return Acad::eInvalidInput; 

	AcDbRegAppTablePointer appTable(pDb->regAppTableId(), AcDb::kForWrite);

	if((es = appTable.openStatus()) != Acad::eOk)
		return es;
    
	if(appTable->has(pAppName))
		return Acad::eOk;
    
	AcDbRegAppTableRecordPointer appTableRecord;
	appTableRecord.create(); 
	appTableRecord->setName(pAppName);
    
	if((es = appTable->add(appTableRecord)) != Acad::eOk)
		return es;

	return Acad::eOk;
}


/////////////////////////////////////////////////////////////////////
// OpeningProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * OpeningProp::kMyAppName = _T("OpeningApp"); 
const ACHAR * OpeningProp::kCategoryName = _T("Vertical Bars"); 

AcRxCategory * OpeningProp::category = NULL;

OpeningProp::OpeningProp() : AcRxProperty(_T("Opening"), AcRxValueType::Desc<double>::value())
{
	if (category == NULL)
	{
		AcRxCategory * parent =  AcRxCategory::rootCategory();
		category = parent->findDescendant(kCategoryName); 
		if (category == NULL)
			category = new AcRxCategory(kCategoryName, parent); 
	}

	attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
	attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}

/// <summary>
/// This is called by the system to get the property value for a specific object
/// </summary>
Acad::ErrorStatus OpeningProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dOpening();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus OpeningProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), OpeningProp::kMyAppName);

	ent->put__dOpening(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * OpeningProp::makeProp() 
{ 
	return new OpeningProp(); 
}

/////////////////////////////////////////////////////////////////////
// VertThicknessProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * VertThicknessProp::kMyAppName = _T("gratingVertThickApp"); 
const ACHAR * VertThicknessProp::kCategoryName = _T("Vertical Bars"); 

AcRxCategory * VertThicknessProp::category = NULL;

VertThicknessProp::VertThicknessProp() : AcRxProperty(_T("Thickness"), AcRxValueType::Desc<double>::value())
{
	if (category == NULL)
	{
		AcRxCategory * parent =  AcRxCategory::rootCategory();
		category = parent->findDescendant(kCategoryName); 
		if (category == NULL)
			category = new AcRxCategory(kCategoryName, parent); 
	}

	attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
	attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}

/// <summary>
/// This is called by the system to get the property value for a specific object
/// </summary>
Acad::ErrorStatus VertThicknessProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dGridVerThickness();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus VertThicknessProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), VertThicknessProp::kMyAppName);

	ent->put__dGridVerThickness(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * VertThicknessProp::makeProp() 
{ 
	return new VertThicknessProp(); 
}

/////////////////////////////////////////////////////////////////////
// TopTipTypeProp ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
 
void makeTopTipTypePropTemplateProperties(class AcRxMemberCollectionBuilder & collectionBuilder, void*);
 
template<>
int TopTipTypePropTemplate<ArCaGrating::TipType>::subToString(const void *instance, ACHAR *buffer, size_t sizeInACHARs, AcRxValueType::StringFormat format) const
{
  const ACHAR* formatString = L"%d";
  ArCaGrating::TipType & value = *(ArCaGrating::TipType*)instance;
  if (buffer==NULL)
    return _scwprintf(formatString,value);
  return swprintf_s(buffer,sizeInACHARs,formatString,value);
}
 
template<>
bool TopTipTypePropTemplate<ArCaGrating::TipType>::subEqualTo( const void *a, const void* b) const
{
  ArCaGrating::TipType & v1 = *(ArCaGrating::TipType*)a;
  ArCaGrating::TipType & v2 = *(ArCaGrating::TipType*)b;
  return v1==v2;
}
 
template<>
struct AcRxValueType::Desc< ArCaGrating::TipType >
{
  __declspec(dllexport) static const AcRxValueType& value() throw();
  static void del();
};
 
TopTipTypePropTemplate<ArCaGrating::TipType>* s_pTopTipTypePropTemplate = NULL;
 
const AcRxValueType& AcRxValueType::Desc< ArCaGrating::TipType >::value() throw()
{
  if (s_pTopTipTypePropTemplate==NULL)
  {
    s_pTopTipTypePropTemplate = new TopTipTypePropTemplate<ArCaGrating::TipType>(L"MyEnumProperties",&makeTopTipTypePropTemplateProperties);

    AcRxEnumTag* pTag;
    pTag = new  AcRxEnumTag  (L"Plane", (int)0);
    s_pTopTipTypePropTemplate->append(*pTag);
    pTag = new  AcRxEnumTag  (L"Arrow", (int)1);
    s_pTopTipTypePropTemplate->append(*pTag);
    pTag = new  AcRxEnumTag  (L"Pointed", (int)2);
    s_pTopTipTypePropTemplate->append(*pTag);
	pTag = new  AcRxEnumTag  (L"Round", (int)3);
    s_pTopTipTypePropTemplate->append(*pTag);
  }
  return *s_pTopTipTypePropTemplate;
};
// This should be called when the value type is not needed anymore
// Best call it when the module gets unloaded (On_kUnloadAppMsg)
void AcRxValueType::Desc< ArCaGrating::TipType >::del()
{
  if (s_pTopTipTypePropTemplate)
  {
    const ACHAR * name = s_pTopTipTypePropTemplate->name();
 
    if (acrxSysRegistry())
      acrxClassDictionary->remove(s_pTopTipTypePropTemplate->name());
 
    s_pTopTipTypePropTemplate = NULL;
  }
};
 
void makeTopTipTypePropTemplateProperties(
  class AcRxMemberCollectionBuilder & collectionBuilder, void*)
{
}
 
// TopTipTypeProp 
 
const ACHAR * TopTipTypeProp::kCategoryName = _T("Vertical Bars");
 
AcRxCategory * TopTipTypeProp::category = NULL;
 
TopTipTypeProp::TopTipTypeProp() : AcRxProperty(_T("Top Tip"), AcRxValueType::Desc< ArCaGrating::TipType >::value())
{
  if (category == NULL)
  {
    AcRxCategory * parent =  AcRxCategory::rootCategory();
    category = parent->findDescendant(kCategoryName);
    if (category == NULL)
      category = new AcRxCategory(kCategoryName, parent);
  }

  attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
  attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}
 
TopTipTypeProp::~TopTipTypeProp()
{
}
 
/// <summary>
/// This is called by the system to get the property value for
/// a specific object
/// </summary>
Acad::ErrorStatus TopTipTypeProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
  AcDbEntity * ent = AcDbEntity::cast(pO);
  if (ent == NULL)
    return Acad::eNotThatKindOfClass;
 
  // Get the value from the custom entity
  // or wherever we stored the value
  ArCaGrating * pCustonEnt = ArCaGrating::cast(pO);
  value = AcRxValue(static_cast<ArCaGrating::TipType>(pCustonEnt->get__kTipTypeTop())); 
 
  return Acad::eOk;
}
 
/// <summary>
/// This is called by the system to retrieve the property value for
/// a specific object
/// </summary>
Acad::ErrorStatus TopTipTypeProp::subSetValue( AcRxObject* pO, const AcRxValue& value) const
{
  AcDbEntity * ent = AcDbEntity::cast(pO);
  if (ent == NULL)
    return Acad::eNotThatKindOfClass;
 
  const ArCaGrating::TipType * val = rxvalue_cast<ArCaGrating::TipType>(&value);
  if (val == NULL)
    return Acad::eInvalidInput;
 
  // Set the property of the custom entity
  // or store it wherever we want to
  ArCaGrating * pCustonEnt = ArCaGrating::cast(pO);
  pCustonEnt->put__kTipTypeTop(*val); 
 
  return Acad::eOk;
}
 
/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * TopTipTypeProp::makeProp()
{
  return new TopTipTypeProp();
}

// LowerTipTypeProp 
 
const ACHAR * LowerTipTypeProp::kCategoryName = _T("Vertical Bars");
 
AcRxCategory * LowerTipTypeProp::category = NULL;
 
LowerTipTypeProp::LowerTipTypeProp() : AcRxProperty(_T("Lower Tip"), AcRxValueType::Desc< ArCaGrating::TipType >::value())
{
  if (category == NULL)
  {
    AcRxCategory * parent =  AcRxCategory::rootCategory();
    category = parent->findDescendant(kCategoryName);
    if (category == NULL)
      category = new AcRxCategory(kCategoryName, parent);
  }

  attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
  attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}
 
LowerTipTypeProp::~LowerTipTypeProp()
{
}
 
/// <summary>
/// This is called by the system to get the property value for
/// a specific object
/// </summary>
Acad::ErrorStatus LowerTipTypeProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
  AcDbEntity * ent = AcDbEntity::cast(pO);
  if (ent == NULL)
    return Acad::eNotThatKindOfClass;
 
  // Get the value from the custom entity
  // or wherever we stored the value
  ArCaGrating * pCustonEnt = ArCaGrating::cast(pO);
  value = AcRxValue(static_cast<ArCaGrating::TipType>(pCustonEnt->get__kTipTypeLower())); 
 
  return Acad::eOk;
}
 
/// <summary>
/// This is called by the system to retrieve the property value for
/// a specific object
/// </summary>
Acad::ErrorStatus LowerTipTypeProp::subSetValue( AcRxObject* pO, const AcRxValue& value) const
{
  AcDbEntity * ent = AcDbEntity::cast(pO);
  if (ent == NULL)
    return Acad::eNotThatKindOfClass;
 
  const ArCaGrating::TipType * val = rxvalue_cast<ArCaGrating::TipType>(&value);
  if (val == NULL)
    return Acad::eInvalidInput;
 
  // Set the property of the custom entity
  // or store it wherever we want to
  ArCaGrating * pCustonEnt = ArCaGrating::cast(pO);
  pCustonEnt->put__kTipTypeLower(*val); 
 
  return Acad::eOk;
}
 
/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * LowerTipTypeProp::makeProp()
{
  return new LowerTipTypeProp();
}

/////////////////////////////////////////////////////////////////////
// QuantityProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * QuantityProp::kMyAppName = _T("QuantityApp"); 
const ACHAR * QuantityProp::kCategoryName = _T("Horizontal Bars"); 

AcRxCategory * QuantityProp::category = NULL;

QuantityProp::QuantityProp() : AcRxProperty(_T("Quantity"), AcRxValueType::Desc<int>::value())
{
	if (category == NULL)
	{
		AcRxCategory * parent =  AcRxCategory::rootCategory();
		category = parent->findDescendant(kCategoryName); 
		if (category == NULL)
			category = new AcRxCategory(kCategoryName, parent); 
	}

	attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
	attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}

/// <summary>
/// This is called by the system to get the property value for a specific object
/// </summary>
Acad::ErrorStatus QuantityProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	int val = 0;
	val = ent->get__iGridHorQuantiy();

	value = AcRxValue(static_cast<int>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus QuantityProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const int * val = rxvalue_cast<int>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), QuantityProp::kMyAppName);

	ent->put__iGridHorQuantiy(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * QuantityProp::makeProp() 
{ 
	return new QuantityProp(); 
}

/////////////////////////////////////////////////////////////////////
// HorizThicknessProp /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

const ACHAR * HorizThicknessProp::kMyAppName = _T("gratingHorizThickApp"); 
const ACHAR * HorizThicknessProp::kCategoryName = _T("Horizontal Bars"); 

AcRxCategory * HorizThicknessProp::category = NULL;

HorizThicknessProp::HorizThicknessProp() : AcRxProperty(_T("Thickness"), AcRxValueType::Desc<double>::value())
{
	if (category == NULL)
	{
		AcRxCategory * parent =  AcRxCategory::rootCategory();
		category = parent->findDescendant(kCategoryName); 
		if (category == NULL)
			category = new AcRxCategory(kCategoryName, parent); 
	}

	attributes().add(new AcRxUiPlacementAttribute(kCategoryName, 0));
	attributes().add(new AcRxGenerateDynamicPropertiesAttribute());
}

/// <summary>
/// This is called by the system to get the property value for a specific object
/// </summary>
Acad::ErrorStatus HorizThicknessProp::subGetValue(const AcRxObject* pO, AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass; // or eNotApplicable

	double val = -1;
	val = ent->get__dGridHorThickness();

	value = AcRxValue(static_cast<double>(val));  

	return Acad::eOk; 
}

/// <summary>
/// This is called by the system to retrieve the property value for a specific object
/// </summary>
Acad::ErrorStatus HorizThicknessProp::subSetValue(AcRxObject* pO, const AcRxValue& value) const
{
	ArCaGrating * ent = ArCaGrating::cast(pO);
	if (ent == NULL)
		return Acad::eNotThatKindOfClass;

	const double * val = rxvalue_cast<double>(&value);
	if (val == NULL)
		return Acad::eInvalidInput; 

	createRegApp(ent->database(), HorizThicknessProp::kMyAppName);

	ent->put__dGridHorThickness(*val);

	return Acad::eOk; 
}

/// <summary>
/// This is called to create an instance of our class
/// </summary>
AcRxObject * HorizThicknessProp::makeProp() 
{ 
	return new HorizThicknessProp(); 
}
