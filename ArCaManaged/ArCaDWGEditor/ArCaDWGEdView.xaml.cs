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
using ArCaManaged.ArCaDWGEditor;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace ArCaManaged.ArCaDWGEditor
{
    /// <summary>
    /// Interaction logic for ArCaDWGEditorView.xaml
    /// </summary>
    public partial class ArCaDWGEdView : Window
    {
        ArCaDWGEdRunView _DWGRun;

        public ArCaDWGEdView()
        {
            InitializeComponent();
            GridMain.DataContext = ArCaDWGEdController.DWGCollection;
        }

        private void btAddDWG_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.Multiselect = true;
            dlg.Title = "Add DWG";
            dlg.Filter = "DWG files (*.dwg)|*.dwg";            

            if (dlg.ShowDialog() == true)
            {
                foreach (string DWGFile in dlg.FileNames)
                {
                    DWGFileModel objDWG = new DWGFileModel(DWGFile);
                    ArCaDWGEdController.DWGCollection.LstDWG.Add(objDWG);
                }
            }
        }

        private void btRemoveDWG_Click(object sender, RoutedEventArgs e)
        {
            //Check if there any selected block in lvBlocks
            if (lbDWG.SelectedIndex == -1)
            {
                MessageBox.Show("Select one DWG first.", "Remove DWG", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }

            //Remove in List
            List<object> objToRemove = new List<object>();
            foreach (object obj in lbDWG.SelectedItems)
                objToRemove.Add(obj);
            foreach (object obj in objToRemove)
                ArCaDWGEdController.DWGCollection.LstDWG.Remove(obj as DWGFileModel);
        }

        private void btClose_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        [DllImport("accore.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "acedTrans")]
        static extern int acedTrans(double[] point, IntPtr fromRb, IntPtr toRb, int disp, double[] result);

        private void btRun_Click(object sender, RoutedEventArgs e)
        {
            //Restart DWGRun
            _DWGRun = new ArCaDWGEdRunView();
            _DWGRun.Owner = this;
            //Set ProgressBar
            _DWGRun.progressBar.Minimum = 0;
            _DWGRun.progressBar.Maximum = ArCaDWGEdController.DWGCollection.LstDWG.Count();
            _DWGRun.progressBar.Value = 0;
            //Show
            _DWGRun.Show();
            //Load Procedures
            ArCaDWGEdController.LoadProcedures();


            //------------------  BackgroundWorker Structure (Start) ---------------------
            //get our dispatcher
            System.Windows.Threading.Dispatcher pdDispatcher = _DWGRun.Dispatcher;

            BackgroundWorker worker = new BackgroundWorker();

            worker.DoWork += delegate(object s, DoWorkEventArgs args)
            {
                //Open each DWG
                foreach (DWGFileModel objDWG in ArCaDWGEdController.DWGCollection.LstDWG)
                {                
                    //Do Procedure
                    ArCaDWGEdController.DoProcedures(objDWG);

                    //create a new delegate for updating our DWGRun
                    UpdateProgressDelegate update = new UpdateProgressDelegate(UpdateDWGRun);

                    //invoke the dispatcher and pass the percentage and max record count
                    pdDispatcher.BeginInvoke(update, objDWG, ArCaDWGEdController.DWGCollection.LstDWG.IndexOf(objDWG) + 1);
                }
            };

            worker.RunWorkerCompleted += delegate(object s, RunWorkerCompletedEventArgs args)
            {
                _DWGRun.Close();
            };

            //run the process then show the progress dialog
            worker.RunWorkerAsync();
            //------------------  BackgroundWorker Structure (End) ---------------------
        }

        //------------------  BackgroundWorker Delegates (Start) ---------------------
        //our delegate used for updating the UI
        public delegate void UpdateProgressDelegate(DWGFileModel objDWG, int recordCount);

        //this is the method that the deleagte will execute
        public void UpdateDWGRun(DWGFileModel objDWG, int recordCount)
        {
            //Update Datacontext
            _DWGRun.MainGrid.DataContext = objDWG;
            //Increment progressbar
            _DWGRun.progressBar.Value = recordCount;
        }
        //------------------  BackgroundWorker Delegates (End) ---------------------
    }
}
