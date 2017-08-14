#include <iostream>
#include <map>
#include "tinyxml.h"

typedef std::string mstr;
typedef int Int32;

std::map<mstr, mstr> g_Map;

mstr CreateHierarchyBlank(Int32 xHierarchy)
{
    mstr xTemp = "";
    for (Int32 i = 0; i < xHierarchy; i++)
    {
        xTemp += "    ";
    }
    return xTemp;
}

void ReadNodeAttr(TiXmlElement* xTiXmlElement, Int32 xHierarchy)
{
    if (xTiXmlElement)
    {
        mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
        TiXmlAttribute* xTiXmlAttribute = xTiXmlElement->FirstAttribute();
        printf("%s", xHierarchyBlank.c_str());
        while (xTiXmlAttribute)
        {
            printf(" [%s]", xTiXmlAttribute->Name());
            xTiXmlAttribute = xTiXmlAttribute->Next();
        }
        printf("\n");
    }
}

void ReadNode(TiXmlElement* xTiXmlElement, Int32 xHierarchy)
{
    mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
    if (xTiXmlElement)
    {
        printf("%s%s", xHierarchyBlank.c_str(), xTiXmlElement->Value());
        ReadNodeAttr(xTiXmlElement, xHierarchy);

        TiXmlElement* xTiSubXmlElement = xTiXmlElement->FirstChildElement();
        while (xTiSubXmlElement)
        {
            ReadNode(xTiSubXmlElement, xHierarchy + 1);
            xTiSubXmlElement = xTiSubXmlElement->NextSiblingElement();
        }
    }
    //if (xTiXmlNode)
    //{
    //    do 
    //    {
    //    } while (xTiXmlNode);
    //    TiXmlNode* xSubTiXmlNode = xTiXmlNode->FirstChild();
    //    if (xSubTiXmlNode)
    //    {
    //        do
    //        {
    //            printf("%s\n", xSubTiXmlNode->Value());
    //            xSubTiXmlNode = xSubTiXmlNode->NextSibling();
    //        } while (xSubTiXmlNode);
    //    }
    //}
}

int main()
{
    TiXmlDocument xDocument;
    if (!xDocument.LoadFile(R"(I:\msgit\ms_develop_test\xml_sync\xml_sync\test.xml)"))
    {
        printf("¥Úø™xml ß∞‹");
        return 1;
    }

    TiXmlElement* xTiXmlElement = xDocument.FirstChildElement();
    if (xTiXmlElement)
    {
        do
        {
            ReadNode(xTiXmlElement, 0);
            xTiXmlElement = xTiXmlElement->NextSiblingElement();
        } while (xTiXmlElement);
    }

    //MsXmlReader xMsXmlReader;
    //if (!xMsXmlReader.Load(R"(I:\msgit\ms_develop_test\xml_sync\xml_sync\test.xml)"))
    //{
    //    printf("º”‘ÿxml ß∞‹!\n");
    //}

    //Int32 iPosParent, iPos, iPosChild;
    //if (xMsXmlReader[NULL])
    //{
    //    while (xMsXmlReader.FindNextElem())
    //    {
    //        xMsXmlReader.BackupPos(iPosParent, iPos, iPosChild);
    //        xMsXmlReader._SetPos(iPosParent, iPos, iPosChild);
    //    }
    //    //xMsXmlReader.firstnode
    //    printf("º”‘ÿxml ß∞‹!\n");
    //}

    system("Pause");
    return 0;
}

