/*
 * Sandbox.java 
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

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.management.*;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.GregorianCalendar;
import java.util.Scanner;
import java.net.Socket;

public class Sandbox {
   
    static {
        System.loadLibrary("sandbox");
    }
    
    private static int _time = 0;
    
    private static int _memory = 0;

    private static long _base_memory = 0;

    private static MemoryMXBean memoryBean = ManagementFactory.getMemoryMXBean();
    
    private static ThreadMXBean threadBean = ManagementFactory.getThreadMXBean();

    private static int port;

    private static int limitTime;

    private static int limitMemory;

    private static int limitOutput;

    private static int uid;

    private static int gid;

    private static String inputFile;

    private static String outputFile;

    private static DataOutputStream out;

    private static Socket socket;

    private static native int setLimits(int timeLimit, int outputLimit, int fileLimit, int uid, int gid);

    private static native void closeLog();
    
    public static native void logError(String message);

    private static Thread targetThread = new Thread() {
        
        private SandboxSecurityManager sandboxSecurityManager = new SandboxSecurityManager();

        private Object[] targetArguments = new Object[] { new String[0] };

        private Method mainMethod = initTargetMainMethod();

        public void run() {
            closeLog();
            if (setLimits(limitTime, limitOutput, 16, uid, gid) < 0) {
                logError("set limits error");
                halt(JudgeResult.SERVER_ERROR);
            }

            mainMethod.setAccessible(true);
            System.setSecurityManager(sandboxSecurityManager);
            sandboxSecurityManager.targetThread = this;

            try {
                mainMethod.invoke(null, targetArguments);
                System.out.close();
                SandboxSecurityManager.targetThread = null;
                updateStatus();
            } catch (InvocationTargetException e) {
                SandboxSecurityManager.targetThread = null;
                Throwable targetException = e.getTargetException();
                logError(printError(targetException));

                if(targetException instanceof OutOfMemoryError) {
                    _memory = limitMemory + 1;
                    halt(JudgeResult.MEMORY_LIMIT_EXCEEDED);
                } else {
                    halt(JudgeResult.RUNTIME_ERROR);
                }
            } catch (Exception e) {
                SandboxSecurityManager.targetThread = null;
                logError(printError(e));
                halt(JudgeResult.SERVER_ERROR);
            }
        }
    };

    private static Method initTargetMainMethod() {
        SandboxClassLoader sandboxClassLoader = new SandboxClassLoader();
        Class<?> targetClass = null;

        try {
            for (File f : new File(".").listFiles()) {
                String name = f.getName();
                if(name.endsWith(".class")) {
                    Class<?> c = Class.forName(name.substring(0, name.length() - 6), false, sandboxClassLoader);
                    if(name.equals("Main.class")) {
                        targetClass = c;
                    }
                }
            }
        } catch (ClassNotFoundException e) {
            logError(printError(e));
            halt(JudgeResult.RUNTIME_ERROR);
        } catch (NoClassDefFoundError e) {
            logError(printError(e));
            halt(JudgeResult.RUNTIME_ERROR);
        } catch (ClassFormatError e) {
            logError(printError(e));
            halt(JudgeResult.SERVER_ERROR);
        } catch (ExceptionInInitializerError e) {
            logError(printError(e));
            halt(JudgeResult.RUNTIME_ERROR);
        } catch (LinkageError e) {
            logError(printError(e));
            halt(JudgeResult.RUNTIME_ERROR);
        }

        if(targetClass == null) {
            logError("No Main.class found");
            halt(JudgeResult.RUNTIME_ERROR);
        }

        Method mainMethod = null;

        try {
            mainMethod = targetClass.getMethod("main", String[].class);
        } catch (NoSuchMethodException e) {
            logError("no main found");
            halt(JudgeResult.RUNTIME_ERROR);
        } catch (NoClassDefFoundError e) {
            logError(printError(e));
            halt(JudgeResult.RUNTIME_ERROR);
        }

        if(!Modifier.isStatic(mainMethod.getModifiers())) {
            logError("main is not static");
            halt(JudgeResult.RUNTIME_ERROR);
        }

        return mainMethod;
    }

    public static void main(String[] args) {
        if(args.length != 8) {
            logError("Invalid args length: " + args.length);
            halt(JudgeResult.SERVER_ERROR);
        }

        GregorianCalendar gc = new GregorianCalendar();

        try {
            Scanner scan = new Scanner(args[0]);
            port = scan.nextInt();
            limitTime = Integer.parseInt(args[1]);
            limitMemory = Integer.parseInt(args[2]);
            limitOutput = Integer.parseInt(args[3]);
            uid = Integer.parseInt(args[4]);
            gid = Integer.parseInt(args[5]);
            inputFile = args[6];
            outputFile = args[7];

            socket = new Socket("127.0.0.1", port);
            out = new DataOutputStream(socket.getOutputStream());

            System.setIn(new BufferedInputStream(new FileInputStream(inputFile)));
            System.setOut(new PrintStream(new BufferedOutputStream(new FileOutputStream(outputFile) {
                public void write(int b) throws IOException {
                    try {
                        super.write(b);
                    } catch (IOException e) {
                        if(e.getMessage().equals("File too large")) {
                            SandboxSecurityManager.targetThread = null;
                            halt(JudgeResult.OUTPUT_LIMIT_EXCEEDED);
                        }
                        throw e;
                    }
                }

                public void write(byte[] b, int off, int len) throws IOException {
                    try {
                        super.write(b, off, len);
                    } catch (IOException e) {
                        if(e.getMessage().equals("File too large")) {
                            SandboxSecurityManager.targetThread = null;
                            halt(JudgeResult.OUTPUT_LIMIT_EXCEEDED);
                        }
                        throw e;
                    }
                }
            })));

            System.setErr(new PrintStream(new BufferedOutputStream(new FileOutputStream("/dev/null"))));
        } catch (Exception e) {
            logError(printError(e));
            halt(JudgeResult.SERVER_ERROR);
            return;
        }

        threadBean.setThreadCpuTimeEnabled(true);

        System.gc();
        _base_memory = memoryBean.getHeapMemoryUsage().getUsed();

        targetThread.start();
        
        for( ; ;) {
            Thread.State state;
            ThreadInfo info = threadBean.getThreadInfo(targetThread.getId());
            if(info == null) {
                state = Thread.State.TERMINATED;
            } else {
                state = info.getThreadState();
            }
            if(state == Thread.State.RUNNABLE || state == Thread.State.NEW || state == Thread.State.TERMINATED) {
                updateStatus();
                try {
                    sendRunningInfo();
                } catch (IOException e) {
                    halt(JudgeResult.SERVER_ERROR);
                }
                if(state == Thread.State.TERMINATED) {
                    break;
                }
            } else if(SandboxSecurityManager.targetThread == null) {
                logError("Invalid thread state " + state);
                halt(JudgeResult.RUNTIME_ERROR);
            }

            try {
                targetThread.join(1000);
            } catch (InterruptedException e) {
                Runtime.getRuntime().halt(0);
                break;
            }
        }
        closeSocket();
    }

    private static String printError(Throwable e) {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        e.printStackTrace(pw);
        return sw.toString();
    }

    public synchronized static void halt(int result) {
        updateStatus();
        try {
            sendRunningInfo();
        } catch (IOException e) {
        
        }
        closeSocket();
        Runtime.getRuntime().halt(result);
    }

    private synchronized static void updateStatus() {
        int m = (int) ((memoryBean.getHeapMemoryUsage().getUsed() - _base_memory) / 1000);

        if(m > _memory) {
            _memory = m;
        }

        if(targetThread != null) {
            long t = threadBean.getThreadCpuTime(targetThread.getId());
            if(t >= 0) {
                t /= 1000000;
                if(t > _time && t <= 1000000) {
                    _time = (int)t;
                }
            }
        }
    }

    private synchronized static void sendRunningInfo() throws IOException {
        if(out != null) {
            out.writeInt(_time);
            out.writeInt(_memory);
            out.flush();
        }
    }

    private static void closeSocket() {
        try {
            if(socket != null) {
                socket.close();
                socket = null;
            }
        } catch (IOException e) {
        
        }
    }
}

