{include file="pageheader.tpl"}
<div class="list-div" id="listDiv">
    <table cellpadding="3" cellspacing="1">
    <tr>
        <th>IP</th>
        <th>PORT</th>
        <th>Connect Time</th>
        <th>Status</th>
    </tr>
    <!-- {foreach from=$client_info item=info} -->
    <tr>
        <td align="center">{$info[0]}</td>
        <td align="center">{$info[1]}</td>
        <td align="center">{$info[2]}</td>
        <td align="center">{$info[3]}</td>
    </tr>
    <!-- {/foreach} -->
    </table>
</div>
{include file="pagefooter.tpl"}

