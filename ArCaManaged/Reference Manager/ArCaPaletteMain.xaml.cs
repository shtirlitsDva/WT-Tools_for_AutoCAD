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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Autodesk.AutoCAD.ApplicationServices;
using ArCaManaged.Reference_Manager.Model;
using ArCaManaged.Reference_Manager.Controller;
using ArCaManaged.Reference_Manager.View;
using ArCaManaged.Utils;
using System.IO;
using System.Windows.Controls.Primitives;
using Autodesk.AutoCAD.DatabaseServices;
using ArCaManaged.Commands;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Windows;
using System.Threading;
using System.ComponentModel;

namespace ArCaManaged.Reference_Manager.View
{
    /// <summary>
    /// Interaction logic for ArCaPaletteMain.xaml
    /// </summary>
    public partial class ArCaPaletteMain : UserControl
    {
        public ArCaPaletteMain()
        {
            InitializeComponent();
        }

        private void AddBlock_Click(object sender, RoutedEventArgs e)
        {
            BlockModel NewBlock = new BlockModel();
            NewBlock.LstBlockProp.Add(new BlockPropModel("Default"));
            ArCaBlockView blockForm = new ArCaBlockView(NewBlock, true);
            Autodesk.AutoCAD.ApplicationServices.Application.ShowModalWindow(blockForm);
        }

        private void DelBlock_Click(object sender, RoutedEventArgs e)
        {
            //Check if there any selected block in lvBlocks
            if (lvBlocks.SelectedIndex == -1)
            {
                MessageBox.Show("Select one block first.", "Delete Block", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }
 
            BlockModel objBlock = lvBlocks.SelectedValue as BlockModel;

            //Ask to user
            MessageBoxResult result = MessageBox.Show("Are you sure to delete the block \"" + objBlock.Name + "\".", "Delete Block", MessageBoxButton.YesNo, MessageBoxImage.Exclamation);
            if (result == MessageBoxResult.No)
                return;

            //Delet BlockModel
            BlockGroupModel objBlockGroup = cbMain.SelectedValue as BlockGroupModel;            
            objBlockGroup.LstBlock.Remove(objBlock);

            //Delet BlockDWG
            using (Database dbRef = new Database(false, true))
            {
                // Read the DWG
                dbRef.ReadDwgFile(ArCaRefMgrController.DBPath, FileShare.ReadWrite, true, "");
                using (Transaction trRef = dbRef.TransactionManager.StartTransaction())
                {
                    BlockTable bt = trRef.GetObject(dbRef.BlockTableId, OpenMode.ForRead) as BlockTable;
                    DBObject obj = trRef.GetObject(bt[objBlock.Name], OpenMode.ForWrite);
                    obj.Erase();
                    trRef.Commit();
                }
                dbRef.SaveAs(ArCaRefMgrController.DBPath, DwgVersion.Current);
            }

            //Write XML
            ArCaRefMgrParser.WriteXML();

            //MessageBox.Show("Block \"" + objBlock.Name + "\" was successfully deleted.", "Block Deleted", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void EdtBlock_Click(object sender, RoutedEventArgs e)
        {
            //Check if there any selected block in lvBlocks
            if (lvBlocks.SelectedIndex == -1)
            {
                MessageBox.Show("Select one block first.", "Edit Block", MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }
            //Edit BlockModel
            BlockModel objBlock = lvBlocks.SelectedValue as BlockModel;
            ArCaBlockView blockForm = new ArCaBlockView(objBlock, false);
            Autodesk.AutoCAD.ApplicationServices.Application.ShowModalWindow(blockForm);
        }

        private void lvBlocks_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            //verifica se o duplo clique foi no scroll bar
            ScrollBar objScroll = ArCaUtils.FindAncestor<ScrollBar>(e.OriginalSource as DependencyObject);
            if (objScroll != null)
                return; //clicou no scroll - não faz nada

            //localiza o item selecionado
            ListViewItem objItem = ArCaUtils.FindAncestor<ListViewItem>(e.OriginalSource as DependencyObject);
            if (objItem == null)
                return;

            //Get BlockModel
            BlockModel objBlockModel = objItem.DataContext as BlockModel;

            //TODO--------Remover in the future-------------
            //Gambiarra pra resolver o Problema com o Focus da Pallet
            Editor ed = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument.Editor;
            if (!ArCaCommands._PalletRefMgr.Dock.Equals(DockSides.None))
                ed.GetPoint("");
            //----------------------------------------------

            //Insert Block
            ArCaCommands.InsestBlock(objBlockModel);
        }
    }
}
