var appPlantDoc='';
var finalDoc='';
var cabinet ='';
var data;
Ext.onReady(function() {
	//数据源 
//    var data = { 
//        name: 'Tommy Maintz', 
//        title: 'Lead Developer', 
//        company: 'Sencha Inc.', 
//        email: 'tommy@sencha.com', 
//        address: '5 Cups Drive', 
//        city: 'Palo Alto', 
//        state: 'CA', 
//        zip: '44102', 
//        drinks: ['Coffee', 'Soda', 'Water'], 
//        kids: [{ 
//                name: 'Joshua', 
//                age:3 
//            },{ 
//                name: 'Matthew', 
//                age:2 
//            },{ 
//                name: 'Solomon', 
//                age:0 
//        }] 
//        }; 
//
//     
//    //呈现组件 
//    var mypanel = new Ext.Panel({ 
//        id: "mypanel", 
//        width: 300, 
//        frame: true, 
//        height: 100, 
//        title: "XTemplate简单示例", 
//        renderTo: Ext.getBody() 
//    }); 
//     
//    //创建模板 
//    var tpl = new Ext.XTemplate( 
//        '<p>Kids: </p>', 
//        '<tpl for="drinks">',       // process the data.kids node 
//            '<p>{.}</p>',  // use current array index to autonumber 
//        '</tpl>' 
//    ); 
//     
//    //重写绑定模板 
//    tpl.overwrite(mypanel.body, data.drinks); // pass the kids property of the data object 
//     
//
//	

	
	var viewport = new Ext.Viewport({
						layout : 'border',
						
						items : [{
									region : 'center',
									
									title:'服务器总览',
									autoScroll : true,
									bodyStyle :'background:#CCDFEA',
									html:'<div id="appPlant"></div>'
								}]

					});
//	window.setTimeout("serverUpdate();",1000);
	
	
	
	
	
	
	
	
	
	
	
	
})
//页面刷新
var task = {
    run: function(){
		serverUpdate();
    },
    interval: 20000 //20 second
}
var runner = new Ext.util.TaskRunner();
runner.start(task);

