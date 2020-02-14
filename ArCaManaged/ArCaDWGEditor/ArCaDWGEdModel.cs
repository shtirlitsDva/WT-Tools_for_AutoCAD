using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Autodesk.AutoCAD.DatabaseServices;
using System.Windows.Media.Imaging;
using ArCaManaged.Utils;
using System.Collections.ObjectModel;

namespace ArCaManaged.ArCaDWGEditor
{
    public class DWGCollectionModel
    {
        #region Attributes

        private ObservableCollection<DWGFileModel> _lstDWG;

        private bool _bZoomExtents;
        private bool _bPurge;
        private bool _bLockViewPorts;

        #endregion

        #region Initialization

        public DWGCollectionModel()
        {
            _lstDWG = new ObservableCollection<DWGFileModel>();
        }

        #endregion

        #region Property

        public ObservableCollection<DWGFileModel> LstDWG
        {
            get { return _lstDWG; }
            set { _lstDWG = value; }
        }

        public bool ZoomExtents
        {
            get { return _bZoomExtents; }
            set { _bZoomExtents = value; }
        }

        public bool Purge
        {
            get { return _bPurge; }
            set { _bPurge = value; }
        }

        public bool LockViewPorts
        {
            get { return _bLockViewPorts; }
            set { _bLockViewPorts = value; }
        }

        #endregion
    }

    public class DWGFileModel
    {
        #region Attributes

        private string _sFilePath;
        private string _sFileName;
        private string _sFileDate;
        private string _sFileSize;

        private bool _bIsChanged;
        private string _sMessageProcess;

        BitmapImage _btmImage;

        #endregion

        #region Initialization

        public DWGFileModel(string sFilePath)
        {
            _sFilePath = sFilePath;
            _sFileName = Path.GetFileNameWithoutExtension(sFilePath);
            _sFileDate = String.Format("{0:MM/dd/yyyy}", File.GetLastWriteTime(sFilePath));
            _sFileSize = String.Format("{0:MM/dd/yyyy}", File.GetLastWriteTime(sFilePath));
            FileInfo fi = new FileInfo(sFilePath);
            _sFileSize = (fi.Length / 1024).ToString() + " KB";
            _bIsChanged = false;
            _sMessageProcess = "Unprocessed";

            using (Database db = new Database(false, true))
            {
                // Read the DWG
                db.ReadDwgFile(sFilePath, FileShare.Read, true, "");
                _btmImage = ArCaUtils.GetBitmapImage(db.ThumbnailBitmap);
            }
        }

        #endregion

        #region Property

        public string FilePath
        {
            get { return _sFilePath; }
            set { _sFilePath = value; }
        }

        public string Name
        {
            get { return _sFileName; }
            set { _sFileName = value; }
        }

        public string Date
        {
            get { return _sFileDate; }
            set { _sFileDate = value; }
        }

        public string Size
        {
            get { return _sFileSize; }
            set { _sFileSize = value; }
        }

        public bool IsChanged
        {
            get { return _bIsChanged; }
            set { _bIsChanged = value; }
        }

        public string MessageProcess
        {
            get { return _sMessageProcess; }
            set { _sMessageProcess = value; }
        }

        public BitmapImage btmImage
        {
            get { return _btmImage; }
            set { _btmImage = value; }
        }

        #endregion
    }
}
