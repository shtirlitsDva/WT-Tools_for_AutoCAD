using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml;
using System.Collections.ObjectModel;
using ArCaManaged.Reference_Manager.Model;

namespace ArCaManaged.Reference_Manager.Controller
{
    class ArCaRefMgrParser
    {
        #region XML Nodes

        // CoCreX nodes
        private const String CONST_S_NODE_ARCA = "ArCa";
        private const String CONST_S_NODE_REFMGR = "ReferenceManager";
        private const String CONST_S_NODE_BLOCKS = "Blocks";
        private const String CONST_S_NODE_BLOCKGROUP = "BlockGroup";
        private const String CONST_S_NODE_BLOCK = "Block";
        private const String CONST_S_NODE_BLOCK_PROPERTY = "BlockProperty";
        

        private const String CONST_S_ATT_NAME = "Name";
        private const String CONST_S_ATT_DESCRIPTION = "Description";

        //Insertions
        private const String CONST_S_ATT_ROTATE = "Rotate";
        private const String CONST_S_ATT_ANGLE = "Angle";

        private const String CONST_S_ATT_SCALE = "Scale";
        private const String CONST_S_ATT_SCLX = "SclX";
        private const String CONST_S_ATT_SCLY = "SclY";
        private const String CONST_S_ATT_SCLZ = "SclZ";

        private const String CONST_S_ATT_MIRROR = "Mirror";
        private const String CONST_S_ATT_LOOP = "Loop";
        private const String CONST_S_ATT_EXPLODE = "Explode";
        private const String CONST_S_ATT_MATRIX = "Matrix";
        private const String CONST_S_ATT_MATRIXWIDTH = "MatrixWidth";
        private const String CONST_S_ATT_MATRIXHEIGHT = "MatrixHeight";
        private const String CONST_S_ATT_MATRIXMAXQUANT = "MatrixMaxQuant";

        #endregion

        #region XML Reader

        public static void ReadXML(ref MainModel objMain)
        {

            Stream Str = GetSream(ArCaRefMgrController.AppConfig);

            XmlReaderSettings objReaderConfig = new XmlReaderSettings();
            objReaderConfig.IgnoreComments = true;
            objReaderConfig.IgnoreWhitespace = true;
            objReaderConfig.IgnoreProcessingInstructions = true;
            XmlReader objReader = XmlReader.Create(Str, objReaderConfig);
            while (objReader.Read() && objReader.NodeType != XmlNodeType.Element) ; // move to the first element of this XML

            if (!objReader.Name.Equals(CONST_S_NODE_ARCA, StringComparison.CurrentCultureIgnoreCase))
                throw new Exception("Invalid XML file or incorrect version");

            if (!objReader.IsEmptyElement)
            {
                while (objReader.Read())
                {
                    //if it's the end Element
                    if (objReader.NodeType == XmlNodeType.EndElement && objReader.Name.Equals(CONST_S_NODE_ARCA, StringComparison.CurrentCultureIgnoreCase))
                        break;

                    switch (objReader.Name)
                    {
                        //case is tag Entity
                        case CONST_S_NODE_REFMGR:
                            ReadRefMgr(objReader, ref objMain);
                            break;

                        default:
                            throw new Exception("Unexpected element: '" + objReader.Name + "'");
                    }
                }
            }
            objReader.Close();
        }

        private static void ReadRefMgr(XmlReader objReader, ref MainModel objMain)
        {
            if (!objReader.IsEmptyElement)
            {
                //set product properties
                while (objReader.Read())
                {
                    //if it's the end Element
                    if (objReader.NodeType == XmlNodeType.EndElement && objReader.Name.Equals(CONST_S_NODE_REFMGR, StringComparison.CurrentCultureIgnoreCase))
                        break;

                    if (objReader.Name.Equals(CONST_S_NODE_BLOCKS, StringComparison.CurrentCultureIgnoreCase))
                        ReadBlocks(objReader, ref objMain);
                    else
                        throw new Exception("Unexpected element: '" + objReader.Name + "'");

                }
            }
        }

