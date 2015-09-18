package prod.nebula.vgw4sida.handler;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.concurrent.atomic.AtomicBoolean;

import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vgw4sida.config.VODConst;
import prod.nebula.vgw4sida.exception.IEvent;
import prod.nebula.vgw4sida.module.resctrl.Controller;
import prod.nebula.vgw4sida.module.resctrl.VODResCtrl;
import prod.nebula.vgw4sida.service.TCPServer;
import prod.nebula.vgw4sida.util.Commons;
import prod.nebula.vgw4sida.util.StringUtil;
import prod.nebula.vgw4sida.util.client.IOSocketClient;

public class RTSPClient extends Thread implements IEvent {

	public final Logger logger = LoggerFactory.getLogger(RTSPClient.class);

	private static final String VERSION = " RTSP/1.0\r\n";
	private static final String RTSP_OK = "200 OK";// RTSP/1.0
	private static final String RTSP_MOVED = "RTSP/1.0 302 Moved Temporarily";
	private static final String RTSP_CLOSE = "SET_PARAMETER";
	private static final String CLOSE_REASON_END = "x-Reason: \"END\"";
	private static final String REQUIRE = "com.comcast.ngod.s1";
	
	public boolean setupFlag = false;

	/** 远程地址 */
	private final InetSocketAddress remoteAddress;

	/** * 本地地址 */
	private final InetSocketAddress localAddress;
	
	private final String ip;
	private final int port;

	/** * 连接通道 */
	public SocketChannel socketChannel;

	/** 发送缓冲区 */
	private final ByteBuffer sendBuf;

	/** 接收缓冲区 */
	private final ByteBuffer receiveBuf;

	private static final int BUFFER_SIZE = 8192;

	/** 端口选择器 */
//	private Selector selector;

	public String address;

	public Status sysStatus;

	public String sessionid;

	/** 线程是否结束的标志 */
	public AtomicBoolean shutdown;

	public int seq = 1;

	public boolean isSended;

	public String trackInfo;

	public String frequencyInfo;

	public String pidInfo;

	public String messageInfo = "";

	private Controller ctrl;

	private String totalTimeStr = "";
	
	private String beginTimeStr = "";
	private String endTimeStr ="";

	private enum Status {
		init, options, describe, setup, play, pause, scale, teardown, move
	}

	/**
	 * 终端标识
	 */
	public String stbid;

	/**
	 * 区域标识
	 */
	public String regionId;

	/**
	 * 流水号
	 */
	public String serialNo;

	public String cmd;

	public String doneDate;

	public String spId;

	/**
	 * 会话信息
	 */
	public IoSession session;

	public String listenIp;
	public int listenPort;
	
	public String purchaseToken;
	
	public String qam_name;
	public String client;
	public String destination;
	public String client_port;

	public String getSpId() {
		return spId;
	}

	public void setSpId(String spId) {
		this.spId = spId;
	}

	public String getStbid() {
		return stbid;
	}

	public void setStbid(String stbid) {
		this.stbid = stbid;
	}

	public String getRegionId() {
		return regionId;
	}

	public void setRegionId(String regionId) {
		this.regionId = regionId;
	}

	/**
	 * @return the listenIp
	 */
	public String getListenIp() {
		return listenIp;
	}

	/**
	 * @param listenIp
	 *            the listenIp to set
	 */
	public void setListenIp(String listenIp) {
		this.listenIp = listenIp;
	}

	/**
	 * @return the listenPort
	 */
	public int getListenPort() {
		return listenPort;
	}

	/**
	 * @param listenPort
	 *            the listenPort to set
	 */
	public void setListenPort(int listenPort) {
		this.listenPort = listenPort;
	}

	/**
	 * @return the serialNo
	 */
	public String getSerialNo() {
		return serialNo;
	}

	/**
	 * @param serialNo
	 *            the serialNo to set
	 */
	public void setSerialNo(String serialNo) {
		this.serialNo = serialNo;
	}



	/**
	 * @return the cmd
	 */
	public String getCmd() {
		return cmd;
	}

	/**
	 * @param cmd
	 *            the cmd to set
	 */
	public void setCmd(String cmd) {
		this.cmd = cmd;
	}

