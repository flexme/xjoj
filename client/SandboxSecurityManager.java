/*
 * SandboxSecurityManager.java
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

import java.security.Permission;
import java.security.SecurityPermission;
import java.util.PropertyPermission;
import java.io.*;

public class SandboxSecurityManager extends SecurityManager {
    
    public static Thread targetThread;

    @Override
    public void checkPermission(Permission perm) {
        this.sandboxCheck(perm);
    }

    @Override
    public void checkPermission(Permission perm, Object context) {
        this.sandboxCheck(perm);
    }

    public void checkExit(int status) {
        return;
    }

    private void sandboxCheck(Permission perm) throws SecurityException {
        if(Thread.currentThread() == targetThread) {
            if(perm instanceof SecurityPermission) {
                if(perm.getName().startsWith("getProperty")) {
                    return;
                }
            } else if (perm instanceof PropertyPermission) {
                if(perm.getActions().equals("read")) {
                    return;
                }
            } else if (perm instanceof FilePermission) {

            }
            
            Sandbox.logError(perm.toString());
            Sandbox.halt(JudgeResult.RUNTIME_ERROR);

            throw new SecurityException(perm.toString());
        }
    }
}

