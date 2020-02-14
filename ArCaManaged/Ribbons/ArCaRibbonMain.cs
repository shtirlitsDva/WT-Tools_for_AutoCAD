using System;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Windows;
using Autodesk.Windows;
using Autodesk.AutoCAD.EditorInput;
using System.Collections.Generic;
using System.Windows;
using System.Drawing;
using ArCaManaged.Utils;
using System.Windows.Media.Imaging;

[assembly: CommandClass(typeof(ArCaManaged.RuntimeRibbon.ArCaRibbonMain))]

namespace ArCaManaged.RuntimeRibbon
{
    class ArCaRibbonMain
    {
        private const String MY_TAB_ID = "MY_TAB_ID";

        static public void InitializeRibbon()
        {
            if (Autodesk.Windows.ComponentManager.Ribbon == null)
            {
                //load the custom Ribbon on startup, but at this point
                //the Ribbon control is not available, so register for
                //an event and wait
                Autodesk.Windows.ComponentManager.ItemInitialized +=
                    new EventHandler<RibbonItemEventArgs>(ComponentManager_ItemInitialized);
            }
            else
            {
                //the assembly was loaded using NETLOAD, so the ribbon
                //is available and we just create the ribbon
                createRibbon();
            }
        }

        static void ComponentManager_ItemInitialized(object sender, RibbonItemEventArgs e)
        {
            //now one Ribbon item is initialized, but the Ribbon control
            //may not be available yet, so check if before
            if (Autodesk.Windows.ComponentManager.Ribbon != null)
            {
                //ok, create Ribbon
                createRibbon();
                //and remove the event handler
                Autodesk.Windows.ComponentManager.ItemInitialized -=
                    new EventHandler<RibbonItemEventArgs>(ComponentManager_ItemInitialized);
            }
        }

        [CommandMethod("removeMyRibbon")]
        public void removeRibbon()
        {
            Autodesk.Windows.RibbonControl ribCntrl =
                Autodesk.AutoCAD.Ribbon.RibbonServices.RibbonPaletteSet.RibbonControl;
            //find the custom tab using the Id
            for (int i = 0; i < ribCntrl.Tabs.Count; i++)
            {
                if (ribCntrl.Tabs[i].Id.Equals(MY_TAB_ID))
                {
                    ribCntrl.Tabs.Remove(ribCntrl.Tabs[i]);
                    return;
                }
            }
        }

