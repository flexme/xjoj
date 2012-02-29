{include file="header.tpl"}
    <div id="main-left">
        {include file="main_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::Contest List</div>
            <div style="padding:10px;text-align:center;">
            <table width="90%" border="0" align="center">
            <tr>
                <td class="th">ID</td>
                <td class="th" width="50%">Title</td>
                <td class="th">Start Time</td>
                <td class="th">Status</td>
            </tr>
            <!--{foreach from=$contest_list item=contest key=i}-->
            <tr class="st{$i%2}">
                <td>{$contest.id}</td>
                <td><a href="contest.php?act=home&cid={$contest.id}">{$contest.title}</a></td>
                <td>{$contest.startTime|date_format:"%Y-%m-%d %H:%M:%S"}</td>
                <td style="color:green">{$contest.status}</td>
            </tr>
            <!--{/foreach}-->
            </table>

            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

