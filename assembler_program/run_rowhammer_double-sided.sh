#!/bin/bash
debug_mode=false

parse() {
    while [ "$#" -gt 0 ]; do
        case "$1" in
            -debug)
            debug_mode=true
            ;;

            *)
            echo "do not recognize flag: $1"
            exit 1
            ;;
        esac
        shift
    done
}

main() {
    nasm -g -f elf64 rowhammer_dounle-sided.asm -o rowhammer_dounle-sided.o
    ld rowhammer_dounle-sided.o -o rowhammer_double-sided

    if [ "$debug_mode" = true ]; then
        echo "Debug mode ON"
        echo "commands:"
        echo "run - run the script"
        echo "ctrl+c - exit script"
        echo "x/10xb $ rdi - without space dollar-rdi, show 10 elements of array"
        echo "info registers - show all registers with value"
        echo ""

        gdb ./rowhammer_double-sided

    else 
        echo ""
        echo "ROWHAMMER IS RUNNING"
        ./rowhammer_double-sided
    fi
}

parse "$@"
main