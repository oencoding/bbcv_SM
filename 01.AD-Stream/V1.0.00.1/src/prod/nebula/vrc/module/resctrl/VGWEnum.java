package prod.nebula.vrc.module.resctrl;

import prod.nebula.vrc.core.Enumeration;

	public enum VGWEnum implements Enumeration {

		CMD("cmd"),
		VERSION("version"),
		APPNAME("appname"),
		HOSTIP("hostip"),
		URL("url"),
		BOOTTIME("boottime"),
		;
		private String desc;
		
		private VGWEnum(String desc) {
			this.desc = desc;
		}

		@Override
		public String getCode() {
			return null;
		}

		@Override
		public String getDesc() {
			return this.desc;
		}
	}

