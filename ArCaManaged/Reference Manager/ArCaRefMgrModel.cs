using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;
using System.IO;
using System.Reflection;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace ArCaManaged.Reference_Manager.Model
{
    public class MainModel
    {
        #region Attributes

        private ObservableCollection<BlockGroupModel> _lstGroup;

        #endregion

        #region Initialization

        public MainModel()
        {
            _lstGroup = new ObservableCollection<BlockGroupModel>();
        }

        #endregion

        #region Property


        public ObservableCollection<BlockGroupModel> LstGroup
        {
            get { return _lstGroup; }
            set { _lstGroup = value; }
        }

        #endregion
    }

    public class BlockGroupModel : INotifyPropertyChanged
    {
        #region Attributes

        private string _sName;
        private ObservableCollection<BlockModel> _lstBlock;

        #endregion

        #region Initialization

        public BlockGroupModel(string sName)
        {
            _sName = sName;
            _lstBlock = new ObservableCollection<BlockModel>();
        }

        #endregion

        #region Property

        public string Name
        {
            get { return _sName; }
            set { 
                _sName = value;
                FirePropertyChanged("Name");
            }
        }


        public ObservableCollection<BlockModel> LstBlock
        {
            get { return _lstBlock; }
            set { _lstBlock = value; }
        }

        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        protected void FirePropertyChanged(string sPropertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(sPropertyName));
        }

        #endregion
    }

    public class BlockModel : INotifyPropertyChanged
    {
        #region Attributes

        private string _sName;
        private string _sDescription;
        private BitmapImage _btmImage;
        private ObservableCollection<BlockPropModel> _lstBlockProp;

        #endregion

        #region Initialization

        public BlockModel()
        {
            _lstBlockProp = new ObservableCollection<BlockPropModel>();
        }

        public BlockModel(string sName, string sDescription)
        {
            _sName = sName;
            _sDescription = sDescription;
            _lstBlockProp = new ObservableCollection<BlockPropModel>();
        }

        #endregion

        #region Methodes

        public BlockModel Clone()
        {
            BlockModel clone = new BlockModel(Name, Description);
            if(btmImage != null)
                clone.btmImage = btmImage.Clone();
            foreach(BlockPropModel BlocProp in _lstBlockProp)
            {
                clone.LstBlockProp.Add(BlocProp.Clone());
            }
            return clone;
        }

        #endregion

        #region Property

        public ObservableCollection<BlockPropModel> LstBlockProp
        {
            get { return _lstBlockProp; }
            set { _lstBlockProp = value; }
        }

        public string Name
        {
            get { return _sName; }
            set { 
                _sName = value;
                FirePropertyChanged("Name");
            }
        }

        public string Description
        {
            get { return _sDescription; }
            set { 
                _sDescription = value;
                FirePropertyChanged("Description");
            }
        }

        public BitmapImage btmImage
        {
            get { return _btmImage; }
            set { 
                _btmImage = value;
                FirePropertyChanged("btmImage");
            }
        }
        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        protected void FirePropertyChanged(string sPropertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(sPropertyName));
        }

        #endregion
    }
    public class BlockPropModel : INotifyPropertyChanged
    {
        #region Attributes

        private string _sName;
        
        private bool _bRotate;
        private double _dAngle;

        private bool _bScale;
        private double _dSclX;
        private double _dSclY;
        private double _dSclZ;

        private bool _bMirror;
        private bool _bLoop;
        private bool _bExplode;

        private bool _bMatrix;
        private double _dMatrixWidth;
        private double _dMatrixHeight;
        private double _dMatrixMaxQuant;

        #endregion

        #region Initialization

        public BlockPropModel(string _Name)
        {
            _sName = _Name;

            _bRotate = false;
            _dAngle = 0;

            _bScale = false;
            _dSclX = 1;
            _dSclY = 1;
            _dSclZ = 1;

            _bLoop = false;
            _bExplode = false;

            _bMatrix = false;
            _dMatrixWidth = 10;
            _dMatrixHeight = 10;
            _dMatrixMaxQuant = 500;
        }

        #endregion

        #region Methodes

        public BlockPropModel Clone()
        {
            BlockPropModel clone = new BlockPropModel(Name);

            clone.Rotate = Rotate;
            clone.Angle = Angle;
            clone.Scale = Scale;
            clone.SclX = SclX;
            clone.SclY = SclY;
            clone.SclZ = SclZ;
            clone.Loop = Loop;
            clone.Explode = Explode;
            clone.Matrix = Matrix;
            clone.MatrixHeight = MatrixHeight;
            clone.MatrixWidth = MatrixWidth;
            clone.MatrixMaxQuant = MatrixMaxQuant;

            return clone;
        }

        #endregion

        #region Property

        public string Name
        {
            get { return _sName; }
            set { 
                _sName = value;
                FirePropertyChanged("Name");
            }
        }
        public bool Rotate
        {
            get { return _bRotate; }
            set { 
                _bRotate = value;
                FirePropertyChanged("Rotate");
            }
        }
        public double Angle
        {
            get { return _dAngle; }
            set { 
                _dAngle = value;
                FirePropertyChanged("Angle");
            }
        }

        public bool Scale
        {
            get { return _bScale; }
            set { 
                _bScale = value;
                FirePropertyChanged("Scale");
            }
        }
        public double SclX
        {
            get { return _dSclX; }
            set { 
                _dSclX = value;
                FirePropertyChanged("SclX");
            }
        }
        public double SclY
        {
            get { return _dSclY; }
            set { 
                _dSclY = value;
                FirePropertyChanged("SclY");
            }
        }
        public double SclZ
        {
            get { return _dSclZ; }
            set { 
                _dSclZ = value;
                FirePropertyChanged("SclZ");
            }
        }

        public bool Mirror
        {
            get { return _bMirror; }
            set { _bMirror = value; }
        }
        public bool Loop
        {
            get { return _bLoop; }
            set { 
                _bLoop = value;
                FirePropertyChanged("Loop");
            }
        }
        public bool Explode
        {
            get { return _bExplode; }
            set { 
                _bExplode = value;
                FirePropertyChanged("Explode");
            }
        }
        public bool Matrix
        {
            get { return _bMatrix; }
            set { 
                _bMatrix = value;
                FirePropertyChanged("Matrix");
            }
        }
        public double MatrixWidth
        {
            get { return _dMatrixWidth; }
            set { 
                _dMatrixWidth = value;
                FirePropertyChanged("MatrixWidth");
            }
        }
        public double MatrixHeight
        {
            get { return _dMatrixHeight; }
            set { 
                _dMatrixHeight = value;
                FirePropertyChanged("MatrixHeight");
            }
        }
        public double MatrixMaxQuant
        {
            get { return _dMatrixMaxQuant; }
            set { 
                _dMatrixMaxQuant = value;
                FirePropertyChanged("MatrixMaxQuant");
            }
        }

        #endregion

        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        protected void FirePropertyChanged(string sPropertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(sPropertyName));
        }

        #endregion
    }
}
