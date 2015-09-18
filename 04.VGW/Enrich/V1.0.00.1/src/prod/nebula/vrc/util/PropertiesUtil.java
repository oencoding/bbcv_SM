package prod.nebula.vrc.util;

//import java.io.File;
//import java.io.FileInputStream;
//import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.service.TCPServer;

public class PropertiesUtil {
	public final Logger logger = LoggerFactory.getLogger(getClass());

	/**
	 * ��Ӧ�������ļ����ļ��������
	 */
	// private File file = null;

	/**
	 * �����ļ�������޸�����
	 */
	// private long lastModifiedTime = 0;

	/**
	 * �����ļ�����Ӧ�����Զ������
	 */
	private Properties properties = null;

	/**
	 * �ļ��ָ��
	 */
	private static String fileSeperator = "/";

	/**
	 * ������ܴ��ڵ�Ωһ��һ��ʵ��
	 */
	private static PropertiesUtil instance = new PropertiesUtil();

	/**
	 * ˽�еĹ����ӣ����Ա�֤����޷�ֱ��ʵ����
	 */
	private PropertiesUtil() {
	}

	/**
	 * ��̬��������
	 * 
	 * @return ����PropertiesUtil ��ĵ�һʵ��
	 */
	synchronized public static PropertiesUtil getInstance() {
		if (null == instance) {
			return instance = new PropertiesUtil();
		}

		return instance;
	}

	/**
	 * ��ȡclass���ڹ���Ŀ¼�еĵ�ַ
	 * 
	 * @return
	 */
	public String getWebClassesPath() {
		String path = getClass().getProtectionDomain().getCodeSource()
				.getLocation().getPath();
		return path;

	}

	/**
	 * ��ȡ����Ŀ¼��classesĿ¼�ĵ�ַ
	 * 
	 * @return
	 * @throws IllegalAccessException
	 */
	public String getClassesPath() throws IllegalAccessException {
		String path = getWebClassesPath();
		if (path.indexOf("classes") > 0) {
			path = path.substring(0, path.indexOf("classes") + 8);
		} else {
			throw new IllegalAccessException(
					"[ReadConfigation]exception:getClassesPath error!");
		}
		return path;
	}

	/**
	 * ��ȡ�����ļ���Ϣ
	 * 
	 * @param moduleName
	 *            �ļ���ַ ��ʽ moduleName
	 * @return
	 * @throws IllegalAccessException
	 */
	public Properties getPropertiesWithPath(String path)
			throws IllegalAccessException {
		properties = new Properties();

		try {
			String configFilePath;
			StringBuffer sb = new StringBuffer("");
			configFilePath = System.getenv("VRC_CONFIG_FILE");
			if (configFilePath != null && !configFilePath.equals("")) {
				sb.append(configFilePath);
				properties = Commons.loadFile(sb.toString());
				return properties;
			}
			configFilePath = System.getProperty("VRC_CONFIG_FILE");
			if (configFilePath != null && !configFilePath.equals("")) {
				sb.append(configFilePath);
				properties = Commons.loadFile(sb.toString());
				return properties;
			}
			properties = loadConfigFile(path);
		} catch (Exception e) {
			logger.error("[PropertiesUtil]exception:", e.getMessage());
			throw new IllegalAccessException("[PropertiesUtil]exception:"
					+ e.getMessage());
		}

		return properties;
	}

	/**
	 * ��ȡ�����ļ���Ϣ
	 * 
	 * @param moduleName
	 *            �ļ���ַ ��ʽ moduleName
	 * @return
	 * @throws IllegalAccessException
	 */
	public Properties getProperties(String moduleName)
			throws IllegalAccessException {
		StringBuffer sb = new StringBuffer("");
		// sb.append(getClassesPath());

		// Ŀ¼�Թ��̴���֮�󲻿��޸�
		// ���� resource/module/modulename/modulename.properties
		sb.append("module");
		sb.append(fileSeperator);
		sb.append(moduleName);
		sb.append(fileSeperator);
		sb.append(moduleName);
		sb.append(".properties");

		properties = new Properties();

		try {
			properties = loadConfigFile(sb.toString());
		} catch (Exception e) {
			logger.error("[PropertiesUtil]exception:", e.getMessage());
			throw new IllegalAccessException("[PropertiesUtil]exception:"
					+ e.getMessage());
		}

		return properties;
	}

	public StringBuffer getModelXmlPath(String moduleName)
			throws IllegalAccessException {
		StringBuffer sb = new StringBuffer("");
		StringBuffer retSb = new StringBuffer("");
		sb.append("module");
		sb.append(fileSeperator);
		sb.append(moduleName);
		sb.append(fileSeperator);
		sb.append(moduleName);
		sb.append(".xml");

		int source = 0;
		source = TCPServer.APP;
		logger.debug("loadfile from " + source);
		return retSb.append(this.getClass().getClassLoader()
				.getResource(sb.toString()).getFile());
	}

	private Properties loadConfigFile(String path) throws IOException {
		int source = 0;
		source = TCPServer.APP;
		logger.debug("loadfile from " + source);
		return Commons.loadPropertiesFile(path);
	}
}
