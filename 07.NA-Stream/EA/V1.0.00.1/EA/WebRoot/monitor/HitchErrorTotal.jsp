<%@ page language="java" contentType="text/html; charset=utf-8"%>
<%@ include file="/common/include/taglib.jsp"%>
<eRedG4:html title="错误码统计">
<eRedG4:import src="/monitor/js/hitchErrorTotal.js" />
<eRedG4:ext.codeStore fields="HITCHLEVEL,ISSEND,HITCHSTATUS"/>
<eRedG4:ext.codeRender fields="HITCHLEVEL,ISSEND,HITCHSTATUS"/>
<eRedG4:body>
</eRedG4:body>
</eRedG4:html>