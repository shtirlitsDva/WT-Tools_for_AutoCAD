#include "StdAfx.h"
#include "ArxDbgUtils.h"
#include "AcadStr.h"

#define szRDS _RXST(_T("Asdk"))

//-----------------------------------------------------------------------------------
//  ArxDbgUtils::openSymbolTable
//-----------------------------------------------------------------------------------
//	get a generic symbol table pointer from a specific symbol table.
//-----------------------------------------------------------------------------------
AcDbSymbolTable* ArxDbgUtils::openSymbolTable(AcRxClass* symTblClass,
											  AcDb::OpenMode mode, AcDbDatabase* db)
{
	// if they didn't pass in a specific database, use the current one
	if (db == NULL) {
		db = acdbHostApplicationServices()->workingDatabase();
		if (db == NULL)
			return(NULL);
	}

	AcDbSymbolTable* symTbl = NULL;
	Acad::ErrorStatus es;

	if (symTblClass == AcDbBlockTableRecord::desc()) {
		AcDbBlockTable* blkTbl;
		es = db->getBlockTable(blkTbl, mode);
		symTbl = blkTbl;
	}
	else if (symTblClass == AcDbDimStyleTableRecord::desc()) {
		AcDbDimStyleTable* dimTbl;
		es = db->getDimStyleTable(dimTbl, mode);
		symTbl = dimTbl;
	}
	else if (symTblClass == AcDbLayerTableRecord::desc()) {
		AcDbLayerTable* layerTbl;
		es = db->getLayerTable(layerTbl, mode);
		symTbl = layerTbl;
	}
	else if (symTblClass == AcDbLinetypeTableRecord::desc()) {
		AcDbLinetypeTable* ltypeTbl;
		es = db->getLinetypeTable(ltypeTbl, mode);
		symTbl = ltypeTbl;
	}
	else if (symTblClass == AcDbTextStyleTableRecord::desc()) {
		AcDbTextStyleTable* textTbl;
		es = db->getTextStyleTable(textTbl, mode);
		symTbl = textTbl;
	}
	else if (symTblClass == AcDbRegAppTableRecord::desc()) {
		AcDbRegAppTable* appTbl;
		es = db->getRegAppTable(appTbl, mode);
		symTbl = appTbl;
	}
	else if (symTblClass == AcDbUCSTableRecord::desc()) {
		AcDbUCSTable* ucsTbl;
		es = db->getUCSTable(ucsTbl, mode);
		symTbl = ucsTbl;
	}
	else if (symTblClass == AcDbViewTableRecord::desc()) {
		AcDbViewTable* viewTbl;
		es = db->getViewTable(viewTbl, mode);
		symTbl = viewTbl;
	}
	else if (symTblClass == AcDbViewportTableRecord::desc()) {
		AcDbViewportTable* vportTbl;
		es = db->getViewportTable(vportTbl, mode);
		symTbl = vportTbl;
	}
	else
		es = Acad::eInvalidInput;	// passed in a class type that is illegal

	ASSERT(es == Acad::eOk);	// should never fail!
	if (es != Acad::eOk) {
		//ArxDbgUtils::rxErrorAlert(es);
		acutPrintf(_T("\n\n\** Error in [ArxDbgUtils::openSymbolTable] :(\n\n"));
		return(NULL);
	}
	else
		return(symTbl);
}

//-----------------------------------------------------------------------------------
//  ArxDbgUtils::openSymbolTableRecord
//-----------------------------------------------------------------------------------
AcDbSymbolTableRecord* ArxDbgUtils::openSymbolTableRecord(AcRxClass* symTblClass,
														  LPCTSTR name,
														  AcDb::OpenMode openMode,
														  AcDbDatabase* db)
{
	AcDbSymbolTable* symTbl;
	AcDbSymbolTableRecord* pRecord = NULL;

	if ((symTbl = openSymbolTable(symTblClass, AcDb::kForRead, db)) == NULL){
		//ArxDbgUtils::rxErrorAlert(Acad::eInvalidInput);
		acutPrintf(_T("\n\n\** Error in [ArxDbgUtils::openSymbolTableRecord] id not found in TableRecord :(\n\n"));
		return (NULL);
	}

	Acad::ErrorStatus es;
	es = symTbl->getAt(name, pRecord, openMode);
	symTbl->close();

	ASSERT(es == Acad::eOk);	// should never fail!
	if (es != Acad::eOk) {
		//ArxDbgUtils::rxErrorAlert(es);
		return(NULL);
	}
	else return(pRecord);
}

//-----------------------------------------------------------------------------------
//  ArxDbgUtils::nameToSymbolId
//-----------------------------------------------------------------------------------
Acad::ErrorStatus ArxDbgUtils::nameToSymbolId(AcRxClass* symTblClass, LPCTSTR name,
											  AcDbObjectId& recordId, AcDbDatabase* db)
{
	AcDbSymbolTable* symTbl;
	if ((symTbl = openSymbolTable(symTblClass, AcDb::kForRead, db)) == NULL)
		return(Acad::eInvalidInput);

	Acad::ErrorStatus es;
	es = symTbl->getAt(name, recordId);
	symTbl->close();
	return(es);
}

