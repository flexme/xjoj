{if $full_page}
{include file="pageheader.tpl"}
{literal}
<style type="text/css">
.rank_star * {
	background:transparent url(../templates/images/star.png) no-repeat scroll 0 0;
	display:block;
	height:22px;
	overflow:hidden;
	text-indent:-3000px;
	width:80px;
}
.rank5 {
	background-position: -63px -21px;
}
.rank10 {
	background-position: -63px -3px;
}
.rank15 {
	background-position: -47px -21px;
}
.rank20 {
	background-position: -47px -3px;
}
.rank25 {
	background-position: -31px -21px;
}
.rank30 {
	background-position: -31px -3px;
}
.rank35 {
	background-position: -15px -21px;
}
.rank40 {
	background-position: -15px -3px;
}
.rank45 {
	background-position: 0px -21px;
}
.rank50 {
	background-position: 0px -3px;
}
</style>
{/literal}
<form method="post" action="" name="listForm">
<div class="list-div" id="listDiv">
{/if}
<table cellspacing="1" cellpadding="1">
    <tr>
        <th width="20%">Problem ID</th>
        <th>Title</th>
        <th width="10%">Level</th>
        <th>Operate</th>
    </tr>
    {foreach from=$problems item=p}
    <tr>
        <td align="center">{$p.problemId}</td>
        <td>{$p.problemName}</td>
        <td align="center"><em class="rank_star"><a class="rank{$p.level}"></a></em></td>
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
Problem ID:<input type="text" name="pid" id="pid" /> Level:
<select id="level">
	<option value="0">No star</option>
	<option value="10">1 star</option>
	<option value="15">1.5 star</option>
	<option value="20">2 star</option>
	<option value="25">2.5 star</option>
	<option value="30">3 star</option>
	<option value="35">3.5 star</option>
	<option value="40">4 star</option>
	<option value="45">4.5 star</option>
	<option value="50">5 star</option>
</select>
<input type="button" class="button" onclick="addproblem()" value="Add" />
</div></td></tr>
</table>
</div>
</form>

<script type="text/javascript">
var cid={$assemb.id};
{literal}
function remove(id) {
    if(confirm('Sure to delete it?')) {
        Ajax.call('assemb.php?act=remove_assemb_problem&id='+id+'&cid='+cid,'',function(res){
            document.getElementById('listDiv').innerHTML = res.content; 
        },'GET','JSON');
    }
}
function addproblem() {
    var problemId=document.getElementById('pid').value;
    var level=document.getElementById('level').value;
    Ajax.call('assemb.php?act=add_assemb_problem&pid='+problemId+'&level='+level+'&cid='+cid,'',function(result) {
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

