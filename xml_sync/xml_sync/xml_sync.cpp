#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include "md5.h"
#include "tinyxml.h"
#include "MsList.hpp"

enum MergeType
{
    MT_NON, // 未变
    MT_ADD, // 增加
    //MT_DEL, // 删除
    MT_CHG, // 修改
    MT_MOV, // 移动
    MT_END, // 结束
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
};

//mstr CreateHierarchyBlank(Int32 xHierarchy)
//{
//    mstr xTemp = "";
//    for (Int32 i = 0; i < xHierarchy; i++) { xTemp += ONCE_BLACK; }
//    return xTemp;
//}

void ReadNodeAttr(XmlRecordInfo& xXmlVector, TiXmlElement* xTiXmlElement, mstr xFullPathKV, Int32 xHierarchy)
{
    if (xTiXmlElement)
    {
        //mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
        TiXmlAttribute* xTiXmlAttribute = xTiXmlElement->FirstAttribute();
        while (xTiXmlAttribute)
        {
            mstr xTempNodeFullPathKV = xFullPathKV;
            xTempNodeFullPathKV += ">";
            xTempNodeFullPathKV += xTiXmlAttribute->Name();
            xXmlVector.m_List_LineName.Add(xTempNodeFullPathKV);
            xTempNodeFullPathKV += "=";
            xTempNodeFullPathKV += xTiXmlAttribute->Value();
            xXmlVector.m_List_LineValue.Add(xTiXmlAttribute->Value());
            xXmlVector.m_List_LineText.Add(xTempNodeFullPathKV);
            xTiXmlAttribute = xTiXmlAttribute->Next();
        }
    }
}

