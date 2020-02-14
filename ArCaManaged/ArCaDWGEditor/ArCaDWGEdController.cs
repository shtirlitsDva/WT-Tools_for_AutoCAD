using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Autodesk.AutoCAD.DatabaseServices;
using ArCaManaged.Utils;
using Autodesk.AutoCAD.ApplicationServices;
using System.IO;
using System.Drawing;
using Autodesk.AutoCAD.Geometry;

namespace ArCaManaged.ArCaDWGEditor
{
    public static class ArCaDWGEdController
    {
        static List<Procedure> _ListProcedures;
        private static DWGCollectionModel _DWGCollection;
        private static bool _bIsInitialized = false;
        private static Bitmap _bmp;

        public static void Initialize()
        {
            _bIsInitialized = true;
            _DWGCollection = new DWGCollectionModel();
            _ListProcedures = new List<Procedure>();
        }

        public static void LoadProcedures()
        {
            _ListProcedures.Clear();

        #region Purge

            if (_DWGCollection.Purge)
            {
                _ListProcedures.Add(new Procedure(delegate(Database db, Transaction tr, DWGFileModel objDWG)
                {
                    // Create the list of objects to "purge"
                    ObjectIdCollection idsToPurge = ArCaUtils.GetObjIdNonGrafical(db);

                    // Call the Purge function to filter the list
                    db.Purge(idsToPurge);

                    if (idsToPurge.Count != 0)
                    {
                        // Erase each of the objects we've been
                        // allowed to
                        foreach (ObjectId id in idsToPurge)
                        {
                            DBObject obj = tr.GetObject(id, OpenMode.ForWrite);
                            obj.Erase();
                        }

                        //Was Changed
                        objDWG.IsChanged = true;
                    }
                }));
            }

        #endregion

        #region Lock View Ports

        if (_DWGCollection.LockViewPorts)
        {
            _ListProcedures.Add(new Procedure(delegate(Database db, Transaction tr, DWGFileModel objDWG)
            {
                //Block Table
                BlockTable btBlock = tr.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable;
                foreach (ObjectId btrId in btBlock)
                {
                    BlockTableRecord ltr = tr.GetObject(btrId, OpenMode.ForWrite) as BlockTableRecord;
                    foreach (ObjectId acObjId in ltr)
                    {
                        if (acObjId.ObjectClass.DxfName == "VIEWPORT")
                        {
                            Viewport vp = tr.GetObject(acObjId, OpenMode.ForWrite) as Viewport;
                            vp.Locked = true;
                            //Was Changed
                            objDWG.IsChanged = true;
                        }
                    }
                }
            }));
        }

        if (_DWGCollection.ZoomExtents)
        {
            _ListProcedures.Add(new Procedure(delegate(Database db, Transaction tr, DWGFileModel objDWG)
            {
                //Model
                ViewportTable vpt = tr.GetObject(db.ViewportTableId, OpenMode.ForWrite) as ViewportTable;
                foreach (ObjectId btrId in vpt)
                {
                    ViewportTableRecord vptrec = tr.GetObject(btrId, OpenMode.ForWrite) as ViewportTableRecord;
                    SetViewportToExtents(db, vptrec);
                }

                //Paper Spaces
                BlockTable btBlock = tr.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable;
                foreach (ObjectId btrId in btBlock)
                {
                    BlockTableRecord btr = tr.GetObject(btrId, OpenMode.ForWrite) as BlockTableRecord;

                    if (!btr.Name.StartsWith("*Paper_Space"))
                        continue;

                    //Retrieve paper space viewport (viewport with the lowest handle)
                    Viewport paperVp = null;
                    long lowestHandle = -1;
                    foreach (ObjectId acObjId in btr)
                    {
                        if (acObjId.ObjectClass.Name == "AcDbViewport")
                        {
                            Viewport vp = tr.GetObject(acObjId, OpenMode.ForRead) as Viewport;

                            if (lowestHandle < 0)
                            {
                                lowestHandle = vp.Handle.Value;
                                paperVp = vp;
                            }
                            else if (vp.Handle.Value < lowestHandle)
                            {
                                paperVp = vp;
                                lowestHandle = vp.Handle.Value;
                            }
                        }
                    }
                    if (paperVp == null)
                        return;
                    paperVp.UpgradeOpen();

                    double scrRatio = (paperVp.Width / paperVp.Height);

                    Matrix3d matWCS2DCS = Matrix3d.PlaneToWorld(paperVp.ViewDirection);

                    matWCS2DCS = Matrix3d.Displacement(paperVp.ViewTarget - Point3d.Origin) * matWCS2DCS;

                    matWCS2DCS = Matrix3d.Rotation(-paperVp.TwistAngle,
                                                    paperVp.ViewDirection,
                                                    paperVp.ViewTarget)
                                                    * matWCS2DCS;

                    matWCS2DCS = matWCS2DCS.Inverse();

                    Extents3d extents = GetExtents(db, btr);

                    extents.TransformBy(matWCS2DCS);

                    double width = (extents.MaxPoint.X - extents.MinPoint.X);

                    double height = (extents.MaxPoint.Y - extents.MinPoint.Y);

                    Point2d center = new Point2d((extents.MaxPoint.X + extents.MinPoint.X) * 0.5,
                                                 (extents.MaxPoint.Y + extents.MinPoint.Y) * 0.5);

                    if (width > (height * scrRatio))
                        height = width / scrRatio;

                    paperVp.ViewHeight = height;
                    paperVp.ViewCenter = center;

                    //Was Changed
                    objDWG.IsChanged = true;
                }

            }));
        }

        #endregion

        }

