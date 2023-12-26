// #include <ZC/Tools/Signal/ZC_SignalConnection.h>

// ZC_SignalConnection::State ZC_SignalConnection::GetState() noexcept
// {
//     return state;
// }

// bool ZC_SignalConnection::Disconnect() noexcept
// {
//     if (state == State::Connected) state = State::Disconnected;
//     return state == State::Disconnected;
// }

// bool ZC_SignalConnection::Connect() noexcept
// {
//     if (state == State::Disconnected) state = State::Connected;
//     return state == State::Connected;
// }

// void ZC_SignalConnection::Terminate() noexcept
// {
//     state = State::Terminated;
// }