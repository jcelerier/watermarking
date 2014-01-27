watermarking
============

Projet watermarking 

Pour compiler, il est préférable d'utiliser QtCreator.

Dépendances :

- Qt5.2
- FFTW3
- libsndfile

Avec QtCreator, ouvrir src/GUI/InterfaceWatermarking/InterfaceWatermarking.pro et compiler.
Sinon, directement avec qmake : 

    $ cd src/GUI/InterfaceWatermarking 
    $ qmake
    $ make
    $ cd ../../../output
    $ ./InterfaceWatermarking

De même pour les tests dans le dossier src/lib_testapp produisant l'exécutable ./lib_testapp

