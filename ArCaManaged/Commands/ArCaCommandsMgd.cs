using System;
using System.IO;
using System.Collections.Generic;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using ArCa;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Windows;
using ArCa.Wrapper;
using System.Collections.ObjectModel;
using ArCaManaged.Reference_Manager.Model;
using ArCaManaged.Reference_Manager.Controller;
using ArCaManaged.Reference_Manager.View;
using ArCaManaged.ArCaDWGEditor;
using ArCaManaged.Utils;
using System.Runtime.InteropServices;

[assembly: CommandClass(typeof(ArCaManaged.Commands.ArCaCommands))]

namespace ArCaManaged.Commands
{
    /// <summary>
    /// Summary description for ArCaCommands.
    /// </summary>
    public class ArCaCommands
    {
        #region Attributes

        static PointMonitorStrategy _PointMonitorStrategy;
        
        public static PaletteSet _PalletRefMgr = null;
        public static ArCaPaletteMain _formRefMgr = null;
        private static string _sLastBlock = string.Empty;

        #endregion

        public ArCaCommands()
        {
        }
        private static void ps_SizeChanged(object sender, PaletteSetSizeEventArgs e)
        {
            System.Windows.Size size = _PalletRefMgr.PaletteSize;
            _formRefMgr.Width = size.Width;
            _formRefMgr.Height = size.Height;
        }
        // Define Command "AsdkCmd1"
        [CommandMethod("RefM")]
        static public void RefMgrPallet() // This method can have any name
        {
            // check to see if this command has not been run before
            if (_PalletRefMgr == null)
            {
                // if not, then create our windows - first the paletteset container window
                // we won't generate a Guid for the paletteset as that would then require
                // demand loading
                _PalletRefMgr = new PaletteSet("Reference Manager");
                // next the WPF UserControl
                _formRefMgr = new ArCaPaletteMain();

                // now add the user control to the paletteset
                _PalletRefMgr.AddVisual("Reference Manager Palette", _formRefMgr);
                _PalletRefMgr.Size = new System.Drawing.Size(345,300);
                _PalletRefMgr.SizeChanged += new PaletteSetSizeEventHandler(ps_SizeChanged);
                _PalletRefMgr.KeepFocus = false;

                ArCaRefMgrController.Initialize();
                _formRefMgr.GridMain.DataContext = ArCaRefMgrController.ObjMain;
            }

            // finally, we need to show the paletteset
            // this also serves as a way to redisplay the paletteset if closed
            // by the user
            _PalletRefMgr.Visible = true;

        }
        [CommandMethod("RefI")]
        static public void CmdInset() // This method can have any name
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

            PromptStringOptions optName = new PromptStringOptions("\nEnter the name of the block: ");
            optName.AllowSpaces = true;

            if (_sLastBlock == string.Empty)
            {
                optName.UseDefaultValue = true;
                optName.DefaultValue = _sLastBlock;
            }

            //Loop
            do
            {
                //Esk the user by block name
                PromptResult ProNameRes = ed.GetString(optName);

                if (ProNameRes.Status == PromptStatus.Cancel)
                    break;//Command canceled, go out of loop
                else
                {
                    BlockModel objBlock = ArCaRefMgrController.GetBlockModelByName(ProNameRes.StringResult);
                    if (objBlock != null)
                    {
                        //Block found, insert.
                        InsestBlock(objBlock);
                        //Save the name of inserted block
                        _sLastBlock = ProNameRes.StringResult;
                        //Go out of Loop
                        break;
                    }
                    else
                        ed.WriteMessage("\nNot foud any block with name \"" + ProNameRes.StringResult + "\".");
                }
            }while(true);
        }

        //Enum to work in PointMonitor
        enum BlockInsetions { Insert, Rotate, Scale, Mirror, Loop, Matrix, Done };

