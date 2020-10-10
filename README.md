# Django

## Things to know about his template

### GLCall

Always wrap your OpenGL calls in the GLCall(...) macro. It will add debug checks that might prove very usefull !

```
TODO : add example
```

### Hide UI

With CTRL + H

### Logging

Use Log::Info("some text {} {}", variable1ThatWillGoInTheCurlyBraces, variable2), Log::Warn or Log::Error (this one will also trigger a breakpoint)
Note that those logs will be removed in release builds. If you want some of them to stay, use Log::Release instead of Log