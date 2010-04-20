<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Compage2.aspx.cs" Inherits="Compage2" %>
<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>查询首页</title>
</head>
<body>
    <form id="form1" runat="server">
        <table style="width: 252px; height: 398px; background-color: #ccffff">
            <tr>
                <td style="width: 3px; height: 23px">
                    <asp:TreeView ID="TreeView1" runat="server" ImageSet="Simple" Height="100px" NodeIndent="10" Width="244px">
                        <ParentNodeStyle Font-Bold="False" />
                        <HoverNodeStyle Font-Underline="True" ForeColor="#DD5555" />
                        <SelectedNodeStyle Font-Underline="True" ForeColor="#DD5555" HorizontalPadding="0px"
                            VerticalPadding="0px" />
                        <Nodes>
                            <asp:TreeNode Text="查询实验安排" Value="查询实验安排">
                                <asp:TreeNode Text="按周次查询课表" Value="按周次查询课表" NavigateUrl="~/Select/tree1.aspx"></asp:TreeNode>
                                <asp:TreeNode Text="按课程查询实验安排" Value="按课程查询实验安排" NavigateUrl="~/Select/tree2.aspx"></asp:TreeNode>
                                <asp:TreeNode Text="按任课教师查询实验安排" Value="按任课教师查询实验安排" NavigateUrl="~/Select/tree3.aspx"></asp:TreeNode>
                                <asp:TreeNode Text="按实验指导老师查询实验安排" Value="按实验指导老师查询实验安排" NavigateUrl="~/Select/tree4.aspx"></asp:TreeNode>
                                <asp:TreeNode Text="按日期查询实验安排" Value="按日期查询实验安排" NavigateUrl="~/Select/tree5.aspx"></asp:TreeNode>
                            </asp:TreeNode>
                   
                        </Nodes>
                        <NodeStyle Font-Names="Verdana" Font-Size="8pt" ForeColor="Black" HorizontalPadding="0px"
                            NodeSpacing="0px" VerticalPadding="0px" />
                    </asp:TreeView>
                </td>
            </tr>
            <tr>
                <td style="width: 3px; height: 22px;">
                    <asp:TreeView ID="TreeView2" runat="server" ImageSet="Simple" Height="71px" NodeIndent="10" Width="244px">
                        <ParentNodeStyle Font-Bold="False" />
                        <HoverNodeStyle Font-Underline="True" ForeColor="#DD5555" />
                        <SelectedNodeStyle Font-Underline="True" ForeColor="#DD5555" HorizontalPadding="0px"
                            VerticalPadding="0px" />
                        <Nodes>
                            <asp:TreeNode Text="查询实验内容" Value="查询实验内容">
                                <asp:TreeNode Text="按课程查询实验内容" Value="按课程查询实验内容"></asp:TreeNode>
                                <asp:TreeNode Text="按任课教师查询实验内容" Value="按任课教师查询实验内容"></asp:TreeNode>
                                <asp:TreeNode Text="按实验指导老师查询实验内容" Value="按实验指导老师查询实验内容"></asp:TreeNode>
                            </asp:TreeNode>
                        </Nodes>
                        <NodeStyle Font-Names="Verdana" Font-Size="8pt" ForeColor="Black" HorizontalPadding="0px"
                            NodeSpacing="0px" VerticalPadding="0px" />
                    </asp:TreeView>
                </td>
            </tr>
            <tr>
                <td style="width: 3px; height: 4px">
                    <asp:TreeView ID="TreeView3" runat="server" ImageSet="Simple" Height="69px" NodeIndent="10" Width="244px">
                        <ParentNodeStyle Font-Bold="False" />
                        <HoverNodeStyle Font-Underline="True" ForeColor="#DD5555" />
                        <SelectedNodeStyle Font-Underline="True" ForeColor="#DD5555" HorizontalPadding="0px"
                            VerticalPadding="0px" />
                        <Nodes>
                            <asp:TreeNode Text="查询成绩" Value="查询成绩">
                                <asp:TreeNode Text="按学生学号查询" Value="按学生学号查询"></asp:TreeNode>
                                <asp:TreeNode Text="按学生姓名查询" Value="按学生姓名查询"></asp:TreeNode>
                            </asp:TreeNode>
                        </Nodes>
                        <NodeStyle Font-Names="Verdana" Font-Size="8pt" ForeColor="Black" HorizontalPadding="0px"
                            NodeSpacing="0px" VerticalPadding="0px" />
                    </asp:TreeView>
                </td>
            </tr>
            <tr>
                <td style="width: 3px; height: 3px">
                    <asp:TreeView ID="TreeView4" runat="server" ImageSet="Simple" Height="72px" NodeIndent="10" Width="244px">
                        <ParentNodeStyle Font-Bold="False" />
                        <HoverNodeStyle Font-Underline="True" ForeColor="#DD5555" />
                        <SelectedNodeStyle Font-Underline="True" ForeColor="#DD5555" HorizontalPadding="0px"
                            VerticalPadding="0px" />
                        <Nodes>
                            <asp:TreeNode Text="查询出勤记录" Value="查询出勤记录">
                                <asp:TreeNode Text="按学生学号查询出勤情况" Value="按学生学号查询出勤情况"></asp:TreeNode>
                                <asp:TreeNode Text="按学生姓名查询出勤情况" Value="按学生姓名查询出勤情况"></asp:TreeNode>
                            </asp:TreeNode>
                        </Nodes>
                        <NodeStyle Font-Names="Verdana" Font-Size="8pt" ForeColor="Black" HorizontalPadding="0px"
                            NodeSpacing="0px" VerticalPadding="0px" />
                    </asp:TreeView>
                    <br />
                    <br />
                    <br />
                </td>
            </tr>
        </table>
        <br />
    </form>
</body>
</html>
