

<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Register.aspx.cs" Inherits="pic_Register"%>

<%@ Register Assembly="Microsoft.ReportViewer.WebForms, Version=8.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a"
    Namespace="Microsoft.Reporting.WebForms" TagPrefix="rsweb" %>
<%@ Register Assembly="CrystalDecisions.Web, Version=10.2.3600.0, Culture=neutral, PublicKeyToken=692fbea5521e1304"
    Namespace="CrystalDecisions.Web" TagPrefix="CR" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
<script language="javascript" type="text/javascript">
// <!CDATA[


// ]]>
</script>

    <link href="/aspnet_client/System_Web/2_0_50727/CrystalReportWebFormViewer3/css/default.css"
        rel="stylesheet" type="text/css" />
</head>
<body>
    <form id="form1" runat="server">
        <br />
        <table style="width: 501px; height: 89px" border="1">
            <tr>
                <td style="width: 36px; height: 206px;">
                    <table style="width: 448px; height: 52px">
                        <tr>
                            <td style="width: 173px; height: 20px;">
                                &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;新用户注册 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                                &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
                                &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;</td>
                        </tr>
                    </table>
                    <br />
                    <table style="width: 474px; height: 25px" border="1">
                        <tr>
                            <td style="width: 125px; height: 8px;">
                                身份：</td>
                            <td style="width: 297px; height: 8px;">
                                &nbsp;<asp:DropDownList ID="DropDownList1" runat="server" Width="135px">
                                    <asp:ListItem Selected="True">任课教师</asp:ListItem>
                                    <asp:ListItem>实验指导老师</asp:ListItem>
                                    <asp:ListItem>实验室管理人员</asp:ListItem>
                                    <asp:ListItem>系统管理人员</asp:ListItem>
                                </asp:DropDownList></td>
                        </tr>
                        <tr>
                            <td style="width: 125px;">
                                姓名：</td>
                            <td style="width: 297px;">
                                <asp:TextBox ID="TextBox1" runat="server" Width="135px"></asp:TextBox>
                                <asp:Label ID="label" runat="server" Width="3px" ForeColor="red">*</asp:Label>
                                <asp:RequiredFieldValidator ID="RequiredFieldValidator1" runat="server" ErrorMessage="姓名不能为空" ControlToValidate="TextBox1"></asp:RequiredFieldValidator></td>
                        </tr>
                        <tr>
                            <td style="width: 125px; height: 1px">
                                学号：</td>
                            <td style="width: 297px; height: 1px">
                                <asp:TextBox ID="TextBox7" runat="server" Width="135px"></asp:TextBox>
                                <asp:Label ID="label1" runat="server" Width="3px" ForeColor="red">*</asp:Label>
                                <asp:RequiredFieldValidator ID="RequiredFieldValidator5" runat="server" ErrorMessage="学号不能为空" ControlToValidate="TextBox2"></asp:RequiredFieldValidator></td>
                        </tr>
                        <tr>
                            <td style="width: 125px; height: 10px">
                                账号：</td>
                            <td style="width: 297px; height: 10px">
                                <asp:TextBox ID="TextBox2" runat="server" Width="135px"></asp:TextBox>
                                <asp:Label ID="label2" runat="server" Width="3px" ForeColor="red" BorderColor="white">*</asp:Label>
                                <asp:RequiredFieldValidator ID="RequiredFieldValidator2" runat="server" ErrorMessage="账号不能为空" ControlToValidate="TextBox2"></asp:RequiredFieldValidator></td>
                        </tr>
                        <tr>
                            <td style="width: 125px; height: 2px">
                                密码：</td>
                            <td style="width: 297px; height: 2px">
                                <asp:TextBox ID="TextBox3" runat="server" Width="135px" TextMode="Password"></asp:TextBox>
                                <asp:Label ID="label3" runat="server" Width="3px" ForeColor="red" BorderColor="white">*</asp:Label>
                                <asp:RequiredFieldValidator ID="RequiredFieldValidator3" runat="server" ErrorMessage="密码不能为空" ControlToValidate="TextBox3"
                                    Width="97px"></asp:RequiredFieldValidator></td>
                        </tr>
                        <tr>
                            <td style="width: 125px; height: 21px">
                                重复密码：</td>
                            <td style="width: 297px; height: 21px">
                                <asp:TextBox ID="TextBox4" runat="server" Width="135px" TextMode="Password"></asp:TextBox>
                                <asp:Label ID="label4" runat="server" Width="3px" ForeColor="red" BorderColor="white">*</asp:Label>
                                <asp:CompareValidator ID="CompareValidator1" runat="server" ErrorMessage="输入不一致"
                                    Width="85px" ControlToValidate="TextBox4" ControlToCompare="TextBox3"></asp:CompareValidator></td>
                        </tr>
                        <tr>
                            <td style="width: 125px; height: 21px">
                                E_mail：</td>
                            <td style="width: 297px; height: 21px">
                                <asp:TextBox ID="TextBox5" runat="server" Width="135px"></asp:TextBox>
                                <asp:Label ID="label5" runat="server" Width="3px" ForeColor="red" BorderColor="white">*</asp:Label>
                                <asp:RegularExpressionValidator ID="RegularExpressionValidator1" runat="server" ErrorMessage="请输入邮箱" ValidationExpression="\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*"
                                    ControlToValidate="TextBox5" Width="81px"></asp:RegularExpressionValidator></td>
                        </tr>
                        <tr>
                            <td style="width: 125px;">
                                电话：</td>
                            <td style="width: 297px;">
                                <asp:TextBox ID="TextBox6" runat="server" Width="135px"></asp:TextBox>
                                <asp:Label ID="label6" runat="server" Width="3px" ForeColor="red" BorderColor="white">*</asp:Label>
                                <asp:RequiredFieldValidator ID="RequiredFieldValidator4" runat="server" ErrorMessage="请输入电话" ControlToValidate="TextBox6"
                                    Width="83px"></asp:RequiredFieldValidator></td>
                        </tr>
                        
                        
                    </table>
                    &nbsp;&nbsp;&nbsp;
                    <table style="width: 449px; height: 41px">
                        <tr>
                            <td style="width: 135px; height: 20px;">
                                <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="注册" />
                                <asp:Button ID="Button2" runat="server" OnClick="Button2_Click" Text="重置" /></td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
        &nbsp;&nbsp;
    </form>
</body>
</html>
