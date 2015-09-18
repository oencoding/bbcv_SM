package prod.nebula.vrc.util.client;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class IOSocket{
  private Socket socket;
  private InputStream input;
  private OutputStream output;
  private String ip;
  private int port;
  private int timeout = 2000;  //ȱʡ��ʱʱ��Ϊ1��
  
  public IOSocket(){
    socket = new Socket();
  }
  
  public IOSocket(String ip, int port,int timeoutseconds) throws IOException {
		this.timeout = timeoutseconds;
		this.ip = ip;
		this.port = port;

		socket = new Socket();
		SocketAddress add = new InetSocketAddress(ip, port);
		try {
			socket.connect(add, 250);
		} catch (Exception e) {
			throw new ConnectException(e.getMessage());
		}
		socket.setSoTimeout(timeout);
		socket.setReuseAddress(false);
  }
  
  /**********************
   * ��������
   */
  public void connect() throws IOException{
    socket.connect(new InetSocketAddress(ip, port), timeout);
  }
  /**********************
   * ��������
   */
  public void connect(String ip, int port) throws IOException{
    socket.connect(new InetSocketAddress(ip, port), timeout);
  }
  /**********************
   * ��������
   */
  public void connect(String ip, int port, int timeout) throws IOException{
    socket.connect(new InetSocketAddress(ip, port), timeout);
  }
  
  /****************
   * ��������������
   * @param sendBuffer
   * @param receiveBuffer
   * @return -1 ����ʧ��
   *         >=0 ���յĳ���
   *         ����ʧ�����쳣
   * @throws IOException 
   ***************/
  public int sendAndreceive(byte[] sendBuffer, byte[] receiveBuffer) throws IOException{
    send(sendBuffer);
    return receive(receiveBuffer);
  }
  
  private void send(byte[] sendBuffer) throws IOException{
    output = socket.getOutputStream();
    output.write(sendBuffer);
  }
  
  private int receive(byte[] receiveBuffer) throws IOException{
	input = socket.getInputStream();
    DataInputStream dis = new DataInputStream(input);
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    int len = -1;
      
    while ((len = dis.read(receiveBuffer))!=-1) {     
    	baos.write(receiveBuffer, 0, len);  
    	if((new String(receiveBuffer)).trim().lastIndexOf("XXEE")>=0)
  		  break;
    } 
    return len;
  }
  
  /**********************
  * �ر�����
  */
  public void close() throws IOException{
    socket.close();
  }
  
  /**********************
   * �Ƿ��Ѿ��ر�����
   */
  public boolean isClose(){
  	if (socket == null) return false;
  	return socket.isClosed();
  }
}