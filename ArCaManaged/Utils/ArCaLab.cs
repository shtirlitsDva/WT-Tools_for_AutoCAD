using System;
using System.IO;
using System.Collections.Generic;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Windows;
using Autodesk.AutoCAD.GraphicsSystem;
//using Autodesk.AutoCAD.GraphicsInterface;
using System.Drawing;

[assembly: CommandClass(typeof(ArCaManaged.Utils.ArCaLab))]
namespace ArCaManaged.Utils
{
    public class ArCaLab
    {
        [CommandMethod("PaperZoomExtents")]
        static public void PaperZoomExtents()
        {
            Document doc = Application.DocumentManager.MdiActiveDocument;
            Database db = doc.Database;
            Editor ed = doc.Editor;

            using (Transaction Tx = db.TransactionManager.StartTransaction())
            {
                BlockTableRecord btr = Tx.GetObject(db.CurrentSpaceId, OpenMode.ForRead) as BlockTableRecord;

                //Retrieve paper space viewport (viewport with the lowest handle)
                Viewport paperVp = null;
                long lowestHandle = -1;

                foreach (ObjectId id in btr)
                {
                    if (id.ObjectClass.Name == "AcDbViewport")
                    {
                        Viewport vp = Tx.GetObject(id, OpenMode.ForRead) as Viewport;

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

                paperVp.UpgradeOpen();

                double scrRatio = (paperVp.Width / paperVp.Height);

                Matrix3d matWCS2DCS = Matrix3d.PlaneToWorld(paperVp.ViewDirection);

                matWCS2DCS = Matrix3d.Displacement(paperVp.ViewTarget - Point3d.Origin) * matWCS2DCS;

                matWCS2DCS = Matrix3d.Rotation(-paperVp.TwistAngle,
                                                paperVp.ViewDirection,
                                                paperVp.ViewTarget)
                                                * matWCS2DCS;

                matWCS2DCS = matWCS2DCS.Inverse();

                Extents3d extents = GetExtents(btr);

                extents.TransformBy(matWCS2DCS);

                double width = (extents.MaxPoint.X - extents.MinPoint.X);

                double height = (extents.MaxPoint.Y - extents.MinPoint.Y);

                Point2d center = new Point2d((extents.MaxPoint.X + extents.MinPoint.X) * 0.5,
                                             (extents.MaxPoint.Y + extents.MinPoint.Y) * 0.5);

                if (width > (height * scrRatio))
                    height = width / scrRatio;

                paperVp.ViewHeight = height;
                paperVp.ViewCenter = center;

                Tx.Commit();
            }
        }

        static Extents3d GetExtents(BlockTableRecord btr)
        {
            Database db = HostApplicationServices.WorkingDatabase;

            double[] min = null;
            double[] max = null;

            using (Transaction Tx = db.TransactionManager.StartTransaction())
            {
                bool FirstIsGone = false;
                foreach (ObjectId id in btr)
                {
                    try
                    {
                        Entity entity = Tx.GetObject(id, OpenMode.ForRead) as Entity;

                        //Check the first elemente
                        if (FirstIsGone)
                        {
                            FirstIsGone = true;
                            Viewport vp = entity as Viewport;
                            //if is a Viewport, not process this elemente
                            if(vp != null)
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
                    }                    
                    catch (System.Exception ex)
                    {
                    }
                }

                return new Extents3d(new Point3d(min), new Point3d(max));
            }
        }

        ////Get bitmap, em desenvolvimento...
        //static public void SnapshotToFile()
        //{
        //    Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
        //    Editor ed = doc.Editor;
        //    Database db = doc.Database;
        //    Manager gsm = doc.GraphicsManager;
        //    string filename = "c:\\test.png";

        //    // Get some AutoCAD system variables
        //    int vpn = System.Convert.ToInt32(Autodesk.AutoCAD.ApplicationServices.Application.GetSystemVariable("CVPORT"));

        //    // Get AutoCAD's GS view for this document...
        //    Autodesk.AutoCAD.GraphicsSystem.View gsv = doc.GraphicsManager.GetGsView(vpn, true);
        //    // ... but create a new one for the actual snapshot
        //    using (Autodesk.AutoCAD.GraphicsSystem.View view = new Autodesk.AutoCAD.GraphicsSystem.View())
        //    {
        //        // Set the view to be just like the one
        //        // in the AutoCAD editor
        //        view.Viewport = gsv.Viewport;
        //        view.SetView(
        //          gsv.Position,
        //          gsv.Target,
        //          gsv.UpVector,
        //          gsv.FieldWidth,
        //          gsv.FieldHeight
        //        );
        //        // Set the visual style to the one passed in
        //        view.VisualStyle = new Autodesk.AutoCAD.GraphicsInterface.VisualStyle(Autodesk.AutoCAD.GraphicsInterface.VisualStyleType.Wireframe2D);

        //        Device dev =
        //          gsm.CreateAutoCADOffScreenDevice();
        //        using (dev)
        //        {
        //            dev.OnSize(gsm.DisplaySize);

        //            // Set the render type and the background color
        //            dev.DeviceRenderType = RendererType.Default;
        //            dev.BackgroundColor = System.Drawing.Color.White;

        //            // Add the view to the device and update it
        //            dev.Add(view);
        //            dev.Update();

        //            using (Autodesk.AutoCAD.GraphicsSystem.Model model = gsm.CreateAutoCADModel())
        //            {
        //                Transaction tr =
        //                  db.TransactionManager.StartTransaction();
        //                using (tr)
        //                {
        //                    // Add the modelspace to the view
        //                    // It's a container but also a drawable
        //                    BlockTable bt =
        //                      (BlockTable)tr.GetObject(
        //                        db.BlockTableId,
        //                        OpenMode.ForRead
        //                      );
        //                    BlockTableRecord btr =
        //                      (BlockTableRecord)tr.GetObject(
        //                        bt[BlockTableRecord.ModelSpace],
        //                        OpenMode.ForRead
        //                      );
        //                    view.Add(btr, model);
        //                    tr.Commit();
        //                }
        //                // Take the snapshot
        //                System.Drawing.Rectangle rect = view.Viewport;
        //                rect.Height += rect.Height;
        //                rect.Width += rect.Width;
        //                using (Bitmap bitmap = view.GetSnapshot(rect))
        //                {
        //                    bitmap.Save(filename);
        //                    ed.WriteMessage(
        //                      "\nSnapshot image saved to: " +
        //                      filename
        //                    );
        //                    // Clean up
        //                    view.EraseAll();
        //                    dev.Erase(view);
        //                }
        //            }
        //        }
        //    }
        //}


        [CommandMethod("CBL")]
        public void CombineBlocksIntoLibrary()
        {
            Document doc =
                Application.DocumentManager.MdiActiveDocument;
            Editor ed = doc.Editor;
            Database destDb = doc.Database;

            // Get name of folder from which to load and import blocks

            PromptResult pr =
              ed.GetString("\nEnter the folder of source drawings: ");

            if (pr.Status != PromptStatus.OK)
                return;
            string pathName = pr.StringResult;

            // Check the folder exists

            if (!Directory.Exists(pathName))
            {
                ed.WriteMessage(
                  "\nDirectory does not exist: {0}", pathName
                );
                return;
            }

            // Get the names of our DWG files in that folder

            string[] fileNames = Directory.GetFiles(pathName, "*.dwg");

            // A counter for the files we've imported

            int imported = 0, failed = 0;

            // For each file in our list

            foreach (string fileName in fileNames)
            {
                // Double-check we have a DWG file (probably unnecessary)

                if (fileName.EndsWith(
                      ".dwg",
                      StringComparison.InvariantCultureIgnoreCase
                    )
                )
                {
                    // Catch exceptions at the file level to allow skipping

                    try
                    {
                        // Suggestion from Thorsten Meinecke...

                        string destName =
                          SymbolUtilityServices.GetSymbolNameFromPathName(
                            fileName, "dwg"
                          );

                        // And from Dan Glassman...

                        destName =
                          SymbolUtilityServices.RepairSymbolName(
                            destName, false
                          );

                        // Create a source database to load the DWG into

                        using (Database db = new Database(false, true))
                        {
                            // Read the DWG into our side database

                            db.ReadDwgFile(fileName, FileShare.Read, true, "");
                            bool isAnno = db.AnnotativeDwg;

                            // Insert it into the destination database as
                            // a named block definition

                            ObjectId btrId = destDb.Insert(
                              destName,
                              db,
                              false
                            );

                            if (isAnno)
                            {
                                // If an annotative block, open the resultant BTR
                                // and set its annotative definition status

                                Transaction tr =
                                  destDb.TransactionManager.StartTransaction();
                                using (tr)
                                {
                                    BlockTableRecord btr =
                                      (BlockTableRecord)tr.GetObject(
                                        btrId,
                                        OpenMode.ForWrite
                                      );
                                    btr.Annotative = AnnotativeStates.True;
                                    tr.Commit();
                                }
                            }

                            // Print message and increment imported block counter

                            ed.WriteMessage("\nImported from \"{0}\".", fileName);
                            imported++;
                        }
                    }
                    catch (System.Exception ex)
                    {
                        ed.WriteMessage(
                          "\nProblem importing \"{0}\": {1} - file skipped.",
                          fileName, ex.Message
                        );
                        failed++;
                    }
                }
            }

            ed.WriteMessage(
              "\nImported block definitions from {0} files{1} in " +
              "\"{2}\" into the current drawing.",
              imported,
              failed > 0 ? " (" + failed + " failed)" : "",
              pathName
            );
        }

        [CommandMethod("mac")]
        public void MacGyver()
        {
            Document doc =
                Application.DocumentManager.MdiActiveDocument;
            Editor ed = doc.Editor;
            Database destDb = doc.Database;

            // Get name of folder from which to load and import blocks

            PromptResult pr =
              ed.GetString("\nEnter the folder of source drawings: ");

            if (pr.Status != PromptStatus.OK)
                return;
            string pathName = pr.StringResult;

            // Check the folder exists

            if (!Directory.Exists(pathName))
            {
                ed.WriteMessage(
                  "\nDirectory does not exist: {0}", pathName
                );
                return;
            }

            // Get the names of our DWG files in that folder

            string[] fileNames = Directory.GetFiles(pathName, "*.dwg");

            // A counter for the files we've imported

            int imported = 0, failed = 0;

            // For each file in our list

            foreach (string fileName in fileNames)
            {
                // Double-check we have a DWG file (probably unnecessary)

                if (fileName.EndsWith(
                      ".dwg",
                      StringComparison.InvariantCultureIgnoreCase
                    )
                )
                {
                    // Catch exceptions at the file level to allow skipping

                    try
                    {
                        // Suggestion from Thorsten Meinecke...

                        string destName =
                          SymbolUtilityServices.GetSymbolNameFromPathName(
                            fileName, "dwg"
                          );

                        // And from Dan Glassman...

                        destName =
                          SymbolUtilityServices.RepairSymbolName(
                            destName, false
                          );

                        // Create a source database to load the DWG into

                        using (Database db = new Database(false, true))
                        {
                            // Read the DWG into our side database

                            db.ReadDwgFile(fileName, FileShare.Read, true, "");

                            Database dbAux = db.Wblock();
                            db.Dispose();
                            dbAux.SaveAs(fileName, DwgVersion.Current);

                            // Print message and increment imported block counter

                            ed.WriteMessage("\nImported from \"{0}\".", fileName);
                            imported++;
                        }
                    }
                    catch (System.Exception ex)
                    {
                        ed.WriteMessage(
                          "\nProblem importing \"{0}\": {1} - file skipped.",
                          fileName, ex.Message
                        );
                        failed++;
                    }
                }
            }

            ed.WriteMessage(
              "\nImported block definitions from {0} files{1} in " +
              "\"{2}\" into the current drawing.",
              imported,
              failed > 0 ? " (" + failed + " failed)" : "",
              pathName
            );
        }

        public static ObjectId criarDefiniçãoDeBloco(ObjectId[] objIds)
        {
            string nomeBloco = "Teste1";
            Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
            using (Transaction trans = db.TransactionManager.StartTransaction())
            {
                try
                {
                    Autodesk.AutoCAD.ApplicationServices.Document Doc = null;
                    Doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                    using (DocumentLock dl = Doc.LockDocument())
                    {

                        //abrir tabela de blocos
                        BlockTable bt = trans.GetObject(db.BlockTableId, OpenMode.ForWrite) as BlockTable;

                        //verificar se o bloco já existe
                        if (bt.Has(nomeBloco)) return bt[nomeBloco];
                        BlockTableRecord novoBloco = new BlockTableRecord();
                        novoBloco.Name = nomeBloco;

                        bt.UpgradeOpen();
                        bt.Add(novoBloco);

                        trans.AddNewlyCreatedDBObject(novoBloco, true);

                        foreach (ObjectId objId in objIds)
                        {
                            DBObject obj = trans.GetObject(objId, OpenMode.ForRead);
                            Entity ent = (Entity)obj;
                            Entity entClone = ent.Clone() as Entity;
                            novoBloco.AppendEntity(entClone);
                            trans.AddNewlyCreatedDBObject(entClone, true);
                        }

                        trans.Commit();

                        return novoBloco.ObjectId;
                    }
                }
                catch (System.Exception ex)
                {
                    trans.Abort();

                    return ObjectId.Null;
                }
            }
        }
        public static void CrateExternalBlock()
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;

            PromptSelectionOptions optSel = new PromptSelectionOptions();
            PromptSelectionResult ProSelRes = ed.GetSelection(optSel);

            switch (ProSelRes.Status)
            {
                case PromptStatus.OK:
                    //Set Block

                    //Criate blockReference
                    ObjectId objBlock = criarDefiniçãoDeBloco(ProSelRes.Value.GetObjectIds());
                    Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
                    using (Transaction trans = db.TransactionManager.StartTransaction())
                    {
                        try
                        {

                            Database dbAux = new Database(true, false);
                            dbAux.SaveAs("C:\\teste1.dwg", DwgVersion.Current);

                            Autodesk.AutoCAD.ApplicationServices.Document Doc = null;
                            Doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.Open("C:\\teste1.dwg", false);

                            ObjectIdCollection ids = new ObjectIdCollection();
                            ids.Add(objBlock);
                            using (DocumentLock dl = Doc.LockDocument())
                            {
                                IdMapping im = new IdMapping();
                                db.WblockCloneObjects(
                                  ids,
                                  Doc.Database.BlockTableId,
                                  im,
                                  DuplicateRecordCloning.MangleName,
                                  false
                                );
                            }

                            Doc.CloseAndSave("C:\\teste1.dwg");
                        }
                        catch (System.Exception ex)
                        {
                            trans.Abort();

                            return;
                        }
                    }
                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
        }


        [CommandMethod("PF")]
        public void PurgeFiles()
        {
            Document doc =
              Application.DocumentManager.MdiActiveDocument;
            Editor ed = doc.Editor;

            PromptResult pr =
              ed.GetString(
                "\nEnter folder containing DWGs to process: "
              );
            if (pr.Status != PromptStatus.OK)
                return;
            string pathName = pr.StringResult;

            string[] fileNames =
              Directory.GetFiles(pathName, "*.dwg");

            // We'll use some counters to keep track
            // of how the processing is going

            int processed = 0, saved = 0, problem = 0;

            foreach (string fileName in fileNames)
            {
                if (fileName.EndsWith(
                      ".dwg",
                      StringComparison.CurrentCultureIgnoreCase
                    )
                )
                {
                    string outputName =
                      fileName.Substring(
                        0,
                        fileName.Length - 4) +
                      "_purged.dwg";
                    Database db = new Database(false, true);
                    using (db)
                    {
                        try
                        {
                            ed.WriteMessage(
                              "\n\nProcessing file: " + fileName
                            );

                            db.ReadDwgFile(
                              fileName,
                              FileShare.ReadWrite,
                              false,
                              ""
                            );

                            db.RetainOriginalThumbnailBitmap = true;

                            int objectsPurged =
                              PurgeDatabase(db);

                            // Display the results

                            ed.WriteMessage(
                              "\nPurged {0} object{1}",
                              objectsPurged,
                              objectsPurged == 1 ? "" : "s"
                            );

                            // Only save if we changed something

                            if (objectsPurged > 0)
                            {
                                ed.WriteMessage(
                                  "\nSaving to file: {0}", outputName
                                );

                                db.SaveAs(
                                  outputName,
                                  DwgVersion.Current
                                );
                                saved++;
                            }
                            processed++;
                        }
                        catch (System.Exception ex)
                        {
                            ed.WriteMessage(
                              "\nProblem processing file: {0} - \"{1}\"",
                              fileName,
                              ex.Message
                            );
                            problem++;
                        }
                    }
                }
            }
            ed.WriteMessage(
              "\n\nSuccessfully processed {0} files," +
              " of which {1} had objects to purge" +
              " and an additional {2} had errors " +
              "during reading/processing.",
              processed,
              saved,
              problem
            );
        }

        [CommandMethod("PC")]
        public void PurgeCurrentDocument()
        {
            Document doc =
              Application.DocumentManager.MdiActiveDocument;
            Database db = doc.Database;
            Editor ed = doc.Editor;

            int count =
              PurgeDatabase(db);

            ed.WriteMessage(
              "\nPurged {0} object{1} from " +
              "the current database.",
              count,
              count == 1 ? "" : "s"
            );
        }

        private static int PurgeDatabase(Database db)
        {
            int idCount = 0;

            Transaction tr =
              db.TransactionManager.StartTransaction();
            using (tr)
            {
                // Create the list of objects to "purge"

                ObjectIdCollection idsToPurge =
                  new ObjectIdCollection();

                // Add all the Registered Application names

                RegAppTable rat =
                  (RegAppTable)tr.GetObject(
                    db.RegAppTableId,
                    OpenMode.ForRead
                );

                foreach (ObjectId raId in rat)
                {
                    if (raId.IsValid)
                    {
                        idsToPurge.Add(raId);
                    }
                }

                // Call the Purge function to filter the list

                db.Purge(idsToPurge);

                Document doc =
                  Application.DocumentManager.MdiActiveDocument;
                Editor ed = doc.Editor;

                ed.WriteMessage(
                  "\nRegistered applications being purged: "
                );

                // Erase each of the objects we've been
                // allowed to

                foreach (ObjectId id in idsToPurge)
                {
                    DBObject obj =
                      tr.GetObject(id, OpenMode.ForWrite);

                    // Let's just add to me "debug" code
                    // to list the registered applications
                    // we're erasing

                    RegAppTableRecord ratr =
                      obj as RegAppTableRecord;
                    if (ratr != null)
                    {
                        ed.WriteMessage(
                          "\"{0}\" ",
                          ratr.Name
                        );
                    }

                    obj.Erase();
                }

                // Return the number of objects erased
                // (i.e. purged)

                idCount = idsToPurge.Count;
                tr.Commit();
            }
            return idCount;
        }
        public void SetViewportToExtents(Database db, ViewportTableRecord viewportTableRec)
        {
            //lets update the database extents first
            //true gives the best fit but will take time
            //db.UpdateExt(false); Does not work correctly

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

        [CommandMethod("ModelZoomExtents")]
        public void ModelZoomExtents()
        {
            Document doc = Application.DocumentManager.MdiActiveDocument;
            Database db = doc.Database;
            Editor ed = doc.Editor;

            using (Transaction Tx = db.TransactionManager.StartTransaction())
            {
                ed.UpdateTiledViewportsInDatabase();

                ViewportTableRecord viewportTableRec = Tx.GetObject(ed.ActiveViewportId, OpenMode.ForWrite) as ViewportTableRecord;

                SetViewportToExtents(db, viewportTableRec);

                ed.UpdateTiledViewportsFromDatabase();

                Tx.Commit();
            }
        }
        [CommandMethod("PaperZoomExtents2")]
        static public void PaperZoomExtents2()
        {
            Document doc = Application.DocumentManager.MdiActiveDocument;
            Database db = doc.Database;
            Editor ed = doc.Editor;

            PromptEntityOptions peo = new PromptEntityOptions("\nSelect a viewport: ");
            peo.SetRejectMessage("\nMust be a viewport...");
            peo.AddAllowedClass(typeof(Viewport), true);

            PromptEntityResult per = ed.GetEntity(peo);

            if (per.Status != PromptStatus.OK) return;

            using (Transaction Tx = db.TransactionManager.StartTransaction())
            {
                Viewport vp = Tx.GetObject(per.ObjectId, OpenMode.ForWrite) as Viewport;


                db.UpdateExt(true);

                double scrRatio = (vp.Width / vp.Height);

                Matrix3d matWCS2DCS = Matrix3d.PlaneToWorld(vp.ViewDirection);

                matWCS2DCS = Matrix3d.Displacement(vp.ViewTarget - Point3d.Origin) * matWCS2DCS;

                matWCS2DCS = Matrix3d.Rotation(-vp.TwistAngle,
                                                vp.ViewDirection,
                                                vp.ViewTarget)
                                                * matWCS2DCS;

                matWCS2DCS = matWCS2DCS.Inverse();

                Extents3d extents = new Extents3d(db.Extmin, db.Extmax);
                extents.TransformBy(matWCS2DCS);

                double width = (extents.MaxPoint.X - extents.MinPoint.X);

                double height = (extents.MaxPoint.Y - extents.MinPoint.Y);

                Point2d center = new Point2d((extents.MaxPoint.X + extents.MinPoint.X) * 0.5,
                                             (extents.MaxPoint.Y + extents.MinPoint.Y) * 0.5);

                if (width > (height * scrRatio))
                    height = width / scrRatio;

                vp.ViewHeight = height;
                vp.ViewCenter = center;

                Tx.Commit();
            }
        }
    }    
}
