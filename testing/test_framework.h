/*
 * test_framework.h
 *
 * A tiny, self-contained unit-testing framework for CS15 exam practice.
 * You should NOT need to edit this file.
 *
 *  - Every TEST runs in its own child process, so a segfault or an infinite
 *    loop in one test is reported as CRASH / TIMEOUT and the remaining tests
 *    still run.
 *  - Run only some tests by passing one or more substrings:
 *        ./build/al1 pushAtBack insertAt
 *  - Flags:
 *        --list        list test names and exit
 *        --no-fork     run every test in this process (use with lldb/gdb/valgrind)
 *        --timeout=N   per-test time limit in seconds (default 5)
 *  - Memory tracking: every `new T[n]` / `delete [] p` in the program is
 *    counted, so tests can detect leaked arrays (see liveArrayCount()).
 *
 * IMPORTANT: include this header in exactly ONE .cpp file per executable
 * (the *_tests.cpp file). It defines main() and the global operator new[].
 */
#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cxxabi.h>
#include <exception>
#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

namespace testing {

/* ------------------------------------------------------------------ */
/*                      Array allocation tracking                      */
/* ------------------------------------------------------------------ */

inline long &liveArrays()  { static long n = 0; return n; }
inline long &totalArrays() { static long n = 0; return n; }

/* Number of arrays allocated with new[] that have not been delete[]'d. */
inline long liveArrayCount()       { return liveArrays(); }
/* Number of arrays EVER allocated with new[] (never decreases).      */
inline long totalArrayAllocations() { return totalArrays(); }

/* ------------------------------------------------------------------ */
/*                           Test registry                             */
/* ------------------------------------------------------------------ */

struct TestFailure {           /* deliberately NOT a std::exception */
    std::string message;
};

struct TestCase {
    std::string name;
    void (*fn)();
};

inline std::vector<TestCase> &registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(const char *name, void (*fn)()) {
        registry().push_back({name, fn});
    }
};

/* ------------------------------------------------------------------ */
/*                  Pretty-printing values in messages                 */
/* ------------------------------------------------------------------ */

template <typename T>
std::string show(const T &value) {
    std::ostringstream ss;
    ss << value;
    return ss.str();
}
inline std::string show(const std::string &s) { return "\"" + s + "\""; }
inline std::string show(const char *s) { return "\"" + std::string(s) + "\""; }
inline std::string show(bool b) { return b ? "true" : "false"; }
template <typename T>
std::string show(const std::vector<T> &v) {
    const size_t LIMIT = 40;
    std::string out = "{";
    for (size_t i = 0; i < v.size() && i < LIMIT; i++) {
        if (i > 0) out += ", ";
        out += show(v[i]);
    }
    if (v.size() > LIMIT) {
        out += ", ... (" + std::to_string(v.size()) + " elements total)";
    }
    return out + "}";
}

inline std::string demangle(const char *name) {
    int status = 0;
    char *real = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string result = (status == 0 && real != nullptr) ? real : name;
    std::free(real);
    return result;
}

/* In forked mode the child reports its result through this pipe. */
inline int  &resultFd()   { static int fd = -1; return fd; }
inline bool &resultSent() { static bool sent = false; return sent; }

inline void sendResult(bool passed, const std::string &detail) {
    if (resultFd() < 0 || resultSent()) return;
    resultSent() = true;
    std::string msg = (passed ? "P" : "F") + detail;
    const char *p = msg.data();
    size_t left = msg.size();
    while (left > 0) {
        ssize_t n = write(resultFd(), p, left);
        if (n <= 0) break;
        p += n;
        left -= static_cast<size_t>(n);
    }
}

[[noreturn]] inline void fail(const char *file, int line,
                              const std::string &msg) {
    std::ostringstream ss;
    ss << file << ":" << line << ": " << msg;
    /* Report now: if a broken list later crashes in its destructor while the
       stack unwinds, the parent still gets this message. */
    sendResult(false, ss.str());
    throw TestFailure{ss.str()};
}

/* ------------------------------------------------------------------ */
/*                              Runner                                 */
/* ------------------------------------------------------------------ */

enum class Outcome { PASS, FAIL, CRASH, TIMEOUT };

struct Result {
    Outcome     outcome;
    std::string detail;
};

inline Result runInline(const TestCase &t) {
    try {
        t.fn();
        return {Outcome::PASS, ""};
    } catch (const TestFailure &f) {
        return {Outcome::FAIL, f.message};
    } catch (const std::exception &e) {
        return {Outcome::FAIL, "uncaught exception " +
                               demangle(typeid(e).name()) + ": " + e.what()};
    } catch (...) {
        return {Outcome::FAIL, "uncaught exception of a non-std type"};
    }
}

inline void flushAll() {
    std::cout.flush();
    std::cerr.flush();
    std::fflush(nullptr);
}

#if defined(__SANITIZE_ADDRESS__)
#define TEST_FRAMEWORK_ASAN 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define TEST_FRAMEWORK_ASAN 1
#endif
#endif

