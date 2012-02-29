<?php
/*
 *  Copyright (C) <2008>  <chenkun cs.kunchen@gmail.com>

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

class render {
    var $LATEX_PATH = "/usr/bin/latex";
    var $DVIPS_PATH = "/usr/bin/dvips";
    var $CONVERT_PATH = "/usr/bin/convert";

    var $TMP_DIR = "latex_tmp";

    var $CACHE_DIR = "latex_cache";

    function wrap($thunk) {
        return <<<EOS
\documentclass[10pt]{article}

% add additional packages here
\usepackage{amsmath}
\usepackage{amsfonts}
\usepackage{amssymb}
\usepackage{color}

\pagestyle{empty}
\begin{document}
\begin{displaymath}
$thunk
\end{displaymath}
\end{document}
EOS;
    }

    function render_latex($thunk, $hash) {
        $thunk = $this->wrap($thunk);

        $fp = fopen($this->TMP_DIR. "/$hash.tex", "w+");
        fputs($fp, $thunk);
        fclose($fp);

        $command = $this->LATEX_PATH . 
            " --interaction=nonstopmode -output-directory ". $this->TMP_DIR.
            " ". $this->TMP_DIR . "/$hash.tex";
        exec($command);

        $command = $this->DVIPS_PATH .
            " -E ". $this->TMP_DIR. "/$hash.dvi".
            " -o ". $this->TMP_DIR. "/$hash.ps";
        exec($command);

        $command = $this->CONVERT_PATH.
            " -density 120 ". $this->TMP_DIR. "/$hash.ps ". $this->TMP_DIR . "/$hash.png";
        exec($command);

        copy($this->TMP_DIR . "/$hash.png", $this->CACHE_DIR. "/$hash.png");
        $this->cleanup($hash);
    }

    function cleanup($hash) {
        unlink($this->TMP_DIR ."/$hash.tex");
        unlink($this->TMP_DIR ."/$hash.aux");
        unlink($this->TMP_DIR ."/$hash.log");
        unlink($this->TMP_DIR ."/$hash.dvi");
        unlink($this->TMP_DIR ."/$hash.ps");
        unlink($this->TMP_DIR ."/$hash.png");
    }
}
