/**
 * 
 */
package prod.nebula.vrc.util;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

/**
 * BASE64����빤����
 * 
 * @author �϶���
 * 
 */
public class Base64Util {
	private static BASE64Encoder encoder = new BASE64Encoder();
	private static BASE64Decoder decoder = new BASE64Decoder();

	/**
	 * ����
	 * 
	 * @param s
	 * @return
	 */
	public static String getBASE64(String s) {
		String st = null;
		if (s != null) {
			try {
				st = encoder.encode(s.getBytes());
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return st == null ? "" : st;
	}

	/**
	 * ����
	 * 
	 * @param s
	 * @return
	 */
	public static String getFromBASE64(String s) {
		byte[] b = null;
		if (s != null) {
			try {
				b = decoder.decodeBuffer(s);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return b == null ? "" : new String(b);
	}
}
