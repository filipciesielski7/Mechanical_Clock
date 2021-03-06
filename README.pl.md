<h1 align="center">
    Zegar Mechaniczny 馃暟
</h1>

_Dost臋pne r贸wnie偶 w wersji po: [English](README.md)_

## O projekcie

Animacja 3D zegara mechanicznego - projekt ko艅cowy z przedmiotu Grafika komputerowa &amp; Wizualizacja na Politechnice Pozna艅skiej.

![animowany gif z zegarem](./preview.gif)

Celem projektu by艂o zaimplementowanie animacji 3D przy u偶yciu [OpenGL](https://www.opengl.org//). W celu za艂adowanie modeli (w wi臋kszo艣ci wygenerowanych w [blenderze](https://www.blender.org/)) zosta艂a wykorzystana biblioteka assimp](https://github.com/assimp/assimp). Stworzony zegar wskazuje aktualn膮 godzin臋 systemow膮, a jego wahad艂o i ko艂a z臋bate pracuj膮 z odpowiedni膮 pr臋dko艣ci膮. Dodatkowo zosta艂 tutaj r贸wnie偶 zaimplementowany ruch kamer膮.

## Poruszanie

- &#8593;: ruch w prz贸d
- &#8595;: ruch w ty艂
- Page up / P, Page down / L, &#8592;, &#8594;: obr贸t kamery
- O: otwieranie i zamykanie drzwiczek zegara

## Uruchamianie

Projekt ten mo偶na uruchomi膰 zar贸wno na Linuxie, jak i Windowsie.

### Wymogi

W celu uruchomienia programu na systemie Linux, zainstalowane na nim musz膮 by膰 nast臋puj膮ce biblioteki:

- GLFW
- GLEW
- GLM
- Assimp

Je艣li chodzi o system Windows, w celu uruchomienia programu potrzebny b臋dzie [Visual Studio](https://visualstudio.microsoft.com/pl/).

### Linux

1. Klonowanie repozytorium
   ```sh
   git clone https://github.com/filipciesielski7/Mechanical_Clock.git
   ```
2. Przej艣cie do folderu _Linux_
   ```
   cd Mechanical_Clock/Linux
   ```
3. Uruchomienie poni偶szej komendy w tym folderze:
   ```
   make run
   ```

### Windows

W celu uruchomienia programu na Windowsie, naj艂atwiej jest otworzy膰 plik z folderu Windows o nazwie **pl_szkielet_03_win.sln** przy pomocy _Visual Studio_, a nast臋nie wcisn膮膰 przycisk **Lokalny debugger Windows** wcze艣niej wybieraj膮c przy nim opcje **x86**.
