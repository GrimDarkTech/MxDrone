# MxDrone

Все команды выполняются в терминале/консоли

## Подготовка
### Windows

1. Загрузите и установите LLVM [Windowsx64](https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/LLVM-18.1.8-win64.exe)
2. Установите Visual Studio Community 2019 [ссылка](https://apps.microsoft.com/detail/xp8cdjnzkfm06w?hl=en-us&gl=RU)
3. Загрузите пакеты для разработки на С++ в Visual Studio Installer:
   - Основные компоненты C++;
   - MSVC (C++ средства сборки x64/x86);
   - C++ AddressSanitazer;
   - Средства профилирования С++;
   - IntelliCode
  
Тут могут быть лишние пункты, но у меня с ними работает :D

4. Загрузите и установите CMake [Windowsx64](https://github.com/Kitware/CMake/releases/download/v3.31.0/cmake-3.31.0-windows-x86_64.msi)

## Установка
1. Склонируйте репозиторий
    ```bash
    git clone --recurse-submodules https://github.com/asc-community/MxEngine
    ```
2. Перейдите в директорию проекта
    ```bash
    cd MxEngine
    ```
3. Отключите сборку примеров

   В директории **MxEngine** отредактируйте файл **CMakeLists.txt**:
   
   Найдите 47-ю строку и замените:
    ```cmake
    if (MXENGINE_BUILD_SAMPLES)
        add_subdirectory(samples/SandboxApplication)
        add_subdirectory(samples/OfflineRendererSample)
        add_subdirectory(samples/PhysicsSample)
        add_subdirectory(samples/ProjectTemplate)
        add_subdirectory(samples/SoundSample)
        add_subdirectory(samples/VRCameraSample)
        add_subdirectory(samples/GrassSample)
        add_subdirectory(samples/Sponza)
        add_subdirectory(samples/PBR)
        add_subdirectory(samples/PathTracing)
        add_subdirectory(samples/Particles)
        add_subdirectory(samples/Doom)
    ```
    на:
    ```cmake
    if (MXENGINE_BUILD_SAMPLES)
        add_subdirectory(Project/MxDrone)
    ```
4. Создайте папку **Project**
    ```bash
    mkdir Project
    ```
5. Положите в данную папку (Project) приложенную папку **MxDrone** с проктом, чтоб это выглядело вот так 
    ```bash
    Project/MxDrone
    ```
    (То есть, внутри папки Project должна находится папка MxDrone с проектом)

6. Снова вернитесь в корень проекта (в MxEngine)
    ```bash
    cd ..
    ```
7. Создайте папку **build** для сборки проекта
    ```bash
    mkdir build
    ```
8. Перейдите в папку **build**а 
    ```bash
    cd build
    ```
9. Выполните подготовку к сборке проекта, используя **CMake**
    ```bash
    cmake ..
    ```
    (В случае возникновения ошибок, читайте текст ошибки. Если ошибка в сабмодуле, возможно стоит его поставить вручную
    Для этого перейдите в папку submodules и откройте в браузере 
    
        https://github.com/asc-community/MxEngine/tree/master/submodules 
    
    и смотрите, на какой модуль ругается сборка. Находим нужный модуль на страничке, переходим. 
    В открывшемся сайте ищем зелёную кнопку CODE, там будет ссылка. Копируем её и в консоле выполняем команду
      
        git clone *ссылка* 
    ) 
10. Выполните сборку проекта, используя **CMake**
    ```bash
    cmake --build . --config Debug
    ```