        static Extents3d GetExtents(Database db, BlockTableRecord btr)
        {
            double[] min = null;
            double[] max = null;
            bool FirstIsGone = false;
            //ObjectIdCollection objColl = new ObjectIdCollection();

            using (Transaction Tx = db.TransactionManager.StartTransaction())
            {
                foreach (ObjectId id in btr)
                {
                    try
                    {
                        Entity entity = Tx.GetObject(id, OpenMode.ForRead) as Entity;

                        //Check the first elemente
                        if (!FirstIsGone)
                        {
                            FirstIsGone = true;
                            Viewport vp = entity as Viewport;
                            //if is a Viewport, not process this elemente
                            if (vp != null)
                                continue;
                        }

                        if (min == null)
                        {
                            min = entity.GeometricExtents.MinPoint.ToArray();
                            max = entity.GeometricExtents.MaxPoint.ToArray();
                        }

                        if (entity.GeometricExtents.MinPoint.X < min[0])
                            min[0] = entity.GeometricExtents.MinPoint.X;
                        if (entity.GeometricExtents.MinPoint.Y < min[1])
                            min[1] = entity.GeometricExtents.MinPoint.Y;
                        if (entity.GeometricExtents.MinPoint.Z < min[2])
                            min[2] = entity.GeometricExtents.MinPoint.Z;

                        if (entity.GeometricExtents.MaxPoint.X > max[0])
                            max[0] = entity.GeometricExtents.MaxPoint.X;
                        if (entity.GeometricExtents.MaxPoint.Y > max[1])
                            max[1] = entity.GeometricExtents.MaxPoint.Y;
                        if (entity.GeometricExtents.MaxPoint.Z > max[2])
                            max[2] = entity.GeometricExtents.MaxPoint.Z;

                        //objColl.Add(id);
                    }
                    catch (System.Exception ex)
                    {
                    }
                }

                //_bmp = Utils.ArCaUtils.GetBitmap(db, objColl);
                return new Extents3d(new Point3d(min), new Point3d(max));
            }
        }