inline std::string signalHint(int sig) {
#ifdef TEST_FRAMEWORK_ASAN
    if (sig == SIGABRT) {
        return " -- read the AddressSanitizer report printed above; the first "
               "'#N ... in' line that names YOUR .cpp file is the bad line";
    }
#endif
    switch (sig) {
    case SIGSEGV:
    case SIGBUS:
        return " -- you probably dereferenced a nullptr/dangling pointer "
               "or read past the end of an array";
    case SIGABRT:
        return " -- often a double delete, deleting a pointer that was not "
               "from new, or a failed assert";
    case SIGFPE:
        return " -- arithmetic error, e.g. integer division/modulo by zero";
    default:
        return "";
    }
}

inline Result runForked(const TestCase &t, unsigned timeoutSec) {
    flushAll();
    int fds[2];
    if (pipe(fds) != 0) return runInline(t);

    pid_t pid = fork();
    if (pid < 0) {
        close(fds[0]);
        close(fds[1]);
        return runInline(t);
    }

    if (pid == 0) { /* ---- child ---- */
        close(fds[0]);
        resultFd() = fds[1];
        alarm(timeoutSec);
        Result r = runInline(t);
        flushAll();
        sendResult(r.outcome == Outcome::PASS, r.detail);
        close(fds[1]);
        _exit(0);
    }

    /* ---- parent ---- */
    close(fds[1]);
    std::string msg;
    char buf[4096];
    ssize_t n;
    while ((n = read(fds[0], buf, sizeof buf)) > 0) msg.append(buf, n);
    close(fds[0]);

    int status = 0;
    waitpid(pid, &status, 0);

    if (!msg.empty() && msg[0] == 'F') {
        std::string detail = msg.substr(1);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            detail += std::string("\n(after this failure the test then ") +
                      (sig == SIGALRM ? "hung" : std::string("crashed with ") +
                                                     strsignal(sig)) +
                      " while cleaning up -- check your destructor/clear)";
        }
        return {Outcome::FAIL, detail};
    }
    if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        if (sig == SIGALRM) {
            return {Outcome::TIMEOUT,
                    "test did not finish within " + std::to_string(timeoutSec) +
                        " seconds -- infinite loop? cycle in your list? "
                        "or an algorithm that is too slow (e.g. O(n^2))"};
        }
        return {Outcome::CRASH, std::string("crashed with signal ") +
                                    strsignal(sig) + signalHint(sig)};
    }
    if (!msg.empty() && msg[0] == 'P') return {Outcome::PASS, ""};
    return {Outcome::CRASH,
            "test process exited unexpectedly (exit status " +
                std::to_string(WIFEXITED(status) ? WEXITSTATUS(status) : -1) +
                "). Did your code call exit()? If you built with SANITIZE=1, "
                "read the sanitizer report printed above."};
}

inline int runAllTests(int argc, char **argv) {
    bool                     useFork = true;
    unsigned                 timeout = 5;
    std::vector<std::string> filters;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--no-fork") {
            useFork = false;
        } else if (arg.rfind("--timeout=", 0) == 0) {
            timeout = static_cast<unsigned>(std::atoi(arg.c_str() + 10));
            if (timeout == 0) timeout = 5;
        } else if (arg == "--list") {
            for (const TestCase &t : registry()) std::cout << t.name << "\n";
            return 0;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "usage: " << argv[0]
                      << " [--list] [--no-fork] [--timeout=N] [filter ...]\n";
            return 0;
        } else {
            filters.push_back(arg);
        }
    }

    bool color = isatty(STDOUT_FILENO) || std::getenv("TEST_FORCE_COLOR") != nullptr;
    const char *GREEN  = color ? "\033[32m" : "";
    const char *RED    = color ? "\033[31m" : "";
    const char *YELLOW = color ? "\033[33m" : "";
    const char *BOLD   = color ? "\033[1m" : "";
    const char *RESET  = color ? "\033[0m" : "";

    int run = 0, passed = 0;
    std::vector<std::string> failedNames;

    for (const TestCase &t : registry()) {
        if (!filters.empty()) {
            bool match = false;
            for (const std::string &f : filters) {
                if (t.name.find(f) != std::string::npos) match = true;
            }
            if (!match) continue;
        }
        run++;
        Result r = useFork ? runForked(t, timeout) : runInline(t);

        switch (r.outcome) {
        case Outcome::PASS:
            passed++;
            std::cout << GREEN << "[ PASS    ] " << RESET << t.name << "\n";
            break;
        case Outcome::FAIL:
            std::cout << RED << "[ FAIL    ] " << RESET << t.name << "\n";
            break;
        case Outcome::CRASH:
            std::cout << RED << "[ CRASH   ] " << RESET << t.name << "\n";
            break;
        case Outcome::TIMEOUT:
            std::cout << YELLOW << "[ TIMEOUT ] " << RESET << t.name << "\n";
            break;
        }
        if (r.outcome != Outcome::PASS) {
            failedNames.push_back(t.name);
            std::istringstream lines(r.detail);
            std::string line;
            while (std::getline(lines, line)) {
                std::cout << "              " << line << "\n";
            }
        }
        std::cout.flush();
    }

    std::cout << "\n" << BOLD << passed << "/" << run << " tests passed"
              << RESET << "\n";
    if (!failedNames.empty() && failedNames.size() <= 10) {
        std::cout << "Not passing yet:";
        for (const std::string &n : failedNames) std::cout << " " << n;
        std::cout << "\n";
    }
    return passed == run ? 0 : 1;
}

} // namespace testing

