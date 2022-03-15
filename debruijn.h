#if !defined  HAVE_DEBRUIJN_H__
#define       HAVE_DEBRUIJN_H__
// This file is part of the FXT library.
// Copyright (C) 2010, 2012, 2014 Joerg Arndt
// License: GNU General Public License version 3 or later,
// see the file COPYING.txt in the main directory.


/// !!!
/// This file is based on the source code from the FXT library
/// written by Joerg Arndt.
/// https://www.jjj.de/fxt/

namespace fxt
{
    typedef unsigned long ulong;

// If defined, arrays are used instead of pointers, this gives a small speedup:
//#define NECKLACE_MAX_ARRAY_LEN 96  // default off

    // Generate necklaces, iterative algorithm.
    class necklace
    {
    public:
#ifndef NECKLACE_MAX_ARRAY_LEN
        ulong* a_;  // the string, NOTE: one-based
        ulong* dv_;  // delta sequence of divisors of n
#else
        ulong a_[NECKLACE_MAX_ARRAY_LEN + 1];
        ulong dv_[NECKLACE_MAX_ARRAY_LEN + 1];
#endif
        ulong n_;   // length of strings
        ulong m1_;  // m-ary strings, m1=m-1
        ulong j_;   // period of the word (if necklaces)

        necklace(const necklace&) = delete;
        necklace& operator=(const necklace&) = delete;
        necklace(necklace&&) noexcept = default;

    public:
        explicit necklace(ulong m, ulong n)
        {
            n_ = (n ? n : 1);  // at least 1
            m1_ = (m > 1 ? m - 1 : 1);  // at least 2
#ifndef NECKLACE_MAX_ARRAY_LEN
            a_ = new ulong[n_ + 1];
            dv_ = new ulong[n_ + 1];
#endif
            for (ulong j = 1; j <= n; ++j)
            { dv_[j] = (0 == (n_ % j)); }  // divisors
            first();
        }

        ~necklace()
        {
#ifndef NECKLACE_MAX_ARRAY_LEN
            delete[] a_;
            delete[] dv_;
#endif
        }

        void first()
        {
            for (ulong j = 0; j <= n_; ++j)
            { a_[j] = 0; }
            j_ = 1;
        }

        const ulong* data() const
        {
            return a_ + 1;
        }

        ulong next_pre()  // next pre-necklace
        // return j (zero when finished)
        {
            // Find rightmost digit that can be incremented:
            ulong j = n_;
            while (a_[j] == m1_)
            { --j; }

            // Increment:
            // if ( 0==j_ )   return 0;  // last
            ++a_[j];

            // Copy periodically:
            for (ulong k = j + 1; k <= n_; ++k)
            { a_[k] = a_[k - j]; }

            j_ = j;
            return j;
        }

        bool is_necklace() const
        {
            return (0 != dv_[j_]);  // whether j divides n
        }

        bool is_lyn() const
        {
            return (j_ == n_);  // whether j equals n
        }

        ulong next()  // next necklace
        {
            do
            {
                next_pre();
                if (0 == j_)
                { return 0; }
            }
            while (0 == dv_[j_]);  // until j divides n
            return j_;
        }

        ulong next_lyn()  // next Lyndon word
        {
            do
            {
                next_pre();
                if (0 == j_)
                { return 0; }
            }
            while (j_ == n_);  // until j equals n
            return j_;  // == n
        }
    };

    // Lexicographic minimal De Bruijn sequence.
    class debruijn : public necklace
    {
    public:
        ulong i_;   // position of current digit in current string

    public:
        explicit debruijn(ulong m, ulong n)
            : necklace(m, n)
        {
            first_string();
        }

        debruijn(debruijn&&) noexcept = default;

        ~debruijn()
        {
            ;
        }

        ulong first_string()
        {
            necklace::first();
            i_ = 1;
            return j_;
        }

        ulong next_string()  // make new string, return its length
        {
            necklace::next();
            i_ = (j_ != 0);
            return j_;
        }

        ulong next_digit()
        // Return current digit and move to next digit.
        // Return m if previous was last.
        {
            if (i_ == 0)
            { return necklace::m1_ + 1; }
            ulong d = a_[i_];
            if (i_ == j_)
            { next_string(); }
            else
            { ++i_; }
            return d;
        }

        ulong first_digit()
        {
            first_string();
            return next_digit();
        }
    };
}

// -------------------------


#endif  // !defined HAVE_DEBRUIJN_H__
