{include file="header.tpl"}
    {literal}
    <style type="text/css">
    p,pre{padding:0;margin:0;}
    pre{color:green;font-size:13pt;}
    #fu li{padding:5px 0;border-bottom:1px solid #EEEEEE;}
    .que{color:green;}
    .ans{color:red;}
    .blue{color:blue;}
    #status{list-style-type:none;}
    #status li,#cea li{border:none;}
    #cea {list-style-position:inside;padding-left:15px;}
    .sp{color:blue;font-weight:bold;}
    </style>
    {/literal}
    <div id="main-left">
        {include file="main_left.tpl"}
    </div>
    <div id="main-right">
        <div class="rectdiv">
            <div class="cat-head">::FAQ</div>
            <div style="text-align:left;padding:10px;">
            {literal}
            <ul id="fu" style="list-style-type:none;margin:0;padding:0;font-size:10pt;">
                <li>
                    <p><span class="que">Q</span>:<strong>本OJ能够支持哪些编译器，编译器的版本和编译选项是什么？</strong></p>
                    <p><span class="ans">A</span>:本OJ运行在Linux系统上，支持的编译器有GUN g++,GNU gcc以及Sun JDK 1.6，也就是你能够提交CPP,C和JAVA的源程序到OJ进行评测。具体的编译选项为：<br/>
                    GCC: <span class="blue">gcc -o p -ansi -O2 -w -lm -DONLINE_JUDGE p.c</span><br/>
                    G++: <span class="blue">g++ -o p -ansi -O2 -w -lm -DONLINE_JUDGE p.cpp</span><br/>
                    JAVA: <span class="blue">javac Main.java</span><br/>
                    编译器的版本为：<br/>
                    <span class="blue">gcc/g++ gcc 版本 4.2.4 (Ubuntu 4.2.4-1ubuntu1)</span><br/>
                    <span class="blue">java version "1.6.0_07"</span><br/>
					另外本OJ也支持不需要编译的动态语言Python。
                </li>
                <li>
                    <p><span class="que">Q</span>:<strong>我的程序的输入输出该如何处理？</strong>
                    <p id="ss"><span class="ans">A</span>:不需要特别处理，直接从标准输入(stdin)读入数据，将结果输出到标准输出(stdout)，不需要什么文件操作，也就是说你提交的程序在本机测试时，应该是从键盘读入数据的，结果输出到屏幕。如果你进行任何文件操作，将会得到"Runtime Error"的评测结果。为了方便理解，下面帖出 100题 A+B Problem 的C++, C, Java，Python程序：<br/>
					<div style="width:48%;float:left;">
                    <strong>C++:</strong><br/>
                    <pre>
#include &lt;iostream&gt;
using namespace std;
int main(){
    int a,b;
    while(cin &gt;&gt; a &gt;&gt; b)
    cout &lt&lt a+b &lt&lt endl;
    return 0;
}
                    </pre>
					</div>
					<div style="width:48%;float:left;">
                    <strong>C:</strong><br/>
                    <pre>
#include &lt;stdio.h&gt;
void main() {
    int a,b;
    while(scanf(&quot;%d %d&quot;,&a, &b) != EOF)
        printf(&quot%d\n&quot,a+b);
}
                    </pre>
					</div>
					<div style="clear:both;"></div>
                    <strong>Java:</strong><br/>
                    <pre>
import java.util.Scanner;

public class Main {
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		while (in.hasNextInt()) {
			int a = in.nextInt();
			int b = in.nextInt();
			System.out.println(a + b);
		}
	}
}
                    </pre>
					<strong>Python:</strong><br/>
					<pre>
