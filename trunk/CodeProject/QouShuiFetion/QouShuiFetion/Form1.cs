using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
 
using System.Text;
using System.Windows.Forms;
using System.Data.SQLite;

private string _dataSource;
private string _password;
private SQLiteConnection _connection;

internal void EnableConnection()

namespace QouShui.DLL.Fetion
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.SizeChanged += new EventHandler(Form1_SizeChanged);
            mainNo = Properties.Settings.Default.no;
            mainFile = System.IO.Path.Combine(System.Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\Fetion", mainNo + "\\history.dat");
            if (!System.IO.Directory.Exists(LeadOutPath))
                System.IO.Directory.CreateDirectory(LeadOutPath);

            leadin(System.IO.Path.GetDirectoryName(mainFile));



            AutoBak();
            merge(System.IO.Path.GetDirectoryName(mainFile), "616777643");
            ds = getSQLiteData(mainFile, mainNo);
            //getAllSQLiteData(System.IO.Path.Combine(System.Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),@"Fetion\650505265"), "650505265" );
            addList();
        }

        void Form1_SizeChanged(object sender, EventArgs e)
        {
            sp.SplitterDistance = 373;
        }

        DSMessage ds = new DSMessage();
        const string format = "Data Source={0};Version=3;Password={1};";
        string mainNo = "";
        string mainFile = "";

        #region 数据获取与合并更新

        private DSMessage getSQLiteData(string file, string pass)
        {
            DSMessage ds = new DSMessage();
            SQLiteConnection conn = null;

            try
            {
                conn = new SQLiteConnection(string.Format(format, file, pass));
                SQLiteDataAdapter adapter = new SQLiteDataAdapter();

                conn.Open();
                adapter.SelectCommand = new SQLiteCommand("select * from Player", conn);
                adapter.Fill(ds, ds.Player.TableName);
                adapter.SelectCommand = new SQLiteCommand("select * from Message", conn);
                adapter.Fill(ds, ds.Message.TableName);
                adapter.SelectCommand = new SQLiteCommand("select * from MessageBody", conn);
                adapter.Fill(ds, ds.MessageBody.TableName);
                adapter.Dispose();
                conn.Close();
                Console.WriteLine(file + " ok");
                return ds;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return new DSMessage();
            }
        }

        private void getAllSQLiteData(string dataPath, string pass)
        {
            ds.Clear();
            string[] list = System.IO.Directory.GetFiles(dataPath, "*history*.dat");
            foreach (string file in list)
            {
                ds.Merge(getSQLiteData(file, pass));
            }
        }

        private void update(DSMessage dsupdate, SQLiteConnection conn)
        {
            SQLiteDataAdapter adapter = new SQLiteDataAdapter();
            SQLiteCommand cmd1 = new SQLiteCommand("insert into Player values(@uri,@Name,@Type)", conn);
            cmd1.Parameters.Add("@uri", DbType.String).SourceColumn = dsupdate.Player.UriColumn.ColumnName;
            cmd1.Parameters.Add("@Name", DbType.String).SourceColumn = dsupdate.Player.NameColumn.ColumnName;
            cmd1.Parameters.Add("@Type", DbType.Int32).SourceColumn = dsupdate.Player.TypeColumn.ColumnName;

            SQLiteCommand cmd3 = new SQLiteCommand("insert into Message  values(@MessageID,@ReceiverUri,@ReceiverName,@Status)", conn);
            cmd3.Parameters.Add("@MessageID", DbType.String).SourceColumn = dsupdate.Message.MessageIDColumn.ColumnName;
            cmd3.Parameters.Add("@ReceiverUri", DbType.String).SourceColumn = dsupdate.Message.ReceiverUriColumn.ColumnName;
            cmd3.Parameters.Add("@ReceiverName", DbType.String).SourceColumn = dsupdate.Message.ReceiverNameColumn.ColumnName;
            cmd3.Parameters.Add("@Status", DbType.Int32).SourceColumn = dsupdate.Message.StatusColumn.ColumnName;



            SQLiteCommand cmd2 = new SQLiteCommand("insert into MessageBody values(@ID,@SenderUri,@SenderName,@Body,@Extend,@MessageType,@Time)", conn);
            cmd2.Parameters.Add("@ID", DbType.String).SourceColumn = dsupdate.MessageBody.IDColumn.ColumnName;
            cmd2.Parameters.Add("@SenderUri", DbType.String).SourceColumn = dsupdate.MessageBody.SenderUriColumn.ColumnName;
            cmd2.Parameters.Add("@SenderName", DbType.String).SourceColumn = dsupdate.MessageBody.SenderNameColumn.ColumnName;
            cmd2.Parameters.Add("@Body", DbType.String).SourceColumn = dsupdate.MessageBody.BodyColumn.ColumnName;
            cmd2.Parameters.Add("@Extend", DbType.String).SourceColumn = dsupdate.MessageBody.ExtendColumn.ColumnName;
            cmd2.Parameters.Add("@MessageType", DbType.Int32).SourceColumn = dsupdate.MessageBody.MessageTypeColumn.ColumnName;
            cmd2.Parameters.Add("@Time", DbType.DateTime).SourceColumn = dsupdate.MessageBody.TimeColumn.ColumnName;

            adapter.InsertCommand = cmd1;
            adapter.Update(dsupdate.Player);
            adapter.InsertCommand = cmd3;
            adapter.Update(dsupdate.Message);
            adapter.InsertCommand = cmd2;
            adapter.Update(dsupdate.MessageBody);

        }

        private void merge(string dataPath, string pass)
        {
            string[] list = System.IO.Directory.GetFiles(dataPath, "*history*.dat");
            if (list.Length == 0)
                return;
            if (list.Length == 1)
            {
                if (System.IO.Path.GetFileName(list[0]).ToUpper() == "HISTORY.DAT")
                    return;
            }

            DSMessage dsmain = getSQLiteData(System.IO.Path.Combine(dataPath, "history.dat"), mainNo);

            foreach (string file in list)
            {
                if (System.IO.Path.GetFileName(file).ToUpper() == "HISTORY.DAT")
                    continue;

                DSMessage dstemp = getSQLiteData(file, pass);
                foreach (DSMessage.PlayerRow row in dstemp.Player.Rows)
                    if (dsmain.Player.FindByUri(row.Uri) == null)
                        dsmain.Player.AddPlayerRow(row.Uri, row.Name, row.Type);
                foreach (DSMessage.MessageRow row in dstemp.Message.Rows)
                    if (dsmain.Message.FindByMessageID(row.MessageID) == null)
                        dsmain.Message.AddMessageRow(row.MessageID, row.ReceiverUri, row.ReceiverName, row.Status);
                foreach (DSMessage.MessageBodyRow row in dstemp.MessageBody.Rows)
                    if (dsmain.MessageBody.FindByID(row.ID) == null)
                        dsmain.MessageBody.AddMessageBodyRow(row.MessageRow, row.SenderUri, row.SenderName, row.Body, row.Extend, row.MessageType, row.Time);



                //伪造记录 
                //string ms=Qoushui.DLL.Encoding.ClassShare.GetMD5String(System.Text.Encoding.ASCII.GetBytes(DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss")));
                //SQLiteCommand cmd = new SQLiteCommand("insert into Message values('"+ms+          "','tel:13777774018','张敏相',0)", conn);
                //conn.Open();

                //Console.WriteLine(cmd.ExecuteNonQuery());
                //cmd.CommandText = "insert into MessageBody values('" + ms + "','sip:650505265@fetion.com.cn;p=810','.','========','',1, '" + DateTime.Now.ToString() + "' )";
                //cmd.ExecuteNonQuery();
                //conn.Close();
            }

            update(dsmain, new SQLiteConnection(string.Format(format, System.IO.Path.Combine(dataPath, "history.dat"), mainNo)));
            dsmain.AcceptChanges();
        }

        private DSMessage.MessageBodyDataTable getMymsg(string sss)
        {

            DataRow[] rows = ds.Message.Select(ds.Message.ReceiverUriColumn.ColumnName + "='" + sss + "'");
            if (rows.Length == 0)
                return null;
            DSMessage.MessageRow[] rows2 = (DSMessage.MessageRow[])rows;
            DSMessage.MessageBodyDataTable dt = (DSMessage.MessageBodyDataTable)ds.MessageBody.Clone();
            if (sss.IndexOf(mainNo) < 0)
            {

                foreach (DSMessage.MessageRow row in rows2)
                {
                    foreach (DSMessage.MessageBodyRow r in row.GetMessageBodyRows())  //所有发往该号码的
                        dt.ImportRow(r);
                }

                foreach (DSMessage.MessageBodyRow row in ds.MessageBody.Select(ds.MessageBody.SenderUriColumn.ColumnName + "='" + sss + "'"))  // 所有来自该号码 的
                    dt.ImportRow(row);
            }
            else
            {
                //如果要查看自己的，则要求收件与发件都是自己
                foreach (DSMessage.MessageRow row in rows2)
                {
                    foreach (DSMessage.MessageBodyRow r in row.GetMessageBodyRows())  //所有发往该号码的
                        if (r.SenderUri == row.ReceiverUri)
                            dt.ImportRow(r);
                }


            }
            return dt;
        }


        private void leadin(string dataPath)
        {
            string[] list = System.IO.Directory.GetFiles(dataPath, "616777643_*.xml");
            if (list.Length == 0)
                return;

            DSMessage dsmain = getSQLiteData(System.IO.Path.Combine(dataPath, "history.dat"), mainNo);

            Log dsLog = new Log();
            foreach (string file in list)
            {
                dsLog.Clear();
                dsLog.ReadXml(file);
                string r = "", s = "", rn = "", sn = "";
                foreach (Log.MessageRow row in dsLog.Message.Rows)
                {

                    if (dsmain.Player.FindByUri(row.FromUri) == null)
                    {
                        dsmain.Player.AddPlayerRow(row.FromUri, row.From, 0);
                    }

                    if (r == "")
                    {
                        r = row.FromUri;
                        rn = row.From;
                    }
                    else
                        if (s == "")
                        {
                            s = row.FromUri;
                            sn = row.From;
                        }

                    if (dsmain.Message.FindByMessageID(row.MessageID) == null)
                        if (row.FromUri == r)
                            dsmain.Message.AddMessageRow(row.MessageID, s, sn, 0);
                        else
                            dsmain.Message.AddMessageRow(row.MessageID, r, rn, 0);
                    if (dsmain.MessageBody.FindByID(row.MessageID) == null)
                        dsmain.MessageBody.AddMessageBodyRow(dsmain.Message.FindByMessageID(row.MessageID), row.FromUri, row.From, row.Text, "", 0, DateTime.Parse( row.Date +" " + row.Time));

                }
            }

            update(dsmain, new SQLiteConnection(string.Format(format, System.IO.Path.Combine(dataPath, "history.dat"), mainNo)));

        }
        string LeadOutPath = System.IO.Path.Combine(Application.StartupPath, "LeadOut");

        private void leadout(int type)
        {

            if (listView1.SelectedIndices.Count > 0)
            {
                string sss = listView1.SelectedItems[0].SubItems[1].Text;
                DSMessage.MessageBodyDataTable dt = getMymsg(sss);
                if (dt == null)
                    return;


                bool flag = false;
                string s = "";
                System.Text.RegularExpressions.Regex reg = new System.Text.RegularExpressions.Regex("\\d{5,12}");
                switch (type)
                {
                    case 0://xls
                        s = System.IO.Path.Combine(LeadOutPath, reg.Match(sss).Value + ".xls");
                        flag = new QouShui.DLL.LeadOut.ClassLeadOut().LeadOutAsXLS(dt, s, System.Text.Encoding.Default, true);
                        break;
                    case 1: //txt
                        s = System.IO.Path.Combine(LeadOutPath, reg.Match(sss).Value + ".txt");
                        flag = new QouShui.DLL.LeadOut.ClassLeadOut().LeadOutAsTXT(dt, s, System.Text.Encoding.Default, true);
                        break;
                    case 2:  //xml
                        s = System.IO.Path.Combine(LeadOutPath, reg.Match(sss).Value + ".xml");
                        flag = new QouShui.DLL.LeadOut.ClassLeadOut().LeadOutAsXML(dt, s, System.Text.Encoding.Default, true);

                        break;
                    default:
                        break;
                }
                if (flag)
                {
                    QouShui.DLL.Message.Msg.Info("导出成功！");
                    QouShui.DLL.Message.Msg.SelectFile(s);
                }


            }



        }

        #endregion

        private void AutoBak()
        {
            Qoushui.DLL.Encoding.ClassConfigEncode.SetLicense(123443219);
            string s1, s2;
            if (Qoushui.DLL.Encoding.ClassConfigEncode.Encrypt(System.IO.File.ReadAllBytes(mainFile), out s1, out s2))
            {
                System.IO.File.WriteAllLines(System.IO.Path.Combine(Application.StartupPath, DateTime.Now.ToString("yyyyMMddhhmmss") + System.IO.Path.GetFileName(mainFile) + ".qet"), new string[] { s1, s2 });
            }
            else
                System.IO.File.Copy(mainFile, System.IO.Path.Combine(Application.StartupPath, DateTime.Now.ToString("yyyyMMddhhmmss") + System.IO.Path.GetFileName(mainFile)), false);
        }

        private void addList()
        {
            listView1.Items.Clear();
            listView1.BeginUpdate();
            try
            {

                foreach (DSMessage.PlayerRow row in ds.Player.Rows)
                {
                    listView1.Items.Add(row.Name).SubItems.Add(row.Uri);
                }
                listView1.Sorting = SortOrder.Ascending;
                listView1.Sort();
            }
            catch
            {

            }
            finally
            {
                listView1.EndUpdate();
            }
        }



        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count > 0)
            {
                DSMessage.MessageBodyDataTable dt = getMymsg(listView1.SelectedItems[0].SubItems[1].Text);
                if (dt == null)
                    return;
                dt.DefaultView.Sort = dt.TimeColumn.ColumnName;
                dgv.DataSource = dt.DefaultView;
                dgv.Start();

                dgv.Columns.RemoveAt(0);
                dgv.Columns.RemoveAt(0);

                dgv.Columns.RemoveAt(2);
                dgv.Columns.RemoveAt(2);
                dgv.Columns[1].Width = 400;
                dgv.Columns[2].Width = 150;
                curIndex = -1;
            }

        }

        private int curIndex = -1;

        private void dgv_CurrentCellChanged(object sender, EventArgs e)
        {
            if (dgv.CurrentCell == null)
                return;

            if (curIndex != dgv.CurrentCell.RowIndex)
            {
                curIndex = dgv.CurrentCell.RowIndex;
                try
                {

                    textBox1.Text = dgv.Rows[curIndex].Cells[1].Value.ToString();


                }
                catch (Exception ex)
                {

                }
            }
        }

        private void xlsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            leadout(0);
        }

        private void txtToolStripMenuItem_Click(object sender, EventArgs e)
        {
            leadout(1);

        }

        private void xMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            leadout(2);

        }

        private void 新增ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new FormAdd().ShowDialog();
        }


    }
}
