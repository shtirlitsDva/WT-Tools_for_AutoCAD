using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;
using System.IO;
using System.Windows.Media.Imaging;
using System.Drawing.Imaging;
using System.Drawing;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.GraphicsSystem;
using Autodesk.AutoCAD.Geometry;
using System;
using Autodesk.AutoCAD.ApplicationServices;

namespace ArCaManaged.Utils
{
    public static class ArCaUtils
    {
        public static double DegreeToRadian(double angle)
        {
            return Math.PI * angle / 180.0;
        }

        public static double RadianToDegree(double radians)
        {
            return radians * 180.0 / Math.PI;
        }

        public static void InserBlock(Database db, bool bExploded, BlockReference blockRefToInsert)
        {
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                try
                {
                    Document Doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                    using (DocumentLock dl = Doc.LockDocument())
                    {
                        BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;
                        if (bExploded)
                        {
                            DBObjectCollection objColl = new DBObjectCollection();
                            blockRefToInsert.Explode(objColl);
                            foreach (DBObject obj in objColl)
                            {
                                currSpace.AppendEntity(obj as Entity);
                                trans.AddNewlyCreatedDBObject(obj, true);
                            }
                        }
                        else
                        {
                            currSpace.AppendEntity(blockRefToInsert);
                            trans.AddNewlyCreatedDBObject(blockRefToInsert, true);
                        }
                        trans.Commit();
                    }
                }
                catch (System.Exception ex)
                {
                    trans.Abort();
                }
            }
        }

        static public Point3dCollection GetPtArray(Point3d ptInit, Point3d ptEnd, double dHeight, double dWidth)
        {
            Point3dCollection pt3DColl = new Point3dCollection();
            bool DoVert = true;
            bool DoHoriz = true;
            bool ToLeft = true;
            bool ToTop = true;
            if (ptInit.X > ptEnd.X)
                ToLeft = false;
            if (ptInit.Y > ptEnd.Y)
                ToTop = false;

            Point3d ptMat = ptInit;
            while (DoHoriz)
            {
                while (DoVert)
                {
                    //Insert Point
                    pt3DColl.Add(ptMat);

                    //Check if continue Vertical
                    if (ToTop)
                    {
                        ptMat = new Point3d(ptMat.X, ptMat.Y + dHeight, ptMat.Z);
                        if (ptMat.Y > ptEnd.Y)
                            DoVert = false;
                    }
                    else
                    {
                        ptMat = new Point3d(ptMat.X, ptMat.Y - dHeight, ptMat.Z);
                        if (ptMat.Y < ptEnd.Y)
                            DoVert = false;
                    }
                }

                //Check if continue Horizontal
                if (ToLeft)
                {
                    ptMat = new Point3d(ptMat.X + dWidth, ptInit.Y, ptMat.Z);
                    if (ptMat.X > ptEnd.X)
                        DoHoriz = false;
                }
                else
                {
                    ptMat = new Point3d(ptMat.X - dWidth, ptInit.Y, ptMat.Z);
                    if (ptMat.X < ptEnd.X)
                        DoHoriz = false;
                }
                //Do Loop in Vertical again
                DoVert = true;
            }
            return pt3DColl;
        }

        //Acha um item do ansestral (eu num consigo explica...)
        public static T FindAncestor<T>(DependencyObject objCurrent) where T : DependencyObject
        {
            do
            {
                if (objCurrent is T)
                {
                    return (T)objCurrent;
                }
                objCurrent = VisualTreeHelper.GetParent(objCurrent);
            }
            while (objCurrent != null);
            return null;
        }

        static public BitmapImage GetBitmapImage(Bitmap bmpBit)
        {
            if (bmpBit == null)
                return null;

            MemoryStream ms = new MemoryStream();
            bmpBit.Save(ms, ImageFormat.Png);
            ms.Position = 0;
            BitmapImage bmpImage = new BitmapImage();
            bmpImage.BeginInit();
            bmpImage.StreamSource = ms;
            bmpImage.EndInit();

            return bmpImage;
        }

        static public BitmapImage LoadImage(Bitmap bmpBit)
        {            
            if (bmpBit == null)
                return null;

            Bitmap pic = bmpBit;
            MemoryStream ms = new MemoryStream();
            pic.Save(ms, ImageFormat.Png);
            BitmapImage bmpImage = new BitmapImage();
            bmpImage.BeginInit();
            bmpImage.StreamSource = ms;
            bmpImage.EndInit();

            return bmpImage;
        }

