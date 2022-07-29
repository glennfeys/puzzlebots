#ifndef SYSPROG_BOTEXCEPTIONS_H
#define SYSPROG_BOTEXCEPTIONS_H

#include <stdexcept>
#include <ios>
#include <sstream>

class BoardError : public std::runtime_error {
public:
    explicit BoardError(const std::string & msg);
    explicit BoardError(const char * msg);
};

class BusError : public BoardError {
public:
    explicit BusError(const std::string & msg);
    explicit BusError(const char * msg);
};

class ModuleError : public BoardError {
public:
    explicit ModuleError(const std::string & msg);
    explicit ModuleError(const char * msg);
};

class ReadNotAllowedError : public ModuleError {
public:
    ReadNotAllowedError();
};

class WriteNotAllowedError : public ModuleError {
public:
    WriteNotAllowedError();
};


class CPUError : public BoardError {
public:
    explicit CPUError(const std::string & msg);
    explicit CPUError(const char * msg);
};


class DecodingError : public CPUError {
public:
    explicit DecodingError(const std::string & msg);
    explicit DecodingError(const char * msg);
};

template <class T>
std::string to_hex_string(T t)
{
    std::ostringstream oss;
    oss << std::hex << t;
    return oss.str();
}

#endif //SYSPROG_BOTEXCEPTIONS_H