	/**
	 * @return the doneDate
	 */
	public String getDoneDate() {
		return doneDate;
	}

	/**
	 * @param doneDate
	 *            the doneDate to set
	 */
	public void setDoneDate(String doneDate) {
		this.doneDate = doneDate;
	}

	public String getSessionid() {
		return sessionid;
	}

	public void setSessionid(String sessionid) {
		this.sessionid = sessionid;
	}
	
	
	

	public String getPurchaseToken() {
		return purchaseToken;
	}

	public void setPurchaseToken(String purchaseToken) {
		this.purchaseToken = purchaseToken;
	}
	
	

	public String getQam_name() {
		return qam_name;
	}

	public void setQam_name(String qam_name) {
		this.qam_name = qam_name;
	}

	public String getClient() {
		return client;
	}

	public void setClient(String client) {
		this.client = client;
	}

	public String getDestination() {
		return destination;
	}

	public void setDestination(String destination) {
		this.destination = destination;
	}

	public String getClient_port() {
		return client_port;
	}

	public void setClient_port(String client_port) {
		this.client_port = client_port;
	}

	public RTSPClient(InetSocketAddress remoteAddress,
			InetSocketAddress localAddress, String address, Controller sender,
			IoSession session) {
		this.remoteAddress = remoteAddress;
		this.localAddress = localAddress;
		ip = remoteAddress.getHostName();
		port = remoteAddress.getPort();
		this.address = address;
		this.session = session;

		// 初始化缓冲区
		sendBuf = ByteBuffer.allocateDirect(BUFFER_SIZE);
		receiveBuf = ByteBuffer.allocateDirect(BUFFER_SIZE);
//		if (selector == null) {
//			// 创建新的Selector
//			try {
//				selector = Selector.open();
//			} catch (final IOException e) {
//				e.printStackTrace();
//			}
//		}
//		startup();
		sysStatus = Status.init;
		shutdown = new AtomicBoolean(false);
		isSended = false;
		ctrl = sender;
	}

	public void startup() {
		try {
			logger.debug("[RTSPClient] 开始CDN长连接  serialNo = " + serialNo);
			// 打开通道
			socketChannel = SocketChannel.open();
			// 绑定到本地端口
			socketChannel.socket().setSoTimeout(5000);
			logger.debug("[RTSPClient] configureBlocking start serialNo = " + serialNo);
			socketChannel.configureBlocking(false);
			logger.debug("[RTSPClient] configureBlocking stop serialNo = " + serialNo);
//			logger.debug("[RTSPClient] localAddress "
//					+ localAddress.getHostName() + ":" + localAddress.getPort());
			socketChannel.socket().bind(localAddress);
			if (socketChannel.connect(remoteAddress)) {
				logger.debug("【VOD网关】开始建立连接:" + remoteAddress);
//				socketChannel.register(selector, SelectionKey.OP_CONNECT
//						| SelectionKey.OP_READ | SelectionKey.OP_WRITE, this);
				logger.debug("【VOD网关】setup端口打开成功");
			}else{
				logger.debug("【VOD网关】setup建立长连接失败。。。");
				TCPServer.getControllerList().remove(ctrl.getSessionId());
			}
			
		} catch (final IOException e1) {
			logger.error("【VOD网关】setup建立长连接失败。。。", e1);
			TCPServer.getControllerList().remove(ctrl.getSessionId());
		}
	}

	public void send(byte[] out) {
		if (out == null || out.length < 1) {
			return;
		}
		synchronized (sendBuf) {
			sendBuf.clear();
			sendBuf.put(out);
			sendBuf.flip();
		}

		// 发送出去
		try {
			write();
			isSended = true;
		} catch (final IOException e) {
			logger.error("发送失败", e);
		}
	}

	public void write() throws IOException {
		if (isConnected()) {
			try {
				socketChannel.write(sendBuf);
			} catch (final IOException e) {
				logger.error("执行写数据失败", e);
			}
		} else {
			logger.debug("通道为空或者没有连接上");
		}
	}

