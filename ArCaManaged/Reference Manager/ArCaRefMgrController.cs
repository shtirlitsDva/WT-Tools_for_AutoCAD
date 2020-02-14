using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using ArCaManaged.Reference_Manager.Model;
using ArCaManaged.Reference_Manager.View;
using System.Windows.Media.Imaging;
using Autodesk.AutoCAD.DatabaseServices;
using System.Drawing.Imaging;
using System.Drawing;
using Autodesk.AutoCAD.ApplicationServices;
using ArCaManaged.Commands;
using ArCaManaged.Utils;

namespace ArCaManaged.Reference_Manager.Controller
{
    static class ArCaRefMgrController
    {
        private static MainModel _objMain;
        private static string _sAppPath;
        private static string _sBlocksPath;
        private static string _sDBPath;

        static public void Initialize()
        {
            _objMain = new MainModel();
            _sAppPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            DBPath = _sAppPath + "\\DB.DWG";    
            ArCaRefMgrParser.ReadXML(ref _objMain);                   
        }

        static public BitmapImage GetThumbnail(string blockName)
        {
            Bitmap bmpPreview;
            using (Database dbRef = new Database(false, true))
            {
                // Read the DWG
                dbRef.ReadDwgFile(DBPath, FileShare.Read, true, "");
                using (Transaction trans = dbRef.TransactionManager.StartTransaction())
                {
                    BlockTable bt = trans.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                    BlockTableRecord BlcTabRec = trans.GetObject(bt[blockName], OpenMode.ForRead) as BlockTableRecord;
                    bmpPreview = BlcTabRec.PreviewIcon;
                }
            }
            return ArCaUtils.GetBitmapImage(bmpPreview);
        }

        public static ObjectId GetBlockId(BlockModel objBlockModel)
        {
            using (Database dbLocal = Application.DocumentManager.MdiActiveDocument.Database)
            {                
                using (Transaction trLocal = dbLocal.TransactionManager.StartTransaction())
                {
                    //abrir tabela de blocos
                    BlockTable bt = trLocal.GetObject(dbLocal.BlockTableId, OpenMode.ForRead) as BlockTable;

                    //verificar se o bloco já existe
                    if (bt.Has(objBlockModel.Name))
                        return bt[objBlockModel.Name];

                    //Não existe, lê o DWG e isere no db
                    IdMapping mapping = new IdMapping();
                    ObjectIdCollection blockIds = new ObjectIdCollection();
                    using (Database dbRef = new Database(false, true))
                    {

                        dbRef.ReadDwgFile(DBPath, FileShare.Read, true, "");
                        using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                        {
                            BlockTable btRef = trRef.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                            if (btRef.Has(objBlockModel.Name))
                            {
                                blockIds.Add(btRef[objBlockModel.Name]);
                                //Unlock document
                                Document Doc = Application.DocumentManager.MdiActiveDocument;
                                using (DocumentLock dl = Doc.LockDocument())
                                {
                                    // Copy blocks from source to destination database
                                    dbRef.WblockCloneObjects(blockIds, dbLocal.BlockTableId, mapping, DuplicateRecordCloning.Replace, false);
                                }
                            }
                        }
                    }
                    //Get BlockRecord objectId from dbLocal
                    BlockTableRecord BlocRec = trLocal.GetObject(bt[objBlockModel.Name], OpenMode.ForRead) as BlockTableRecord;
                    trLocal.Commit();
                    return BlocRec.Id;
                }
            }
        }

        public static ObjectIdCollection GetIdCollectionFromBlock(string sBlockName, Database db)
        {
            ObjectIdCollection objIdColl = new ObjectIdCollection();
            //Set Selection                
            using (Transaction trRef = db.TransactionManager.StartTransaction())
            {
                BlockTable bt = trRef.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable;
                BlockTableRecord btr = trRef.GetObject(bt[sBlockName], OpenMode.ForRead) as BlockTableRecord;

                foreach (ObjectId entId in btr)
                    objIdColl.Add(entId);
            }
            return objIdColl;
        }

        public static BlockModel GetBlockModelByName(string name)
        {
            foreach (BlockGroupModel objBlockGroup in ArCaRefMgrController.ObjMain.LstGroup)//Na Tôra!
                foreach (BlockModel objBlock in objBlockGroup.LstBlock)
                    if(objBlock.Name == name)
                        return objBlock;
                
            return null;
        }

        public static MainModel ObjMain
        {
            get { return _objMain; }
            set { _objMain = value; }
        }

        public static string AppPath
        {
            get { return _sAppPath; }
            set { _sAppPath = value; }
        }

        public static string AppConfig
        {
            get { return _sAppPath + "\\ArCaConfig.xml"; }
        }

        public static string DBPath
        {
            get { return ArCaRefMgrController._sDBPath; }
            set { ArCaRefMgrController._sDBPath = value; }
        }
    }
}
