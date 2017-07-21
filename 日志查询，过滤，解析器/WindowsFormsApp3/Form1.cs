using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        void SelectKeyLog(String xFile)
        {
            List<String> xListResult = new List<String>();
            List<String> xListKey = new List<String>();
            xListKey.Add(@"RoomPlayerResult");
            xListKey.Add(@"MultiBattleSpace::LeaveSpace");
            xListKey.Add(@"RoomPlayerResult");
            xListKey.Add(@"time out quit");
            xListKey.Add(@"pTeam m_kList.erase(it++);");
            xListKey.Add(@"TeamManager::");

            var xListLines = File.ReadAllLines(xFile);

            foreach (String xLine in xListLines)
            {
                foreach (String xKey in xListKey)
                {
                    if (xLine.IndexOf(xKey) > 0)
                    {
                        xListResult.Add(xLine);
                        this.Text = String.Format("已经找到[{0}]", xListResult.Count);
                        break;
                    }
                }
            }

            File.WriteAllLines(xFile + ".log", xListResult);
        }


        Dictionary<Int64, String> m_DictResult = new Dictionary<Int64, String>();
        void SelectID_NAME_TO_DICT(String xFile)
        {
            m_DictResult.Clear();
            //List<String> xListKey = new List<String>();
            //xListKey.Add(@"RoomPlayerResult");

            var xListLines = File.ReadAllLines(xFile);
            foreach (String xLine in xListLines)
            {
                Int32 xIndex = xLine.IndexOf("RoomPlayerResult");
                if (xIndex > 0)
                {
                    String xSubString = xLine.Substring(xIndex);
                    xIndex = xSubString.IndexOf(",ID");
                    var xTempList = xSubString.Split(',');
                    xTempList[1] = xTempList[1].Substring(2);
                    xTempList[2] = xTempList[2];

                    Int64 xID = Int64.Parse(xTempList[1]);
                    if (!m_DictResult.ContainsKey(xID))
                    {
                        m_DictResult.Add(xID, xTempList[2]);
                        this.Text = String.Format("已解析[{0}]个ID_NAME", m_DictResult.Count);
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //SelectKeyLog(@"C:\Users\Administrator\Desktop\大主宰，roomlog\room\log_20170719101906\ID_NAME_DICT.log");
            SelectID_NAME_TO_DICT(@"C:\Users\Administrator\Desktop\大主宰，roomlog\room\log_20170719101906\ID_NAME_DICT.log");
            MessageBox.Show("拉取名字ID对应表完成");
        }






        void FillIDNameToLog(String xFile)
        {
            String xAllLogs = File.ReadAllText(xFile);
            foreach (var xID_Name in m_DictResult)
            {
                xAllLogs = xAllLogs.Replace(xID_Name.Key.ToString(), String.Format("{0}{1}", xID_Name.Value, xID_Name.Key));
            }

            File.WriteAllText(xFile + ".log", xAllLogs);
        }

        private void B_填充名字ID记录到LOG_Click(object sender, EventArgs e)
        {
            FillIDNameToLog(@"C:\Users\Administrator\Desktop\大主宰，roomlog\room\log_20170719101906\room.2017-07-18.log.log");
            FillIDNameToLog(@"C:\Users\Administrator\Desktop\大主宰，roomlog\room\log_20170719101906\room.log.log.log");
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void B_过滤日志_Click(object sender, EventArgs e)
        {
            foreach (String xFile in Directory.EnumerateFiles(@"C:\Users\Administrator\Desktop\dzz_log"))
            {
                SelectKeyLog(xFile);
            }
        }
    }
}


