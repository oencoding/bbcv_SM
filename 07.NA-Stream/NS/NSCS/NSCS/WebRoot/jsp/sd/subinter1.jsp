<%@ page language="java" import="java.util.*,org.slf4j.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>


<%request.setCharacterEncoding("UTF-8"); %>
<%response.setHeader("Charset","UTF-8");%>

<%response.setCharacterEncoding("UTF-8"); %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
 <meta http-equiv="content-type" content="text/html; charset=UTF-8">
<title>vod1页面</title>


	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
	<script type="text/javascript" src="js/jquery.min.js"></script>
	<script src='js/jquery-latest.js'></script>
<% response.setContentType("text/html;charset=UTF-8");%>

<style>
*{	padding:0;
	margin:0;}
body{
	margin:0;
	padding:0;
	font-size:28px;	
	font:bold 28px "Microsoft YaHei"
}
.container{
	width:720px;
	height:576px;
	position:relative;
	background:#00f url("css/adspecial.jpg") no-repeat;
	overflow:hidden;
}
.logo{
	position: absolute;
	left: 10px;
	top: 20px;
	font-size: 26px;
	font-weight:bold;
	color:#fff;
	//text-shadow: 0 0 5px #fff, 0 0 10px #fff, 0 0 15px #fff, 0 0 40px #ff00de, 0 0 70px #ff00de;
	text-shadow: 1px 1px rgba(197, 223, 248,0.8),2px 2px rgba(197, 223, 248,0.8),3px 3px rgba(197, 223, 248,0.8),4px 4px rgba(197, 223, 248,0.8),5px 5px rgba(197, 223, 248,0.8),6px 6px rgba(197, 223, 248,0.8);
}
.container p.text{
	width: 100%;
	height: 60px;
	position:absolute;
	left:0;
	bottom:0;
	text-align: center;	
	text-shadow: 0 1px 1px #000;
}
.container p.text a{
	color: #fff;
	font-weight: 900;
	font-size: 20px;
	text-decoration: none;
	<!--box-shadow: 0 0 20px 5px #fff;-->	
}
.video{
	position: absolute;
	left: 180px;
	top: 75px;
	width:464px;
	height:379px;
	background:#000;
	border-radius:5px;
}
.video video{
	width:464px;
	height:379px;
}
</style>

<% String  usernamenow =request.getParameter("username");	%>
<% String  streamidnow =request.getParameter("streamid");	%>
<% String  vodidnow =request.getParameter("vodid");	%>
 <script>


 <%Logger logger = LoggerFactory.getLogger("");
 
 
 logger.info("【标清等待点播页日志】"); 
 logger.info("usernamenow="+usernamenow+""); 
 logger.info("streamidnow="+streamidnow+""); 
 logger.info("vodidnow="+vodidnow+""); 
 %>
 //定义详情id;


 function queryStatus(){    

  var url ="<%=basePath%>mobile/ShowVodInfo?flag=get&streamid=<%=streamidnow%>&vodid=<%=vodidnow%>&username=<%=usernamenow%>&resolution=1"; 
   
     $.post(url, function(data) {
    	   if(data!=""&&"error"!=data.trim()){
        	  
    		   window.location.href=data;
    	        }	
 	});
		
   
	}	

setInterval("queryStatus();",2000); 
    
</script>










 <% String  streamid1 = request.getParameter("id"); %>






  </head>
  	<% String  username =request.getParameter("username");	
	if(username==null){
		username="";
	}else{
		
		if(username.length()==11){
			  
				String str = username.substring(0, 3);				
				String str1 = username.substring(7,11);
				username= str+"****"+str1;
						
		}
		
	}
	
	%>
	
	<% String base = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+"/";%>
     <% base = base+"RCM"+"/"; %>
  <body>
  
  <div class="container">
<div class="logo">单向终端云点播</div>
<div class="video">
	<video src="<%=base %>res/video/philips.mp4" autoplay="true" loop poster="img/vposter.jpg"></video>
</div>
	<p class="text">
	<a href="#" id="id1" style="display: block;">等待用户<%=username%>点播（<span id="endtime">90</span>）</a>
	<a href="#" id="id2" style="display: none;"><span style='color:darkorange' >节目即将播放...</span></a>
	</p>

</div>
  
  
  


<script type="text/javascript">




var CID = "endtime";

if(window.CID != null)
{
    var iTime = document.getElementById(CID).innerText;
    var Account;
    RemainTime();
}


function RemainTime()
{
	
	 $.ajax( {
	        url: "TransitionServlet?method=2&username=<%=usernamenow%>", //这里是静态页的地址
	        type: "GET", //静态页用get方法，否则服务器会抛出405错误
	        success: function(data){
	
		 if(data.trim()=="success"){
			 iTime=-1;
			 }
     	//document.getElementById(CID).innerHTML = "<span style='color:red'>我收不了啦！</span>";  

	        }
	});

    var sSecond="",sTime="";
    if (iTime >= 0)
    {
    
    	 //  document.getElementById("id2").style.display="none";//显示
        if (iTime >= 0){
            sSecond = iTime + "秒";
            sTime="<span style='color:darkorange'>" + sSecond + "</font>";
        }

        if(iTime==0){
            clearTimeout(Account);
         	<% String  streamid =request.getParameter("streamid");%> 

         	 window.location.href="<%=basePath%>OverTimeServlet?streamid="+<%=streamid%>+"";
          }
        else
        {
            Account = setTimeout("RemainTime()",1000);
        }
        iTime=iTime-1;
    }
    else
    {
     //  sTime="<span style='color:darkorange'>精彩马上呈现";
     document.getElementById("id1").style.display="none";//显示
     document.getElementById("id2").style.display="";//显示
 
     
   // alert(iTime1);
          //  document.getElementById(CID).innerHTML = "<span style='color:red'>我收不了啦！</span>";  

       	 $.ajax( {
 	        url: "<%=basePath%>TransitionServlet?method=3&username=<%=usernamenow%>", //这里是静态页的地址
 	        type: "GET", //静态页用get方法，否则服务器会抛出405错误
 	        success: function(data){

 	        }
 	});
       
    }
    document.getElementById(CID).innerHTML = sTime;
}




</script>



  </body>
</html>
