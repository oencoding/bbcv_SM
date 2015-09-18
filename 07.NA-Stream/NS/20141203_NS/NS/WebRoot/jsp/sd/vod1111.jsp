<%@ page language="java" import="java.util.*,org.slf4j.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<% String base = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+"/";%>
<% base = base+"RCM"+"/"; %>



<html lang="zh-CN">
<head>
    <meta charset="utf-8">
    <meta name="page-view-size" content="1280*720"/>
    <title>城市列表</title>
    <link rel="stylesheet" href="blurbg/style/clstyle.css"/>
    
    
    
<% String  streamid = request.getParameter("id"); %>

<%--打印日志--%>

 <%Logger logger = LoggerFactory.getLogger("【导航页日志】");logger.info("流化TV导航页面【streamid="+streamid+"】，【标清频道】"); %>

 <script>
 
 //定义详情id;
 var vodid;

 function queryStatus(){    

     var steam_id = <%=streamid%>
     $.post("<%=basePath%>skipServlet?resolution=1&streamid="+steam_id+"&vodid="+vodid, function(data) {
    	   if(data!=""&&"error"!=data.trim()){
    	     	  window.location.href=data;   	   
    	        }	
 	});
		
	}	

// queryStatus();
setInterval("queryStatus();",1000); 
		    
</script>    
    
    
    
    
    
    
    
    
    
</head>
<body>
<div class="poetic view">
	<div class="background"></div>
</div>
<div class="clcontainer">
    <div class="clleft">
        <div class="video">
			<div class="dg-wrapper">
				
			</div>
            <video id="video"></video>
        </div>
        <div class="descript">
            <p id="cname"></p>
			<ul class="movieinfo">
				<li class="director">导演</li>
				<li class="actor">主演</li>
				<li class="datetime">上映日期</li>
			</ul>
            <p id="cintro"></p>
            <p id="subtxt"></p>
        </div>
    </div>
    <div class="clright" id="dg-container">
        <div class="logo"></div>
		<div class="information">手机扫描二维码点播</div>		
		<div class="qcode">
			<div id="qrcode"></div>
		</div>
    </div>
    
    <div class='example'>
        <div class="thumb">
            <div class="inner">
                <img src="">
                <div class="title"></div>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript" src="js/jquery-1.11.0.min.js"></script>
<script type="text/javascript" src="<%= base%>res/data/sdtvdata1.js"></script>
<script type="text/javascript" src="js/data1.js"></script>
<script type="text/javascript" src="js/jquery.qrcode.min.js"></script>
<script type="text/javascript">
var $container, instance, citylist = ['fuzhou', 'longyan', 'nanping', 'ningde', 'pingtan', 'putian', 'quanzhou', 'sanming', 'xiamen', 'zhangzhou'], Otimer, textinfo = [], videoSrc = [], nameinfo = [], imgSrc = [],director=[],actor=[],ondate=[];
$(document).ready(function () {
	$container = $("#dg-container");
    //初始化
	init(); 
	var currentIndex  = 0,flag = true;
	var changeIndex = function(){
			currentIndex++;
			if(flag){
				moveCurrent(1);
				if(currentIndex==2){
					currentIndex = 0; 
					flag = false;
				}
			}
			else{
				moveCurrent(-1);
				if(currentIndex==2){
					currentIndex = 0;
					flag = true;
				}
			}				
			setTimeout(changeIndex,3000);
	}
	setTimeout(changeIndex,3000);
    
    //绑定按键操作
    document.onkeydown = grabEvent;
    function grabEvent(event) {
        switch (event.keyCode) {
            case 38://up
                moveCurrent(-1);
                break;
            case 40://down
                moveCurrent(1);
                break;
            case 37://left
                break;
            case 39://right
                break;
            case 13://select
                break;
            case 45: //back
            case 46: //exit
                history.go(-1);
                break;
        }
    }
});

