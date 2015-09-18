package prod.nebula.vrc.util;

/**
 * 
 * @author Chenmm
 * 
 *         ��ֵͨ�ű���ö��ֵ
 * 
 */

public class KeyEnum {

	/**
	 * �����豸���� 1001 ��ң�����豸
	 */
	public enum KeyDevType {

		IrrControl(1001);

		private int devType;

		private KeyDevType(int devType) {
			this.devType = devType;
		}

		public int getDevType() {
			return devType;
		}

	}

	/**
	 * ң������ֵ״̬ 3 ���ͷ� 4 ������
	 */
	public enum IrrKeyStat {
		IrrStatUp(3), IrrStatDown(4);

		private int status;

		private IrrKeyStat(int status) {
			this.status = status;
		}

		public int getStatus() {
			return status;
		}
	}

}
