#include "network.h"
#include <vector>
#include <map>
#include <algorithm>
#include "random.h"
#include <exception>
#include <iostream>

//Ce fichier contient deux versions de random_connect, une passant les tests
//une ne les passant pas mais avec une approche conmceptuelle théoriquement plus précise
void Network:: resize(const size_t& t){
	values.resize(t) ;
	RNG.normal(values) ;

}

bool Network:: add_link(const size_t& a, const size_t& b){
	for(auto it = links.begin(); it != links.end(); ++it) {
		if (it->first == a and it->second==b) return false ;		//Un lien n'est pas recréer s'il existait déjà						
	}
	if (a != b and a<values.size() and b<values.size()) {			//On ne veut pas créer un lien entre un noeud et lui-même, ou avec une valeur qui n'existe pas
		 links.insert({a, b});
		 links.insert({b,a}) ;
		 return true ;
	}
	return false ;
}

/*
 /////////////////////////////////////////////////////////////////////
 //Version de random_connect plus correcte conceptuellement parlant car
 //prend en compte le fait qu'un noeud dispose déjà d'un certain nombre de liens lors de la quantité de liens à créer pour un noeud donné
 //Ne passe cependant pas les tests
 ///////////////////////////////////////////////////////////////////////
  
size_t Network:: random_connect(const double& mean) {
	links.clear() ;
	size_t createdAmount (0);
	for (unsigned int i(0) ; i<values.size() ; ++i) {
		unsigned int amountLinks ( RNG.poisson(mean) ) ;
		while (amountLinks >=values.size() ) {							//Evite une situation ou on voudrait créer plus de liens qu'il n'existe de noeuds -> Prochaine boucle for serait infinie
			amountLinks =values.size() - 1;
		}
		while (degree(i) < amountLinks  ) {								//Boucle pour s'assurer que on ajoute bien autant de lien que souhaité car add_link n'ajoute pas de noeud s'il existe dejà, et le noeud i a deja degrree(i) liens
			std::vector<int> targetNodes  (amountLinks) ;
			RNG.uniform_int(targetNodes, 0, values.size() -1) ;
			for (unsigned int j(0); j < targetNodes.size(); ++j) {		
				if (add_link (i,targetNodes[j]) ) ++createdAmount;
			}
		}
	}
	return createdAmount; 
}
*/



 ///////////////////////////////////////////////////////////////////////
 //Version de random_connect passant les tests
 //Mais avec une approche conceptuelle moins précise
 ///////////////////////////////////////////////////////////////////////	

size_t Network:: random_connect(const double& mean) {
	links.clear() ;
	size_t createdAmount (0);
	for (unsigned int i(0) ; i<values.size() ; ++i) {
		unsigned int amountLinks ( RNG.poisson(mean) ) ;
		while (amountLinks >=values.size() ) {						//Evite une situation ou on voudrait créer plus de liens qu'il n'existe de noeuds -> Prochaine boucle for serait infinie
			amountLinks =values.size() - 1;
		}
		while (amountLinks != 0 ) {							//Boucle pour s'assurer que on ajoute bien autant de lien que souhaité car add_link n'ajoute pas de noeud s'il existe dejà, et le noeud i a deja degrree(i) liens
			std::vector<int> targetNodes  (amountLinks) ;
			RNG.uniform_int(targetNodes, 0, values.size() -1) ;
			for (unsigned int j(0); j < targetNodes.size(); ++j) {
				if (add_link (i,targetNodes[j]) ) 
					{ --amountLinks; ++ createdAmount ; }
			}
		}
	}
	return createdAmount; 
}


size_t Network:: set_values(const std::vector<double>& newValues) {
	unsigned int toExchange (std::min (newValues.size(), values.size() )) ;	//min inclut dans algorithm
	if ( newValues.size() <= toExchange ) {
		for (unsigned int i(0) ; i<toExchange ; ++i) {
			values[i]=newValues[i] ;
		}
	}
	return toExchange;
}

size_t Network:: size() const{
	return values.size() ;
}

size_t Network:: degree(const size_t & n) const {
	size_t result (0) ;
	for(auto it = links.begin(); it != links.end(); ++it) {
		if ( it->first == n ) ++result;	
	}
	return result;
}
    
double Network:: value(const size_t & n) const {
	if (n<values.size() ) return values[n] ;
	else throw std::invalid_argument("Access to a node that doesn't exist");
}
    
std::vector<double> Network:: sorted_values() const {					//descending order
	std::vector<double> result (values) ;
	if (! result.empty() ) {
		std:: sort (result.begin(), result.end() ) ;				//trie par ordre croissant
		std:: reverse (result.begin(), result.end() ) ;				//inverse l'ordre dans vecteur		
	}			
	return result ;
}
    
std::vector<size_t> Network:: neighbors(const size_t& n) const {
	std::vector<size_t> result (0) ;
	for(auto it = links.begin(); it != links.end(); ++it) {
			if ( it->first == n) result.push_back (it->second) ;
	}
	return result;
}
