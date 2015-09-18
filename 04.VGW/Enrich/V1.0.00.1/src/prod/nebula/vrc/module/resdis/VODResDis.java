/**
 * 
 */
package prod.nebula.vrc.module.resdis;

import java.net.InetSocketAddress;
import java.util.Map;

import net.sf.json.JSONObject;

import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.config.JSONConstants;
import prod.nebula.vrc.config.MsgConstants;
import prod.nebula.vrc.config.VODConst;
import prod.nebula.vrc.exception.VODException;
import prod.nebula.vrc.handler.RTSPClient;
import prod.nebula.vrc.module.resctrl.Controller;
import prod.nebula.vrc.module.resctrl.VODResCtrl;
import prod.nebula.vrc.module.resdis.dao.VCAResDisDao;
import prod.nebula.vrc.module.resdis.dto.VODResDisReqBean;
import prod.nebula.vrc.service.TCPServer;
import prod.nebula.vrc.util.Commons;
import prod.nebula.vrc.util.IpUtil;
import prod.nebula.vrc.util.ParamUtils;

/**
 * VOD���ƴ���--VOD��Դ����
 * 
 * @author �϶���
 * 
 */
public class VODResDis {
	public static final Logger logger = LoggerFactory
			.getLogger(VODResDis.class);
	/**
	 * Զ��CDN��������ַ
	 */
	private String remoteAddrIp;

	/**
	 * Զ��CDN�������˿�
	 */
	private int remoteAddrPort;

	private String localIpAddress;

	private static VODResDis vodResDis = new VODResDis();

	private VODResDis() {

	}

	public static VODResDis getInstance() {
		if (null == vodResDis) {
			return vodResDis = new VODResDis();
		}

		return vodResDis;
	}

	/**
	 * VOD��Դ���ȹ���
	 * 
	 * @param revArr
	 * @return
	 */
	public void login(IoSession session, Map<String, Object> message) {
		VODResDisReqBean request = null;
		try {
			// 1��������
			logger.info("��VOD���ء���ʼ����������Ϣ");
			request = VCAResDisDao.getInstance().getReqBean(message);
			logger.info("��VOD���ء���������������Ϣ");
			
			// int listenPort = TCPServer.beginPort + TCPServer.seq++;
			
			// ��ֻ֤��Ψһ��¼
			logout(request.getSessionId());
			
			// 2������Ƶ
			logger.info("��VOD���ء���ʼ������Ƶ����");
			startPlayer(request, session);
			logger.info("��VOD���ء���ɲ�����Ƶ����");
		} catch (VODException e) {
			logger.error("��VOD���ء�", e);
			int errorCode = e.getErrorCode();
			// String errorMessage = e.getMessage();
			VODResCtrl.getInstance().returnMessage(
					session,
					VODResCtrl.getInstance().getResp("login", "", "", "", "",
							-1, "", errorCode, ""));
		}

	}

