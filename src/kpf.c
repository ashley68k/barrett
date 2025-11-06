/*
Copyright (C) 1994-1995 Apogee Software, Ltd.
Copyright (C) 2002-2015 icculus.org, GNU/Linux port
Copyright (C) 2017-2018 Steven LeVesque
Copyright (C) 2025 lunarmeadow (she/her)
Copyright (C) 2025 erysdren (it/its)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include "miniz.h"
#include "miniz_common.h"
#include "miniz_zip.h"
#include "kpf.h"
#include "kpfdata.h"

static mz_zip_archive kpfArc;

// TODO: search for kpf in LE install
const char* GetKPFPath(void)
{
    return "RottEX.kpf";
}

void InitKPF(void)
{
    mz_bool status;
    mz_zip_error err;

    if(mz_zip_validate_file_archive(GetKPFPath(), NULL, &err))
        status = mz_zip_reader_init_file(&kpfArc, GetKPFPath(), 0);

    if(!status)
    {
        printf("Couldn't initialize KPF file!\n");
        ShutdownKPF();
        return;
    }

    // fetch functions do error handling, just let them exit
    if(!FetchBetaWalls(kpfArc))
        return;
}

void ShutdownKPF(void)
{
    mz_zip_reader_end(&kpfArc);
}