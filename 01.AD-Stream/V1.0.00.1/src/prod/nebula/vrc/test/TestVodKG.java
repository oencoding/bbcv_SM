/**
 * 
 */
package prod.nebula.vrc.test;

import prod.nebula.vrc.config.VODConst;
import prod.nebula.vrc.util.client.TcpClient;

/**
 * @author �϶���
 *
 */
public class TestVodKG {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		TcpClient client = new TcpClient();
		String msg = client.sendStr("10.0.138.19", 8572, 1000, null, getStrToKGUPDATE());
		System.out.println(msg);
	}
	
	/**
	 * ���ĸ�ʽVEDIO_ROUTE_UPDATE_REQ|app_user_id|vod_ip|vod_port
	 * @param vodPort
	 * @return
	 */
	public static String getStrToKGUPDATE(){
		StringBuffer sendStr = new StringBuffer("");
		sendStr.append("XXBB");
		sendStr.append(VODConst.VEDIO_ROUTE_UPDATE_REQ);
		sendStr.append(VODConst.SEPARATOR);
		sendStr.append(10);
		sendStr.append(VODConst.SEPARATOR);
		sendStr.append("10.0.114.199");
		sendStr.append(VODConst.SEPARATOR);
		sendStr.append(9000);
		sendStr.append("XXEE");
		return sendStr.toString();
	}
	
	/**
	 * ���ĸ�ʽVEDIO_ROUTE_UPDATE_REQ|app_user_id|vod_ip|vod_port
	 * @param vodPort
	 * @return
	 */
	public static String getStrToKGFREE(){
		StringBuffer sendStr = new StringBuffer("");
		sendStr.append("XXBB");
		sendStr.append(VODConst.VEDIO_ROUTE_FREE_REQ);
		sendStr.append(VODConst.SEPARATOR);
		sendStr.append(10);
		sendStr.append(VODConst.SEPARATOR);
		sendStr.append("10.0.114.199");
		sendStr.append(VODConst.SEPARATOR);
		sendStr.append(9000);
		sendStr.append("XXEE");
		return sendStr.toString();
	}

}
