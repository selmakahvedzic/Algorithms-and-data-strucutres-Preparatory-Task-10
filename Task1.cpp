#include <iostream>
#include <vector>
using namespace std;

template<typename TipOznake>
class Grana;


template<typename TipOznake>
class Cvor;


template<typename TipOznake>
class GranaIterator;



template<typename TipOznake>
class UsmjereniGraf
{
public:
    UsmjereniGraf(int brCvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int brCvorova)=0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina)=0;
    virtual void obrisiGranu(int polazni, int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni,float tezina)=0;
    virtual float dajTezinuGrane(int polazni, int dolazni) const=0;
    virtual bool postojiGrana(int polazni, int dolazni)=0;
    virtual void postaviOznakuCvora(int cvor, TipOznake o)=0;
    virtual TipOznake dajOznakuCvora(int cvor)const=0;
    virtual void postaviOznakuGrane(int polazni,int dolazni, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazni,int dolazni) const=0;

    Grana<TipOznake> dajGranu(int polazni, int dolazni)
    {
        return Grana<TipOznake>(this,polazni,dolazni);
    }
    Cvor<TipOznake> dajCvor(int c)
    {
        return Cvor<TipOznake> (this,c);
    }

    virtual GranaIterator<TipOznake> dajGranePocetak()=0;
    virtual GranaIterator<TipOznake> dajGraneKraj()=0;
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni)=0;
};


template<typename TipOznake>
class Grana
{
    UsmjereniGraf<TipOznake>* g;
    int polazni, dolazni;
public:
    Grana(UsmjereniGraf<TipOznake>* g, int polazni, int dolazni): g(g),polazni(polazni), dolazni(dolazni) {}
    float dajTezinu() const
    {
        return g->dajTezinuGrane(polazni,dolazni);
    }
    void postaviTezinu(float tezina)
    {
        g->postaviTezinuGrane(polazni,dolazni,tezina);
    }
    TipOznake dajOznaku() const
    {
        return g->dajOznakuGrane(polazni,dolazni);
    }
    void postaviOznaku(TipOznake oznaka)
    {
        g->postaviOznakuGrane(polazni, dolazni, oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor()
    {
        return g->dajCvor(polazni);
    }
    Cvor<TipOznake> dajDolazniCvor()
    {
        return g->dajCvor(dolazni);
    }
};


template<typename TipOznake>
class Cvor
{
    UsmjereniGraf<TipOznake>* g;
    int indeks;
public:
    Cvor(UsmjereniGraf<TipOznake>* g, int indeks): g(g), indeks(indeks) {}
    ~Cvor() {}
    TipOznake dajOznaku() const
    {
        return g->dajOznakuCvora(indeks);
    }
    void postaviOznaku(TipOznake oznaka)
    {
        g->postaviOznakuCvora(indeks, oznaka);
    }
    int dajRedniBroj() const
    {
        return indeks;
    }
};



template<typename TipOznake>
class MatricaGraf: public UsmjereniGraf<TipOznake>
{
    struct PodaciGrane {
        TipOznake oznaka;
        float tezina;
        bool postoji;
    };
    vector<vector<PodaciGrane>> mat;
    vector<TipOznake> oznake_cvorova;
public:
    ~MatricaGraf() {}

    MatricaGraf(int brCvorova): UsmjereniGraf<TipOznake>(brCvorova)
    {
        postaviBrojCvorova(brCvorova);
    }

    int dajBrojCvorova() const
    {
        return mat.size();
    }

    void postaviBrojCvorova(int brCvorova)
    {
        if(brCvorova<mat.size()) {
            throw "Ne može se smanjiti broj cvorova! ";
        }
        PodaciGrane nepostojeca;
        nepostojeca.postoji=false;
        for(int i=0; i<mat.size(); i++) {
            mat[i].resize(brCvorova,nepostojeca);
        }
        vector<PodaciGrane> prazanRed(brCvorova,nepostojeca);
        mat.resize(brCvorova,prazanRed);
        oznake_cvorova.resize(brCvorova);
    }


    void dodajGranu(int polazni, int dolazni,float tezina)
    {
        PodaciGrane g;
        g.tezina=tezina;
        g.postoji=true;
        mat[polazni][dolazni]=g;
    }


    void obrisiGranu(int polazni, int dolazni)
    {
        mat[polazni][dolazni].postoji=false;
    }


    void postaviTezinuGrane(int polazni, int dolazni, float tezina)
    {
        mat[polazni][dolazni].tezina=tezina;
    }


    float dajTezinuGrane(int polazni, int dolazni) const
    {
        return mat[polazni][dolazni].tezina;
    }


    bool postojiGrana(int polazni, int dolazni)
    {
        return mat[polazni][dolazni].postoji;
    }


    void postaviOznakuCvora(int cvor, TipOznake oznaka)
    {
        oznake_cvorova[cvor]=oznaka;
    }


    TipOznake dajOznakuCvora(int c) const
    {
        return oznake_cvorova[c];
    }


    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka)
    {
        mat[polazni][dolazni].oznaka=oznaka;
    }


    TipOznake dajOznakuGrane(int polazni, int dolazni) const
    {
        return mat[polazni][dolazni].oznaka;
    }


    Grana<TipOznake> dajGranu(int polazni, int dolazni)
    {
        return Grana<TipOznake>(this, polazni, dolazni);
    }


    Cvor<TipOznake> dajCvor(int cvor)
    {
        return Cvor<TipOznake> (this, cvor);
    }


    GranaIterator<TipOznake> dajSljedecuGranu(int polazni, int dolazni)
    {
        for(int i=polazni; i<mat.size(); i++) {
            for(int j=0; j<mat.size(); j++) {
                if(i==polazni && j<=dolazni) continue;
                if(mat[i][j].postoji) {
                    return GranaIterator<TipOznake>(this,i,j);
                }
            }
        }
        return GranaIterator<TipOznake>(this,-1,-1);
    }


    GranaIterator<TipOznake> dajGranePocetak()
    {
        GranaIterator<TipOznake> it(this,0,-1);
        return ++it;
    }


    GranaIterator<TipOznake> dajGraneKraj()
    {
        GranaIterator<TipOznake> it(this,-1,-1);
        return it;
    }
};

template<typename TipOznake>
class GranaIterator
{
    UsmjereniGraf<TipOznake>* g;
    int polazni, dolazni;
public:
    GranaIterator(UsmjereniGraf<TipOznake>* g, int polazni, int dolazni): g(g), polazni(polazni), dolazni(dolazni) {}

