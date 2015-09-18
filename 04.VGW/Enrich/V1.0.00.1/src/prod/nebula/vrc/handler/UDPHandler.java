package prod.nebula.vrc.handler;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.vrc.module.resctrl.Controller;
import prod.nebula.vrc.module.resctrl.VODResCtrl;
import prod.nebula.vrc.module.resctrl.dto.VODRecvKeyMsgBean;
import prod.nebula.vrc.util.KeyEnum;

public class UDPHandler extends IoHandlerAdapter {
	public static final Logger logger = LoggerFactory
			.getLogger(UDPHandler.class);

	private Controller ctrl;

	public UDPHandler(Controller ctrl) {
		super();
		this.ctrl = ctrl;
	}

	/**
	 * ��ǰ���ű���
	 */
	private static int scale = 0;

	/**
	 * @return the scale
	 */
	public static int getScale() {
		return scale;
	}

	/**
	 * @param scale
	 *            the scale to set
	 */
	public static void setScale(int scale) {
		UDPHandler.scale = scale;
	}

	// messageSent��Server��Ӧ��Clinet�ɹ��󴥷����¼�
	@Override
	public void messageSent(IoSession session, Object message) throws Exception {
	}

	// �׳��쳣�������¼�
	@Override
	public void exceptionCaught(IoSession session, Throwable cause)
			throws Exception {
		cause.printStackTrace();
		session.close(true);
	}

	// Server���յ�UDP���󴥷����¼�
	@Override
	public void messageReceived(IoSession session, Object message)
			throws Exception {
		IoBuffer buffer = (IoBuffer) message;
		byte[] recvBytes = buffer.array();
		buffer.free();

		VODRecvKeyMsgBean bean = null;

		int devType = getDevType(recvBytes);
		// �ж��Ƿ�Ϊң�����豸
		if (devType == KeyEnum.KeyDevType.IrrControl.getDevType()) {
			int seqNum = getInt(recvBytes, 4);
			int keyValue = getInt(recvBytes, 8);
			int keyStatus = getInt(recvBytes, 12);
			// �жϰ����Ƿ��ͷ�
			if (!isKeyRelease(keyStatus)) {
				return;
			}
			bean = new VODRecvKeyMsgBean(devType, seqNum, keyValue, keyStatus);
			logger.info("�յ���ֵ��" + keyValue);
		} else {
			return;
		}

		if (bean != null) {
			VODResCtrl.getInstance().VODresDisManage(session,
					String.valueOf(bean.getKeyValue()), ctrl);
		}
	}

	private boolean isKeyRelease(int keyStatus) {
		return keyStatus == KeyEnum.IrrKeyStat.IrrStatUp.getStatus();
	}

	private int getDevType(byte[] bytes) {
		return getInt(bytes, 0);
	}

	private int getInt(byte[] bytes, int startLoc) {
		byte[] buf = new byte[4];
		System.arraycopy(bytes, startLoc, buf, 0, 4);
		return byte2Int(buf);
	}

	private int byte2Int(byte[] buf) {
		int sum = 0;
		for (int i = 0; i < 4; i++) {
			int temp = ((int) buf[i]) & 0xff;

			temp <<= i * 8;
			sum = temp + sum;

		}
		return sum;
	}

	// ���ӹرմ������¼�
	@Override
	public void sessionClosed(IoSession session) throws Exception {
	}

	// �������Ӵ������¼�
	@Override
	public void sessionCreated(IoSession session) throws Exception {
	}

	// �Ự����
	@Override
	public void sessionIdle(IoSession session, IdleStatus status)
			throws Exception {
	}

	// �����Ӵ������¼�������sessionCreated���������ڣ�һ�����ӵ�ַ��A����һ������Server�Ὠ��һ��SessionĬ�ϳ�ʱʱ��Ϊ1���ӣ���ʱ��δ�ﵽ��ʱʱ��������ӵ�ַ��A����һ����Server����������sessionOpened�����ӵ�ַ��A����һ����Server��������������ӳ�ʱ����Server��������ʱ��ͬʱ����sessionCreated��sessionOpened�����¼���
	@Override
	public void sessionOpened(IoSession session) throws Exception {
	}
}