function serverUpdate(){
	
	
	var data = {
		    name: '张三',
		    job: 'C#程序员',
		    company: '惠普',
		    email: 'zhangsan@163.com',
		    address: '武汉市洪山区光谷软件园',
		    city: '武汉',
		    state: '正常',
		    zip: '430000',
		    drinks: ['绿茶', '红酒', '咖啡'],
		    friends: [{
		        name: '李四',
		        age: 6,
		        like: '鲜花'
		    }, {
		        name: '王五',
		        age: 26,
		        like: '足球'
		    }, {
		        name: '赵六',
		        age: 81,
		        like: '游戏'
		    }]
		};
	 
	
	var tpl = new Ext.XTemplate(
		      '<table cellpadding=0 cellspacing=0 border=1 width=400px>',
		      '<tr><td colspan=2 align=center><b>{name}的个人资料</b></td></tr>',
		      '<tr><td>姓名：</td><td>{name}</td></tr>',
		      '<tr><td>工作：</td><td>{job}</td></tr>',
		      '<tr><td>公司：</td><td>{company}</td></tr>',
		      '<tr><td>地址：</td><td>{address}</td></tr>',
		      '<tr><td>喜好饮品：</td><td>{drinks}</td></tr>',
		      '<tr><td>他的好友：</td><td>',
		      '<tpl for="friends">',
		      '<p>{name}：{age}岁</p>',
		      '</tpl></td></tr>',
		      '</table>'
		);
	finalDoc='';
	Ext.Ajax.request({
		url : 'crsm/crsmServerMonitor.ered?reqCode=querycabinetList',//查询机柜列表
//		async :  false,//同步请求数据
	    success: function(response1) {
			/******************遍历机柜********************/
			 var groupCabinet = eval('('+response1.responseText+')');
	    	 var groupCabinetTotal = groupCabinet.TOTALCOUNT;//机柜总数
	    	 var groupCabinetRoot = groupCabinet.ROOT;//机柜信息
			/*****************查询每个机柜上的服务器******************/
	    	 
	    	 
//	 		var viewport = new Ext.Viewport({
//				layout : 'border',
//				items : [{
//					xtype : 'portal',
//					region : 'center',
//					margins : '3 3 3 3',
//					items : [ {
//								columnWidth : .4,
//								style : 'padding:8px 8px 8px 8px',
//								items : [{
//									title : '文档下载 ',
//									html : '<div style=height:155px;line-height:25px class=commoncss>&nbsp;&nbsp;如果G4Studio与你无缘, 那我推荐如下一些优秀的开发平台类产品. '
//										+ ' <br>&nbsp;&nbsp;AppFuse：<a href="http://www.appfuse.org" target="_blank">www.appfuse.org</a>'
//										+ ' <br>&nbsp;&nbsp;Primeton EOS：<a href="http://www.primeton.com/products/bap" target="_blank">www.primeton.com</a>'
//										+ ' </div>'
//								}, {
//									title : '联系方式1',
//									// tools : tools,
//									html : '<div style=height:60px;line-height:25px class=commoncss>&nbsp;&nbsp;电子邮箱: 307916217@qq.com<br>&nbsp;&nbsp;项目主页：<a href="http://www.g4it.org" target="_blank">www.g4it.org</a></div>'
//								}]
//							},{
//								columnWidth : .4,
//								style : 'padding:8px 8px 8px 8px',
//								items : [{
//									title : '联系方式',
//									// tools : tools,
//									html : '<div style=height:60px;line-height:25px class=commoncss>&nbsp;&nbsp;电子邮箱: 307916217@qq.com<br>&nbsp;&nbsp;项目主页：<a href="http://www.g4it.org" target="_blank">www.g4it.org</a></div>'
//								}]
//							}
//							]
//				}]
//			});
	    	 
	    	
			 for(var i=0;i<groupCabinetTotal;i++){
				{(function(){
					 var s=i;
					 var cabinetDom='';//服务器列表生成
					 var cabinetDom2='';//服务器列表生成
					 var edit='';
					 var CabinetAddress=groupCabinetRoot[s].cabinet_address;//机房地址
					 var CabinetContainer=groupCabinetRoot[s].cabinet_ratepower;//额定功率
					 var CabinetId=groupCabinetRoot[s].cabinet_id;//机柜标识
					 var list = groupCabinetRoot[s].list;//服务器信息队列
					 var listSize = groupCabinetRoot[s].listsize;//服务器个数
					// var title='<div class="head"  >接入网关('+CabinetAddress+"-"+CabinetContainer+')</div>';//抬头/('+CabinetAddress+":600"+CabinetContainer+')
					 var title='<div class="head" style="color:blue;font-weight:800;font-size:15px;" ><a href="#_" onclick="go()">接入网关<div class="head" style="display:inline;font-size:1px;font-weight:10;">('+CabinetAddress+":600"+CabinetContainer+')</div></a></div>';//抬头
					 var flash = true;
					 
					
					 for(var j=0;j<listSize;j++){
						 var serverInfo = list[j];
						 var serverIP = serverInfo.server_ip;
						 var serverPort = serverInfo.server_port;
						 var cabinet_card = serverInfo.cabinet_card;
						 var cabinetid = serverInfo.cabinet_id;
						 var serverType = serverInfo.vtype;
						 var serverStatus = serverInfo.vstatus;
						 var status = statusImg(serverStatus);
						 var type = changeType(serverType);
							
						 var serverIP = "192.168.100.11";
						 var serverPort = "2000";
						// var cabinet_card = "2";
						 var cabinetid = "1";
						 var serverType = "1";
						 var serverStatus = "0";
						 var status = statusImg(cabinet_card);
						 var type = changeType("HD");
						 if(j==0){
							 
							 cabinetDom+='<div class ="each clearfix">键值网关:</div>'+'<div class ="each clearfix">'
				    	 	   +'<a href="#_" onclick=editServer("'+serverIP+'","'+serverStatus+'","'+cabinetid+'","'+serverPort+'")>'+cabinet_card+'   '+serverIP+'   '+type+'   '+'<img src="'+status+'"/></a>'
				    	 	   +'</div>';//重写机柜模板 
						 }else{
							 
							 cabinetDom+='<div class ="each clearfix">'
					    	 	   +'<a href="#_" onclick=editServer("'+serverIP+'","'+serverStatus+'","'+cabinetid+'","'+serverPort+'")>'+cabinet_card+'   '+serverIP+'   '+type+'   '+'<img src="'+status+'"/></a>'
					    	 	   +'</div>';//重写机柜模板
						 }
						 
		     										   
						 
					 }
					 
						
					 for(var jj=0;jj<listSize;jj++){
						 var serverInfo = list[jj];
						 var serverIP = serverInfo.server_ip;
						 var serverPort = serverInfo.server_port;
						 var cabinet_card = serverInfo.cabinet_card;
						 var cabinetid = serverInfo.cabinet_id;
						 var serverType = serverInfo.vtype;
						 var serverStatus = serverInfo.vstatus;
						 var status = statusImg(serverStatus);
						 var type = changeType(serverType);
							
						 var serverIP = "192.168.100.11";
						 var serverPort = "2000";
						// var cabinet_card = "2";
						 var cabinetid = "1";
						 var serverType = "1";
						 var serverStatus = "0";
						 var status = statusImg(cabinet_card);
						 var type = changeType("HD");
				
						 
		                 if(jj==0){
							 
							 cabinetDom2+='<div class ="each clearfix">信令网关:</div>'+'<div class ="each clearfix">'
				    	 	   +'<a href="#_" onclick=editServer("'+serverIP+'","'+serverStatus+'","'+cabinetid+'","'+serverPort+'")>'+cabinet_card+'   '+serverIP+'   '+type+'   '+'<img src="'+status+'"/></a>'
				    	 	   +'</div>';//重写机柜模板 
						 }else{
							 
							 cabinetDom2+='<div class ="each clearfix">'
					    	 	   +'<a href="#_" onclick=editServer("'+serverIP+'","'+serverStatus+'","'+cabinetid+'","'+serverPort+'")>'+cabinet_card+'   '+serverIP+'   '+type+'   '+'<img src="'+status+'"/></a>'
					    	 	   +'</div>';//重写机柜模板
						 } 											   
						 
					 }
//					 if(flash){//没有服务器上架，编辑机柜信息
//						 cabinetDom+='<div class ="each clearfix">'
//				    	 		   +'<a href="#_" onclick=editCabinet("'+CabinetAddress+'","'+CabinetContainer+'","'+listSize+'")>增加信息</a>'
//				    	 		   +'</div>';
//					 }

					 cabinet='<div class="box" style="margin:5px 10px 20px 30px;width:300px;height:200px">'+ title+cabinetDom+cabinetDom2+'</div>';
				//	 tpl.overwrite(Ext.get("div1"), data);
				})();
				}
				finalDoc += cabinet;	//机柜图形
			 }
			/****************************************/
			appPlantDoc ='<div class="appMonitor">'
				+finalDoc
				+'</div>';
			document.getElementById("appPlant").innerHTML =appPlantDoc;
	    }
	})
}
	function statusImg(value){
		if(value=='1'){
			return "crsm/server_error.png";
		}else if(value=='2'){
			return "crsm/server_normal.png";
		}else if(value=='3'){
			return "crsm/server_offline.png";
		}else{
		//	return "../server_none.png";
			return "crsm/server_normal.png";
		}
	}
	
	
	function go(){
          alert("可以对接入网关相关的操作");
	}
	function changeType(value){
		if(value=='SD'){
			return "信息1";
		}else if(value=='HD'){
			return "信息1"+"&nbsp;信息2"+"&nbsp;信息3"+"&nbsp;信息4";
		}else{
			return null;
		}
	}
	/**********************机柜维护开始*****************************/
	var addCabinetFormPanel = new Ext.form.FormPanel({
				id : 'addCabinetFormPanel',
				name : 'addCabinetFormPanel',
				defaultType : 'textfield',
				labelAlign : 'right',
				labelWidth : 90,
				frame : false,
				bodyStyle : 'padding:5 5 0',
				items : [{
									fieldLabel : '机柜地址',
									name : 'cabinet_address',
									id : 'cabinet_address',
									allowBlank : false,	
									readOnly:true,
									fieldClass : 'x-custom-field-disabled',
									anchor : '99%'
								},{
									fieldLabel : '机柜容量',
									name : 'card_num',
									id : 'card_num',
									allowBlank : false,
									anchor : '99%',
									xtype : 'numberfield', // 设置为数字输入框类型
									allowDecimals : false, // 是否允许输入小数
									allowNegative : false, // 是否允许输入负数
									minValue : 5,
									minValueText : '机柜最小容量为5个位置',
									maxValue : 14,
									maxValueText : '机柜最大容量为14个位置'
								},{
									fieldLabel : '额定功率',
									name : 'cabinet_ratePower',
									id : 'cabinet_ratePower',
									allowBlank : false,	
									anchor : '99%'
								}]
			});

	var addCabinetWindow = new Ext.Window({
				layout : 'fit',
				width : 280,
				height : 150,
				resizable : false,
				draggable : true,
				closeAction : 'hide',
				modal : true,
				title : '<span>增加信息</span>',
				collapsible : true,
				titleCollapse : true,
				maximizable : false,
				buttonAlign : 'right',
				border : false,
				animCollapse : true,
				pageY : 20,
				pageX : document.body.clientWidth / 2 - 420 / 2,
				animateTarget : Ext.getBody(),
				constrain : true,
				items : [addCabinetFormPanel],
				buttons : [{
					text : '删除',
					iconCls : 'deleteIcon',
					handler : function() {
						deleteCabinet();
					}
				},{
					text : '保存',
					iconCls : 'acceptIcon',
					handler : function() {
						updateCabinet();
					}
				},  {
					text : '关闭',
					iconCls : 'deleteIcon',
					handler : function() {
						addCabinetWindow.hide();
					}
				}]
			});

			function editCabinet(CabinetId,CabinetContainer,listsize){//增加信息
				addCabinetFormPanel.getForm().reset();
				addCabinetWindow.show();
				Ext.getCmp('cabinet_address').setValue(CabinetId);
				Ext.getCmp('cabinet_ratePower').setValue(CabinetContainer);
				Ext.getCmp('card_num').setValue(listsize);
			}
			function updateCabinet(){//修改机柜信息
				if (!addCabinetFormPanel.form.isValid()) {
					return;
				}
				addCabinetFormPanel.form.submit({
					url : 'crsm/crsmServerMonitor.ered?reqCode=updateCabinetInfo',
					waitTitle : '提示',
					method : 'POST',
					waitMsg : '正在处理数据,请稍候...',
					success : function(form, action) {
						Ext.Msg.show({'title':'提示',msg:'机柜信息更新成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO});
						addCabinetWindow.hide();
						serverUpdate();
					},
					failure : function(form, action) {
					}
				});
			}
			function deleteCabinet(){//删除机柜
				addCabinetFormPanel.form.submit({
					url : './crsmServerMonitor.ered?reqCode=deleteCabinetItems',
					waitTitle : '提示',
					method : 'POST',
					waitMsg : '正在处理数据,请稍候...',
					success : function(response) {
						serverUpdate();
						addCabinetWindow.hide();
						Ext.Msg.show({'title':'提示',msg:'机柜删除成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO});
					},
					failure : function(form, action) {
					}
				});
			}

	/**********************机柜维护结束*****************************/


	function newServer(k,CabinetId){
		/**********************服务器信息编辑开始********************/
		var serverForm = new Ext.form.FormPanel({
						labelAlign : 'right', // 标签对齐方式
						bodyStyle : 'padding:5 5 0', // 表单元素和表单面板的边距
						buttonAlign : 'center',
						autoScroll : true,
						defaultType : 'textfield',
						id : 'serverForm',
						name : 'serverForm',
						items : [{
							fieldLabel : '服务器ID', // 标签
							name : 'server_id', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							id:'server_id',
							anchor : '90%'// 宽度百分比
						},{
							fieldLabel : '服务器版本',
							name : 'region_id',
							allowBlank : false,
							anchor : '90%'
						},{
							fieldLabel : '备用信息', // 标签
							name : 'reserve', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							anchor : '90%' // 宽度百分比
						},{
							fieldLabel : '验证码', // 标签
							name : 'auth_key', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							anchor : '90%'// 宽度百分比
						},{
							fieldLabel : '流水号', // 标签
							name : 'auth_seq', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							anchor : '90%' // 宽度百分比
						},{
							fieldLabel : '验证TOKEN', // 标签
							name : 'auth_token', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							anchor : '90%' // 宽度百分比
						},{
							fieldLabel : 'RETURN_URL', // 标签
							name : 'return_url', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							anchor : '90%' // 宽度百分比
						},{
							fieldLabel : '机柜ID', // 标签
							name : 'cabinet_id', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							emptyText:CabinetId,
							anchor : '90%' // 宽度百分比
						},{
							fieldLabel : '机柜编号', // 标签
							name : 'cabinet_code', // name:后台根据此name属性取值
							allowBlank : true, // 是否允许为空
							emptyText:k,
							anchor : '90%' // 宽度百分比
						}]
						});
		var serverWindow = new Ext.Window({
						layout : 'fit',
						width : 450,
						height : 400,
						resizable : false,
						draggable : true,
						closeAction : 'hide',
						title : '<span class="commoncss">编辑服务器信息</span>',
						// iconCls : 'page_addIcon',
						modal : true,
						collapsible : true,
						titleCollapse : true,
						maximizable : false,
						buttonAlign : 'right',
						border : false,
						animCollapse : true,
						animateTarget : Ext.getBody(),
						constrain : true,
						items : [serverForm], // 嵌入的表单面板
						buttons : [{
								text : '保存',
								iconCls : 'acceptIcon',
								handler : function() {
//									singleFormSave();
									serverWindow.close();
								}
							},{ // 窗口底部按钮配置
								text : '重置', // 按钮文本
								iconCls : 'tbar_synchronizeIcon', // 按钮图标
								handler : function() { // 按钮响应函数
									serverForm.form.reset();
								}
							}]
					});
		/**********************服务器信息编辑结束********************/
		/**********************服务器列表开始************************/
		var serverListStore = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						 url :'./appGroupManager.ered?reqCode=queryCabinetList'
					}),
			reader : new Ext.data.JsonReader({
						totalProperty : 'TOTALCOUNT',
						root : 'ROOT'
					}, [{
								name : 'cabinet_local'
							}, {
								name : 'cabinet_id'
							}]),
			baseParams : {
				areacode : ''
			}
		});
	var serverListCombo = new Ext.form.ComboBox({
				hiddenName : 'cabinet_local',
				fieldLabel : '服务名称',
				emptyText : '请选择...',
				triggerAction : 'all',
				store : serverListStore,
				displayField : 'cabinet_local',
				valueField : 'cabinet_local',
				loadingText : '正在加载数据...',
				mode : 'local', // 数据会自动读取,如果设置为local又调用了store.load()则会读取2次；也可以将其设置为local，然后通过store.load()方法来读取
				forceSelection : true,
				allowBlank : false,
				typeAhead : true,
				resizable : true,
				editable : true,
				anchor : '100%',
				'beforequery':function(e){                     
		    	 var combo = e.combo;  
		         combo.expand(); 
		         if(!e.forceAll){    
		            var input = e.query;    
		            // 检索的正则   
		            var regExp = new RegExp(".*" + input + ".*");  
		            // 执行检索   
		            combo.store.filterBy(function(record,id){    
		            // 得到每个record的项目名称值   
		            var text = record.get(combo.displayField);    
		            return regExp.test(text);   
		           });        
		          return false;  
		        }  
		    }
			});
	serverListStore.load();
	var newForm = new Ext.form.FormPanel({
						labelAlign : 'right', // 标签对齐方式
						bodyStyle : 'padding:5 5 0', // 表单元素和表单面板的边距
						buttonAlign : 'center',
						autoScroll : true,
						defaultType : 'textfield',
						id : 'newForm',
						name : 'newForm',
						items : [serverListCombo]
						});
	var newWindow = new Ext.Window({
						layout : 'fit',
						width : 300,
						height : 150,
						resizable : false,
						draggable : true,
						closeAction : 'hide',
						title : '<span class="commoncss">上架服务器</span>',
						// iconCls : 'page_addIcon',
						modal : true,
						collapsible : true,
						titleCollapse : true,
						maximizable : false,
						buttonAlign : 'right',
						border : false,
						animCollapse : true,
						animateTarget : Ext.getBody(),
						constrain : true,
						items : [newForm], // 嵌入的表单面板
						buttons : [{
								text : '下一步',
								iconCls : 'acceptIcon',
								handler : function() {
//									singleFormSave();
									
									var params = newForm.getForm().getValues();
									var s = eval(params);
									var  cabinet_id= s["cabinet_local"];
									newWindow.close();
									//TODO 同时带参数机柜ID，服务器IP到服务编辑页面
									serverForm.form.reset();
									serverWindow.show();
									document.getElementById("server_id").value=cabinet_id;
									
								}
							}]
					});
	/*******************服务器列表结束***************************/
		newForm.getForm().reset();
		newWindow.show();
//		document.getElementById('editCabinet').style.display="none";
		
	}
	/*******************S 编辑服务器开始 S********************/
	/*******************S 非正常状态 S*************************************/
	var serverForm = new Ext.form.FormPanel({
						labelAlign : 'right', // 标签对齐方式
						bodyStyle : 'padding:5 5 0', // 表单元素和表单面板的边距
						buttonAlign : 'center',
						autoScroll : true,
						defaultType : 'textfield',
						id : 'serverForm',
						name : 'serverForm',
						items : [{
							fieldLabel : '机柜标识', // 标签
							name : 'cabinet_id', // name:后台根据此name属性取值
							id:'cabinet',
							hideLabel: 'true',
							hidden:true,
							anchor : '90%'// 宽度百分比
						},{
							fieldLabel : '服务器ID', // 标签
							name : 'server_id', // name:后台根据此name属性取值
							id:'server_ip',
							readOnly:true,
							fieldClass : 'x-custom-field-disabled',
							anchor : '90%'// 宽度百分比
						},{
							fieldLabel : '服务器端口', // 标签
							name : 'server_port', // name:后台根据此name属性取值
							id:'serverport',
							readOnly:true,
							fieldClass : 'x-custom-field-disabled',
							anchor : '90%'// 宽度百分比
						},{
							fieldLabel : '服务器状态',
							name : 'vstatus',
							id:'vstatus',
							readOnly:true,
							fieldClass : 'x-custom-field-disabled',
							anchor : '90%'
						}]
						});
	var serverWindow = new Ext.Window({
						layout : 'fit',
						width : 300,
						height : 200,
						resizable : false,
						draggable : true,
						closeAction : 'hide',
						title : '<span class="commoncss">修改网关信息</span>',
						// iconCls : 'page_addIcon',
						modal : true,
						collapsible : true,
						titleCollapse : true,
						maximizable : false,
						buttonAlign : 'right',
						border : false,
						animCollapse : true,
						animateTarget : Ext.getBody(),
						constrain : true,
						items : [serverForm], // 嵌入的表单面板
						buttons : [{
								text : '认证',
								id:'auth',
								hidden:true,
								iconCls : 'keyIcon',
								handler : function() {
									var params = serverForm.getForm().getValues();
									var s = eval(params);
									var  serverip= s["server_id"];
									var  serverport= s["server_port"];
									var serveripport = serverip+":"+serverport;
									Ext.Ajax.request({
									    url: './crsmServerMonitor.ered?reqCode=serverAuthOperate&server_ip='+serveripport,
									    success: function(response) {
											var t= eval('('+response.responseText+')');
												if(t.success){
													Ext.Msg.show({title:'提示',msg:'服务器认证事件已触发成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO});
												}else{
													Ext.Msg.show({title:'提示',msg:'服务器认证事件触发失败！',buttons:Ext.Msg.OK,icon:Ext.Msg.ERROR});
												}
									    },failure : function(response) {
										}
									})
									serverWindow.hide();
									
								}
							},{
								text : '上线',
								id:'online',
								hidden:true,
								iconCls : 'acceptIcon',
								handler : function() {
									var params = serverForm.getForm().getValues();
									var s = eval(params);
									var  serverip= s["server_id"];
									var  serverport= s["server_port"];
									var serveripport = serverip+":"+serverport;
									Ext.Ajax.request({
									    url: './crsmServerMonitor.ered?reqCode=serverOnlineOperate&server_ip='+serveripport,
									    success: function(response) {
											var t= eval('('+response.responseText+')');
												if(t.success){
													Ext.Msg.show({title:'提示',msg:'服务器上线事件已触发成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO});
												}else{
													Ext.Msg.show({title:'提示',msg:'服务器上线事件触发失败！',buttons:Ext.Msg.OK,icon:Ext.Msg.ERROR});
												}
									    },failure : function(response) {
									    	
										}
									})
									serverWindow.hide();
									
								}
							},{
								text : '解绑机柜',
								iconCls : 'acceptIcon',
								handler : function() {
									var params = serverForm.getForm().getValues();
									var s = eval(params);
									var  serverip= s["server_id"];
									var  cabinetID= s["cabinet_id"];
									Ext.Ajax.request({
									    url: './crsmServerMonitor.ered?reqCode=serverDeleteCabinet&server_ip='+serverip+'&cabinet_id='+ cabinetID,
									    success: function(response) {
											var t= eval('('+response.responseText+')');
												if(t.success){
													Ext.Msg.show({title:'提示',msg:'服务器解绑机柜成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO,fn: function(btnId,text,opt){
					                                    if(btnId == 'ok'){
					                                       serverUpdate();
					                                    }}
													});
												}else{
													Ext.Msg.show({title:'提示',msg:'服务器解绑机柜失败！',buttons:Ext.Msg.OK,icon:Ext.Msg.ERROR});
												}
									    },failure : function(response) {
									    	
										}
									})
									serverWindow.hide();
									
								}
							}]
					});
	/****************E 非正常结束 E********************/
	/****************S 正常开始 S**********************/
	function openWindow(tstatus, ip,port,CabinetID) {
	var jsonData ;
	var message = "IP为";
	var serverID = ip +":" + port;
	Ext.Ajax.request({
     url: './crsmServerMonitor.ered?reqCode=queryServerSingleByID&server_ip='+ip,
     success: function(response){
     	//下架，强制下架，解绑,true为禁止
     	flags = [false,false,false];
     	
		jsonData = eval('('+response.responseText+')');

		message += ip+"<br><br> 服务器位置:"+jsonData.cabinet_address+"第"+jsonData.cabinet_card+"位置";
     	
     	message +="<br><br>"+"CPU使用状况:"+jsonData.cpu_info;
     	
     	message += "<br><br>"+"内存使用状况:"+jsonData.mem_info;
     	
     	message += "<br><br>"+"支持总路数:"+jsonData.max_num;
     	
     	message += "<br><br>"+"在线路数:"+jsonData.online_num;
     	
     	message += "<br><br>"+"流花路数:"+jsonData.liuhua_num;
     	
     	message += "<br><br>"+"系统版本:"+jsonData.sys_version;
     	
     	message += "<br><br>"+"vlc版本:"+jsonData.vlc_version;
     	
     	message += "<br><br>"+"chrome版本:"+jsonData.chrome_version;
     	
     	message += "<br><br>"+"chrome插件版本:"+jsonData.chromeplush_version;
     	
     	message += "<br><br>"+"pulseaudio版本:"+jsonData.pulseaudio_version;
     	
     	message += "<br><br>"+"xencoder版本:"+jsonData.xencoder_version;
     	
     	message += "<br><br>"+"dm816x版本:"+jsonData.dm816x_version;
     	
		var firstWindow = new Ext.Window({
			title : '<span class="commoncss">服务器信息</span>', // 窗口标题
			html: '<div style="padding:10px; font-size:12px;">'+message+'</div>',
			iconCls : 'imageIcon',
			autoScroll : true,
			layout : 'fit', // 设置窗口布局模式
			width : 350, // 窗口宽度
			height : 400, // 窗口高度
			// tbar : tb, // 工具栏
			closable : true, // 是否可关闭
			closeAction : 'hide', // 关闭策略
			bodyStyle : 'background-color:#FFFFFF',
			collapsible : true, // 是否可收缩
			maximizable : true, // 设置是否可以最大化
			animateTarget : Ext.getBody(),
			border : true, // 边框线设置
			pageY : 100, // 页面定位Y坐标
			pageX : document.body.clientWidth / 2 - 600 / 2, // 页面定位X坐标
			constrain : true,
			// 设置窗口是否可以溢出父容器
			buttonAlign : 'center',
			buttons : [ {
				text : '预下线',
				iconCls : 'acceptIcon',
				handler : function() {
					Ext.Ajax.request({
				    url: './crsmServerMonitor.ered?reqCode=serverPerOfflineOperate&server_ip='+serverID,
				    success: function(response) {
						var t= eval('('+response.responseText+')');
							if(t.success){
								firstWindow.hide();
								Ext.Msg.show({title:'提示',msg:'服务器预下线事件已触发成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO,fn: function(btnId,text,opt){
                                    if(btnId == 'ok'){
                                       serverUpdate();
                                    }}});
							}else{
								Ext.Msg.show({title:'提示',msg:'服务器预下线事件触发失败！',buttons:Ext.Msg.OK,icon:Ext.Msg.ERROR});
								firstWindow.hide();
							}
				    },failure : function(response) {
				    	
					}
				})
				}
					
				}, {
					text : '强制下线',
					iconCls : 'acceptIcon',
					handler : function() {
						Ext.Ajax.request({
				    url: './crsmServerMonitor.ered?reqCode=serverOfflineOperate&server_ip='+serverID,
				    success: function(response) {
						var t= eval('('+response.responseText+')');
							if(t.success){
								firstWindow.hide();
								Ext.Msg.show({title:'提示',msg:'服务器强制下线事件已触发成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO,fn: function(btnId,text,opt){
                                    if(btnId == 'ok'){
                                       serverUpdate();
                                    }}});
							}else{
								Ext.Msg.show({title:'提示',msg:'服务器强制下线事件触发失败！',buttons:Ext.Msg.OK,icon:Ext.Msg.ERROR});
								firstWindow.hide();
							}
				    },failure : function(response) {
				    	
					}
				})
					}
				},{
					text : '解绑机柜',
					iconCls : 'acceptIcon',
					handler : function() {
						Ext.Ajax.request({
									    url: './crsmServerMonitor.ered?reqCode=serverDeleteCabinet&server_ip='+ip+'&cabinet_id='+ CabinetID,
									    success: function(response) {
											var t= eval('('+response.responseText+')');
												if(t.success){
													firstWindow.hide();
													Ext.Msg.show({title:'提示',msg:'服务器解绑机柜成功！',buttons:Ext.Msg.OK,icon:Ext.Msg.INFO,fn: function(btnId,text,opt){
					                                    if(btnId == 'ok'){
					                                       serverUpdate();
					                                    }}
													});
												}else{
													Ext.Msg.show({title:'提示',msg:'服务器解绑机柜失败！',buttons:Ext.Msg.OK,icon:Ext.Msg.ERROR});
													firstWindow.hide();
												}
									    },failure : function(response) {
									    	
										}
									})
					}
				},
					{
					text : '关闭',
					iconCls : 'deleteIcon',
					handler : function() {
						firstWindow.hide();
					}
				} ]
			});
			firstWindow.show(); // 显示窗口
		}
	});
	};
	/***************E 正常结束 E*****************/
	function editServer(serverIP,status,cabinet_id,serverPort){
		if(serverIP=='null')
			return;
		if(status=='normal'){//在运行
			openWindow(status,serverIP,serverPort,cabinet_id);
		}else{//其他
			serverForm.getForm().reset();
			serverWindow.show();
			document.getElementById("server_ip").value=serverIP;
			document.getElementById("vstatus").value=status;
			document.getElementById("cabinet").value=cabinet_id;
			document.getElementById("serverport").value=serverPort;
			if(status=='register'){
				Ext.getCmp('auth').show();
				Ext.getCmp('online').hide();
			}else{
				Ext.getCmp('online').show();
				Ext.getCmp('auth').hide();
			}
		}
		
		/*******************E 编辑服务器结束 E********************/
	}