/* ------------------------------------------------------------------ */
/*                          Assertion macros                           */
/* ------------------------------------------------------------------ */

#define TEST(name)                                                            \
    static void test_##name();                                                \
    static ::testing::Registrar registrar_##name(#name, test_##name);        \
    static void test_##name()

#define ASSERT_TRUE(cond)                                                     \
    do {                                                                      \
        if (!(cond))                                                          \
            ::testing::fail(__FILE__, __LINE__,                               \
                            "ASSERT_TRUE(" #cond ") failed");                 \
    } while (0)

#define ASSERT_FALSE(cond)                                                    \
    do {                                                                      \
        if (cond)                                                             \
            ::testing::fail(__FILE__, __LINE__,                               \
                            "ASSERT_FALSE(" #cond ") failed");                \
    } while (0)

/* Optional extra context string is printed on failure. */
#define ASSERT_EQ_MSG(expected, actual, context)                              \
    do {                                                                      \
        const auto &exp_ = (expected);                                        \
        const auto &act_ = (actual);                                          \
        if (!(exp_ == act_)) {                                                \
            ::testing::fail(__FILE__, __LINE__,                               \
                std::string("ASSERT_EQ(" #expected ", " #actual ") failed")   \
                + (std::string(context).empty() ? std::string()              \
                                                : "  [" + std::string(context) + "]") \
                + "\n    expected: " + ::testing::show(exp_)                  \
                + "\n    actual:   " + ::testing::show(act_));                \
        }                                                                     \
    } while (0)

#define ASSERT_EQ(expected, actual) ASSERT_EQ_MSG(expected, actual, "")

#define ASSERT_THROWS(ExceptionType, statement)                               \
    do {                                                                      \
        bool        threwExpected_ = false;                                   \
        std::string other_;                                                   \
        try {                                                                 \
            statement;                                                        \
        } catch (const ExceptionType &) {                                     \
            threwExpected_ = true;                                            \
        } catch (const std::exception &e_) {                                  \
            other_ = ::testing::demangle(typeid(e_).name()) +                 \
                     " (what(): \"" + e_.what() + "\")";                      \
        } catch (...) {                                                       \
            other_ = "an exception of a non-std type";                        \
        }                                                                     \
        if (!threwExpected_) {                                                \
            ::testing::fail(__FILE__, __LINE__,                               \
                std::string("ASSERT_THROWS(" #ExceptionType ", " #statement   \
                            ") failed")                                       \
                + "\n    expected: " #ExceptionType " to be thrown"           \
                + "\n    actual:   "                                          \
                + (other_.empty() ? std::string("nothing was thrown")         \
                                  : other_));                                 \
        }                                                                     \
    } while (0)

#define ASSERT_NO_THROW(statement)                                            \
    do {                                                                      \
        try {                                                                 \
            statement;                                                        \
        } catch (const std::exception &e_) {                                  \
            ::testing::fail(__FILE__, __LINE__,                               \
                "ASSERT_NO_THROW(" #statement ") failed: threw "              \
                + ::testing::demangle(typeid(e_).name()) + " (what(): \""     \
                + e_.what() + "\")");                                         \
        }                                                                     \
    } while (0)

/* ------------------------------------------------------------------ */
/*      Global operator new[] / delete[] replacements (for leak checks) */
/* ------------------------------------------------------------------ */

/*
 * Like a debugging allocator: new arrays are filled with garbage (0xCD) and
 * deleted arrays are scribbled over (0xDD) before being freed. That way,
 * reading uninitialized slots or using an array after delete [] gives
 * obviously-wrong values instead of "accidentally working".
 */
namespace testing {
const std::size_t ARRAY_HEADER = 16; /* keeps 16-byte alignment */
}

void *operator new[](std::size_t size) {
    char *raw = static_cast<char *>(std::malloc(size + ::testing::ARRAY_HEADER));
    if (raw == nullptr) throw std::bad_alloc();
    *reinterpret_cast<std::size_t *>(raw) = size;
    std::memset(raw + ::testing::ARRAY_HEADER, 0xCD, size);
    ++::testing::liveArrays();
    ++::testing::totalArrays();
    return raw + ::testing::ARRAY_HEADER;
}

void operator delete[](void *p) noexcept {
    if (p == nullptr) return;
    char *raw = static_cast<char *>(p) - ::testing::ARRAY_HEADER;
    std::memset(p, 0xDD, *reinterpret_cast<std::size_t *>(raw));
    --::testing::liveArrays();
    std::free(raw);
}

void operator delete[](void *p, std::size_t) noexcept {
    operator delete[](p);
}

int main(int argc, char **argv) {
    return ::testing::runAllTests(argc, argv);
}

#endif
