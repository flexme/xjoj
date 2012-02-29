/* 
 * JudgeResult.java
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

public class JudgeResult {
    public final static int RUNTIME_ERROR = 1003 - 1000;

    public final static int TIME_LIMIT_EXCEEDED = 1006 - 1000;

    public final static int MEMORY_LIMIT_EXCEEDED = 1007 - 1000;

    public final static int OUTPUT_LIMIT_EXCEEDED = 1008 - 1000;

    public final static int FLOATING_POINT_ERROR = 1011 - 1000;

    public final static int SEGMENTATION_FAULT = 1012 - 1000;

    public final static int SERVER_ERROR = 1014 - 1000;

    public final static int RESTRICTED_FUNCTION = 1015 - 1000;
}

