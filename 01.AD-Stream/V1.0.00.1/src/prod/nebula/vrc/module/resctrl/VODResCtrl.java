/**
 * 
 */
package prod.nebula.vrc.module.resctrl;

import java.nio.charset.Charset;
import java.util.Map;
import java.util.Map.Entry;

import net.sf.json.JSONObject;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.config.Command;
import prod.nebula.vrc.config.JSONConstants;
import prod.nebula.vrc.config.VODConst;
import prod.nebula.vrc.module.resctrl.dto.VODResCtrlReqBean;
import prod.nebula.vrc.service.TCPServer;
import prod.nebula.vrc.util.Commons;
import prod.nebula.vrc.util.client.IOSocketClient;
import prod.nebula.vrc.util.client.TcpClient;

/**
 * VOD���ƴ���--VOD��Դ������
 * 
 * @author �϶���
 * 
 */
public class VODResCtrl {
	public static final Logger logger = LoggerFactory
			.getLogger(VODResCtrl.class);

	private Controller ctrl;

	private static VODResCtrl vodResCtrl = new VODResCtrl();

	private VODResCtrl() {

	}

	/**
	 * ��ȡVOD��Դ���ƹ�����ʵ��
	 * 
	 * @return VODResCtrl VOD��Դ���ƹ�����ʵ��
	 */
	public static VODResCtrl getInstance() {
		if (null == vodResCtrl) {
			return vodResCtrl = new VODResCtrl();
		}

		return vodResCtrl;
	}

	/**
	 * VOD��Դ���ƹ�����,��Ӧ��������
	 * 
	 * @param session
	 *            ������Ϣ
	 * @param message
	 *            ���յ�������
	 */
	public void VODresDisManage(IoSession session, String keyValue,
			Controller ctrl) {

		try {
			this.ctrl = ctrl;
			this.ctrl.setRuntime(System.currentTimeMillis() / 1000);
			logger.debug("��VOD���ء���ǰ״̬:" + ctrl.getStatus());
			logger.debug("��VOD���ء���ǰ���ܼ�ֵ:"+keyValue);
			if ("11".equals(keyValue)) {
				sendCTAS(VODConst.KEYVALUE_LEFT, ctrl);
			} else if ("6".equals(keyValue)) {
				sendCTAS(VODConst.KEYVALUE_RIGHT, ctrl);
			}
			switch (ctrl.getStatus()) {
			case init:
				break;
			case options:
				break;
			case describe:
				break;
			case setup:
				break;
			case scale:
				// 1.���״̬�°����
				if (ctrl.getClient().getScale() > 0
						&& keyValue.equals(VODConst.KEYVALUE_FAST)) {
					if (ctrl.getClient().getScale() < VODConst.SCALE_FAST_TIMES) {
						ctrl.getClient().setScale(
								ctrl.getClient().getScale() + 2);
						doScale(ctrl);
					} else {
						doPlay(ctrl);
					}
				}
				// 2.����״̬�°�����
				if (ctrl.getClient().getScale() < 0
						&& keyValue.equals(VODConst.KEYVALUE_SLOW)) {
					if (ctrl.getClient().getScale() > VODConst.SCALE_SLOW_TIMES) {
						ctrl.getClient().setScale(
								ctrl.getClient().getScale() - 2);
						doScale(ctrl);
					} else {
						doPlay(ctrl);
					}

				}

				// 3.����״̬�µ���
				if ( keyValue.equals(VODConst.KEYVALUE_FAST)) {
					doPlay(ctrl);
				}

				// 4.���״̬�µ����
				if ( keyValue.equals(VODConst.KEYVALUE_SLOW)) {
					doPlay(ctrl);
				}

				// 5�κ�״̬�µ㲥��
				if (keyValue.equals(VODConst.KEYVALUE_PLAY)) {
					doPlay(ctrl);
				}

				// 6�κ�״̬�µ��˳�������
				if (keyValue.equals(VODConst.KEYVALUE_EXIT)
						|| keyValue.equals(VODConst.KEYVALUE_BACK)) {
					back();
				}
				break;
			case play:
				// ��ǰ״̬Ϊ���ţ���ȷ����ͣ
				if (keyValue.equals(VODConst.KEYVALUE_PLAY)) {
					doPause(ctrl);
				}

				// ��ǰ״̬Ϊ���ţ�������/�˳�(ֹͣ) ֹͣ
				if (keyValue.equals(VODConst.KEYVALUE_EXIT)
						|| keyValue.equals(VODConst.KEYVALUE_BACK)) {
					back();
				}

				// ����״̬ ���
				if (keyValue.equals(VODConst.KEYVALUE_FAST)) {
					logger.debug("��ǰ����Ϊ����������Ϊ2.����������");
					ctrl.getClient().setScale(2);
					doScale(ctrl);
				}

				// ����״̬ ����
				if (keyValue.equals(VODConst.KEYVALUE_SLOW)) {
					logger.debug("��ǰ����Ϊ���ˣ�����Ϊ-2.����������");
					ctrl.getClient().setScale(-2);
					doScale(ctrl);
				}
				break;
			case pause:
				// ��ǰ״̬Ϊ��ͣ����ȷ�ϲ���
				if (keyValue.equals(VODConst.KEYVALUE_PLAY)) {
					doPlay(ctrl);
				}

				// ��ǰ״̬Ϊ��ͣ��������/�˳�(ֹͣ) ֹͣ
				if (keyValue.equals(VODConst.KEYVALUE_EXIT)
						|| keyValue.equals(VODConst.KEYVALUE_BACK)) {
					back();
				}
				break;
			case teardown:
				break;
			default:
				break;
			}

		} catch (Exception e) {
			logger.error("��VOD���ء�����VOD��Ƶ", e);
		}

	}

