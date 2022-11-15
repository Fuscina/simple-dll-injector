<!-- Header-->
<br/>
<div align="center">
  <img width=200px height=200px src="https://i.imgur.com/69RvLup.png" alt="Logo"></a>
  <h3 align="center"><strong>Simple DLL Injector</strong></h3>

  <p align="center">
   <!-- n/a -->
  </p>
</div>

---

## Features

- Fast injection time.
- Command-line support.
- LoadLibrary for injection method.

## How to use

- ExeName = The name of the process you want to inject in to. Eg: Game.exe
- DLLName = The name of the dll you want to inject. Eg: Test.dll
- KeepOpen = If you want to keep the injector open after inject. Set to (true, 1, on) or (false, 0, off)

```cmd
start injector.exe ExeName DLLName KeepOpen
```

## Examples

- Number 1 (Normal CMD)
```cmd
start injector.exe Game.exe Test.dll (true or false)
```

- Number 2 (For powershell or terminal)
```cmd
start injector.exe { Game.exe Test.dll (true or false) }
```

<!-- Footer-->
<br/> 
<div align="center">
  <p align="center">
    Made with ❤️
  </p>
</div>
