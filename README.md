# You Are An Idiot

This is just a simple joke, which makes a lot of small moving windows on your screen. It originally appeared at <http://youareanidiot.org>, but it was removed later. You can still see the original version on Web Archive: <https://web.archive.org/web/20040804124508/http://www.youareanidiot.org/>. To try it, you need to enable Flash player and disable Pop-up Blocker.

Recently it was recreated on <https://youareanidiot.cc/>, you can watch the whole story of this website in this video.

I recreated this 'virus' as a Windows desktop app, which has the advantage of that you don't need to enable any pop-up blocker, it works just out-of-the-box.
By default, it opens new windows under the same program instance, so you can easily close it with Task manager. To make it a bit more annoying, you can run it from command line with `--dangerous` flag. That makes new windows open as new program, which completely bloats both your screen and Task manager and makes it effectively unclosable.

The principle is simple: when you try to close any of the windows (either clicking on the close button or pressing Alt+F4), it spawns a number of new windows. Soon it starts overloading your computer, which may then crash or freeze. It really doesn't do anything dangerous or harmful, it never touches any of your files, so you don't have to worry.

You can find the program's full source code in this repository if you are curious how it works. The source is in a single file (Idiot.cpp) and contains a lot of comments to describe what is going on.

![idiot](https://user-images.githubusercontent.com/83636769/124245323-2d470e80-db20-11eb-9631-8a525cf0bf5c.png)

**Warning:** Before launching this app, you should first know how to terminate it (through Task Manager).

**Disclaimer:** You use this software on your own responsibility, see License

# License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
