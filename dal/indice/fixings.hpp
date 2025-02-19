//
// Created by wegam on 2022/1/20.
//

#pragma once

#include <dal/storage/storable.hpp>
#include <dal/time/datetime.hpp>
#include <map>

namespace Dal {
    class FixHistory_ {
    public:
        using vals_t = std::map<DateTime_, double>;

    private:
        vals_t vals_;

    public:
        FixHistory_(const vals_t& vals) : vals_(vals) {}
        double Find(const DateTime_& fix_time, bool quiet = false) const;
    };

    namespace FixHistory {
        const FixHistory_& Empty();
    }

    class Fixings_ : public Storable_ {
    public:
        using vals_t = std::map<DateTime_, double>;
        const vals_t vals_;
        Fixings_(const String_& index_name, const vals_t& vals = vals_t())
            : Storable_("Fixings", index_name), vals_(vals) {}
    };
} // namespace Dal
