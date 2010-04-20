using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;

public partial class pic_Register : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
    protected void Button1_Click(object sender, EventArgs e)
    {
        SqlConnection mycon = new SqlConnection("Data Source=(local);DataBase=Labsystem;User id=sa1;pwd=sa1");
        SqlCommand mycom = new SqlCommand("select * from [User] where [Uid]='TextBox2.Text'",mycon);
        
        mycon.Open();
        int count = Convert.ToInt32(mycom.ExecuteScalar());
        if(count>0)
        {
            Response.Write("<script>alert('该用户已存在！')；location='javascript:history.go(-1)'</script>");        
        }
        SqlCommand mycom1 = new SqlCommand("select * from [Uinfo] where [Uno]='TextBox7.Text'",mycon);
        int count1=Convert.ToInt32(mycom1.ExecuteScalar());
        if(count1<0)
        {
            Response.Write("<script>alert('该学号不存在！')；location='javascript:history.go(-1)'</script>"); 
        
        }
        else
        {
            string sqlstr1 = null;
            sqlstr1 = "insert into [User](Uidentity,Uname,Uid,pwd,email,phone)values('" + DropDownList1.SelectedItem.Value + "','" + TextBox1.Text + "'," + TextBox2.Text + "','" + TextBox3.Text + "','" + TextBox5.Text + "','" + TextBox6.Text + "')";

            SqlCommand com = new SqlCommand(sqlstr1, mycon);
            com.ExecuteNonQuery();
            Response.Write("<script>alert('注册成功')</script>");
            mycon.Close();
        }
        
    }
    protected void Button2_Click(object sender, EventArgs e)
    {
        TextBox1.Text = String.Empty;
        TextBox2.Text = String.Empty;
        TextBox3.Text = String.Empty;
        TextBox4.Text = String.Empty;
        TextBox5.Text = String.Empty;
        TextBox6.Text = String.Empty;
        TextBox7.Text = String.Empty;
        DropDownList1.SelectedIndex = 1;
    }
}
