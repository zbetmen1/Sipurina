//Add node
void dodaj_cvor(cvor *t, int value, cvor *novi){
    if(!t){
        t = new cvor(value,0, 0, this);
        return;
    }
    if(value < t->kljuc()) {
        if(t->levi) {
           t->levi->dodaj_cvor(t->levi, value, novi);
        }
        else {
            t->levi = novi;
            novi->roditelj= t;
        }
    }
    else if(value > t->kljuc()) {
        if(t->desni) {
            t->desni->dodaj_cvor(t->desni, value, novi);
        }
        else {
            t->desni = novi;
            novi->roditelj= t;
        }
    }
}

//Delete node
void brisi_cvor(cvor* t, int value){
    if(t){
        if(value < t->kljuc)
            brisi_cvor(t->levi, value);
        else if(value > t->kljuc)
            brisi_cvor(t->desni, value);
        else{
            if(t->levi == nullptr && t->desni == nullptr){
                cvor* pom = t;
                if(t->roditelj->levi == t)
                    t->roditelj->levi = nullptr;
                else
                    t->roditelj->desni = nullptr;
                delete pom->m_line;
                delete pom;
            }
            else if(t->levi == nullptr) {
                cvor* pom = t;
                if(t->roditelj->levi == t){
                    t->roditelj->levi = t->desni;
                    t->desni->roditelj = t->roditelj;
                }
                else{
                    t->roditelj->desni = t->desni;
                    t->desni->roditelj = t->roditelj;
                }
                delete pom->m_line;
                delete pom;
            }else if(t->desni == nullptr){
                cvor* pom = t;
                if(t->roditelj->levi == t){
                    t->roditelj->levi = t->levi;
                    t->levi->roditelj = t->roditelj;
                }
                else{
                    t->roditelj->desni = t->levi;
                    t->levi->roditelj = t->roditelj;
                }
                delete pom->m_line;
                delete pom;
            }
            else{
                //nadji minimum desnog podstabla
                cvor* min = nadji_minimum(t->desni, 5000);
                //zameni kljuci
                int tmp = min->kljuc;
                min->kljuc = t->kljuc;
                t->kljuc = tmp;
                brisi_cvor(t->desni, value);
            }
        }
    }
}
cvor* cvor::nadji_minimum(cvor* e, int min){
    cvor* minimalni;
    while(1){
        if(e->kljuc() < min){
            minimalni = e;
            min = e->kljuc;
        }
        else
            e=e->levi;
        if(!e)
            break;
    }
    return minimalni;
}

cvor* VisualTree::nadji_cvor(int value, cvor*e)
{
    if(e){
        if(e->kljuc() < value){
            return nadji_cvor(value, e->desni());
        }
        else if(e->kljuc() > value){
            return nadji_cvor(value, e->levi());
        }
        else{
            return e;
        }
    }
    else return 0;
}