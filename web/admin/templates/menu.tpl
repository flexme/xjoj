<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>{#webname#} Menu</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link href="styles/general.css" rel="stylesheet" type="text/css" />
{literal}
<style type="text/css">
body {
  background: #80BDCB;
}
#tabbar-div {
  background: #278296;
  padding-left: 10px;
  height: 21px;
  padding-top: 0px;
}
#tabbar-div p {
  margin: 1px 0 0 0;
}
.tab-front {
  background: #80BDCB;
  line-height: 20px;
  font-weight: bold;
  padding: 4px 15px 4px 18px;
  border-right: 2px solid #335b64;
  cursor: hand;
  cursor: pointer;
}
.tab-back {
  color: #F4FAFB;
  line-height: 20px;
  padding: 4px 15px 4px 18px;
  cursor: hand;
  cursor: pointer;
}
.tab-hover {
  color: #F4FAFB;
  line-height: 20px;
  padding: 4px 15px 4px 18px;
  cursor: hand;
  cursor: pointer;
  background: #2F9DB5;
}
#top-div
{
  padding: 3px 0 2px;
  background: #BBDDE5;
  margin: 5px;
  text-align: center;
}
#main-div {
  border: 1px solid #345C65;
  padding: 5px;
  margin: 5px;
  background: #FFF;
}
#menu-list {
  padding: 0;
  margin: 0;
}
#menu-list ul {
  padding: 0;
  margin: 0;
  list-style-type: none;
  color: #335B64;
}
#menu-list li {
  padding-left: 16px;
  line-height: 16px;
  cursor: hand;
  cursor: pointer;
}
#main-div a:visited, #menu-list a:link, #menu-list a:hover {
  color: #335B64
  text-decoration: none;
}
#menu-list a:active {
  color: #EB8A3D;
}
.explode {
  background: url(images/menu_minus.gif) no-repeat 0px 3px;
  font-weight: bold;
}
.collapse {
  background: url(images/menu_plus.gif) no-repeat 0px 3px;
  font-weight: bold;
}
.menu-item {
  background: url(images/menu_arrow.gif) no-repeat 0px 3px;
  font-weight: normal;
}
#help-title {
  font-size: 14px;
  color: #000080;
  margin: 5px 0;
  padding: 0px;
}
#help-content {
  margin: 0;
  padding: 0;
}
.tips {
  color: #CC0000;
}
.link {
  color: #000099;
}
</style>
{/literal}
</head>
<body>
<div id="main-div">
<div id="menu-list">
<ul>
<li class="explode" key="02_problems" name="menu">
    Problems        <ul>
          <li class="menu-item"><a href="problems.php?act=list" target="main-frame">Problems List</a></li>
		  <li class="menu-item"><a href="problems.php?act=add" target="main-frame">Add Problem</a></li>
		  <li class="menu-item"><a href="problems.php?act=problem_html" target="main-frame">Gen Html</a></li>
        </ul>
      </li>
  <li class="explode" key="20_contest" name="menu">
	Contest		<ul>
	  <li class="menu-item"><a href="contest.php?act=list" target="main-frame">Contest List</a></li>
	  <li class="menu-item"><a href="contest.php?act=add" target="main-frame">Add Contest</a></li>
	</ul>
  </li>
  <li class="explode" key="20_assemble" name="menu">
    Assemble		<ul>
	  <li class="menu-item"><a href="assemb.php?act=list" target="main-frame">Assemble List</a></li>
	  <li class="menu-item"><a href="assemb.php?act=add" target="main-frame">Add Assemble</a></li>
	</ul>
  </li>
  <li class="explode" key="20_news" name="menu">
    News    <ul>
    <li class="menu-item"><a href="news.php?act=list" target="main-frame">News List</a></li>
    <li class="menu-item"><a href="news.php?act=add" target="main-frame">Add News</a></li>
    </ul>
  </li>
  <li class="explode" key="20_compilers" name="menu">
    Compiler    <ul>
    <li class="menu-item"><a href="compiler.php?act=list" target="main-frame">Compiler List</a></li>
    <li class="menu-item"><a href="compiler.php?act=add" target="main-frame">Add Compiler</a></li>
    </ul>
  </li>
  <li class="explode" key="20_user" name="menu">
	User		<ul>
	  <li class="menu-item"><a href="user.php?act=user_list" target="main-frame">User List</a></li>
      <li class="menu-item"><a href="user.php?act=user_source" target="main-frame">User Source</a></li>
	  <li class="menu-item"><a href="user.php?act=list" target="main-frame">Admin User List</a></li>
	  <li class="menu-item"><a href="user.php?act=add" target="main-frame">Add Admin User</a></li>
	  <li class="menu-item"><a href="user.php?act=log" target="main-frame">Admin Log</a></li>
	</ul>
  </li>
  <li class="explode" key="20_system" name="menu">
  System        <ul>
	  <li class="menu-item"><a href="rejudge.php?act=list" target="main-frame">Rejudge</a></li>
      <li class="menu-item"><a href="system.php?act=client_info" target="main-frame">Client Info</a></li>
    </ul>
  </li>
</ul>
</div>
</div>
<script type="text/javascript" src="js/utils.js"></script>
<script language="JavaScript">
<!--
var collapse_all = "{$lang.collapse_all}";
var expand_all = "{$lang.expand_all}";
var collapse = true;
{literal}
function toggleCollapse()
{
  var items = document.getElementsByTagName('LI');
  for (i = 0; i < items.length; i++)
  {
    if (collapse)
    {
      if (items[i].className == "explode")
      {
        toggleCollapseExpand(items[i], "collapse");
      }
    }
    else
    {
      if ( items[i].className == "collapse")
      {
        toggleCollapseExpand(items[i], "explode");
        ToggleHanlder.Reset();
      }
    }
  }

  collapse = !collapse;
  document.getElementById('toggleImg').src = collapse ? 'images/menu_minus.gif' : 'images/menu_plus.gif';
  document.getElementById('toggleImg').alt = collapse ? collapse_all : expand_all;
}

function toggleCollapseExpand(obj, status)
{
  if (obj.tagName.toLowerCase() == 'li' && obj.className != 'menu-item')
  {
    for (i = 0; i < obj.childNodes.length; i++)
    {
      if (obj.childNodes[i].tagName == "UL")
      {
        if (status == null)
        {
          if (obj.childNodes[1].style.display != "none")
          {
            obj.childNodes[1].style.display = "none";
            //ToggleHanlder.RecordState(obj.getAttribute("key"), "collapse");
            obj.className = "collapse";
          }
          else
          {
            obj.childNodes[1].style.display = "block";
            //ToggleHanlder.RecordState(obj.getAttribute("key"), "explode");
            obj.className = "explode";
          }
          break;
        }
        else
        {
          if( status == "collapse")
          {
            //ToggleHanlder.RecordState(obj.getAttribute("key"), "collapse");
            obj.className = "collapse";
          }
          else
          {
            //ToggleHanlder.RecordState(obj.getAttribute("key"), "explode");
            obj.className = "explode";
          }
          obj.childNodes[1].style.display = (status == "explode") ? "block" : "none";
        }
      }
    }
  }
}
document.getElementById('menu-list').onclick = function(e)
{
  var obj = Utils.srcElement(e);
  toggleCollapseExpand(obj);
}
{/literal}
</script>
</body>
</html>
