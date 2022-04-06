/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   arrow_left_png;
    const int            arrow_left_pngSize = 844;

    extern const char*   arrow_right_png;
    const int            arrow_right_pngSize = 575;

    extern const char*   icon_png;
    const int            icon_pngSize = 11025;

    extern const char*   title_png;
    const int            title_pngSize = 41527;

    extern const char*   bubbles_jpg;
    const int            bubbles_jpgSize = 1022374;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
