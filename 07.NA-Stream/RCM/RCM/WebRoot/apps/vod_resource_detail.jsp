<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %> 
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">

    
    
    <% String flag = request.getParameter("flag");
    String id = request.getParameter("id");
    
    if("add".equals(flag)){
    	
    	request.setAttribute("method","add");
    	request.setAttribute("methodStr","添加");
    }else{
    	
    	request.setAttribute("method","updateUrl");
    	request.setAttribute("methodStr","修改");	
    	
    }
    
    
    
    %>
    
    <title>tv导航模板添加或修改</title>
   <meta http-equiv="content-type" content="text/html; charset=UTF-8">
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
    
    



<script language="javascript" type="text/javascript" src="<%=basePath%>dist/My97DatePicker/WdatePicker.js"></script>




	    <script type="text/javascript" src="js/common.js"></script>
	    
    <link rel="stylesheet" type="text/css" href="css/style.css" />


	

  </head>
  
  <body style="background: url(<%=basePath%>apps/images/1_right_1.jpg) repeat-x; padding: 3px 6px 0 6px;">
    <table id="navTbl" width="100%" cellspacing="0" cellpadding="0"
		class="navTbl">
		<tr>
		    <td width="10">
				<img src="images/1_ico1.gif" width="26" height="9">
			</td>
			<td class="navtd">

				当前位置：点播资源&gt;&gt;VOD资源&gt;&gt;VOD资源详情</td>
			<td align="right">
				<a href="javaScript:window.history.go(-1);"><img src="<%=basePath%>apps/images/ico/7.gif" alt="返回" border="0"></a>
				<a><img src="<%=basePath%>apps/images/ico/a6.gif" alt="帮助" border="0">
				</a>
			</td>
			<td width="10"></td>
		</tr>
	</table>
  
  <!-- 显示信息-->	
	<div align="left">	
		<font color="red"  size="2">
		</font>		
	</div>
  <br>
   

     <table width="70%"  cellpadding="0" cellspacing="0"
		 style="margin:10px 0; border:1px solid #BECFD6;" align="center" >

		<tr>
			<td nowrap class="tab02"  id="vodname" align="right">
				分辨率：
			<br/></td>
			<td class="tab01" >
		         <c:if test="${tvNavigate.resolution=='0'}">高清</c:if>
			     <c:if test="${tvNavigate.resolution=='1'}">标清</c:if>
				
			<br/></td>
		</tr>


		<tr>
			<td nowrap class="tab02" align="right">
				名称：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.name}
			<br/></td>
		</tr>
		<tr>
			<td nowrap class="tab02" align="right">
				评分：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.grade}
			<br/></td>
		</tr>
		<tr>
			<td nowrap class="tab02" align="right">
				导演：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.director}
			<br/></td>
		</tr>

		<tr>
			<td nowrap class="tab02" align="right">
				主演：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.actor}
			<br/></td>
		</tr>
				<tr>
			<td nowrap class="tab02" align="right">
				年份：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.years}
		
			<br/></td>
		</tr>
	
			<tr>
			<td nowrap class="tab02" align="right">
				时长：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.runtime}
			<br/></td>
		</tr>
			
			<tr>
			<td nowrap class="tab02" align="right">
				类型：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.type}
			<br/></td>
		</tr>
					<tr>
			<td nowrap class="tab02" align="right">
				地区：
			<br/></td>
			<td class="tab01" >
			${tvNavigate.area}
			<br/></td>
		</tr>
				<tr>
			<td nowrap class="tab02" align="right">
				描述：
			<br/></td>
			<td class="tab01" >
			<textarea rows="5" cols="100">${tvNavigate.description}</textarea>
		
				
			<br/></td>
		</tr>
				<tr>
			<td nowrap class="tab02" align="right">
				地址：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.path}
			<br/></td>
		</tr>
				<tr>
			<td nowrap class="tab02" align="right">
				大海报地址：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.bigPosterPath}
			<br/></td>
		</tr>
				<tr>
			<td nowrap class="tab02" align="right">
				小海报地址：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.littlePosterPath}
			<br/></td>
		</tr>
				<tr>
			<td nowrap class="tab02" align="right">
				rtspUrl地址：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.rtspUrl}
			<br/></td>
		</tr>

				<tr>
			<td nowrap class="tab02" align="right">
				其他信息：
			<br/></td>
			<td class="tab01" >
				${tvNavigate.other}
			<br/></td>
		</tr>		
		<tr>
			<td colspan="6" nowrap align="center" >
            
              <input type="button" name="button1" value="返回" onclick="javaScript:window.history.go(-1);">
            </td>
        </tr>
		
	</table>

  </body>

</html>
