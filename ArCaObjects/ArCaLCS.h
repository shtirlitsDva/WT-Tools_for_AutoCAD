#pragma once

class ArCaLCS
{
public:
	ArCaLCS(void);
	~ArCaLCS(void);

	void SetAlignSys(AcGePoint3d ptBase, AcGePoint3d ptAux);
	AcGeMatrix3d GetAlignSysByPoint(AcGePoint3d ptToPoint) const;
	AcGeMatrix3d GetAlignSysNomalized() const;

	AcGePoint3d _p3dBase;
	AcGePoint3d _p3dAux;
	AcGeMatrix3d _matAlignSys;
	double _dScale;
};
