using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace QouShui.DLL.Fetion
{
    public partial class FormAdd : Form
    {
        public FormAdd()
        {
            InitializeComponent();
        mainFile=    System.IO.Path.Combine(System.Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Fetion", Properties.Settings.Default.no + "\\616777643_13767525810.xml");
             
            dslog.ReadXml( mainFile );
        }
        Log dslog = new Log();
        string mainFile = "";
        const string bin = "tel:13767525810";
        const string me = "sip:650505265@fetion.com.cn;p=810";
        private void button1_Click(object sender, EventArgs e)
        {
            //sip:573719766@fetion.com.cn;p=1450
            //tel:13767506553

            string ms=Qoushui.DLL.Encoding.ClassShare.GetMD5String(System.Text.Encoding.ASCII.GetBytes(DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss")));
           
            if(checkBox1.Checked)
                dslog.Message.AddMessageRow(ms, textBox1.Text, ".", me, dateTimePicker1.Value.Date.ToString("yyyy-MM-dd"), dateTimePicker1.Value.ToShortTimeString());
            else
            dslog.Message.AddMessageRow(ms, textBox1.Text, "冰冰", bin, dateTimePicker1.Value.Date.ToString("yyyy-MM-dd"), dateTimePicker1.Value.ToShortTimeString());
            dslog.WriteXml(mainFile );
            QouShui.DLL.Message.Msg.Info("ok");

        }
        private void write()
        {

        }

    }
}
