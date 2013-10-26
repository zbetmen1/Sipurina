void lkd(cvor* e){
    if(e){
        lkd(e->levi);
		printf(" %d", e->kljuc);
        lkd(e->desni);
    }
}

void kld(cvor* e){
    if(e){
		printf(" %d", e->kljuc);
        kld(e->levi);
        kld(e->desni);
    }  
}

void ldk(cvor* e){
    if(e){
        ldk(e->levi);
        ldk(e->desni);
		printf(" %d", e->kljuc);
    }  
}