        static public ObjectIdCollection GetObjIdNonGrafical(Database db)
        {
            ObjectIdCollection idsNonGraficals = new ObjectIdCollection();

            Transaction tr = db.TransactionManager.StartTransaction();
            using (tr)
            {               

                //Block Table
                BlockTable btBlock = tr.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable;
                foreach (ObjectId btrId in btBlock)
                    idsNonGraficals.Add(btrId);

                //Dim Style
                DimStyleTable tbDimSty = tr.GetObject(db.DimStyleTableId, OpenMode.ForRead) as DimStyleTable;
                foreach (ObjectId btrId in tbDimSty)
                    idsNonGraficals.Add(btrId);

                //Layer
                LayerTable tbLayer = tr.GetObject(db.LayerTableId, OpenMode.ForRead) as LayerTable;
                foreach (ObjectId btrId in tbLayer)
                    idsNonGraficals.Add(btrId);

                //Line types
                LinetypeTable tbLinetype = tr.GetObject(db.LinetypeTableId, OpenMode.ForRead) as LinetypeTable;
                foreach (ObjectId btrId in tbLinetype)
                    idsNonGraficals.Add(btrId);

                //Table Style
                TextStyleTable tbTextStyle = tr.GetObject(db.TextStyleTableId, OpenMode.ForRead) as TextStyleTable;
                foreach (ObjectId btrId in tbTextStyle)
                    idsNonGraficals.Add(btrId);

                foreach (ObjectId objId in idsNonGraficals)
                {
                    if (!objId.IsValid)
                        idsNonGraficals.Remove(objId);
                }
            }

            return idsNonGraficals;
        }

        public static Bitmap GetBitmap(Database db, ObjectIdCollection objIdColl)
        {
            using (db)
            {
                using (Transaction trans = db.TransactionManager.StartTransaction())
                {
                    try
                    {
                        Manager manager = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.GraphicsManager;
                        Device device = manager.CreateAutoCADOffScreenDevice();
                        using (device)
                        {
                            device.OnSize(new System.Drawing.Size(100, 100));
                            Autodesk.AutoCAD.GraphicsSystem.Model model = manager.CreateAutoCADModel();
                            using (model)
                            {

                                double dMinX = 0, dMinY = 0, dMinZ = 0;
                                double dMaxX = 0, dMaxY = 0, dMaxZ = 0;

                                bool isUnstarted = true;
                                Autodesk.AutoCAD.GraphicsSystem.View view = new Autodesk.AutoCAD.GraphicsSystem.View();
                                foreach (ObjectId objId in objIdColl)
                                {
                                    // será usado pra definir limites para criar bitmap
                                    Entity ent = trans.GetObject(objId, OpenMode.ForWrite) as Entity;
                                    //Min Point
                                    if (ent.GeometricExtents.MinPoint.X < dMinX || isUnstarted)
                                        dMinX = ent.GeometricExtents.MinPoint.X;
                                    if (ent.GeometricExtents.MinPoint.Y < dMinY || isUnstarted)
                                        dMinY = ent.GeometricExtents.MinPoint.Y;
                                    if (ent.GeometricExtents.MinPoint.Z < dMinZ || isUnstarted)
                                        dMinZ = ent.GeometricExtents.MinPoint.Z;

                                    //Max Point
                                    if (ent.GeometricExtents.MaxPoint.X > dMaxX || isUnstarted)
                                        dMaxX = ent.GeometricExtents.MaxPoint.X;
                                    if (ent.GeometricExtents.MaxPoint.Y > dMaxY || isUnstarted)
                                        dMaxY = ent.GeometricExtents.MaxPoint.Y;
                                    if (ent.GeometricExtents.MaxPoint.Z > dMaxZ || isUnstarted)
                                        dMaxZ = ent.GeometricExtents.MaxPoint.Z;

                                    isUnstarted = false;
                                    view.Add(ent, model);
                                    
                                }

                                Point3d p3dLeftLower = new Point3d(dMinX, dMinY, dMinZ);
                                Point3d p3dRightTop = new Point3d(dMaxX, dMaxY, dMaxZ);
                                device.Add(view);

                                view.ZoomExtents(p3dLeftLower, p3dRightTop);
                                System.Drawing.Rectangle rect = new Rectangle();// view.Viewport;

                                return view.GetSnapshot(rect);
                            }
                        }
                    }
                    catch (System.Exception ex)
                    {
                        trans.Abort();
                        return null;
                    }
                }
            }
        }
    }
}
