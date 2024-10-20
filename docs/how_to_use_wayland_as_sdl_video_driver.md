# 如何使用 wayland 作为 SDL 的 Video Driver

在 Linux 平台，默认使用 x11 作为 SDL 的 Video Driver。如果想使用 wayland，可以按下列步骤：


## 编译

```
SDL_VIDEODRIVER=wayland scons
```

## 运行

```
./bin/demoui
```

> 以下可选

## 使用 weston

* 安装 weston

```
sudo apt install weson
sudo apt install libwayland-dev libegl1-mesa libgles2-mesa
```

* 运行 weston

```
weston --socket=wayland-1
```

* 运行程序

```
export WAYLAND_DISPLAY=wayland-1
```

```
./bin/demoui
```


