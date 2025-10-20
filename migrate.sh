rg -l '^namespace gr' | xargs gsed -i 's/namespace gr/namespace gen/'
find . -name "*.cpp" | xargs gsed -i 's/gramma/genesis/g'
find . -name "*.hpp" | xargs gsed -i 's/gramma/genesis/g'
find . -name "*.cpp" | xargs gsed -i 's/gr::/gen::/g'
find . -name "*.hpp" | xargs gsed -i 's/gr::/gen::/g'

