
package prod.nebula.mcs.module.handle;

import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;



public class CommunicationStandard {
	
	public static final int BUFFER_SIZE = 512;
	public static final String CHARSET_NAME = "UTF-8";
	public static final long SEND_TIME_OUT = 5000l;
	
	public static final long RESPONSE_TIME = 30000l;
	
	
	public static final int HEADER_LENGTH    = 12; //指令头
	public static final byte SYN_HEADER      =0x0c; //同步头
	public static final byte MAJOR_VERSION   = 0x01; //主版本
	public static final byte MINOR_VERSION   = 0x00; //子版本
	public static final byte COMPILE_VERSION = 0x00; //编译版本
	
	public static final String MESSAGE = "message";
	public static final String RESPONSE = "response";
	public static final String ALARM = "alarm";
	
	public static final long HEART_BEAT_INTERVAL = 3000l;
	public static final long RECONNECT_INTERVAL = 5000l;
    
    //初始化包头
	public byte[] initPacketHeader(String strXml) throws UnsupportedEncodingException
	{
		int xmlBytesLen = strXml.getBytes(CommunicationStandard.CHARSET_NAME).length;
		byte[] header = new byte[HEADER_LENGTH];
		header[0] = SYN_HEADER;
		header[1] = MAJOR_VERSION;
		header[2] = MINOR_VERSION;
		header[3] = COMPILE_VERSION;
		header[4] = (byte)((xmlBytesLen>>24)&0xff);
		header[5] = (byte)((xmlBytesLen>>16)&0xff);
		header[6] = (byte)((xmlBytesLen>>8)&0xff);
		header[7] = (byte)((xmlBytesLen)&0xff);		
		long sum =0;
		for (int i = 0; i < 8; i++) {
			long h = (header[i]&0xff);
			sum += h;
		}
//		System.out.println("h = (header[i]&0xff):"+sum);
//		long summ = parsePacketHeader(header);
		header[8] = (byte)((sum>>24)&0xff);
		header[9] = (byte)((sum>>16)&0xff);
		header[10] = (byte)((sum>>8)&0xff);
		header[11] = (byte)(sum&0xff);		
		return header;
	}
	
	//包头校验和
    public Integer parsePacketHeader(byte[] header)
    {
        if(header == null || header.length < HEADER_LENGTH){
            return 0;
        }
        int[] headerBuf = new int[HEADER_LENGTH];
        //版本号的判断
        if( header[1] != MAJOR_VERSION
                || header[2] != MINOR_VERSION
                || header[3] != COMPILE_VERSION)
        {
            return 0;
        }
        for (int i = 0; i < 4; i++) {
			headerBuf[i] = (header[i]&0xff);
		}
        headerBuf[4] = (header[4]&0xff)<<24;
        headerBuf[5] = (header[5]&0xff)<<16;
        headerBuf[6] = (header[6]&0xff)<<8;
        headerBuf[7] = header[7]&0xff;
        //指令的长度
        Integer nCmdLen = headerBuf[4] + headerBuf[5]+ headerBuf[6] + headerBuf[7];
        long checkSum = 0;
        for(int i = 0; i < 8; ++i){
            checkSum += (header[i]&0xff);
        }
        long getSum = 0;
//        headerBuf[8] = (header[8]&0xff) << 24;
//        headerBuf[9] = (header[9]&0xff) << 16;
//        headerBuf[10] = (header[10]&0xff) << 8;
//        headerBuf[11] = (header[11]&0xff);
//        getSum =headerBuf[8] + headerBuf[9] + headerBuf[10] + headerBuf[11];
        getSum=headerBuf[8];
        getSum<<=8;
        getSum+=headerBuf[9];
        getSum<<=8;
        getSum+=headerBuf[10];
        getSum<<=8;
        getSum+=headerBuf[11];
        //判断校验和是否正确
//        if(checkSum != getSum){
//            return 0;
//        }
        return nCmdLen;        
    }
    

    
    public StringBuilder bytes2HexString(byte[] b) {
    	  StringBuilder ret = new StringBuilder();
    	  for (int i = 0; i < b.length; i++) {
    	   String hex = Integer.toHexString(b[ i ] & 0xFF);
    	   if (hex.length() == 1) {
    		   ret.append('0');
    	   }
    	   ret.append( hex.toUpperCase());
    	   ret.append( " ");
    	  }
    	  return ret;
    }
    //<message id="9223372036854775807" length = 33	
	public static Long getMessageId(byte[] b,String msgType){
		try {
				String s = new String(b, 0,b.length>40? 40:b.length,CHARSET_NAME);
				if (msgType==null||s.contains(msgType)){
				Pattern p = Pattern.compile("(?<=\"|')[\\d]+(?=\"|')");
					Matcher m = p.matcher(s);
					if (m.find()){
						return Long.parseLong(m.group(0));
					}
				}
		} catch (Exception e) {
		} 
		return null;
	}
	
	public static String getMessageType(byte[] b){
		try {
			String s = new String(b, 0,b.length>40? 40:b.length,CHARSET_NAME);
			Pattern p = Pattern.compile("(?<=<)[\\S]+(?=\\s*|\\t|\\r|\\n)");
			Matcher m = p.matcher(s);
			if (m.find()){
				return m.group(0);
			}
		} catch (Exception e) {
		} 
		return null;
	}
	

	
	public static String getText(String s){
		try {
			int begin = s.indexOf(">");
			int end = s.lastIndexOf("<");
			return new String(s.getBytes("utf-8"),begin+1,end-begin-1,CHARSET_NAME);
		} catch (Exception e) {
		}
	    return null;
	}
	

	public static void main(String[] args) throws Exception{
		InputStream in = Thread.currentThread().getContextClassLoader().getResourceAsStream("cfg.xml");
		byte[] b = new byte[in.available()];
		in.read(b);
		long time = System.currentTimeMillis();
		CommunicationStandard ccs = new CommunicationStandard();
		String ack = ccs.getResponseAck(new String(b, "utf-8"));
		System.out.println(ack);
		String cfg = getCfg(new String(b,"utf-8"));
		System.out.println(cfg);
		System.out.println("用时:"+(System.currentTimeMillis()-time));
	}
	
	public static String getResponseAck(String ack){
		try {
			byte[] b = ack.getBytes(CHARSET_NAME);
			String s = new String(b, 0,b.length>100? b.length:100,CHARSET_NAME);
			if (s.indexOf("<response")>=0){
			Pattern p = Pattern.compile("(?<=ack=[\"|'])[\\d]+(?=\"|')");
			Matcher m = p.matcher(s);
			if (m.find()){
				return m.group(0);
			}
			}
		} catch (Exception e) {
		} 
		return "-1";
	}
	
	public  String getByteResponseAck(byte[] ack){
		try {
			CommunicationStandard ccs = new CommunicationStandard();
			String s = new String(ack, 0,ack.length>100? ack.length:100,CHARSET_NAME);
			System.out.println(s);
			if (s.indexOf("<response")>=0){
			Pattern p = Pattern.compile("(?<=ack=[\"|'])[\\d]+(?=\"|')");
			Matcher m = p.matcher(s);
			if (m.find()){
				return m.group(0);
			}
			}
		} catch (Exception e) {
		} 
		return "-1";
	}
	
	public static String getCfg(String cfg){
		int first  = cfg.indexOf("<body>");
		int last = cfg.lastIndexOf("</body>");
		if (first>0&&last>0){
			return cfg.substring(first+6, last);
		}
		return null;
	}
}
