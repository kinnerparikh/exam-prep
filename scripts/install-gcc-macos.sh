#!/bin/bash
set -euo pipefail

usage() {
    cat <<'EOF'
Usage: ./scripts/install-gcc-macos.sh

Install GNU GCC with Homebrew on an Apple Silicon or Intel Mac.
If Homebrew is missing, ask before installing it from https://brew.sh.
Apple's Command Line Tools (or Xcode) and internet access are required.
Homebrew/macOS support limits still apply; old Macs may not be supported.

Existing GCC installations are reused. The script does not replace Apple's
compilers or change your shell configuration. It prints a C++17 make command.
Run as your normal user, not with sudo.
EOF
}

if [ "$#" -gt 0 ]; then
    case "$1" in
        -h|--help) usage; exit 0 ;;
        *) usage >&2; exit 2 ;;
    esac
fi

if [ "$(uname -s)" != Darwin ]; then
    printf 'This installer is for macOS only.\n' >&2
    exit 1
fi

case "$(uname -m)" in
    arm64|x86_64) ;;
    *) printf 'An Apple Silicon or 64-bit Intel Mac is required.\n' >&2; exit 1 ;;
esac

if [ "$EUID" -eq 0 ]; then
    printf 'Run this script as your normal user, without sudo.\n' >&2
    exit 1
fi

if ! xcode-select -p >/dev/null 2>&1 || ! xcrun --find clang >/dev/null 2>&1; then
    printf 'Apple developer tools are missing or not configured.\n' >&2
    printf 'Run xcode-select --install, finish installation, then rerun this script.\n' >&2
    exit 1
fi

find_brew() {
    if command -v brew >/dev/null 2>&1; then
        command -v brew
    elif [ -x /opt/homebrew/bin/brew ]; then
        printf '/opt/homebrew/bin/brew\n'
    elif [ -x /usr/local/bin/brew ]; then
        printf '/usr/local/bin/brew\n'
    else
        return 1
    fi
}

work_dir=$(mktemp -d "${TMPDIR:-/tmp}/cs15-gcc.XXXXXX")
cleanup() {
    rm -f "$work_dir/homebrew-install.sh" "$work_dir/check.cpp" "$work_dir/check"
    rmdir "$work_dir"
}
trap cleanup EXIT

if ! brew_bin=$(find_brew); then
    if [ ! -t 0 ]; then
        printf 'Homebrew is missing. Rerun in an interactive Terminal to install it.\n' >&2
        exit 1
    fi
    printf 'Install Homebrew using its official installer (may ask for your Mac password)? [y/N] '
    read -r answer
    case "$answer" in
        y|Y|yes|YES) ;;
        *) printf 'Homebrew installation cancelled.\n'; exit 1 ;;
    esac
    curl --fail --show-error --silent --location \
        https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh \
        --output "$work_dir/homebrew-install.sh"
    /bin/bash "$work_dir/homebrew-install.sh"
    brew_bin=$(find_brew) || {
        printf 'Homebrew was not found after installation. See https://brew.sh.\n' >&2
        exit 1
    }
fi

brew_prefix=$("$brew_bin" --prefix)
export PATH="$brew_prefix/bin:$brew_prefix/sbin:$PATH"

if "$brew_bin" list --versions gcc >/dev/null 2>&1; then
    printf 'Using the existing Homebrew GCC installation.\n'
else
    # Older Homebrew releases may not recognize bottles for newer macOS versions.
    "$brew_bin" update
    "$brew_bin" install gcc
fi

gcc_prefix=$("$brew_bin" --prefix gcc)
cxx=
for candidate in "$gcc_prefix"/bin/g++-[0-9]*; do
    if [ -x "$candidate" ]; then
        cxx=$candidate
        break
    fi
done

if [ -z "$cxx" ]; then
    printf 'No versioned GNU C++ compiler found in %s/bin.\n' "$gcc_prefix" >&2
    exit 1
fi
cc="$gcc_prefix/bin/gcc-${cxx##*g++-}"
if [ ! -x "$cc" ]; then
    printf 'The matching GNU C compiler was not found: %s\n' "$cc" >&2
    exit 1
fi

cat > "$work_dir/check.cpp" <<'EOF'
#include <iostream>
#if !defined(__GNUC__) || defined(__clang__)
#error "Expected GNU GCC, not Apple clang."
#endif
int main() {
    std::cout << "GNU GCC C++17 is working.\n";
}
EOF
"$cxx" -std=c++17 "$work_dir/check.cpp" -o "$work_dir/check"
"$work_dir/check"

printf '\nInstalled GNU GCC %s\n' "$("$cxx" -dumpfullversion)"
printf 'C compiler:   %s\nC++ compiler: %s\n' "$cc" "$cxx"
printf '\nFrom the exam-prep folder, run:\n'
printf '  make CXX="%s" BUILD=build-gcc al1\n' "$cxx"
printf '\nThe system gcc/g++ commands may still be Apple clang; use the path above.\n'
printf 'Keep the default clang++ compiler for make SANITIZE=1 on macOS.\n'
