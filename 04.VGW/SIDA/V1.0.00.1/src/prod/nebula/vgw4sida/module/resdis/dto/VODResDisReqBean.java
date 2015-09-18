package prod.nebula.vgw4sida.module.resdis.dto;

/**
 * VOD资源调度请求封装类
 * 
 * @author 严东军
 * 
 */
public class VODResDisReqBean {

	/**
	 * VOD资源调度-请求头信息
	 */
	private String head;

	/**
	 * VOD资源调度-VOD播放接入请求指令字
	 */
	private String reqHead;

	/**
	 * VOD资源调度-运营商用户编号，如STB_ID
	 */
	private String userId;

	/**
	 * VOD资源调度-区域标识
	 */
	private String regionId;

	/**
	 * VOD资源调度-点播的RTSP地址
	 */
	private String rtspAddr;

	/**
	 * 节目编号
	 */
	private String serviceId;

	/**
	 * 通信模块IP
	 */
	private String kgIp;

	/**
	 * 通信模块对应端口
	 */
	private String kgPort;

	/**
	 * VOD资源调度-流水号
	 */
	private String serialNo;

	/**
	 * 交易时间（yyyyMMddHHmmss）
	 */
	private String doneDate;

	/**
	 * 消息类型
	 */
	private String cmd;

	private String sessionId;

	private String appUserId;

	private String spId;
	
	private String totalTime;
	private String purchaseToken;
	
	private String ipqamIP;
	private String ipqamPort;
	
	private String vodname;
	private String posterurl;
	
	
	

	/**
	 * VOD资源调度-请求尾信息
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
	
	

	public String getTotalTime() {
		return totalTime;
	}

	public void setTotalTime(String totalTime) {
		this.totalTime = totalTime;
	}

	
	
	public String getPurchaseToken() {
		return purchaseToken;
	}

	public void setPurchaseToken(String purchaseToken) {
		this.purchaseToken = purchaseToken;
	}
	
	
	

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

	
	
	public String getVodname() {
		return vodname;
	}

	public void setVodname(String vodname) {
		this.vodname = vodname;
	}

	public String getPosterurl() {
		return posterurl;
	}

	public void setPosterurl(String posterurl) {
		this.posterurl = posterurl;
	}

	@Override
	public String toString() {
		return super.toString();
	}

}
