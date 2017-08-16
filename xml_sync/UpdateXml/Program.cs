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
            XmlDocument xXmlDocument = new XmlDocument();
            xXmlDocument.Load(@"test1.xml");

            XmlDocument xOutXmlDocument = new XmlDocument();

            StreamReader xStreamReader = File.OpenText("out.txt");
            String xRoot = xStreamReader.ReadLine();
            var xTemps = xRoot.Split('=');
            String[] xRootNode = xTemps[0].Split('>');
            String xRootValue = xTemps[1];
            xOutXmlDocument.AppendChild(xOutXmlDocument.CreateElement(xRootNode[0]));
            XmlAttribute xRootAttr = xOutXmlDocument.CreateAttribute(xRootNode[1]);
            xRootAttr.Value = xRootValue;
            xOutXmlDocument[xRootNode[0]].Attributes.Append(xRootAttr);

            String xLine = null;
            do
            {
                xLine = xStreamReader.ReadLine();
                if (xLine != null)
                {
                    List<XmlElement> xListNode = new List<XmlElement>();
                    xTemps = xLine.Split('=');
                    String[] xListNodeNames = xTemps[0].Split('>');
                    String xValue = xTemps[1];

                    for (int xNodeNameIndex = 1; xNodeNameIndex < xListNodeNames.Length - 1; xNodeNameIndex++)
                    {
                        XmlElement xNewNode = xOutXmlDocument.CreateElement(xListNodeNames[xNodeNameIndex]);
                        if (xListNode.Count > 0)
                        {
                            xListNode[xListNode.Count - 1].AppendChild(xNewNode);
                        }
                        xListNode.Add(xNewNode);
                    }
                    XmlAttribute xNewAttr = xOutXmlDocument.CreateAttribute(xListNodeNames[xListNodeNames.Length - 1]);
                    xNewAttr.Value = xValue;
                    xListNode[xListNode.Count - 1].Attributes.Append(xNewAttr);
                    xOutXmlDocument[xRootNode[0]].AppendChild(xListNode[0]);
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
