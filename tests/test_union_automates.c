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

void wrap_liberer_automate( Automate * aut ){
	if( aut ){
		liberer_automate(aut);
	}
}

int test_automate_du_union(){

	int result = 1;

	{
		Automate * aut1 = mot_to_automate("a");
		Automate * aut2 = mot_to_automate("b");

		Automate * unio = creer_union_des_automates( aut1, aut2 );
		
		TEST(
			1
			&& unio
			&& le_mot_est_reconnu( unio, "a" )
			&& le_mot_est_reconnu( unio, "b" )
			&& ! le_mot_est_reconnu( unio, "" )
			&& ! le_mot_est_reconnu( unio, "aa" )
			&& ! le_mot_est_reconnu( unio, "bb" )
			&& ! le_mot_est_reconnu( unio, "aaa" )
			&& ! le_mot_est_reconnu( unio, "aab" )
			&& ! le_mot_est_reconnu( unio, "aba" )
			&& ! le_mot_est_reconnu( unio, "abb" )
			&& ! le_mot_est_reconnu( unio, "baa" )
			&& ! le_mot_est_reconnu( unio, "bab" )
			&& ! le_mot_est_reconnu( unio, "bba" )
			&& ! le_mot_est_reconnu( unio, "bbb" )
			, result
		);
		wrap_liberer_automate( aut1 );
		wrap_liberer_automate( aut2 );
		wrap_liberer_automate( unio );
	}


	{
		Automate * aut1 = creer_automate();

		ajouter_transition( aut1, 0, 'a', 1 );
		ajouter_transition( aut1, 1, 'b', 2 );
		ajouter_transition( aut1, 2, 'a', 2 );
		ajouter_transition( aut1, 2, 'b', 1 );
		ajouter_etat_initial( aut1, 0 );
		ajouter_etat_final( aut1, 1 );
		

		Automate * aut2 = creer_automate();

		ajouter_transition( aut2, 0, 'b', 1 );
		ajouter_transition( aut2, 1, 'a', 1 );
		ajouter_transition( aut2, 0, 'a', 0 );
		ajouter_transition( aut2, 1, 'b', 0 );
		ajouter_etat_initial( aut2, 0 );
		ajouter_etat_final( aut2, 1 );

		Automate * unio = creer_union_des_automates( aut1, aut2 );

		TEST(
			1
			&& unio
			&& ! le_mot_est_reconnu( unio, "" )
			&& le_mot_est_reconnu( unio, "a" )
			&& le_mot_est_reconnu( unio, "b" )

			&& ! le_mot_est_reconnu( unio, "aa" )
			&& le_mot_est_reconnu( unio, "ab" )
			&& le_mot_est_reconnu( unio, "ba" )
			&& ! le_mot_est_reconnu( unio, "bb" )

			&& ! le_mot_est_reconnu( unio, "aaa" )
			&& le_mot_est_reconnu( unio, "aab" )
			&& le_mot_est_reconnu( unio, "aba" )
			&& le_mot_est_reconnu( unio, "abb" )
			&& le_mot_est_reconnu( unio, "baa" )
			&& ! le_mot_est_reconnu( unio, "bab" )
			&& ! le_mot_est_reconnu( unio, "bba" )
			&& le_mot_est_reconnu( unio, "bbb" )

			&& ! le_mot_est_reconnu( unio, "aaaa" )
			&& le_mot_est_reconnu( unio, "aaab" )
			&& le_mot_est_reconnu( unio, "aaba" )
			&& ! le_mot_est_reconnu( unio, "aabb" )
			&& le_mot_est_reconnu( unio, "abaa" )
			&& le_mot_est_reconnu( unio, "abab" )
			&& ! le_mot_est_reconnu( unio, "abba" )
			&& le_mot_est_reconnu( unio, "abbb" )
			&& le_mot_est_reconnu( unio, "baaa" )
			&& ! le_mot_est_reconnu( unio, "baab" )
			&& ! le_mot_est_reconnu( unio, "baba" )
			&& le_mot_est_reconnu( unio, "babb" )
			&& ! le_mot_est_reconnu( unio, "bbaa" )
			&& le_mot_est_reconnu( unio, "bbab" )
			&& le_mot_est_reconnu( unio, "bbba" )
			&& ! le_mot_est_reconnu( unio, "bbbb" )
			, result
		);
		wrap_liberer_automate( aut1 );
		wrap_liberer_automate( aut2 );
		wrap_liberer_automate( unio );
	}
	
	{
		Automate * aut1 = creer_automate();

		ajouter_transition( aut1, 0, 'a', 1 );
		ajouter_transition( aut1, 1, 'b', 2 );
		ajouter_transition( aut1, 2, 'a', 2 );
		ajouter_transition( aut1, 2, 'b', 1 );
		ajouter_etat_initial( aut1, 0 );
		ajouter_etat_final( aut1, 1 );
		

		Automate * aut2 = creer_automate();

		ajouter_transition( aut2, 0, 'c', 1 );
		ajouter_transition( aut2, 1, 'd', 1 );
		ajouter_transition( aut2, 0, 'd', 0 );
		ajouter_transition( aut2, 1, 'e', 0 );
		ajouter_etat_initial( aut2, 0 );
		ajouter_etat_final( aut2, 1 );

		Automate * unio = creer_union_des_automates( aut1, aut2 );

		TEST(
			1
			&& unio
			&& ! le_mot_est_reconnu( unio, "" )
			&& le_mot_est_reconnu( unio, "a" )
			&& ! le_mot_est_reconnu( unio, "b" )
			&& le_mot_est_reconnu( unio, "c" )
			&& ! le_mot_est_reconnu( unio, "d" )
			&& ! le_mot_est_reconnu( unio, "e" )
			
			&& ! le_mot_est_reconnu( unio, "aa" )
			&& ! le_mot_est_reconnu( unio, "ab" )
			&& ! le_mot_est_reconnu( unio, "ac" )
			&& ! le_mot_est_reconnu( unio, "ad" )
			&& ! le_mot_est_reconnu( unio, "ae" )
			&& ! le_mot_est_reconnu( unio, "ba" )
			&& ! le_mot_est_reconnu( unio, "bb" )
			&& ! le_mot_est_reconnu( unio, "bc" )
			&& ! le_mot_est_reconnu( unio, "bd" )
			&& ! le_mot_est_reconnu( unio, "be" )
			&& ! le_mot_est_reconnu( unio, "ca" )
			&& ! le_mot_est_reconnu( unio, "cb" )
			&& ! le_mot_est_reconnu( unio, "cc" )
			&& le_mot_est_reconnu( unio, "cd" )
			&& ! le_mot_est_reconnu( unio, "ce" )
			&& ! le_mot_est_reconnu( unio, "da" )
			&& ! le_mot_est_reconnu( unio, "db" )
			&& le_mot_est_reconnu( unio, "dc" )
			&& ! le_mot_est_reconnu( unio, "dd" )
			&& ! le_mot_est_reconnu( unio, "de" )
			&& ! le_mot_est_reconnu( unio, "ea" )
			&& ! le_mot_est_reconnu( unio, "eb" )
			&& ! le_mot_est_reconnu( unio, "ec" )
			&& ! le_mot_est_reconnu( unio, "ed" )
			&& ! le_mot_est_reconnu( unio, "ee" )

			&& ! le_mot_est_reconnu( unio, "aaa" )
			&& ! le_mot_est_reconnu( unio, "aab" )
			&& ! le_mot_est_reconnu( unio, "aba" )
			&& le_mot_est_reconnu( unio, "abb" )
			&& ! le_mot_est_reconnu( unio, "cdc" )
			&& le_mot_est_reconnu( unio, "cdd" )
			&& ! le_mot_est_reconnu( unio, "cde" )
			&& le_mot_est_reconnu( unio, "cec" )
			&& ! le_mot_est_reconnu( unio, "ced" )
			&& ! le_mot_est_reconnu( unio, "cee" )
			&& ! le_mot_est_reconnu( unio, "dcc" )
			&& le_mot_est_reconnu( unio, "dcd" )
			&& ! le_mot_est_reconnu( unio, "dce" )
			&& le_mot_est_reconnu( unio, "ddc" )
			&& ! le_mot_est_reconnu( unio, "ddd" )
			&& ! le_mot_est_reconnu( unio, "dde" )

			&& ! le_mot_est_reconnu( unio, "aaaa" )
			&& ! le_mot_est_reconnu( unio, "aaab" )
			&& ! le_mot_est_reconnu( unio, "aaba" )
			&& ! le_mot_est_reconnu( unio, "aabb" )
			&& ! le_mot_est_reconnu( unio, "abaa" )
			&& le_mot_est_reconnu( unio, "abab" )
			&& ! le_mot_est_reconnu( unio, "abba" )
			&& ! le_mot_est_reconnu( unio, "abbb" )

			, result
		);
		wrap_liberer_automate( aut1 );
		wrap_liberer_automate( aut2 );
		wrap_liberer_automate( unio );
	}

	{

	  //Test union  automates non déterministe, non complet, plusieurs etats initiaux et finaux
	  Automate *a1 = creer_automate();
	  
	  ajouter_transition( a1, 0, 'a', 2 );
	  ajouter_transition( a1, 1, 'a', 0 );
	  ajouter_transition( a1, 1, 'b', 2 );
	  ajouter_transition( a1, 1, 'b', 4 );
	  ajouter_transition( a1, 2, 'a', 5 );
	  ajouter_transition( a1, 3, 'a', 4 );
	  ajouter_transition( a1, 4, 'a', 3 );
	  ajouter_transition( a1, 5, 'b', 4 );
	  ajouter_transition( a1, 5, 'b', 3 );
	  ajouter_etat_initial( a1, 0 );
	  ajouter_etat_initial( a1, 1 );
	  ajouter_etat_final( a1, 3 );
	  ajouter_etat_final( a1, 4 );
	  


	  Automate *a2 = creer_automate();
	  
	  ajouter_transition( a2, 0, 'b', 2 );
	  ajouter_transition( a2, 1, 'a', 6 );
	  ajouter_transition( a2, 2, 'a', 5 );
	  ajouter_transition( a2, 2, 'a', 3 );
	  ajouter_transition( a2, 3, 'a', 3 );
	  ajouter_transition( a2, 3, 'b', 2 );
	  ajouter_transition( a2, 6, 'b', 3 );
	  ajouter_transition( a2, 6, 'b', 6 );
	  ajouter_etat_initial( a2, 0 );
	  ajouter_etat_initial( a2, 1 );
	  ajouter_etat_final( a2, 5 );

	  Automate * unio = creer_union_des_automates( a1, a2 );
	  
	  TEST(
	       1
	       && unio
	       && ! le_mot_est_reconnu( unio, "" )
	       && le_mot_est_reconnu( unio, "abba" )
	       && le_mot_est_reconnu( unio, "baaaba" )
	       
	       && ! le_mot_est_reconnu( unio, "a" )
	       &&  le_mot_est_reconnu( unio, "b" )
	       
	       && le_mot_est_reconnu( unio, "aab" ) //reconu par a1 mais pas par a2
	       , result
	       );
	  
	  wrap_liberer_automate( a1 );
	  wrap_liberer_automate( a2 );
	  wrap_liberer_automate( unio );
	
	
	
	
	}

	return result;
}


int main(){
  
  if( ! test_automate_du_union() ){ return 1; }
  
  return 0;
}
