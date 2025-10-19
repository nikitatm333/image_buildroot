# image_buildroot

Этот репозиторий содержит инструменты для сборки образов с помощью buildroot в контейнере Docker.

## Содержимое

- `Dockerfile` — образ на Ubuntu с предустановленным Buildroot.
- `run_docker.sh` — скрипт для сборки/пересборки/запуска контейнера с работчей директорией /opt/buildroot, также монтирует в корень /opt/buildroot/output - результаты сборки.

## Использование
```
chmod +x run_docker.sh 
```
```
./run_docker.sh
# Если нужно, то можно пересобрать ./run_docker.sh --rebuild
```


