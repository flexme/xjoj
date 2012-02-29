<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>{#webname#} - login</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link href="styles/general.css" rel="stylesheet" type="text/css" />
<link href="styles/main.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/utils.js"></script>
<script type="text/javascript" src="js/validator.js"></script>
{literal}
<style type="text/css">
body {
  color: white;
}
</style>
{/literal}
</head>
<body style="background: #278296">
<form method="post" action="user.php" name='theForm' onsubmit="return validate()">
  <table cellspacing="0" cellpadding="0" style="margin-top: 100px" align="center">
  <tr>
    <td style="font-weight:bold;font-size:16px;">{#webname#} Admin Login</td>
    <td style="padding-left: 50px">
      <table>
      <tr>
        <td>Username:</td>
        <td><input type="text" name="username" /></td>
      </tr>
      <tr>
        <td>Password:</td>
        <td><input type="password" name="password" /></td>
      </tr>
      <tr><td>&nbsp;</td><td><input type="submit" value="Login" class="button" /></td></tr>
      </table>
    </td>
  </tr>
  </table>
  <input type="hidden" name="act" value="signin" />
</form>
<script language="JavaScript">
<!--
  document.forms['theForm'].elements['username'].focus();
  {literal}
  /**
   * 检查表单输入的内容
   */
  function validate()
  {
    var validator = new Validator('theForm');
    validator.required('username', 'username should not be empty');
    validator.required('password', 'password should not be empty');
    return validator.passed();
  }
  {/literal}
//-->
</script>
</body>