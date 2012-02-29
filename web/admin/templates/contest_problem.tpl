{if $full_page}
{include file="pageheader.tpl"}
<form method="post" action="" name="listForm">
<div class="list-div" id="listDiv">
{/if}
<table cellspacing="1" cellpadding="1">
    <tr>
        <th width="20%">Problem ID</th>
        <th>Title</th>
        <th width="10%">Order</th>
        <th>Operate</th>
    </tr>
    {foreach from=$problems item=p}
    <tr>
        <td align="center">{$p.problemId} Problem {$p.name}</td>
        <td>{$p.problemName}</td>
        <td align="center">{$p.sort_order}</td>
        <td align="center"><a href="javascript:;" onclick="remove({$p.id})" title="delete"><img src="images/icon_drop.gif" width="16" height="16" border="0"/></a></td>
    </tr>
    {foreachelse}
    <tr><td class="no-records" colspan="4">No Records</td></tr>
    {/foreach}
</table>

{if $full_page}
</div>
<div class="list-div">
<table>
<tr><td><input type="button" class="button" value="Add A Problem To The Contest" onclick="document.getElementById('pinfo').style.display='block';"/></tr></tr>
<tr><td>
<div id="pinfo" style="display:none;">
Problem ID:<input type="text" name="pid" id="pid" /> Sort Order:<input type="text" name="sort_order" id="sort_order" /><input type="button" class="button" onclick="addproblem()" value="Add" />
</div></td></tr>
</table>
</div>
</form>

<script type="text/javascript">
var cid={$contest.id};
{literal}
function remove(id) {
    if(confirm('Sure to delete it?')) {
        Ajax.call('contest.php?act=remove_contest_problem&id='+id+'&cid='+cid,'',function(res){
            document.getElementById('listDiv').innerHTML = res.content; 
        },'GET','JSON');
    }
}
function addproblem() {
    var problemId=document.getElementById('pid').value;
    var sort_order=document.getElementById('sort_order').value;
    Ajax.call('contest.php?act=add_contest_problem&pid='+problemId+'&sort_order='+sort_order+'&cid='+cid,'',function(result) {
        if(result.error) {
            alert(result.message);
        }else{
            document.getElementById('listDiv').innerHTML = result.content;
        }
    },'GET','JSON');
}
</script>
{/literal}

{include file="pagefooter.tpl"}
{/if}

