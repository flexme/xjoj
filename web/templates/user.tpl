{include file="header.tpl"}
    <div id="main-left">
        {include file="main_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::{$title}</div>
            <div style="text-align:left;padding:10px;">
                <!-- {if $error_msg} -->
                <div class="error">
                    >> {$error_msg}
                </div>
                <!-- {/if} -->
                <!-- {if $action eq update} -->
                <form action="user.php" method="post" name="theForm">
                <table cellpadding="0" cellspacing="0" border="0" width="100%">
                <tr><td width="20%">Username:</td>
                    <td>{$user_info.username}</td>
                </tr>
                <tr><td>Your E-Mail:</td>
                    <td><input type="text" name="email" class="inp" tabindex="1" value="{$user_info.email}"/></td></tr>
                <tr><td>Default Compiler:</td>
                    <td>{html_radios name="languageId" options=$compilers selected=$user_info.defaultLanguageId}</td>
                <tr><td colspan="2"><hr/></td></tr>
                <tr><td>Old Password:</td>
                    <td><input type="password" name="old_password" class="inp" tabindex="2" /></td></tr>
                <tr><td>New Password:</td>
                    <td><input type="password" name="password" class="inp" tabindex="3" /></td></tr>
                <tr><td colspan="2"><hr/></td></tr>
                <tr><td colspan="2">Plan:<br/>
					<textarea cols="50" name="plan" rows="5" wrap="virtual" class="inp" tabindex="4">{$user_info.plan}</textarea>
				</td></tr>
                <tr><td colspan="2"><hr/></td></tr>
                <tr>
                <td align="center" colSpan="2"><input type="submit" value="update" tabindex="5" /><input type="hidden" name="act" value="do_update" /></td></tr>
                </table>
                </form>
                <!-- {elseif $action eq detail} -->
                <div><font size="5" color="#db6d00">{$user_info.username}</font>
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                <font size="3">Solved:</font>
                <font color="red" size="4">{$user_info.ac_total}</font>
                Submit:<font color="red">{$user_info.total_submit}</font>
                </div> 
                <div style="margin-top:15px;">
                <b>Plan:</b><div style="padding:5px;margin:0;">{$user_info.plan}</div>
                </div>
                <div style="margin-top:15px;" id="plist">
                <b>Solved Problems:</b>
                <ul>
                    <!-- {foreach from=$solved_problems item=problem} -->
                    <li><a href="problem_html/{$problem.problemId}.html">{$problem.problemId}</a></li>
                    <!-- {/foreach} -->
                </ul>
                <div class="clear"></div>
                </div>
                <!-- {/if} -->
            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

