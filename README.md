# python API for WCP

## Setup and tests on uboonegpvm

### utility shell functions

```bash
path-remove ()
{
    local IFS=':';
    local NEWPATH;
    local DIR;
    local PATHVARIABLE=${2:-PATH};
    for DIR in ${!PATHVARIABLE};
    do  
        if [ "$DIR" != "$1" ]; then
            NEWPATH=${NEWPATH:+$NEWPATH:}$DIR;
        fi;
    done;
    export $PATHVARIABLE="$NEWPATH"
}

path-prepend ()
{
    path-remove "$1" "$2";
    local PATHVARIABLE="${2:-PATH}";
    export $PATHVARIABLE="$1${!PATHVARIABLE:+:${!PATHVARIABLE}}"
}

path-append ()
{
    path-remove "$1" "$2";
    local PATHVARIABLE="${2:-PATH}";
    export $PATHVARIABLE="${!PATHVARIABLE:+${!PATHVARIABLE}:}$1"
}
```

### setup ups enviroments on uboonegpvm

```bash
source /cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh
setup uboonecode v08_00_00_42 -q e17:prof

setup numpy v1_14_3 -q e17:openblas:p2714b:prof
setup libtorch v1_0_1 -q e17:prof
setup SparseConvNet 8422a6f -q e17:prof
```

### obtain 3 git repositories

- wcp: https://github.com/BNLIF/wire-cell
- wcp-pyinf: python scripts https://github.com/BNLIF/wire-cell-pyinf
- wcp-pydata: model and test data https://github.com/BNLIF/wire-cell-pydata

### compile WCP

Configure:
```bash
env CC=gcc CXX=g++ FC=gfortran ./waf-tools/waf configure \
    --with-root="$ROOTSYS" \
    --boost-includes="$BOOST_INC" \
    --boost-libs="$BOOST_LIB" \
    --boost-mt \
    --with-fftw="$FFTW_FQ_DIR" \
    --with-fftw-include="$FFTW_INC" \
    --with-fftw-lib="$FFTW_LIBRARY" \
    --with-eigen="$EIGEN_DIR" \
    --with-glpk="$GLPK_FQ_DIR/" --with-glpk-include="$GLPK_INC" --with-glpk-lib="$GLPK_LIB" \
    --with-python="$PYTHON_ROOT/" --with-python-include="$PYTHON_INCLUDE" --with-python-lib="$PYTHON_LIB" \
    --prefix="../opt"
```
Build and install:
```bash
./waf-tools/waf -p install
```

### envs for local build test

```bash
# local build of WCP
WCP_FQ_DIR=/uboone/app/users/yuhw/products/wire-cell/
path-prepend $WCP_FQ_DIR/lib/ LD_LIBRARY_PATH
path-prepend $WCP_FQ_DIR/lib64/ LD_LIBRARY_PATH
path-prepend $WCP_FQ_DIR/bin/ PATH

# add pyinf to PYTHONPATH
WCPPYINF_FQ_DIR=/uboone/app/users/yuhw/wire-cell-pyinf
path-prepend $WCPPYINF_FQ_DIR PYTHONPATH

WCPPYDATA_FQ_DIR=/uboone/app/users/yuhw/wire-cell-pydata
```
### test python, C++ enviroment and communication
```
./build/pyutil/test_call_func test_funcs multiply 2 3
```

### inputs for tests using `wire-cell-prod-nue`

[link](https://www.phy.bnl.gov/~yuhw/nue-cc-production/nue-dl-test/)



