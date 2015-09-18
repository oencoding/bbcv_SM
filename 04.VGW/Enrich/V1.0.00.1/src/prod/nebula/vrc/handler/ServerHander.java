package prod.nebula.vrc.handler;

import java.util.HashMap;
import java.util.Map;

import net.sf.json.JSONObject;

import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.config.Command;
import prod.nebula.vrc.config.JSONConstants;
import prod.nebula.vrc.config.MsgConstants;
import prod.nebula.vrc.config.VODConst;
import prod.nebula.vrc.core.common.constants.ResConstants;
import prod.nebula.vrc.module.resctrl.Controller;
import prod.nebula.vrc.module.resctrl.VODResCtrl;
import prod.nebula.vrc.module.resctrl.dto.VodCtrlReq;
import prod.nebula.vrc.module.resdis.VODResDis;
import prod.nebula.vrc.service.TCPServer;
import prod.nebula.vrc.util.Commons;
import prod.nebula.vrc.util.StringUtil;

/**
 * VODM������
 * 
 * @author hj
 * 
 */
public class ServerHander extends IoHandlerAdapter {

	public final Logger logger = LoggerFactory.getLogger(getClass());

	// �׳��쳣�������¼�

	@Override
	public void exceptionCaught(IoSession session, Throwable cause)
			throws Exception {
		logger.warn("��VOD���ء�������Ϣ�����쳣", cause);
		session.close(true);
	}

