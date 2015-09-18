/**  
 * 类名称：RcmManager 
 * 类描述： 
 * 创建人：PengFei   
 * 创建时间：2014-10-14 下午07:40:35 
 */
package prod.nebula.service.dto;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import prod.nebula.service.util.ConnectionUtil;
import prod.nebula.service.util.DatabaseConn;
import prod.nebula.service.util.PropertiesUtil;
import prod.nebula.service.util.SQLCommand;

/**    
 * 类名称：RcmManager   
 * 类描述：  对 资源管理数据库svstb的操作。
 * 创建人：PengFei   
 * 创建时间：2014-10-14 下午07:40:35    
 * 备注：   
 * @version    
 *    
 */
public class RcmManager {
	private static final Logger logger = LoggerFactory.getLogger(RcmManager.class);
	
	
	public MobileDetailPage getMbDetailPage() {	
		
		
		Connection	conn = DatabaseConn.getConn();
		String sql =" select m.id ,m.url  from mobile_detail_page m where m.use_state=1 and state=0 ";
		PreparedStatement ps = null;
		System.out.println(sql);
		ResultSet rs = null;
		MobileDetailPage u =new MobileDetailPage();
	try {
		 ps =conn.prepareStatement(sql);
		
		rs = ps.executeQuery();

		while(rs.next()){
			
			u.setId(rs.getInt("id"));
			u.setUrl(rs.getString("url"));

		}
		
	} catch (SQLException e) {
		logger.error("数据库查询出错！");
		e.printStackTrace();
	} finally {
		try {
			if (rs != null)
				rs.close();
		} catch (Exception e) {
			logger.error("数据库查询后关闭资源错误");
		}
		try {
			if (ps != null)
				ps.close();
		} catch (Exception e) {
			logger.error("数据库查询后关闭资源错误");
		}
		try {
			if (conn != null)
				conn.close();
		} catch (Exception e) {
			logger.error("数据库查询后关闭资源错误");
		}
	}
		
		return u;
	
	}
        
	
    public VodResourceInfo getVod(String id){
    	
    	Connection	conn = DatabaseConn.getConn();
		String sql =" select v.id,v.name,v.grade,v.director,v.actor,v.years,v.runtime,v.big_poster_path,v.little_poster_path,v.type,v.area,v.other ,v.path,v.description from vod_resource_info v where v.id="+id;
		PreparedStatement ps = null;
		
		ResultSet rs = null;
		VodResourceInfo v=new VodResourceInfo();
	try {
		 ps =conn.prepareStatement(sql);
		
		rs = ps.executeQuery();

		while(rs.next()){
			
			v.setId(rs.getInt("id"));
		    v.setName(rs.getString("name"));
		    v.setGrade(rs.getInt("grade"));
		    v.setDirector(rs.getString("director"));
		    v.setDescription(rs.getString("description"));
		    v.setActor(rs.getString("actor"));
		    v.setYears(rs.getString("years"));
		    v.setRuntime(rs.getString("runtime"));
		    v.setPath(rs.getString("path"));
		    v.setBigPosterPath(rs.getString("big_poster_path"));
		    v.setLittlePosterPath(rs.getString("v.little_poster_path"));
		    v.setType(rs.getString("v.type"));
		    v.setArea(rs.getString("v.area"));
		    v.setOther(rs.getString("v.other"));
             
		}
		
	} catch (SQLException e) {
		logger.error("数据库查询vod资源出错！");
		e.printStackTrace();
	} finally {
		try {
			if (rs != null)
				rs.close();
		} catch (Exception e) {
			logger.error("数据库查询后关闭资源错误");
		}
		try {
			if (ps != null)
				ps.close();
		} catch (Exception e) {
			logger.error("数据库查询后关闭资源错误");
		}
		try {
			if (conn != null)
				conn.close();
		} catch (Exception e) {
			logger.error("数据库查询后关闭资源错误");
		}
	}
		
		return v;

    }
	
    
}