    Grana<TipOznake> operator* ()
    {
        return Grana<TipOznake> (g, polazni, dolazni);
    }


    bool operator==(const GranaIterator& it) const
    {
        return(g==it.g && polazni==it.polazni && dolazni==it.dolazni);
    }


    bool operator!=(const GranaIterator& it) const
    {
        return !(*this==it);
    }


    GranaIterator& operator++()
    {
        if(polazni==-1 && dolazni==-1) {
            throw "Iterator pokazuje iza kraja! ";
        }
        GranaIterator gr=(g->dajSljedecuGranu(polazni,dolazni));
        polazni=gr.polazni;
        dolazni=gr.dolazni;
        return *this;
    }


    GranaIterator operator++(int)
    {
        GranaIterator pom=this;
        ++(*this);
        return pom;
    }
};



int main()
{
    UsmjereniGraf<bool> *g=new MatricaGraf<bool>(6);
    g->dodajGranu(1,0,3.2);
    g->dodajGranu(0,1,2.3);
    g->dodajGranu(0,0,3.4);
    for(GranaIterator<bool> it = g->dajGranePocetak();
            it!=g->dajGraneKraj(); ++it) {
        cout<<"("<<(*it).dajPolazniCvor().dajRedniBroj()<<","<<
            (*it).dajDolazniCvor().dajRedniBroj()<<") -> "<<(*it).dajTezinu()<<"; ";
    }
    
    
    delete g;
    return 0;
}
