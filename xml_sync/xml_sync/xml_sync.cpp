#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "md5.h"
#include "tinyxml.h"
#include "MsList.hpp"

//#define ONCE_BLACK "    "
#define ONCE_BLACK ""
//
//struct TextRecordInfo
//{
//    TextRecordInfo() : m_Text("") {}
//    mstr m_Md5;
//    mstr m_Text;
//};
//
//struct XmlRecordInfo
//{
//    FILE* m_OutFileName;
//    MsList<mstr> m_List_LineText;
//    MsList<mstr> m_List_LineMd5;
//    std::map<mstr, MsList<Int32>> m_Dict_Md5_Text;
//};
//

struct XmlRecordInfo
{
    FILE* m_OutFileName;
    MsList<mstr> m_List_LineText;
    //MsList<mstr> m_List_LineMd5;
    //std::map<mstr, MsList<Int32>> m_Dict_Md5_Text;
};

mstr CreateHierarchyBlank(Int32 xHierarchy)
{
    mstr xTemp = "";
    for (Int32 i = 0; i < xHierarchy; i++) { xTemp += ONCE_BLACK; }
    return xTemp;
}


void ReadNodeAttr(XmlRecordInfo& xXmlVector, TiXmlElement* xTiXmlElement, mstr xFullPathKV, Int32 xHierarchy)
{
    if (xTiXmlElement)
    {
        mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
        TiXmlAttribute* xTiXmlAttribute = xTiXmlElement->FirstAttribute();
        while (xTiXmlAttribute)
        {
            mstr xTempNodeFullPathKV = xFullPathKV;
            xTempNodeFullPathKV += "->";
            xTempNodeFullPathKV += xTiXmlAttribute->Name();
            xTempNodeFullPathKV += ",";
            xTempNodeFullPathKV += xTiXmlAttribute->Value();

            mstr xMd5 = GETSTRMD5(xTempNodeFullPathKV);
            //while ((Int32)xXmlVector.m_List_LineText.GetCount() < xLineNumber)
            //{
                //Char xBuff[4096];
                //sprintf(xBuff, "%s[%s=%s]", xFullPathKV.c_str(), xTiXmlAttribute->Name(), xTiXmlAttribute->Value());
            xXmlVector.m_List_LineText.Add(xTempNodeFullPathKV);
            //xXmlVector.m_List_LineMd5.Add(xMd5);
            //xXmlVector.m_Dict_Md5_Text[xMd5].Add(xLineNumber);
            //}
            //xLineNumber++;
            //TextRecordInfo* xTextRecordInfo = xXmlVector.m_List_LineText[xLineNumber - 1];
            //xXmlVector.m_Dict_Md5_Text[xMd5].Add(xTextRecordInfo);
            //fprintf(xXmlVector.m_OutFileName, "%08x:%s%s[%s=%s]:%s\n", xLineNumber++, GETSTRMD5(xTempNodeFullPathKV).c_str(), CreateHierarchyBlank(xHierarchy).c_str(), xFullPathKV.c_str(), xTiXmlAttribute->Name(), xTiXmlAttribute->Value());
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
        //xXmlVector.m_XmlVector.push_back(xTiXmlElement->Value());
        //xXmlVector.m_XmlMap[xTiXmlElement->Value()]++;

        mstr xTempNodeFullPathKV = xFullPathKV;
        if (xHierarchy != 0) { xTempNodeFullPathKV += "->"; }
        xTempNodeFullPathKV += xTiXmlElement->Value();

        ReadNodeAttr(xXmlVector, xTiXmlElement, xTempNodeFullPathKV, xHierarchy + 1);
        //printf("%s%s(%s)\n", xHierarchyBlank.c_str(), xTiXmlElement->Value(), xTempNodeFullPathKV.c_str());
        //printf("%s\n", xTempNodeFullPathKV.c_str());


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
    XmlInfo1.m_OutFileName = fopen("./1.out.txt", "w+");
    {
        TiXmlDocument xDocument;
        if (!xDocument.LoadFile("./test1.xml"))
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

    //printf("----------------------\n");
    //system("cls");

    XmlRecordInfo XmlInfo2;
    XmlInfo2.m_OutFileName = fopen("./2.out.txt", "w+");
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


    //auto x1 = XmlInfo1.m_Dict_Md5_Text["0347f7e68541d577571b2f57b794e4f2"];
    //auto x2 = XmlInfo2.m_Dict_Md5_Text["0347f7e68541d577571b2f57b794e4f2"];



    std::map<Int32, Int32> xUpdate;
    for (Int32 xSrcLine = 0; xSrcLine < XmlInfo1.m_List_LineText.GetCount(); xSrcLine++)
    {
        mstr& xItem = XmlInfo1.m_List_LineText[xSrcLine];
        Int32 xTarLine = XmlInfo2.m_List_LineText.IndexOf(xItem);
        xUpdate[xSrcLine] = xTarLine;
    }


    std::map<Int32, mstr> xTempSet;
    for (auto xUpdateLine : xUpdate)
    {
        xTempSet[xUpdateLine.second] = XmlInfo1.m_List_LineText[xUpdateLine.first];
    }

    Int32 xMax = ((--xTempSet.end())->first) - 1;
    FILE* xFile = fopen("d:\\out.txt", "w+");
    for (Int32 i = 0; i < xMax; i++)
    {
        if (xTempSet.find(i) != xTempSet.end())
        {
            fprintf(xFile, "%s\n", xTempSet[i].c_str());
        }
        else
        {
            fprintf(xFile, "\n");
        }
    }
    fclose(xFile);


    xFile = fopen("d:\\1.txt", "w+");
    for (Int32 i = 0; i < XmlInfo1.m_List_LineText.GetCount(); i++)
    {
        fprintf(xFile, "%s\n", XmlInfo1.m_List_LineText[i].c_str());
    }
    fclose(xFile);


    xFile = fopen("d:\\2.txt", "w+");
    for (Int32 i = 0; i < XmlInfo2.m_List_LineText.GetCount(); i++)
    {
        fprintf(xFile, "%s\n", XmlInfo2.m_List_LineText[i].c_str());
    }
    fclose(xFile);
    system("Pause");



    //std::map<Int32, mstr> xTempSet;
    //for (Int32 i = 0; i < XmlInfo1.m_List_LineMd5.GetCount(); i++)
    //{
    //    auto xItem = XmlInfo1.m_Dict_Md5_Text[XmlInfo1.m_List_LineMd5[i]];
    //    for (Int32 xLine : xItem.m_Container)
    //    {
    //        xTempSet[xLine] = XmlInfo1.m_List_LineText[xLine - 1];
    //    }
    //}



    //Int32 xMax = ((--xTempSet.end())->first) - 1;
    //for (Int32 i = 0; i < xMax; i++)
    //{
    //    if (xTempSet.find(i) != xTempSet.end())
    //    {
    //        fprintf(XmlInfo1.m_OutFileName, "%s\n", xTempSet[i].c_str());
    //    }
    //    else
    //    {
    //        fprintf(XmlInfo1.m_OutFileName, "\n");
    //    }
    //}




    //xTempSet.clear();
    //for (auto xItem : XmlInfo2.m_Dict_Md5_Text)
    //{
    //    for (Int32 xLine : xItem.second.m_Container)
    //    {
    //        xTempSet[xLine] = XmlInfo2.m_List_LineText[xLine - 1];
    //    }
    //}

    //xMax = ((--xTempSet.end())->first) - 1;
    //for (Int32 i = 0; i < xMax; i++)
    //{
    //    if (xTempSet.find(i) != xTempSet.end())
    //    {
    //        fprintf(XmlInfo2.m_OutFileName, "%s\n", xTempSet[i].c_str());
    //    }
    //    else
    //    {
    //        fprintf(XmlInfo2.m_OutFileName, "\n");
    //    }
    //}


    fclose(XmlInfo1.m_OutFileName);
    fclose(XmlInfo2.m_OutFileName);



    system("Pause");
    return 0;
}

