bin_PROGRAMS=judge
judge_SOURCES=compiler.cpp executer.cpp main.cpp config.cpp javaExecuter.cpp normalExecuter.cpp specialJudge.cpp defaultJudge.cpp traceProcess.cpp judgeThread.cpp downloadFile.cpp compileError.h disabled_syscall.h judge.h compiler.h error.h judgeStatus.h specialJudge.h config.h executer.h judgeThread.h sqlError.h defaultJudge.h javaExecuter.h normalExecuter.h traceProcess.h downloadFile.h
judge_LDADD=$(top_srcdir)/lib/libxjoj.a
judge_LDFLAGS=-D_GNU_SOURCE
INCLUDES=-I$(top_srcdir)/lib
DEFS+=-D_GNU_SOURCE
LIBS+=-lpthread -lz
EXTRA_DIST=makejava SandboxClassLoader.java Sandbox.h SandboxSecurityManager.java Sandbox.cpp Sandbox.java JudgeResult.java client_config.ini data client_log

