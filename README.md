# C++ / OpenGL project template

## Things to know about his template

### GLCall

Always wrap your OpenGL calls in the GLCall(...) macro. It will add debug checks even if your computer doesn't support modern OpenGL debugging.
NB : You can control the look of the modern debugging messages in the Framework/GLDebugCallback.h file, as well as ignore some warnings

```
TODO : add example
```

### Hide UI

With CTRL + H

### Logging

Use Log::Info("some text {} {}", variable1ThatWillGoInTheCurlyBraces, variable2), Log::Warn or Log::Error (this one will also trigger a breakpoint)
Note that those logs will be removed in release builds. If you want some of them to stay, use Log::Release instead of Log

### imgui.ini

imgui.ini stores the layout of your UI. It is nice to have it on the repo, so that anyone cloning it will get a nice UI layout from the get go.

But you might want to do 

```
git update-index --assume-unchanged imgui.ini
```

to ignore it from your commits (it does change every time you move a window in your app, so basically it will be present in every commit), and only commit it once in a while when new windows are added for example.

## TODO list

* Finish this readme
* Test the template on Linux (and Mac)