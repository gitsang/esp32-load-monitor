# Load Output

![](./res/image.jpg)

## Esp32

用 Arduino IDE，直接打开 ino 文件

在首选项中新增以下两个附加开发板管理网址

- https://github.com/espressif/arduino-esp32/releases/download/2.0.2/package_esp32_index.json
- https://github.com/espressif/arduino-esp32/releases/download/2.0.2/package_esp32_dev_index.json

在开发板管理器中下载 ESP32

选择 `DOIT ESP32 DEVKIT V1`（根据自己板子调整，一般板子上都有型号，选个差不多的就行）

DEVKITV1 这个板子上传时候需要在出现 Connecting 时长按 Boot 按键，每个板子的上传模式开启方式都不相同，需要根据型号到网上查找。

写入成功就可以从电脑上拔下接到充电宝之类的上面了。

### Esp32 接线

左侧电压表正极接 D13 端口，负极接 GND
右侧电压表正极接 D15 端口，负极接 GND

### Esp32 API

有两个 http api 都是 GET 方法，比较简单

`/left/{LValue}`: LValue 为 0 - 255，超出会循环。

可通过 curl、postman、浏览器等测试

```
curl http://192.168.5.39/left/100
```

左边的电压表应该输出 100/255*3.3 = 1.3v 

右侧的 API 与左侧相似

`/right/{RValue}`

## sys load output

负载输出这个就很简单了，获取负载然后转换成最大值是 255 的数字，再 GET API 就行了。

我是用 go 实现的，可以参考一下