	/**
	 * ��CTAS�����������Ƽ�ֵ
	 * 
	 * @param keyvalueLeft
	 */
	private void sendCTAS(int keyvalue, Controller ctrl) {
		String ip = TCPServer.getConfig().getCtasIp();
		int port = TCPServer.getConfig().getCtasPort();
		logger.info("��VGW�������������Ƽ�  [" + ip + ":" + port + "]:" + keyvalue);
		send(keyvalue + "", ctrl, "2", ip, port);
		send(keyvalue + "", ctrl, "3", ip, port);
	}

	private void send(String keyvalue, Controller ctrl, String keystatus,
			String ip, int port) {
		IOSocketClient client = new IOSocketClient();

		JSONObject json = new JSONObject();
		json.put("cmd", "KeyDown");
		json.put("termid", "");
		json.put("outputid", ctrl.getSessionId());
		json.put("keydev", "1001");
		json.put("key", keyvalue);
		json.put("keystatus", keystatus);
		json.put("dvbinfo", ctrl.getAreaid() + ":0:0");
		client.sendStr(ip, port, 2000, null, json.toString() + "XXEE", null);
	}

	/**
	 * VOD��Դ���ƹ���������ӦHTTP����
	 * 
	 * @param keyValue
	 *            ��������
	 * @param ctrl
	 *            ��ǰ������
	 */
	public void VODOperate(String keyValue, Controller ctrl) {
		try {
			this.ctrl = ctrl;
			this.ctrl.setRuntime(System.currentTimeMillis() / 1000);
			logger.debug("��VOD���ء���ǰ״̬:" + ctrl.getStatus());
			switch (ctrl.getStatus()) {
			case init:
				break;
			case options:
				break;
			case describe:
				break;
			case setup:
				break;
			case scale:
				// 1.���״̬�°����
				if (ctrl.getClient().getScale() > 0
						&& keyValue.equals(VODConst.KEYVALUE_FAST)) {
					if (ctrl.getClient().getScale() < VODConst.SCALE_FAST_TIMES) {
						ctrl.getClient().setScale(
								ctrl.getClient().getScale() + 2);
						doScale(ctrl);
					} else {
						doPlay(ctrl);
					}
				}
				// 2.����״̬�°�����
				if (ctrl.getClient().getScale() < 0
						&& keyValue.equals(VODConst.KEYVALUE_SLOW)) {
					if (ctrl.getClient().getScale() > VODConst.SCALE_SLOW_TIMES) {
						ctrl.getClient().setScale(
								ctrl.getClient().getScale() - 2);
						doScale(ctrl);
					} else {
						doPlay(ctrl);
					}

				}

				// 3.����״̬�µ���
				if (ctrl.getClient().getScale() < 0
						&& keyValue.equals(VODConst.KEYVALUE_FAST)) {
					doPlay(ctrl);
				}
				
				// 4.���״̬�µ����
				if (ctrl.getClient().getScale() > 0
						&& keyValue.equals(VODConst.KEYVALUE_SLOW)) {
					doPlay(ctrl);
				}

				// 5�κ�״̬�µ㲥��
				if (keyValue.equals(VODConst.KEYVALUE_PLAY)) {
					doPlay(ctrl);
				}

				// 6�κ�״̬�µ��˳�������
				if (keyValue.equals(VODConst.KEYVALUE_EXIT)
						|| keyValue.equals(VODConst.KEYVALUE_BACK)) {
					back();
				}
				break;
			case play:
				// ��ǰΪ����״̬ ѡʱ�䲥��
				if (keyValue.equals(VODConst.KEYVALUE_CHOOSETIME)) {
					doChooseTime(ctrl);
				}

				// ��ǰ״̬Ϊ���ţ�����ͣ
				if (keyValue.equals(VODConst.KEYVALUE_PAUSE)) {
					doPause(ctrl);
				}

				// ��ǰ״̬Ϊ���ţ�������/�˳�(ֹͣ) ֹͣ
				if (keyValue.equals(VODConst.KEYVALUE_EXIT)
						|| keyValue.equals(VODConst.KEYVALUE_BACK)) {
					back();
				}

				// ����״̬ ���
				if (keyValue.equals(VODConst.KEYVALUE_FAST)) {
					ctrl.getClient().setScale(2);
					doScale(ctrl);
				}

				// ����״̬ ����
				if (keyValue.equals(VODConst.KEYVALUE_SLOW)) {
					ctrl.getClient().setScale(-2);
					doScale(ctrl);
				}
				break;
			case pause:
				// ��ǰ״̬Ϊ��ͣ��������
				if (keyValue.equals(VODConst.KEYVALUE_PLAY)) {
					doPlay(ctrl);
				}

				// ��ǰ״̬Ϊ��ͣ����������
				if (keyValue.equals(VODConst.KEYVALUE_RESUME)) {
					doResume(ctrl);
				}

				// ��ǰΪ��ͣ״̬ ѡʱ�䲥��
				if (keyValue.equals(VODConst.KEYVALUE_CHOOSETIME)) {
					doChooseTime(ctrl);
				}
				
				// ��ǰ״̬Ϊ��ͣ��������/�˳�(ֹͣ) ֹͣ
				if (keyValue.equals(VODConst.KEYVALUE_EXIT)
						|| keyValue.equals(VODConst.KEYVALUE_BACK)) {
					back();
				}
				break;
			case teardown:
				break;
			default:
				break;
			}

		} catch (Exception e) {
			logger.error("��VOD���ء�����VOD��Ƶ", e);
		}

	}

