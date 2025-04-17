#pragma once
class TypeIdGenerator {
public:
    template<typename T>
    static std::size_t Get() {
        static const std::size_t id = GetNextId();
        return id;
    }

private:
    static std::size_t& Counter() {
        static std::size_t counter = 0;
        return counter;
    }

    static std::size_t GetNextId() {
        return Counter()++;
    }
};


