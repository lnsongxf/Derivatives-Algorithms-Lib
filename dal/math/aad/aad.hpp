/*
 * Modified by wegamekinglc on 2020/12/13.
 * Written by Antoine Savine in 2018
 * This code is the strict IP of Antoine Savine
 * License to use and alter this code for personal and commercial applications
 * is freely granted to any person or company who purchased a copy of the book
 * Modern Computational Finance: AAD and Parallel Simulations
 * Antoine Savine
 * Wiley, 2018
 * As long as this comment is preserved at the top of the file
 */

#pragma once

#ifdef AADET_ENABLED
#include <dal/math/aad/expr.hpp>
#else
#include <dal/math/aad/number.hpp>
#endif

namespace Dal {

    struct NumResultsResetterForAAD_ {
        ~NumResultsResetterForAAD_() {
            Tape_::multi_ = false;
            Node_::num_adj_ = 1;
        }
    };

    inline auto SetNumResultsForAAD(bool multi = false, const size_t& num_results = 1) {
        Tape_::multi_ = multi;
        Node_::num_adj_ = num_results;
        return std::make_unique<NumResultsResetterForAAD_>();
    }

    template <class IT_> inline void PutOnTape(IT_ begin, IT_ end) {
        std::for_each(begin, end, [](Number_& n) { n.PutOnTape(); });
    }

    template <class IT1_, class IT2_> inline void ConvertCollection(IT1_ src_begin, IT1_ src_end, IT2_ dest_begin) {
        using dest_type = std::remove_reference_t<decltype(*dest_begin)>;
        std::transform(src_begin, src_end, dest_begin, [](const auto& source) { return dest_type(source); });
    }

} // namespace Dal