	/**
	 * ����Ӧ����ҳ
	 */
	@SuppressWarnings("unused")
	public void backToApp() {
		// ��ʱ������ṹ����Ϊ�ͷ���ǰ������Ϣ�ں�
		try {
			tearDownClient(ctrl);
		} catch (Exception e1) {
			logger.error("��VOD���ء�[�ر���Ƶ���Ž��ղ���ʧ��]", e1);
		}

		TcpClient tcpClient = new TcpClient();
		String sendMsg = getBackMsgToERM();
		logger.debug("send:" + TCPServer.getConfig().getCSCGAddress() + ":"
				+ TCPServer.getConfig().getCSCGPort() + "---:" + sendMsg);
		String revMsg = tcpClient.sendStr(TCPServer.getConfig()
				.getCSCGAddress(), TCPServer.getConfig().getCSCGPort(), 1000,
				null, sendMsg);
	}

	/**
	 * ���͸�CSCG�ķ�����Ϣ
	 * 
	 * @param stbid
	 * @param regionId
	 * @return
	 */
	private String getBackMsgToERM() {
		String retMsg = getGoBackResp("", "", ctrl.getSerialNo(),
				ctrl.getSessionId(), "1001", "");
		return retMsg;
	}

	/**
	 * ��ȡ����ģ������
	 * 
	 * @param revStr
	 * @return VODResCtrlReqBean ������������ݵ�ģ��
	 */
	@SuppressWarnings("unused")
	private VODResCtrlReqBean getBean(String revStr) {

		String head = TCPServer.getConfig().getDatagramHeader();
		String end = TCPServer.getConfig().getDatagramEnd();
		VODResCtrlReqBean bean = null;

		// ����ͷ��β�ж�
		if (revStr.indexOf(head) >= 0 && revStr.indexOf(end) >= 0) {

			revStr = revStr.substring(revStr.indexOf(head) + head.length(),
					revStr.indexOf(end));
			String[] revArr = revStr.split("\\|");

			// �����VOD��Դ����������ɷ��ؾ���ֵ
			if (!Commons.isNullorEmptyString(revArr[0])
					&& !Commons.isNullorEmptyString(revArr[1])) {
				bean = new VODResCtrlReqBean();
				bean.setHead(head);
				bean.setReqHead(revArr[0]);
				bean.setKeyValue(revArr[1]);
				bean.setEnd(end);
			}
		}

		return bean;
	}

