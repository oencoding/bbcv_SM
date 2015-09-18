var appPlantDoc='';
var finalDoc='';
var cabinet ='';
Ext.onReady(function() {
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
	window.setTimeout("serverUpdate();",1000);
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
			 for(var i=0;i<groupCabinetTotal;i++){
				{(function(){
					 var s=i;
					 var cabinetDom='';//服务器列表生成
					 var edit='';
					 var CabinetAddress=groupCabinetRoot[s].cabinet_address;//机房地址
					 var CabinetContainer=groupCabinetRoot[s].cabinet_ratepower;//额定功率
					 var CabinetId=groupCabinetRoot[s].cabinet_id;//机柜标识
					 var list = groupCabinetRoot[s].list;//服务器信息队列
					 var listSize = groupCabinetRoot[s].listsize;//服务器个数
					 var title='<div class="head">'+CabinetAddress+"-"+CabinetContainer+'</div>';//抬头
					 var flash = true;
					 
						 new Ext.Viewport({
							layout : 'border',
							items : [{
								xtype : 'portal',
								region : 'center',
								margins : '3 3 3 3',
								items : [ {
											columnWidth : .3,
											style : 'padding:8px 8px 8px 8px',
											items : [{
												title : '文档下载 ',
												html : '<div style=height:155px;line-height:25px class=commoncss>&nbsp;&nbsp;如果G4Studio与你无缘, 那我推荐如下一些优秀的开发平台类产品. '
													+ ' <br>&nbsp;&nbsp;AppFuse：<a href="http://www.appfuse.org" target="_blank">www.appfuse.org</a>'
													+ ' <br>&nbsp;&nbsp;Primeton EOS：<a href="http://www.primeton.com/products/bap" target="_blank">www.primeton.com</a>'
													+ ' </div>'
											}, {
												title : '联系方式',
												// tools : tools,
												html : '<div style=height:60px;line-height:25px class=commoncss>&nbsp;&nbsp;电子邮箱: 307916217@qq.com<br>&nbsp;&nbsp;项目主页：<a href="http://www.g4it.org" target="_blank">www.g4it.org</a></div>'
											}, {
												title : '舍我其谁',
												// tools : tools,
												html : '<div style=height:155px;line-height:25px class=commoncss>&nbsp;&nbsp;如果G4Studio与你无缘, 那我推荐如下一些优秀的开发平台类产品. '
														+ ' <br>&nbsp;&nbsp;AppFuse：<a href="http://www.appfuse.org" target="_blank">www.appfuse.org</a>'
														+ ' <br>&nbsp;&nbsp;Primeton EOS：<a href="http://www.primeton.com/products/bap" target="_blank">www.primeton.com</a>'
														+ ' </div>'
											}]
										}]
							}]
						});
					 
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
						 if(serverIP!=null){
							 flash = false;
						 }			
						 cabinetDom+='<div class ="each clearfix">'
				    	 	   +'<a href="#_" onclick=editServer("'+serverIP+'","'+serverStatus+'","'+cabinetid+'","'+serverPort+'")>'+cabinet_card+'   '+serverIP+'   '+type+'   '+'<img src="'+status+'"/></a>'
				    	 	   +'</div>';//重写机柜模板
					 }
					 if(flash){//没有服务器上架，编辑机柜信息
						 cabinetDom+='<div class ="each clearfix">'
				    	 		   +'<a href="#_" onclick=editCabinet("'+CabinetAddress+'","'+CabinetContainer+'","'+listSize+'")>编辑机柜</a>'
				    	 		   +'</div>';
					 }
//					 Ext.Ajax.request({
//						 url : './crsmServerMonitor.ered?reqCode=queryServerList&cabinet_address='+CabinetAddress,//查询服务器列表
//						 success:function(response){
//						 	var groupServer = eval('('+response.responseText+')');
//		    	 			var groupServerTotal = groupServer.TOTALCOUNT;//服务器总数
//		    	 			var groupServerRoot = groupServer.ROOT;//服务器信息	
//		    	 			if(groupServerTotal>0){
//		    	 				for(var j=0;j<groupServerTotal;j++){
//			    	 				var serverAddress = groupServerRoot[j].cabinet_card;//服务器位置
//				    	 			var serverIP = groupServerRoot[j].vncm_ip;//服务器IP
//				    	 			var serverType = groupServerRoot[j].vncm_type;//服务器类型
//				    	 			var serverStatus = groupServerRoot[j].status;//服务器状态
//			//	    	 			var status = statusImg(serverStatus);//生成状态图片
//				    	 			cabinetDom+='<div class ="each clearfix">'
//				    	 			+'<a href="#_" onclick=editServer("'+serverIP+'")>'+(serverAddress+1)+'   '+serverType+'   '+serverIP+'   '+serverStatus+'</a>'
//				    	 			+'</div>';//重写机柜模板
//			    	 			}
//		    	 			}else{
//		    	 				edit='<div class ="each clearfix">'
//				    	 		+'<a href="#_" onclick=editCabinet("'+CabinetAddress+'")>编辑机柜</a>'
//				    	 		+'</div>';
//		    	 			}
//						 }
//					 })
					 cabinet='<div class="box" >'+ title+cabinetDom+'</div>';
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
		if(value=='register'){
			return "../crsm/server_error.png";
		}else if(value=='normal'){
			return "../crsm/server_normal.png";
		}else if(value=='unused'){
			return "../crsm/server_offline.png";
		}else{
			return "../crsm/server_none.png";
		}
	}
	function changeType(value){
		if(value=='SD'){
			return "标";
		}else if(value=='HD'){
			return "高";
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
				title : '<span>编辑机柜</span>',
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

			function editCabinet(CabinetId,CabinetContainer,listsize){//编辑机柜
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
						title : '<span class="commoncss">维护服务器物理信息</span>',
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