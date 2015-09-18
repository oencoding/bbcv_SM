package com.bbcvision.Multiscreen.netservices;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;

import android.text.TextUtils;
import android.util.Log;

import com.bbcvision.Multiscreen.bean.ChooseTimeInfo;
import com.bbcvision.Multiscreen.bean.KeySendInfo;
import com.bbcvision.Multiscreen.bean.SessionQueryInfo;
import com.bbcvision.Multiscreen.bean.UnBindInfo;
import com.bbcvision.Multiscreen.bean.UserBingInfo;
import com.bbcvision.Multiscreen.bean.UserLoginInfo;
import com.bbcvision.Multiscreen.bean.VodPlayInfo;
import com.google.gson.Gson;

public class JSONService {
	public static int regJSON(String json) {
		int request = -2;
		if (TextUtils.isEmpty(json)) {
			request = -1;
		} else {
			JSONObject jsonObj;
			try {
				jsonObj = new JSONObject(json);
				request = jsonObj.getInt("return_code");
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
		return request;
	}

	public static Map<String, String> accessJSON(String json) {
		Map<String, String> results = new HashMap<String, String>();
		if (TextUtils.isEmpty(json)) {
			results.put("return_code", "-1");
		} else {
			JSONObject jsonObj;
			try {
				jsonObj = new JSONObject(json);
				String return_code = jsonObj.getString("return_code");
				String service_url = jsonObj.getString("service_url");
				String token = jsonObj.getString("token");
				results.put("return_code", return_code);
				results.put("service_url", service_url);
				results.put("token", token);
			} catch (JSONException e) {
				e.printStackTrace();
				results.put("return_code", "-1");
			}
		}
		return results;
	}

	public static UserLoginInfo loginSON(String jsonData) {
		Gson gson = new Gson();
		UserLoginInfo user = gson.fromJson(jsonData, UserLoginInfo.class);
		//Log.i("UserLoginInfo", user.getReturn_code()+user.getUrl());
		return user;
	}

	public static UserBingInfo bindJSON(String jsonData) {
		Gson gson = new Gson();
		UserBingInfo userBingInfo = gson.fromJson(jsonData, UserBingInfo.class);
		return userBingInfo;
	}
	
	public static VodPlayInfo vodPlayJSON(String jsonData) {
		Gson gson = new Gson();
		VodPlayInfo vodPlayInfo = gson.fromJson(jsonData, VodPlayInfo.class);
		return vodPlayInfo;
	}
	
	public static UnBindInfo unBindJSON(String jsonData) {
		Gson gson = new Gson();
		UnBindInfo unBindInfo = gson.fromJson(jsonData, UnBindInfo.class);
		return unBindInfo;
	}
	
	public static KeySendInfo keySendInfoJSON(String jsonData) {
		Gson gson = new Gson();
		KeySendInfo keySendInfo = gson.fromJson(jsonData, KeySendInfo.class);
		return keySendInfo;
	}
	
	public static ChooseTimeInfo chooseTimeInfoJSON(String jsonData) {
		Gson gson = new Gson();
		ChooseTimeInfo chooseTimeInfo = gson.fromJson(jsonData, ChooseTimeInfo.class);
		return chooseTimeInfo;
	}
	
	public static SessionQueryInfo eessionQueryInfoJSON(String jsonData) {
		Gson gson = new Gson();
		SessionQueryInfo eessionQueryInfo = gson.fromJson(jsonData, SessionQueryInfo.class);
		return eessionQueryInfo;
	}
}
