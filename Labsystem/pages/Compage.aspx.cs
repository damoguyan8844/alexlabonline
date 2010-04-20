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


using System.Drawing;

public partial class Compage : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }

    protected void Button2_Click(object sender, EventArgs e)
    {
        Response.Redirect("Register.aspx", false);

    }

    protected void Button1_Click(object sender, EventArgs e)
    {
        SqlConnection con1 = new SqlConnection("Data Source=(local);DataBase=Labsystem;User id=sa1;pwd=sa1");
        SqlCommand sql1 = new SqlCommand("select * from [User1] where [Uid]='+TextBox1.Text+'", con1);
        SqlCommand sql2 = new SqlCommand();
        sql2.Parameters.Add(new SqlParameter("@id",SqlDbType.Char,20));
        sql2.Parameters.Add(new SqlParameter("@pwd",SqlDbType.VarChar,10));
        sql2.Parameters["@id"].Value = TextBox1.Text;
        sql2.Parameters["@pwd"].Value = TextBox2.Text;
        sql2.CommandText = "select * from [User1] where Uid=@id and pwd =@pwd";
        sql2.Connection = con1;
        con1.Open();
        string count = Convert.ToString(sql2.ExecuteScalar());
        if (count.Length==0)
        {
            Response.Write("<script>alert('该用户不存在或是密码不正确！');window.location='Compage.aspx'</script>");
        }
        else if (TextBox3.Text.Equals(Session["CheckCode"].ToString()))
        {
            if(DropDownList1.SelectedItem.Text.Equals("授课教师"))
            {
                //Response.Redirect("TeachersMainpage.aspx", false);
                Response.Write("<script   Language=\"Javascript\">top.location=\"TeachersMainpage.aspx\";");
                Response.Write("<");
                Response.Write("/");
                Response.Write("script>");
            }
            else if (DropDownList1.SelectedItem.Text.Equals("实验指导老师"))
            {
                //Response.Redirect("TeachersMainpage.aspx", false);
                Response.Write("<script   Language=\"Javascript\">top.location=\"TeachersMainpage.aspx\";");
                Response.Write("<");
                Response.Write("/");
                Response.Write("script>");
            }
            else if (DropDownList1.SelectedItem.Text.Equals("实验室管理人员"))
            {
                //Response.Redirect("CommonAdminister.aspx", false);
                Response.Write("<script   Language=\"Javascript\">top.location=\"CommonAdminister.aspx\";");
                Response.Write("<");
                Response.Write("/");
                Response.Write("script>");
            }
            else if (DropDownList1.SelectedItem.Text.Equals("系统管理员"))
            {
                //Response.Redirect("SystemManager.aspx", false);
                Response.Write("<script   Language=\"Javascript\">top.location=\"SystemManager.aspx\";");
                Response.Write("<");
                Response.Write("/");
                Response.Write("script>");
            }
           
        }
        else
        {
            Response.Write("<script>alert('该用户不存在或是密码不正确！');location='javascript:history.go(-1)'</script>");
        }
        
    }
}
