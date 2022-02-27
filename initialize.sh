echo
echo ===============================
echo ====[ updating submodules ]====
echo ===============================
echo

git submodule update --init --recursive -j 8

echo
echo ======================================
echo ====[ downloading rust toolchain ]====
echo ======================================
echo

RUSTUP_HOME="$PWD/toolchain/tmp" CARGO_HOME="$PWD/toolchain/tmp" bash -c 'curl https://sh.rustup.rs -sSf | sh -s -- --profile minimal --default-toolchain nightly --no-modify-path -y'
cp toolchain/tmp/bin/cargo toolchain
rm -r toolchain/tmp

echo
echo ================================
echo ====[ setting up toolchain ]====
echo ================================
echo

cd toolchain
rm -rf ubbs
./cargo new ubbs --lib
mv cargo ubbs
echo [lib] >> ubbs/Cargo.toml
echo name = \"ubbs\" >> ubbs/Cargo.toml
echo crate-type = [\"cdylib\"] >> ubbs/Cargo.toml

echo
echo ==========================
echo ====[ finished setup ]====
echo ==========================
echo
