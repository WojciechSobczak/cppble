#include "scanner.hpp"

#include "windows/win_scanner.hpp"

namespace cppble
{

std::unique_ptr<scanner> scanner::create() {
#ifdef CPPBLE_WINDOWS_OS
    return std::make_unique<WinScanner>();
#else
#error "std::unique_ptr<scanner> scanner::create() is not supported on this platform:
#endif
}

}