	/**
	 * ���CDN������
	 * 
	 * @param sessionId
	 * @return
	 */
	public Controller getCtrl(String sessionId) {
		if (prod.nebula.vrc.util.StringUtil.assertNotNull(sessionId)) {
			Map<String, Controller> ctrls = TCPServer.getControllerList();
			if (null != ctrls && ctrls.size() > 0) {
				for (Entry<String, Controller> entry : ctrls.entrySet()) {
					if (entry.getKey().equals(sessionId)) {
						return entry.getValue();
					}
				}
			}
		}

		return null;

	}

	/**
	 * �رռ����߳�
	 * 
	 * @param ctrl
	 */
	private void tearDownClient(Controller ctrl) {
		ctrl.getClient().doTeardown();
		ctrl.setStatus(Commons.Status.teardown);
		ctrl.getClient().shutdown.set(true);
		ctrl.getClient().shutdown();
	}

	/**
	 * ��ͣ
	 * 
	 * @param ctrl
	 */
	private void doPause(Controller ctrl) {
		ctrl.getClient().doPause();
		ctrl.setStatus(Commons.Status.pause);
	}

	/**
	 * ����
	 * 
	 * @param ctrl
	 */
	private void doPlay(Controller ctrl) {
		ctrl.getClient().doPlay();
		ctrl.setStatus(Commons.Status.play);
	}

	/**
	 * ��/����
	 * 
	 * @param ctrl
	 */
	private void doScale(Controller ctrl) {
		logger.debug("����ģʽ����.....");
		ctrl.getClient().doScale();
		ctrl.setStatus(Commons.Status.scale);
	}

	/**
	 * ѡ��ʱ�䲥��
	 * 
	 * @param ctrl
	 */
	private void doChooseTime(Controller ctrl) {
		ctrl.getClient().doChooseTime(ctrl.getBeginTime());
		ctrl.setStatus(Commons.Status.play);
	}

