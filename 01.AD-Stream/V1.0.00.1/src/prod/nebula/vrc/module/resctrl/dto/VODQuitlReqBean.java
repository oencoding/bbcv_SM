/**
 * 
 */
package prod.nebula.vrc.module.resctrl.dto;


/**
 * VOD��Դ���� VOD��Դ�ͷ������װ��
 * 
 * @author �϶���
 * 
 */
public class VODQuitlReqBean {

	/**
	 * VOD��Դ����-��������
	 */
	private String command;

	/**
	 * VOD��Դ����-��ˮ��
	 */
	private String serialNo;

	public VODQuitlReqBean() {

	}
	
	public String getCommand() {
		return command;
	}

	public void setCommand(String command) {
		this.command = command;
	}
	
	public String getSerialNo() {
		return serialNo;
	}

	public void setSerialNo(String serialNo) {
		this.serialNo = serialNo;
	}
}