	public byte[] recieve() {
		if (isConnected()) {
			try {
				int len = 0;
				int readBytes = 0;

				synchronized (receiveBuf) {
					receiveBuf.clear();
					try {
						while ((len = socketChannel.read(receiveBuf)) > 0) {
							readBytes += len;
						}
					} finally {
						receiveBuf.flip();
					}
					if (readBytes > 0) {
						final byte[] tmp = new byte[readBytes];
						receiveBuf.get(tmp);
						return tmp;
					} else {
						logger.debug("接收到数据为空,重新启动连接");
						return null;
					}
				}
			} catch (final IOException e) {
				logger.error("接收消息错误", e);
			}
		} else {
			logger.debug("端口没有连接");
		}
		return null;
	}

	public boolean isConnected() {
		return socketChannel != null && socketChannel.isConnected();
	}


	public void shutdown() {
		if (isConnected()) {
			try {
				socketChannel.close();
				logger.debug("端口关闭成功");
			} catch (final IOException e) {
				logger.error("端口关闭错误:", e);
			} finally {
				socketChannel = null;
			}
		} else {
			logger.debug(" 通道为空或者没有连接");
		}
	}

	//发送报文到CDN
	@Override
	public void run() {
		// 启动主循环流程
		while (!shutdown.get()) {
			try {
//				if (isConnected() && (!isSended)) {
				if ((!isSended)) {
					switch (sysStatus) {
					case init:
						doSetup();
						break;
					case setup:
						if (sessionid == null && sessionid.length() > 0) {
							logger.debug("setup还没有正常返回");
						} else {
							setupFlag = true;
						}
						break;
					case play:
						break;
					case scale:
						break;
					case pause:
						break;
					default:
						break;
					}
					try {
						if (setupFlag) {
							//定时检查
							doPing();
							Thread.sleep(TCPServer.getConfig().getDooptionTime());
						} else {
							Thread.sleep(100);
						}
					} catch (final Exception e) {
					}
				}
			} catch (final Exception e) {
				e.printStackTrace();
			}
		}

		shutdown();
	}
	
