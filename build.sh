#!/usr/bin/env bash

compile_debug() {
	meson setup build "-Dbuildtype=debug"
	meson compile -C build
}

compile_release() {
	meson setup build "-Dbuildtype=release"
	meson compile -C build
}

install() {
	meson install -C build
}

clean() {
	rm -rf build
}

help() {
	printf "usage: ./build.sh [options...]\n\n"
	printf "options:\n"
	printf "\t-r - compile release\n"
	printf "\t-d - compile debug\n"
	printf "\t-i - install\n"
	printf "\t-c - clean\n"
	printf "\t-h - prints this help message\n"
}

if [ -z "$*" ]; then
	compile_release
	exit 0
fi

while getopts "rdich" opt; do
	case $opt in
		r) compile_release ;;
		d) compile_debug ;;
		i) install ;;
		c) clean ;;
		h) help ;;
		*) echo "unknown option" ;;
	esac
done

exit 0
