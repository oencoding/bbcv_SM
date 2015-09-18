/** 
 * Project: msi-service
 * author : PengSong
 * File Created at 2013-12-20 
 *
 * Copyright bbcvision Corporation Limited. 
 * All rights reserved. 
 *
 * This software is the confidential and proprietary information of 
 * bbcvision company. ("Confidential Information"). You shall not 
 * disclose such Confidential Information and shall use it only in 
 * accordance with the terms of the license agreement you entered into 
 * with bbcvision.com. 
 */
package prod.nebula.service.socket.mgw;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.URLEncoder;

import javax.annotation.Resource;

import net.sf.json.JSONObject;

import org.apache.log4j.Logger;
import org.springframework.stereotype.Service;

import prod.nebula.commons.config.UcmsConfig;
import prod.nebula.exception.msi.BbcvMsiException;
import prod.nebula.mdb.MdbRedis;
import prod.nebula.service.socket.center.CenterServiceImpl;
import prod.nebula.service.socket.center.StringUtil;
import prod.nebula.service.socket.center.VodKeyCtrlReq;

import com.bbcv.mdb.redis.impl.MDBDataImpl;

/** 
 * TODO Comment of MgwKeysControlServiceImpl 
 * 
 * @author PengSong 
 */
@Service("mgwKeyControlService")
public class MgwKeyControlServiceImpl {

	private static final Logger logger=Logger.getLogger(MgwKeyControlServiceImpl.class);
	@Resource(name="mdbredis",type=MdbRedis.class)
	private MdbRedis mdbredis;
	
