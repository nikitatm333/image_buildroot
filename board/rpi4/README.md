# Raspberry Pi 4 — Companion Device

Минимальный headless образ для RPi4 с дисплеем ST7735S на SPI.

## Железо

### Дисплей ST7735S (1.8", SPI)

```
ST7735S    →    RPi4 Pin    →    GPIO
VCC        →    Pin 1       →    3.3V
GND        →    Pin 6       →    GND
CLK        →    Pin 23      →    GPIO11 (SPI0_CLK)
SDA        →    Pin 19      →    GPIO10 (SPI0_MOSI)
RS (DC)    →    Pin 18      →    GPIO24
RST        →    Pin 22      →    GPIO25
CS         →    Pin 24      →    GPIO8  (SPI0_CE0)
```

Вторая дорожка на модуле (SD_CS, SD_MOSI, SD_SCL, SD_MISO) — SD карта, не используется.

## Структура

```text
board/rpi4/
├── config.txt              # конфиг загрузчика RPi
├── genimage.cfg.in         # шаблон разметки SD карты (boot 64M + rootfs 120M)
├── linux.defconfig         # конфиг ядра
├── post-image.sh           # скрипт финальной сборки образа
├── overlays/
│   ├── st7735r-nobl.dts    # исходник overlay для дисплея (без backlight)
│   └── st7735r-nobl.dtbo   # скомпилированный overlay
├── patches/linux/          # патчи ядра
└── rootfs_overlay/
    └── etc/
        ├── fstab           # автомонтирование /boot при старте
        └── network/
            └── interfaces  # статический IP eth0 → 10.0.0.1
```

## Сборка

```bash
./run_docker.sh

# в контейнере
make BR2_EXTERNAL=/br-ext-buildroot rpi4_companion_defconfig
make -j$(nproc)
```

## Прошивка

```bash
lsblk  # найти карту
sudo umount /dev/sdX1 /dev/sdX2 2>/dev/null
sudo dd if=out/images/sdcard.img of=/dev/sdX bs=4M status=progress
sudo sync
```

## Подключение

| Способ | Команда |
|--------|---------|
| SSH | `ssh root@10.0.0.1` |
| SCP на плату | `scp -O file root@10.0.0.1:/root/` |
| SCP с платы | `scp -O root@10.0.0.1:/path/file ./` |
| UART | `sudo minicom -D /dev/ttyUSB0 -b 115200` |

Поднять ethernet на хосте:

```bash
sudo nmcli device set enp42s0 managed no
sudo ip addr add 10.0.0.2/24 dev enp42s0
sudo ip link set enp42s0 up
```

## Что включено в образ

- SSH (dropbear) — без пароля
- Статический IP `10.0.0.1` на `eth0`
- SPI включён (`/dev/spidev0.0`, `/dev/spidev0.1`)
- I2C включён
- Дисплей ST7735S — `/dev/fb0`, `/dev/dri/card0`
- `/boot` монтируется автоматически

## Быстрое обновление ядра без перезаливки образа

```bash
# в контейнере
make linux-rebuild

# на хосте
scp -O out/images/Image root@10.0.0.1:/boot/Image
ssh root@10.0.0.1 reboot
```

## Проверка дисплея

```bash
# случайные цвета
cat /dev/urandom > /dev/fb0

# красный экран
printf '\x00\x00\xff\xff%.0s' $(seq 1 $((160*128))) > /dev/fb0

# чёрный экран
dd if=/dev/zero of=/dev/fb0
```

## Запуск модели на llama
```
/usr/local/bin/llama-simple-chat -m /root/models/qwen2.5-0.5b-instruct-q4_k_m.gguf
```
