for f in t/*.lox; do
    echo "=== $f"
    ./build-asan/clox "$f"
    echo "exit: $?"
done