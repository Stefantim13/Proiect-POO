#ifndef PLATFORM_EXCEPTION_H
#define PLATFORM_EXCEPTION_H

#include <exception>

class PlatformException : public std::exception {
public:
    virtual const char* what() const noexcept override;
};

class InvalidPlatformTypeException : public PlatformException {
public:
    const char* what() const noexcept override;
};

class FontLoadException : public std::exception {
public:
    const char* what() const noexcept override;
};

#endif // PLATFORM_EXCEPTION_H
