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
#include "ArCaWrpDynamicArea.h"

//-----------------------------------------------------------------------------
//- Template class that wraps GCHandle from mscorlib.dll
#include <gcroot.h>

//-----------------------------------------------------------------------------
//- autodesk interop header, to aid converting between unmanaged ObjectARX and managed ObjectARX.NET
#include "mgdinterop.h"

//-----------------------------------------------------------------------------
ArCa::Wrapper::ArCaWrpDynamicArea::ArCaWrpDynamicArea () 
	: Autodesk::AutoCAD::DatabaseServices::Entity ((System::IntPtr)new ArCaDynamicArea (), true)
{
}

//-----------------------------------------------------------------------------
ArCa::Wrapper::ArCaWrpDynamicArea::ArCaWrpDynamicArea (System::IntPtr unmanagedPointer, bool bAutoDelete)
	: Autodesk::AutoCAD::DatabaseServices::Entity (unmanagedPointer, bAutoDelete)
{
}

void ArCa::Wrapper::ArCaWrpDynamicArea::put_PolyLPoint3D( Point3d pt )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put_PolyLPoint3D(GETPOINT3D(pt)));
}

void ArCa::Wrapper::ArCaWrpDynamicArea::set_p3dArea()
{
	GetImpObj()->put_p3dArea();
}

Point3d ArCa::Wrapper::ArCaWrpDynamicArea::get_FirstPolyLPoint3D()
{	
	return ToPoint3d( GetImpObj()->get_FirstPolyLPoint3D() );
}

void ArCa::Wrapper::ArCaWrpDynamicArea::move_LastPolyLPoint3D( Point3d pt )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->move_LastPolyLPoint3D(GETPOINT3D(pt)));
}

void ArCa::Wrapper::ArCaWrpDynamicArea::set_del_PolyLPoint3D_movel()
{
	GetImpObj()->del_PolyLPoint3D_movel();
}

void ArCa::Wrapper::ArCaWrpDynamicArea::switch_IsFinished()
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->switch_IsFinished());	
}