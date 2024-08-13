# Sysboard
sysboard is a simple virtual keyboard (On screen keyboard) for wayland written in gtkmm 4<br>
![preview](https://github.com/System64fumo/sysboard/blob/main/preview.png "preview")

# Configuration
sysboard can be configured in 3 ways<br>
1: By changing config.hpp and recompiling (Suckless style)<br>
2: Using a config file (~/.config/sys64/board/config.conf)<br>
3: Using launch arguments<br>
```
arguments:
  -m	Set margin
  -H	Set height multiplier
  -l	Set layout (full, mobile, mobile_numbers)
  -v	Prints version info
```

# Signals
While the keyboard can show up on it's own, Sometimes you might want to trigger it manually<br>
You can send signals to show/hide the window<br>
``pkill -10 sysboard`` to show<br>
``pkill -12 sysboard`` to hide<br>
``pkill -34 sysboard`` to toggle<br>
Note: When triggered manually it will not hide on it's own<br>

# Theming
sysboard uses your gtk4 theme by default, However it can be also load custom css,<br>
Just copy the included style.css file to ~/.config/sys64/board/style.css<br>

# Also check out
[wvkbd](https://github.com/jjsullivan5196/wvkbd)<br>
[wf-osk](https://github.com/WayfireWM/wf-osk)<br>