        //Block Insertor
        static public void InsestBlock(BlockModel objBlockModel)
        {
            Database db = Application.DocumentManager.MdiActiveDocument.Database;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

            //InsertionState
            BlockInsetions insState = BlockInsetions.Insert;

            //Variables
            Point3d p3dInsetion = new Point3d();
            Point3d p3dMatrixCorner = new Point3d();
            

            //Get objId
            ObjectId objId = ArCaRefMgrController.GetBlockId(objBlockModel);
            //Create BlockReference
            BlockReference blockRef = new BlockReference(new Point3d(), objId);
            //Get BlockPropertyModel
            BlockPropModel objBloPro = objBlockModel.LstBlockProp[0];
            //Set properties in BlockRefernce
            blockRef.Rotation = ArCaUtils.DegreeToRadian(objBloPro.Angle);
            blockRef.ScaleFactors = new Scale3d(objBloPro.SclX, objBloPro.SclY, objBloPro.SclZ);

            //Mirror Settings
            bool bTop = true;
            bool bLeft = true;

            //Strategy for Preview in PointMonitor
            _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
            {
                try
                {
                    switch(insState)
                    {
                    case BlockInsetions.Insert:
                        blockRef.Position = e.Context.ComputedPoint;
                        e.Context.DrawContext.Geometry.Draw(blockRef);
                    break;
                    case BlockInsetions.Rotate:                      
                        blockRef.Rotation = Vector3d.XAxis.GetAngleTo(p3dInsetion.GetVectorTo(e.Context.ComputedPoint), Vector3d.ZAxis);
                        e.Context.DrawContext.Geometry.Draw(blockRef);
                    break;
                    case BlockInsetions.Scale:
                        blockRef.ScaleFactors = new Scale3d(p3dInsetion.DistanceTo(e.Context.ComputedPoint));
                        e.Context.DrawContext.Geometry.Draw(blockRef);
                    break;
                    case BlockInsetions.Mirror:

                    if (e.Context.ComputedPoint.X < p3dInsetion.X && bLeft)
                    {
                        blockRef.ScaleFactors = new Scale3d(-(blockRef.ScaleFactors.X), blockRef.ScaleFactors.Y, blockRef.ScaleFactors.Z);
                        bLeft = false;
                    }
                    else if (e.Context.ComputedPoint.X > p3dInsetion.X && !bLeft)
                    {
                        blockRef.ScaleFactors = new Scale3d(-(blockRef.ScaleFactors.X), blockRef.ScaleFactors.Y, blockRef.ScaleFactors.Z);
                        bLeft = true;
                    }
                    if (e.Context.ComputedPoint.Y < p3dInsetion.Y && bTop)
                    {
                        blockRef.ScaleFactors = new Scale3d(blockRef.ScaleFactors.X, -(blockRef.ScaleFactors.Y), blockRef.ScaleFactors.Z);
                        bTop = false;
                    }
                    else if (e.Context.ComputedPoint.Y > p3dInsetion.Y && !bTop)
                    {
                        blockRef.ScaleFactors = new Scale3d(blockRef.ScaleFactors.X, -(blockRef.ScaleFactors.Y), blockRef.ScaleFactors.Z);
                        bTop = true;
                    }
                    e.Context.DrawContext.Geometry.Draw(blockRef);
                    break;

                    case BlockInsetions.Matrix:
                    //Get pt array
                    Point3dCollection pt3DColl = Utils.ArCaUtils.GetPtArray(p3dInsetion, e.Context.ComputedPoint, objBloPro.MatrixHeight, objBloPro.MatrixWidth);
                    double iLimitator = objBloPro.MatrixMaxQuant;
                    foreach (Point3d pt3D in pt3DColl)
                    {
                        //Insert Block
                        blockRef.Position = pt3D;
                        e.Context.DrawContext.Geometry.Draw(blockRef);
                        //Limitator to evoid chash in machine
                        iLimitator--;
                        if (iLimitator < 1)
                            break;
                    }                            
                    break;
                    }                    
                }
                catch (System.Exception ex)
                {
                }
            });

            //add pointMonitor
            ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

