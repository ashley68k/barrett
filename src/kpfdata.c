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

#include "kpfdata.h"
#include "kpf.h"

#include "png.h"

#include "miniz.h"
#include "miniz_zip.h"
#include "rt_def.h"

boolean FetchBetaWalls(mz_zip_archive kpfArc)
{
    void *filePtr;
    size_t decompSize;

    for(unsigned long i = 0; i < ARRAY_COUNT(betaWalls); i++)
    {
        int fileIdx = mz_zip_reader_locate_file(&kpfArc, betaWalls[i], NULL, 
            MZ_ZIP_FLAG_IGNORE_PATH | MZ_ZIP_FLAG_CASE_SENSITIVE);

        if(fileIdx < 0)
        {
            if(mz_zip_reader_is_file_a_directory(&kpfArc, fileIdx))
            {
                printf("Attempted read %s is directory!", betaWalls[i]);
                ShutdownKPF();
                return false;
            }

            printf("Failed to locate beta wall lump %s", betaWalls[i]);
            ShutdownKPF();
            return false;
        }

        filePtr = mz_zip_reader_extract_file_to_heap(&kpfArc, betaWalls[i], &decompSize, NULL);

        if(!decompSize)
        {
            printf("File %s failed to decompress!", betaWalls[i]);
            ShutdownKPF();
            return false;
        }

        PNGDecode(filePtr, decompSize);

        mz_free(filePtr);
    }

    return true;
}