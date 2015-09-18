/**
 * 
 */
package prod.nebula.vrc.module.resctrl.dto;

import prod.nebula.vrc.config.VODConst;

/**
 * VOD��Դ���������װ��
 * @author �϶���
 *
 */
public class VODResCtrlReqBean {
	/**
	 * VOD��Դ����-����ͷ��Ϣ
	 */
	private String head;
	
	/**
	 * VOD��Դ����-��������
	 */
	private String reqHead;
	
	/**
	 * VOD��Դ����-��ֵ��Ϣ
	 */
	private String keyValue;
	
	/**
	 * VOD��Դ����-����β��Ϣ
	 */
	private String end;
	
	public VODResCtrlReqBean(){
		
	}

	/**
	 * @return the head
	 */
	public String getHead() {
		return head;
	}

	/**
	 * @param head the head to set
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
	 * @param reqHead the reqHead to set
	 */
	public void setReqHead(String reqHead) {
		this.reqHead = reqHead;
	}

	/**
	 * @return the keyValue
	 */
	public String getKeyValue() {
		return keyValue;
	}

	/**
	 * @param keyValue the keyValue to set
	 */
	public void setKeyValue(String keyValue) {
		this.keyValue = keyValue;
	}

	/**
	 * @return the end
	 */
	public String getEnd() {
		return end;
	}

	/**
	 * @param end the end to set
	 */
	public void setEnd(String end) {
		this.end = end;
	}

	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer("");
		sb.append(this.getHead());
		sb.append(VODConst.SEPARATOR);
		sb.append(this.getReqHead());
		sb.append(VODConst.SEPARATOR);
		sb.append(this.getKeyValue());
		sb.append(this.getEnd());
		return sb.toString();
	}
	
}