        public static void SetViewportToExtents(Database db, ViewportTableRecord viewportTableRec)
        {
            try
            {
                //lets update the database extents first
                //true gives the best fit but will take time
                db.UpdateExt(false); //Does not work corectly

                //get the screen aspect ratio to calculate the height and width
                double scrRatio = (viewportTableRec.Width / viewportTableRec.Height);

                //prepare Matrix for DCS to WCS transformation
                Matrix3d matWCS2DCS = Matrix3d.PlaneToWorld(viewportTableRec.ViewDirection);

                //for DCS target point is the origin
                matWCS2DCS = Matrix3d.Displacement(viewportTableRec.Target - Point3d.Origin) * matWCS2DCS;

                //WCS Xaxis is twisted by twist angle
                matWCS2DCS = Matrix3d.Rotation(-viewportTableRec.ViewTwist,
                                                viewportTableRec.ViewDirection,
                                                viewportTableRec.Target)
                                                * matWCS2DCS;

                matWCS2DCS = matWCS2DCS.Inverse();

                //tranform the extents to the DCS defined by the viewdir
                Extents3d extents = new Extents3d(db.Extmin, db.Extmax);
                extents.TransformBy(matWCS2DCS);

                //width of the extents in current view
                double width = (extents.MaxPoint.X - extents.MinPoint.X);

                //height of the extents in current view
                double height = (extents.MaxPoint.Y - extents.MinPoint.Y);

                //get the view center point
                Point2d center = new Point2d((extents.MaxPoint.X + extents.MinPoint.X) * 0.5,
                                             (extents.MaxPoint.Y + extents.MinPoint.Y) * 0.5);

                //check if the width 'fits' in current window
                //if not then get the new height as per the viewports aspect ratio
                if (width > (height * scrRatio))
                    height = width / scrRatio;

                viewportTableRec.Height = height;
                viewportTableRec.Width = height * scrRatio;
                viewportTableRec.CenterPoint = center;
            }
            catch (Exception ex)
            {
            }            
        }

        public static void DoProcedures(DWGFileModel objDWG)
        {

            //Restart change status
            objDWG.IsChanged = false;
            
            Database db = new Database(false, true);
            using (db)
            {
                try
                {
                    objDWG.MessageProcess = "Open file: " + objDWG.Name;

                    db.ReadDwgFile(objDWG.FilePath, FileShare.ReadWrite, false, "");
                    _bmp = db.ThumbnailBitmap;
                    objDWG.MessageProcess = "Processing file: " + objDWG.Name;
                                            
                    using (Transaction trans = db.TransactionManager.StartTransaction())
                    {
                        foreach (Procedure objProcedure in _ListProcedures)
                            objProcedure.Run(db,trans, objDWG);

                        trans.Commit();
                    }
                    
                    if (objDWG.IsChanged)
                    {
                        db.ThumbnailBitmap = _bmp;
                        objDWG.MessageProcess = "Saving file: " + objDWG.Name;
                        db.SaveAs(objDWG.FilePath, DwgVersion.Current);                            
                    }
                }
                catch (System.Exception ex)
                {
                    objDWG.MessageProcess = string.Format("Problem processing file: {0} - \"{1}\"", objDWG.Name, ex.Message);
                }
            }
        }

        public static DWGCollectionModel DWGCollection
        {
            get { return ArCaDWGEdController._DWGCollection; }
            set { ArCaDWGEdController._DWGCollection = value; }
        }

        public static bool IsInitialized
        {
            get { return ArCaDWGEdController._bIsInitialized; }
            set { ArCaDWGEdController._bIsInitialized = value; }
        }
    }

    //Procedure Strategy
    internal class Procedure
    {
        public delegate void CommandProcedureRunMethod(Database db, Transaction tr, DWGFileModel objDWG);
        private CommandProcedureRunMethod _evRun;

        public Procedure(CommandProcedureRunMethod evRun)
        {
            _evRun = evRun;
        }

        public void Run(Database db, Transaction tr, DWGFileModel objDWG)
        {
            _evRun.Invoke(db, tr, objDWG);
        }
    }
}
