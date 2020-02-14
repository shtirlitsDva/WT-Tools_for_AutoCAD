using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Autodesk.AutoCAD.Runtime;
using Autodesk.Windows;
using ArCaManaged.RuntimeRibbon;
using Autodesk.AutoCAD.ApplicationServices;
using System.Threading;
using System.ComponentModel;
using Autodesk.AutoCAD.DatabaseServices;
using System.Runtime.InteropServices;
using System.IO;
using System.Reflection;

[assembly: ExtensionApplication(typeof(ArCaManaged.Initialize.Startup))]

namespace ArCaManaged.Initialize
{
    class Startup : IExtensionApplication
    {
        [DllImport("accore.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl, EntryPoint = "acedArxLoad")]
        private static extern int acedArxLoad(string app);
        private static SplashScreen _Splash;
        public void Initialize()
        {
            //SplashScreen
            ShowSplashScreen();
            //Ribbon
            ArCaRibbonMain.InitializeRibbon();

            string appPath = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string appDirCusntonEntity = Path.GetDirectoryName(appPath) + "\\ArCaArCaObjects.dbx";
            int iStatus1 = acedArxLoad(appDirCusntonEntity);            
        }

        private void ShowSplashScreen()
        {
            BackgroundWorker worker = new BackgroundWorker();
            _Splash = new SplashScreen();
            System.Windows.Threading.Dispatcher pdDispatcher = _Splash.Dispatcher;

            worker.DoWork += delegate(object s, DoWorkEventArgs args)
            {                
                //create a new delegate for updating our DWGRun
                UpdateProgressDelegate update = new UpdateProgressDelegate(UpdateDWGRun);

                //invoke the dispatcher and pass the percentage and max record count
                pdDispatcher.BeginInvoke(update, 2000);
            };

            worker.RunWorkerAsync();
            _Splash.ShowDialog();
        }

        //------------------  BackgroundWorker Delegates (Start) ---------------------
        //our delegate used for updating the UI
        public delegate void UpdateProgressDelegate(int recordCount);

        //this is the method that the deleagte will execute
        public void UpdateDWGRun(int iSleep)
        {
            Thread.Sleep(iSleep);
            _Splash.Close();
        }
        //------------------  BackgroundWorker Delegates (End) ---------------------

        public void Terminate()
        {

        }
    }

}
