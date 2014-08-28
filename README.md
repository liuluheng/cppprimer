cppprimer
=========

cppprimer code

10. chapter 10
   * absInt.cc
     for_each transform with lambda
     ``` c++
     // pass a lambda to for_each to print each element in vi
     for_each(vi.begin(), vi.end(), [](int i) { cout << i << " "; });
     // replace negative values by their absolute value
     transform(vi.begin(), vi.end(), vi.begin(),
               [](int i) { return i < 0 ? -i : i; });
     ```
   * accum.cc
     use of fill fill_n accumulate
   * accum4.cc
     use of istream_iterator

   * avg_price.cc 
     `ostream_iterator<> out_iter(cout, "\n");`
   * lambad.cc
     - different use of lambda
   * newcount-size.cc
  
