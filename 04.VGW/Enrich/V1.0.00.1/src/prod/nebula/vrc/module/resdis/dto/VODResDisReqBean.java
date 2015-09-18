package prod.nebula.vrc.module.resdis.dto;

/**
 * VOD��Դ���������װ��
 * 
 * @author �϶���
 * 
 */
public class VODResDisReqBean {

	/**
	 * VOD��Դ����-����ͷ��Ϣ
	 */
	private String head;

	/**
	 * VOD��Դ����-VOD���Ž�������ָ����
	 */
	private String reqHead;

	/**
	 * VOD��Դ����-��Ӫ���û���ţ���STB_ID
	 */
	private String userId;

	/**
	 * VOD��Դ����-�����ʶ
	 */
	private String regionId;

	/**
	 * VOD��Դ����-�㲥��RTSP��ַ
	 */
	private String rtspAddr;

	/**
	 * ��Ŀ���
	 */
	private String serviceId;

	/**
	 * ͨ��ģ��IP
	 */
	private String kgIp;

	/**
	 * ͨ��ģ���Ӧ�˿�
	 */
	private String kgPort;

	/**
	 * VOD��Դ����-��ˮ��
	 */
	private String serialNo;

	/**
	 * ����ʱ�䣨yyyyMMddHHmmss��
	 */
	private String doneDate;

	/**
	 * ��Ϣ����
	 */
	private String cmd;

	private String sessionId;

	private String appUserId;

	private String spId;

	private String areaid;
	
	private String ipqamIP;

	private String ipqamPort;
	
	private String frequency;
	
	private String pid;
	
	
	
	public String getIpqamIP() {
		return ipqamIP;
	}

	public void setIpqamIP(String ipqamIP) {
		this.ipqamIP = ipqamIP;
	}

	public String getIpqamPort() {
		return ipqamPort;
	}

	public void setIpqamPort(String ipqamPort) {
		this.ipqamPort = ipqamPort;
	}

	public String getFrequency() {
		return frequency;
	}

	public void setFrequency(String frequency) {
		this.frequency = frequency;
	}

	public String getPid() {
		return pid;
	}

	public void setPid(String pid) {
		this.pid = pid;
	}

	public String getAreaid() {
		return areaid;
	}

	public void setAreaid(String areaid) {
		this.areaid = areaid;
	}

	/**
	 * VOD��Դ����-����β��Ϣ
	 */
	private String end;

	public VODResDisReqBean() {

	}

	/**
	 * @return the head
	 */
	public String getHead() {
		return head;
	}

	/**
	 * @param head
	 *            the head to set
	 */
	public void setHead(String head) {
		this.head = head;
	}

	/**
	 * @return the reqHead
	 */
	public String getReqHead() {
		return reqHead;
	}

	/**
	 * @param reqHead
	 *            the reqHead to set
	 */
	public void setReqHead(String reqHead) {
		this.reqHead = reqHead;
	}

	/**
	 * @return the userId
	 */
	public String getUserId() {
		return userId;
	}

	/**
	 * @param userId
	 *            the userId to set
	 */
	public void setUserId(String userId) {
		this.userId = userId;
	}

	/**
	 * @return the regionId
	 */
	public String getRegionId() {
		return regionId;
	}

	/**
	 * @param regionId
	 *            the regionId to set
	 */
	public void setRegionId(String regionId) {
		this.regionId = regionId;
	}

	public String getSpId() {
		return spId;
	}

	public void setSpId(String spId) {
		this.spId = spId;
	}

	/**
	 * @return the rtspAddr
	 */
	public String getRtspAddr() {
		return rtspAddr;
	}

	/**
	 * @param rtspAddr
	 *            the rtspAddr to set
	 */
	public void setRtspAddr(String rtspAddr) {
		this.rtspAddr = rtspAddr;
	}

	/**
	 * @return the serviceId
	 */
	public String getServiceId() {
		return serviceId;
	}

	/**
	 * @param serviceId
	 *            the serviceId to set
	 */
	public void setServiceId(String serviceId) {
		this.serviceId = serviceId;
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
	 * @return the end
	 */
	public String getEnd() {
		return end;
	}

	/**
	 * @param end
	 *            the end to set
	 */
	public void setEnd(String end) {
		this.end = end;
	}

	/**
	 * @return the kgIp
	 */
	public String getKgIp() {
		return kgIp;
	}

	/**
	 * @param kgIp
	 *            the kgIp to set
	 */
	public void setKgIp(String kgIp) {
		this.kgIp = kgIp;
	}

	/**
	 * @return the kgPort
	 */
	public String getKgPort() {
		return kgPort;
	}

	/**
	 * @param kgPort
	 *            the kgPort to set
	 */
	public void setKgPort(String kgPort) {
		this.kgPort = kgPort;
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
	 * @return the sessionId
	 */
	public String getSessionId() {
		return sessionId;
	}

	/**
	 * @param sessionId
	 *            the sessionId to set
	 */
	public void setSessionId(String sessionId) {
		this.sessionId = sessionId;
	}

	/**
	 * @return the appUserId
	 */
	public String getAppUserId() {
		return appUserId;
	}

	/**
	 * @param appUserId
	 *            the appUserId to set
	 */
	public void setAppUserId(String appUserId) {
		this.appUserId = appUserId;
	}

	@Override
	public String toString() {
		return super.toString();
	}

}
