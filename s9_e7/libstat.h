#pragma once
#include <cmath>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace statistics {
    namespace aggregations {
        class Sum {
        public:
            void PutValue(double value);

            [[nodiscard]] std::optional<double> Get() const;

            static std::string_view GetValueName() {
                return "sum";
            }

        private:
            double sum_ = 0;
        };

        class Max {
        public:
            void PutValue(double value);

            [[nodiscard]] std::optional<double> Get() const;

            static std::string_view GetValueName() {
                return "max";
            }

        private:
            std::optional<double> cur_max_;
        };

        class Mean {
        public:
            void PutValue(double value);

            [[nodiscard]] std::optional<double> Get() const;

            static std::string_view GetValueName() {
                return "mean";
            }

        private:
            Sum sum_;
            size_t count_ = 0;
        };

        class StandardDeviation {
        public:
            void PutValue(double value);

            [[nodiscard]] std::optional<double> Get() const;

            static std::string_view GetValueName() {
                return "standard deviation";
            }

        private:
            Sum sum_;
            Sum sum_sq_;
            size_t count_ = 0;
        };

        class Mode {
        public:
            void PutValue(double value);

            [[nodiscard]] std::optional<double> Get() const;

            static std::string_view GetValueName() {
                return "mode";
            }

        private:
            std::unordered_map<double, size_t> counts_;
            std::optional<double> cur_max_;
            size_t cur_count_ = 0;
        };
    } // END OF NAMESPACE

    template <typename Aggreg>
    class AggregPrinter {
    public:
        void PutValue(double value) {
            inner_.PutValue(value);
        }

        void Print(std::ostream& out) const {
            auto val = inner_.Get();
            out << inner_.GetValueName() << " is ";
            if (val) {
                out << *val;
            } else {
                out << "undefined";
            }
            out << std::endl;
        }

    private:
        Aggreg inner_;
    };

    namespace tests {
        void AggregSum();
        void AggregMax();
        void AggregMean();
        void AggregStandardDeviation();
        void AggregMode();
        void AggregPrinter();
    } // END OF NAMESPACE
} // END OF NAMESPACE