# br-ext-buildroot

Внешнее дерево Buildroot (BR2_EXTERNAL) для сборки образов embedded Linux в Docker контейнере.

## Концепция

Репозиторий организован как `BR2_EXTERNAL` для Buildroot — стандартный механизм расширения без модификации основного дерева Buildroot. Одно репо — несколько плат.

## Структура

```text
.
├── board/
│   └── rpi4/          # Raspberry Pi 4 — см. board/rpi4/README.md
├── configs/           # defconfig для каждой платы
├── Config.in          # обязателен для BR2_EXTERNAL (может быть пустым)
├── external.mk        # обязателен для BR2_EXTERNAL (может быть пустым)
├── external.desc      # имя и описание external дерева
├── Dockerfile         # сборочный контейнер Ubuntu + Buildroot
└── run_docker.sh      # запуск контейнера
```

## Быстрый старт

```bash
git clone git@github.com:nikitatm333/br-ext-buildroot.git
cd br-ext-buildroot
./run_docker.sh
```

Внутри контейнера:

```bash
make BR2_EXTERNAL=/br-ext-buildroot <board>_defconfig
make -j$(nproc)
```

Образ появится в `out/images/sdcard.img`.

## Поддерживаемые платы

| Плата | defconfig | Описание |
|-------|-----------|----------|
| Raspberry Pi 4 | `rpi4_companion_defconfig` | Companion device с дисплеем ST7735S — подробнее в [board/rpi4/README.md](board/rpi4/README.md) |

## Добавление новой платы

```bash
mkdir -p board/<board_name>/patches/linux
mkdir -p board/<board_name>/rootfs_overlay
# создать configs/<board_name>_defconfig
```

## Шпаргалка Buildroot

```bash
# загрузить конфиг платы (всегда первая команда после запуска контейнера)
make BR2_EXTERNAL=/br-ext-buildroot <board>_defconfig

make menuconfig          # конфиг системы
make linux-menuconfig    # конфиг ядра
make -j$(nproc)          # умная пересборка (только изменившееся)
make linux-rebuild       # пересобрать только ядро
make <pkg>-rebuild       # пересобрать конкретный пакет
make <pkg>-dirclean      # очистить пакет полностью

# сохранить конфиги
make savedefconfig BR2_DEFCONFIG=/br-ext-buildroot/configs/<board>_defconfig
make linux-savedefconfig
cp output/build/linux-custom/defconfig /br-ext-buildroot/board/<board>/linux.defconfig
```
