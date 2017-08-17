using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;

namespace UpdateXml
{
    class XmlRecordInfo
    {
        public List<String> m_List_LineText = new List<String>();
        public List<String> m_List_LineName = new List<String>();
        public List<String> m_List_LineValue = new List<String>();
        public List<String> m_List_Node_Depth_Num = new List<String>();
    };

    class Program
    {
        #region Xml转AttrLine
        static void ReadNodeAttr(XmlRecordInfo xXmlRecordInfo, XmlElement xXmlNode, String xFullPathKV, Int32 xNodeDepth, ref Int32 xNodeNum, Int32 xParentNodeIndex)
        {
            if (xXmlNode != null)
            {
                for (Int32 i = 0; i < xXmlNode.Attributes.Count; i++)
                {
                    XmlAttribute xXmlAttribute = xXmlNode.Attributes[i];
                    xXmlRecordInfo.m_List_Node_Depth_Num.Add(String.Format("{0:D4}:{1:D4}", xNodeDepth, xNodeNum));
                    String xTempNodeFullPathKV = xFullPathKV;
                    xTempNodeFullPathKV += ">";
                    xTempNodeFullPathKV += xXmlAttribute.Name;
                    xXmlRecordInfo.m_List_LineName.Add(xTempNodeFullPathKV);
                    xTempNodeFullPathKV += "=";
                    xTempNodeFullPathKV += xXmlAttribute.Value;
                    xXmlRecordInfo.m_List_LineValue.Add(xXmlAttribute.Value);
                    xXmlRecordInfo.m_List_LineText.Add(xTempNodeFullPathKV);
                }
            }
        }

        static void ReadNode(XmlRecordInfo xXmlRecordInfo, XmlElement xXmlNode, String xFullPathKV, Int32 xNodeDepth, ref Int32 xNodeNum, Int32 xParentNodeIndex)
        {
            xNodeNum++;
            if (xXmlNode != null)
            {
                String xTempNodeFullPathKV = xFullPathKV;
                if (xNodeDepth != 0) { xTempNodeFullPathKV += ">"; }
                xTempNodeFullPathKV += xXmlNode.Name;

                ReadNodeAttr(xXmlRecordInfo, xXmlNode, xTempNodeFullPathKV, xNodeDepth + 1, ref xNodeNum, xNodeNum - 1);

                XmlElement xSubXmlElement = (XmlElement)xXmlNode.FirstChild;
                //TiXmlElement* xTiSubXmlElement = xTiXmlElement->FirstChildElement();
                while (xSubXmlElement != null)
                {
                    ReadNode(xXmlRecordInfo, xSubXmlElement, xTempNodeFullPathKV, xNodeDepth + 1, ref xNodeNum, xNodeNum - 1);
                    xSubXmlElement = (XmlElement)xSubXmlElement.NextSibling;
                    //xTiSubXmlElement = xTiSubXmlElement->NextSiblingElement();
                }
            }
        }

        static List<String> ReadXmlToLineAttr(String xXmlFileName)
        {
            XmlDocument xXmlDocument = new XmlDocument();
            xXmlDocument.Load(xXmlFileName);
            return ReadXmlToLineAttr(xXmlDocument);
        }

        static List<String> ReadXmlToLineAttr(XmlDocument xXmlDocument)
        {
            XmlRecordInfo xXmlRecordInfo = new XmlRecordInfo();
            XmlNode xXmlNode = xXmlDocument.FirstChild;
            if (xXmlNode.NodeType != XmlNodeType.Element) { xXmlNode = xXmlNode.NextSibling; }
            XmlElement xXmlElement = (XmlElement)xXmlNode;
            if (xXmlElement != null)
            {
                Int32 xNodeNum = 0;
                do
                {
                    String xFullPathKV = "";
                    ReadNode(xXmlRecordInfo, xXmlElement, xFullPathKV, 0, ref xNodeNum, 0);
                    xXmlElement = (XmlElement)xXmlElement.NextSibling;
                } while (xXmlElement != null);
            }
            return xXmlRecordInfo.m_List_LineText;
        }
        #endregion

