/*
 * e9alloc.h
 * Copyright (C) 2020 National University of Singapore
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __E9ALLOC_H
#define __E9ALLOC_H

#include <cstdint>

#include "e9patch.h"

const Alloc *allocate(Binary *B, intptr_t lb, intptr_t ub,
    const Trampoline *T, const Instr *I, bool same_page = false);
bool reserve(Binary *B, intptr_t lb, intptr_t ub);
void deallocate(Binary *B, const Alloc *a);

#endif
