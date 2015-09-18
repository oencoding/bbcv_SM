/**
 * 
 */
package prod.nebula.vrc.module.http;

import java.io.Serializable;

/**
 * ͨ�ñ����ֶζ���
 */
public class Constants implements Serializable {
	private static final long serialVersionUID = 2493690826973813015L;
	/** ģ������ http */
	public final static String HTTP_MODULE_NAME = "http";

	public final static String HTTP_SERVER_PORT = "http.server.port";
	public final static String HTTP_MIN_THREADS = "http.thread.min";
	public final static String HTTP_MAX_THREADS = "http.thread.max";
	public final static String HTTP_MAX_IDLETIMEMS = "http.idletimes.max";

	public final static String PLAY = "/play";

	public final static String RESUME = "/resume";
	
	public final static String PAUSE = "/pause";
	
	public final static String FORWARD = "/forward";
	
	public final static String BACKWARD = "/backward";
	
	public final static String CHOOSETIME = "/choose";
	
	public final static String GETTIME = "/gettime";

	public final static String OK = "OK";
	public final static String FAILED = "FAILED!";

	/** �ڲ������쳣 */
	public static final int INTERNAL_ERROR = -1001;
	public static final String INTERNAL_ERROR_MSG = "INTERNAL_ERROR";

	/** �ỰIDΪ�� */
	public static final int SESSIONIDEMPTY = -1002;
	public static final String SESSIONIDEMPTY_MSG = "SESSIONIDEMPTY";
}
