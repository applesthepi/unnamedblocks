@ECHO OFF

SET /A CMAKE_DEPENDENCY_COUNT = 0
SET /A CARGO_DEPENDENCY_COUNT = 1
SET /A DEPENDENCY_COUNT = %CMAKE_DEPENDENCY_COUNT% + %CARGO_DEPENDENCY_COUNT%
SET /A PROGRESS_COUNT = 0

CALL :BUILD_DEPENDENCIES
CALL :CONFIGURE_UB

EXIT /B 0

:BUILD_DEPENDENCIES
ECHO building dependencies...

CALL :BUILD_CARGO "vp-engine",
cd dependencies\vp-engine
mkdir include
mkdir lib\debug
mkdir lib\release
xcopy target\cxxbridge\rust\cxx.h include /y
xcopy target\debug\vpe.lib lib\debug /y
xcopy target\release\vpe.lib lib\release /y
xcopy target\cxxbridge\vpe\src\bindings\*.h include /sy
cd ../../

ECHO ...building finished
EXIT /B 0

:BUILD_CMAKE
SET /A PROGRESS_COUNT = %PROGRESS_COUNT% + 1
ECHO [ %PROGRESS_COUNT% / %DEPENDENCY_COUNT% ] building... %~1
::
ECHO [ %PROGRESS_COUNT% / %DEPENDENCY_COUNT% ] ...finished %~1
EXIT /B 0

:BUILD_CARGO
SET /A PROGRESS_COUNT = %PROGRESS_COUNT% + 1
ECHO [ %PROGRESS_COUNT% / %DEPENDENCY_COUNT% ] building... %~1

cd /d dependencies/%~1
SET RUSTFLAGS=-Ctarget-feature=-crt-static
cargo build
cargo build --release
cd /d ../../

ECHO [ %PROGRESS_COUNT% / %DEPENDENCY_COUNT% ] ...finished %~1
EXIT /B 0

:TEST_CARGO
cd /d dependencies/%~1
cargo test --test %~2
cd /d ../../
EXIT /B 0

:CONFIGURE_UB
CALL configure.bat
EXIT /B 0