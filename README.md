# Home - Project Civilization

> By Quentin Souvigent, Lucas Marquet, Violaine Bec and Antoine Coutant

This is a school project of four french students of the [ENSEA](<www.ensea.fr/>). During our last year of school, we were asked to adapt a board game into a video game. Thus, we choose to implement the game "Civilization: A New Dawn".

Develop branch build: [![Nightly build](https://github.com/NiskuT/Civilization/actions/workflows/nightly.yml/badge.svg?branch=develop)](https://github.com/NiskuT/Civilization/actions/workflows/nightly.yml)

## [Project report - here](https://niskut.github.io/Civilization/index.html)
<b>Documentation</b>, rules, code and report.


## Principle
This game is a strategy board game in which two to four players act as the leaders of history’s most memorable civilizations. Over the course of the game, they will expand their domains, gain new technologies, and build many of humanity’s greatest wonders. In the end, one civilization will rise above all others to leave its indelible mark upon history.

If you want to know more about the rules and the documentation of the game you can click [here](https://niskut.github.io/Civilization/index.html).
To read the rules, go fisrt on the tab called "List of pages" and then click on "rules".

<img src="./ressources/img/hud/hud.png">


## Constraints

Our build system must use [this template](<www.github.com/cbares/plt>).

Other constraints are:

* We must create an UML diagram of our project with [Dia](<www.dia-installer.de/>). This diagram is used to generate all the header files of the project thanks to the open source software "dia2code". Thus we should not create header files by ourselves.
* We must use SFML as graphic lirary.
* The game must be multiplayer with a server part.
* The game must implement 3 different types of AI.

## Build documentation

### Prerequisites

To build the documentation, you must install doxygen, graphviz, libpng-dev and dia.

From the root of the project, you can now build the site with:

```shell
doxygen docs/Doxyfile
```

Then, you can access the documentation from `html/index.html`

### Windows

To build our project on Windows, you must have installed <b>MinGW</b> with all its default libraries and it lpthread libraries. You must also have <b>CMake</b> and <b>git</b> installed on your machine.

Now go to the `Civilization\build` folder and type `cmake -G "MinGW Makefiles" ..`.  Finally, type `cmake --build .` to launch the project.


### Linux

To build our project on Linux, you must have installed <b>git</b>, <b>gcc</b>/<b>g++</b>, <b>cmake</b> and <b>libsfml-dev</b>. 

If you also want to build testing, you will need to install <b>libboost-dev</b>.
For building code coverage, add in addition to that <b>lcov</b> and <b>gcovr</b>.

Now go to the `Civilization\build` folder and type `cmake ..`. You can add the following options: <b>-DCMAKE_BUILD_TYPE=Release</b>, <b>-DBUILD_TESTING=ON</b> and <b>-DBUILD_CODE_COVERAGE=ON</b>. Finally, type `cmake --build .` to build the project. Then, you can start the programs located in build/bin.
