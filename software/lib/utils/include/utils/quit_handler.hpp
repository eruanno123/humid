/**
 ******************************************************************************
 * @file    quit_handler.hpp
 * @author  Mateusz Maciąg
 * @date    2020-11-02
 * @brief   Quit signal handler.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef QUIT_HANDLER_HPP_
#define QUIT_HANDLER_HPP_

/* Includes ---------------------------------------------------------------- */

#include <atomic>
#include <initializer_list>

/* Constants and macros ---------------------------------------------------- */
// clang-format off


// clang-format on

/* Types ------------------------------------------------------------------- */

namespace utils
{

/* Class ------------------------------------------------------------------- */

class QuitHandler
{
public:
    virtual ~QuitHandler() {}
    virtual bool quitRequested() const = 0;
    operator bool() const { return quitRequested(); }
};

/**
 * @brief   Simple application quit handler.
 *
 * Usage: create instance with required list of signals
 *
 *      SignalQuitHandler obj = { SIGINT, SIGTERM }
 *
 * Use object to detect quit request:
 *
 *      while(!obj) { ... process ... }
 */
class SignalQuitHandler : public QuitHandler
{
    static std::atomic_bool m_quit;
    static void handleSignal(int) { m_quit.store(true); }

    void init(std::initializer_list<int> siglist);

public:
    SignalQuitHandler();
    SignalQuitHandler(std::initializer_list<int> siglist) { init(siglist); }
    bool quitRequested() const override;
    operator bool() const { return quitRequested(); }
};

/* Exposed API ------------------------------------------------------------- */

} // namespace utils

#endif /* QUIT_HANDLER_H_ */

/**** END OF FILE ****/
