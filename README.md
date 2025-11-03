# br-ext-buildroot

Этот репозиторий содержит инструменты для сборки образов с помощью buildroot в контейнере Docker.

# Структура репозитория

Проект организован как внешний BR2_EXTERNAL для Buildroot — в корне лежат платы, конфиги и вспомогательные скрипты.

## Содержимое

- `Dockerfile` — образ на Ubuntu с предустановленным Buildroot.
- `run_docker.sh` — скрипт для сборки/пересборки/запуска контейнера с работчей директорией /opt/buildroot, также монтирует в корень /opt/buildroot/output/images - результаты сборки.

## Использование
```
chmod +x run_docker.sh 
```
```
./run_docker.sh
# Если нужно, то можно пересобрать ./run_docker.sh --rebuild
```

## Структура

```text
.
├── board/             # board support
├── configs/           # defconfig для buildroot
├── package/           # кастомные package
├── patches/           # патчи
├── host-scripts/      # опционально: вспомогательные скрипты
├── run_docker.sh      # скрипт запуска контейнера buildroot
└── Dockerfile         # образ окружения для сборки
```