//-----------------------------------------------------------------------------------
//  ArxDbgUtils::openLayerTable
//-----------------------------------------------------------------------------------
//	since layer table is opened frequently, have special access function
//	to make it easier
//-----------------------------------------------------------------------------------
AcDbLayerTable* ArxDbgUtils::openLayerTable(AcDb::OpenMode mode, AcDbDatabase* db)
{
	AcDbLayerTable* layerTbl;
	Acad::ErrorStatus es;
	if (db == NULL) {
		db = acdbHostApplicationServices()->workingDatabase();
		if (db == NULL)
			return(NULL);
	}

	es = db->getLayerTable(layerTbl, mode);
	ASSERT(es == Acad::eOk);	// should never fail!
	if (es != Acad::eOk) {
		//ArxDbgUtils::rxErrorAlert(es);
		acutPrintf(_T("\n\n\** Error in [ArxDbgUtils::openLayerTable] :(\n\n"));
		return(NULL);
	}
	else
		return(layerTbl);
}
//-----------------------------------------------------------------------------------
//	ArxDbgUtils::addNewTextStyle
//-----------------------------------------------------------------------------------
Acad::ErrorStatus ArxDbgUtils::addNewTextStyle(AcDbTextStyleTable*& pTextStyleTbl,
											   LPCTSTR TextName,
											   LPCTSTR FontName,
											   double  TSize)
{
	if (pTextStyleTbl->has(TextName)) return Acad::eOk;

	AcDbTextStyleTableRecord *pTextStyleTblRcd = new AcDbTextStyleTableRecord;
	pTextStyleTblRcd->setName(TextName);
	pTextStyleTblRcd->setBigFontFileName(_T(""));
	pTextStyleTblRcd->setFileName(FontName);
	pTextStyleTblRcd->setFlagBits(0);
	pTextStyleTblRcd->setIsShapeFile(Adesk::kFalse);
	pTextStyleTblRcd->setIsVertical(Adesk::kFalse);
	pTextStyleTblRcd->setObliquingAngle(0.0);
	pTextStyleTblRcd->setPriorSize(1.0);
	pTextStyleTblRcd->setTextSize(TSize);
	pTextStyleTblRcd->setXScale(1.0);
	pTextStyleTbl->add(pTextStyleTblRcd);
	pTextStyleTblRcd->close();

	return Acad::eOk;
}
//-----------------------------------------------------------------------------------
//	ArxDbgUtils::GetLayerIdFromString
//-----------------------------------------------------------------------------------
Acad::ErrorStatus ArxDbgUtils::GetLayerIdFromString(LPCTSTR layerName, 
												   AcDbObjectId& idLayer, 
												   BOOL createIfNotFound,
												   AcDbDatabase* db)
{
 	//se o nome do layer for vazio, nem tnenta fazer nada
 	if (_tcslen(layerName) == 0)
 		return Acad::eInvalidInput;
 
 	Acad::ErrorStatus es;
 
 	//******************** if layer already exists, then just return ****************
 	AcDbLayerTable* layTbl = ArxDbgUtils::openLayerTable(AcDb::kForRead, db);
 	if (layTbl->has(layerName)) {
 		es = layTbl->getAt(layerName, idLayer, Adesk::kFalse);
 		if (es != Acad::eOk) {
 			ASSERT(0);
 			layTbl->close();
 			return(es);
 		}
 		layTbl->close();
 		return(Acad::eOk);
 	}
 
 	//*********************** se não criar, retornar o layer zero *******************
 	if (!createIfNotFound){
 		es = layTbl->getAt(AcadString::zero, idLayer, Adesk::kTrue);
 		if (es != Acad::eOk) {
 			ASSERT(0);
 			layTbl->close();
 			return(es);
 		}
 		layTbl->close();
 		return(Acad::eOk);
 	}
 	
 	//********************************** upgrade to write ***************************
 	es = layTbl->upgradeOpen();
 	if (es != Acad::eOk) {
 		ASSERT(0);
 		layTbl->close();
 		return(es);
 	}
 	
 	//************************** make sure the name gets set ok *********************
 	AcDbLayerTableRecord* newRec = new AcDbLayerTableRecord;
 	es = newRec->setName(layerName);
 	if (es != Acad::eOk) {
 		delete newRec;
 		layTbl->close();
 		return(Acad::eInvalidInput);
 	}
 	
 	//****************** look up value for default linetype CONTINUOUS, *************
 	//                 AcDbLayerTableRecord doesn't set this automatically 
 	AcDbObjectId ltypeId;
 	if (ArxDbgUtils::nameToSymbolId(AcDbLinetypeTableRecord::desc(),
 		AcadString::continuous, ltypeId, db) == Acad::eOk) {
 		newRec->setLinetypeObjectId(ltypeId);
 	}
 
 	//****************************** adding the new record **************************
 	es = layTbl->add(idLayer,newRec);
 	if (es != Acad::eOk)
 		delete newRec;
 	else
 		newRec->close();
 
 	layTbl->close();
 
 	return(es);
 }
