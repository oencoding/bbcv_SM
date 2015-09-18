package prod.nebula.vrc.service;

import java.net.InetSocketAddress;
import java.nio.charset.Charset;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.buffer.SimpleBufferAllocator;
import org.apache.mina.core.future.ConnectFuture;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.executor.ExecutorFilter;
import org.apache.mina.transport.socket.SocketConnector;
import org.apache.mina.transport.socket.nio.NioSocketConnector;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.handler.TCPHandler;

public class TCPClient extends IoHandlerAdapter {
	public static final Logger logger = LoggerFactory
			.getLogger(TCPClient.class);

	private static IoSession session;

	public TCPClient() {

	}

	public TCPClient(String host, int port) {
		logger.debug("Starting Mina TCPClient v2.0...");
		IoBuffer.setUseDirectBuffer(false);
		IoBuffer.setAllocator(new SimpleBufferAllocator());

		SocketConnector connector = new NioSocketConnector();
		connector.getSessionConfig().setTcpNoDelay(true);
		connector.getSessionConfig().setKeepAlive(true);
		connector.getSessionConfig().setReuseAddress(true);

		/* �����̳߳� */
		Executor threadPool = Executors.newCachedThreadPool();
		connector.getFilterChain().addLast("exector",
				new ExecutorFilter(threadPool));
		
		connector.setHandler(new TCPHandler());
		try {
			InetSocketAddress lsa = new InetSocketAddress(host, port);
			ConnectFuture cf = connector.connect(lsa);
			cf.awaitUninterruptibly();
			if (cf.isConnected()) {
				session = cf.getSession();
			}
		} catch (Exception e) {
			logger.error("��TCP�ͻ��ˡ���������ʧ��", e);
		}
	}
	
	/**
	 * ������Ϣ
	 * 
	 * @param message
	 */
	public void sendMessage(String message) {
		try {
			StringBuffer sf = new StringBuffer();
			sf.append(message);
			IoBuffer buffer = IoBuffer.allocate(2048, true);// �����direct��heap��ʽ�Ļ�������
			buffer.setAutoExpand(true);// �Զ�����
			buffer.setAutoShrink(true);// �Զ�����
			buffer.putString(sf.toString(), Charset.forName(("gbk"))
					.newEncoder());
			buffer.flip();
			buffer.free();
			session.write(buffer);
		} catch (Exception e) {
			logger.error("��TCP�ͻ��ˡ�������Ϣʧ��", e);
		}

	}

	public static void main(String[] args) {
	}

	@Override
	public void messageReceived(IoSession session, Object message)
			throws Exception {
	}

	@Override
	public void exceptionCaught(IoSession session, Throwable cause)
			throws Exception {
		session.close(true);
	}

}
