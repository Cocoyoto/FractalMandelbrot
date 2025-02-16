//MADE WITH OLC PIXEL GAME ENGINE
// https://github.com/OneLoneCoder/olcPixelGameEngine

#define OLC_PGE_APPLICATION
#include "Mandelbrot.h"

int main()
{
	// zoom avec le clic gauche ou la molette vers le haut
	// dezoom avec le clic droit ou la molette vers le bas
	// deplacement avec les fleches directionnelles
	// quitter avec la touche echap
	// reset avec la touche R
	// afficher la fractale de julia avec la touche J
	// masquer la fractale de julia avec la touche H
	// changer le nombre d'iterations avec les touches + et - du pavé numerique
	Mandelbrot mendel;
	if (mendel.Construct(800,800,1,1,false,true))
	{
		mendel.Start();
	}

	return 0;
}