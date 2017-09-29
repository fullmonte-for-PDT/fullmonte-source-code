#include "MemTraceLogger.hpp"

// On launch, write out previous tetra record unless it's the same tetra
void MemTraceLogger::eventLaunch(const Ray3,unsigned IDt,double)
{
    if (Nabs == (unsigned)-1)
        Nabs=0;
    else if (IDt_current != IDt)
    {
        logTetraHits(IDt_current,Nabs);
        Nabs=0;
    }

    IDt_current=IDt;
}

// At a boundary (no refractive index difference or matched boundary), write out previous tetra record
void MemTraceLogger::eventBoundary(const Point3,int,int,int IDte)
{
    logTetraHits(IDt_current,Nabs);
    IDt_current=IDte;
    Nabs=0;
}

// At interface, mark the next IDt but don't score it yet (we may reflect back into current tetra)
void MemTraceLogger::eventInterface(const Ray3,int,unsigned IDt)
{
    IDt_next=IDt;
}

// Record an absorption - IDt_current is always correct
void MemTraceLogger::eventAbsorb(const Point3 p,unsigned IDt,double,double)
{
    if (IDt != IDt_current)
        cerr << "LoggerMemTrace::eventAbsorb - Oops! IDt_current is not correct!" << endl;
    ++Nabs;
}

// In case of refraction, we have entered the new tetra recorded at the interface
void MemTraceLogger::eventRefract(const Point3,UVect3)
{
    logTetraHits(IDt_current,Nabs);
    Nabs=0;
    IDt_current=IDt_next;
}

// Record exit
void MemTraceLogger::eventExit(const Ray3,int IDf,double)
{
    if (IDf==IDf_last_exit)
        ++Nexit;
    else {
        if (IDf_last_exit != 0)
            logExit(IDf_last_exit,Nexit);
        Nexit=1;
        IDf_last_exit=IDf;
    }
}

void MemTraceLogger::logTetraHits(unsigned IDt,unsigned Nabs)
{
    TetraRecord tmp = { IDt,Nabs} ;
    Nabs=0;
}

void MemTraceLogger::logExit(int IDf,unsigned Nexit)
{
    ExitRecord tmp = {abs(IDf),Nexit};
    Nexit=0;
}
