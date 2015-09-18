package prod.nebula.vrc.service;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import net.sf.json.JSONObject;

import org.apache.log4j.PropertyConfigurator;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import prod.nebula.vrc.config.ServerConfiguration;
import prod.nebula.vrc.core.CoreLoader;
import prod.nebula.vrc.core.common.constants.ResConstants;
import prod.nebula.vrc.module.resctrl.Constants;
import prod.nebula.vrc.module.resctrl.Controller;
import prod.nebula.vrc.module.resctrl.VGWEnum;
import prod.nebula.vrc.module.resctrl.VODResCtrl;
import prod.nebula.vrc.util.ApplicationContextHelper;
import prod.nebula.vrc.util.Commons;
import prod.nebula.vrc.util.IpUtil;
import prod.nebula.vrc.util.client.Client;
import prod.nebula.vrc.util.client.UdpClient;

public class TCPServer {
	public static final Logger logger = LoggerFactory
			.getLogger(TCPServer.class);
	/** UDP��������ֽ� */
	public static final int VRC_READ_BUFFER_SIZE = 4096;

	/** UDP���뻺�����Ĵ�С */
	public static final int VRC_RECEIVE_BUFFER_SIZE = 1024;

	/** UDP����������Ĵ�С */
	public static final int VRC_SEND_BUFFER_SIZE = 1024;

	/** ����˿� */
	public static final int VRC_PORT = 8571;

	/** �̳߳ش�С */
	public static final int VRC_POOL_SIZE = 100;

	/** �ͻ���UDP��ʼ�˿� */
	public static final int VRC_CLIENT_BEGIN_UDP_PORT = 10155;

	/** �ͻ����̳߳ش�С */
	public static final int VRC_CLIENT_POOL_SIZE = 10;

	/** �ɷ���˿ڴ�С */
	public static final int VRC_CLIENT_PORT_SIZE = 1000;

	/** ��ͷ */
	public static final String VRC_DATAGRAM_HEADER = "XXBB";

	/** ��β */
	public static final String VRC_DATAGRAM_END = "XXEE";

	/**
	 * ����IP��ַ
	 */
	public static final String VRC_LOCALIPADDRESS = "10.0.138.8";

	public static int VRC_CHANGE = 0;

	/** ServerӦ�������ļ��� */
	private final String VRC_CONFIG_FILE = "config.properties";

	private static ServerConfiguration config = new ServerConfiguration();

	// �ڴ�����,��¼�û�״̬
	private static Map<String, Controller> controllerList = new HashMap<String, Controller>();
	
	public static ServerConfiguration getConfig() {
		return config;
	}

	public static Map<String, Controller> getControllerList() {
		return controllerList;
	}

	private int source;

	public final static int ENV = 1;

	public final static int OPT = 2;

	public final static int APP = 4;

	public static int beginPort;
	public static int[] portArray;
	public static int seq = 0;
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		logger.info("��VOD���ء���ʼ��ʼ��");
		TCPServer server = new TCPServer();
		try {
			new ClassPathXmlApplicationContext("spring/applicationContext.xml");
		} catch (Exception e) {
			logger.info("��VOD���ء�ģ������쳣", e);
		}
		logger.info("��VOD���ء���ģ��������");
		
