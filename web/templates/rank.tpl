{include file="header.tpl"}
    <div id="main-left">
        {include file="main_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::Authors Ranklist</div>
        <div style="text-align:center;padding:10px;">
            <table border="0" cellpadding="0" cellspacing="1" width="100%" align="center">
            <tr><td colspan="5" height="30px" style="font-size:12px;">
            {if $page gt 1}<a href="?page={$page-1}">{else}<font color="gray">{/if}&lt;&lt;Prev page{if $page gt 1}</a>{else}</font>{/if}&nbsp;&nbsp;
            {if $page lt $total_pages}<a href="?page={$page+1}">{else}<font color="gray">{/if}Next page&gt;&gt;{if $page lt $total_pages}</a>{else}</font>{/if}
            </td></tr>
            <tr style="font-size:17px;">
                <td class="th">Rank</td>
                <td class="th">UserID</td>
                <td class="th">Plan</td>
                <td class="th">Solved</td>
                <td class="th">Submit</td>
            </tr>
            <!--{foreach from=$ranklist item=info key=i}-->
            <tr class="st{$i%2}">
                <td>{$offset+$i+1}</td>
                <td><a href="user.php?act=detail&authorId={$info.user_id}">{$info.username}</a></td>
                <td>{$info.plan|truncate:60}</td>
                <td>{if $info.ac_total}{$info.ac_total}{else}0{/if}</td>
                <td>{$info.total_submit}</td>
            </tr>
            <!--{/foreach}-->
            <tr><td colspan="5" height="30px" style="font-size:12px;">
            {if $page gt 1}<a href="?page={$page-1}">{else}<font color="gray">{/if}&lt;&lt;Prev page{if $page gt 1}</a>{else}</font>{/if}&nbsp;&nbsp;
            {if $page lt $total_pages}<a href="?page={$page+1}">{else}<font color="gray">{/if}Next page&gt;&gt;{if $page lt $total_pages}</a>{else}</font>{/if}
            </td></tr>
            </table>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

