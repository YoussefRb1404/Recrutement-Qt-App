#include "loginc.h"



loginC::loginC(int id, int mdp)
{
    this->id = id;
    this->mdp = mdp;
    ;
}

// Méthodes setter
void loginC::setid(const int n) { id = n; }
void loginC::setmdp(const int n) { mdp = n; }

