<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>{#webname#}</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link href="styles/general.css" rel="stylesheet" type="text/css" />
{literal}
<style type="text/css">
#header-div {
  background: #278296;
  border-bottom: 1px solid #FFF;
}

#logo-div {
  height: 50px;
  float: left;
}

#submenu-div {
  height: 50px;
}

#submenu-div ul {
  margin: 0;
  padding: 0;
  list-style-type: none;
}

#submenu-div li {
  float: right;
  padding: 0 10px;
  margin: 5px 0;
  border-left: 1px solid #FFF;
}

#submenu-div a:visited, #submenu-div a:link {
  color: #FFF;
  text-decoration: none;
}

#submenu-div a:hover {
  color: #F5C29A;
}

#loading-div {
  clear: right;
  text-align: right;
  display: block;
}

#menu-div {
  background: #80BDCB;
  font-weight: bold;
  padding-left: 30px;
  height: 24px;
}

#menu-div ul {
  margin: 0;
  padding: 0;
  list-style-type: none;
}

#menu-div li {
  float: left;
  padding: 0 20px;
  margin: 5px 0;
  border-right: 1px solid #192E32;
}

#menu-div a:visited, #menu-div a:link {
  text-decoration: none;
  color: #335B64;
}

#menu-div a:hover {
  color: #F5C29A;
}
</style>
{/literal}
<script type="text/javascript">
var admin_id={$admin_id};
</script>
</head>
<body>
<div id="header-div">
  <div id="logo-div" style="font-size:20px;color:#fff;line-height:45px;">
    {#webname#} Control Panel
  </div>
  <div id="submenu-div">
    <ul>
      <li><a href="user.php?act=modify" target="main-frame">Profile</a></li>
      <li><a href="javascript:window.top.frames['main-frame'].document.location.reload();window.top.frames['header-frame'].document.location.reload()">Refresh</a></li>
    </ul>
    
    <div id="load-div" style="padding: 5px 10px 0 0; text-align: right; color: #FF9900; display: none;width:40%;float:right;"><img src="images/top_loader.gif" width="16" height="16" alt="Loading" style="vertical-align: middle" /> Loading</div>
  </div>
</div>
<div id="menu-div">
  <ul>
    <li><a href="index.php?act=main" target="main-frame">Home Page</a></li>
    <li style="float: right; border-right: 0; "><a href="user.php?act=logout" target="_top">Log out</a></li>
    <li style="float: right; "><a href="index.php?act=clear_cache" target="main-frame">Clear Cache</a></li>
  </ul>
  <br class="clear" />
</div>
</body>
</html>
