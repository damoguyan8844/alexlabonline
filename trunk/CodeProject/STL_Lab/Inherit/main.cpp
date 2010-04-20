#include <iostream>
#include <complex>

using namespace std;

class Base
{
    public:
    virtual void f(int);
    virtual void f(double);
    virtual void g(int i=10);
};
void Base::f(int)
{
    cout<<"Base:f(int)"<<endl;
}
void Base::f(double)
{
    cout<<"Base:f(double)"<<endl;
}
void Base::g(int i)
{
    cout<<"Base:"<<i<<endl;
}

class Derived: public Base
{
 public:
    void f( complex<double>);
    void g(int i=20);
};

void Derived::f(complex<double> )
{
    cout<<"Derived:f(comples)"<<endl;
}
void Derived::g(int i)
{
    cout<<"Derived:"<<i<<endl;
}
int main()
{
    Base b;
    Derived d;
    Base * pb= new Derived;
    b.f(1.0);
    d.f(1.0);
    pb->f(1.0);
    b.g();
    d.g();
    pb->g();

    delete (Derived * )pb;
    return 0;
}