void ReadNode(XmlRecordInfo& xXmlVector, TiXmlElement* xTiXmlElement, mstr xFullPathKV, Int32 xHierarchy)
{
    //mstr xHierarchyBlank = CreateHierarchyBlank(xHierarchy);
    if (xTiXmlElement)
    {
        mstr xTempNodeFullPathKV = xFullPathKV;
        if (xHierarchy != 0) { xTempNodeFullPathKV += ">"; }
        xTempNodeFullPathKV += xTiXmlElement->Value();

        ReadNodeAttr(xXmlVector, xTiXmlElement, xTempNodeFullPathKV, xHierarchy + 1);

        TiXmlElement* xTiSubXmlElement = xTiXmlElement->FirstChildElement();
        while (xTiSubXmlElement)
        {
            ReadNode(xXmlVector, xTiSubXmlElement, xTempNodeFullPathKV, xHierarchy + 1);
            xTiSubXmlElement = xTiSubXmlElement->NextSiblingElement();
        }
    }
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

    FILE* xFile = fopen("d:\\1.txt", "w+");
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
    //printf(GETSTRMD5(XmlVector1[0].c_str()).c_str());
    //auto x1 = XmlInfoSrc.m_Dict_Md5_Text["0347f7e68541d577571b2f57b794e4f2"];
    //auto x2 = XmlInfoTar.m_Dict_Md5_Text["0347f7e68541d577571b2f57b794e4f2"];


    MergeInfo xMergeInfo;
    MsList<MergeInfo> xListMergeLines;
    for (Int32 xTarLine = 0; xTarLine < XmlInfoTar.m_List_LineText.GetCount(); xTarLine++)
    {
        mstr& xTarItem = XmlInfoTar.m_List_LineText[xTarLine];
        Int32 xSrcLine = XmlInfoSrc.m_List_LineText.IndexOf(xTarItem);
        if (xSrcLine >= 0)
        {
            if (xTarLine == xSrcLine)
            {
                xMergeInfo.m_Type = MergeType::MT_NON;
            }
            else
            {
                xMergeInfo.m_Type = MergeType::MT_MOV;
            }
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
    xMergeInfo.m_Type = MergeType::MT_END;
    xListMergeLines.Add(xMergeInfo);


    xFile = fopen("d:\\up.txt", "w+");
    for (Int32 i = 0; i < xListMergeLines.GetCount(); i++)
    {
        switch (xListMergeLines[i].m_Type)
        {
            // 未变
            case MergeType::MT_NON:
            {
                fprintf(xFile, "\n");
            }break;

            // 增加
            case MergeType::MT_ADD:
            {
                fprintf(xFile, "A%s\n", xListMergeLines[i].MergeData.m_AddValue.c_str());
            }break;

            // 修改
            case MergeType::MT_CHG:
            {
                fprintf(xFile, "C%d=%s\n", xListMergeLines[i].MergeData.m_SrcLineNewValue.first, xListMergeLines[i].MergeData.m_SrcLineNewValue.second.c_str());
            }break;

            // 移动
            case MergeType::MT_MOV:
            {
                fprintf(xFile, "M%d\n", xListMergeLines[i].MergeData.m_SrcLine);
            }break;

            // 结束
            case MergeType::MT_END:
            {
                fprintf(xFile, "E");
            }break;
        }
    }
    fclose(xFile);



    xListMergeLines.Clear();
    FILE* xOutFile = fopen("d:\\out.txt", "w+");
    std::fstream xRead("d:\\up.txt", std::ios::in);
    Int32 xOutIndex = 0;
    while (!xRead.eof())
    {
        Char szBuff[2048];
        xRead.getline(szBuff, 2048);
        Char xFlag = -1;
        Char szText[2048];
        if (strlen(szBuff) == 0)
        {
            if (xOutIndex < XmlInfoSrc.m_List_LineText.GetCount())
            {
                fprintf(xOutFile, "%s\n", XmlInfoSrc.m_List_LineText[xOutIndex].c_str());
            }
        }
        else
        {
            sscanf(szBuff, "%c%s\n", &xFlag, szText);
        }
        switch (xFlag)
        {
            //// 未变
            //case '':
            //{
            //    fprintf(xOutFile, "%s\n", XmlInfoSrc.m_List_LineText[xOutIndex].c_str());
            //}break;

            // 增加
            case 'A':
            {
                fprintf(xFile, "%s\n", szText);
            }break;

            // 修改
            case 'C':
            {
                Int32 xSrcLine;
                sscanf(szBuff, "C%d=%s\n", &xSrcLine, szText);
                fprintf(xFile, "%s=%s\n", XmlInfoSrc.m_List_LineName[xSrcLine].c_str(), szText);
            }break;

            // 移动
            case 'M':
            {
                Int32 xSrcLine;
                sscanf(szBuff, "M%d\n", &xSrcLine);
                fprintf(xFile, "%s\n", XmlInfoSrc.m_List_LineText[xSrcLine].c_str());
            }break;

            // 结束
            case 'E':
            {
                xOutIndex = -1;
            }break;
        }
        if (xOutIndex == -1)
        {
            break;
        }
        else
        {
            xOutIndex++;
        }
    }
    fclose(xFile);
    fclose(xOutFile);















    fclose(XmlInfoSrc.m_OutFileName);
    fclose(XmlInfoTar.m_OutFileName);








    //XmlRecordInfo XmlInfoOut;
    //XmlInfoTar.m_OutFileName = fopen("d:/out.txt", "r+");
    //{
    //    TiXmlDocument xDocument;
    //    FILE* xOutFile = fopen("d:\\out.txt", "w+");
    //    while (!xRead.eof())
    //    {
    //        Char szBuff[2048];
    //        xRead.getline(szBuff, 2048);
    //        if (strlen(szBuff) == 0)
    //        {
    //        }
    //    }
    //    //if (!xDocument.LoadFile("./test2.xml"))
    //    //{
    //    //    printf("打开xml失败");
    //    //    return 1;
    //    //}

    //    TiXmlNode xTiXmlNode;
    //    xDocument.InsertEndChild()
    //    TiXmlElement* xTiXmlElement = xDocument.FirstChildElement();
    //    if (xTiXmlElement)
    //    {
    //        do
    //        {
    //            mstr xFullPathKV;
    //            ReadNode(XmlInfoTar, xTiXmlElement, xFullPathKV, 0);
    //            xTiXmlElement = xTiXmlElement->NextSiblingElement();
    //        } while (xTiXmlElement);
    //    }
    //}






    system("Pause");
    return 0;
}

