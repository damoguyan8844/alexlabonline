<%@ Page Language="C#" AutoEventWireup="true"  CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
</head>
<body>
    <form id="form1" runat="server">
        <table style="width: 793px; height: 100px" border="2">
            <tr>
                <td style="width: 583px; height: 2px;">
                    <asp:Image ID="Image1" runat="server" src="banner1.jpg" Height="121px" Width="782px"/></td>
            </tr>
        </table>
        <table style="width: 556px; height: 498px" border="2">
            <tr>
                <td style="width: 169691px;height:502px;">
                    <table style="width: 215px; height: 442px; background-image: url(pages/2.jpg);">
                        <tr>
                            <td style="width: 3px; height: 23px">
                                <asp:TreeView ID="TreeView1" runat="server" Height="71px" ImageSet="Arrows" Width="215px">
                                    <ParentNodeStyle Font-Bold="False" />
                                    <HoverNodeStyle Font-Underline="True" ForeColor="#5555DD" />
                                    <SelectedNodeStyle Font-Underline="True" ForeColor="#5555DD" HorizontalPadding="0px"
                                        VerticalPadding="0px" />
                                    <Nodes>
                                        <asp:TreeNode Text="查询实验安排" Value="查询实验安排">
                                            <asp:TreeNode NavigateUrl="~/pages/select1.aspx" Target="MainFrame" Text="按周次查询课表"
                                                Value="按周次查询课表"></asp:TreeNode>
                                            <asp:TreeNode NavigateUrl="~/pages/select1.aspx" Text="按课程查询实验安排" Value="按课程查询实验安排" Target="MainFrame">
                                            </asp:TreeNode>
                                            <asp:TreeNode NavigateUrl="~/pages/select1.aspx" Text="按任课教师查询" Value="按任课教师查询" Target="MainFrame">
                                            </asp:TreeNode>
                                            <asp:TreeNode NavigateUrl="~/pages/select1.aspx" Text="按实验指导老师查询" Value="按实验指导老师查询实验安排" Target="MainFrame">
                                            </asp:TreeNode>
                                            <asp:TreeNode NavigateUrl="~/pages/select1.aspx" Text="按日期查询实验安排" Value="按日期查询实验安排" Target="MainFrame">
                                            </asp:TreeNode>
                                        </asp:TreeNode>
                                    </Nodes>
                                    <NodeStyle Font-Names="Tahoma" Font-Size="10pt" ForeColor="Black" HorizontalPadding="5px"
                                        NodeSpacing="0px" VerticalPadding="0px" />
                                </asp:TreeView>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 3px; height: 22px">
                                <asp:TreeView ID="TreeView2" runat="server" Height="71px" ImageSet="Arrows" Width="215px">
                                    <ParentNodeStyle Font-Bold="False" />
                                    <HoverNodeStyle Font-Underline="True" ForeColor="#5555DD" />
                                    <SelectedNodeStyle Font-Underline="True" ForeColor="#5555DD" HorizontalPadding="0px"
                                        VerticalPadding="0px" />
                                    <Nodes>
                                        <asp:TreeNode Text="查询实验内容" Value="查询实验内容">
                                            <asp:TreeNode Text="按课程查询实验内容" Value="按课程查询实验内容" NavigateUrl="~/pages/ResearchEcontent1.aspx" Target="MainFrame"></asp:TreeNode>
                                            <asp:TreeNode Text="按任课教师查询" Value="按任课教师查询" NavigateUrl="~/pages/ResearchEcontent1.aspx" Target="MainFrame"></asp:TreeNode>
                                            <asp:TreeNode Text="按实验指导老师查询" Value="按实验指导老师查询" NavigateUrl="~/pages/ResearchEcontent1.aspx" Target="MainFrame"></asp:TreeNode>
                                        </asp:TreeNode>
                                    </Nodes>
                                    <NodeStyle Font-Names="Tahoma" Font-Size="10pt" ForeColor="Black" HorizontalPadding="5px"
                                        NodeSpacing="0px" VerticalPadding="0px" />
                                </asp:TreeView>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 3px; height: 4px">
                                <asp:TreeView ID="TreeView3" runat="server" Height="69px" ImageSet="Arrows" Width="214px">
                                    <ParentNodeStyle Font-Bold="False" />
                                    <HoverNodeStyle Font-Underline="True" ForeColor="#5555DD" />
                                    <SelectedNodeStyle Font-Underline="True" ForeColor="#5555DD" HorizontalPadding="0px"
                                        VerticalPadding="0px" />
                                    <Nodes>
                                        <asp:TreeNode Text="查询成绩" Value="查询成绩">
                                            <asp:TreeNode Text="按学生学号查询" Value="按学生学号查询" NavigateUrl="~/pages/ResearchScore.aspx" Target="MainFrame"></asp:TreeNode>
                                            <asp:TreeNode Text="按学生姓名查询" Value="按学生姓名查询" NavigateUrl="~/pages/ResearchScore.aspx" Target="MainFrame"></asp:TreeNode>
                                        </asp:TreeNode>
                                    </Nodes>
                                    <NodeStyle Font-Names="Tahoma" Font-Size="10pt" ForeColor="Black" HorizontalPadding="5px"
                                        NodeSpacing="0px" VerticalPadding="0px" />
                                </asp:TreeView>
                            </td>
                        </tr>
                        <tr>
                            <td style="width: 3px; height: 3px">
                                <asp:TreeView ID="TreeView4" runat="server" Height="72px" ImageSet="Arrows" Width="214px">
                                    <ParentNodeStyle Font-Bold="False" />
                                    <HoverNodeStyle Font-Underline="True" ForeColor="#5555DD" />
                                    <SelectedNodeStyle Font-Underline="True" ForeColor="#5555DD" HorizontalPadding="0px"
                                        VerticalPadding="0px" />
                                    <Nodes>
                                        <asp:TreeNode Text="查询出勤记录" Value="查询出勤记录">
                                            <asp:TreeNode Text="按学生学号查询出勤情况" Value="按学生学号查询出勤情况" NavigateUrl="~/pages/ResearchAttend1.aspx" Target="MainFrame"></asp:TreeNode>
                                            <asp:TreeNode Text="按学生姓名查询出勤情况" Value="按学生姓名查询出勤情况" NavigateUrl="~/pages/ResearchAttend1.aspx" Target="MainFrame"></asp:TreeNode>
                                        </asp:TreeNode>
                                    </Nodes>
                                    <NodeStyle Font-Names="Tahoma" Font-Size="10pt" ForeColor="Black" HorizontalPadding="5px"
                                        NodeSpacing="0px" VerticalPadding="0px" />
                                </asp:TreeView>
                                <br />
                            </td>
                        </tr>
                    </table>
                
                </td>
                <td style="width: 357px;height:502px;">
                <iframe src="pages\Compage.aspx" name ="MainFrame" width="346"  frameborder="1" scrolling="no" height="500" style="width: 552px; background-color: #ccffff; height: 494px;"></iframe>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>
