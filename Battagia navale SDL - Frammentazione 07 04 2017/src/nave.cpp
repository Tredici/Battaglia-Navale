#include "..\include\nave.h"

Nave::Nave(int l, bool oriz)
: length{l}, orizz{oriz}, prua{}
{
    if(l<1)
        throw std::logic_error{"Nave con dimensione impossibile"};
    parti = std::vector<bool>(length, false);
}


bool Nave::fuoco(Indice_cella i)
{
    if(orizz)
    {
        if(prua.riga==i.riga)
        {
            if(prua.colonna<=i.colonna&&prua.colonna+length>i.colonna)
            {
                parti[i.colonna-prua.colonna] = true;
                return true;
            }
        }
        else return false;
    }
    else//La nave è in verticale
    {
        if(prua.colonna==i.colonna)
        {
            if(prua.riga<=i.riga&&prua.riga+length>i.riga)
            {
                parti[i.riga-prua.riga] = true;
                return true;
            }
        }
        else return false;
    }
}

bool Nave::affondato() const
{
    for(bool x : parti)
        if( !x )
            return false;
    return true;
}

bool Nave::intercept(const Nave& n) const//Le due navi hanno una intercezione?
{
    Indice_cella ind = prua;
    for(int i=0; i<length; ++i)
    {
        if( n.cella_interna(ind) )
            return  true;
        if(orizz)
            ++ind.colonna;
        else
            ++ind.riga;
    }
    return false;
}


bool Nave::cella_interna(Indice_cella i) const//La cella in questione è interna
{
    if(orizz)
    {
        if(prua.riga==i.riga)
        {
            if(prua.colonna <= i.colonna && i.colonna < prua.colonna+length)
            {
                return true;
            }
        }
        return false;
    }
    //La nave è in verticale
    if(prua.colonna==i.colonna)
    {
        if( prua.riga <= i.riga && i.riga < prua.riga+length )
        {
            return true;
        }
    }
    return false;
}

Indice_cella Nave::cella_poppa() const
{
    Indice_cella cella = cella_prua();
    if( orizz )
        cella.colonna += length-1;
    else
        cella.riga += length-1;
    return cella;
}