            //Looping for Insertion in Loop
            do
            {
                //***Insertion point***
                insState = BlockInsetions.Insert;

                PromptPointOptions opcPt = new PromptPointOptions("\nClick on the insertion point: ");
                opcPt.AllowNone = false;
                PromptPointResult ProPtRes = ed.GetPoint(opcPt);

                if (ProPtRes.Status != PromptStatus.OK)
                    //Command Canceled
                    break;

                p3dInsetion = ProPtRes.Value;
                p3dMatrixCorner = ProPtRes.Value;

                //***ROTATE***
                if (objBloPro.Rotate)
                {
                    insState = BlockInsetions.Rotate;

                    PromptAngleOptions opcAng = new PromptAngleOptions("\nDefine the rotation angle: ");
                    opcAng.AllowNone = true;
                    opcAng.UseBasePoint = true;
                    opcAng.BasePoint = p3dInsetion;
                    opcAng.DefaultValue = 0;
                    PromptDoubleResult ProAngRes = ed.GetAngle(opcAng);

                    if (ProAngRes.Status == PromptStatus.Cancel)
                        //Command Canceled, exit to "Loopin"
                        break;
                }

                //***SCALE***
                if (objBloPro.Scale)
                {
                    insState = BlockInsetions.Scale;

                    PromptDistanceOptions opcScl = new PromptDistanceOptions("\nDefine the scale factor: ");
                    opcScl.AllowNone = true;
                    opcScl.UseBasePoint = true;
                    opcScl.BasePoint = p3dInsetion;
                    opcScl.DefaultValue = 1;
                    PromptDoubleResult ProSclRes = ed.GetDistance(opcScl);

                    if (ProSclRes.Status == PromptStatus.Cancel)
                        //Command Canceled, exit to "Loopin"
                        break;
                }

                //***MIRROR***
                if (objBloPro.Mirror)
                {
                    insState = BlockInsetions.Mirror;

                    PromptPointOptions opcMatCor = new PromptPointOptions("\nClick on the point that define the mirror direction: ");
                    opcMatCor.AllowNone = false;
                    opcMatCor.UseBasePoint = true;
                    opcMatCor.BasePoint = p3dInsetion;
                    PromptPointResult ProMatCorRes = ed.GetPoint(opcMatCor);

                    if (ProMatCorRes.Status != PromptStatus.OK)
                        //Command Canceled, exit to "Loopin"
                        break;
                }

                //***MATRIX***
                if (objBloPro.Matrix)
                {
                    insState = BlockInsetions.Matrix;

                    PromptPointOptions opcMatCor = new PromptPointOptions("\nClick on the corner point of the matrix: ");
                    opcMatCor.AllowNone = false;
                    PromptPointResult ProMatCorRes = ed.GetPoint(opcMatCor);

                    if (ProMatCorRes.Status != PromptStatus.OK)
                        //Command Canceled, exit to "Loopin"
                        break;

                    p3dMatrixCorner = ProMatCorRes.Value;
                }

                //***Insertion of Block***
                //Get pt array
                Point3dCollection pt3DColl = Utils.ArCaUtils.GetPtArray(p3dInsetion, p3dMatrixCorner, objBloPro.MatrixHeight, objBloPro.MatrixWidth);                
                double iLimitator = objBloPro.MatrixMaxQuant;
                foreach (Point3d pt3D in pt3DColl)
                {
                    //Set Position
                    blockRef.Position = pt3D;
                    //We need clone the block reference to obtain a block wiht other objectID in each iteration
                    BlockReference blockRefClone = blockRef.Clone() as BlockReference;
                    //Insert Block
                    Utils.ArCaUtils.InserBlock(db, objBloPro.Explode, blockRefClone);
                    //Limitator to evoid chash in machine
                    iLimitator--;
                    if (iLimitator < 1)
                        break;
                }                              
            } while (objBloPro.Loop);