import sys
while True:
	line = sys.stdin.readline()
	if line:
		p = line.split(" ")
		res = int(p[0]) + int(p[1])
		print res
	else:
		break
					</pre>
                </li>
                <li>
                    <p><span class="que">Q</span>:<strong>我提交了程序，在状态页面我的程序的JudgeStatus为XXXXX,这个是什么意思？</strong></p>
                    <p><span class="ans">A</span>:这个就是OJ评测你提交的程序的结果。它们所表示的状态如下：
                    <ul id="status">
                        <li><span class="sp">Waiting</span>：你的程序刚刚提交，正在等待OJ评测你的程序。</li>
                        <li><span class="sp">Compiling</span>：OJ正在编译你的程序。</li>
                        <li><span class="sp">Accepted</span>：OK!你的程序是正确的 ^_^。</li>
                        <li><span class="sp">Wrong Answer</span>：你的程序运行的结果和正确结果不同。状态页面CASE那一览显示的是你的程序在第几个样例上出错的。</li>
                        <li><span class="sp">Presentation Error</span>：你的程序运行的结果是正确的，但是格式和正确结果有点不一样。比如中间多了回车或者空格，请仔细检查程序的输出部分，离AC就差一点点啦！</li>
                        <li><span class="sp">Time Limit Exceeded</span>：你的程序运行时间太长了，超过了对应题目的时间限制。</li>
                        <li><span class="sp">Memory Limit Exceeded</span>：你的程序运行所用的内存太多了，超过了对应题目的限制。</li>
                        <li><span class="sp">Output Limit Exceeded</span>：你的程序输出了太多的内容了，超过了OJ的限制，请检查你的程序中是否有可能存在无限循环输出。</li>
                        <li><span class="sp">Floating Point Error</span>：Divide by 0，除0错误。</li>
                        <li><span class="sp">Segmentation Fault</span>：有2种情况可能导致次错误：
                            <ul style="list-style-position:inside;padding-left:15px;">
                                <li>buffer overflow -- 缓冲区溢出，也就是你访问了非法内存，比如你申请了一个大小为2的数组a，却访问了a[10000]，或者在C/C++中访问了空指针等等。</li>
                                <li>stack overflow -- 堆栈溢出，有可能你在c/c++程序的函数中申请了过大的空间，或者递归层次太多，导致堆栈溢出了，请记住堆栈的大小为8192KB.</li>
                            </ul>
                        </li>
                        <li><span class="sp">Runtime Error</span>：你的程序在运行的时候出错了，异常终止了，导致这种状态的情况有很多，一般都是一些非法操作，比如文件操作，Java中的网络操作等等。Java中Runtime Error的更详细的叙述见下。另外C/C++中越界访问函数中定义的数组也会导致Runtime Error。</li>
                        <li><span class="sp">Restricted Function</span>：你的程序调用了不该调用的函数，比如fork(),execv(),socket等等危险的函数，请牢记一点：在你的程序中只做与解决问题有关的事情，任何其他的操作都会被OJ拦截下来。</li>
                        <li><span class="sp">Compilation Error</span>：编译错误，如果你的程序在本机能够正常编译，请在下面的<font color="blue">编译出错怎么办</font>查看更详细的解决办法。
                        <li><span class="sp">Server Error</span>： 服务器出错了 @_@ ，请联系管理员，谢谢！</li>
                    </ul>
                </li>
                <li>
                    <p><span class="que">Q</span>:<strong>编译出错怎么办，在我的机器上编译以过啊？</strong></p>
                    <p><span class="ans">A</span>:如果你确定在你的机器上编译可以通过，但是提交到OJ后显示编译出错Compilation Error，请检查一下几点：
                        <ul id="cea">
                            <li>在C++中main函数必须要声明为int main(),并且最后要return 0,否则会导致编译错误。
                            <li>在Vistual C++中，for(int i=0;i...)这种语句中i的作用域被扩大，也就是在for循环外面i继续存在着，这是Vistual C++的一个Bug，标准C++中规定这种情况i的作用域为for循环体内。</li>
                            <li>itoa不属于ANSI，所以不要用这个函数。</li>
                            <li>假如你要用64位的整数类型，在VC中有<font color="red">__int64</font>这种类型，但是这不属于标准C++，你应该用<font color="red">long long</font>这种类型，long long的读入输出可以用cin,cout,用scanf,printf的话应该这样：<font color="red">scanf("%lld"...) printf("%lld...)</font></li>
                            <li>如果你是用的Java语言，请确保你的主类名为<span class="blue">public class Main</span>,并且包含"<span class="blue">public static void main(String[] args)</span>"这个方法，并且不要打包，即程序中不能包含<span class="blue">package</span>语句（某些IDE会自动生成这条语句）。</li>
                        </ul>
                </li>
                <li>
                    <p><span class="que">Q</span>:<strong>得到了Runtime Error是什么原因？</strong></p>
                    <p><span class="ans">A</span>:如果你是用的Java语言，请检查主类的定义以及main函数的定义（上面编译出错的地方已经详细说明），并且检查你的程序中是否使用了不该使用的类（文件操作，网络操作等等）。如果你用的其他的静态语言，请检查你的程序中是否进行了一些“越权”的操作，最有可能的是使用了被OJ禁止的系统调用，比如进程操作 fork，文件操作open等等。

            </ul>
            {/literal}
            </div>
        </div>
    </div>
    <div class="clear"></div>
</div>
{include file="footer.tpl"}

