// name:example2_1.cpp
// alias:Rubish

#include <iostream>

using namespace std;

int compare(const void *arg1, const void *arg2);

void copypaste_samp1(void)
{
 const int max_size = 10;  // ��������Ԫ�ص�������
 int num[max_size];   // ��������

 // �ӱ�׼�����豸����������ͬʱ�ۼ����������
 // ֱ��������Ƿ���������Ϊֹ
 int n;
 for (n = 0; cin >> num[n]; n ++);

 // C��׼���еĿ�������quick-sort������
 qsort(num, n, sizeof(int), compare);

 // ���������������׼����豸
 for (int i = 0; i < n; i ++)
  cout << num[i] << "\n";
}

// �Ƚ��������Ĵ�С��
// ���*(int *)arg1��*(int *)arg2С���򷵻�-1
// ���*(int *)arg1��*(int *)arg2���򷵻�1
// ���*(int *)arg1����*(int *)arg2���򷵻�0
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
 vector<int> num;  // STL�е�vector����
 int element;

 // �ӱ�׼�����豸����������
 // ֱ��������Ƿ���������Ϊֹ
 while (cin >> element)
  num.push_back(element);

 // STL�е������㷨
 sort(num.begin(), num.end());

 // ���������������׼����豸
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

 // STL�е�vector����
 int_vector num;

 // �ӱ�׼�����豸����������
 // ֱ��������Ƿ���������Ϊֹ
 copy(istream_itr(cin), istream_itr(), back_ins_itr(num));

 // STL�е������㷨
 sort(num.begin(), num.end());

 // ���������������׼����豸
 copy(num.begin(), num.end(), ostream_itr(cout, "\n"));
}


/*
int main(void)
{
    count<<"copypaste_samp3 is cool"<<endl;
}
*/
