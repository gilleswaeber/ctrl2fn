ctrl2fn
=======

A [Linux Interception Tools'](https://gitlab.com/interception/linux/tools) plugin to
change the right <kbd>Ctrl</kbd> key so that the arrow keys act as <kbd>Home</kbd>/<kbd>End</kbd>/<kbd>PgUp</kbd>/<kbd>PgDn</kbd> when pressed.
The right <kbd>Ctrl</kbd> key still acts normally when used with other keys.

Combinations work, e.g. <kbd>Shift</kbd>+<kbd>RCtrl</kbd>+<kbd>→</kbd> = <kbd>Shift</kbd>+<kbd>End</kbd>, and <kbd>LCtrl</kbd>+<kbd>RCtrl</kbd>+<kbd>←</kbd>  = <kbd>Ctrl</kbd>+<kbd>Home</kbd>.

The <kbd>fn</kbd> key is not a real key. It is usually implemented directly by the keyboard hardware.
This only changes the behavior of the arrow keys when pressed to simulate a <kbd>fn</kbd> in those cases.

Some keyboards seem not to register both <kbd>Ctrl</kbd> keys when pressed simultaneously.
This is a hardware problem.

## Installation
This depends on the [Interception Tools](https://gitlab.com/interception/linux/tools) and can be used on any Linux with Wayland, X11 or in the terminal.

Build:
```sh
git clone git@github.com:gilleswaeber/ctrl2fn.git
cd ctrl2fn
cmake -Bbuild
cmake --build build
```

Install (into `/usr/local/bin`):
```sh
sudo cmake --install build
```

## Execution
**ctrl2fn** is an [Interception Tools](https://gitlab.com/interception/linux/tools) plugin.
A suggested `udevmon` job configuration is:

```yaml
- JOB: intercept -g $DEVNODE | ctrl2fn | uinput -d $DEVNODE
  DEVICE:
    EVENTS:
      EV_KEY: [KEY_RIGHTCTRL, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN]
```

When using the daemon, the above goes in `/etc/interception/udevmon.yaml`.

## Thanks
The C code is based on the [space2meta](https://gitlab.com/interception/linux/plugins/space2meta) plugin by Francisco Lopes da Silva, released under MIT license.

## License
Available under MIT License, © 2021 Gilles Waeber

## Other platforms
AutoHotkey user? This is similar to:
```ahk
>^Left::Send {Home}
+>^Left::Send +{Home}
!>^Left::Send !{Home}
<^>^Left::Send ^{Home}
+<^>^Left::Send +^{Home}
>^Right::Send {End}
+>^Right::Send +{End}
!>^Right::Send !{End}
<^>^Right::Send ^{End}
+<^>^Right::Send +^{End}
>^Down::Send {PgDn}
+>^Down::Send +{PgDn}
!>^Down::Send !{PgDn}
<^>^Down::Send ^{PgDn}
+<^>^Down::Send +^{PgDn}
>^Up::Send {PgUp}
+>^Up::Send +{PgUp}
!>^Up::Send !{PgUp}
<^>^Up::Send ^{PgUp}
+<^>^Up::Send +^{PgUp}
```
