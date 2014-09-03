cppprimer
=========

cppprimer code

10. chapter 10
   *    absInt.cc
        for_each transform with lambda

        ```C++
        // pass a lambda to for_each to print each element in vi
        for_each(vi.begin(), vi.end(), [](int i) { cout << i << " "; });
        // replace negative values by their absolute value
        transform(vi.begin(), vi.end(), vi.begin(),
                  [](int i) { return i < 0 ? -i : i; });
        ```

   *    accum.cc

        use of fill fill_n accumulate
   *    accum4.cc

        use of istream_iterator

   *    avg_price.cc 

        `ostream_iterator<> out_iter(cout, "\n");`

   *    bind2.cc

        ```C++
        for_each(words.begin(), words.end(),
                 bind(print, ref(cout), _1, ' '));
        ```

   *    lambad.cc

        - different use of lambda

   *    newcount-size.cc

   
   *    rcomma.cc

        ```
        rcomma.base()
        ```

   *    reverse.cc

        reverse reverse_copy remove_if remove_copy_if
        for_each find find_if

   *    reverse-iter.cc

        reverse_iterator rev_it.base()
   *    use_find.cc

        find

11. Chapter 11
   *    map1.cc

        map insert return `pair<map<string, size_t>::iterator, bool>`
        and could not store the same value(a pair)
  
   *    multimap.cc

        ```C++
        // definitions of authors and search_item as above
        // beg and end denote the range of elements for this author
        for (auto beg = authors.lower_bound(search_item),
                  end = authors.upper_bound(search_item);
             beg != end; ++beg)
          cout << beg->second << endl; // print each title

        // definitions of authors and search_item as above
        // pos holds iterators that denote the range of elements for this key
        for (auto pos = authors.equal_range(search_item);
             pos.first != pos.second; ++pos.first)
          cout << pos.first->second << endl; // print each title

        ```
