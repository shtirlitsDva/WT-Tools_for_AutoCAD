#ifndef ARXDBG_ACADSTR_H
#define ARXDBG_ACADSTR_H

#pragma warning(disable: 4275)
#pragma warning(disable: 4251) 

//-----------------------------------------------------------------------------------
//	struct AcadVar 
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadVar {
	static const TCHAR* adserr;
	static const TCHAR* cmdecho;
	static const TCHAR* cvport;
	static const TCHAR* filedia;
	static const TCHAR* snapang;
	static const TCHAR* viewdir;
	static const TCHAR* viewsize;
};

//-----------------------------------------------------------------------------------
//	struct AcadCmd
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadCmd {
	static const TCHAR* linetype;
	static const TCHAR* menuload;
	static const TCHAR* menuunload;
	static const TCHAR* pan;
	static const TCHAR* zoom;
};

//-----------------------------------------------------------------------------------
//	struct AcadCmdOpt 
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadCmdOpt {
	static const TCHAR* load;
};

//-----------------------------------------------------------------------------------
//	struct AcadApi 
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadApi {
	static const TCHAR* colorDbox;
};

//-----------------------------------------------------------------------------------
//	struct AcadString 
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadString {
	static const TCHAR* continuous;		// needs to be translated
	static const TCHAR* bylayer;			// needs to be translated
	static const TCHAR* byblock;			// needs to be translated
	static const TCHAR* zero;			// default layer
	static const TCHAR* wcAll;			// wild card for all
	static const TCHAR* nullStr;			// ""
	static const TCHAR* openBrace;		// "{"
	static const TCHAR* endBrace;		// "}"
	static const TCHAR* standard;		// STANDARD
	static const TCHAR* varies1;			// *VARIES*
	static const TCHAR* none;			// None
	static const TCHAR* varies2;			// VARIES
};

//-----------------------------------------------------------------------------------
//	struct AcadFile 
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadFile {		
	static const TCHAR* ltypesStandard;	// "acad.lin"
	static const TCHAR* ltypesComplex;	// "ltypeshp.lin"
};

//-----------------------------------------------------------------------------------
// struct AcadUtilLinetypes
//-----------------------------------------------------------------------------------
struct AFX_EXT_CLASS AcadUtilLinetypes {
	static const TCHAR* continuous;
	static const TCHAR* dash;
	static const TCHAR* dashdot;
	static const TCHAR* dashed;
	static const TCHAR* dot;
	static const TCHAR* center2;
};

#endif	// ARXDBG_ACADSTR_H

