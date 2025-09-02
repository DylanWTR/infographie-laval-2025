# Éditeur de scène OpenGL

> Projet développé dans le cadre du cours "IFT-3100 Infographie" à l'université Laval, est une application interactive pour constuire, modifier et rendre des scènes visuelles. 

## Dépendances du projet

- GLFW 
- GLAD 
- OpenGL 3.3+
- Dear ImGui
- Assimp
- STBImage

## Structure du projet

    /include       # Fichiers d'en-tête  
    /source        # Fichiers source  
    /glad          # Chargeur de fonction OpenGL  
    /imgui         # Bibliothèque - Interface utilisateur
    /shaders       # Fichiers de shader GLSL
    /resources     # Resources du projet (textures, polices...)

## Commandes Makefle

    make - Compiler le projet
    make clean - Supprimer les fichiers de compilation
    make flush - Supprimer les fichiers de compilation (conserve les dépendances)
    make fclean - Supprimer les binaires et les fichiers de compilation
    make re - Supprimer les fichiers de compilation et recompile le projet
    make debug - Compiler avec les symboles de débugguage
    make gdb - Éxécuter avec GDB
    make valgrind - Éxécuter avec valgrind

## Auteurs

Timothé MEDICO - Ali RAAD - Tom DESALMAND - Dylan WINTER
