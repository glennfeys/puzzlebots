#include "BotBerryExceptions.h"

BoardError::BoardError(const std::string &msg) : runtime_error(msg) {  }
BoardError::BoardError(const char *msg) : runtime_error(msg) {  }

BusError::BusError(const std::string &msg) : BoardError(msg) {  }
BusError::BusError(const char *msg) : BoardError(msg) {  }

ModuleError::ModuleError(const std::string &msg) : BoardError(msg) {  }
ModuleError::ModuleError(const char *msg) : BoardError(msg) {  }

ReadNotAllowedError::ReadNotAllowedError() : ModuleError("Read not allowed") {  }
WriteNotAllowedError::WriteNotAllowedError() : ModuleError("Write not allowed") {  }

CPUError::CPUError(const std::string &msg) : BoardError(msg) {  }
CPUError::CPUError(const char *msg) : BoardError(msg) {  }

DecodingError::DecodingError(const std::string &msg) : CPUError(msg) {  }
DecodingError::DecodingError(const char *msg) : CPUError(msg) {  }
