dllchecker
==========

dllchecker for .dll extensions of sketchup ruby plug-in

Assuming you have a sketchup plug-in with following file structure,

      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name.rb
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\xxxx.rb
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\...
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\...
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\xxxx.rb
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\dll\xxxx.dll
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\dll\...
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\dll\...
      C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\dll\xxxx.dll

this extension includs two files
C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\dllChecker.dll
C:\Program Files (x86)\SketchUp\SketchUp 2013\Plugins\su_pluginn_name\dll\Dlls_Install.exe

It copys all xxx.dll files under "su_pluginn_name\dll" folder into your SketchUp installation folder, 
which is "C:\Program Files (x86)\SketchUp\SketchUp 2013" in this case.

In a .rb file it can use the extension as 

      require "su_pluginn_name/dllChecker"
      DllChecker::DllChecker.launchDllsInstaller($:[0]+"/su_pluginn_name/dll/", "Dlls_Install.exe")

This lauchs Dlls_Install.exe as Administrator to have the sufficient permission to do the copying.
The package contains a visual studio 2012 solution with two projects: 

      dllChecker (c++/.dll) and Dlls_Install (c#/wpf)

It uses ruby1.8.6 and swig 2.0.0+.

It is using the MIT license. 

The MIT License (MIT)

Copyright (c) 2013 ice.huli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

