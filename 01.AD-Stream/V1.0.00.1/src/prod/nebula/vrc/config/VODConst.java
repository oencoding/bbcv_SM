/**
 * 
 */
package prod.nebula.vrc.config;

/**
 * VOD���ƴ��������
 * 
 * @author �϶���
 * 
 */
public class VODConst {

	/**
	 * VOD��Դ����--����VOD��������ͷ��Ϣ
	 */
	public static final String VOD_CTRL_VOD_PLAY_REQ = "VOD_CTRL_VOD_PLAY_REQ";

	/**
	 * VOD��Դ����--����VOD������Ӧͷ��Ϣ
	 */
	public static final String VOD_CTRL_VOD_PLAY_RESP = "VOD_CTRL_VOD_PLAY_RESP";

	/**
	 * VOD��Դ����--VOD���ſ�������ͷ��Ϣ
	 */
	public static final String VOD_CTRL_VOD_PLAY_CTRL_REQ = "VOD_CTRL_VOD_PLAY_CTRL_REQ";

	/**
	 * VOD��Դ����--VOD���ſ�����Ӧͷ��Ϣ
	 */
	public static final String VOD_CTRL_VOD_PLAY_CTRL_RESP = "VOD_CTRL_VOD_PLAY_CTRL_RESP";

	/**
	 * VOD��Դ����--���Ž����������ͷ��Ϣ
	 */
	public static final String VOD_CTRL_VOD_RESULT_GET_REQ = "VOD_CTRL_VOD_RESULT_GET_REQ";

	/**
	 * VOD�˳�����ͷ
	 */
	public static final String BUSI_CTRL_EXIT_VOD_REQ = "BUSI_CTRL_EXIT_VOD_REQ";

	/**
	 * ҵ�����VOD�˳�����ͷ��Ϣ
	 */
	public static final String VOD_CA_VOD_QUIT_REQ = "VOD_CA_VOD_QUIT_REQ";

	public static final String APP_ENV_CTL_KEY_ACC_REQ = "APP_ENV_CTL_KEY_ACC_REQ";

	/**
	 * ҵ�����VOD�˳���Ӧͷ��Ϣ
	 */
	public static final String VOD_CA_VOD_PLAY_RESP = "VOD_CA_VOD_PLAY_RESP";

	/**
	 * ҵ���������VOD�˳�-ָ��
	 */
	public static final String CSCG_COMMAND = "quit";

	/**
	 * VOD�˳��ɹ���
	 */
	public static final int VOD_QUITE_OK = 0;

	/**
	 * VOD�˳�ʧ����
	 */
	public static final int VOD_QUITE_FAIL = -1;

	/**
	 * ֪ͨ������Ƶ���ŵ�ַ�ӿ�����ͷ
	 */
	public static final String VEDIO_ROUTE_UPDATE_REQ = "VEDIO_ROUTE_UPDATE_REQ";

	/**
	 * ֪ͨ������Ƶ���ŵ�ַ�ӿ���Ӧͷ
	 */
	public static final String VEDIO_ROUTE_UPDATE_RESP = "VEDIO_ROUTE_UPDATE_RESP";

	/**
	 * ֪ͨ�Ƴ���Ƶ���ŵ�ַ�ӿ�����ͷ
	 */
	public static final String VEDIO_ROUTE_FREE_REQ = "VEDIO_ROUTE_FREE_REQ";

	/**
	 * ֪ͨ�Ƴ���Ƶ���ŵ�ַ�ӿ���Ӧͷ
	 */
	public static final String VEDIO_ROUTE_FREE_RESP = "VEDIO_ROUTE_UPDATE_RESP";

	/**
	 * �ָ��
	 */
	public static final String SEPARATOR = "|";

	/**
	 * VOD��Դ����--���سɹ���
	 */
	public static final int RETURNCODE_OK = 0;

	/**
	 * VOD��Դ����-���ش�����
	 */
	public static final int RETURNCODE_ERROR = -1;

	/****************** HID��׼��ֵת����10������Ϣ *********************/
	/**
	 * ����
	 */
	public static final String KEYVALUE_PLAY = "40";

	/**
	 * ��ͣ���������
	 */
	public static final String KEYVALUE_RESUME = "43";

	/**
	 * ��ͣ
	 */
	public static final String KEYVALUE_PAUSE = "pause";

	/**
	 * ����
	 */
	public static final String KEYVALUE_BACK = "158";

	/**
	 * �˳�/ֹͣ
	 */
	public static final String KEYVALUE_EXIT = "41";

	/**
	 * ���
	 */
	public static final String KEYVALUE_FAST = "8";

	/**
	 * ����
	 */
	public static final String KEYVALUE_SLOW = "4";

	/**
	 * ѡʱ��
	 */
	public static final String KEYVALUE_CHOOSETIME = "80";

	/**
	 *������
	 */
	public static final int KEYVALUE_LEFT = 0x0b;
	
	/**
	 * ������
	 */
	public static final int KEYVALUE_RIGHT = 0x06;

	/****************** HID��׼��ֵת����10������Ϣ *********************/

	/**
	 * VOD���Ž���󷵻ظ�ҵ����ȵĲ��Ž��--���ųɹ�
	 */
	public static final int PLAYTYPE_OK = 0;

	/**
	 * VOD���Ž���󷵻ظ�ҵ����ȵĲ��Ž��--����ʧ��
	 */
	public static final int PLAYTYPE_FAIL = -1;

	/**
	 * VOD��Դ����
	 */
	public static final int RESTYPE_VOD = 3;

	/**
	 * VOD�˳�����
	 */
	public static final String VOD_QUITE_COMMOND = "exit vod";

	/**
	 * ����������
	 */
	public static final int STBTYPE = 1;

	/**
	 * ���������
	 */
	public static final int SCALE_FAST_TIMES = 8;

	/**
	 * �����˱���
	 */
	public static final int SCALE_SLOW_TIMES = -8;

	// ----------------�쳣�붨�� start----------------
	// ����
	public static final int VOD_OK = 0;

	// �ڲ�����
	public static final int INTERNAL_ERROR = -2201;

	// ��������쳣
	public static final int REQMESSAGE_ERROR = -2202;

	// VODͨ��ģ�齻���쳣
	public static final int VODC_ERROR = -2203;

	// VOD�����������쳣
	public static final int PLAYVOD_ERROR = -2204;

	// ----------------�쳣�붨�� end ----------------

}
