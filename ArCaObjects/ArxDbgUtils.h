#ifndef ARXDBG_UTILS_H
#define ARXDBG_UTILS_H

#pragma warning(disable:4251)  //Problemas com CString
#pragma warning(disable:4275)  //Problemas com CStringList

//-----------------------------------------------------------------------------------
//	struct ArxDbgUtils
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS ArxDbgUtils {

	enum Color {
		kByblock	= 0,
		kRed		= 1,
		kYellow		= 2,
		kGreen		= 3,
		kCyan		= 4,
		kBlue		= 5,
		kMagenta	= 6,
		kWhite		= 7,
		kBylayer	= 256
	};

	enum LinetypeFile {
		kStandardLinetypes,
		kComplexLinetypes,
		kAllLinetypes
	};

	enum LinearUnit {
		kCurrentLU 		= -1,
		kScientific		= 1,
		kDecimal		= 2,
		kEngineering	= 3,
		kArchitectural	= 4,
		kFractional		= 5
	};

	enum AngularUnit {
		kCurrentAU		= -1,
		kDegrees		= 0,
		kDegMinSec		= 1,
		kGrads			= 2,
		kRadians		= 3,
		kSurveyor		= 4
	};

	enum Precision {
		kCurrentPrecLU	= -1,
		kCurrentPrecAU	= -1
	};

static AcDbSymbolTable*		openSymbolTable(AcRxClass* classType, AcDb::OpenMode mode, AcDbDatabase* db = NULL);
static AcDbSymbolTableRecord*	openSymbolTableRecord(AcRxClass* symTblClass,LPCTSTR name,AcDb::OpenMode openMode,AcDbDatabase* db = NULL);
static Acad::ErrorStatus	nameToSymbolId(AcRxClass* symTblClass, LPCTSTR name,AcDbObjectId& symbolId, AcDbDatabase* db = NULL);
static AcDbLayerTable*		openLayerTable(AcDb::OpenMode mode, AcDbDatabase* db = NULL);
static Acad::ErrorStatus	addNewTextStyle(AcDbTextStyleTable*& pTextStyleTbl, LPCTSTR TextName, LPCTSTR FontName, double TSize);
static Acad::ErrorStatus	GetLayerIdFromString(LPCTSTR layerName, AcDbObjectId& idLayer, BOOL createIfNotFound, AcDbDatabase* db = NULL);

};
#endif	// ARXDBG_UTILS_H