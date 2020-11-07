#ifndef CONSTANTS_INC
#define CONSTANTS_INC

namespace SendingCodes {
    enum Action {
        AUTHENTIFICATION,
        REGISTRATION,
    };

    enum ReturnCode {
        FAIL_AUTHENTIFICATION,
        FAIL_REGISTRATION,
        SUCCESS_REGISTRATION,
        SUCCESS_AUTHENTIFICATION
    };
}

#endif // CONSTANTS_INC
