#pragma once
#include <random>
struct MATH {
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float K_CONST =   8.9875517923e9f; // N·m²/C²
    static constexpr float E_CHARGE =  1.602176634e-19f; // C
    static constexpr float E_MASS =    9.10938356e-31f; // kg
    static constexpr float P_MASS =    1.67262192369e-27f; // kg
    static constexpr float N_MASS =    1.67492749804e-27f;
    static constexpr float P_CHARGE = E_CHARGE;
    static constexpr float N_CHARGE = 0.0f;


};

class NumberGenerator
{
protected:
    std::random_device rd;
    std::mt19937 gen;

    NumberGenerator()
            : gen(rd())
    {}
};


template<typename T>
class RealNumberGenerator : public NumberGenerator
{
private:
    std::uniform_real_distribution<T> dis;

public:
    RealNumberGenerator()
            : NumberGenerator()
            , dis(0.0f, 1.0f)
    {}

    // random_device is not copyable
    RealNumberGenerator(const RealNumberGenerator<T>& right)
            : NumberGenerator()
            , dis(right.dis)
    {}

    float get()
    {
        return dis(gen);
    }

    float getUnder(T max)
    {
        return get() * max;
    }

    float getRange(T min, T max)
    {
        return min + get() * (max - min);
    }

    float getRange(T width)
    {
        return getRange(-width * 0.5f, width * 0.5f);
    }
};


template<typename T>
class RNG
{
private:
    static RealNumberGenerator<T> gen;

public:
    static T get()
    {
        return gen.get();
    }

    static float getUnder(T max)
    {
        return gen.getUnder(max);
    }

    static uint64_t getUintUnder(uint64_t max)
    {
        return static_cast<uint64_t>(gen.getUnder(static_cast<float>(max) + 1.0f));
    }

    static float getRange(T min, T max)
    {
        return gen.getRange(min, max);
    }

    static float getRange(T width)
    {
        return gen.getRange(width);
    }

    static float getFullRange(T width)
    {
        return gen.getRange(static_cast<T>(2.0f) * width);
    }

    static bool proba(float threshold)
    {
        return get() < threshold;
    }
};

using RNGf = RNG<float>;

template<typename T>
RealNumberGenerator<T> RNG<T>::gen = RealNumberGenerator<T>();


template<typename T>
class IntegerNumberGenerator : public NumberGenerator
{
public:
    IntegerNumberGenerator()
            : NumberGenerator()
    {}

    // random_device is not copyable
    IntegerNumberGenerator(const IntegerNumberGenerator<T>& right)
            : NumberGenerator()
    {}

    T getUnder(T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, max);
        return dist(gen);
    }

    T getRange(T min, T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(gen);
    }
};


template<typename T>
class RNGi
{
private:
    static IntegerNumberGenerator<T> gen;

public:
    static T getUnder(T max)
    {
        return gen.getUnder(max);
    }

    static T getRange(T min, T max)
    {
        return gen.getRange(min, max);
    }
};

template<typename T>
IntegerNumberGenerator<T> RNGi<T>::gen;

using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;