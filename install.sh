BIN="featcode"
REPO="angt/$BIN"
URL="https://github.com/$REPO/releases/latest/download"

die() {
	for msg; do echo "$msg"; done >&2
	exit 111
}

check_bin() {
	command -v "$1" >/dev/null 2>/dev/null
}

main() {
	case "$(uname -m)" in
	(arm64|aarch64) ARCH=aarch64 ;;
	(amd64|x86_64)  ARCH=x86_64  ;;
	(*) die "Arch not supported"
	esac

	case "$(uname -s)" in
	(Linux)  OS=linux ;;
	(Darwin) OS=macos ;;
	(*) die "OS not supported"
	esac

	check_bin curl ||
		die "Please install curl"

	curl -fsSL "$URL/$ARCH-$OS-$BIN" -o "$BIN" ||
		die "Failed to download"

	chmod +x "$BIN"
}
main "$@"