//根据地址栏城市初始化数据
function init() {
    var name = getQueryStringByName('city'),len = info.length;
    for (var i = 0; i < len; i++) {
        var clone = $(".example .thumb").clone(true);
        clone.find('img').attr('src', info['movie' + i].poster);

        clone.find('.title').html(info['movie' + i].name);
        videoSrc[i] = info['movie' + i].videosrc;
        textinfo[i] = "剧情介绍:"+info['movie' + i].movieintro;
        nameinfo[i] = info['movie' + i].name;
        imgSrc[i] = info['movie' + i].poster;
		
		director[i] = info['movie' + i].director;
		actor[i] = info['movie' + i].actor;
		ondate[i] = info['movie' + i].ondata;
        $('.dg-wrapper').append(clone);
    }

    for (; i < 4; i++) {
        $('.dg-wrapper').append('<div class="thumb none"></div>');
    }
	$("#qrcode").qrcode({width:185,height:200,text:"http://192.168.100.11/multiscreen/BBCV_Local-release-1.0.00.92.apk"});	
	$(".thumb").eq(0).addClass("current");
	$(".poetic.view").css("opacity",1);
	moveCurrent(0);
}

var singleHeight = 79 + 8;

function moveCurrent(flag) {
    var $thumb = $(".dg-wrapper .thumb")
    var thumbLen = $thumb.length;
    var noneLen = $(".none").length;

    var len = thumbLen - noneLen;

    var index = $(".dg-wrapper .thumb").index($(".current"));

    index += flag;


    if (index >= 0 && index < len) {
        clearInterval(Otimer);
        $(".current").removeClass("current");
        $thumb.eq(index).addClass("current");
        showfocus(index);
        moveWrapper();
        var oldBg = parent.$('.poetic .background');
        var newBg = $('<div class="background">').css({
            'opacity': '0',
            'background-image': 'url(' + $('.current img').attr('src') + ')',
			'box-shadow':'inset 0 -15px 150px 355px rgba(0,0,0,.5)'

        }).insertAfter(oldBg);
        setTimeout(function () {
            oldBg.css('opacity', 0).one('webkitTransitionEnd', function () {
                $(this).remove();
            });
            newBg.css('opacity', 1);
        }, 0);


    }

    function moveWrapper() {
        $(".dg-wrapper").finish();
        var curTop = $(".current").position().top;
        var wraTop = $(".dg-wrapper").position().top;
        console.log(curTop + wraTop)
        if ((curTop + wraTop) < 0) {
            if (Math.abs(wraTop) <= 3 * singleHeight) {
                $(".dg-wrapper").animate({"top": '0px'}, 300, function () {

                })
            }
            else {
                $(".dg-wrapper").animate({"top": (wraTop + 3 * singleHeight) + 'px'}, 300, function () {

                })
            }
        }
        else if ((curTop + wraTop) >= 3 * singleHeight) {
            if ((thumbLen - index) > 3) {
                $(".dg-wrapper").animate({"top": (wraTop - 3 * singleHeight) + 'px'}, 300, function () {

                })
            }
            else {

                $(".dg-wrapper").animate({"top": (wraTop - (thumbLen - index) * singleHeight) + 'px'}, 300, function () {

                })
            }
        }
    }
}


//获取初始化城市
function getindex() {
    for (var i = 0; i < citylist.length; i++) {
        if (citylist[i] == getQueryStringByName('city')) {
            return i;
        }
    }
    return 2;
}

//获取地址栏中的参数值
function getQueryStringByName(name) {

    var result = location.search.match(new RegExp("[\?\&]" + name + "=([^\&]+)", "i"));

    if (result == null || result.length < 1) {

        return "";

    }

    return result[1];

}
/*根据焦点项目更换视频和文字*/
function showfocus(index) {
    var video = $('#video:first').attr('src', videoSrc[index]).get(0);
    video.play();
    video.loop = true;
    $('#cname').text(nameinfo[index]);
    $('#subtxt').text(textinfo[index]);
	
	$(".movieinfo .director").text("导演:"+director[index]);
	$(".movieinfo .actor").text("主演:"+actor[index]);
	$(".movieinfo .datetime").text("上映日期:"+ondate[index]);
	
    changeText($("#subtxt"), $('#cintro').empty(), 100);
}

/*文字的打字效果*/
function changeText(cont1, cont2, speed) {
    clearInterval(Otimer);
    var Otext = cont1.text();
    var Ocontent = Otext.split("");
    var i = 0;

    function show() {
        if (i < Ocontent.length) {
            cont2.append(Ocontent[i]);
            i = i + 1;
        }
    }
    Otimer = setInterval(show, speed);
}
</script>
</body>
</html>