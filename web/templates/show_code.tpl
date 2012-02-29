<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="Content-Language" content="en-US">
    <title>Source Code</title>
    <link type="text/css" rel="stylesheet" href="templates/sh_style.css">
    <script type="text/javascript" src="templates/js/sh_main.js"></script>
    <script type="text/javascript" src="templates/js/sh_{$info.ext}.js"></script>
</head>
<body lang="en-US" link="blue" alink="blue" vlink="blue" onload="sh_highlightDocument();">
    <p align="center"><font size="4" color="#333399">Source Code</font></p>
    <table align="center" style="font-size:10pt">
        <tr><td><b>Problem:</b> <a href="problem_html/{$info.problemId}.html">{$info.problemId}</a></td>
            <td width="10px"></td><td><b>User:</b> <a href="user.php?act=detail&authorId={$login_user.user_id}">{$login_user.username}</a></td>
        </tr>
        <tr><td><b>Memory:</b> {$info.memory}KB</td>
            <td width="10px"></td><td><b>Time:</b> {$info.time}MS</td>
        </tr>
        <tr><td><b>Language:</b> {$info.displayName}</td>
            <td width="10px"></td><td><b>Result:</b> <font color=red>{$info.judgeStatus}</font></td>
        </tr>
    </table>

    <ul><li><font color=#333399 size=5>Source Code</font></li>
    <pre class="sh_{$info.ext}">{$info.code}</pre>
    </ul>
</body>
</html>

