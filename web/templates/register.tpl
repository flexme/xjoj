{include file="header.tpl"}
	<div id="main-left">
		{include file="main_left.tpl"}
	</div>
	<div id="main-right">
		<div class="rectdiv">
			<div class="cat-head">::Register</div>
			<div style="text-align:left;padding:20px;">
				<!-- {if $error_msg} -->
				<div class="error">
					>> {$error_msg}
				</div>
				<!-- {/if} -->
				<form action="user.php" method="post" name="theForm">
				<table cellspacing="0" cellpadding="0" border="0" width="100%">
				<tr><td width="20%">Username:</td>
					<td style="text-align:left;"><input name="username" class="inp" size="20" tabindex="1" value="{$user.username}" /></td></tr>
				<tr><td>Your E-Mail:</td>
					<td><input name="email" class="inp" tabindex="2" value="{$user.email}" /></td></tr>
                <tr><td>Default Compiler:</td>
                    <td>{html_radios name="languageId" options=$compilers selected=$user.languageId}</td>
                </tr>
				<tr><td colspan="2"><hr/></td></tr>
				<tr><td>Your Password:</td>
					<td><input type="password" name="password" class="inp" tabindex="3" value="{$user.password}" /> <span style="color:red">at least 6 chars</span></td></tr>
				<tr><td>Confirm Password:</td>
					<td><input type="password" name="confirm_password" class="inp" tabindex="4" value="{$user.confirm_password}" /></td></tr>
				<tr><td colspan="2"><hr/></td></tr>
				<tr><td colspan="2">Plan:<br/>
					<textarea cols="50" name="plan" rows="5" wrap="virtual" class="inp" tabindex="5">{$user.plan}</textarea>
				</td></tr>
				<tr><td colspan="2"><hr/></td></tr>
				<tr>
				<td align="center" colSpan="2"><input type="submit" value="register" tabindex="6" /><input type="reset" value="reset" /><input type="hidden" name="act" value="act_register" /></td></tr>
				</table></form>
			</div>
		</div>
	</div>
	<div class="clear"></div>
</div>
<script type="text/javascript">
document.forms['theForm'].elements['username'].focus();
</script>
{include file="footer.tpl"}

