package prod.nebula.vgw4vlc.handler;

import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;

import net.sf.json.JSONObject;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vgw4vlc.core.common.ResConstants;
import prod.nebula.vgw4vlc.service.TCPServer;
import prod.nebula.vgw4vlc.util.StringUtil;
import prod.nebula.vgw4vlc.util.client.IOSocket;
import prod.nebula.vgw4vlc.util.client.IOSocketClient;
import prod.nebula.vgw4vlc.util.client.TcpClient;

public class reportType{
	Logger logger = LoggerFactory.getLogger(reportType.class);
	private final Timer timer = new Timer(); 
	private final int seconds;
	
	public reportType(int seconds) { 
		this.seconds = seconds; 
	}
	
	/**
	 * 定时任务启动
	 * 每隔多少秒执行一次
	 */
	public void start() { 
		timer.schedule(new TimerTask(){public void run(){task();}},0,seconds * 1000 ); 
	} 
	
	public void task() {
		if(!TCPServer.getConfig().isReportType()){
			JSONObject json = new JSONObject();
			json.put("cmd", "reportType");
			json.put("type", ResConstants.TYPE);
			json.put("serialno",UUID.randomUUID().toString().replace("-", ""));
			String sendStr = json.toString()+"XXEE";
			TcpClient client = new TcpClient();
			String retStr = client.sendStr(TCPServer.getConfig().getSMserverIP(), TCPServer.getConfig().getSMserverPORT(), 4000, null, sendStr);
			if(retStr!=null&&!"".equals(retStr)){
				retStr = retStr.replace("XXEE", "");
				JSONObject retJson = JSONObject.fromObject(retStr);
				if(retJson.getString("ret_code")=="0"||"0".equals(retJson.getString("ret_code"))){
					TCPServer.getConfig().setReportType(true);
				}
			}
		}
	}

}
