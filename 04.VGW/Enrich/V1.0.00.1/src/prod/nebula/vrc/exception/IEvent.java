package prod.nebula.vrc.exception;

import java.io.IOException;
import java.nio.channels.SelectionKey;

/**
* 
* IEvent.java �����¼�����������Selector���Խ��в���ʱ����������ӿ��еķ�����
* 
* 2007-3-22 ����03:35:51
* 
* @author sycheng
* @version 1.0
*/
public interface IEvent {

    /**
    * ��channel�õ�connect�¼�ʱ�����������
    * 
    * @param key
    * @throws IOException
    */
    void connect(SelectionKey key) throws IOException;

    /**
    * ��channel�ɶ�ʱ�����������
    * 
    * @param key
    * @throws IOException
    */
    void read(SelectionKey key) throws IOException;

    /**
    * ��channel��дʱ�����������
    * 
    * @throws IOException
    */
    void write() throws IOException;
    
    /**
    * ��channel��������ʱ����
    * 
    * @param e
    */
    void error(Exception e);
}
