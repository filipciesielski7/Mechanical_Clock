<h1 align="center">
    Zegar Mechaniczny 🕰
</h1>

_Dostępne również w wersji po: [English](README.md)_

## O projekcie

Animacja 3D zegara mechanicznego - projekt końcowy z przedmiotu Grafika komputerowa &amp; Wizualizacja na Politechnice Poznańskiej.

![animowany gif z zegarem](./preview.gif)

Celem projektu było zaimplementowanie animacji 3D przy użyciu [OpenGL](https://www.opengl.org//). W celu załadowanie modeli (w większości wygenerowanych w [blenderze](https://www.blender.org/)) została wykorzystana biblioteka assimp](https://github.com/assimp/assimp). Stworzony zegar wskazuje aktualną godzinę systemową, a jego wahadło i koła zębate pracują z odpowiednią prędkością. Dodatkowo został tutaj również zaimplementowany ruch kamerą.

## Poruszanie

- &#8593;: ruch w przód
- &#8595;: ruch w tył
- Page up / P, Page down / L, &#8592;, &#8594;: obrót kamery
- O: otwieranie i zamykanie drzwiczek zegara

## Uruchamianie

Projekt ten można uruchomić zarówno na Linuxie, jak i Windowsie.

### Wymogi

W celu uruchomienia programu na systemie Linux, zainstalowane na nim muszą być następujące biblioteki:

- GLFW
- GLEW
- GLM
- Assimp

Jeśli chodzi o system Windows, w celu uruchomienia programu potrzebny będzie [Visual Studio](https://visualstudio.microsoft.com/pl/).

### Linux

1. Klonowanie repozytorium
   ```sh
   git clone https://github.com/filipciesielski7/Mechanical_Clock.git
   ```
2. Przejście do folderu _Linux_
   ```
   cd Mechanical_Clock/Linux
   ```
3. Uruchomienie poniższej komendy w tym folderze:
   ```
   make run
   ```

### Windows

W celu uruchomienia programu na Windowsie, najłatwiej jest otworzyć plik z folderu Windows o nazwie **pl_szkielet_03_win.sln** przy pomocy _Visual Studio_, a nastęnie wcisnąć przycisk **Lokalny debugger Windows** wcześniej wybierając przy nim opcje **x86**.