	/*
	 * 处理CDN返回报文
	 */
	public void handle(byte[] msg) {
		String tmp = new String(msg);
		logger.debug("【VOD Res Ctrl】CDN return message：\r\n" + tmp);
		//int cseqPos = tmp.toUpperCase().indexOf("CSEQ");
		int cseqPos = tmp.toUpperCase().lastIndexOf("CSEQ");
		String seqStr = "";
		if (cseqPos > 0) {
			seqStr = tmp.substring(cseqPos + 6);
			seqStr = seqStr.substring(0, seqStr.indexOf("\r\n")).trim();
			logger.debug("return seq:" + seqStr + ";oper:"
					+ ctrl.getSetupoperHis().get(Integer.valueOf(seqStr)));
		}
		
//		String rtime1 = getValueByKey(tmp, "Range: npt=");
//		String rtemps1[] = rtime1.split("\\-");
//		if (rtemps1.length == 2) {
//			beginTimeStr = rtemps1[0];
//			endTimeStr = rtemps1[1];
//			ctrl.setCurrentTime(beginTimeStr);
//			ctrl.setTotalTime(endTimeStr);
//			// reset Controller, update play time
//		}
		
		if (tmp.startsWith(RTSP_MOVED)) {
			shutdown.set(true);
			String rtspUrl = tmp.substring(tmp.indexOf("rtsp://"));

			String playUrl = rtspUrl.replace('\r', ' ').replace('\n', ' ')
					.replaceAll(" {2,}", " ").trim();

			rtspUrl = rtspUrl.substring(7);
			String ip = rtspUrl.substring(0, rtspUrl.indexOf(":"));
			int port = Integer.valueOf(rtspUrl.substring(
					rtspUrl.indexOf(":") + 1, rtspUrl.indexOf("/")));
			RTSPClient client = new RTSPClient(new InetSocketAddress(ip, port),
					new InetSocketAddress(TCPServer.getConfig()
							.getLocalIpAddress(), 0), playUrl, ctrl, session);
			ctrl.setClient(client);
			client.setStbid(stbid);
			client.setListenIp(listenIp);
			client.setListenPort(listenPort);
			client.setRegionId(regionId);
			client.setSerialNo(serialNo);
			client.setCmd(cmd);
			client.setDoneDate(doneDate);
			client.setSessionid(sessionid);
			client.start();
			return;
		}
		if (tmp.contains(CLOSE_REASON_END) && tmp.contains(RTSP_CLOSE)) {
			shutdown.set(true);
			IOSocketClient client = new IOSocketClient();
			String ip = TCPServer.getConfig().getCSCGAddress();
			int port = TCPServer.getConfig().getCSCGPort();
			client.sendStr(
					ip,
					port,
					1000,
					null,
					VODResCtrl.getInstance().getGoBackResp("goback", doneDate,
							serialNo, stbid, spId, messageInfo,ctrl.getStbid()), null);
		}

		//返回正常
		if (tmp.indexOf(RTSP_OK) >= 0) {
			/*
			 * 返回流程用switch
			 */
			switch (sysStatus) {
			case init:
				break;
			case setup:
				break;
			case play:
				break;
			case pause:
				break;
			case scale:
				break;
			case teardown:
				break;
			default:
				break;
			}
			/*
			 * 判断命令是否成功，然后返回报文数据
			 */
			switch (ctrl.getSetupoperHis().get(Integer.valueOf(seqStr))) {
			case init:
				logger.debug("init ok");
				break;
			case options:
				logger.debug("options ok");
				break;
			case describe:
				logger.debug("describe ok");
				break;
			case setup:
				sessionid = tmp.substring(tmp.indexOf("Session: ")+9,tmp.indexOf("OnDemandSessionId")).trim();
				String OnDemandSessionId = tmp.substring(tmp.indexOf("OnDemandSessionId:")+18,tmp.indexOf("ClientSessionId")).trim();
				String ClientSessionId = tmp.substring(tmp.indexOf("ClientSessionId:")+16,tmp.indexOf("Content-Type")).trim();
				String rtspUrl = tmp.substring(tmp.indexOf("rtsp://")+7,tmp.indexOf("c=IN")).trim();
				String playRtspUrl = rtspUrl.split("/")[0];		//play 请求真实用到的rtsp地址
				ctrl.setClientSessionId(ClientSessionId);
				ctrl.setOnDemandSessionId(OnDemandSessionId);
				if (sessionid != null && sessionid.length() > 0) {
					sysStatus = Status.setup;
					ctrl.setStatus(Commons.Status.setup);
					String ip = playRtspUrl.substring(0, playRtspUrl.indexOf(":"));
					int port = Integer.valueOf(playRtspUrl.substring(
							playRtspUrl.indexOf(":") + 1, playRtspUrl.length()));
					RtspPlayClient client = new RtspPlayClient(new InetSocketAddress(ip, port),
							new InetSocketAddress(TCPServer.getConfig()
									.getLocalIpAddress(), 0), "rtsp://"+playRtspUrl, ctrl, session,sessionid);
					client.setStbid(stbid);
					client.setRegionId(regionId);
					client.setSerialNo(serialNo);
					client.setListenIp(listenIp);
					client.setListenPort(listenPort);
					client.setCmd(cmd);
					client.setDoneDate(doneDate);
					client.setSessionid(sessionid);
					client.setSpId(spId);
					ctrl.setPlayClient(client);
					if(TCPServer.resetController(ctrl)){
						client.start();
						try {
							Thread.sleep(3000);
							setupFlag = true;
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
				break;
			case play:
//				logger.debug("play ok");
//				logger.info("【VGW网关】=========播放开始，正确返回...");
//				String time = getValueByKey(tmp,"Range: npt=");
////				String delayStr = getValueByKey(tmp,"x-Ts-info: ");
////				String scaleStr = getValueByKey(tmp,"Scale: ");
//				String temps[] =time.split("\\-");
//				if(temps.length==2){
//					beginTimeStr = temps[0];
//					endTimeStr = temps[1];
////					float bt = Float.parseFloat(beginTimeStr);
////					float et = Float.parseFloat(endTimeStr);
////					float delay = Float.parseFloat(delayStr);
////					float scale = Float.parseFloat(scaleStr);
////					float xtsinfo =  Float.parseFloat(delayStr)/1000000;
////					
////					//当前播放时间(秒) = begin(秒) + (cur-ts - x-Ts-info)/1000000*scale
////					float currTime = bt+()
//					
//					ctrl.setCurrentTime(beginTimeStr);
//					ctrl.setTotalTime(endTimeStr);
//					//重置Controler，更新播放时间
//					TCPServer.resetController(ctrl);
//				}
//				logger.info("【VGW】==========播放开始，正确返回总时长,"+totalTimeStr);
//				VODResCtrl.getInstance().returnMessage(
//						session,
//						VODResCtrl.getInstance().getResp(this.cmd,
//								this.doneDate, this.serialNo, this.listenIp,
//								this.sessionid, this.listenPort,
//								this.messageInfo, VODConst.VOD_OK, endTimeStr));
				break;
				
			case choosetime:
				logger.debug("choosetime-play ok");
				break;
			//继续播放
			case resume:
//				logger.debug("resume-play ok");
//				logger.info("【VGW】=========继续播放开始，正确返回...");
//				String rtime = getValueByKey(tmp,"Range: npt=");
//				String rtemps[] =rtime.split("\\-");
//				if(rtemps.length==2){
//					beginTimeStr = rtemps[0];
//					endTimeStr = rtemps[1];
//					ctrl.setCurrentTime(beginTimeStr);
//					ctrl.setTotalTime(endTimeStr);
//					//重置Controler，更新播放时间
//					TCPServer.resetController(ctrl);
//				}
//				logger.info("【VGW】==========继续播放播放，返回时间"+rtime);
				break;
			case pause:
				logger.debug("pause ok");
				break;
			case scale:
				logger.debug("scale ok");
				break;
			case teardown:
				logger.debug("teardown ok");
				break;
			default:
				break;
			}
			isSended = false;
		} else if (!tmp.startsWith(RTSP_MOVED)) {
			logger.debug("返回错误：" + tmp);
//			VODResCtrl.getInstance().returnMessage(
//					session,
//					VODResCtrl.getInstance().getResp(this.cmd,
//							this.doneDate, this.serialNo, "",
//							this.sessionid, -1,
//							"", VODConst.INTERNAL_ERROR,""));

		} else {
			logger.debug("返回错误：" + tmp);
//			VODResCtrl.getInstance().returnMessage(
//					session,
//					VODResCtrl.getInstance().getResp(this.cmd,
//							this.doneDate, this.serialNo, "",
//							this.sessionid, -1,
//							"", VODConst.INTERNAL_ERROR,""));
			setupFlag = false;
			shutdown.set(true);
		}

	}

	public void connect(SelectionKey key) throws IOException {
		if (isConnected()) {
			return;
		}
		// 完成SocketChannel的连接
		socketChannel.finishConnect();
		while (!socketChannel.isConnected()) {
			try {
				Thread.sleep(300);
			} catch (final InterruptedException e) {
			}
			socketChannel.finishConnect();
		}

	}

	public void error(Exception e) {
		e.printStackTrace();
	}

	public void read(SelectionKey key) throws IOException {
		// 接收消息
		final byte[] msg = recieve();
		if (msg != null) {
			handle(msg);
		} else {
			key.cancel();
		}
	}

	
	public void doPing() {
		StringBuilder sb = new StringBuilder();
		sb.append("PING ");
		sb.append(this.address);
		sb.append(VERSION);
		sb.append("CSeq: ");
		sb.append(seq++);
		sb.append("\r\n");
		sb.append("Session: ");
		sb.append(this.sessionid);
		sb.append("\r\n");
		sb.append("OnDemandSessionId: ");
		sb.append(ctrl.getOnDemandSessionId());
		sb.append("\r\n");
		sb.append("\r\n");
		if (ctrl.getSetupoperHis() != null)
			ctrl.getSetupoperHis().put(seq - 1, Commons.Status.options);
		logger.debug("【VOD网关】doPing：" + sb.toString());
//		send(sb.toString().getBytes());
		sendAndrev(sb.toString());
	}
	
	
	public void doTeardown() {
		StringBuilder sb = new StringBuilder();
		sb.append("TEARDOWN ");
		sb.append(this.address);
		sb.append(VERSION);
		sb.append("CSeq: ");
		sb.append(seq++);
		sb.append("\r\n");
		sb.append("Require: ");
		sb.append(this.REQUIRE);
		sb.append("\r\n");
		sb.append("Reason: 200 \"user pressed stop\"");
		sb.append("\r\n");
		sb.append("Session: ");
		sb.append(this.sessionid);
		sb.append("\r\n");
		sb.append("OnDemandSessionId: ");
		sb.append(ctrl.getOnDemandSessionId());
		sb.append("\r\n");
		sb.append("ClientSessionId: ");
		sb.append(ctrl.getClientSessionId());
		sb.append("\r\n");
		sb.append("\r\n");
		if (ctrl.getSetupoperHis() != null)
			ctrl.getSetupoperHis().put(seq - 1, Commons.Status.teardown);
		logger.debug("【VOD网关】doTeardown：" + sb.toString());
//		send(sb.toString().getBytes());
		sendAndrev(sb.toString());
	}

	
	public void doSetup() {
		StringBuilder sb = new StringBuilder();
		sb.append("SETUP ");
		sb.append(this.address);
		sb.append("/;");
		sb.append("purchaseToken=");
		sb.append(this.purchaseToken);
		sb.append(";serverID=1.1.1.1");
		sb.append(VERSION);
		sb.append("CSeq: ");
		sb.append(seq++);
		sb.append("\r\n");
		sb.append("StreamControlProto: rtsp");
		sb.append("\r\n");
		sb.append("Transport: MP2T/DVBC/QAM;unicast;client=");
		sb.append(this.client);
		sb.append(";qam_name=");
		sb.append(this.qam_name);
		sb.append(",MP2T/DVBC/UDP;unicast;client=");
		sb.append(this.client);
		sb.append(";destination=");
		sb.append(this.destination);
		sb.append(";client_port=");
		sb.append(this.client_port);
		sb.append("\r\n");
		sb.append("Require: ");
		sb.append(this.REQUIRE);
		sb.append("\r\n");
		sb.append("ClientSessionId: ");
		sb.append(StringUtil.getSerialno());
		sb.append("\r\n");
		sb.append("\r\n");
		if (ctrl.getSetupoperHis() != null)
			ctrl.getSetupoperHis().put(seq - 1, Commons.Status.setup);
		logger.debug("【VOD网关】doSetup：" + sb.toString());
//		send(sb.toString().getBytes());
		sendAndrev(sb.toString());
//		String a ="RTSP/1.0 200 OK\r\n"
//				+"CSeq: 1\r\n"
//				+"Transport: MP2T/DVBC/UDP;unicast;destination=10.169.0.15;client_port=6000;server_port=1;ssrc=1.2.3.1\r\n"
//				+"Session: 25124580\r\n"
//				+"OnDemandSessionId: 797258d3fc81424bb0616a51e1e32b56\r\n"
//				+"ClientSessionId: 0023b8a8d8bc69\r\n"
//				+"Content-Type: application/sdp\r\n"
//				+"Content-Length: 169\r\n"
//
//				+"v=0\r\n"
//				+"o=- 797258d3fc81424bb0616a51e1e32b56 463145212 IN IP4 127.0.0.1\r\n"
//				+"s=\r\n"
//				+"t=0 0\r\n"
//				+"a=control:rtsp://10.255.201.59:5554/25124580\r\n"
//				+"c=IN IP4 0.0.0.0\r\n"
//				+"m=video 0 udp MP2T\r\n";
//		handle(a.getBytes());
	}

	
	public static byte[] hexStringToByte(String hex) {
		int len = (hex.length() / 2);
		byte[] result = new byte[len];
		char[] achar = hex.toCharArray();
		for (int i = 0; i < len; i++) {
			int pos = i * 2;
			result[i] = (byte) (toByte(achar[pos]) << 4 | toByte(achar[pos + 1]));
		}
		return result;
	}

	private static byte toByte(char c) {
		byte b = (byte) "0123456789ABCDEF".indexOf(c);
		return b;
	}
	
	private String getValueByKey(String input,String key){
		String ret = "";
		if(input.indexOf(key)!=-1){
			String temp = input.substring(input.indexOf(key) + key.length());
			ret = temp.substring(0, temp.indexOf("\r\n"));
		}
		return ret;
	}
	
	public void sendAndrev(String send){
		IOSocketClient client = new IOSocketClient();
		String a = client.sendStr(ip, port, 4000, null, send, null);
		isSended = true;
		handle(a.getBytes());
	}
}
