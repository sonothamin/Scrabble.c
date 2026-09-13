#!/usr/bin/env bash
# uninstall.sh :: Removes Scrabble installed by install.sh

set -euo pipefail

CMD_NAME="scrabble"

SYSTEM_INSTALL_DIR="/opt/scrabble"
SYSTEM_BIN_DIR="/usr/local/bin"

USER_INSTALL_DIR="${HOME}/.local/share/scrabble"
USER_BIN_DIR="${HOME}/.local/bin"

RED=$'\033[0;31m'; GREEN=$'\033[0;32m'; YELLOW=$'\033[1;33m'; BLUE=$'\033[0;34m'; NC=$'\033[0m'

info()  { printf "%s[INFO]%s  %s\n"  "$BLUE"   "$NC" "$1"; }
ok()    { printf "%s[OK]%s    %s\n"  "$GREEN"  "$NC" "$1"; }
warn()  { printf "%s[WARN]%s  %s\n"  "$YELLOW" "$NC" "$1"; }
err()   { printf "%s[ERROR]%s %s\n"  "$RED"    "$NC" "$1" >&2; }
die()   { err "$1"; exit 1; }

confirm() {
    local prompt="$1" reply
    read -r -p "$prompt [y/N] " reply </dev/tty
    [[ "$reply" =~ ^[Yy]$ ]]
}

remove_install() {
    # remove_install <install_dir> <bin_dir> <needs_sudo:0|1>
    local install_dir="$1" bin_dir="$2" need_sudo="$3"
    local wrapper="${bin_dir}/${CMD_NAME}"
    local sudo_cmd=""

    if [[ "$need_sudo" -eq 1 && "$EUID" -ne 0 ]]; then
        command -v sudo >/dev/null 2>&1 || die "Removing ${install_dir} needs root, and 'sudo' isn't available."
        sudo_cmd="sudo"
    fi

    info "Found install at ${install_dir}"
    if ! confirm "Remove ${install_dir} and ${wrapper}?"; then
        warn "Skipped ${install_dir}."
        return
    fi

    if [[ -d "$install_dir" ]]; then
        $sudo_cmd rm -rf -- "$install_dir" \
            && ok "Removed ${install_dir}." \
            || err "Failed to remove ${install_dir}. Check permissions."
    fi

    if [[ -f "$wrapper" || -L "$wrapper" ]]; then
        $sudo_cmd rm -f -- "$wrapper" \
            && ok "Removed ${wrapper}." \
            || err "Failed to remove ${wrapper}. Check permissions."
    fi
}

FOUND_ANY=0

if [[ -d "$SYSTEM_INSTALL_DIR" || -f "${SYSTEM_BIN_DIR}/${CMD_NAME}" ]]; then
    FOUND_ANY=1
    remove_install "$SYSTEM_INSTALL_DIR" "$SYSTEM_BIN_DIR" 1
fi

if [[ -d "$USER_INSTALL_DIR" || -f "${USER_BIN_DIR}/${CMD_NAME}" ]]; then
    FOUND_ANY=1
    remove_install "$USER_INSTALL_DIR" "$USER_BIN_DIR" 0
fi

echo
if [[ "$FOUND_ANY" -eq 0 ]]; then
    warn "No Scrabble installation found in known locations:"
    echo "    ${SYSTEM_INSTALL_DIR} / ${SYSTEM_BIN_DIR}/${CMD_NAME}"
    echo "    ${USER_INSTALL_DIR} / ${USER_BIN_DIR}/${CMD_NAME}"
    exit 0
fi

if command -v "$CMD_NAME" >/dev/null 2>&1; then
    warn "'${CMD_NAME}' is still resolvable on PATH from another location: $(command -v "$CMD_NAME")"
else
    ok "Uninstall complete. '${CMD_NAME}' is no longer on PATH."
fi
