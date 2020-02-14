// (C) Copyright 2005-2007 by Autodesk, Inc. 
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
#include "StdAfx.h"
#include "ArCaWrpRampa.h"

//-----------------------------------------------------------------------------
//- Template class that wraps GCHandle from mscorlib.dll
#include <gcroot.h>

//-----------------------------------------------------------------------------
//- autodesk interop header, to aid converting between unmanaged ObjectARX and managed ObjectARX.NET
#include "mgdinterop.h"

//-----------------------------------------------------------------------------
ArCa::Wrapper::ArCaWrpRampa::ArCaWrpRampa () 
	: Autodesk::AutoCAD::DatabaseServices::Entity ((System::IntPtr)new ArCaRampa (), true)
{
}

//-----------------------------------------------------------------------------
ArCa::Wrapper::ArCaWrpRampa::ArCaWrpRampa (System::IntPtr unmanagedPointer, bool bAutoDelete)
	: Autodesk::AutoCAD::DatabaseServices::Entity (unmanagedPointer, bAutoDelete)
{
}

void ArCa::Wrapper::ArCaWrpRampa::set_p3dStart( Point3d pt )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__p3dStart(GETPOINT3D(pt)));
}

Point3d ArCa::Wrapper::ArCaWrpRampa::get_p3dStart()
{	
	return ToPoint3d( GetImpObj()->get__p3dStart() );
}

void ArCa::Wrapper::ArCaWrpRampa::set_p3dEnd( Point3d pt )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__p3dEnd(GETPOINT3D(pt)));
}

Point3d ArCa::Wrapper::ArCaWrpRampa::get_p3dEnd()
{	
	return ToPoint3d( GetImpObj()->get__p3dEnd() );
}