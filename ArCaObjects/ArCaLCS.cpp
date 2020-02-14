#include "StdAfx.h"
#include "ArCaLCS.h"

ArCaLCS::ArCaLCS(void)
{
	_dScale = 10;
}

ArCaLCS::~ArCaLCS(void)
{
}

void ArCaLCS::SetAlignSys(AcGePoint3d ptBase, AcGePoint3d ptAux)
{
	//Cauculate _p3dAux value
	double dist = ptBase.distanceTo(ptAux);
	AcGePoint3d p3dTrans = ptAux;

	p3dTrans.x -= ptBase.x;
	p3dTrans.y -= ptBase.y;
	p3dTrans.z -= ptBase.z;

	double dX = (p3dTrans.x * _dScale) / dist;
	double dY = (p3dTrans.y * _dScale) / dist;
	double dZ = (p3dTrans.z * _dScale) / dist;
	
	ptAux.set(dX, dY, dZ);

	ptAux.x += ptBase.x;
	ptAux.y += ptBase.y;
	ptAux.z += ptBase.z;

	//Set _p3dBase and _p3dAux values
	_p3dBase = ptBase;
	_p3dAux = ptAux;

	//Set _matAlignSys value
	AcGeVector3d vDir = _p3dAux - _p3dBase;
	double distt = _p3dAux.distanceTo(_p3dBase);
	AcGeVector3d vDir2 = (AcGePoint3d(distt,0,0) - AcGePoint3d::kOrigin);
	_matAlignSys.setToAlignCoordSys(AcGePoint3d::kOrigin, vDir2, AcGeVector3d::kYAxis, AcGeVector3d::kZAxis, _p3dBase, vDir, vDir.perpVector(), vDir.normal());

}

AcGeMatrix3d ArCaLCS::GetAlignSysNomalized() const
{
	//Create Inverted Matrix
	AcGeMatrix3d matAlignSysInverted;
	AcGeVector3d vDir = _p3dAux - _p3dBase;
	AcGeVector3d vPer = vDir.perpVector();
	AcGeVector3d vNor = vDir.normal();
	matAlignSysInverted.setToAlignCoordSys( _p3dBase, vDir, vDir.perpVector(), AcGeVector3d::kZAxis, AcGePoint3d::kOrigin, AcGeVector3d(_p3dBase.distanceTo(_p3dAux),0,0), AcGeVector3d::kYAxis, AcGeVector3d::kZAxis);
	return matAlignSysInverted;
}

AcGeMatrix3d ArCaLCS::GetAlignSysByPoint(AcGePoint3d ptToPoint) const
{
	//Cauculate ptAux value
	double dist = _p3dBase.distanceTo(_p3dAux);
	AcGePoint3d p3dTrans = _p3dAux;

	p3dTrans.x -= _p3dBase.x;
	p3dTrans.y -= _p3dBase.y;
	p3dTrans.z -= _p3dBase.z;

	double dX = (p3dTrans.x * _dScale) / dist;
	double dY = (p3dTrans.y * _dScale) / dist;
	double dZ = (p3dTrans.z * _dScale) / dist;
	
	AcGePoint3d ptAux;
	ptAux.set(dX, dY, dZ);

	ptAux.x += ptToPoint.x;
	ptAux.y += ptToPoint.y;
	ptAux.z += ptToPoint.z;

	//Set matAlignSys value
	AcGeVector3d vDir = ptAux - ptToPoint;
	double distt = ptAux.distanceTo(ptToPoint);
	AcGeVector3d vDir2 = (AcGePoint3d(distt,0,0) - AcGePoint3d::kOrigin);
	AcGeMatrix3d matAlignSys;
	matAlignSys.setToAlignCoordSys(AcGePoint3d::kOrigin, vDir2, AcGeVector3d::kYAxis, AcGeVector3d::kZAxis, ptToPoint, vDir, vDir.perpVector(), vDir.normal());
	return matAlignSys;
}