		try {
			server.init(args);
			
			String rtspUrl = TCPServer.getConfig().getRegionId();
			if (null != rtspUrl && "".equals(rtspUrl)) {
				logger.info("regionid="+rtspUrl);
			}
			
			//������־�ϱ�
			reportBoot();
			// server.start();
		} catch (Exception e) {
			logger.error("��VOD���ء���ʼ���쳣" + e.getMessage());
			System.exit(1);
		}
		logger.info("��VOD���ء���ʼ�����,�󶨶˿�:" + TCPServer.config.getPort());
	}

	/**
	 * 
	 * @throws IOException
	 * 
	 *             public void start() throws Exception { try {
	 *             logger.info("��VOD���ء������̿�ʼ����"); SocketAcceptor acceptor = new
	 *             NioSocketAcceptor();// ����һ��TCP�Ľ�����
	 *             IoBuffer.setUseDirectBuffer(false); IoBuffer.setAllocator(new
	 *             SimpleBufferAllocator());
	 * 
	 *             acceptor.setReuseAddress(true);
	 *             acceptor.getSessionConfig().setReuseAddress(true);
	 *             acceptor.getSessionConfig().setKeepAlive(true);
	 *             acceptor.getSessionConfig().setTcpNoDelay(true);
	 *             acceptor.getSessionConfig().setReceiveBufferSize(4096);
	 *             acceptor.getSessionConfig().setMaxReadBufferSize(4096);
	 * 
	 *             // ���ý������Ĵ������ acceptor.setHandler(new ServerHander());
	 *             MinaTextLineCodecFactory lineCodec = new
	 *             MinaTextLineCodecFactory( "UTF-8", "XXEE", "XXEE");
	 *             lineCodec.setDecoderMaxLineLength(4096);
	 *             lineCodec.setEncoderMaxLineLength(4096);
	 * 
	 *             // �����̳߳� Executor threadPool =
	 *             Executors.newFixedThreadPool(config .getPoolSize());
	 *             acceptor.getFilterChain().addLast("exector", new
	 *             ExecutorFilter(threadPool));
	 *             acceptor.getFilterChain().addLast("codec", new
	 *             ProtocolCodecFilter(lineCodec));
	 *             acceptor.getFilterChain().addLast("logger", new
	 *             LoggingFilter());
	 * 
	 *             InetSocketAddress isa = new
	 *             InetSocketAddress(config.getPort()); acceptor.bind(isa);
	 *             TCPServer.beginPort = config.getClientBeginUDPPort();
	 *             TCPServer.portArray = new int[config.getClientPortSize()];
	 *             for (int i = 0; i < TCPServer.portArray.length; i++) {
	 *             TCPServer.portArray[i] = beginPort + i; }
	 *             logger.info("��VOD���ء������̼������"); } catch (Exception e) { throw
	 *             new Exception("��VOD���ء������̼���ʧ�ܣ�", e); }
	 * 
	 *             }
	 */
	/**
	 * ���������������
	 * 
	 * @param args
	 *            ���������в���
	 * @throws Exception
	 *             �����ȡӦ�������ļ�ʱ�����쳣
	 */
	private void init(String[] args) throws Exception {
		TCPServer.beginPort = config.getClientBeginUDPPort();
		TCPServer.portArray = new int[config.getClientPortSize()];
		for (int i = 0; i < TCPServer.portArray.length; i++) {
			TCPServer.portArray[i] = beginPort + i;
		}
		try {
			initLog4j();
			
			Properties p = loadConfigFile();
			populate(p);
			logger.info(TCPServer.config.toString());
		} catch (Exception e) {
			throw new Exception("���������ļ��쳣", e);
		}
	}
	
	 public void initLog4j() throws IOException {
		String configFilePath;
		Properties p = null;
		configFilePath = System.getenv("VRC_LOG_PATH");
		if (configFilePath != null && !configFilePath.equals("")) {
			p = Commons.loadFile(configFilePath);
			if (p != null) {
				source = TCPServer.ENV;
				PropertyConfigurator.configure(configFilePath);
				return;
			}
		}
		
		configFilePath = System.getProperty("VRC_LOG_PATH");
		if (configFilePath != null && !configFilePath.equals("")) {
			p = Commons.loadFile(configFilePath);
			if (p != null) {
				source = TCPServer.OPT;
				PropertyConfigurator.configure(configFilePath);
				return;
			}
		}
	}
	
	private Properties loadConfigFile() throws IOException {
		Properties p = null;
		String configFilePath;
		configFilePath = System.getenv("VRC_CONFIG_FILE");
		logger.debug("System getenv VRC_CONFIG_FILE=" + configFilePath);
		if (configFilePath != null && !configFilePath.equals("")) {
			p = Commons.loadFile(configFilePath);
			if (p != null) {
				source = TCPServer.ENV;
				return p;
			}
		}

		configFilePath = System.getProperty("VRC_CONFIG_FILE");
		logger.debug("System getProperty VRC_CONFIG_FILE=" + configFilePath);
		if (configFilePath != null && !configFilePath.equals("")) {
			p = Commons.loadFile(configFilePath);
			if (p != null) {
				source = TCPServer.OPT;
				return p;
			}
		}
		source = TCPServer.APP;
		return Commons.loadPropertiesFile(VRC_CONFIG_FILE);
	}
	
	/**
	 * 
	 * @return
	 * @throws IOException
	 */
	private ServerConfiguration populate(Properties p) throws IOException {
		config.setPort(Commons.getIntPropertyValue(p, "vgw.port", VRC_PORT));
		config.setPoolSize(Commons.getIntPropertyValue(p, "vgw.pool.size",
				VRC_POOL_SIZE));
		config.setReadBufferSize(Commons.getIntPropertyValue(p,
				"VRC_READ_BUFFER_SIZE", VRC_READ_BUFFER_SIZE));
		config.setReceiveBufferSize(Commons.getIntPropertyValue(p,
				"VRC_RECEIVE_BUFFER_SIZE", VRC_RECEIVE_BUFFER_SIZE));
		config.setSendBufferSize(Commons.getIntPropertyValue(p,
				"VRC_SEND_BUFFER_SIZE", VRC_SEND_BUFFER_SIZE));
		config.setClientBeginUDPPort(Commons.getIntPropertyValue(p,
				"VRC_CLIENT_BEGIN_UDP_PORT", VRC_CLIENT_BEGIN_UDP_PORT));
		config.setClientPoolSize(Commons.getIntPropertyValue(p,
				"VRC_CLIENT_POOL_SIZE", VRC_CLIENT_POOL_SIZE));
		config.setClientPortSize(Commons.getIntPropertyValue(p,
				"VRC_CLIENT_PORT_SIZE", VRC_CLIENT_PORT_SIZE));
		config.setDatagramHeader(Commons.getStringPropertyValue(p,
				"VRC_DATAGRAM_HEADER", VRC_DATAGRAM_HEADER));
		config.setDatagramEnd(Commons.getStringPropertyValue(p,
				"VRC_DATAGRAM_END", VRC_DATAGRAM_END));
		config.setLocalIpAddress(Commons.getStringPropertyValue(p,
				"vgw.localipaddress", VRC_LOCALIPADDRESS));
		config.setRegionId(Commons
				.getStringPropertyValue(p, "VRC_REGIONID", ""));
		config.setRtspAddr(Commons.getStringPropertyValue(p, "VRC_RTSP_ADDR",
				""));
		
		config.setBwIp(Commons.getStringPropertyValue(p, "bgw.ip", ""));
		config.setBwPort(Commons.getIntPropertyValue(p, "bgw.port", 0));
		config.setInterval(Commons.getIntPropertyValue(p,
				"bgw.heartbeat.interval", 6000));
		config.setVodTimeout(Commons.getIntPropertyValue(p, "vod.timeout",
				86400));
		config.setDooptionTime(Commons.getIntPropertyValue(p, "dooption.time",
				20000));
		
		config.setCtasIp(Commons.getStringPropertyValue(p, "ctas.ip",
				"127.0.0.1"));
		config.setCtasPort(Commons.getIntPropertyValue(p, "ctas.port", 20000));
		config.setSource(source);
		return config;
	}

	/**
	 * �����������Ƶ���ſ�����
	 * 
	 * @return
	 */
	public static synchronized boolean resetController(Controller ctrl) {
		try {
			String sessionId = ctrl.getSessionId();
			if (!Commons.isNullorEmptyString(sessionId)) {
				controllerList.put(sessionId, ctrl);
			}
		} catch (Exception e) {
			return false;
		}
		return true;
	}

	/**
	 * ����������Ƶ���ſ�����
	 * 
	 * @return
	 */
	public static synchronized boolean putController(Controller ctrl) {
		try {
			String sessionId = ctrl.getSessionId();
			if (!Commons.isNullorEmptyString(sessionId)) {
				Object object = controllerList.get(sessionId);
				if (null != object && object instanceof Controller) {
					Controller temp = (Controller) object;
					VODResCtrl.getInstance().logout(temp);
					temp = null;
				}
				controllerList.put(sessionId, ctrl);
			}
		} catch (Exception e) {
			return false;
		}
		return true;
	}
	
	 public static void reportBoot(){
		    /*
			 * XXXBB{"cmd":"bootlog","appname":"CSCS","version":"0.9.1","hostip":"218.108.0.1","url":"3305","boottime":"2012-12-12 12:11:00.789"}XXEE
			 */
	        CoreLoader core = (CoreLoader)ApplicationContextHelper.getBean("core");
			String ReportIp = core.getProperties().getProperty(Constants.USERREPORT_IP,
					"127.0.0.1");
		    int ReportPort = Integer.parseInt(core.getProperties().getProperty(Constants.USERREPORT_PORT,
				     "36016"));
		    Client udpClient = new UdpClient();

		    int VGWPort = Integer.parseInt(core.getProperties().getProperty(Constants.VGW_PORT,
				     "8577"));
		    
			Date d = new Date();
			SimpleDateFormat sdf=new SimpleDateFormat(Constants.DATE_FORMAT_DETAIL);
			String currdate=sdf.format(d);
			
		    JSONObject json = new JSONObject();
		    json.put(VGWEnum.CMD.getDesc(), "bootlog");
		    json.put(VGWEnum.APPNAME.getDesc(), "VGW");
		    json.put(VGWEnum.VERSION.getDesc(), ResConstants.VERSION);
		    json.put(VGWEnum.HOSTIP.getDesc(), IpUtil.getIp());
		    json.put(VGWEnum.URL.getDesc(),"VGW���ʶ˿�:"+VGWPort);
		    json.put(VGWEnum.BOOTTIME.getDesc(), currdate);
		    udpClient.sendStr(ReportIp, ReportPort, 4000, null, Constants.COM_PREFIX+json.toString()+Constants.COM_SUFFIX);
		}
}