        private static void ReadBlocks(XmlReader objReader, ref MainModel objMain)
        {
            if (!objReader.IsEmptyElement)
            {
                //set product properties
                while (objReader.Read())
                {
                    //if it's the end Element
                    if (objReader.NodeType == XmlNodeType.EndElement && objReader.Name.Equals(CONST_S_NODE_BLOCKS, StringComparison.CurrentCultureIgnoreCase))
                        break;

                    if (objReader.Name.Equals(CONST_S_NODE_BLOCKGROUP, StringComparison.CurrentCultureIgnoreCase))
                    {
                        BlockGroupModel objBGroup = new BlockGroupModel(objReader.GetAttribute(CONST_S_ATT_NAME));
                        objMain.LstGroup.Add(objBGroup);
                        ReadBlockGroup(objReader, ref objMain);
                    }
                    else
                        throw new Exception("Unexpected element: '" + objReader.Name + "'");

                }
            }
        }

        private static void ReadBlockGroup(XmlReader objReader, ref MainModel objMain)
        {
            if (!objReader.IsEmptyElement)
            {
                //set product properties
                while (objReader.Read())
                {
                    //if it's the end Element
                    if (objReader.NodeType == XmlNodeType.EndElement && objReader.Name.Equals(CONST_S_NODE_BLOCKGROUP, StringComparison.CurrentCultureIgnoreCase))
                        break;

                    if (objReader.Name.Equals(CONST_S_NODE_BLOCK, StringComparison.CurrentCultureIgnoreCase))
                    {
                        string sName = objReader.GetAttribute(CONST_S_ATT_NAME);
                        string sDescription = objReader.GetAttribute(CONST_S_ATT_DESCRIPTION);
                        BlockModel objBlock = new BlockModel(sName, sDescription);

                        objBlock.btmImage = ArCaRefMgrController.GetThumbnail(sName);
                        objMain.LstGroup[objMain.LstGroup.Count - 1].LstBlock.Add(objBlock);
                        ReadBlock(objReader, ref objMain, ref objBlock);
                    }
                    else
                        throw new Exception("Unexpected element: '" + objReader.Name + "'");

                }
            }
        }

        private static void ReadBlock(XmlReader objReader, ref MainModel objMain, ref BlockModel objBlock)
        {
            if (!objReader.IsEmptyElement)
            {
                //set product properties
                while (objReader.Read())
                {
                    //if it's the end Element
                    if (objReader.NodeType == XmlNodeType.EndElement && objReader.Name.Equals(CONST_S_NODE_BLOCK, StringComparison.CurrentCultureIgnoreCase))
                        break;

                    if (objReader.Name.Equals(CONST_S_NODE_BLOCK_PROPERTY, StringComparison.CurrentCultureIgnoreCase))
                    {
                        BlockPropModel objBlockProp = new BlockPropModel(objReader.GetAttribute(CONST_S_ATT_NAME));

                        //Insertions
                        objBlockProp.Rotate = System.Convert.ToBoolean(objReader.GetAttribute(CONST_S_ATT_ROTATE));
                        objBlockProp.Angle = double.Parse(objReader.GetAttribute(CONST_S_ATT_ANGLE));

                        objBlockProp.Scale = System.Convert.ToBoolean(objReader.GetAttribute(CONST_S_ATT_SCALE));
                        objBlockProp.SclX = double.Parse(objReader.GetAttribute(CONST_S_ATT_SCLX));
                        objBlockProp.SclY = double.Parse(objReader.GetAttribute(CONST_S_ATT_SCLY));
                        objBlockProp.SclZ = double.Parse(objReader.GetAttribute(CONST_S_ATT_SCLZ));

                        objBlockProp.Mirror = System.Convert.ToBoolean(objReader.GetAttribute(CONST_S_ATT_MIRROR));
                        objBlockProp.Loop = System.Convert.ToBoolean(objReader.GetAttribute(CONST_S_ATT_LOOP));
                        objBlockProp.Explode = System.Convert.ToBoolean(objReader.GetAttribute(CONST_S_ATT_EXPLODE));

                        objBlockProp.Matrix = System.Convert.ToBoolean(objReader.GetAttribute(CONST_S_ATT_MATRIX));
                        objBlockProp.MatrixWidth = double.Parse(objReader.GetAttribute(CONST_S_ATT_MATRIXWIDTH));
                        objBlockProp.MatrixHeight = double.Parse(objReader.GetAttribute(CONST_S_ATT_MATRIXHEIGHT));
                        objBlockProp.MatrixMaxQuant = double.Parse(objReader.GetAttribute(CONST_S_ATT_MATRIXMAXQUANT));
                        //Add
                        objBlock.LstBlockProp.Add(objBlockProp);
                    }
                    else
                        throw new Exception("Unexpected element: '" + objReader.Name + "'");

                }
            }
        }

