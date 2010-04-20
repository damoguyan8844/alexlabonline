// name:example2_1.cpp
// alias:Rubish

#include <iostream>

using namespace std;

int compare(const void *arg1, const void *arg2);

void copypaste_samp1(void)
{
 const int max_size = 10;  // 数组允许元素的最大个数
 int num[max_size];   // 整型数组

 // 从标准输入设备读入整数，同时累计输入个数，
 // 直到输入的是非整型数据为止
 int n;
 for (n = 0; cin >> num[n]; n ++);

 // C标准库中的快速排序（quick-sort）函数
 qsort(num, n, sizeof(int), compare);

 // 将排序结果输出到标准输出设备
 for (int i = 0; i < n; i ++)
  cout << num[i] << "\n";
}

// 比较两个数的大小，
// 如果*(int *)arg1比*(int *)arg2小，则返回-1
// 如果*(int *)arg1比*(int *)arg2大，则返回1
// 如果*(int *)arg1等于*(int *)arg2，则返回0
int compare(const void *arg1, const void *arg2)
{
 return (*(int *)arg1 < *(int *)arg2) ? -1 :
   (*(int *)arg1 > *(int *)arg2) ? 1 : 0;
}

// name:example2_2.cpp
// alias:The first STL program

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void copypaste_samp2(void)
{
 vector<int> num;  // STL中的vector容器
 int element;

 // 从标准输入设备读入整数，
 // 直到输入的是非整型数据为止
 while (cin >> element)
  num.push_back(element);

 // STL中的排序算法
 sort(num.begin(), num.end());

 // 将排序结果输出到标准输出设备
 for (size_t i = 0; i < num.size(); i ++)
  cout << num[i] << "\n";
}


// name:example2_3.cpp
// alias:aesthetic version

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void copypaste_samp3(void)
{
 typedef vector<int>    int_vector;
 typedef istream_iterator<int>    istream_itr;
 typedef ostream_iterator<int>    ostream_itr;
 typedef back_insert_iterator< int_vector > back_ins_itr;

 // STL中的vector容器
 int_vector num;

 // 从标准输入设备读入整数，
 // 直到输入的是非整型数据为止
 copy(istream_itr(cin), istream_itr(), back_ins_itr(num));

 // STL中的排序算法
 sort(num.begin(), num.end());

 // 将排序结果输出到标准输出设备
 copy(num.begin(), num.end(), ostream_itr(cout, "\n"));
}


/*
int main(void)
{
    count<<"copypaste_samp3 is cool"<<endl;
}
*/
