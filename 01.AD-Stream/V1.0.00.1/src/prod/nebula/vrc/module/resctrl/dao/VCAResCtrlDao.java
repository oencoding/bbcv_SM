/**
 * 
 */
package prod.nebula.vrc.module.resctrl.dao;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.config.VODConst;
import prod.nebula.vrc.module.resctrl.dto.VODQuitlReqBean;
import prod.nebula.vrc.service.TCPServer;
import prod.nebula.vrc.util.Commons;

/**
 * VOD���ƴ���-��Դ����
 * 
 * @author �϶���
 * 
 */
public class VCAResCtrlDao {
	public final Logger logger = LoggerFactory.getLogger(getClass());

	private static String head = TCPServer.getConfig().getDatagramHeader();
	private static String end = TCPServer.getConfig().getDatagramEnd();

	private static VCAResCtrlDao vcaResCtrlDao = new VCAResCtrlDao();

	private VCAResCtrlDao() {

	}

	/**
	 * VOD���ƴ���-��Դ�������ȡ
	 * 
	 * @return VCAResCtrlDao VOD���ƴ���-��Դ������
	 */
	public static VCAResCtrlDao getInstance() {
		if (null == vcaResCtrlDao) {
			return vcaResCtrlDao = new VCAResCtrlDao();
		}

		return vcaResCtrlDao;
	}

	/**
	 * 
	 * 
	 * @param recvMsg
	 * @return
	 */
	public boolean checkParamForVODQuit(String recvMsg) {
		// ����ͷ��β�ж�
		if (!Commons.isNullorEmptyString(recvMsg) && recvMsg.indexOf(head) >= 0
				&& recvMsg.indexOf(end) >= 0) {
			recvMsg = recvMsg.substring(recvMsg.indexOf(head) + head.length(),
					recvMsg.indexOf(end));
			String[] revArr = recvMsg.split("\\|");

			if (revArr.length == 3 && !Commons.isNullorEmptyString(revArr[0])
					&& !Commons.isNullorEmptyString(revArr[1])
					&& !Commons.isNullorEmptyString(revArr[2])) {
				return true;
			}
		}
		return false;
	}

	/**
	 * ��ȡ����ģ������
	 * 
	 * @param revStr
	 * @return VODQuitlReqBean ������������ݵ�ģ��
	 */
	public VODQuitlReqBean getVODQuitBean(String recvMsg) {
		VODQuitlReqBean bean = new VODQuitlReqBean();
		recvMsg = recvMsg.substring(recvMsg.indexOf(head) + head.length(),
				recvMsg.indexOf(end));
		String[] revArr = recvMsg.split("\\|");
		bean.setCommand(revArr[1]);
		bean.setSerialNo(revArr[2]);
		return bean;
	}

	/**
	 * ���سɹ���Ϣ
	 * 
	 * @return String �ɹ���Ϣ
	 */
	public String getOKMsgForVodQuit(String serialNo) {
		// VOD_CA_VOD_QUIT_REQP��VOD��Դ����VOD�˳�Ӧ��ָ����
		// return_code�������Ƿ�ɹ���ֵ>=0��ʾ�ɹ����룬ֵ<0��ʾ�������
		// serial_no����ˮ��
		StringBuffer retMsg = new StringBuffer("");
		retMsg.append(head);
		retMsg.append(VODConst.VOD_CA_VOD_PLAY_RESP);
		retMsg.append(VODConst.SEPARATOR);
		retMsg.append(VODConst.VOD_QUITE_OK);
		retMsg.append(VODConst.SEPARATOR);
		retMsg.append(serialNo);
		retMsg.append(end);

		return retMsg.toString();
	}

	/**
	 * ��ô�����Ϣ
	 * 
	 * @param revArr
	 * @return
	 */
	public String getWrongMsgForVodQuit() {
		// VOD_CA_VOD_QUIT_REQP��VOD��Դ����VOD�˳�Ӧ��ָ����
		// return_code�������Ƿ�ɹ���ֵ>=0��ʾ�ɹ����룬ֵ<0��ʾ�������
		// serial_no����ˮ��
		StringBuffer retMsg = new StringBuffer("");
		retMsg.append(head);
		retMsg.append(VODConst.VOD_CA_VOD_PLAY_RESP);
		retMsg.append(VODConst.SEPARATOR);
		retMsg.append(VODConst.VOD_QUITE_FAIL);
		retMsg.append(VODConst.SEPARATOR);
		retMsg.append("");
		retMsg.append(end);

		return retMsg.toString();
	}

}