	/**
	 * ��ʼ������Ƶ����
	 * 
	 * @param bean
	 *            VOD��Դ��������ģ��
	 */
	public void startPlayer(VODResDisReqBean bean, IoSession session)
			throws VODException {
		Controller sender = null;
		RTSPClient client = null;
		try {
			logger.debug("��ʼ��ÿ���UDP�˿�");
			int listenPort = IpUtil.getAvailablePort();
			logger.debug("��ÿ���UDP�˿ڣ�" + listenPort);
			localIpAddress = TCPServer.getConfig().getLocalIpAddress();
			String stbid = bean.getUserId();
			String rtspUrl = TCPServer.getConfig().getRtspAddr ();
			String regionId = TCPServer.getConfig().getRegionId();
			int length = 0;
			
			if (null != regionId && "".equals(regionId)) {
				regionId = bean.getRegionId();
			} else {
				length = regionId.split(",").length;
				regionId = regionId.split(",")[TCPServer.VRC_CHANGE];
				if (null != regionId) {
					regionId = regionId.trim();
				}
			}
			
			if (null != rtspUrl && "".equals(rtspUrl)) {
				rtspUrl = bean.getRtspAddr();
			} else {
				rtspUrl = rtspUrl.split(",")[TCPServer.VRC_CHANGE];
				if (null != rtspUrl) {
					rtspUrl = rtspUrl.trim();
				}
			}
			
			if (regionId.split(",").length > 0) {
				TCPServer.VRC_CHANGE++;
				if (TCPServer.VRC_CHANGE >= length) {
					TCPServer.VRC_CHANGE = 0;
				}
			}
			
			String serialNo = bean.getSerialNo();
			String doneDate = bean.getDoneDate();
			String cmd = bean.getCmd();
			String sessionId = bean.getSessionId();
			String appUserId = bean.getAppUserId();
			String spId = bean.getSpId();
			String areaid = bean.getAreaid();
			String ipqamIP = bean.getIpqamIP();
			String ipqamPort = bean.getIpqamPort();
			String frequency = bean.getFrequency();
			String pid = bean.getPid();

			logger.debug("��VOD���ء�appUserId=" + appUserId + " sessionId="
					+ sessionId + "areaid=" + areaid + " regionId=" + regionId
					+ " rtsp=" + rtspUrl+" localIpAddress="+localIpAddress);
			
			rtspUrl = rtspUrl.substring(rtspUrl.indexOf("rtsp://"));
			// String playUrl = rtspUrl.substring(0,
			// rtspUrl.lastIndexOf(".ts") + 3);
			String playUrl = rtspUrl;
			rtspUrl = rtspUrl.substring(7);
			remoteAddrIp = rtspUrl.substring(0, rtspUrl.indexOf(":"));
			remoteAddrPort = Integer.valueOf(rtspUrl.substring(
					rtspUrl.indexOf(":") + 1, rtspUrl.indexOf("/")));

			sender = new Controller(listenPort);
			sender.setRuntime(System.currentTimeMillis() / 1000);
			client = new RTSPClient(new InetSocketAddress(remoteAddrIp,
					remoteAddrPort), new InetSocketAddress(localIpAddress, 0),
					playUrl, sender, session);
			client.setStbid(appUserId);
			client.setRegionId(regionId);
			client.setSerialNo(serialNo);
			client.setListenIp(localIpAddress);
			client.setListenPort(listenPort);
			client.setCmd(cmd);
			client.setDoneDate(doneDate);
			client.setSessionid(sessionId);
			client.setSpId(spId);
			client.setIpqamIp(ipqamIP);
			client.setIpqamPort(ipqamPort);
			client.setFrequency(frequency);
			client.setPid(pid);
			sender.setStbid(stbid);
			sender.setClient(client);
			sender.setRegionId(regionId);
			sender.setSerialNo(serialNo);
			sender.setSessionId(sessionId);
			sender.setAppUserId(appUserId);
			sender.setAreaid(areaid);
			
			if (TCPServer.putController(sender)) {
				client.start();
				sender.start();
			}
		} catch (Exception e) {
			VODResCtrl.getInstance().logout(sender);
			if (sender != null) {
				logger.debug("����ֹͣ�ػ��߳�");
				sender.getAcceptor().dispose(false);
				sender.setRuntimeThread(false);
			}
			TCPServer.getControllerList().remove(sender.getSessionId());
			client = null;
			sender = null;
			throw new VODException(VODConst.PLAYVOD_ERROR, "����VOD�������쳣");
		}
	}

	/**
	 * �ǳ�����
	 * 
	 * @param msgreq
	 */
	public void logout(IoSession session, Map<String, Object> message) {

		JSONObject retObj = new JSONObject();
		retObj.put(JSONConstants.CMD,
				ParamUtils.getParameter(message, MsgConstants.COMMAND, ""));
		retObj.put(JSONConstants.RETCODE, "0");
		retObj.put(
				JSONConstants.SERIALNO,
				ParamUtils.getParameter(message, MsgConstants.SERIALNO,
						Commons.getSerialNo()));
		VODResCtrl.getInstance().returnMessage(session, retObj.toString());

		try {
			logger.debug("start logout");
			String sessionId = ParamUtils.getParameter(message,
					JSONConstants.SESSIONID, "");
			logger.info("sessionId=" + sessionId);
			logout(sessionId);
		} catch (Exception e) {
			logger.error("��VOD���ء����ٲ�����Ƶ�߳�ʧ��");
		}

	}

	public static void logout(String sessionId) {
		if (!Commons.isNullorEmptyString(sessionId)) {
			TCPServer.getControllerList().containsKey(sessionId);
			Object object = TCPServer.getControllerList().get(sessionId);
			if (null != object && object instanceof Controller) {
				Controller temp = (Controller) object;
				VODResCtrl.getInstance().logout(temp);
				if (temp != null) {
					logger.debug("����ֹͣ�ػ��߳�");
					temp.getAcceptor().dispose(false);
					temp.setRuntimeThread(false);
				}
				TCPServer.getControllerList().remove(sessionId);
				temp = null;
			}
		}
	}
}
