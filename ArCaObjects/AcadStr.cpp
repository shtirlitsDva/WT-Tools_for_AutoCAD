#include "StdAfx.h"
#include "AcadStr.h"

//-----------------------------------------------------------------------------------
//	AutoCAD system variables
//-----------------------------------------------------------------------------------
const TCHAR* AcadVar::adserr		= _T("errno");
const TCHAR* AcadVar::cmdecho		= _T("cmdecho");
const TCHAR* AcadVar::cvport		= _T("cvport");
const TCHAR* AcadVar::filedia		= _T("filedia");
const TCHAR* AcadVar::snapang		= _T("snapang");
const TCHAR* AcadVar::viewdir		= _T("viewdir");
const TCHAR* AcadVar::viewsize		= _T("viewsize");

//-----------------------------------------------------------------------------------
//	AutoCAD command names
//-----------------------------------------------------------------------------------
const TCHAR* AcadCmd::linetype		= _T("'_.LINETYPE");
const TCHAR* AcadCmd::menuload		= _T("_.MENULOAD");
const TCHAR* AcadCmd::menuunload	= _T("_.MENUUNLOAD");
const TCHAR* AcadCmd::pan			= _T("'_.-PAN");
const TCHAR* AcadCmd::zoom			= _T("'_.ZOOM");

//-----------------------------------------------------------------------------------
//	AutoCAD command options
//-----------------------------------------------------------------------------------
const TCHAR* AcadCmdOpt::load		= _T("_LOAD");

//-----------------------------------------------------------------------------------
//	AutoCAD API strings
//-----------------------------------------------------------------------------------
const TCHAR* AcadApi::colorDbox		= _T("acad_colordlg");

//-----------------------------------------------------------------------------------
//	AutoCAD common strings
//-----------------------------------------------------------------------------------
const TCHAR* AcadString::continuous	= _T("CONTINUOUS");		// needs to be translated
const TCHAR* AcadString::bylayer	= _T("BYLAYER");		// needs to be translated
const TCHAR* AcadString::byblock	= _T("BYBLOCK");		// needs to be translated
const TCHAR* AcadString::zero		= _T("0");			// default layer
const TCHAR* AcadString::wcAll		= _T("*");			// wild card for all
const TCHAR* AcadString::nullStr	= _T("");			// ""
const TCHAR* AcadString::openBrace	= _T("{");
const TCHAR* AcadString::endBrace	= _T("}");
const TCHAR* AcadString::standard	= _T("STANDARD");
const TCHAR* AcadString::varies1	= _T("*VARIES*");
const TCHAR* AcadString::none       = _T("*NONE*");
const TCHAR* AcadString::varies2	= _T("VARIES");

//-----------------------------------------------------------------------------------
//	AutoCAD files
//-----------------------------------------------------------------------------------
const TCHAR* AcadFile::ltypesStandard	= _T("acad.lin");
const TCHAR* AcadFile::ltypesComplex	= _T("ltypeshp.lin");

//-----------------------------------------------------------------------------------
//	AcadUtilLinetypes
//-----------------------------------------------------------------------------------
const TCHAR* AcadUtilLinetypes::continuous	= _T("CONTINUOUS");
const TCHAR* AcadUtilLinetypes::dash		= _T("ACAD_ISO02W100");
const TCHAR* AcadUtilLinetypes::dashdot		= _T("ACAD_ISO10W100");
const TCHAR* AcadUtilLinetypes::dashed		= _T("DASHED");
const TCHAR* AcadUtilLinetypes::dot			= _T("DOT");
const TCHAR* AcadUtilLinetypes::center2		= _T("CENTER2");
