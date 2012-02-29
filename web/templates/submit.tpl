{include file="header.tpl"}
	<div id="main-left">
        {if $cid}
        {include file="contest_left.tpl"}
        {else}
		{include file="main_left.tpl"}
        {/if}
	</div>
	<div id="main-right">
		<div class="rectdiv">
			<div class="cat-head">::Submit Solution</div>
			<div style="text-align:left;padding:20px;">
                <!-- {if $runId} -->
                Your code has been submitted.You can view the <a href="status.php?cid={$cid}&enable_search={$enable_search}">run status</a> here.
                <!-- {else} -->
				<!-- {if $error_mesg} -->
				<div class="error">
				>> {$error_mesg}
				</div>
				<!-- {/if} -->
				<form action="submit.php" method="post" accept-charset="dos-866">
				<table width="100%" cellspacing="2" cellpadding="2" border="0">
				<tr><td width="10%"></td>
				<td width="12%">Username:</td>
				<td><input name="username" class="inp" value="{if $login_user.username}{$login_user.username}{else}{$submit.username}{/if}" style="width:200px;"> </td>
				</tr>
				<!-- {if !$smarty.cookies.user_id} -->
				<tr><td width="10%"></td>
				<td>Password:</td>
				<td><input type="password" name="password" class="inp" value="" style="width:200px;"></td>
				</tr>
				<!-- {/if} -->
				<tr><td width="10%"></td>
				<td>Problem:</td>
                {if $cid}
                <td>
                <select name="problemId">
                <option value="0">Choose</option>
                {if $problemId}{assign var="t_pid" value=$problemId}{else}{assign var="t_pid" value=$submit.problemId}{/if}
                <!--{foreach from=$problems item=p}-->
                <option value="{$p.problemId}" {if $t_pid eq $p.problemId}selected{/if}>{$p.name}</option>
                <!--{/foreach}-->
                </select>
                </td>
                {else}
				<td><input name="problemId" class="inp" value="{if $problemId}{$problemId}{else}{$submit.problemId}{/if}" style="width:200px;"></td>
                {/if}
                </tr>
				<tr>
				<td width="10%"></td>
				<td>Compiler:</td>
                <td>
                {if !$submit.languageId}
                {assign var="lanId" value=$login_user.defaultLanguageId}
                {else}
                {assign var="lanId" value=$submit.languageId}
                {/if}
				{html_radios name="languageId" options=$compilers selected=$lanId}
				</td></tr>
				<tr><td width="20%"></td><td width="100%" colspan="2">Code:<br>
				<textarea cols="60" name="source" rows="13" wrap="virtual" class="inp">{$submit.source}</textarea>
				</td></tr></table>
				<table align="center" height="5%"><tr><td></td></tr>
				<table align="center">
				<tr>
				<td align="middle" colspan="2" width="60%">
				<input type="hidden" name="act" value="sendfile" />
                <input type="hidden" name="cid" value="{$cid}" />
				<input type="submit" value="submit"></td>
				</tr>
				</table>
				</form>
                <!-- {/if} -->
			</div>
		</div>
	</div>
	<div style="clear:both;"></div>
</div>
{include file="footer.tpl"}

