{include file="pageheader.tpl"}
{literal}
<script type="text/javascript">
	function rejudge_runId() {
		var runId = document.getElementById('runId').value;
		runId = parseInt(runId);
		Ajax.call('rejudge.php?act=rejudge_runId&runId='+runId,'', callback, 'GET', 'JSON');
	}

	function rejudge_contest() {
		var cid = document.getElementById('cid').value;
		cid = parseInt(cid);
		Ajax.call('rejudge.php?act=rejudge_contest&cid='+cid,'',callback, 'GET', 'JSON');
	}

	function callback(result) {
		if(result.error) {
			alert(result.err_msg);
			return;
		}
		alert("OK");
	}
</script>
{/literal}

<div class="form-div">
	Rejudge RunID:<input type="text" size="15" id="runId" />
	<input type="button" class="button" value="Rejudge" onclick="rejudge_runId()" />
</div>

<div class="form-div">
	Rejudge Contest ID:<input type="text" size="15" id="cid" />
	<input type="button" class="button" value="Rejudge" onclick="rejudge_contest()" />
</div>
{include file="pagefooter.tpl"}