	@Resource(name="centerService",type=CenterServiceImpl.class)
	private CenterServiceImpl centerService;
	
	
	public void keyControl(String username,String key_value, String status,String stream_id,String nickname) throws BbcvMsiException, IOException{
		
		logger.info("收到APP键值：========="+key_value);
		int key_val_num = Integer.parseInt(key_value.substring(2), 16);
		logger.info("键值截取：========="+key_val_num);

		switch (key_val_num) {//键值映射
		case 0: // 上
			key_val_num = Integer.parseInt(("0x52").substring(2), 16);
			break;
		case 1: // 下
			key_val_num = Integer.parseInt(("0x51").substring(2), 16);
			break;
		case 3: // 左
			key_val_num = Integer.parseInt(("0x50").substring(2), 16);
			break;
		case 2: // 右
			key_val_num = Integer.parseInt(("0x4f").substring(2), 16);
			break;

		case 17: // 1
			key_val_num = Integer.parseInt(("0x1e").substring(2), 16);
			break;
		case 18: // 2
			key_val_num = Integer.parseInt(("0x1f").substring(2), 16);
			break;
		case 19: // 3
			key_val_num = Integer.parseInt(("0x20").substring(2), 16);
			break;
		case 20: // 4
			key_val_num = Integer.parseInt(("0x21").substring(2), 16);
			break;

		case 29: // 退出
			key_val_num = Integer.parseInt("158");
			break;
		case 31: // 确定
			key_val_num = Integer.parseInt(("0x28").substring(2), 16);
			break;

		default:
		}
		
		String vgw_ip="";
		String vgw_port="";
		MDBDataImpl mdbDateImpl = (MDBDataImpl) mdbredis.getMdbImpl();
		if(status=="2"||"2".equals(status)){
			vgw_ip = mdbDateImpl.getString("CLOUD_KEY_SEND_IP_"+username);
			vgw_port = mdbDateImpl.getString("CLOUD_KEY_SEND_PORT_"+username);
			VodKeyCtrlReq keyCtrlDown = new VodKeyCtrlReq(12312, key_val_num, 2);
			byte[] keyCtrlDownSend = vodKeyConvertToByteArray(keyCtrlDown);
			vodKeyCtrl(keyCtrlDownSend, keyCtrlDownSend.length, vgw_ip,
					Integer.valueOf(vgw_port));
			try {
				Thread.sleep(20);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			VodKeyCtrlReq keyCtrlUp = new VodKeyCtrlReq(12312, key_val_num, 3);
			byte[] keyCtrlUpSend = vodKeyConvertToByteArray(keyCtrlUp);
			vodKeyCtrl(keyCtrlUpSend, keyCtrlUpSend.length, vgw_ip,
					Integer.valueOf(vgw_port));
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}else{
			String key_addr = mdbDateImpl.getString(UcmsConfig.VGW_KEY_SEND+"_"+username);
			String[] key_addrs = key_addr.split(":");
			vgw_ip = key_addrs[0];
			vgw_port = key_addrs[1];
			VodKeyCtrlReq keyCtrlDown = new VodKeyCtrlReq(12312, key_val_num, 2);
			byte[] keyCtrlDownSend = vodKeyConvertToByteArray(keyCtrlDown);
			vodKeyCtrl(keyCtrlDownSend, keyCtrlDownSend.length, vgw_ip,
					Integer.valueOf(vgw_port));
			try {
				Thread.sleep(20);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			VodKeyCtrlReq keyCtrlUp = new VodKeyCtrlReq(12312, key_val_num, 3);
			byte[] keyCtrlUpSend = vodKeyConvertToByteArray(keyCtrlUp);
			vodKeyCtrl(keyCtrlUpSend, keyCtrlUpSend.length, vgw_ip,
					Integer.valueOf(vgw_port));
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
//			String key_addr = mdbDateImpl.getString(UcmsConfig.VGW_KEY_SEND+"_"+username);
//			String[] key_addrs = key_addr.split(":");
//			vgw_ip = key_addrs[0];
//			vgw_port = key_addrs[1];
//			if(mdbDateImpl.getString("LOGOUTVOD_"+username)=="true"||"true".equals(mdbDateImpl.getString("LOGOUTVOD_"+username))){
//				if(key_val_num==158){//退出键
//					//首次点击“退出”键，向MS发出退出请求报文，流化退出确认页面；
//					JSONObject quitObject = new JSONObject();
//					quitObject.put("cmd", "quit");
//					quitObject.put("stream_id", stream_id);
//					String user = (nickname==null||"".equals(nickname))?username:nickname;
//					String usernamegbk = new String(user.getBytes(),"gbk");
//					String usernameutf = URLEncoder.encode(usernamegbk, "utf-8");
//					quitObject.put("username", usernameutf);
//					String quitString = quitObject.toString();
//					String quitRetString = centerService.send(quitString);
//					if(quitRetString!=null&&!"".equals(quitRetString)){
//						quitRetString = quitRetString.replace("XXEE", "");
//						JSONObject retJson = JSONObject.fromObject(quitRetString);
//						if(retJson.getString("ret_code")=="0"||"0".equals(retJson.getString("ret_code")))
//							mdbDateImpl.putString("LOGOUTVOD_"+username, "false");
//					}
//				}else{//其他键
//					VodKeyCtrlReq keyCtrlDown = new VodKeyCtrlReq(12312, key_val_num, 2);
//					byte[] keyCtrlDownSend = vodKeyConvertToByteArray(keyCtrlDown);
//					vodKeyCtrl(keyCtrlDownSend, keyCtrlDownSend.length, vgw_ip,
//							Integer.valueOf(vgw_port));
//					try {
//						Thread.sleep(20);
//					} catch (InterruptedException e) {
//						e.printStackTrace();
//					}
//					VodKeyCtrlReq keyCtrlUp = new VodKeyCtrlReq(12312, key_val_num, 3);
//					byte[] keyCtrlUpSend = vodKeyConvertToByteArray(keyCtrlUp);
//					vodKeyCtrl(keyCtrlUpSend, keyCtrlUpSend.length, vgw_ip,
//							Integer.valueOf(vgw_port));
//					try {
//						Thread.sleep(10);
//					} catch (InterruptedException e) {
//						e.printStackTrace();
//					}
//				}
//			}else{
//				//已经按过一次退出键逻辑
//				if(key_val_num==158){//退出键
//					VodKeyCtrlReq keyCtrlDown = new VodKeyCtrlReq(12312, key_val_num, 2);
//					byte[] keyCtrlDownSend = vodKeyConvertToByteArray(keyCtrlDown);
//					vodKeyCtrl(keyCtrlDownSend, keyCtrlDownSend.length, vgw_ip,
//							Integer.valueOf(vgw_port));
//					try {
//						Thread.sleep(20);
//					} catch (InterruptedException e) {
//						e.printStackTrace();
//					}
//					VodKeyCtrlReq keyCtrlUp = new VodKeyCtrlReq(12312, key_val_num, 3);
//					byte[] keyCtrlUpSend = vodKeyConvertToByteArray(keyCtrlUp);
//					vodKeyCtrl(keyCtrlUpSend, keyCtrlUpSend.length, vgw_ip,
//							Integer.valueOf(vgw_port));
//					try {
//						Thread.sleep(10);
//					} catch (InterruptedException e) {
//						e.printStackTrace();
//					}
//				}else{//其他键
//					//按其他键，向MS发出退出恢复请求报文，TV画面切回VOD视频；
//					JSONObject quitObject = new JSONObject();
//					quitObject.put("cmd", "recovery");
//					quitObject.put("stream_id", stream_id);
//					String user = (nickname==null||"".equals(nickname))?username:nickname;
//					String usernamegbk = new String(user.getBytes(),"gbk");
//					String usernameutf = URLEncoder.encode(usernamegbk, "utf-8");
//					quitObject.put("username", usernameutf);
//					String vodname = mdbDateImpl.getString("VODNAME_"+username);
//					String vodnamegbk = new String(vodname.getBytes(),"gbk");
//					String vodnameutf = URLEncoder.encode(vodnamegbk, "utf-8");
//					quitObject.put("vodname", vodnameutf);
//					quitObject.put("posterurl", mdbDateImpl.getString("POSTERURL_"+username));
//					String recoveryString = quitObject.toString();
//					String recoveryRetString = centerService.send(recoveryString);
//					if(recoveryRetString!=null&&!"".equals(recoveryRetString)){
//						recoveryRetString = recoveryRetString.replace("XXEE", "");
//						JSONObject retJson = JSONObject.fromObject(recoveryRetString);
//						if(retJson.getString("ret_code")=="0"||"0".equals(retJson.getString("ret_code")))
//							mdbDateImpl.putString("LOGOUTVOD_"+username, "true");
//					}
//				}
//				
//			}
		}
	}
	
	
	public static byte[] vodKeyConvertToByteArray(VodKeyCtrlReq req) {
		byte[] bufLogin = new byte[64];

		byte[] temp = StringUtil.toLH(req.getDev_type());
		System.arraycopy(temp, 0, bufLogin, 0, temp.length);

		temp = StringUtil.toLH(req.getSequence_num());
		System.arraycopy(temp, 0, bufLogin, 4, temp.length);

		temp = StringUtil.toLH(req.getKey_value());
		System.arraycopy(temp, 0, bufLogin, 8, temp.length);

		temp = StringUtil.toLH(req.getKey_status());
		System.arraycopy(temp, 0, bufLogin, 12, temp.length);

		return bufLogin;
	}
	
	public void vodKeyCtrl(byte[] sendDataByte, int send_length, String vodIp,
			int vodPort) {

		try {
			logger.info("vod send key to VGW ip= " + vodIp + ",port="
					+ vodPort);
			InetAddress ia = InetAddress.getByName(vodIp);
			DatagramSocket socket = new DatagramSocket(0);
			socket.connect(ia, vodPort);
			byte[] buffer = new byte[1024];
			DatagramPacket dp = new DatagramPacket(sendDataByte,
					sendDataByte.length);

			socket.send(dp);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
