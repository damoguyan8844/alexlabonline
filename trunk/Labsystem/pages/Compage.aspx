<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Compage.aspx.cs" Inherits="Compage" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>登录页面</title>
<script language="javascript" type="text/javascript">
// <!CDATA[

// ]]>
</script>
</head>
<body>
    <form id="form1" runat="server">
        <table style="width: 506px; height: 479px">
            <tr>
                <td style="width: 89px">
                </td>
                <td style="width: 374px">
                    <table style="width: 355px; height: 60px; background-image: url(1.jpg);" border="1">
                        <tr>
                            <td style="width: 389px; height: 5px; font-size: 16px; font-family: 黑体;">
                                &nbsp;<br />
                                &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp;&nbsp; 系统登录<br />
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 389px; height: 19px">
                                <table style="width: 270px; height: 80px" border="1">
                                    <tr>
                                        <td style="width: 270px; height: 10px; font-size: 12px; color: #000000; font-family: 新宋体;">
                                            身 份：</td>
                                        <td style="width: 148px; height: 10px">
                                            <asp:DropDownList ID="DropDownList1" runat="server" Width="111px">
                                                <asp:ListItem Selected="True">授课教师</asp:ListItem>
                                                <asp:ListItem>实验指导老师</asp:ListItem>
                                                <asp:ListItem>实验室管理人员</asp:ListItem>
                                                <asp:ListItem>系统管理员</asp:ListItem>
                                            </asp:DropDownList></td>
                                    </tr>
                                    <tr>
                                        <td style="width: 270px; height: 1px; font-size: 12px;">
                                            账 号：</td>
                                        <td style="width: 148px; height: 1px">
                                            <asp:TextBox ID="TextBox1" runat="server" Width="105px"></asp:TextBox>
                                            </td>
                                    </tr>
                                    <tr>
                                        <td style="width: 270px; font-size: 12px;">
                                            密 码：</td>
                                        <td style="width: 148px;">
                                            <asp:TextBox ID="TextBox2" runat="server" Width="105px" TextMode="Password"></asp:TextBox>
                                            &nbsp;</td>
                                    </tr>
                                    <tr>
                                        <td style="width: 270px; height: 5px; font-size: 12px;">
                                            验证码：</td>
                                        <td style="width: 148px; height: 5px">
                                            <asp:TextBox ID="TextBox3" runat="server" Width="105px"></asp:TextBox>
                                            </td>
                                        <td style="width: 204px; height: 5px">
                                        <img id="IMG1" runat="server" src="checkcode.aspx" alt=""/>
                                            </td>    
                                    </tr>
                                </table>
                                </td>
                                </tr>
                        <tr>
                            <td style="width: 389px; height: 17px">
                                <table style="width: 347px; height: 71px">
                                    <tr>
                                        <td style="width: 156px">
                                        </td>
                                        <td>
                                        </td>
                                        <td>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="width: 156px">
                                            &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                                &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;<asp:Button ID="Button2" runat="server" Text="注册" OnClick="Button2_Click" /></td>
                                        <td>
                                        </td>
                                        <td>
                                &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="登录" Width="40px" /></td>
                                    </tr>
                                    <tr>
                                        <td style="width: 156px">
                                        </td>
                                        <td>
                                        </td>
                                        <td>
                                        </td>
                                    </tr>
                                </table>
                                &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp;</td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
        &nbsp;
    </form>
</body>
</html>
