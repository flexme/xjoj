{include file="pageheader.tpl"}
<script type="text/javascript">
var minid={$range.minr};
var maxid={$range.maxr};
var total={$total};
var start=0;
var end=0;
var now=0;
var next=0;
{literal}
function gen_html() {

	start=document.getElementById('start').value;
	end=document.getElementById('end').value;
	if(start!=parseInt(start) || start<minid || start>maxid) {
		alert('Wrong Min Id');
		return;
	}
	start=parseInt(start);
	if(end!=parseInt(end) || end<minid || end>maxid) {
		alert('Wrong Max Id');
		return;
	}
	end=parseInt(end);
	if(minid>maxid) {
		alert('Wrong Id Range');
		return;
	}
	now=start;
	next=now+100;
	if(next>maxid) next=maxid;

	document.getElementById('gen_info').innerHTML='Generating Html Files For Problem '+now+' To '+next;
	start_gen();
}
function start_gen() {
	Ajax.call('problems.php?act=gen_html&start='+now+'&end='+next,'',function(result){
		if(result.error) {
			alert(result.err_msg);
			return;
		}
		if(next>=maxid) return;
		now=next+1;
		next=now+100;
		if(next>maxid) next=maxid;
		document.getElementById('gen_info').innerHTML='Generating Html Files For Problem '+now+' To '+next;
		start_gen();
	},'GET','JSON');
}
{/literal}
</script>

<div class="form-div">
	Start ID:<input type="text" size="15" id="start" />
	End ID:<input type="text" size="15" id="end" />
	<input type="button" class="button" value="Gen Html" onclick="gen_html();"/>
</div>

<div class="list-div">
	<table cellpadding="3" cellspacing="1">
	<tr><td>There {$total} Problems. Min ID is {$range.minr}, Max ID is {$range.maxr}.</td></tr>
	<tr><td><div id="gen_info" style="color:red;font-size:14px;"><div></td></tr>
	</table>
</div>
{include file="pagefooter.tpl"}
