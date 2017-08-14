#include <iostream>
#include <map>
#include <vector>
#include "md5.h"
#include "tinyxml.h"

typedef std::string mstr;
typedef int Int32;

#define ONCE_BLACK " "


struct XmlRecordInfo
{
    std::vector<mstr> m_XmlVector;
    std::map<mstr, mstr> m_XmlMap;
};


mstr CreateHierarchyBlank(Int32 xHierarchy)
{
    mstr xTemp = "";
    for (Int32 i = 0; i < xHierarchy; i++)
    {
        xTemp += ONCE_BLACK;
    }
    return xTemp;
}

void ReadNodeAttr(TiXmlElement* xTiXmlElement, mstr xFullPathKV, Int32 xHierarchy)
{
    if (xTiXmlElement)
    {
        mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
        TiXmlAttribute* xTiXmlAttribute = xTiXmlElement->FirstAttribute();
        while (xTiXmlAttribute)
        {
            //printf("%s[%s=%s]", ONCE_BLACK, xTiXmlAttribute->Name(), xTiXmlAttribute->Value());
            xTiXmlAttribute = xTiXmlAttribute->Next();
        }
        //printf("\n");
    }
}

void ReadNode(XmlRecordInfo& xXmlVector, TiXmlElement* xTiXmlElement, mstr xFullPathKV, Int32 xHierarchy)
{
    mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
    if (xTiXmlElement)
    {
        xXmlVector.m_XmlVector.push_back(xTiXmlElement->Value());
        //xXmlVector.m_XmlMap[xTiXmlElement->Value()]++;

        mstr xTempNodeFullPathKV = xFullPathKV;
        if (xHierarchy != 0) { xTempNodeFullPathKV += "->"; }
        xTempNodeFullPathKV += xTiXmlElement->Value();
        ReadNodeAttr(xTiXmlElement, xTempNodeFullPathKV, xHierarchy);
        //printf("%s%s(%s)\n", xHierarchyBlank.c_str(), xTiXmlElement->Value(), xTempNodeFullPathKV.c_str());
        //printf("%s\n", xTempNodeFullPathKV.c_str());
        printf("%s %s\n", GETSTRMD5(xTempNodeFullPathKV.c_str()).c_str(), xTempNodeFullPathKV.c_str());

        TiXmlElement* xTiSubXmlElement = xTiXmlElement->FirstChildElement();
        while (xTiSubXmlElement)
        {
            //mstr xTempElemFullPathKV = xTempNodeFullPathKV;
            //xTempElemFullPathKV += "->"; xTempElemFullPathKV += xTiSubXmlElement->Value();
            //ReadNodeAttr(xTiSubXmlElement, xTempElemFullPathKV, xHierarchy);
            //printf("%s%s(%s)\n", xHierarchyBlank.c_str(), xTiSubXmlElement->Value(), xTempElemFullPathKV.c_str());
            //xTempElemFullPathKV += "->"; xTempElemFullPathKV += xTiSubXmlElement->Value();
            ReadNode(xXmlVector, xTiSubXmlElement, xTempNodeFullPathKV, xHierarchy + 1);
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
    XmlRecordInfo XmlInfo1;
    XmlRecordInfo XmlInfo2;
    {
        TiXmlDocument xDocument;
        if (!xDocument.LoadFile("./test.xml"))
        {
            printf("打开xml失败");
            return 1;
        }
        TiXmlElement* xTiXmlElement = xDocument.FirstChildElement();
        if (xTiXmlElement)
        {
            do
            {
                mstr xFullPathKV;
                ReadNode(XmlInfo1, xTiXmlElement, xFullPathKV, 0);
                xTiXmlElement = xTiXmlElement->NextSiblingElement();
            } while (xTiXmlElement);
        }
    }

    {
        TiXmlDocument xDocument;
        if (!xDocument.LoadFile("./test2.xml"))
        {
            printf("打开xml失败");
            return 1;
        }
        TiXmlElement* xTiXmlElement = xDocument.FirstChildElement();
        if (xTiXmlElement)
        {
            do
            {
                mstr xFullPathKV;
                ReadNode(XmlInfo2, xTiXmlElement, xFullPathKV, 0);
                xTiXmlElement = xTiXmlElement->NextSiblingElement();
            } while (xTiXmlElement);
        }
    }

    //printf(GETSTRMD5(XmlVector1[0].c_str()).c_str());


    system("Pause");
    return 0;
}

