using System;
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
using System.Windows.Shapes;
using System.Collections.ObjectModel;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;
using System.IO;
using Autodesk.AutoCAD.DatabaseServices;
using ArCaManaged.Utils;
using ArCaManaged.Reference_Manager.Model;

namespace ArCaManaged.ArCaDWGEditor
{
    /// <summary>
    /// Interaction logic for ArCaDWGEdRunView.xaml
    /// </summary>
    public partial class ArCaDWGEdRunView : Window
    {

        public ArCaDWGEdRunView()
        {
            InitializeComponent(); 
        }
    }
}
