package prod.nebula.vgw4vlc.handler;

import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vgw4vlc.module.resctrl.Controller;
import prod.nebula.vgw4vlc.service.TCPServer;

public class ControllerThread{
	Logger logger = LoggerFactory.getLogger(ControllerThread.class);
	private final Timer timer = new Timer(); 
	private final int seconds;
	private final int timeLong = TCPServer.getConfig().getThreadTime();
	
	public ControllerThread(int seconds) { 
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
		Map controllerMap = TCPServer.getControllerList();
		boolean flag = true;
		while(flag){
			if(!controllerMap.isEmpty()){
				Set controllerSet = controllerMap.keySet();
				Iterator controllerIterator = controllerSet.iterator();
				while (controllerIterator.hasNext()) {
					Controller ctrl = (Controller) controllerMap.get(controllerIterator.next());
					logger.info("session:============="+ctrl.getSessionId());
					long currentTime = System.currentTimeMillis()/1000;
					long createTime = ctrl.getRuntime();
					if(currentTime-createTime>timeLong){
						ctrl.getClient().doOption1();
					}
				}
			}else{
				logger.info("当前没有用户点播。。。。");
			}
			try {
				Thread.sleep(seconds * 1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
				flag=false;
			}
		}
		
	}

}
