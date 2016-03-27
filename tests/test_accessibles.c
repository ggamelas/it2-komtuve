/*
 *   Ce fichier fait partie d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux
 *
 *   Copyright (C) 2015 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "automate.h"
#include "outils.h"


int test__accessibles(){

	int result = 1;

	Automate* automate = creer_automate();

	ajouter_etat( automate, 0);
	ajouter_etat( automate, 1);
	ajouter_etat( automate, 2);
	ajouter_etat( automate, 3);
	ajouter_etat( automate, 4);
	
	ajouter_etat_initial(automate, 0);
	ajouter_etat_initial(automate, 1);

	ajouter_transition( automate, 0, 'a', 2);
	ajouter_transition( automate, 1, 'b', 4);
	ajouter_transition( automate, 2, 'b', 3);

	Ensemble* ens1 = creer_ensemble( NULL, NULL, NULL);
	ajouter_element(ens1, 0);
	ajouter_element(ens1, 1);
	ajouter_element(ens1, 2);
	ajouter_element(ens1, 3);
	ajouter_element(ens1, 4); 

	TEST(
	     1
	     && automate
	     && comparer_ensemble( accessibles( automate), ens1) == 0 
	     , result
	     );

	liberer_ensemble(ens1);
	liberer_automate( automate );

	return result;
}


int main(){

	if( ! test__accessibles() ){ return 1; }

	return 0;
}