        #region AttrLine文件升级
        static void AttrLineUp(String xSrcFile, String xUpFile, String xOutFile)
        {
            File.Delete(xOutFile);
            String[] xSrcAttrLines = ReadXmlToLineAttr(xSrcFile).ToArray();
            String[] xUpLines = File.ReadAllLines(xUpFile);
            Char xFlag = ' ';
            String xText = "";
            for (Int32 i = 0; i < xUpLines.Length; i++)
            {
                xFlag = ' ';
                if (xUpLines[i].Length == 0)
                {
                    if (i < xSrcAttrLines.Length)
                    {
                        File.AppendAllText(xOutFile, String.Format("{0}\n", xSrcAttrLines[i]));
                    }
                }
                else
                {
                    xFlag = xUpLines[i][0];
                    xText = xUpLines[i].Substring(1);
                    //sscanf(szBuff, "%c%s\n", &xFlag, szText);
                }
                switch (xFlag)
                {
                    // 增加
                    case 'A':
                        {
                            File.AppendAllText(xOutFile, String.Format("{0}\n", xText));
                            //fprintf(xFile, "%s\n", szText);
                        }
                        break;

                    // 修改
                    case 'C':
                        {
                            xFlag = xUpLines[i].Split('=')[0][0];
                            Int32 xSrcLine = Int32.Parse(xUpLines[i].Split('=')[0].Substring(1));
                            xText = xUpLines[i].Split('=')[1];
                            File.AppendAllText(xOutFile, String.Format("{0}={1}\n", xSrcAttrLines[xSrcLine].Split('=')[0], xText));
                            //sscanf(szBuff, "C%d=%s\n", &xSrcLine, szText);
                            //fprintf(xFile, "%s=%s\n", XmlInfoSrc.m_List_LineName[xSrcLine].c_str(), szText);
                        }
                        break;

                    // 移动
                    case 'M':
                        {
                            Int32 xSrcLine = Int32.Parse(xUpLines[i].Substring(1));
                            File.AppendAllText(xOutFile, String.Format("{0}\n", xSrcAttrLines[xSrcLine]));
                            //sscanf(szBuff, "M%d\n", &xSrcLine);
                            //fprintf(xFile, "%s\n", XmlInfoSrc.m_List_LineText[xSrcLine].c_str());
                        }
                        break;

                    // 结束
                    case 'E': { } break;
                }
                if (xFlag == 'E') { break; }
            }
        }
        #endregion


