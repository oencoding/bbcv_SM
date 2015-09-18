package prod.nebula.vrc.util;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Properties;
import java.util.UUID;

import org.apache.mina.core.buffer.IoBuffer;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * ����������
 * 
 * @author �϶���
 * 
 */
public class Commons {
	public enum Status {
		init, options, describe, setup, play, pause, scale, teardown,move,choosetime,resume
	}

	private static final Logger logger = LoggerFactory.getLogger(Commons.class);
	
	public static String getSerialNo(){
		UUID uuid = UUID.randomUUID();
		return uuid.toString().replaceAll("-", ""); 
	}

	/**
	 * ��ȡ��Ҫ�������ļ���ת����Properties���󡣸÷���ʹ��Commons����������������ļ���
	 * 
	 * @param file
	 *            ��ȡ�������ļ�����
	 * @return �����ļ�����
	 * @throws IOException
	 *             ��ȡ�����ļ�ʱ������IO�쳣
	 */
	public static Properties loadPropertiesFile(String file) throws IOException {
		logger.info("��VOD���ء���ʼ���� " + file);
		InputStream in = Commons.class.getClassLoader().getResourceAsStream(
				file);
		Properties p = new Properties();
		p.load(in);
		logger.info("��VOD���ء�����  " + file + " ���");
		return p;
	}

	public static Properties loadFile(String file) throws IOException {
		try {
			FileInputStream fin = new FileInputStream(file);
			Properties p = new Properties();
			p.load(fin);
			return p;
		} catch (FileNotFoundException e) {
			return null;
		}

	}

	public static int getIntPropertyValue(Properties p, String name, int def) {
		String value = Commons.getStringPropertyValue(p, name);
		return Commons.isNullorEmptyString(value) ? def : Integer
				.parseInt(value);
	}

	public static String getStringPropertyValue(Properties p, String name) {
		return Commons.getStringPropertyValue(p, name, "");
	}

	public static String getStringPropertyValue(Properties p, String name,
			String def) {
		String value = p.getProperty(name);
		if (Commons.isNullorEmptyString(value)) {
			return Commons.isNullorEmptyString(def) ? "" : def;
		}
		return value;
	}

	/**
	 * ������Ϣ�͹�ϵ����","Ϊ������ַ�����
	 * 
	 * @param str
	 * @return
	 */
	public static String[] splitString(String str) {
		return splitString(",", 0);
	}

	public static String[] splitString(String str, int limit) {
		return str.split(",", limit);
	}

	/**
	 * ���Ϊ�շ���TRUE
	 * 
	 * @param str
	 * @return
	 */
	public static boolean isNullorEmptyString(String str) {
		return (str == null || str.equals("")) ? true : false;
	}

	public static String getThisDayStr() {
		Calendar cal = Calendar.getInstance();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMddHH");
		return sdf.format(cal.getTime());
	}

	/**
	 * ת�����յ�����ΪGBK����
	 * 
	 * @param message
	 * @return
	 * @throws UnsupportedEncodingException
	 */
	public static String getGBKCharactor(Object message)
			throws UnsupportedEncodingException {
		if (message instanceof IoBuffer) {
			IoBuffer buffer = (IoBuffer) message;
			byte[] bb = buffer.array();
			String revStr = new String(bb, "GBK");
			return revStr;
		}

		return "";
	}

	/**
	 * ���طǿ��ַ���
	 * 
	 * @param str
	 * @return String ����һ���ǿյ��ַ���
	 */
	public static String getStrValue(String str) {
		return (null == str ? "" : str);
	}
}
