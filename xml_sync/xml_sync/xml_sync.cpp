#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "md5.h"
#include "tinyxml.h"
#include "MsList.hpp"

#define _Max(a,b)    (((a) > (b)) ? (a) : (b))

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

enum MergeType
{
    MT_ADD, // 增加
    MT_DEL, // 删除
    MT_CHG, // 修改
    MT_MOV, // 移动
};


struct MergeInfo
{
    MergeType m_Type;
    struct
    {
        mstr m_AddValue;
        mstr m_DelValue;
        std::pair<Int32, mstr> m_SrcLineNewValue;
        Int32 m_SrcLine;
    } MergeData;
};


struct XmlRecordInfo
{
    FILE* m_OutFileName;
    MsList<mstr> m_List_LineText;
    MsList<mstr> m_List_LineName;
    MsList<mstr> m_List_LineValue;
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
            xXmlVector.m_List_LineName.Add(xTempNodeFullPathKV);
            xTempNodeFullPathKV += "=";
            xTempNodeFullPathKV += xTiXmlAttribute->Value();
            xXmlVector.m_List_LineValue.Add(xTiXmlAttribute->Value());

            //mstr xMd5 = GETSTRMD5(xTempNodeFullPathKV);
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
    XmlRecordInfo XmlInfoSrc;
    XmlInfoSrc.m_OutFileName = fopen("./1.out.txt", "w+");
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
                ReadNode(XmlInfoSrc, xTiXmlElement, xFullPathKV, 0);
                xTiXmlElement = xTiXmlElement->NextSiblingElement();
            } while (xTiXmlElement);
        }
    }

    //printf("----------------------\n");
    //system("cls");

    XmlRecordInfo XmlInfoTar;
    XmlInfoTar.m_OutFileName = fopen("./2.out.txt", "w+");
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
                ReadNode(XmlInfoTar, xTiXmlElement, xFullPathKV, 0);
                xTiXmlElement = xTiXmlElement->NextSiblingElement();
            } while (xTiXmlElement);
        }
    }

    //printf(GETSTRMD5(XmlVector1[0].c_str()).c_str());


    //auto x1 = XmlInfoSrc.m_Dict_Md5_Text["0347f7e68541d577571b2f57b794e4f2"];
    //auto x2 = XmlInfoTar.m_Dict_Md5_Text["0347f7e68541d577571b2f57b794e4f2"];


    MsList<MergeInfo> xListMergeLines;
    for (Int32 xTarLine = 0; xTarLine < XmlInfoTar.m_List_LineText.GetCount(); xTarLine++)
    {
        MergeInfo xMergeInfo;
        mstr& xTarItem = XmlInfoTar.m_List_LineText[xTarLine];
        Int32 xSrcLine = XmlInfoSrc.m_List_LineText.IndexOf(xTarItem);
        if (xSrcLine >= 0)
        {
            xMergeInfo.m_Type = MergeType::MT_MOV;
            xMergeInfo.MergeData.m_SrcLine = xSrcLine;
            xListMergeLines.Add(xMergeInfo);
        }
        else
        {
            mstr& xTarName = XmlInfoTar.m_List_LineName[xTarLine];
            Int32 xSrcNameLine = XmlInfoSrc.m_List_LineName.IndexOf(xTarName);
            if (xSrcNameLine >= 0)
            {
                xMergeInfo.m_Type = MergeType::MT_CHG;
                xMergeInfo.MergeData.m_SrcLineNewValue = std::pair<Int32, mstr>(xSrcNameLine, XmlInfoTar.m_List_LineValue[xTarLine]);
                xListMergeLines.Add(xMergeInfo);
            }
            else
            {
                xMergeInfo.m_Type = MergeType::MT_ADD;
                xMergeInfo.MergeData.m_AddValue = XmlInfoTar.m_List_LineText[xTarLine];
                xListMergeLines.Add(xMergeInfo);
            }
        }
    }



    FILE* xFile = fopen("d:\\up.txt", "w+");
    for (Int32 i = 0; i < xListMergeLines.GetCount(); i++)
    {
        switch (xListMergeLines[i].m_Type)
        {
            // 增加
            case MergeType::MT_ADD:
            {
                fprintf(xFile, "A\t%s\n", xListMergeLines[i].MergeData.m_AddValue.c_str());
            }break;

            // 删除
            case MergeType::MT_DEL:
            {
                fprintf(xFile, "D\n");
            }break;

            // 修改
            case MergeType::MT_CHG:
            {
                fprintf(xFile, "C%d\t%s\n", xListMergeLines[i].MergeData.m_SrcLineNewValue.first, xListMergeLines[i].MergeData.m_SrcLineNewValue.second.c_str());
            }break;

            // 移动
            case MergeType::MT_MOV:
            {
                if (i == xListMergeLines[i].MergeData.m_SrcLine)
                {
                    fprintf(xFile, "-\n");
                }
                else
                {
                    fprintf(xFile, "M%d\n", xListMergeLines[i].MergeData.m_SrcLine);
                }
            }break;
        }
    }
    fclose(xFile);




    xFile = fopen("d:\\1.txt", "w+");
    for (Int32 i = 0; i < XmlInfoSrc.m_List_LineText.GetCount(); i++)
    {
        fprintf(xFile, "%s\n", XmlInfoSrc.m_List_LineText[i].c_str());
    }
    fclose(xFile);


    xFile = fopen("d:\\2.txt", "w+");
    for (Int32 i = 0; i < XmlInfoTar.m_List_LineText.GetCount(); i++)
    {
        fprintf(xFile, "%s\n", XmlInfoTar.m_List_LineText[i].c_str());
    }
    fclose(xFile);



    fclose(XmlInfoSrc.m_OutFileName);
    fclose(XmlInfoTar.m_OutFileName);

    system("Pause");
    return 0;
}

