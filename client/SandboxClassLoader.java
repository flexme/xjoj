/*
 * SandboxClassLoader.java
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

import java.io.File;
import java.io.FileInputStream;

class SandboxClassLoader extends ClassLoader {
    
    @Override
    public Class<?> loadClass(String name) throws ClassNotFoundException {
        //System.out.println("load class "+name);
        if(!isAllowedClassName(name)) {
            throw new ClassNotFoundException(name);
        }

        Class<?> ret = super.loadClass(name);
        if(Error.class.isAssignableFrom(ret)) {
            throw new ClassNotFoundException(name);
        }
        return ret;
    }

    @Override
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        //System.out.println("find class "+name);
        if(name.indexOf('.') >= 0) {
            throw new ClassNotFoundException(name);
        }
        File classFile = new File(name + ".class");
        byte[] classContent = new byte[(int) classFile.length()];
        try {
            FileInputStream in = new FileInputStream(classFile);
            in.read(classContent);
            in.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw new ClassNotFoundException(name);
        }
        return defineClass(name, classContent);
    }

    private boolean isAllowedClassName(String name) {
        if(name.startsWith("java.lang.")) {
            if(name.indexOf(',', 10) >= 0 || name.endsWith(".ClassNotFoundException")) {
                return false;
            }
        } else if (!name.startsWith("java.util.") && !name.startsWith("java.io.") &&
                !name.startsWith("java.text.") && !name.startsWith("java.net.") &&
                !name.startsWith("java.math.") && !name.startsWith("java.nio.") &&
                name.indexOf('.') >= 0) {
            return false;            
        }
        return true; 
    }

    private Class<?> defineClass(String name, byte[] b) {
        return this.defineClass(name, b, 0, b.length);
    }
}

