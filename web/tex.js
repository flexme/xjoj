function gettex() {
    var c = document.getElementsByName('tex');
    for(var i = 0; i < c.length; i++) {
        c[i].innerHTML = "<img alt='fomula' src='" + TEX_PHP_SRC + "?tex_code=" + encodeURIComponent(c[i].innerHTML) + "'></img>";
    }
}
