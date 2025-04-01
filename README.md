# PPP - Moisés Antonio Pestano Castro

Proyecto de Desarrollo de aplicaciones multiplataforma de Moisés Antonio Pestano Castro.

2º DAM - [CIFP Villa de Agüimes](https://www3.gobiernodecanarias.org/medusa/edublog/cifpvilladeaguimes/) (curso 2024-2025)

## Contenidos
* [Estructura del proyecto](#estructura-del-proyecto)
* [Documentación](#documentación)
* [Fichero con la propuesta de desarrollo](https://docs.google.com/document/d/1ZHYAikIjYY18YQjFMotrN0AV_ZaOR0lpfGBYCWZqIyY/edit?usp=sharing)
* [Documentación de desarrollo](https://docs.google.com/document/d/1qw4rgYoQ6y2dBHWNrKZDMzAUaxLyvtd4ZV75yPZrpJs/edit?usp=sharing).
* [Cronograma del proyecto](https://docs.google.com/spreadsheets/d/1a-LlX0UmK2l04HHzsKKS6br2GuBsId14AkqSybdtL3A/edit?usp=sharing).
* [Repositorio GitHub](https://github.com/MoisesAPC/PPP).

## Estructura del proyecto
```
/PPP
  ├── build     # Ejecutable y artefactos de compilación.
  ├── docs      # Documentación (Doxygen)
  ├── examples  # Ejemplos de ficheros binarios de entrada validados por el programa.
  ├── include   # Ficheros de cabecera en C / C++.
  ├── src       # Código fuente en C / C++.
  ├── ui        # Archivos de diseño de la interfaz de Qt.
  ├── Doxygen   # Fichero de creación de la documentación con Doxygen.
  ├── PPP.pro   # Fichero de proyecto de Qt.
```

Si compilas el ejecutable utilizando QtCreator, el ejecutable y los respectivos DLL se encontrarán en los siguientes directorios:
* Debug:   ```\PPP\build\Desktop_Qt_6_8_2_MinGW_64_bit-Debug\debug```
* Release: ```\PPP\build\Desktop_Qt_6_8_2_MinGW_64_bit-Release\release```

## Documentación
La documentación se encuentra en [docs/html/index.html](docs/html/index.html).

Este proyecto fue desarrollado en C / C++ usando [Qt Creator](https://www.qt.io/product/development-tools), más específicamente:
```
Qt Creator 15.0.1
Based on Qt 6.8.1 (MSVC 2022, x86_64)

Built on Jan 22 2025 13:47:55
```
