# This file is part of CMake-codecov.
#
# SPDX-FileCopyrightText: RWTH Aachen University, Federal Republic of Germany
# SPDX-FileContributor: Alexander Haase, alexander.haase@rwth-aachen.de
#
# SPDX-License-Identifier: BSD-3-Clause


# recurse into subdirectories
add_subdirectory(libfoo)
add_subdirectory(bar)
add_subdirectory(libheader)

if(CMAKE_Fortran_COMPILER)
    add_subdirectory(fortran)
endif()
