package prod.nebula.vrc.util;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

public class ApplicationContextHelper implements ApplicationContextAware{
	private static ApplicationContext appCtx;  
    /** 
     * �˷������԰�ApplicationContext����inject����ǰ������Ϊһ����̬��Ա������ 
     * @param applicationContext ApplicationContext ����. 
     * @throws BeansException 
     */  
    @Override  
    public void setApplicationContext( ApplicationContext applicationContext ) throws BeansException {  
        appCtx = applicationContext;  
    }  
    /** 
     * ����һ�������ķ������������ǿ��ٵõ�һ��BEAN 
     * @param beanName bean������ 
     * @return ����һ��bean���� 
     */  
    public static Object getBean( String beanName ) {  
        return appCtx.getBean( beanName );  
    } 
}
