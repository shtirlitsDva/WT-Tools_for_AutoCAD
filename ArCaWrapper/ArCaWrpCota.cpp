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
#include "ArCaWrpCota.h"

//-----------------------------------------------------------------------------
//- Template class that wraps GCHandle from mscorlib.dll
#include <gcroot.h>

//-----------------------------------------------------------------------------
//- autodesk interop header, to aid converting between unmanaged ObjectARX and managed ObjectARX.NET
#include "mgdinterop.h"

//-----------------------------------------------------------------------------
ArCa::Wrapper::ArCaWrpCota::ArCaWrpCota () 
	: Autodesk::AutoCAD::DatabaseServices::Entity ((System::IntPtr)new ArCaCota (), true)
{
}

//-----------------------------------------------------------------------------
ArCa::Wrapper::ArCaWrpCota::ArCaWrpCota (System::IntPtr unmanagedPointer, bool bAutoDelete)
	: Autodesk::AutoCAD::DatabaseServices::Entity (unmanagedPointer, bAutoDelete)
{
}

void ArCa::Wrapper::ArCaWrpCota::set_p3dInsertion( Point3d pt )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__p3dInsertion(GETPOINT3D(pt)));
}

Point3d ArCa::Wrapper::ArCaWrpCota::get_p3dInsertion()
{	
	return ToPoint3d( GetImpObj()->get__p3dInsertion() );
}

void ArCa::Wrapper::ArCaWrpCota::set_dCota( double val )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__dCota(val));
}

double ArCa::Wrapper::ArCaWrpCota::get_dCota()
{	
	return GetImpObj()->get__dCota();
}

void ArCa::Wrapper::ArCaWrpCota::set_dScaleFactor( double val )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__dScaleFactor(val));
}

double ArCa::Wrapper::ArCaWrpCota::get_dScaleFactor()
{	
	return GetImpObj()->get__dScaleFactor();
}

void ArCa::Wrapper::ArCaWrpCota::set_bDynamicQuote( bool val )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__bDynamicQuote(val));
}

bool ArCa::Wrapper::ArCaWrpCota::get_bDynamicQuote()
{	
	return GetImpObj()->get__bDynamicQuote();
}

void ArCa::Wrapper::ArCaWrpCota::set_Type( int iType )
{
	Autodesk::AutoCAD::Runtime::Interop::Check(GetImpObj()->put__kTypeByString(iType));
}