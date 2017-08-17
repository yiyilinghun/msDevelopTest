using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;

namespace UpdateXml
{
    class Program
    {
        static void Main(string[] args)
        {
            //XmlDocument xXmlDocument = new XmlDocument();
            //xXmlDocument.Load(@"test1.xml");

            XmlDocument xOutXmlDocument = new XmlDocument();

            StreamReader xStreamReader = File.OpenText(@"D:\out.txt");
            StreamReader xNodeReader = File.OpenText(@"D:\node.txt");
            String xRoot = xStreamReader.ReadLine();
            String xRootNodeInfo = xNodeReader.ReadLine();
            var xTemps = xRoot.Split('=');
            String[] xRootNode = xTemps[0].Split('>');
            String xRootValue = xTemps[1];
            xOutXmlDocument.AppendChild(xOutXmlDocument.CreateElement(xRootNode[0]));
            XmlAttribute xRootAttr = xOutXmlDocument.CreateAttribute(xRootNode[1]);
            xRootAttr.Value = xRootValue;
            xOutXmlDocument[xRootNode[0]].Attributes.Append(xRootAttr);

            String xLine = null;
            //String xNodeInfo = null;
            XmlElement xBakupNode = null;
            Int32 xBakupNodeDepth = -1;
            Int32 xBakupNodeIndex = -1;
            do
            {
                xLine = xStreamReader.ReadLine();
                String xNodeInfo = xNodeReader.ReadLine();
                if (xNodeInfo != null && xLine != null)
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
                    else if (xBakupNodeDepth == -1 && xBakupNodeIndex == -1)
                    {
                        List<XmlElement> xListNode = new List<XmlElement>();
                        for (int xNodeNameIndex = 1; xNodeNameIndex < xListNodeNames.Length - 1; xNodeNameIndex++)
                        {
                            XmlElement xNewNode = xOutXmlDocument.CreateElement(xListNodeNames[xNodeNameIndex]);
                            if (xListNode.Count > 0)
                            {
                                xListNode[xListNode.Count - 1].AppendChild(xNewNode);
                            }
                            xListNode.Add(xNewNode);
                        }
                        xOutXmlDocument[xRootNode[0]].AppendChild(xListNode[0]);

                        XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                        xNewAttr.Value = xValue;
                        xBakupNode = xListNode[xListNode.Count - 1];
                        xBakupNode.Attributes.Append(xNewAttr);
                    }
                    else if (xNodeDepth > xBakupNodeDepth && xNodeIndex != xBakupNodeIndex)
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
                        for (Int32 i = 0; i < xBakupNodeDepth - xNodeDepth; i++)
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
            } while (xLine != null);

            xOutXmlDocument.Save(@"d:\out.xml");


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
