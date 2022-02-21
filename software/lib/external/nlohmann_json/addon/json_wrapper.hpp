/**
 ******************************************************************************
 * @file 	json_wrapper.hpp
 * @author 	Mateusz Maciąg <mateusz.maciag@embevity.com>
 * @date	2017-10-10

 * @brief   Wrapper for nlohmann::json enabling ordered maps
 ******************************************************************************
 */

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef SRC_PROTOCOL_JSON_JSON_WRAPPER_HPP_
#define SRC_PROTOCOL_JSON_JSON_WRAPPER_HPP_

/* Includes ---------------------------------------------------------------- */

#include <nlohmann/json.hpp>
#include <optional>

#include "fifo_map.hpp"

/**
 * @brief   Enable or disable stronly-ordered JSON.
 *
 *          Enable this macro if the JSON entries must appear
 *          in order as declared in the data serialization code.
 *          This is not required by JSON itself so the default
 *          JSON implementation does not provide such feature.
 *
 *          The hack is to use custom container as described
 *          here: https://github.com/nlohmann/json#notes
 *
 *          All the application code must use @c nlohmann::custom_json
 *          type instead of @c nlohmann::json
 */
#define USE_STRONGLY_ORDERED_JSON 1
namespace nlohmann
{

/* Constants and macros ---------------------------------------------------- */


/* Types ------------------------------------------------------------------- */

#if USE_STRONGLY_ORDERED_JSON

/**
 * @brief   Custom container used by N.Lohmann's JSON engine.
 */
template <class K, class V, class dummy_compare, class A>
using custom_map = fifo_map<K, V, fifo_map_compare<K>, A>;

/**
 * @brief   Custom JSON engine using FIFO ordered map.
 */
using custom_json = basic_json<custom_map>;


#else

using custom_json = json;

#endif

/* Exposed API ------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* JSON std::optional wrapper                                              */
/* ------------------------------------------------------------------------- */

template <typename T>
struct adl_serializer<std::optional<T>>
{
    static void to_json(json& j, const std::optional<T>& opt)
    {
        if (opt == std::nullopt)
        {
            j = nullptr;
        }
        else
        {
            j = *opt; // this will call adl_serializer<T>::to_json which will
                      // find the free function to_json in T's namespace!
        }
    }

    static void from_json(const json& j, std::optional<T>& opt)
    {
        if (j.is_null())
        {
            opt = std::nullopt;
        }
        else
        {
            opt = j.get<T>(); // same as above, but with
                              // adl_serializer<T>::from_json
        }
    }
};

} /* namespace nlohmann */

#endif /* SRC_PROTOCOL_JSON_JSON_WRAPPER_HPP_ */

/**** END OF FILE ****/
