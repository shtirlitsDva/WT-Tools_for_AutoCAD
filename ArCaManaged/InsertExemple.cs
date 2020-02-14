using System;
using System.IO;
using System.Collections.Generic;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.Windows;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;

[assembly: CommandClass(typeof(ArCaManaged.Commands.ArCaCommands2))]

namespace ArCaManaged.Commands
{
    /// <summary>
    /// Summary description for ArCaCommands.
    /// </summary>
    public class ArCaCommands2
    {
        #region Attributes

        static PointMonitorStrategy _PointMonitorStrategy;

        private static string _sLastBlock = string.Empty;

        #endregion

        public ArCaCommands2()
        {
        }
        
        public static ObjectId GetBlockId(string sBlockName)
        {
            using (Database dbLocal = Application.DocumentManager.MdiActiveDocument.Database)
            {
                using (Transaction trLocal = dbLocal.TransactionManager.StartTransaction())
                {
                    //abrir tabela de blocos
                    BlockTable bt = trLocal.GetObject(dbLocal.BlockTableId, OpenMode.ForRead) as BlockTable;

                    //verificar se o bloco já existe
                    if (bt.Has(sBlockName))
                    {
                        trLocal.Commit();
                        return bt[sBlockName];
                    }
                    else
                    {
                        //Não existe, retorna null
                        trLocal.Commit();
                        return ObjectId.Null;
                    }
                }
            }
        }

        [CommandMethod("insBlock")]
        static public void CmdInsertBlock()
        {
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;
            Database db = Application.DocumentManager.MdiActiveDocument.Database;

            PromptStringOptions optName = new PromptStringOptions("\nEnter the name of the block: ");
            optName.AllowSpaces = true;

            if (_sLastBlock == string.Empty)
            {
                optName.UseDefaultValue = true;
                optName.DefaultValue = _sLastBlock;
            }

            //Loop
            do
            {
                //Esk the user by block name
                PromptResult ProNameRes = ed.GetString(optName);

                if (ProNameRes.Status == PromptStatus.Cancel)
                    break;//Command canceled, go out of loop
                else
                {
                    //Get objId
                    ObjectId objId = GetBlockId(ProNameRes.StringResult);

                    if(objId.IsValid)
                    {
                        //Create BlockReference
                        BlockReference blockRef = new BlockReference(new Point3d(), objId);

                        //Strategy for Preview in PointMonitor
                        _PointMonitorStrategy = new PointMonitorStrategy(delegate(object sender, PointMonitorEventArgs e)
                        {
                            try
                            {
                                //Refresh gost block.
                                blockRef.Position = e.Context.ComputedPoint;
                                e.Context.DrawContext.Geometry.Draw(blockRef);
                            }
                            catch (System.Exception ex)
                            {
                            }
                        });

                        
                        //add pointMonitor
                        ed.PointMonitor += new PointMonitorEventHandler(PointMonitor);

                        //Get Second point
                        PromptPointOptions opcoesPonto = new PromptPointOptions("\nClick on the place where block will be inserted: ");
                        opcoesPonto.AllowNone = false;

                        PromptPointResult ProPtRes = ed.GetPoint(opcoesPonto);

                        //remove pointMonitor
                        ed.PointMonitor -= new PointMonitorEventHandler(PointMonitor);

                        //Definitive insertion
                        switch (ProPtRes.Status)
                        {
                            case PromptStatus.OK:
                                //Create Grating
                                using (Transaction trans = db.TransactionManager.StartTransaction())
                                {
                                    try
                                    {
                                        //Set Position
                                        blockRef.Position = ProPtRes.Value;
                                        //We need clone the block reference to obtain a block wiht other objectID in each iteration
                                        BlockReference blockRefClone = blockRef.Clone() as BlockReference;                                        
                                        //Get block table records
                                        BlockTableRecord currSpace = trans.GetObject(db.CurrentSpaceId, OpenMode.ForWrite) as BlockTableRecord;
                                        //Insert Block
                                        currSpace.AppendEntity(blockRefClone);
                                        trans.AddNewlyCreatedDBObject(blockRefClone, true);
                                        trans.Commit();
                                    }
                                    catch (System.Exception ex)
                                    {
                                        trans.Abort();
                                    }
                                }
                                break;
                            case PromptStatus.Cancel:
                                //Command Canceled
                                break;
                        }

                        //Save the name of inserted block
                        _sLastBlock = ProNameRes.StringResult;

                        //Go out of Loop
                        break;
                    }
                    else
                        ed.WriteMessage("\nNot foud any block with name \"" + ProNameRes.StringResult + "\".");
                }
            }while(true);           
        }

        //PointMonitor Event
        static void PointMonitor(object sender, PointMonitorEventArgs e)
        {
            //Do Strategy
            _PointMonitorStrategy.Run(sender, e);
        }

        //PointMonitor Strategy
        internal class PointMonitorStrategy
        {
            public delegate void CommandProcedureRunMethod(object sender, PointMonitorEventArgs e);
            private CommandProcedureRunMethod _evRun;

            public PointMonitorStrategy(CommandProcedureRunMethod evRun)
            {
                _evRun = evRun;
            }

            public void Run(object sender, PointMonitorEventArgs e)
            {
                _evRun.Invoke(sender, e);
            }
        }
    }
}