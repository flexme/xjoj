{include file="pageheader.tpl"}
<div class="list-div" id="listDiv">
    <table cellpadding="3" cellspacing="1">
    <tr>
        <th>TD</th>
        <th>Username</th>
        <th>Last Login Time</th>
        <th>Last Login IP</th>
        <th>Operate</th>
    </tr>
    <!-- {foreach from=$users item=user} -->
    <tr>
        <td align="center">{$user.user_id}</td>
        <td>{$user.username}</td>
        <td align="center">{$user.last_login|date_format:"%Y-%m-%d %H:%M:%S"}</td>
        <td align="center">{$user.last_ip}</td>
        <td align="center"><a href="user.php?act=modify&user_id={$user.user_id}"><img src="images/icon_edit.gif" width="16" height="16" border="0" /></a>
        <a href="#" onclick="javascript:if(confirm('Be sure to delete this user?')) location.href='user.php?act=delete&user_id={$user.user_id}';"><img src="images/icon_drop.gif" width="16" height="16" border="0" /></a></td>
    </tr>
    <!-- {/foreach} -->
    </table>
</div>
{include file="pagefooter.tpl"}
