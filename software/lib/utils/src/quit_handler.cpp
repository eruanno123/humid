/**
 ******************************************************************************
 * @file    quit_handler.cpp
 * @author  Mateusz Maciąg
 * @date    2020-11-02
 * @brief   Quit signal handler.
 ******************************************************************************
 */

/* Includes ---------------------------------------------------------------- */
#include "utils/quit_handler.hpp"

#include <signal.h>

namespace utils
{

std::atomic_bool SignalQuitHandler::m_quit{false};

SignalQuitHandler::SignalQuitHandler() : SignalQuitHandler({SIGINT, SIGTERM}) {}

void SignalQuitHandler::init(std::initializer_list<int> siglist)
{
    static struct sigaction sa = {};
    sa.sa_handler              = SignalQuitHandler::handleSignal;
    sigfillset(&sa.sa_mask);
    for (auto s : siglist)
    {
        sigaction(s, &sa, nullptr);
    }
}

bool SignalQuitHandler::quitRequested() const
{
    return m_quit;
}

} // namespace utils

/**** END OF FILE ****/
