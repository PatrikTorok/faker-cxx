#pragma once

#include <functional>
#include <span>

#include "Datatype.h"
#include "Number.h"

namespace faker
{
class Helper
{
public:
    /**
     * @brief Get a random element from an STL container.
     *
     * @tparam T an element type of the container.
     *
     * @param data The container.
     *
     * @return T a random element from the container.
     *
     * @code
     * Helper::arrayElement<char>(std::string{"abcd"}) // "b"
     * Helper::arrayElement<std::string>(std::vector<std::string>{{"hello"}, {"world"}}) // "hello"
     * @endcode
     */
    template <class T>
    static T arrayElement(std::span<const T> data)
    {
        const auto index = Number::integer<size_t>(data.size() - 1);

        return data[index];
    }

    /**
     * @brief Returns shuffled STL container.
     *
     * @tparam T an element type of the container.
     *
     * @param data The container.
     *
     * @return Container with shuffled elements.
     *
     * @code
     * Helper::arrayElement<char>(std::string{"abcd"}) // "dcba"
     * @endcode
     */
    template <class T>
    static std::vector<T> shuffle(std::vector<T> data)
    {
        std::shuffle(std::begin(data), std::end(data), pseudoRandomGenerator);

        return data;
    }

    /**
     * @brief Returns the given string parsed symbol by symbol and replaced the placeholders with digits ("0" - "9").
     * "!" will be replaced by digits >=2 ("2" - "9").
     *
     * @param str The template to parse string.
     *
     * @param symbol The symbol to replace with digits. Defaults to '#'.
     *
     * @return The string replaced symbols with digits.
     *
     * @code
     * Helper::replaceSymbolWithNumber() // ""
     * Helper::replaceSymbolWithNumber("#####") // "04812"
     * Helper::replaceSymbolWithNumber("!####") // "27378"
     * Helper::replaceSymbolWithNumber("Your pin is: !####") // "29841"
     * @endcode
     */
    static std::string replaceSymbolWithNumber(std::string str, const char& symbol = '#');

    /**
     * @brief Returns credit card schema with replaced symbols and patterns in a credit card  including Luhn checksum
     * This method supports both range patterns `[4-9]` as well as the patterns used by `replaceSymbolWithNumber()`.
     * `L` will be replaced with the appropriate Luhn checksum.
     *
     * @param inputString TThe credit card format pattern. Defaults to "6453-####-####-####-###L".
     *
     * @param symbol The symbol to replace with a digit. Defaults to '#'.
     *
     * @return The string replaced symbols with digits.
     *
     * @code
     * Helper::replaceCreditCardSymbols() // "6453-4876-8626-8995-3771"
     * Helper::replaceCreditCardSymbols("1234-[4-9]-##!!-L") // "1234-9-5298-2"
     * @endcode
     */
    static std::string replaceCreditCardSymbols(const std::string& inputString = "6453-####-####-####-###L",
                                                char symbol = '#');

    /**
     * @brief Returns the replaced regex-like expression in the string with matching values.
     *
     * Supported patterns:
     * - `.{times}` => Repeat the character exactly `times` times.
     * - `.{min,max}` => Repeat the character `min` to `max` times.
     * - `[min-max]` => Generate a number between min and max (inclusive).
     *
     * @param input The template string to to parse.
     *
     * @return The replaced regex-like expression in the string with matching values.
     *
     * @code
     * Helper::regexpStyleStringParse() // ""
     * Helper::regexpStyleStringParse("#{5}") // "#####"
     * Helper::regexpStyleStringParse("#{2,9}") // "#######"
     * Helper::regexpStyleStringParse("[500-15000]") // "8375"
     * Helper::regexpStyleStringParse("#{3}test[1-5]") // "###test3"
     * @endcode
     */
    static std::string regexpStyleStringParse(const std::string& input);

    /**
     * @brief Returns a random key from given object.
     *
     * @tparam T The type of the object to select from.
     *
     * @param object The object to be used.
     *
     * @throws  If the given object is empty
     *
     * @return A random key from given object.
     *
     * @code
     * std::unordered_map<int, std::string> testMap = {
     * {1, "one"},
     * {2, "two"},
     * {3, "three"}
     * };
     * Helper::objectKey(testMap) // "2"
     * @endcode
     */
    template <typename T>
    static typename T::key_type objectKey(const T& object)
    {
        if (object.empty())
        {
            throw std::runtime_error("Object is empty.");
        }

        std::vector<typename T::key_type> keys;
        for (const auto& entry : object)
        {
            keys.push_back(entry.first);
        }

        return arrayElement<typename T::key_type>(keys);
    }

    /**
     * @brief Returns the result of the callback if the probability check was successful, otherwise empty string.
     *
     *
     * @tparam TResult The type of result of the given callback.
     *
     * @param callback The callback to that will be invoked if the probability check was successful.
     * @param options.probability The probability (`[0.00, 1.00]`) of the callback being invoked. Defaults to `0.5`.
     *
     * @return The result of the callback if the probability check was successful, otherwise empty string.
     *
     * @code
     * Helper::maybe<std::string>([]() { return "Hello World!"; }) // ""
     * Helper::maybe<int>([]() { return 42; }, 0.9) // "42"
     * @endcode
     */
    template <typename TResult>
    static TResult maybe(std::function<TResult()> callback, double probability = 0.5)
    {
        if (Datatype::boolean(probability))
        {
            return callback();
        }
        return TResult();
    }

private:
    static std::random_device randomDevice;
    static std::mt19937 pseudoRandomGenerator;
};
}