            //remove pointMonitor
            ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);
        }


        public static ObjectId GetBlockId(string sBlockName)
        {
            using (Database dbLocal = Application.DocumentManager.MdiActiveDocument.Database)
            {
                using (Transaction trLocal = dbLocal.TransactionManager.StartTransaction())
                {
                    //abrir tabela de blocos
                    BlockTable bt = trLocal.GetObject(dbLocal.BlockTableId, OpenMode.ForRead) as BlockTable;

                    //verificar se o bloco já existe
                    if (bt.Has(sBlockName))
                    {
                        trLocal.Commit();
                        return bt[sBlockName];
                    }
                    else
                    {
                        //Não existe, retorna null
                        trLocal.Commit();
                        return ObjectId.Null;
                    }
                }
            }
        }

        [CommandMethod("insBlock")]
        static public void CmdInsertBlock()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            Database db = Application.DocumentManager.MdiActiveDocument.Database;

            PromptStringOptions optName = new PromptStringOptions("\nEnter the name of the block: ");
            optName.AllowSpaces = true;

            if (_sLastBlock == string.Empty)
            {
                optName.UseDefaultValue = true;
                optName.DefaultValue = _sLastBlock;
            }

            //Loop
            do
            {
                //Esk the user by block name
                PromptResult ProNameRes = ed.GetString(optName);

                if (ProNameRes.Status == PromptStatus.Cancel)
                    break;//Command canceled, go out of loop
                else
                {
                    //Get objId
                    ObjectId objId = GetBlockId(ProNameRes.StringResult);

                    if(objId.IsValid)
                    {
                        //Create BlockReference
                        BlockReference blockRef = new BlockReference(new Point3d(), objId);

                        //Strategy for Preview in PointMonitor
                        _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
                        {
                            try
                            {
                                //Refresh gost block.
                                blockRef.Position = e.Context.ComputedPoint;
                                e.Context.DrawContext.Geometry.Draw(blockRef);
                            }
                            catch (System.Exception ex)
                            {
                            }
                        });

                        
                        //add pointMonitor
                        ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

                        //Get Second point
                        PromptPointOptions opcoesPonto = new PromptPointOptions("\nClick on the place where block will be inserted: ");
                        opcoesPonto.AllowNone = false;

                        PromptPointResult ProPtRes = ed.GetPoint(opcoesPonto);

                        //remove pointMonitor
                        ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);

                        //Definitive insertion
                        switch (ProPtRes.Status)
                        {
                            case PromptStatus.OK:
                                //Create Grating
                                using (Transaction trans = db.TransactionManager.StartTransaction())
                                {
                                    try
                                    {
                                        //Set Position
                                        blockRef.Position = ProPtRes.Value;
                                        //We need clone the block reference to obtain a block wiht other objectID in each iteration
                                        BlockReference blockRefClone = blockRef.Clone() as BlockReference;                                        
                                        //Get block table records
                                        BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;
                                        //Insert Block
                                        currSpace.AppendEntity(blockRefClone);
                                        trans.AddNewlyCreatedDBObject(blockRefClone, true);
                                        trans.Commit();
                                    }
                                    catch (System.Exception ex)
                                    {
                                        trans.Abort();
                                    }
                                }
                                break;
                            case PromptStatus.Cancel:
                                //Command Canceled
                                break;
                        }

                        //Save the name of inserted block
                        _sLastBlock = ProNameRes.StringResult;

                        //Go out of Loop
                        break;
                    }
                    else
                        ed.WriteMessage("\nNot foud any block with name \"" + ProNameRes.StringResult + "\".");
                }
            }while(true);           
        }

        //Code to use .dbx functions.
        // The entry point for the method can be found using depends.exe.
        [DllImport(@"D:\Vitor Box\ArCaApplication\x64\Debug\ArCaArCaObjects.dbx", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl, EntryPoint = "?ReloadThumbnail@ArxDbgUtils@@SAXXZ")]
        private static extern void ReloadThumbnail(string name);
 
        [CommandMethod("relo")]
        static public void TestMethod()
        {
            ReloadThumbnail("MyBlock");
        }

        [CommandMethod("dwged")]
        static public void CmdDWGEditor()
        {
            //If DWG Collection was never instantiated
            if (!ArCaDWGEdController.IsInitialized)
                ArCaDWGEdController.Initialize();            

            ArCaDWGEdView dwgEd = new ArCaDWGEdView();
            Autodesk.AutoCAD.ApplicationServices.Application.ShowModalWindow(dwgEd);
        }

        [CommandMethod("Quota")]
        static public void CmdCota()
        {
            Database db = Application.DocumentManager.MdiActiveDocument.Database;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

            //Set Type of Quota
            PromptKeywordOptions optionType = new PromptKeywordOptions("\nSelect the type of quota: ");
            optionType.AllowNone = false;
            optionType.Keywords.Add("Arrow");
            optionType.Keywords.Add("Leader");
            optionType.Keywords.Add("Plane");
            optionType.Keywords.Default = "Arrow";
            PromptResult ProTypeRes = ed.GetKeywords(optionType);
            int iType = 0;
            switch (ProTypeRes.StringResult)
            {
                case "Arrow":
                    iType = 0;
                    break;
                case "Leader":
                    iType = 1;
                    break;
                case "Plane":
                    iType = 2;
                    break;
            }

            //Strategy for Preview in PointMonitor
            _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
            {
                try
                {
                    ArCaWrpCota objCota = new ArCaWrpCota();
                    objCota.Type = iType;
                    objCota.p3dInsertion = e.Context.ComputedPoint;
                    e.Context.DrawContext.Geometry.Draw(objCota);
                }
                catch (System.Exception ex)
                {
                }
            });

            //add pointMonitor
            ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

            //Get Second point
            PromptPointOptions opcoesPonto = new PromptPointOptions("\nClick on the local where the quota will be inserted: ");
            opcoesPonto.AllowNone = false;

            PromptPointResult ProPtRes = ed.GetPoint(opcoesPonto);

            //remove pointMonitor
            ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);

            switch (ProPtRes.Status)
            {
                case PromptStatus.OK:
                    //Create Grating
                    using (Transaction trans = db.TransactionManager.StartTransaction())
                    {
                        try
                        {
                            ArCaWrpCota objCota = new ArCaWrpCota();
                            objCota.Type = iType;
                            objCota.p3dInsertion = ProPtRes.Value;
                            BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;
                            currSpace.AppendEntity(objCota);
                            trans.AddNewlyCreatedDBObject(objCota, true);
                            trans.Commit();
                        }
                        catch (System.Exception ex)
                        {
                            trans.Abort();
                        }
                    }
                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
        }

        [CommandMethod("Grating")]
        static public void CmdGrating()
        {
            Database db = Application.DocumentManager.MdiActiveDocument.Database;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

            //Get First point
            Point3d Pt1 = ed.GetPoint("\nClick on initial corner of grating: ").Value;

            //Strategy for Preview in PointMonitor
            _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
            {
                try
                {
                    ArCaWrpGrating objGrating = new ArCaWrpGrating();
                    objGrating.p3dLowerLeft = Pt1;
                    objGrating.p3dTopRight = e.Context.ComputedPoint;
                    e.Context.DrawContext.Geometry.Draw(objGrating);
                }
                catch (System.Exception ex)
                {
                }
            });

            //add pointMonitor
            ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

            //Get Second point
            PromptPointOptions opcoesPonto = new PromptPointOptions("\nClick on final corner of grating: ");
            opcoesPonto.AllowNone = false;

            PromptPointResult ProPtRes = ed.GetPoint(opcoesPonto);

            //remove pointMonitor
            ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);

            switch (ProPtRes.Status)
            {
                case PromptStatus.OK:
                    //Create Grating
                    using (Transaction trans = db.TransactionManager.StartTransaction())
                    {
                        try
                        {
                            ArCaWrpGrating objGrating = new ArCaWrpGrating();
                            objGrating.p3dLowerLeft = Pt1;
                            objGrating.p3dTopRight = ProPtRes.Value;
                            BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;
                            currSpace.AppendEntity(objGrating);
                            trans.AddNewlyCreatedDBObject(objGrating, true);
                            trans.Commit();
                        }
                        catch (System.Exception ex)
                        {
                            trans.Abort();
                        }
                    }
                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
        }

        [CommandMethod("Ramp")]
        static public void CmdRampa()
        {
            Database db = Application.DocumentManager.MdiActiveDocument.Database;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

            //Get First point
            Point3d Pt1 = ed.GetPoint("\nClick on initial point of the ramp (Lower side): ").Value;

            //Strategy for Preview in PointMonitor
            _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
            {
                try
                {
                    ArCaWrpRampa objRampa = new ArCaWrpRampa();
                    objRampa.p3dStart = Pt1;
                    objRampa.p3dEnd = e.Context.ComputedPoint;
                    e.Context.DrawContext.Geometry.Draw(objRampa);
                }
                catch (System.Exception ex)
                {
                }
            });

            //add pointMonitor
            ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

            //Get Second point
            PromptPointOptions opcoesPonto = new PromptPointOptions("\nClick on final point of the ramp (Higher side): ");
            opcoesPonto.AllowNone = false;
            opcoesPonto.BasePoint = Pt1;
            PromptPointResult ProPtRes = ed.GetPoint(opcoesPonto);

            //remove pointMonitor
            ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);

            switch (ProPtRes.Status)
            {
                case PromptStatus.OK:
                    //Crate Rampa
                    using (Transaction trans = db.TransactionManager.StartTransaction())
                    {
                        try
                        {
                            BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;

                            ArCaWrpRampa objRampa = new ArCaWrpRampa();
                            objRampa.p3dStart = Pt1;
                            objRampa.p3dEnd = ProPtRes.Value;

                            currSpace.AppendEntity(objRampa);
                            trans.AddNewlyCreatedDBObject(objRampa, true);
                            trans.Commit();
                        }
                        catch (System.Exception ex)
                        {
                            trans.Abort();
                        }
                    }
                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
        }
        [CommandMethod("Permeable")]
        static public void CmdDynarea() // This method can have any name
        {
            Database db = Application.DocumentManager.MdiActiveDocument.Database;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            List<Point3d> lstP3d = new List<Point3d>();
            PromptPointResult ProPtRes = ed.GetPoint("\nSelect the first point: ");

            //If nothing point, return
            if (ProPtRes.Status != PromptStatus.OK)
                return;

            lstP3d.Add(ProPtRes.Value);

            //Strategy for Preview in PointMonitor
            _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
            {
                try
                {
                    ArCaWrpDynamicArea objDynArea = new ArCaWrpDynamicArea();
                    foreach (Point3d pt in lstP3d)
                        objDynArea.put_PolyLPoint3D(pt);

                    objDynArea.put_PolyLPoint3D(e.Context.ComputedPoint);
                    e.Context.DrawContext.Geometry.Dispose();
                    e.Context.DrawContext.Geometry.Draw(objDynArea);
                }
                catch (System.Exception ex)
                {
                }
            });

            //add pointMonitor
            ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

            PromptPointOptions opcoesPonto = new PromptPointOptions("\nSelect the second point: ");
            opcoesPonto.AllowNone = true;

            Boolean doLoop = true;
            while (doLoop)
            {
                ProPtRes = ed.GetPoint(opcoesPonto);
                switch (ProPtRes.Status)
                {
                    case PromptStatus.OK:
                        lstP3d.Add(ProPtRes.Value);
                        break;
                    case PromptStatus.None:
                        //Create DynamicArea
                        using (Transaction trans = db.TransactionManager.StartTransaction())
                        {
                            try
                            {
                                ArCaWrpDynamicArea objDynArea = new ArCaWrpDynamicArea();
                                foreach (Point3d pt in lstP3d)
                                    objDynArea.put_PolyLPoint3D(pt);

                                objDynArea.switch_IsFinished();

                                BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;
                                currSpace.AppendEntity(objDynArea);
                                trans.AddNewlyCreatedDBObject(objDynArea, true);
                                trans.Commit();
                            }
                            catch (System.Exception ex)
                            {
                                trans.Abort();
                            }
                        }
                        doLoop = false;
                        break;
                    case PromptStatus.Cancel:
                        //Command Canceled
                        doLoop = false;
                        break;
                }
            }

            //remove pointMonitor
            ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);

        }

        //PointMonitor Event
        static void PointMonitor(object sender, PointMonitorEventArgs e)
        {
            //Do Strategy
            _PointMonitorStrategy.Run(sender, e);
        }

        //PointMonitor Strategy
        internal class PointMonitorStrategy
        {
            public delegate void CommandProcedureRunMethod(object sender, PointMonitorEventArgs e);
            private CommandProcedureRunMethod _evRun;

            public PointMonitorStrategy(CommandProcedureRunMethod evRun)
            {
                _evRun = evRun;
            }

            public void Run(object sender, PointMonitorEventArgs e)
            {
                _evRun.Invoke(sender, e);
            }
        }
    }
}