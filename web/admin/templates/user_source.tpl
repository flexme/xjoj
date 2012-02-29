{include file="pageheader.tpl"}

<link type="text/css" rel="stylesheet" href="../templates/sh_style.css">
<script type="text/javascript" src="../templates/js/sh_main.js"></script>
<script type="text/javascript" src="../templates/js/sh_cpp.js"></script>
<script type="text/javascript" src="../templates/js/sh_c.js"></script>
<script type="text/javascript" src="../templates/js/sh_java.js"></script>
<script type="text/javascript" src="../templates/js/sh_py.js"></script>
<div class="form-div">
    <form action="javascript:getSource()" name="getSource">
        <img src="images/icon_search.gif" width="26" height="22" border="0" alt="GET" />
        RunId<input type="text" name="runId" size="15" />
        <input type="submit" value="Get" class="button" />
        <div id="source" style="display:none;">
            <table align="center" style="font-size:10pt">
                <tr><td><b>Problem:</b> <a href="#" target="_blank" id="pid"></a></td>
                <td width="10px"></td><td><b>User:</b><a href="#" target="_blank" id="aid"></a></td>
                </tr>
                <tr><td><b>Memory:</b> <span id="memory"></span>KB</td>
                <td width="10px"></td><td><b>Time:</b> <span id="time"></span>MS</td>
                </tr>
                <tr><td><b>Language:</b> <span id="lang"></span></td>
                <td width="10px"></td><td><b>Result:</b> <font color=red id="judgeStatus"></font></td>
                </tr>
            </table>

            <pre class="" id="cc"></pre>
        </div>
    </form>
</div>

{literal}
<script type="text/javascript">
    function getSource() {
        var runId = document.forms['getSource'].elements['runId'].value;
        if(runId != parseInt(runId)) {
            alert('runId must be a integer!');
            return;
        }

        Ajax.run('user.php?act=get_source&runId=' + runId, null, function(result) {
            if(result.error) {
                alert(result.err_msg);
                return;
            }

            document.getElementById('source').style.display = "block";
            document.getElementById('pid').href = "../problem_html/" + result['content']['problemId'] + ".html";
            document.getElementById('pid').innerHTML = result['content']['problemId'];
            document.getElementById('aid').href = "../user.php?act=detail&authorId=" + result['content']['user_id'];
            document.getElementById('aid').innerHTML = result['content']['username'];
            document.getElementById('memory').innerHTML = result['content']['memory'];
            document.getElementById('time').innerHTML = result['content']['time'];
            document.getElementById('lang').innerHTML = result['content']['displayName'];
            document.getElementById('judgeStatus').innerHTML = result['content']['judgeStatus']; 
            document.getElementById('cc').innerHTML = result['content']['code'];
            document.getElementById('cc').className = "sh_" + result['content']['ext'];
            sh_highlightDocument();
        }, 'GET', 'JSON');

    }
</script>
{/literal}

{include file="pagefooter.tpl"}

