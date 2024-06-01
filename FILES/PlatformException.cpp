#include "PlatformException.h"

const char* PlatformException::what() const noexcept {
    return "Platform exception occurred.";
}

const char* InvalidPlatformTypeException::what() const noexcept {
    return "Invalid platform type.";
}

const char* FontLoadException::what() const noexcept {
    return "Error loading font";
}