        [CommandMethod("addMyRibbon")]
        static public void createRibbon()
        {
            Autodesk.Windows.RibbonControl ribCntrl =
                Autodesk.AutoCAD.Ribbon.RibbonServices.RibbonPaletteSet.RibbonControl;
            //can also be Autodesk.Windows.ComponentManager.Ribbon;     

            //add the tab
            RibbonTab ribTab = new RibbonTab();
            ribTab.Title = "WT-Tools";
            ribTab.Id = MY_TAB_ID;
            ribCntrl.Tabs.Add(ribTab);

            //create and add both panels
            //addRegistry(ribTab);
            addRefMgr(ribTab);
            addWDGed(ribTab);
            addCusEnt(ribTab);            
            //addPanel2(ribTab);

            //set as active tab
            ribTab.IsActive = true;
        }
        static private void addRefMgr(RibbonTab ribTab)
        {
            //create the panel source
            Autodesk.Windows.RibbonPanelSource ribSourcePanel = new RibbonPanelSource();
            ribSourcePanel.Title = "References";
            //now the panel
            RibbonPanel ribPanel = new RibbonPanel();
            ribPanel.Source = ribSourcePanel;
            ribTab.Panels.Add(ribPanel);
            //create button Refm
            Autodesk.Windows.RibbonButton ribButton1 = new RibbonButton();
            ribButton1.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpI = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.RefMgr);
            ribButton1.LargeImage = bmpI;
            ribButton1.Image = bmpI;
            ribButton1.Size = RibbonItemSize.Large;
            ribButton1.Text = "Reference Manager";
            ribButton1.ShowText = true;
            ribButton1.CommandParameter = "refm ";
            ribButton1.ShowImage = true;
            ribButton1.CommandHandler = new AdskCommandHandler();
            ribSourcePanel.Items.Add(ribButton1);
        }
        static private void addWDGed(RibbonTab ribTab)
        {
            //create the panel source
            Autodesk.Windows.RibbonPanelSource ribSourcePanel = new RibbonPanelSource();
            ribSourcePanel.Title = "DWGs";
            //now the panel
            RibbonPanel ribPanel = new RibbonPanel();
            ribPanel.Source = ribSourcePanel;
            ribTab.Panels.Add(ribPanel);
            //create button Refm
            Autodesk.Windows.RibbonButton ribButton1 = new RibbonButton();
            ribButton1.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpI = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.dwged);
            ribButton1.LargeImage = bmpI;
            ribButton1.Image = bmpI;
            ribButton1.Size = RibbonItemSize.Large;
            ribButton1.Text = "DWG Editor";
            ribButton1.ShowText = true;
            ribButton1.CommandParameter = "dwged ";
            ribButton1.ShowImage = true;
            ribButton1.CommandHandler = new AdskCommandHandler();
            ribSourcePanel.Items.Add(ribButton1);
        }
        static private void addCusEnt(RibbonTab ribTab)
        {
            //create the panel source
            Autodesk.Windows.RibbonPanelSource ribSourcePanel = new RibbonPanelSource();
            ribSourcePanel.Title = "WT-Tools Entities";
            //now the panel
            RibbonPanel ribPanel = new RibbonPanel();
            ribPanel.Source = ribSourcePanel;
            ribTab.Panels.Add(ribPanel);

            //create button
            Autodesk.Windows.RibbonButton ribButtonGrating = new RibbonButton();
            ribButtonGrating.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpI = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.grating);
            ribButtonGrating.LargeImage = bmpI;
            ribButtonGrating.Image = bmpI;
            ribButtonGrating.Size = RibbonItemSize.Large;
            ribButtonGrating.Text = "Grating";
            ribButtonGrating.ShowText = true;
            ribButtonGrating.CommandParameter = "grating ";
            ribButtonGrating.CommandHandler = new AdskCommandHandler();

            //create button
            Autodesk.Windows.RibbonButton ribButtonRamp = new RibbonButton();
            ribButtonRamp.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpIRamp = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.Ramp);
            ribButtonRamp.LargeImage = bmpIRamp;
            ribButtonRamp.Image = bmpIRamp;
            ribButtonRamp.Size = RibbonItemSize.Large;
            ribButtonRamp.Text = "Ramp";
            ribButtonRamp.ShowText = true;
            ribButtonRamp.CommandParameter = "ramp ";
            ribButtonRamp.CommandHandler = new AdskCommandHandler();

            //create button
            //create a Ribbon List Button
            Autodesk.Windows.RibbonSplitButton ribListBtn = new RibbonSplitButton();

            Autodesk.Windows.RibbonButton ribButtonQuota = new RibbonButton();
            ribButtonQuota.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpIQuota = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.Quota);
            ribButtonQuota.LargeImage = bmpIQuota;
            ribButtonQuota.Image = bmpIQuota;
            ribButtonQuota.Size = RibbonItemSize.Large;
            ribButtonQuota.Text = "Arrow";
            ribButtonQuota.ShowText = true;
            ribButtonQuota.CommandParameter = "quota arrow ";
            ribButtonQuota.CommandHandler = new AdskCommandHandler();

            Autodesk.Windows.RibbonButton ribButtonQuota_Leader = new RibbonButton();
            ribButtonQuota_Leader.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpIQuota_Leader = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.Quota_Leader);
            ribButtonQuota_Leader.LargeImage = bmpIQuota_Leader;
            ribButtonQuota_Leader.Image = bmpIQuota;
            ribButtonQuota_Leader.Size = RibbonItemSize.Large;
            ribButtonQuota_Leader.Text = "Leader";
            ribButtonQuota_Leader.ShowText = true;
            ribButtonQuota_Leader.CommandParameter = "quota leader ";
            ribButtonQuota_Leader.CommandHandler = new AdskCommandHandler();

            Autodesk.Windows.RibbonButton ribButtonQuota_Plane = new RibbonButton();
            ribButtonQuota_Leader.Orientation = System.Windows.Controls.Orientation.Vertical;
            BitmapImage bmpIQuota_Plane = ArCaUtils.GetBitmapImage(ArCaManaged.Properties.Resources.Quota_Plane);
            ribButtonQuota_Plane.LargeImage = bmpIQuota_Plane;
            ribButtonQuota_Plane.Image = bmpIQuota;
            ribButtonQuota_Plane.Size = RibbonItemSize.Large;
            ribButtonQuota_Plane.Text = "Plane";
            ribButtonQuota_Plane.ShowText = true;
            ribButtonQuota_Plane.CommandParameter = "quota plane ";
            ribButtonQuota_Plane.CommandHandler = new AdskCommandHandler();

            ribListBtn.Text = "Quota";
            ribListBtn.Size = RibbonItemSize.Large;
            ribListBtn.ShowText = true;
            ribListBtn.Items.Add(ribButtonQuota);
            ribListBtn.Items.Add(ribButtonQuota_Leader);
            ribListBtn.Items.Add(ribButtonQuota_Plane);

            ribSourcePanel.Items.Add(ribButtonGrating);
            ribSourcePanel.Items.Add(ribButtonRamp);
            ribSourcePanel.Items.Add(ribListBtn);
        }
        //Only an Example
        static private void addRegistry(RibbonTab ribTab)
        {
            //create the panel source
            Autodesk.Windows.RibbonPanelSource ribSourcePanel = new RibbonPanelSource();
            ribSourcePanel.Title = "Edit registry";
            //now the panel
            RibbonPanel ribPanel = new RibbonPanel();
            ribPanel.Source = ribSourcePanel;
            ribTab.Panels.Add(ribPanel);

            //create button 1
            Autodesk.Windows.RibbonButton ribButton1 = new RibbonButton();
            ribButton1.Text = "Register WT-CAD";
            ribButton1.CommandParameter = "REGISTERME ";
            ribButton1.ShowText = true;
            ribButton1.CommandHandler = new AdskCommandHandler();
            //now create button 2
            Autodesk.Windows.RibbonButton ribButton2 = new RibbonButton();
            ribButton2.Text = "Unregister WT-CAD";
            ribButton2.CommandParameter = "UNREGISTERME ";
            ribButton2.ShowText = true;
            ribButton2.CommandHandler = new AdskCommandHandler();

            //create a tooltip
            Autodesk.Windows.RibbonToolTip ribToolTip = new RibbonToolTip();
            ribToolTip.Command = "REGISTER WT-CAD";
            ribToolTip.Title = "Register WT-CAD command";
            ribToolTip.Content = "Register WT-CAD plugn on AutoCAD startup";
            ribToolTip.ExpandedContent = "This commands adds or remove CAD-WT plugin in the list of registered plugins AutoCAD, When the command \"Register WT-CAD\" is activated, AutoCAD will start the CAD-WT when AutoCAD starts, and the command \"Unregister WT-CAD\" undo this initialization.";
            ribButton1.ToolTip = ribToolTip;

            //now add the 2 button with a RowBreak
            ribSourcePanel.Items.Add(ribButton1);
            ribSourcePanel.Items.Add(new RibbonRowBreak());
            ribSourcePanel.Items.Add(ribButton2);

            //now add and expanded panel (with 1 button
            Autodesk.Windows.RibbonRowPanel ribExpPanel = new RibbonRowPanel();
            Autodesk.Windows.RibbonButton ribExpButton1 = new RibbonButton();
            ribExpButton1.Text = "On expanded panel";
            ribExpButton1.ShowText = true;
            ribExpPanel.Items.Add(ribExpButton1);
            //and add it to source
            ribSourcePanel.Items.Add(new RibbonPanelBreak());
            ribSourcePanel.Items.Add(ribExpPanel);
        }

        //Only an Example
        private static void addPanel2(RibbonTab ribTab)
        {
            //create the panel source
            Autodesk.Windows.RibbonPanelSource ribSourcePanel = new RibbonPanelSource();
            ribSourcePanel.Title = "Controls";
            //now the panel
            RibbonPanel ribPanel = new RibbonPanel();
            ribPanel.Source = ribSourcePanel;
            ribTab.Panels.Add(ribPanel);

            //create a Ribbon text
            Autodesk.Windows.RibbonTextBox ribTxt = new RibbonTextBox();
            ribTxt.Width = 100;
            ribTxt.IsEmptyTextValid = false;
            ribTxt.InvokesCommand = true;
            ribTxt.CommandHandler = new AdskCommandHandler();

            //create a Ribbon List Button
            Autodesk.Windows.RibbonSplitButton ribListBtn = new RibbonSplitButton();
            Autodesk.Windows.RibbonButton ribButton1 = new RibbonButton();
            ribButton1.Text = "Call LINE command";
            ribButton1.ShowText = true;
            ribButton1.CommandParameter = "LINE ";
            ribButton1.CommandHandler = new AdskCommandHandler();
            Autodesk.Windows.RibbonButton ribButton2 = new RibbonButton();
            ribButton2.Text = "Call CIRCLE command";
            ribButton2.ShowText = true;
            ribButton2.CommandParameter = "CIRCLE ";
            ribButton2.CommandHandler = new AdskCommandHandler();
            ribListBtn.Text = "Some options";
            ribListBtn.ShowText = true;
            ribListBtn.Items.Add(ribButton1);
            ribListBtn.Items.Add(ribButton2);

            ribSourcePanel.Items.Add(ribTxt);
            ribSourcePanel.Items.Add(new RibbonRowBreak());
            ribSourcePanel.Items.Add(ribListBtn);
        }

        [CommandMethod("registerMe")]
        static public void RegisterMe()
        {
            //AutoCAD (or vertical) and Application keys
            Microsoft.Win32.RegistryKey acadKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(Autodesk.AutoCAD.DatabaseServices.HostApplicationServices.Current.MachineRegistryProductRootKey);
            Microsoft.Win32.RegistryKey acadAppKey = acadKey.OpenSubKey("Applications", true);

            //already registered?
            string curAssemblyName = System.Reflection.Assembly.GetExecutingAssembly().GetName().Name;
            String[] subKeys = acadAppKey.GetSubKeyNames();
            foreach (String subKey in subKeys)
                if (subKey.Equals(curAssemblyName))
                {
                    acadAppKey.Close();
                    return;
                }

            //assembly location and description (as full name)
            string curAssemblyPath = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string curAssemblyFullName = System.Reflection.Assembly.GetExecutingAssembly().GetName().FullName;

            //create the addin key
            Microsoft.Win32.RegistryKey acadAppAddInKey = acadAppKey.CreateSubKey(curAssemblyName);
            acadAppAddInKey.SetValue("DESCRIPTION", curAssemblyFullName, Microsoft.Win32.RegistryValueKind.String);
            acadAppAddInKey.SetValue("LOADCTRLS", 14, Microsoft.Win32.RegistryValueKind.DWord);
            acadAppAddInKey.SetValue("LOADER", curAssemblyPath, Microsoft.Win32.RegistryValueKind.String);
            acadAppAddInKey.SetValue("MANAGED", 1, Microsoft.Win32.RegistryValueKind.DWord);

            acadAppKey.Close();
        }

        [CommandMethod("UnregisterMe")]
        static public void UnregisterMe()
        {
            //AutoCAD (or vertical) and Application keys
            Microsoft.Win32.RegistryKey acadKey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(Autodesk.AutoCAD.DatabaseServices.HostApplicationServices.Current.MachineRegistryProductRootKey);
            Microsoft.Win32.RegistryKey acadAppKey = acadKey.OpenSubKey("Applications", true);

            //get assembly name and delete
            string curAssemblyName = System.Reflection.Assembly.GetExecutingAssembly().GetName().Name;
            acadAppKey.DeleteSubKeyTree(curAssemblyName);
            acadAppKey.Close();
        }
    }

    public class AdskCommandHandler : System.Windows.Input.ICommand
    {
        public bool CanExecute(object parameter)
        {
            return true;
        }

        public event EventHandler CanExecuteChanged;

        public void Execute(object parameter)
        {
            //is from a Ribbon Button?
            RibbonButton ribBtn = parameter as RibbonButton;
            if (ribBtn != null)
                Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.SendStringToExecute((String)ribBtn.CommandParameter, true, false, true);

            //is from s Ribbon Textbox?
            RibbonTextBox ribTxt = parameter as RibbonTextBox;
            if (ribTxt != null)
                MessageBox.Show(ribTxt.TextValue);
        }
    }
}