	// Server���յ�TCP���󴥷����¼�
	@Override
	public void messageReceived(IoSession session, Object msg) throws Exception {
		String revStr = msg.toString();
		Map<String, Object> message = new HashMap<String, Object>();
		
		try {
			JSONObject revObj = JSONObject.fromObject(revStr);
			String serialNo = StringUtil.ToBeString(revObj
					.get(JSONConstants.SERIALNO));
			if (!StringUtil.assertNotNull(serialNo)) {
				serialNo = Commons.getSerialNo();
			}
			logger.info("��VOD�����ߡ��յ���Ϣ:" + revStr + serialNo);

			String cmd = StringUtil.ToBeString(revObj.get(JSONConstants.CMD));
			message.put(MsgConstants.COMMAND, cmd);
			message.put(MsgConstants.SERIALNO, serialNo);
			message.put(MsgConstants.DONEDATE, StringUtil.getNowDate());
			message.put(MsgConstants.APPUSERID,
					StringUtil.ToBeString(revObj.get(JSONConstants.APPUSERID)));
			message.put(MsgConstants.SPUSERID,
					StringUtil.ToBeString(revObj.get(JSONConstants.SPUSERID)));
			message.put(MsgConstants.AREAID,
					StringUtil.ToBeString(revObj.get(JSONConstants.AREAID)));
			message.put(JSONConstants.SESSIONID,
					StringUtil.ToBeString(revObj.get(JSONConstants.SESSIONID)));
			Command command = Command.type(cmd);
			switch (command) {
			case LOGIN: {
				message.put(MsgConstants.SPID,
						StringUtil.ToBeString(revObj.get(JSONConstants.SID)));

				message.put(MsgConstants.IP,
						StringUtil.ToBeString(revObj.get(JSONConstants.IIP)));
				message.put(MsgConstants.PORT,
						StringUtil.ToBeString(revObj.get(JSONConstants.IPORT)));

				message.put(MsgConstants.GATEWAYIP,
						StringUtil.ToBeString(revObj.get(JSONConstants.SIP)));
				message.put(MsgConstants.GATEWAYPORT,
						StringUtil.ToBeString(revObj.get(JSONConstants.SPORT)));
				
				message.put(MsgConstants.MESSAGEINFO,
						StringUtil.ToBeString(revObj.get(JSONConstants.MSG)));
				
				message.put(MsgConstants.IPQAMIP,
						StringUtil.ToBeString(revObj.get(MsgConstants.IPQAMIP)));
				
				message.put(MsgConstants.IPQAMPORT,
						StringUtil.ToBeString(revObj.get(MsgConstants.IPQAMPORT)));
				
				message.put(MsgConstants.PID,
						StringUtil.ToBeString(revObj.get(MsgConstants.PID)));
				
				message.put(MsgConstants.FREQUENCY,
						StringUtil.ToBeString(revObj.get(MsgConstants.FREQUENCY)));
				/*
				 * VODResCtrl.getInstance().returnMessage( session,
				 * VODResCtrl.getInstance().getResp(cmd, "", "", "", "", 0, "",
				 * VODConst.VOD_OK));
				 */
				logger.info("��VOD���ء���½���̿�ʼ");
				VODResDis.getInstance().login(session, message);
				logger.info("��VOD���ء���½���̽���");
			}
				break;
			case LOGOUT: {
				logger.info("��VOD���ء��˳����̿�ʼ");
				VODResDis.getInstance().logout(session, message);
				logger.info("��VOD���ء��˳����̽���");
			}
				break;
			case PLAY: {
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				Controller cdnCtrl = VODResCtrl.getInstance()
						.getCtrl(sessionid);
				VODResCtrl.getInstance().VODOperate(VODConst.KEYVALUE_PLAY,
						cdnCtrl);
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case RESUME: {
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				Controller cdnCtrl = VODResCtrl.getInstance()
						.getCtrl(sessionid);
				VODResCtrl.getInstance().VODOperate(VODConst.KEYVALUE_RESUME,
						cdnCtrl);
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case PAUSE: {
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				Controller cdnCtrl = VODResCtrl.getInstance()
						.getCtrl(sessionid);
				VODResCtrl.getInstance().VODOperate(VODConst.KEYVALUE_PAUSE,
						cdnCtrl);
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case FORWARD: {
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				
				Controller cdnCtrl = VODResCtrl.getInstance()
						.getCtrl(sessionid);
				VODResCtrl.getInstance().VODOperate(VODConst.KEYVALUE_FAST,
						cdnCtrl);
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case BACKWARD: {
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				Controller cdnCtrl = VODResCtrl.getInstance()
						.getCtrl(sessionid);
				VODResCtrl.getInstance().VODOperate(VODConst.KEYVALUE_SLOW,
						cdnCtrl);
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case CHOOSETIME: {
				VodCtrlReq vodctrl = (VodCtrlReq) JSONObject.toBean(revObj,
						VodCtrlReq.class);
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				Controller cdnCtrl = VODResCtrl.getInstance()
						.getCtrl(sessionid);
				logger.info("���յ��ĵ�ǰʱ�䣺"+revObj.getString("begintime"));
				logger.info("���յ��ĵ�ǰʱ��תVodCtrlReq��"+vodctrl.getBegintime());
				cdnCtrl.setCurrentTime(vodctrl.getBegintime());
				logger.info("���ú�ĵ�ǰʱ�䣺"+cdnCtrl.getCurrentTime());
				VODResCtrl.getInstance().VODOperate(
						VODConst.KEYVALUE_CHOOSETIME, cdnCtrl);
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case GETTIME: {
				String sessionid = StringUtil.ToBeString(revObj
						.get(JSONConstants.SESSIONID));
				session.write(retRespose(TCPServer.getControllerList().get(
						sessionid)));
				session.close(false);
			}
				break;
			case APP_PLATFORM_EXIST_REQ: {
				session.write("{\"retcode\":\"0\"}");
				session.close(false);
			}
				break;
			case SESSION_CHECK: {
				String sessionid = StringUtil.ToBeString(message
						.get(MsgConstants.APPUSERID));
				String code = "-1";
				// �Ự���
				if (TCPServer.getControllerList().get(sessionid) != null) {
					code = "0";
				}
				session.write("{\"retcode\":\"" + code + "\"}");
				session.close(false);
			}
				break;
			case QUERY_TYPE:{
				session.write("{\"cmd\":\"query_type\",\"ret_code\":\"0\",\"vgw_type\":\""+ResConstants.TYPE+"\"}");
				session.close(false);
			}
				break;
			}
		} catch (Exception e) {
			logger.error("��VOD���ء����ղ�������Ϣ�쳣", e);
		}
	}

	private String retRespose(Controller cdnCtrl) {
		JSONObject retObj = new JSONObject();
		retObj.put(JSONConstants.CURRENTTIME, cdnCtrl.getCurrentTime());
		retObj.put(JSONConstants.TOTALTIME, cdnCtrl.getTotalTime());
		return retObj.toString();
	}

}
