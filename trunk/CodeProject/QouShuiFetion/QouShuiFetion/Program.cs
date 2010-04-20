using System;
using System.Collections.Generic;
 
using System.Windows.Forms;

namespace QouShui.DLL.Fetion
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
           Application.Run(new Form1());
            //Console.WriteLine(Qoushui.DLL.Encoding.ClassShare.GetMD5String(System.Text.Encoding.ASCII.GetBytes(DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss"))));

            //d6cc5b41d9a14a83be135bf2a406ca92
            //0420F053E4A6D483DB6B7FD97E9FFE30
        }
    }
}