        private static Stream GetSream(String sFileName)
        {
            StreamReader reader = new StreamReader(sFileName, Encoding.GetEncoding("iso8859-1"));
            string content = reader.ReadToEnd();
            reader.Close();
            return new MemoryStream(Encoding.Default.GetBytes(content));
        }

        #endregion

        #region XML Writer

        public static void WriteXML()
        {
            try
            {
                XmlWriterSettings XMLsetings = new XmlWriterSettings();
                XMLsetings.Encoding = Encoding.GetEncoding("iso-8859-9");
                using (XmlWriter writer = XmlWriter.Create(ArCaRefMgrController.AppConfig,XMLsetings))
                {
                    //Start XML
                    writer.WriteStartDocument();
                    //Start First Tag "ArCa"
                    writer.WriteStartElement(CONST_S_NODE_ARCA);
                    //Start Tag ReferenceManager
                    writer.WriteStartElement(CONST_S_NODE_REFMGR);
                    //Start Tag Blocks
                    writer.WriteStartElement(CONST_S_NODE_BLOCKS);
                    foreach (BlockGroupModel objGroup in ArCaRefMgrController.ObjMain.LstGroup)
                    {
                        //Start Tag BlockGroup
                        writer.WriteStartElement(CONST_S_NODE_BLOCKGROUP);
                        writer.WriteAttributeString(CONST_S_ATT_NAME, objGroup.Name);

                        foreach (BlockModel objBlock in objGroup.LstBlock)
                        {
                            //Start Tag Block
                            writer.WriteStartElement(CONST_S_NODE_BLOCK);
                            writer.WriteAttributeString(CONST_S_ATT_NAME, objBlock.Name);
                            writer.WriteAttributeString(CONST_S_ATT_DESCRIPTION, objBlock.Description);
                            //Start Tag BlockProperty
                            foreach (BlockPropModel objBlockProp in objBlock.LstBlockProp)
                            {
                                writer.WriteStartElement(CONST_S_NODE_BLOCK_PROPERTY);
                                writer.WriteAttributeString(CONST_S_ATT_ROTATE, objBlockProp.Rotate.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_ANGLE, objBlockProp.Angle.ToString());

                                writer.WriteAttributeString(CONST_S_ATT_SCALE, objBlockProp.Scale.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_SCLX, objBlockProp.SclX.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_SCLY, objBlockProp.SclY.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_SCLZ, objBlockProp.SclZ.ToString());

                                writer.WriteAttributeString(CONST_S_ATT_MIRROR, objBlockProp.Mirror.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_LOOP, objBlockProp.Loop.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_EXPLODE, objBlockProp.Explode.ToString());

                                writer.WriteAttributeString(CONST_S_ATT_MATRIX, objBlockProp.Matrix.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_MATRIXWIDTH, objBlockProp.MatrixWidth.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_MATRIXHEIGHT, objBlockProp.MatrixHeight.ToString());
                                writer.WriteAttributeString(CONST_S_ATT_MATRIXMAXQUANT, objBlockProp.MatrixMaxQuant.ToString());
                                //End Tag BlockProperty
                                writer.WriteEndElement();
                            }
                            //End Tag Block
                            writer.WriteEndElement();
                        }
                        //End Tag BlockGroup
                        writer.WriteEndElement();
                    }
                    //End Tag Blocks
                    writer.WriteEndElement();
                    //End Tag ReferenceManager
                    writer.WriteEndElement();
                    //End Tag ArCa
                    writer.WriteEndElement();
                    //End XML
                    writer.WriteEndDocument();
                }
            }
            catch (System.Exception ex)
            {
                /*MessageBox.Show(string.Format("O seguinte erro foi encontrado no arquivo selecionado: \n{0}", ex.Message), "Arquivo invalido", MessageBoxButton.OK, MessageBoxImage.Error);*/
            }
        }
        #endregion
    }
}
