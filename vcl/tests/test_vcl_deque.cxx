#include <vcl/vcl_iostream.h>
#include <vcl/vcl_deque.h>

int main()
{
  typedef vcl_deque<int> mydeque;
  mydeque dq;
  
  dq.push_front(2);
  dq.push_back(3);
  dq.push_front(1);

  for(mydeque::iterator p = dq.begin(); p != dq.end(); ++p)
    cout << *p << endl;

  return 0;  
}
