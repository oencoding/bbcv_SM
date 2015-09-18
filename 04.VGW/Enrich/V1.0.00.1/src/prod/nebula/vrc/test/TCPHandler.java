package prod.nebula.vrc.test;

import java.nio.charset.Charset;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;

public class TCPHandler extends IoHandlerAdapter {
	
	@Override
	public void exceptionCaught(IoSession session, Throwable cause)
			throws Exception {
		super.exceptionCaught(session, cause);
	}

	@Override
	public void messageReceived(IoSession session, Object message)
			throws Exception {
		IoBuffer buffer = (IoBuffer) message;
		String recvMsg = buffer.getString(Charset.forName("gbk").newDecoder());
		buffer.free();
		System.out.println("��TCPHander��message recieve:��" + recvMsg);
		session.close(true);
	}
	
	@Override
	public void messageSent(IoSession session, Object message) throws Exception {
		super.messageSent(session, message);
	}

	@Override
	public void sessionClosed(IoSession session) throws Exception {
		System.out.println("sessionClosed");
		session.close(true);
	}
	
	@Override
	public void sessionCreated(IoSession session) throws Exception {
		super.sessionCreated(session);
	}

	@Override
	public void sessionIdle(IoSession session, IdleStatus status)
			throws Exception {
		super.sessionIdle(session, status);
	}
	
	@Override
	public void sessionOpened(IoSession session) throws Exception {
		super.sessionOpened(session);
	}

}
