using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using ArCaManaged.Reference_Manager.Model;
using ArCaManaged.Reference_Manager.Controller;
using ArCaManaged.Reference_Manager.View;
using ArCaManaged.Commands;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.GraphicsSystem;
using Autodesk.AutoCAD.GraphicsInterface;
using ArCaManaged.Utils;

namespace ArCaManaged.Reference_Manager.View
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class ArCaBlockView : Window
    {
        private ObjectIdCollection objIdColl;
        private Bitmap bmpPreview;
        private BlockModel _objBlockModOld;
        private BlockModel _objBlockModNew;
        private bool _bDbIsInExternalDWG;
        private bool _IsAddMode;
        private bool _SelectionIsABlock;

        public ArCaBlockView(BlockModel objBlockModel, bool IsAddMode)
        {
            InitializeComponent();
            _IsAddMode = IsAddMode;
            _objBlockModOld = objBlockModel;
            _objBlockModNew = objBlockModel.Clone();
            GridMain.DataContext = _objBlockModNew;
            cbCategory.ItemsSource = ArCaManaged.Commands.ArCaCommands._formRefMgr.cbMain.ItemsSource;
            cbCategory.SelectedIndex = ArCaManaged.Commands.ArCaCommands._formRefMgr.cbMain.SelectedIndex;
            GridProp.DataContext = _objBlockModNew.LstBlockProp[0];//Set First property
            objIdColl = new ObjectIdCollection();

            if (IsAddMode)
                InitializeAddMode();
            else
                InitializeEditMode();            
        }

        private void InitializeAddMode()
        {
            //Set Form Tile and Botton
            Form.Title = "Add Block";
            btOk.Content = "Add";
            //Define LocalDWG
            _bDbIsInExternalDWG = false;
            //Define SelectionIsABlock
            _SelectionIsABlock = false;
        }

        private void InitializeEditMode()
        {
            //Set Form Tile
            Form.Title = "Edit Block";
            btOk.Content = "Edit";
            //Define ExternalDWG
            _bDbIsInExternalDWG = true;
            //Define SelectionIsABlock
            _SelectionIsABlock = false;
            //Set Category
            foreach (BlockGroupModel objIt in ArCaRefMgrController.ObjMain.LstGroup)
                if (objIt.LstBlock.Contains(_objBlockModNew))
                    cbCategory.SelectedIndex = ArCaRefMgrController.ObjMain.LstGroup.IndexOf(objIt);


            //Read Block DWG
            using (Database dbRef = new Database(false, true))
            {

                // Read the DWG
                dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.Read, true, "");

                //Set Selection                
                using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                {
                    BlockTable bt = trRef.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                    BlockTableRecord btr = trRef.GetObject(bt[_objBlockModNew.Name], OpenMode.ForRead) as BlockTableRecord;

                    //Set Base Point
                    Point3d pt3dBase = btr.Origin;
                    tbX.Text = pt3dBase.X.ToString();
                    tbY.Text = pt3dBase.Y.ToString();
                    tbZ.Text = pt3dBase.Z.ToString();

                    foreach (ObjectId entId in btr)
                        objIdColl.Add(entId);
                }

                //Set Selection Text
                tblObjSelDesc.Text = objIdColl.Count.ToString() + " objects in this block.";
            }
        }

        private void btPick_Click(object sender, RoutedEventArgs e)
        {
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            this.Hide();

            PromptPointOptions opcoesPonto = new PromptPointOptions("Specify insertion base point: ");
            opcoesPonto.AllowNone = false;

            PromptPointResult ProPtRes = ed.GetPoint(opcoesPonto);

            switch (ProPtRes.Status)
            {
                case PromptStatus.OK:
                    //Set Pick Point
                    tbX.Text = ProPtRes.Value.X.ToString();
                    tbY.Text = ProPtRes.Value.Y.ToString();
                    tbZ.Text = ProPtRes.Value.Z.ToString();
                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
            this.Show();            
        }

        private void btSelObj_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();

            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;

            PromptSelectionOptions optSel = new PromptSelectionOptions();
            PromptSelectionResult ProSelRes = ed.GetSelection(optSel);

            switch (ProSelRes.Status)
            {
                case PromptStatus.OK:
                    //Insert Wblock by baita gambiarra horrorosa que dá medo, usando sendstringtoexecute e lisp.
                    //string strToExecute = "_-WBLOCK C:/teste1.dwg _Y  " + tbX.Text + "," + tbY.Text + "," + tbZ.Text + " _L ";
                    //int filedia = Convert.ToInt32(Autodesk.AutoCAD.ApplicationServices.Application.GetSystemVariable("FILEDIA"));
                    //string strToExecute = "(COMMAND \"_-WBLOCK\" \"C:/teste1.dwg\" \"_Y\" \"\" \"" + tbX.Text + "," + tbY.Text + "," + tbZ.Text + "\" \"_P\" \"\" \"_UNDO\" \"1\" \"_FILEDIA\" \"" + filedia.ToString() + "\") ";                    
                    //Autodesk.AutoCAD.ApplicationServices.Application.SetSystemVariable("FILEDIA", 0);
                    //Doc.SendStringToExecute(strToExecute, false, false, false);

                    //Criate Wblock
                    Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
                    using (db)
                    {
                        using (Transaction trans = db.TransactionManager.StartTransaction())
                        {
                            try
                            {
                                objIdColl.Clear();
                                foreach (ObjectId objId in ProSelRes.Value.GetObjectIds())
                                    objIdColl.Add(objId);

                                //Define LocalDWG
                                _bDbIsInExternalDWG = false;
                                //Define SelectionIsABlock
                                _SelectionIsABlock = false;
                                //Set Thumbnail
                                _objBlockModNew.btmImage = GetBitmap();
                                //Set Selection Text
                                tblObjSelDesc.Text = objIdColl.Count.ToString() + " objects selecteds.";

                                //Enable fields
                                tbName.IsEnabled = true;
                                tbX.IsEnabled = true;
                                tbY.IsEnabled = true;
                                tbZ.IsEnabled = true;
                                btPick.IsEnabled = true;
                            }
                            catch (System.Exception ex)
                            {
                                trans.Abort();
                            }
                        }
                    }

                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
            this.Show();         
        }

        private void btSelBlo_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();

            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;

            PromptEntityOptions optSel = new PromptEntityOptions("\nSelect a block: ");
            optSel.SetRejectMessage("\nMust be a Block...");
            optSel.AddAllowedClass(typeof(BlockReference), true);
            PromptEntityResult ProSelRes = ed.GetEntity(optSel);

            switch (ProSelRes.Status)
            {
                case PromptStatus.OK:
                    //Criate Wblock
                    Database db = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
                    using (db)
                    {
                        using (Transaction trans = db.TransactionManager.StartTransaction())
                        {
                            try
                            {
                                BlockReference br = (BlockReference)trans.GetObject(ProSelRes.ObjectId, OpenMode.ForRead);
                                objIdColl.Clear();
                                objIdColl.Add(br.ObjectId);
                                BlockTableRecord btr = new BlockTableRecord();
                                if(br.IsDynamicBlock)
                                    btr = (BlockTableRecord)trans.GetObject(br.DynamicBlockTableRecord, OpenMode.ForRead);
                                else
                                    btr = (BlockTableRecord)trans.GetObject(br.BlockTableRecord, OpenMode.ForRead);


                                //Define LocalDWG
                                _bDbIsInExternalDWG = false;
                                //Define SelectionIsABlock
                                _SelectionIsABlock = true;
                                //Set Thumbnail
                                _objBlockModNew.btmImage = GetBitmap();
                                //Set Selection Text
                                tblObjSelDesc.Text = "Block \"" + btr.Name + "\" selected.";
                                //Set Name
                                _objBlockModNew.Name = btr.Name;
                                //Set Pick Point
                                tbX.Text = btr.Origin.X.ToString();
                                tbY.Text = btr.Origin.Y.ToString();
                                tbZ.Text = btr.Origin.Z.ToString();

                                //Desable fields
                                tbName.IsEnabled = false;
                                tbX.IsEnabled = false;
                                tbY.IsEnabled = false;
                                tbZ.IsEnabled = false;
                                btPick.IsEnabled = false;

                                //Set Properties
                                //Angle
                                _objBlockModNew.LstBlockProp[0].Angle = ArCaUtils.RadianToDegree(br.Rotation);
                                //Scale
                                _objBlockModNew.LstBlockProp[0].SclX = br.ScaleFactors.X;
                                _objBlockModNew.LstBlockProp[0].SclY = br.ScaleFactors.Y;
                                _objBlockModNew.LstBlockProp[0].SclZ = br.ScaleFactors.Z;
                            }
                            catch (System.Exception ex)
                            {
                                trans.Abort();
                            }
                        }
                    }

                    break;
                case PromptStatus.Cancel:
                    //Command Canceled
                    break;
            }
            this.Show(); 
        }

        private void btCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btOk_Click(object sender, RoutedEventArgs e)
        {
            if (tbName.Text == "")
            {
                MessageBox.Show("The field \"Name\" must be filled.", "Field is not filled", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }
            if (cbCategory.SelectedIndex == -1)
            {
                MessageBox.Show("You must select one category.", "Category not selected", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }
            if (objIdColl.Count == 0)
            {
                MessageBox.Show("Select at least one object.", "Objects not selected", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }
            //Check ambiguous block name
            if (_objBlockModOld.Name != _objBlockModNew.Name)
            {
                using (Database dbRef = new Database(false, true))
                {
                    //Read dbRef
                    dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.ReadWrite, true, "");
                    using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                    {
                        BlockTable btRef = trRef.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                        BlockTableRecord btr;
                        foreach (ObjectId objId in btRef)
                        {
                            btr = trRef.GetObject(objId, OpenMode.ForRead) as BlockTableRecord;
                            if (btr.Name == _objBlockModNew.Name)
                            {
                                MessageBox.Show("The block called \"" + _objBlockModNew.Name + "\"  already exists in the library block. Choose another name.", "Invalid Name", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                                trRef.Abort();
                                return;
                            }
                        }
                        trRef.Abort();
                    }
                }
            }
            
            Database dbLocal = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Database;
                          
            Point3d basePoint = new Point3d(Convert.ToDouble(tbX.Text), Convert.ToDouble(tbY.Text), Convert.ToDouble(tbZ.Text));
            using (dbLocal)
            {
                //-----------------Add Mode--------------
                if (_IsAddMode)
                {
                    //-----------Selection Block Mode------------
                    if (_SelectionIsABlock)
                    {
                        //Start Transaction to change BlockTableRecord
                        using (Transaction trLocal = dbLocal.TransactionManager.StartTransaction())
                        {
                            Document Doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                            using (DocumentLock dl = Doc.LockDocument())
                            {
                                BlockTable bt = trLocal.GetObject(dbLocal.BlockTableId, OpenMode.ForRead) as BlockTable;
                                BlockTableRecord BlocRec = trLocal.GetObject(bt[_objBlockModNew.Name], OpenMode.ForWrite) as BlockTableRecord;
                                //Change PreviewIcon
                                BlocRec.PreviewIcon = bmpPreview;
                            }
                            trLocal.Commit();
                        }
                        using (Database dbRef = new Database(false, true))
                        {
                            //Read dbRef
                            dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.ReadWrite, true, "");
                            // Copy blocks from source to destination database
                            IdMapping mapping = new IdMapping();
                            dbLocal.WblockCloneObjects(objIdColl, dbRef.BlockTableId, mapping, DuplicateRecordCloning.Replace, false);
                            //Save dbRef
                            dbRef.SaveAs(ArCaRefMgrController.DBPath, DwgVersion.Current);
                        }
                    }
                    //------------------Selection Objects Mode------------------
                    else
                    {
                        using (Database dbAux = new Database())
                        {
                            //Create dbAux (block)
                            dbLocal.Wblock(dbAux, objIdColl, basePoint, DuplicateRecordCloning.Ignore);
                            using (Database dbRef = new Database(false, true))
                            {
                                //Read dbRef
                                dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.ReadWrite, true, "");
                                //Insert dbAuc (Block) in dbRef
                                ObjectId obj = dbRef.Insert(_objBlockModNew.Name, dbAux, true);
                                //Start Transaction to change BlockTableRecord
                                using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                                {
                                    BlockTableRecord BlocRec = trRef.GetObject(obj, OpenMode.ForWrite) as BlockTableRecord;
                                    //Change PreviewIcon
                                    BlocRec.PreviewIcon = bmpPreview;
                                }
                                //Save dbRef
                                dbRef.SaveAs(ArCaRefMgrController.DBPath, DwgVersion.Current);
                            }
                        }
                    }
                }
                //----------------Edit Mode----------------
                else
                {
                    //-------------Selection was not changed-------------
                    if (_bDbIsInExternalDWG)
                    {
                        using (Database dbRef = new Database(false, true))
                        {
                            //Read dbRef
                            dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.ReadWrite, true, "");
                            //Start Transaction to change BlockTableRecord
                            using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                            {
                                BlockTable btRef = trRef.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                                BlockTableRecord BlocRec = trRef.GetObject(btRef[_objBlockModOld.Name], OpenMode.ForWrite) as BlockTableRecord;
                                //Change Origin
                                BlocRec.Origin = basePoint;
                                //Chage Name
                                BlocRec.Name = _objBlockModNew.Name;
                                trRef.Commit();
                            }
                            //Save dbRef
                            dbRef.SaveAs(ArCaRefMgrController.DBPath, DwgVersion.Current);
                        }
                    }
                    //----------------Selection was changed------------------
                    else
                    {
                        using (Database dbAux = new Database())
                        {
                            //Create dbAux (block)
                            dbLocal.Wblock(dbAux, objIdColl, basePoint, DuplicateRecordCloning.Ignore);
                            using (Database dbRef = new Database(false, true))
                            {
                                //Read dbRef
                                dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.ReadWrite, true, "");
                                //*************Delete Old Block************
                                //Start Transaction to delet BlockTableRecord
                                using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                                {
                                    BlockTable btRef = trRef.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                                    BlockTableRecord BlocRec = trRef.GetObject(btRef[_objBlockModOld.Name], OpenMode.ForWrite) as BlockTableRecord;
                                    //Delet BlockTableRecord
                                    BlocRec.Erase();
                                    trRef.Commit();
                                }
                                //************Insert New Block**************
                                //-----------Selection Block Mode------------
                                if (_SelectionIsABlock)
                                {
                                    //Start Transaction to change BlockTableRecord
                                    using (Transaction trLocal = dbLocal.TransactionManager.StartTransaction())
                                    {
                                        Document Doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                                        using (DocumentLock dl = Doc.LockDocument())
                                        {
                                            BlockTable btRef = trLocal.GetObject(dbLocal.BlockTableId, OpenMode.ForRead) as BlockTable;
                                            BlockTableRecord BlocRec = trLocal.GetObject(btRef[_objBlockModNew.Name], OpenMode.ForWrite) as BlockTableRecord;
                                            //Change PreviewIcon
                                            BlocRec.PreviewIcon = bmpPreview;
                                        }
                                        trLocal.Commit();
                                    }

                                    // Copy blocks from source to destination database
                                    IdMapping mapping = new IdMapping();
                                    dbLocal.WblockCloneObjects(objIdColl, dbRef.BlockTableId, mapping, DuplicateRecordCloning.Replace, false);
                                    //Save dbRef
                                    dbRef.SaveAs(ArCaRefMgrController.DBPath, DwgVersion.Current);

                                }
                                //--------------Selection Objects Mode--------------
                                else
                                {
                                    //Insert dbAuc (Block) in dbRef
                                    ObjectId obj = dbRef.Insert(_objBlockModNew.Name, dbAux, true);
                                    //Start Transaction to change BlockTableRecord
                                    using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                                    {
                                        BlockTableRecord BlocRec = trRef.GetObject(obj, OpenMode.ForWrite) as BlockTableRecord;
                                        //Change PreviewIcon
                                        BlocRec.PreviewIcon = bmpPreview;
                                        //Change Origin
                                        BlocRec.Origin = basePoint;
                                        //Chage Name
                                        BlocRec.Name = _objBlockModNew.Name;
                                        trRef.Commit();
                                    }
                                }
                                //***********************************************
                                //Save dbRef
                                dbRef.SaveAs(ArCaRefMgrController.DBPath, DwgVersion.Current);
                            }
                        }
                    }
                }
            }

            var lstFound = ArCaRefMgrController.ObjMain.LstGroup.Where(objTag => objTag.Name.Equals((cbCategory.SelectedValue as BlockGroupModel).Name));
            BlockGroupModel objBlockGroup = lstFound.First();

            //Remove in all Lists
            bool IsInserted = false;
            foreach (BlockGroupModel objIt in ArCaRefMgrController.ObjMain.LstGroup)
                if (objIt.LstBlock.Contains(_objBlockModOld))
                {
                    int position = objIt.LstBlock.IndexOf(_objBlockModOld);
                    objIt.LstBlock.Remove(_objBlockModOld);
                    if (objBlockGroup == objIt)
                    {
                        //Insert if is in the same group
                        objBlockGroup.LstBlock.Insert(position, _objBlockModNew);
                        IsInserted = true;
                    }
                } 

            //If not inset, add in correct group
            if(!IsInserted)
                objBlockGroup.LstBlock.Add(_objBlockModNew);


            //Write XML
            ArCaRefMgrParser.WriteXML();

            this.Close();
        }

        public BitmapImage GetBitmap()
        {
            //Suprimido pq tava dando fatal erro quando lia do bloco externo
            //if (_bDbIsInExternalDWG)
            //    db.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.Read, true, "");
            //else                
            try
            {
                Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
                using (Database db = doc.Database)
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
                                    //Suprimido pq tava dando fatal erro quando lia do bloco externo
                                    //if (rbWhite.IsChecked != true)
                                    //    device.BackgroundColor = System.Drawing.Color.Black;
                                    //else
                                    device.BackgroundColor = System.Drawing.Color.White;

                                    double dMinX = 0, dMinY = 0, dMinZ = 0;
                                    double dMaxX = 0, dMaxY = 0, dMaxZ = 0;

                                    Boolean isUnstarted = true;
                                    Autodesk.AutoCAD.GraphicsSystem.View view = new Autodesk.AutoCAD.GraphicsSystem.View();
                                    ObjectIdCollection objIdColl2 = new ObjectIdCollection();

                                    //As pitombas dos objectsId estao trocando de numero quando eu do Read no DWG, to tendo que lere os Ids no Bloco denovo
                                    if (_bDbIsInExternalDWG)
                                    {
                                        using (Transaction tr = db.TransactionManager.StartTransaction())
                                        {
                                            BlockTable bt = tr.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable;
                                            BlockTableRecord btr = tr.GetObject(bt[_objBlockModNew.Name], OpenMode.ForRead) as BlockTableRecord;
                                            foreach (ObjectId entId in btr)
                                                objIdColl2.Add(entId);
                                        }
                                    }
                                    else
                                        objIdColl2 = objIdColl;

                                    foreach (ObjectId objId in objIdColl2)
                                    {
                                        // será usado pra definir limites para criar bitmap
                                        using (DocumentLock dl = doc.LockDocument())
                                        {
                                            Entity ent = trans.GetObject(objId, OpenMode.ForRead) as Entity;
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
                                    }

                                    Point3d p3dLeftLower = new Point3d(dMinX, dMinY, dMinZ);
                                    Point3d p3dRightTop = new Point3d(dMaxX, dMaxY, dMaxZ);
                                    device.Add(view);

                                    view.ZoomExtents(p3dLeftLower, p3dRightTop);
                                    System.Drawing.Rectangle rect = new System.Drawing.Rectangle();// view.Viewport;
                                    bmpPreview = view.GetSnapshot(rect);
                                    return ArCaUtils.GetBitmapImage(bmpPreview);
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
            catch (System.Exception ex)
            {
                return null;
            }
        }

        //Suprimido pq tava dando fatal erro quando lia do bloco externo
        //private void rbImageColorCheck(object sender, RoutedEventArgs e)
        //{
        //    if(_objBlockModNew != null)
        //        _objBlockModNew.btmImage = GetBitmap();
        //}
    }
}
