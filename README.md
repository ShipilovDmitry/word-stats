# word-stats
Count word stats

Для запуска программы ее необходимо сначала скомпилировать. 

## Если в системе установлены cmake и g++
Из корня репозитория выполнить команды:

```sh
cmake -Bbuil-DCMAKE_BUILD_TYPE=Released
cmake --build build/
./build/bin/word-stats input.txt output.txt
```

## Если в системе есть Docker
Из корня репозитория выполнить команды:
```sh
docker build . -t word-stats
docker run -it word-stats 
./build/bin/word-stats ./build/bin/input.txt ./build/bin/output.txt
```

Файлы input.txt и output.txt нужно будет положить в контейнер. Сделать это можно в секции COPY Dockerfile, там есть соответствующие заготовки для этого