	/**
	 * ��������
	 * 
	 * @param ctrl
	 */
	private void doResume(Controller ctrl) {
		ctrl.getClient().doResume();
		ctrl.setStatus(Commons.Status.play);
	}

	/**
	 * ����
	 */
	private void back() {
		try {
			backToApp();
		} catch (Exception e) {
			logger.error("��VOD���ء�back", e);
		}
	}

	/**
	 * �ǳ�
	 */
	public void logout(Controller ctrl) {
		try {
			tearDownClient(ctrl);
		} catch (Exception e1) {
			logger.error("��VOD���ء�[�ر���Ƶ���Ž����߳�ʧ��]", e1);
		}
	}

	/**
	 * ������Ϣ��ҵ�����
	 * 
	 * @param session
	 * @param retMsg
	 */
	public void returnMessage(IoSession session, String retMsg) {
		try {
			StringBuffer sf = new StringBuffer();
			sf.append(Commons.getStrValue(retMsg));
			IoBuffer buffer = IoBuffer.allocate(2048, true);// �����direct��heap��ʽ�Ļ�������
			buffer.setAutoExpand(true);// �Զ�����
			buffer.setAutoShrink(true);// �Զ�����
			buffer.putString(sf.toString(), Charset.forName(("gbk"))
					.newEncoder());
			buffer.flip();
			buffer.free();
			logger.info("��VOD�����ߡ�������Ϣ:" + retMsg);
			session.write(buffer);
			logger.info("��VOD���ء���Ϣ�������");
		} catch (Exception e) {
			logger.error("��VOD���ء�send message to CSCG fail", e);
		} finally {
			try {
				session.close(false);
			} catch (Exception e) {
				logger.error("��VOD���ء� TCP CLOSE Error: " + e);
			}
		}
	}

	public String getGoBackResp(String cmd, String doneDate, String serialNo,
			String sessionId, String spId, String messageInfo) {

		if (ctrl != null) {
			logger.debug("����ֹͣ�ػ��߳�");
			ctrl.getAcceptor().dispose(false);
			ctrl.setRuntimeThread(false);
		}
		TCPServer.getControllerList().remove(sessionId);

		JSONObject sendJson = new JSONObject();
		sendJson.put(JSONConstants.CMD, Command.GOBACK.value());
		sendJson.put(JSONConstants.SESSIONID, sessionId);
		// sendJson.put(JSONConstants.SPID, spId);
		sendJson.put(JSONConstants.SERIALNO, serialNo);
		sendJson.put(JSONConstants.MSG, messageInfo);
		
		return sendJson.toString() + "XXEE";
	}

	/**
	 * ��óɹ���Ϣ
	 * 
	 * @param revArr
	 * @return
	 */
	public String getResp(String cmd, String doneDate, String serialNo,
			String listenIp, String sessionId, int listenPort,
			String messageInfo, int errorCode, String totalTime) {
		JSONObject retObj = new JSONObject();
		retObj.put(JSONConstants.CMD, cmd);
		retObj.put(JSONConstants.RETCODE, String.valueOf(errorCode));
		if (errorCode == 0) {
			retObj.put(JSONConstants.CIP, listenIp);
			retObj.put(JSONConstants.CPORT, String.valueOf(listenPort));
			retObj.put(JSONConstants.MESSAGEINFO, messageInfo);
			retObj.put(JSONConstants.TOTALTIME, totalTime);
			retObj.put(JSONConstants.SERIALNO, serialNo);
		} else if (errorCode < 0) {
			if (ctrl != null) {
				logger.debug("����ֹͣ�ػ��߳�");
				ctrl.getAcceptor().dispose(false);
				ctrl.setRuntimeThread(false);
			}
			TCPServer.getControllerList().remove(sessionId);
		}
		return retObj.toString();
	}
}