        static void Main(string[] args)
        {
            AttrLineUp(@"./test1.xml", @"./up.txt", @"./out.txt");

            GC.Collect();

            XmlDocument xOutXmlDocument = new XmlDocument();
            String[] xStreamOutLines = File.ReadAllLines(@"./out.txt");
            String[] xStreamNodeLines = File.ReadAllLines(@"./node.txt");
            String[] xTemps = xStreamOutLines[0].Split('=');
            String xRootNodeName = xTemps[0].Split('>')[0];

            xOutXmlDocument.AppendChild(xOutXmlDocument.CreateElement(xRootNodeName));

            XmlElement xBakupNode = xOutXmlDocument[xRootNodeName];
            Int32 xBakupNodeDepth = 1;
            Int32 xBakupNodeIndex = 1;
            for (Int32 i = 0; i < xStreamOutLines.Length; i++)
            {
                String xLine = xStreamOutLines[i];
                String xNodeInfo = xStreamNodeLines[i];
                if (!String.IsNullOrEmpty(xNodeInfo) && !String.IsNullOrEmpty(xLine))
                {
                    Int32 xNodeDepth = Int32.Parse(xNodeInfo.Split(':')[0]);
                    Int32 xNodeIndex = Int32.Parse(xNodeInfo.Split(':')[1]);

                    xTemps = xLine.Split('=');
                    String[] xListNodeNames = xTemps[0].Split('>');
                    String xValue = xTemps[1];
                    if (xNodeDepth == xBakupNodeDepth && xNodeIndex == xBakupNodeIndex)
                    {
                        XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                        xNewAttr.Value = xValue;
                        xBakupNode.Attributes.Append(xNewAttr);
                        //xBakupNode = xBakupNode;
                    }
                    //else if (xBakupNodeDepth == -1 && xBakupNodeIndex == -1)
                    //{
                    //    List<XmlElement> xListNode = new List<XmlElement>();
                    //    for (int xNodeNameIndex = 1; xNodeNameIndex < xListNodeNames.Length - 1; xNodeNameIndex++)
                    //    {
                    //        XmlElement xNewNode = xOutXmlDocument.CreateElement(xListNodeNames[xNodeNameIndex]);
                    //        if (xListNode.Count > 0)
                    //        {
                    //            xListNode[xListNode.Count - 1].AppendChild(xNewNode);
                    //        }
                    //        xListNode.Add(xNewNode);
                    //    }
                    //    xOutXmlDocument[xRootNodeName].AppendChild(xListNode[0]);

                    //    XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                    //    xNewAttr.Value = xValue;
                    //    xBakupNode = xListNode[xListNode.Count - 1];
                    //    xBakupNode.Attributes.Append(xNewAttr);
                    //}
                    else if (xNodeDepth > xBakupNodeDepth && xNodeIndex != xBakupNodeIndex)
                    {
                        List<XmlElement> xListNode = new List<XmlElement>();
                        for (int xNodeNameIndex = xNodeDepth - (xNodeDepth - xBakupNodeDepth); xNodeNameIndex < xListNodeNames.Length - 1; xNodeNameIndex++)
                        {
                            XmlElement xNewNode = xOutXmlDocument.CreateElement(xListNodeNames[xNodeNameIndex]);
                            if (xListNode.Count > 0)
                            {
                                xListNode[xListNode.Count - 1].AppendChild(xNewNode);
                            }
                            xListNode.Add(xNewNode);
                        }
                        xBakupNode.AppendChild(xListNode[0]);

                        XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                        xNewAttr.Value = xValue;
                        xBakupNode = xListNode[xListNode.Count - 1];
                        xBakupNode.Attributes.Append(xNewAttr);
                    }
                    else if (xNodeDepth == xBakupNodeDepth && xNodeIndex != xBakupNodeIndex)
                    {
                        List<XmlElement> xListNode = new List<XmlElement>();
                        for (int xNodeNameIndex = xNodeDepth - 1; xNodeNameIndex < xListNodeNames.Length - 1; xNodeNameIndex++)
                        {
                            XmlElement xNewNode = xOutXmlDocument.CreateElement(xListNodeNames[xNodeNameIndex]);
                            if (xListNode.Count > 0)
                            {
                                xListNode[xListNode.Count - 1].AppendChild(xNewNode);
                            }
                            xListNode.Add(xNewNode);
                        }
                        xBakupNode.ParentNode.AppendChild(xListNode[0]);

                        XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                        xNewAttr.Value = xValue;
                        xBakupNode = xListNode[xListNode.Count - 1];
                        xBakupNode.Attributes.Append(xNewAttr);
                    }
                    else
                    {
                        for (Int32 j = 0; j < xBakupNodeDepth - xNodeDepth; j++)
                        {
                            xBakupNode = (XmlElement)xBakupNode.ParentNode;
                        }
                        List<XmlElement> xListNode = new List<XmlElement>();
                        for (int xNodeNameIndex = xNodeDepth - 1; xNodeNameIndex < xListNodeNames.Length - 1; xNodeNameIndex++)
                        {
                            XmlElement xNewNode = xOutXmlDocument.CreateElement(xListNodeNames[xNodeNameIndex]);
                            if (xListNode.Count > 0)
                            {
                                xListNode[xListNode.Count - 1].AppendChild(xNewNode);
                            }
                            xListNode.Add(xNewNode);
                        }
                        xBakupNode.ParentNode.AppendChild(xListNode[0]);

                        XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                        xNewAttr.Value = xValue;
                        xBakupNode = xListNode[xListNode.Count - 1];
                        xBakupNode.Attributes.Append(xNewAttr);
                    }
                    xBakupNodeDepth = xNodeDepth;
                    xBakupNodeIndex = xNodeIndex;
                }
            }

            xOutXmlDocument.Save(@"./out.xml");


            //foreach (String xLine in File.ReadAllLines("out.txt"))
            //{
            //    XmlElement xTempNode = null;
            //    var xTemps = xLine.Split('=');
            //    String xNodes = xTemps[0];
            //    String xValue = xTemps[1];

            //    String[] xListNode = xNodes.Split('>');
            //    for (int i = 0; i < xListNode.Length - 1; i++)
            //    {
            //        if (xTempNode == null)
            //        {
            //            if (xOutXmlDocument[xListNode[i]] == null)
            //            {
            //                XmlElement xNewTempNode = xOutXmlDocument.CreateElement(xListNode[i]);
            //                if (xTempNode == null)
            //                {
            //                    xOutXmlDocument.AppendChild(xNewTempNode);
            //                }
            //                xTempNode = xNewTempNode;
            //            }
            //            else
            //            {
            //                xTempNode = xOutXmlDocument[xListNode[i]];
            //            }
            //        }
            //        else
            //        {
            //            if (xTempNode[xListNode[i]] == null)
            //            {
            //                XmlElement xNewTempNode = xOutXmlDocument.CreateElement(xListNode[i]);
            //                xTempNode.AppendChild(xNewTempNode);
            //                xTempNode = xNewTempNode;
            //            }
            //            else
            //            {
            //                xTempNode = xTempNode[xListNode[i]];
            //            }
            //        }
            //    }
            //    XmlAttribute xAttr = xOutXmlDocument.CreateAttribute(xListNode[xListNode.Length - 1]);
            //    xAttr.Value = xValue;
            //    xTempNode.Attributes.Append(xAttr);
            //}

            ////xOutXmlDocument
        }
    }
}
