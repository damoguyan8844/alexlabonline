#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;


typedef struct __point
{
    __point()
    {
        x=0;
        y=0;
    }
    __point (long x_,long y_):x(x_),y(y_)
    {
    }

    //obsolete version , const is critical
    friend ostream & operator << ( ostream & ios , const __point  & pt );
    //also can be a member function as below
    //ostream & operator <<( ostream * out) const

    long x;
    long y;
}  point;


ostream & operator << ( ostream & ios , const point  & pt )
{
    return ios<<"("<<pt.x<<","<<pt.y<<")";
}

struct printx: public binary_function<int, int, int>
{
    int operator()(int & a, int b)const
    {
        long lngolda=a;
        a+=b;
        return lngolda+b;
    }

};


struct point_move: public binary_function<point , long , point >
{
    point   operator()(point a, long b) const
    {
        return point(a.x+b,a.y);
    }
};

int main_for_each_transform()
{
    vector<int> my;
    my.push_back(0);
    my.push_back(1);
    my.push_back(2);
    copy(my.begin(), my.end(), ostream_iterator<int>(cout, " "));
    cout<<"\n-----"<<endl;
    for_each(my.begin(),my.end(), bind2nd(printx() , 3) );
    cout<<"\n-----"<<endl;
    copy(my.begin(), my.end(), ostream_iterator<int>(cout, " "));
    cout<<"\n-----"<<endl;
    transform(my.begin(),my.end(),my.begin(), bind2nd(printx() , 3) );
    cout<<"\n-----"<<endl;
    copy(my.begin(), my.end(), ostream_iterator<int>(cout, " "));
    return 0;
}


int main_for_each_transform2()
{
    cout<<""<<endl;

    vector<point > vecP(2);
    //cout<<vecP[0]<<endl;
    std::copy(vecP.begin(),vecP.end(),ostream_iterator<point>(cout," ") );

    std::for_each(vecP.begin(),vecP.end(), std::bind2nd(point_move(),3));

    std::copy(vecP.begin(),vecP.end(),ostream_iterator<point>(cout," ") );

    std::transform(vecP.begin(),vecP.end(),vecP.begin(), std::bind2nd(point_move(),3));

    std::copy(vecP.begin(),vecP.end(),ostream_iterator<point>(cout," ") );

}
