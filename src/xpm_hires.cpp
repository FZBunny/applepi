/*****************************************************************************

    File: "xpm_hires.cpp"

    Copyright (C) 2003,2021 Bruce Ward

    This file is part of ApplePi, an Apple2 emulator.
    (Apple2 is a trademark of Apple, Inc.)

    "ApplePi" is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    "ApplePi" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You may obtain a copy of the GNU General Public License by writing to:

    The Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA 02110-1335 USA

    Or simply visit: https://www.gnu.org/licenses/gpl-3.0.html

*****************************************************************************/



/***************************************************/
/*                                                 */
/*    xpm_hires_0 is used if bit 7 of byte is 0    */
/*                                                 */
/***************************************************/

const char* xpm_hires_0[] =
{
    "14 2048 4 1",
    "X c #FFFFFF",
    ". c #000000",
    "p c #FF00FF",
    "g c #00E000",

// ************ for bytes on an even column ***********

// bit 6 of previous byte=0  and bit 0 of next byte=0

    "..............",  // 00
    "..............",
    "pppp..........",  // 01
    "..............",
    "..gggg........",  // 02
    "..............",
    "XXXXgg........",  // 03
    "..............",
    "....pppp......",  // 04
    "..............",
    "pppppppp......",  // 05
    "..............",
    "..XXXXpp......",  // 06
    "..............",
    "XXXXXXpp......",  // 07
    "..............",
    "......gggg....",  // 08
    "..............",
    "pppp..gggg....",  // 09
    "..............",
    "..gggggggg....",  // 0A
    "..............",
    "XXXXgggggg....",  // 0B
    "..............",
    "....XXXXgg....",  // 0C
    "..............",
    "ppppXXXXgg....",  // 0D
    "..............",
    "..XXXXXXgg....",  // 0E
    "..............",
    "XXXXXXXXgg....",  // 0F
    "..............",
    "........pppp..",  // 10
    "..............",
    "pppp....pppp..",  // 11
    "..............",
    "..gggg..pppp..",  // 12
    "..............",
    "XXXXgg..pppp..",  // 13
    "..............",
    "....pppppppp..",  // 14
    "..............",
    "pppppppppppp..",  // 15
    "..............",
    "..XXXXpppppp..",  // 16
    "..............",
    "XXXXXXpppppp..",  // 17
    "..............",
    "......XXXXpp..",  // 18
    "..............",
    "pppp..XXXXpp..",  // 19
    "..............",
    "..ggggXXXXpp..",  // 1A
    "..............",
    "XXXXggXXXXpp..",  // 1B
    "..............",
    "....XXXXXXpp..",  // 1C
    "..............",
    "ppppXXXXXXpp..",  // 1D
    "..............",
    "..XXXXXXXXpp..",  // 1E
    "..............",
    "XXXXXXXXXXpp..",  // 1F
    "..............",
    "..........gggg",  // 20
    "..............",
    "pppp......gggg",  // 21
    "..............",
    "..gggg....gggg",  // 22
    "..............",
    "XXXXgg....gggg",  // 23
    "..............",
    "....pppp..gggg",  // 24
    "..............",
    "pppppppp..gggg",  // 25
    "..............",
    "..XXXXpp..gggg",  // 26
    "..............",
    "XXXXXXpp..gggg",  // 27
    "..............",
    "......gggggggg",  // 28
    "..............",
    "pppp..gggggggg",  // 29
    "..............",
    "..gggggggggggg",  // 2A
    "..............",
    "XXXXgggggggggg",  // 2B
    "..............",
    "....XXXXgggggg",  // 2C
    "..............",
    "ppppXXXXgggggg",  // 2D
    "..............",
    "..XXXXXXgggggg",  // 2E
    "..............",
    "XXXXXXXXgggggg",  // 2F
    "..............",
    "........XXXXgg",  // 30
    "..............",
    "pppp....XXXXgg",  // 31
    "..............",
    "..gggg..XXXXgg",  // 32
    "..............",
    "XXXXgg..XXXXgg",  // 33
    "..............",
    "....ppppXXXXgg",  // 34
    "..............",
    "ppppppppXXXXgg",  // 35
    "..............",
    "..XXXXppXXXXgg",  // 36
    "..............",
    "XXXXXXppXXXXgg",  // 37
    "..............",
    "......XXXXXXgg",  // 38
    "..............",
    "pppp..XXXXXXgg",  // 39
    "..............",
    "..ggggXXXXXXgg",  // 3A
    "..............",
    "XXXXggXXXXXXgg",  // 3B
    "..............",
    "....XXXXXXXXgg",  // 3C
    "..............",
    "ppppXXXXXXXXgg",  // 3D
    "..............",
    "..XXXXXXXXXXgg",  // 3E
    "..............",
    "XXXXXXXXXXXXgg",  // 3F
    "..............",
    "............pp",  // 40
    "..............",
    "pppp........pp",  // 41
    "..............",
    "..gggg......pp",  // 42
    "..............",
    "XXXXgg......pp",  // 43
    "..............",
    "....pppp....pp",  // 44
    "..............",
    "pppppppp....pp",  // 45
    "..............",
    "..XXXXpp....pp",  // 46
    "..............",
    "XXXXXXpp....pp",  // 47
    "..............",
    "......gggg..pp",  // 48
    "..............",
    "pppp..gggg..pp",  // 49
    "..............",
    "..gggggggg..pp",  // 4A
    "..............",
    "XXXXgggggg..pp",  // 4B
    "..............",
    "....XXXXgg..pp",  // 4C
    "..............",
    "ppppXXXXgg..pp",  // 4D
    "..............",
    "..XXXXXXgg..pp",  // 4E
    "..............",
    "XXXXXXXXgg..pp",  // 4F
    "..............",
    "........pppppp",  // 50
    "..............",
    "pppp....pppppp",  // 51
    "..............",
    "..gggg..pppppp",  // 52
    "..............",
    "XXXXgg..pppppp",  // 53
    "..............",
    "....pppppppppp",  // 54
    "..............",
    "pppppppppppppp",  // 55
    "..............",
    "..XXXXpppppppp",  // 56
    "..............",
    "XXXXXXpppppppp",  // 57
    "..............",
    "......XXXXpppp",  // 58
    "..............",
    "pppp..XXXXpppp",  // 59
    "..............",
    "..ggggXXXXpppp",  // 5A
    "..............",
    "XXXXggXXXXpppp",  // 5B
    "..............",
    "....XXXXXXpppp",  // 5C
    "..............",
    "ppppXXXXXXpppp",  // 5D
    "..............",
    "..XXXXXXXXpppp",  // 5E
    "..............",
    "XXXXXXXXXXpppp",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "pppp......XXXX",  // 61
    "..............",
    "..gggg....XXXX",  // 62
    "..............",
    "XXXXgg....XXXX",  // 63
    "..............",
    "....pppp..XXXX",  // 64
    "..............",
    "pppppppp..XXXX",  // 65
    "..............",
    "..XXXXpp..XXXX",  // 66
    "..............",
    "XXXXXXpp..XXXX",  // 67
    "..............",
    "......ggggXXXX",  // 68
    "..............",
    "pppp..ggggXXXX",  // 69
    "..............",
    "..ggggggggXXXX",  // 6A
    "..............",
    "XXXXggggggXXXX",  // 6B
    "..............",
    "....XXXXggXXXX",  // 6C
    "..............",
    "ppppXXXXggXXXX",  // 6D
    "..............",
    "..XXXXXXggXXXX",  // 6E
    "..............",
    "XXXXXXXXggXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "pppp....XXXXXX",  // 71
    "..............",
    "..gggg..XXXXXX",  // 72
    "..............",
    "XXXXgg..XXXXXX",  // 73
    "..............",
    "....ppppXXXXXX",  // 74
    "..............",
    "ppppppppXXXXXX",  // 75
    "..............",
    "..XXXXppXXXXXX",  // 76
    "..............",
    "XXXXXXppXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "pppp..XXXXXXXX",  // 79
    "..............",
    "..ggggXXXXXXXX",  // 7A
    "..............",
    "XXXXggXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "ppppXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=0

    "gg............",  // 00
    "..............",
    "XXpp..........",  // 01
    "..............",
    "gggggg........",  // 02
    "..............",
    "XXXXgg........",  // 03
    "..............",
    "gg..pppp......",  // 04
    "..............",
    "XXpppppp......",  // 05
    "..............",
    "ggXXXXpp......",  // 06
    "..............",
    "XXXXXXpp......",  // 07
    "..............",
    "gg....gggg....",  // 08
    "..............",
    "XXpp..gggg....",  // 09
    "..............",
    "gggggggggg....",  // 0A
    "..............",
    "XXXXgggggg....",  // 0B
    "..............",
    "gg..XXXXgg....",  // 0C
    "..............",
    "XXppXXXXgg....",  // 0D
    "..............",
    "ggXXXXXXgg....",  // 0E
    "..............",
    "XXXXXXXXgg....",  // 0F
    "..............",
    "gg......pppp..",  // 10
    "..............",
    "XXpp....pppp..",  // 11
    "..............",
    "gggggg..pppp..",  // 12
    "..............",
    "XXXXgg..pppp..",  // 13
    "..............",
    "gg..pppppppp..",  // 14
    "..............",
    "XXpppppppppp..",  // 15
    "..............",
    "ggXXXXpppppp..",  // 16
    "..............",
    "XXXXXXpppppp..",  // 17
    "..............",
    "gg....XXXXpp..",  // 18
    "..............",
    "XXpp..XXXXpp..",  // 19
    "..............",
    "ggggggXXXXpp..",  // 1A
    "..............",
    "XXXXggXXXXpp..",  // 1B
    "..............",
    "gg..XXXXXXpp..",  // 1C
    "..............",
    "XXppXXXXXXpp..",  // 1D
    "..............",
    "ggXXXXXXXXpp..",  // 1E
    "..............",
    "XXXXXXXXXXpp..",  // 1F
    "..............",
    "gg........gggg",  // 20
    "..............",
    "XXpp......gggg",  // 21
    "..............",
    "gggggg....gggg",  // 22
    "..............",
    "XXXXgg....gggg",  // 23
    "..............",
    "gg..pppp..gggg",  // 24
    "..............",
    "XXpppppp..gggg",  // 25
    "..............",
    "ggXXXXpp..gggg",  // 26
    "..............",
    "XXXXXXpp..gggg",  // 27
    "..............",
    "gg....gggggggg",  // 28
    "..............",
    "XXpp..gggggggg",  // 29
    "..............",
    "gggggggggggggg",  // 2A
    "..............",
    "XXXXgggggggggg",  // 2B
    "..............",
    "gg..XXXXgggggg",  // 2C
    "..............",
    "XXppXXXXgggggg",  // 2D
    "..............",
    "ggXXXXXXgggggg",  // 2E
    "..............",
    "XXXXXXXXgggggg",  // 2F
    "..............",
    "gg......XXXXgg",  // 30
    "..............",
    "XXpp....XXXXgg",  // 31
    "..............",
    "gggggg..XXXXgg",  // 32
    "..............",
    "XXXXgg..XXXXgg",  // 33
    "..............",
    "gg..ppppXXXXgg",  // 34
    "..............",
    "XXppppppXXXXgg",  // 35
    "..............",
    "ggXXXXppXXXXgg",  // 36
    "..............",
    "XXXXXXppXXXXgg",  // 37
    "..............",
    "gg....XXXXXXgg",  // 38
    "..............",
    "XXpp..XXXXXXgg",  // 39
    "..............",
    "ggggggXXXXXXgg",  // 3A
    "..............",
    "XXXXggXXXXXXgg",  // 3B
    "..............",
    "gg..XXXXXXXXgg",  // 3C
    "..............",
    "XXppXXXXXXXXgg",  // 3D
    "..............",
    "ggXXXXXXXXXXgg",  // 3E
    "..............",
    "XXXXXXXXXXXXgg",  // 3F
    "..............",
    "gg..........pp",  // 40
    "..............",
    "XXpp........pp",  // 41
    "..............",
    "gggggg......pp",  // 42
    "..............",
    "XXXXgg......pp",  // 43
    "..............",
    "gg..pppp....pp",  // 44
    "..............",
    "XXpppppp....pp",  // 45
    "..............",
    "ggXXXXpp....pp",  // 46
    "..............",
    "XXXXXXpp....pp",  // 47
    "..............",
    "gg....gggg..pp",  // 48
    "..............",
    "XXpp..gggg..pp",  // 49
    "..............",
    "gggggggggg..pp",  // 4A
    "..............",
    "XXXXgggggg..pp",  // 4B
    "..............",
    "gg..XXXXgg..pp",  // 4C
    "..............",
    "XXppXXXXgg..pp",  // 4D
    "..............",
    "ggXXXXXXgg..pp",  // 4E
    "..............",
    "XXXXXXXXgg..pp",  // 4F
    "..............",
    "gg......pppppp",  // 50
    "..............",
    "XXpp....pppppp",  // 51
    "..............",
    "gggggg..pppppp",  // 52
    "..............",
    "XXXXgg..pppppp",  // 53
    "..............",
    "gg..pppppppppp",  // 54
    "..............",
    "XXpppppppppppp",  // 55
    "..............",
    "ggXXXXpppppppp",  // 56
    "..............",
    "XXXXXXpppppppp",  // 57
    "..............",
    "gg....XXXXpppp",  // 58
    "..............",
    "XXpp..XXXXpppp",  // 59
    "..............",
    "ggggggXXXXpppp",  // 5A
    "..............",
    "XXXXggXXXXpppp",  // 5B
    "..............",
    "gg..XXXXXXpppp",  // 5C
    "..............",
    "XXppXXXXXXpppp",  // 5D
    "..............",
    "ggXXXXXXXXpppp",  // 5E
    "..............",
    "XXXXXXXXXXpppp",  // 5F
    "..............",
    "gg........XXXX",  // 60
    "..............",
    "XXpp......XXXX",  // 61
    "..............",
    "gggggg....XXXX",  // 62
    "..............",
    "XXXXgg....XXXX",  // 63
    "..............",
    "gg..pppp..XXXX",  // 64
    "..............",
    "XXpppppp..XXXX",  // 65
    "..............",
    "ggXXXXpp..XXXX",  // 66
    "..............",
    "XXXXXXpp..XXXX",  // 67
    "..............",
    "gg....ggggXXXX",  // 68
    "..............",
    "XXpp..ggggXXXX",  // 69
    "..............",
    "ggggggggggXXXX",  // 6A
    "..............",
    "XXXXggggggXXXX",  // 6B
    "..............",
    "gg..XXXXggXXXX",  // 6C
    "..............",
    "XXppXXXXggXXXX",  // 6D
    "..............",
    "ggXXXXXXggXXXX",  // 6E
    "..............",
    "XXXXXXXXggXXXX",  // 6F
    "..............",
    "gg......XXXXXX",  // 70
    "..............",
    "XXpp....XXXXXX",  // 71
    "..............",
    "gggggg..XXXXXX",  // 72
    "..............",
    "XXXXgg..XXXXXX",  // 73
    "..............",
    "gg..ppppXXXXXX",  // 74
    "..............",
    "XXppppppXXXXXX",  // 75
    "..............",
    "ggXXXXppXXXXXX",  // 76
    "..............",
    "XXXXXXppXXXXXX",  // 77
    "..............",
    "gg....XXXXXXXX",  // 78
    "..............",
    "XXpp..XXXXXXXX",  // 79
    "..............",
    "ggggggXXXXXXXX",  // 7A
    "..............",
    "XXXXggXXXXXXXX",  // 7B
    "..............",
    "gg..XXXXXXXXXX",  // 7C
    "..............",
    "XXppXXXXXXXXXX",  // 7D
    "..............",
    "ggXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=0  and bit 0 of next byte=1

    "..............",  // 00
    "..............",
    "pppp..........",  // 01
    "..............",
    "..gggg........",  // 02
    "..............",
    "XXXXgg........",  // 03
    "..............",
    "....pppp......",  // 04
    "..............",
    "pppppppp......",  // 05
    "..............",
    "..XXXXpp......",  // 06
    "..............",
    "XXXXXXpp......",  // 07
    "..............",
    "......gggg....",  // 08
    "..............",
    "pppp..gggg....",  // 09
    "..............",
    "..gggggggg....",  // 0A
    "..............",
    "XXXXgggggg....",  // 0B
    "..............",
    "....XXXXgg....",  // 0C
    "..............",
    "ppppXXXXgg....",  // 0D
    "..............",
    "..XXXXXXgg....",  // 0E
    "..............",
    "XXXXXXXXgg....",  // 0F
    "..............",
    "........pppp..",  // 10
    "..............",
    "pppp....pppp..",  // 11
    "..............",
    "..gggg..pppp..",  // 12
    "..............",
    "XXXXgg..pppp..",  // 13
    "..............",
    "....pppppppp..",  // 14
    "..............",
    "pppppppppppp..",  // 15
    "..............",
    "..XXXXpppppp..",  // 16
    "..............",
    "XXXXXXpppppp..",  // 17
    "..............",
    "......XXXXpp..",  // 18
    "..............",
    "pppp..XXXXpp..",  // 19
    "..............",
    "..ggggXXXXpp..",  // 1A
    "..............",
    "XXXXggXXXXpp..",  // 1B
    "..............",
    "....XXXXXXpp..",  // 1C
    "..............",
    "ppppXXXXXXpp..",  // 1D
    "..............",
    "..XXXXXXXXpp..",  // 1E
    "..............",
    "XXXXXXXXXXpp..",  // 1F
    "..............",
    "..........gggg",  // 20
    "..............",
    "pppp......gggg",  // 21
    "..............",
    "..gggg....gggg",  // 22
    "..............",
    "XXXXgg....gggg",  // 23
    "..............",
    "....pppp..gggg",  // 24
    "..............",
    "pppppppp..gggg",  // 25
    "..............",
    "..XXXXpp..gggg",  // 26
    "..............",
    "XXXXXXpp..gggg",  // 27
    "..............",
    "......gggggggg",  // 28
    "..............",
    "pppp..gggggggg",  // 29
    "..............",
    "..gggggggggggg",  // 2A
    "..............",
    "XXXXgggggggggg",  // 2B
    "..............",
    "....XXXXgggggg",  // 2C
    "..............",
    "ppppXXXXgggggg",  // 2D
    "..............",
    "..XXXXXXgggggg",  // 2E
    "..............",
    "XXXXXXXXgggggg",  // 2F
    "..............",
    "........XXXXgg",  // 30
    "..............",
    "pppp....XXXXgg",  // 31
    "..............",
    "..gggg..XXXXgg",  // 32
    "..............",
    "XXXXgg..XXXXgg",  // 33
    "..............",
    "....ppppXXXXgg",  // 34
    "..............",
    "ppppppppXXXXgg",  // 35
    "..............",
    "..XXXXppXXXXgg",  // 36
    "..............",
    "XXXXXXppXXXXgg",  // 37
    "..............",
    "......XXXXXXgg",  // 38
    "..............",
    "pppp..XXXXXXgg",  // 39
    "..............",
    "..ggggXXXXXXgg",  // 3A
    "..............",
    "XXXXggXXXXXXgg",  // 3B
    "..............",
    "....XXXXXXXXgg",  // 3C
    "..............",
    "ppppXXXXXXXXgg",  // 3D
    "..............",
    "..XXXXXXXXXXgg",  // 3E
    "..............",
    "XXXXXXXXXXXXgg",  // 3F
    "..............",
    "............XX",  // 40
    "..............",
    "pppp........XX",  // 41
    "..............",
    "..gggg......XX",  // 42
    "..............",
    "XXXXgg......XX",  // 43
    "..............",
    "....pppp....XX",  // 44
    "..............",
    "pppppppp....XX",  // 45
    "..............",
    "..XXXXpp....XX",  // 46
    "..............",
    "XXXXXXpp....XX",  // 47
    "..............",
    "......gggg..XX",  // 48
    "..............",
    "pppp..gggg..XX",  // 49
    "..............",
    "..gggggggg..XX",  // 4A
    "..............",
    "XXXXgggggg..XX",  // 4B
    "..............",
    "....XXXXgg..XX",  // 4C
    "..............",
    "ppppXXXXgg..XX",  // 4D
    "..............",
    "..XXXXXXgg..XX",  // 4E
    "..............",
    "XXXXXXXXgg..XX",  // 4F
    "..............",
    "........ppppXX",  // 50
    "..............",
    "pppp....ppppXX",  // 51
    "..............",
    "..gggg..ppppXX",  // 52
    "..............",
    "XXXXgg..ppppXX",  // 53
    "..............",
    "....ppppppppXX",  // 54
    "..............",
    "ppppppppppppXX",  // 55
    "..............",
    "..XXXXppppppXX",  // 56
    "..............",
    "XXXXXXppppppXX",  // 57
    "..............",
    "......XXXXppXX",  // 58
    "..............",
    "pppp..XXXXppXX",  // 59
    "..............",
    "..ggggXXXXppXX",  // 5A
    "..............",
    "XXXXggXXXXppXX",  // 5B
    "..............",
    "....XXXXXXppXX",  // 5C
    "..............",
    "ppppXXXXXXppXX",  // 5D
    "..............",
    "..XXXXXXXXppXX",  // 5E
    "..............",
    "XXXXXXXXXXppXX",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "pppp......XXXX",  // 61
    "..............",
    "..gggg....XXXX",  // 62
    "..............",
    "XXXXgg....XXXX",  // 63
    "..............",
    "....pppp..XXXX",  // 64
    "..............",
    "pppppppp..XXXX",  // 65
    "..............",
    "..XXXXpp..XXXX",  // 66
    "..............",
    "XXXXXXpp..XXXX",  // 67
    "..............",
    "......ggggXXXX",  // 68
    "..............",
    "pppp..ggggXXXX",  // 69
    "..............",
    "..ggggggggXXXX",  // 6A
    "..............",
    "XXXXggggggXXXX",  // 6B
    "..............",
    "....XXXXggXXXX",  // 6C
    "..............",
    "ppppXXXXggXXXX",  // 6D
    "..............",
    "..XXXXXXggXXXX",  // 6E
    "..............",
    "XXXXXXXXggXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "pppp....XXXXXX",  // 71
    "..............",
    "..gggg..XXXXXX",  // 72
    "..............",
    "XXXXgg..XXXXXX",  // 73
    "..............",
    "....ppppXXXXXX",  // 74
    "..............",
    "ppppppppXXXXXX",  // 75
    "..............",
    "..XXXXppXXXXXX",  // 76
    "..............",
    "XXXXXXppXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "pppp..XXXXXXXX",  // 79
    "..............",
    "..ggggXXXXXXXX",  // 7A
    "..............",
    "XXXXggXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "ppppXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=1

    "gg............",  // 00
    "..............",
    "XXpp..........",  // 01
    "..............",
    "gggggg........",  // 02
    "..............",
    "XXXXgg........",  // 03
    "..............",
    "gg..pppp......",  // 04
    "..............",
    "XXpppppp......",  // 05
    "..............",
    "ggXXXXpp......",  // 06
    "..............",
    "XXXXXXpp......",  // 07
    "..............",
    "gg....gggg....",  // 08
    "..............",
    "XXpp..gggg....",  // 09
    "..............",
    "gggggggggg....",  // 0A
    "..............",
    "XXXXgggggg....",  // 0B
    "..............",
    "gg..XXXXgg....",  // 0C
    "..............",
    "XXppXXXXgg....",  // 0D
    "..............",
    "ggXXXXXXgg....",  // 0E
    "..............",
    "XXXXXXXXgg....",  // 0F
    "..............",
    "gg......pppp..",  // 10
    "..............",
    "XXpp....pppp..",  // 11
    "..............",
    "gggggg..pppp..",  // 12
    "..............",
    "XXXXgg..pppp..",  // 13
    "..............",
    "gg..pppppppp..",  // 14
    "..............",
    "XXpppppppppp..",  // 15
    "..............",
    "ggXXXXpppppp..",  // 16
    "..............",
    "XXXXXXpppppp..",  // 17
    "..............",
    "gg....XXXXpp..",  // 18
    "..............",
    "XXpp..XXXXpp..",  // 19
    "..............",
    "ggggggXXXXpp..",  // 1A
    "..............",
    "XXXXggXXXXpp..",  // 1B
    "..............",
    "gg..XXXXXXpp..",  // 1C
    "..............",
    "XXppXXXXXXpp..",  // 1D
    "..............",
    "ggXXXXXXXXpp..",  // 1E
    "..............",
    "XXXXXXXXXXpp..",  // 1F
    "..............",
    "gg........gggg",  // 20
    "..............",
    "XXpp......gggg",  // 21
    "..............",
    "gggggg....gggg",  // 22
    "..............",
    "XXXXgg....gggg",  // 23
    "..............",
    "gg..pppp..gggg",  // 24
    "..............",
    "XXpppppp..gggg",  // 25
    "..............",
    "ggXXXXpp..gggg",  // 26
    "..............",
    "XXXXXXpp..gggg",  // 27
    "..............",
    "gg....gggggggg",  // 28
    "..............",
    "XXpp..gggggggg",  // 29
    "..............",
    "gggggggggggggg",  // 2A
    "..............",
    "XXXXgggggggggg",  // 2B
    "..............",
    "gg..XXXXgggggg",  // 2C
    "..............",
    "XXppXXXXgggggg",  // 2D
    "..............",
    "ggXXXXXXgggggg",  // 2E
    "..............",
    "XXXXXXXXgggggg",  // 2F
    "..............",
    "gg......XXXXgg",  // 30
    "..............",
    "XXpp....XXXXgg",  // 31
    "..............",
    "gggggg..XXXXgg",  // 32
    "..............",
    "XXXXgg..XXXXgg",  // 33
    "..............",
    "gg..ppppXXXXgg",  // 34
    "..............",
    "XXppppppXXXXgg",  // 35
    "..............",
    "ggXXXXppXXXXgg",  // 36
    "..............",
    "XXXXXXppXXXXgg",  // 37
    "..............",
    "gg....XXXXXXgg",  // 38
    "..............",
    "XXpp..XXXXXXgg",  // 39
    "..............",
    "ggggggXXXXXXgg",  // 3A
    "..............",
    "XXXXggXXXXXXgg",  // 3B
    "..............",
    "gg..XXXXXXXXgg",  // 3C
    "..............",
    "XXppXXXXXXXXgg",  // 3D
    "..............",
    "ggXXXXXXXXXXgg",  // 3E
    "..............",
    "XXXXXXXXXXXXgg",  // 3F
    "..............",
    "gg..........XX",  // 40
    "..............",
    "XXpp........XX",  // 41
    "..............",
    "gggggg......XX",  // 42
    "..............",
    "XXXXgg......XX",  // 43
    "..............",
    "gg..pppp....XX",  // 44
    "..............",
    "XXpppppp....XX",  // 45
    "..............",
    "ggXXXXpp....XX",  // 46
    "..............",
    "XXXXXXpp....XX",  // 47
    "..............",
    "gg....gggg..XX",  // 48
    "..............",
    "XXpp..gggg..XX",  // 49
    "..............",
    "gggggggggg..XX",  // 4A
    "..............",
    "XXXXgggggg..XX",  // 4B
    "..............",
    "gg..XXXXgg..XX",  // 4C
    "..............",
    "XXppXXXXgg..XX",  // 4D
    "..............",
    "ggXXXXXXgg..XX",  // 4E
    "..............",
    "XXXXXXXXgg..XX",  // 4F
    "..............",
    "gg......ppppXX",  // 50
    "..............",
    "XXpp....ppppXX",  // 51
    "..............",
    "gggggg..ppppXX",  // 52
    "..............",
    "XXXXgg..ppppXX",  // 53
    "..............",
    "gg..ppppppppXX",  // 54
    "..............",
    "XXppppppppppXX",  // 55
    "..............",
    "ggXXXXppppppXX",  // 56
    "..............",
    "XXXXXXppppppXX",  // 57
    "..............",
    "gg....XXXXppXX",  // 58
    "..............",
    "XXpp..XXXXppXX",  // 59
    "..............",
    "ggggggXXXXppXX",  // 5A
    "..............",
    "XXXXggXXXXppXX",  // 5B
    "..............",
    "gg..XXXXXXppXX",  // 5C
    "..............",
    "XXppXXXXXXppXX",  // 5D
    "..............",
    "ggXXXXXXXXppXX",  // 5E
    "..............",
    "XXXXXXXXXXppXX",  // 5F
    "..............",
    "gg........XXXX",  // 60
    "..............",
    "XXpp......XXXX",  // 61
    "..............",
    "gggggg....XXXX",  // 62
    "..............",
    "XXXXgg....XXXX",  // 63
    "..............",
    "gg..pppp..XXXX",  // 64
    "..............",
    "XXpppppp..XXXX",  // 65
    "..............",
    "ggXXXXpp..XXXX",  // 66
    "..............",
    "XXXXXXpp..XXXX",  // 67
    "..............",
    "gg....ggggXXXX",  // 68
    "..............",
    "XXpp..ggggXXXX",  // 69
    "..............",
    "ggggggggggXXXX",  // 6A
    "..............",
    "XXXXggggggXXXX",  // 6B
    "..............",
    "gg..XXXXggXXXX",  // 6C
    "..............",
    "XXppXXXXggXXXX",  // 6D
    "..............",
    "ggXXXXXXggXXXX",  // 6E
    "..............",
    "XXXXXXXXggXXXX",  // 6F
    "..............",
    "gg......XXXXXX",  // 70
    "..............",
    "XXpp....XXXXXX",  // 71
    "..............",
    "gggggg..XXXXXX",  // 72
    "..............",
    "XXXXgg..XXXXXX",  // 73
    "..............",
    "gg..ppppXXXXXX",  // 74
    "..............",
    "XXppppppXXXXXX",  // 75
    "..............",
    "ggXXXXppXXXXXX",  // 76
    "..............",
    "XXXXXXppXXXXXX",  // 77
    "..............",
    "gg....XXXXXXXX",  // 78
    "..............",
    "XXpp..XXXXXXXX",  // 79
    "..............",
    "ggggggXXXXXXXX",  // 7A
    "..............",
    "XXXXggXXXXXXXX",  // 7B
    "..............",
    "gg..XXXXXXXXXX",  // 7C
    "..............",
    "XXppXXXXXXXXXX",  // 7D
    "..............",
    "ggXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",


// ************ for bytes on an odd column ***********

// bit 6 of previous byte=0  and bit 0 of next byte=0

    "..............",  // 00
    "..............",
    "gggg..........",  // 01
    "..............",
    "..pppp........",  // 02
    "..............",
    "XXXXpp........",  // 03
    "..............",
    "....gggg......",  // 04
    "..............",
    "gggggggg......",  // 05
    "..............",
    "..XXXXgg......",  // 06
    "..............",
    "XXXXXXgg......",  // 07
    "..............",
    "......pppp....",  // 08
    "..............",
    "gggg..pppp....",  // 09
    "..............",
    "..pppppppp....",  // 0A
    "..............",
    "XXXXpppppp....",  // 0B
    "..............",
    "....XXXXpp....",  // 0C
    "..............",
    "ggggXXXXpp....",  // 0D
    "..............",
    "..XXXXXXpp....",  // 0E
    "..............",
    "XXXXXXXXpp....",  // 0F
    "..............",
    "........gggg..",  // 10
    "..............",
    "gggg....gggg..",  // 11
    "..............",
    "..pppp..gggg..",  // 12
    "..............",
    "XXXXpp..gggg..",  // 13
    "..............",
    "....gggggggg..",  // 14
    "..............",
    "gggggggggggg..",  // 15
    "..............",
    "..XXXXgggggg..",  // 16
    "..............",
    "XXXXXXgggggg..",  // 17
    "..............",
    "......XXXXgg..",  // 18
    "..............",
    "gggg..XXXXgg..",  // 19
    "..............",
    "..ppppXXXXgg..",  // 1A
    "..............",
    "XXXXppXXXXgg..",  // 1B
    "..............",
    "....XXXXXXgg..",  // 1C
    "..............",
    "ggggXXXXXXgg..",  // 1D
    "..............",
    "..XXXXXXXXgg..",  // 1E
    "..............",
    "XXXXXXXXXXgg..",  // 1F
    "..............",
    "..........pppp",  // 20
    "..............",
    "gggg......pppp",  // 21
    "..............",
    "..pppp....pppp",  // 22
    "..............",
    "XXXXpp....pppp",  // 23
    "..............",
    "....gggg..pppp",  // 24
    "..............",
    "gggggggg..pppp",  // 25
    "..............",
    "..XXXXgg..pppp",  // 26
    "..............",
    "XXXXXXgg..pppp",  // 27
    "..............",
    "......pppppppp",  // 28
    "..............",
    "gggg..pppppppp",  // 29
    "..............",
    "..pppppppppppp",  // 2A
    "..............",
    "XXXXpppppppppp",  // 2B
    "..............",
    "....XXXXpppppp",  // 2C
    "..............",
    "ggggXXXXpppppp",  // 2D
    "..............",
    "..XXXXXXpppppp",  // 2E
    "..............",
    "XXXXXXXXpppppp",  // 2F
    "..............",
    "........XXXXpp",  // 30
    "..............",
    "gggg....XXXXpp",  // 31
    "..............",
    "..pppp..XXXXpp",  // 32
    "..............",
    "XXXXpp..XXXXpp",  // 33
    "..............",
    "....ggggXXXXpp",  // 34
    "..............",
    "ggggggggXXXXpp",  // 35
    "..............",
    "..XXXXggXXXXpp",  // 36
    "..............",
    "XXXXXXggXXXXpp",  // 37
    "..............",
    "......XXXXXXpp",  // 38
    "..............",
    "gggg..XXXXXXpp",  // 39
    "..............",
    "..ppppXXXXXXpp",  // 3A
    "..............",
    "XXXXppXXXXXXpp",  // 3B
    "..............",
    "....XXXXXXXXpp",  // 3C
    "..............",
    "ggggXXXXXXXXpp",  // 3D
    "..............",
    "..XXXXXXXXXXpp",  // 3E
    "..............",
    "XXXXXXXXXXXXpp",  // 3F
    "..............",
    "............gg",  // 40
    "..............",
    "gggg........gg",  // 41
    "..............",
    "..pppp......gg",  // 42
    "..............",
    "XXXXpp......gg",  // 43
    "..............",
    "....gggg....gg",  // 44
    "..............",
    "gggggggg....gg",  // 45
    "..............",
    "..XXXXgg....gg",  // 46
    "..............",
    "XXXXXXgg....gg",  // 47
    "..............",
    "......pppp..gg",  // 48
    "..............",
    "gggg..pppp..gg",  // 49
    "..............",
    "..pppppppp..gg",  // 4A
    "..............",
    "XXXXpppppp..gg",  // 4B
    "..............",
    "....XXXXpp..gg",  // 4C
    "..............",
    "ggggXXXXpp..gg",  // 4D
    "..............",
    "..XXXXXXpp..gg",  // 4E
    "..............",
    "XXXXXXXXpp..gg",  // 4F
    "..............",
    "........gggggg",  // 50
    "..............",
    "gggg....gggggg",  // 51
    "..............",
    "..pppp..gggggg",  // 52
    "..............",
    "XXXXpp..gggggg",  // 53
    "..............",
    "....gggggggggg",  // 54
    "..............",
    "gggggggggggggg",  // 55
    "..............",
    "..XXXXgggggggg",  // 56
    "..............",
    "XXXXXXgggggggg",  // 57
    "..............",
    "......XXXXgggg",  // 58
    "..............",
    "gggg..XXXXgggg",  // 59
    "..............",
    "..ppppXXXXgggg",  // 5A
    "..............",
    "XXXXppXXXXgggg",  // 5B
    "..............",
    "....XXXXXXgggg",  // 5C
    "..............",
    "ggggXXXXXXgggg",  // 5D
    "..............",
    "..XXXXXXXXgggg",  // 5E
    "..............",
    "XXXXXXXXXXgggg",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "gggg......XXXX",  // 61
    "..............",
    "..pppp....XXXX",  // 62
    "..............",
    "XXXXpp....XXXX",  // 63
    "..............",
    "....gggg..XXXX",  // 64
    "..............",
    "gggggggg..XXXX",  // 65
    "..............",
    "..XXXXgg..XXXX",  // 66
    "..............",
    "XXXXXXgg..XXXX",  // 67
    "..............",
    "......ppppXXXX",  // 68
    "..............",
    "gggg..ppppXXXX",  // 69
    "..............",
    "..ppppppppXXXX",  // 6A
    "..............",
    "XXXXppppppXXXX",  // 6B
    "..............",
    "....XXXXppXXXX",  // 6C
    "..............",
    "ggggXXXXppXXXX",  // 6D
    "..............",
    "..XXXXXXppXXXX",  // 6E
    "..............",
    "XXXXXXXXppXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "gggg....XXXXXX",  // 71
    "..............",
    "..pppp..XXXXXX",  // 72
    "..............",
    "XXXXpp..XXXXXX",  // 73
    "..............",
    "....ggggXXXXXX",  // 74
    "..............",
    "ggggggggXXXXXX",  // 75
    "..............",
    "..XXXXggXXXXXX",  // 76
    "..............",
    "XXXXXXggXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "gggg..XXXXXXXX",  // 79
    "..............",
    "..ppppXXXXXXXX",  // 7A
    "..............",
    "XXXXppXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "ggggXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=0

    "pp............",  // 00
    "..............",
    "XXgg..........",  // 01
    "..............",
    "pppppp........",  // 02
    "..............",
    "XXXXpp........",  // 03
    "..............",
    "pp..gggg......",  // 04
    "..............",
    "XXgggggg......",  // 05
    "..............",
    "ppXXXXgg......",  // 06
    "..............",
    "XXXXXXgg......",  // 07
    "..............",
    "pp....pppp....",  // 08
    "..............",
    "XXgg..pppp....",  // 09
    "..............",
    "pppppppppp....",  // 0A
    "..............",
    "XXXXpppppp....",  // 0B
    "..............",
    "pp..XXXXpp....",  // 0C
    "..............",
    "XXggXXXXpp....",  // 0D
    "..............",
    "ppXXXXXXpp....",  // 0E
    "..............",
    "XXXXXXXXpp....",  // 0F
    "..............",
    "pp......gggg..",  // 10
    "..............",
    "XXgg....gggg..",  // 11
    "..............",
    "pppppp..gggg..",  // 12
    "..............",
    "XXXXpp..gggg..",  // 13
    "..............",
    "pp..gggggggg..",  // 14
    "..............",
    "XXgggggggggg..",  // 15
    "..............",
    "ppXXXXgggggg..",  // 16
    "..............",
    "XXXXXXgggggg..",  // 17
    "..............",
    "pp....XXXXgg..",  // 18
    "..............",
    "XXgg..XXXXgg..",  // 19
    "..............",
    "ppppppXXXXgg..",  // 1A
    "..............",
    "XXXXppXXXXgg..",  // 1B
    "..............",
    "pp..XXXXXXgg..",  // 1C
    "..............",
    "XXggXXXXXXgg..",  // 1D
    "..............",
    "ppXXXXXXXXgg..",  // 1E
    "..............",
    "XXXXXXXXXXgg..",  // 1F
    "..............",
    "pp........pppp",  // 20
    "..............",
    "XXgg......pppp",  // 21
    "..............",
    "pppppp....pppp",  // 22
    "..............",
    "XXXXpp....pppp",  // 23
    "..............",
    "pp..gggg..pppp",  // 24
    "..............",
    "XXgggggg..pppp",  // 25
    "..............",
    "ppXXXXgg..pppp",  // 26
    "..............",
    "XXXXXXgg..pppp",  // 27
    "..............",
    "pp....pppppppp",  // 28
    "..............",
    "XXgg..pppppppp",  // 29
    "..............",
    "pppppppppppppp",  // 2A
    "..............",
    "XXXXpppppppppp",  // 2B
    "..............",
    "pp..XXXXpppppp",  // 2C
    "..............",
    "XXggXXXXpppppp",  // 2D
    "..............",
    "ppXXXXXXpppppp",  // 2E
    "..............",
    "XXXXXXXXpppppp",  // 2F
    "..............",
    "pp......XXXXpp",  // 30
    "..............",
    "XXgg....XXXXpp",  // 31
    "..............",
    "pppppp..XXXXpp",  // 32
    "..............",
    "XXXXpp..XXXXpp",  // 33
    "..............",
    "pp..ggggXXXXpp",  // 34
    "..............",
    "XXggggggXXXXpp",  // 35
    "..............",
    "ppXXXXggXXXXpp",  // 36
    "..............",
    "XXXXXXggXXXXpp",  // 37
    "..............",
    "pp....XXXXXXpp",  // 38
    "..............",
    "XXgg..XXXXXXpp",  // 39
    "..............",
    "ppppppXXXXXXpp",  // 3A
    "..............",
    "XXXXppXXXXXXpp",  // 3B
    "..............",
    "pp..XXXXXXXXpp",  // 3C
    "..............",
    "XXggXXXXXXXXpp",  // 3D
    "..............",
    "ppXXXXXXXXXXpp",  // 3E
    "..............",
    "XXXXXXXXXXXXpp",  // 3F
    "..............",
    "pp..........gg",  // 40
    "..............",
    "XXgg........gg",  // 41
    "..............",
    "pppppp......gg",  // 42
    "..............",
    "XXXXpp......gg",  // 43
    "..............",
    "pp..gggg....gg",  // 44
    "..............",
    "XXgggggg....gg",  // 45
    "..............",
    "ppXXXXgg....gg",  // 46
    "..............",
    "XXXXXXgg....gg",  // 47
    "..............",
    "pp....pppp..gg",  // 48
    "..............",
    "XXgg..pppp..gg",  // 49
    "..............",
    "pppppppppp..gg",  // 4A
    "..............",
    "XXXXpppppp..gg",  // 4B
    "..............",
    "pp..XXXXpp..gg",  // 4C
    "..............",
    "XXggXXXXpp..gg",  // 4D
    "..............",
    "ppXXXXXXpp..gg",  // 4E
    "..............",
    "XXXXXXXXpp..gg",  // 4F
    "..............",
    "pp......gggggg",  // 50
    "..............",
    "XXgg....gggggg",  // 51
    "..............",
    "pppppp..gggggg",  // 52
    "..............",
    "XXXXpp..gggggg",  // 53
    "..............",
    "pp..gggggggggg",  // 54
    "..............",
    "XXgggggggggggg",  // 55
    "..............",
    "ppXXXXgggggggg",  // 56
    "..............",
    "XXXXXXgggggggg",  // 57
    "..............",
    "pp....XXXXgggg",  // 58
    "..............",
    "XXgg..XXXXgggg",  // 59
    "..............",
    "ppppppXXXXgggg",  // 5A
    "..............",
    "XXXXppXXXXgggg",  // 5B
    "..............",
    "pp..XXXXXXgggg",  // 5C
    "..............",
    "XXggXXXXXXgggg",  // 5D
    "..............",
    "ppXXXXXXXXgggg",  // 5E
    "..............",
    "XXXXXXXXXXgggg",  // 5F
    "..............",
    "pp........XXXX",  // 60
    "..............",
    "XXgg......XXXX",  // 61
    "..............",
    "pppppp....XXXX",  // 62
    "..............",
    "XXXXpp....XXXX",  // 63
    "..............",
    "pp..gggg..XXXX",  // 64
    "..............",
    "XXgggggg..XXXX",  // 65
    "..............",
    "ppXXXXgg..XXXX",  // 66
    "..............",
    "XXXXXXgg..XXXX",  // 67
    "..............",
    "pp....ppppXXXX",  // 68
    "..............",
    "XXgg..ppppXXXX",  // 69
    "..............",
    "ppppppppppXXXX",  // 6A
    "..............",
    "XXXXppppppXXXX",  // 6B
    "..............",
    "pp..XXXXppXXXX",  // 6C
    "..............",
    "XXggXXXXppXXXX",  // 6D
    "..............",
    "ppXXXXXXppXXXX",  // 6E
    "..............",
    "XXXXXXXXppXXXX",  // 6F
    "..............",
    "pp......XXXXXX",  // 70
    "..............",
    "XXgg....XXXXXX",  // 71
    "..............",
    "pppppp..XXXXXX",  // 72
    "..............",
    "XXXXpp..XXXXXX",  // 73
    "..............",
    "pp..ggggXXXXXX",  // 74
    "..............",
    "XXggggggXXXXXX",  // 75
    "..............",
    "ppXXXXggXXXXXX",  // 76
    "..............",
    "XXXXXXggXXXXXX",  // 77
    "..............",
    "pp....XXXXXXXX",  // 78
    "..............",
    "XXgg..XXXXXXXX",  // 79
    "..............",
    "ppppppXXXXXXXX",  // 7A
    "..............",
    "XXXXppXXXXXXXX",  // 7B
    "..............",
    "pp..XXXXXXXXXX",  // 7C
    "..............",
    "XXggXXXXXXXXXX",  // 7D
    "..............",
    "ppXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=0  and bit 0 of next byte=1

    "..............",  // 00
    "..............",
    "gggg..........",  // 01
    "..............",
    "..pppp........",  // 02
    "..............",
    "XXXXpp........",  // 03
    "..............",
    "....gggg......",  // 04
    "..............",
    "gggggggg......",  // 05
    "..............",
    "..XXXXgg......",  // 06
    "..............",
    "XXXXXXgg......",  // 07
    "..............",
    "......pppp....",  // 08
    "..............",
    "gggg..pppp....",  // 09
    "..............",
    "..pppppppp....",  // 0A
    "..............",
    "XXXXpppppp....",  // 0B
    "..............",
    "....XXXXpp....",  // 0C
    "..............",
    "ggggXXXXpp....",  // 0D
    "..............",
    "..XXXXXXpp....",  // 0E
    "..............",
    "XXXXXXXXpp....",  // 0F
    "..............",
    "........gggg..",  // 10
    "..............",
    "gggg....gggg..",  // 11
    "..............",
    "..pppp..gggg..",  // 12
    "..............",
    "XXXXpp..gggg..",  // 13
    "..............",
    "....gggggggg..",  // 14
    "..............",
    "gggggggggggg..",  // 15
    "..............",
    "..XXXXgggggg..",  // 16
    "..............",
    "XXXXXXgggggg..",  // 17
    "..............",
    "......XXXXgg..",  // 18
    "..............",
    "gggg..XXXXgg..",  // 19
    "..............",
    "..ppppXXXXgg..",  // 1A
    "..............",
    "XXXXppXXXXgg..",  // 1B
    "..............",
    "....XXXXXXgg..",  // 1C
    "..............",
    "ggggXXXXXXgg..",  // 1D
    "..............",
    "..XXXXXXXXgg..",  // 1E
    "..............",
    "XXXXXXXXXXgg..",  // 1F
    "..............",
    "..........pppp",  // 20
    "..............",
    "gggg......pppp",  // 21
    "..............",
    "..pppp....pppp",  // 22
    "..............",
    "XXXXpp....pppp",  // 23
    "..............",
    "....gggg..pppp",  // 24
    "..............",
    "gggggggg..pppp",  // 25
    "..............",
    "..XXXXgg..pppp",  // 26
    "..............",
    "XXXXXXgg..pppp",  // 27
    "..............",
    "......pppppppp",  // 28
    "..............",
    "gggg..pppppppp",  // 29
    "..............",
    "..pppppppppppp",  // 2A
    "..............",
    "XXXXpppppppppp",  // 2B
    "..............",
    "....XXXXpppppp",  // 2C
    "..............",
    "ggggXXXXpppppp",  // 2D
    "..............",
    "..XXXXXXpppppp",  // 2E
    "..............",
    "XXXXXXXXpppppp",  // 2F
    "..............",
    "........XXXXpp",  // 30
    "..............",
    "gggg....XXXXpp",  // 31
    "..............",
    "..pppp..XXXXpp",  // 32
    "..............",
    "XXXXpp..XXXXpp",  // 33
    "..............",
    "....ggggXXXXpp",  // 34
    "..............",
    "ggggggggXXXXpp",  // 35
    "..............",
    "..XXXXggXXXXpp",  // 36
    "..............",
    "XXXXXXggXXXXpp",  // 37
    "..............",
    "......XXXXXXpp",  // 38
    "..............",
    "gggg..XXXXXXpp",  // 39
    "..............",
    "..ppppXXXXXXpp",  // 3A
    "..............",
    "XXXXppXXXXXXpp",  // 3B
    "..............",
    "....XXXXXXXXpp",  // 3C
    "..............",
    "ggggXXXXXXXXpp",  // 3D
    "..............",
    "..XXXXXXXXXXpp",  // 3E
    "..............",
    "XXXXXXXXXXXXpp",  // 3F
    "..............",
    "............XX",  // 40
    "..............",
    "gggg........XX",  // 41
    "..............",
    "..pppp......XX",  // 42
    "..............",
    "XXXXpp......XX",  // 43
    "..............",
    "....gggg....XX",  // 44
    "..............",
    "gggggggg....XX",  // 45
    "..............",
    "..XXXXgg....XX",  // 46
    "..............",
    "XXXXXXgg....XX",  // 47
    "..............",
    "......pppp..XX",  // 48
    "..............",
    "gggg..pppp..XX",  // 49
    "..............",
    "..pppppppp..XX",  // 4A
    "..............",
    "XXXXpppppp..XX",  // 4B
    "..............",
    "....XXXXpp..XX",  // 4C
    "..............",
    "ggggXXXXpp..XX",  // 4D
    "..............",
    "..XXXXXXpp..XX",  // 4E
    "..............",
    "XXXXXXXXpp..XX",  // 4F
    "..............",
    "........ggggXX",  // 50
    "..............",
    "gggg....ggggXX",  // 51
    "..............",
    "..pppp..ggggXX",  // 52
    "..............",
    "XXXXpp..ggggXX",  // 53
    "..............",
    "....ggggggggXX",  // 54
    "..............",
    "ggggggggggggXX",  // 55
    "..............",
    "..XXXXggggggXX",  // 56
    "..............",
    "XXXXXXggggggXX",  // 57
    "..............",
    "......XXXXggXX",  // 58
    "..............",
    "gggg..XXXXggXX",  // 59
    "..............",
    "..ppppXXXXggXX",  // 5A
    "..............",
    "XXXXppXXXXggXX",  // 5B
    "..............",
    "....XXXXXXggXX",  // 5C
    "..............",
    "ggggXXXXXXggXX",  // 5D
    "..............",
    "..XXXXXXXXggXX",  // 5E
    "..............",
    "XXXXXXXXXXggXX",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "gggg......XXXX",  // 61
    "..............",
    "..pppp....XXXX",  // 62
    "..............",
    "XXXXpp....XXXX",  // 63
    "..............",
    "....gggg..XXXX",  // 64
    "..............",
    "gggggggg..XXXX",  // 65
    "..............",
    "..XXXXgg..XXXX",  // 66
    "..............",
    "XXXXXXgg..XXXX",  // 67
    "..............",
    "......ppppXXXX",  // 68
    "..............",
    "gggg..ppppXXXX",  // 69
    "..............",
    "..ppppppppXXXX",  // 6A
    "..............",
    "XXXXppppppXXXX",  // 6B
    "..............",
    "....XXXXppXXXX",  // 6C
    "..............",
    "ggggXXXXppXXXX",  // 6D
    "..............",
    "..XXXXXXppXXXX",  // 6E
    "..............",
    "XXXXXXXXppXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "gggg....XXXXXX",  // 71
    "..............",
    "..pppp..XXXXXX",  // 72
    "..............",
    "XXXXpp..XXXXXX",  // 73
    "..............",
    "....ggggXXXXXX",  // 74
    "..............",
    "ggggggggXXXXXX",  // 75
    "..............",
    "..XXXXggXXXXXX",  // 76
    "..............",
    "XXXXXXggXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "gggg..XXXXXXXX",  // 79
    "..............",
    "..ppppXXXXXXXX",  // 7A
    "..............",
    "XXXXppXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "ggggXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=1

    "pp............",  // 00
    "..............",
    "XXgg..........",  // 01
    "..............",
    "pppppp........",  // 02
    "..............",
    "XXXXpp........",  // 03
    "..............",
    "pp..gggg......",  // 04
    "..............",
    "XXgggggg......",  // 05
    "..............",
    "ppXXXXgg......",  // 06
    "..............",
    "XXXXXXgg......",  // 07
    "..............",
    "pp....pppp....",  // 08
    "..............",
    "XXgg..pppp....",  // 09
    "..............",
    "pppppppppp....",  // 0A
    "..............",
    "XXXXpppppp....",  // 0B
    "..............",
    "pp..XXXXpp....",  // 0C
    "..............",
    "XXggXXXXpp....",  // 0D
    "..............",
    "ppXXXXXXpp....",  // 0E
    "..............",
    "XXXXXXXXpp....",  // 0F
    "..............",
    "pp......gggg..",  // 10
    "..............",
    "XXgg....gggg..",  // 11
    "..............",
    "pppppp..gggg..",  // 12
    "..............",
    "XXXXpp..gggg..",  // 13
    "..............",
    "pp..gggggggg..",  // 14
    "..............",
    "XXgggggggggg..",  // 15
    "..............",
    "ppXXXXgggggg..",  // 16
    "..............",
    "XXXXXXgggggg..",  // 17
    "..............",
    "pp....XXXXgg..",  // 18
    "..............",
    "XXgg..XXXXgg..",  // 19
    "..............",
    "ppppppXXXXgg..",  // 1A
    "..............",
    "XXXXppXXXXgg..",  // 1B
    "..............",
    "pp..XXXXXXgg..",  // 1C
    "..............",
    "XXggXXXXXXgg..",  // 1D
    "..............",
    "ppXXXXXXXXgg..",  // 1E
    "..............",
    "XXXXXXXXXXgg..",  // 1F
    "..............",
    "pp........pppp",  // 20
    "..............",
    "XXgg......pppp",  // 21
    "..............",
    "pppppp....pppp",  // 22
    "..............",
    "XXXXpp....pppp",  // 23
    "..............",
    "pp..gggg..pppp",  // 24
    "..............",
    "XXgggggg..pppp",  // 25
    "..............",
    "ppXXXXgg..pppp",  // 26
    "..............",
    "XXXXXXgg..pppp",  // 27
    "..............",
    "pp....pppppppp",  // 28
    "..............",
    "XXgg..pppppppp",  // 29
    "..............",
    "pppppppppppppp",  // 2A
    "..............",
    "XXXXpppppppppp",  // 2B
    "..............",
    "pp..XXXXpppppp",  // 2C
    "..............",
    "XXggXXXXpppppp",  // 2D
    "..............",
    "ppXXXXXXpppppp",  // 2E
    "..............",
    "XXXXXXXXpppppp",  // 2F
    "..............",
    "pp......XXXXpp",  // 30
    "..............",
    "XXgg....XXXXpp",  // 31
    "..............",
    "pppppp..XXXXpp",  // 32
    "..............",
    "XXXXpp..XXXXpp",  // 33
    "..............",
    "pp..ggggXXXXpp",  // 34
    "..............",
    "XXggggggXXXXpp",  // 35
    "..............",
    "ppXXXXggXXXXpp",  // 36
    "..............",
    "XXXXXXggXXXXpp",  // 37
    "..............",
    "pp....XXXXXXpp",  // 38
    "..............",
    "XXgg..XXXXXXpp",  // 39
    "..............",
    "ppppppXXXXXXpp",  // 3A
    "..............",
    "XXXXppXXXXXXpp",  // 3B
    "..............",
    "pp..XXXXXXXXpp",  // 3C
    "..............",
    "XXggXXXXXXXXpp",  // 3D
    "..............",
    "ppXXXXXXXXXXpp",  // 3E
    "..............",
    "XXXXXXXXXXXXpp",  // 3F
    "..............",
    "pp..........XX",  // 40
    "..............",
    "XXgg........XX",  // 41
    "..............",
    "pppppp......XX",  // 42
    "..............",
    "XXXXpp......XX",  // 43
    "..............",
    "pp..gggg....XX",  // 44
    "..............",
    "XXgggggg....XX",  // 45
    "..............",
    "ppXXXXgg....XX",  // 46
    "..............",
    "XXXXXXgg....XX",  // 47
    "..............",
    "pp....pppp..XX",  // 48
    "..............",
    "XXgg..pppp..XX",  // 49
    "..............",
    "pppppppppp..XX",  // 4A
    "..............",
    "XXXXpppppp..XX",  // 4B
    "..............",
    "pp..XXXXpp..XX",  // 4C
    "..............",
    "XXggXXXXpp..XX",  // 4D
    "..............",
    "ppXXXXXXpp..XX",  // 4E
    "..............",
    "XXXXXXXXpp..XX",  // 4F
    "..............",
    "pp......ggggXX",  // 50
    "..............",
    "XXgg....ggggXX",  // 51
    "..............",
    "pppppp..ggggXX",  // 52
    "..............",
    "XXXXpp..ggggXX",  // 53
    "..............",
    "pp..ggggggggXX",  // 54
    "..............",
    "XXggggggggggXX",  // 55
    "..............",
    "ppXXXXggggggXX",  // 56
    "..............",
    "XXXXXXggggggXX",  // 57
    "..............",
    "pp....XXXXggXX",  // 58
    "..............",
    "XXgg..XXXXggXX",  // 59
    "..............",
    "ppppppXXXXggXX",  // 5A
    "..............",
    "XXXXppXXXXggXX",  // 5B
    "..............",
    "pp..XXXXXXggXX",  // 5C
    "..............",
    "XXggXXXXXXggXX",  // 5D
    "..............",
    "ppXXXXXXXXggXX",  // 5E
    "..............",
    "XXXXXXXXXXggXX",  // 5F
    "..............",
    "pp........XXXX",  // 60
    "..............",
    "XXgg......XXXX",  // 61
    "..............",
    "pppppp....XXXX",  // 62
    "..............",
    "XXXXpp....XXXX",  // 63
    "..............",
    "pp..gggg..XXXX",  // 64
    "..............",
    "XXgggggg..XXXX",  // 65
    "..............",
    "ppXXXXgg..XXXX",  // 66
    "..............",
    "XXXXXXgg..XXXX",  // 67
    "..............",
    "pp....ppppXXXX",  // 68
    "..............",
    "XXgg..ppppXXXX",  // 69
    "..............",
    "ppppppppppXXXX",  // 6A
    "..............",
    "XXXXppppppXXXX",  // 6B
    "..............",
    "pp..XXXXppXXXX",  // 6C
    "..............",
    "XXggXXXXppXXXX",  // 6D
    "..............",
    "ppXXXXXXppXXXX",  // 6E
    "..............",
    "XXXXXXXXppXXXX",  // 6F
    "..............",
    "pp......XXXXXX",  // 70
    "..............",
    "XXgg....XXXXXX",  // 71
    "..............",
    "pppppp..XXXXXX",  // 72
    "..............",
    "XXXXpp..XXXXXX",  // 73
    "..............",
    "pp..ggggXXXXXX",  // 74
    "..............",
    "XXggggggXXXXXX",  // 75
    "..............",
    "ppXXXXggXXXXXX",  // 76
    "..............",
    "XXXXXXggXXXXXX",  // 77
    "..............",
    "pp....XXXXXXXX",  // 78
    "..............",
    "XXgg..XXXXXXXX",  // 79
    "..............",
    "ppppppXXXXXXXX",  // 7A
    "..............",
    "XXXXppXXXXXXXX",  // 7B
    "..............",
    "pp..XXXXXXXXXX",  // 7C
    "..............",
    "XXggXXXXXXXXXX",  // 7D
    "..............",
    "ppXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    ".............."
} ;



/***************************************************/
/*                                                 */
/*    xpm_hires_1 is used if bit 7 of byte is 1    */
/*                                                 */
/***************************************************/

const char* xpm_hires_1[] =
{
    "14 2048 4 1",
    ". c #000000",
    "X c #FFFFFF",
    "b c #4090FF",
    "o c #FFA000",

// ************ for bytes on an even column ***********

// bit 6 of previous byte=0  and bit 0 of next byte=0

    "..............",  // 00
    "..............",
    "bbbb..........",  // 01
    "..............",
    "..oooo........",  // 02
    "..............",
    "XXXXoo........",  // 03
    "..............",
    "....bbbb......",  // 04
    "..............",
    "bbbbbbbb......",  // 05
    "..............",
    "..XXXXbb......",  // 06
    "..............",
    "XXXXXXbb......",  // 07
    "..............",
    "......oooo....",  // 08
    "..............",
    "bbbb..oooo....",  // 09
    "..............",
    "..oooooooo....",  // 0A
    "..............",
    "XXXXoooooo....",  // 0B
    "..............",
    "....XXXXoo....",  // 0C
    "..............",
    "bbbbXXXXoo....",  // 0D
    "..............",
    "..XXXXXXoo....",  // 0E
    "..............",
    "XXXXXXXXoo....",  // 0F
    "..............",
    "........bbbb..",  // 10
    "..............",
    "bbbb....bbbb..",  // 11
    "..............",
    "..oooo..bbbb..",  // 12
    "..............",
    "XXXXoo..bbbb..",  // 13
    "..............",
    "....bbbbbbbb..",  // 14
    "..............",
    "bbbbbbbbbbbb..",  // 15
    "..............",
    "..XXXXbbbbbb..",  // 16
    "..............",
    "XXXXXXbbbbbb..",  // 17
    "..............",
    "......XXXXbb..",  // 18
    "..............",
    "bbbb..XXXXbb..",  // 19
    "..............",
    "..ooooXXXXbb..",  // 1A
    "..............",
    "XXXXooXXXXbb..",  // 1B
    "..............",
    "....XXXXXXbb..",  // 1C
    "..............",
    "bbbbXXXXXXbb..",  // 1D
    "..............",
    "..XXXXXXXXbb..",  // 1E
    "..............",
    "XXXXXXXXXXbb..",  // 1F
    "..............",
    "..........oooo",  // 20
    "..............",
    "bbbb......oooo",  // 21
    "..............",
    "..oooo....oooo",  // 22
    "..............",
    "XXXXoo....oooo",  // 23
    "..............",
    "....bbbb..oooo",  // 24
    "..............",
    "bbbbbbbb..oooo",  // 25
    "..............",
    "..XXXXbb..oooo",  // 26
    "..............",
    "XXXXXXbb..oooo",  // 27
    "..............",
    "......oooooooo",  // 28
    "..............",
    "bbbb..oooooooo",  // 29
    "..............",
    "..oooooooooooo",  // 2A
    "..............",
    "XXXXoooooooooo",  // 2B
    "..............",
    "....XXXXoooooo",  // 2C
    "..............",
    "bbbbXXXXoooooo",  // 2D
    "..............",
    "..XXXXXXoooooo",  // 2E
    "..............",
    "XXXXXXXXoooooo",  // 2F
    "..............",
    "........XXXXoo",  // 30
    "..............",
    "bbbb....XXXXoo",  // 31
    "..............",
    "..oooo..XXXXoo",  // 32
    "..............",
    "XXXXoo..XXXXoo",  // 33
    "..............",
    "....bbbbXXXXoo",  // 34
    "..............",
    "bbbbbbbbXXXXoo",  // 35
    "..............",
    "..XXXXbbXXXXoo",  // 36
    "..............",
    "XXXXXXbbXXXXoo",  // 37
    "..............",
    "......XXXXXXoo",  // 38
    "..............",
    "bbbb..XXXXXXoo",  // 39
    "..............",
    "..ooooXXXXXXoo",  // 3A
    "..............",
    "XXXXooXXXXXXoo",  // 3B
    "..............",
    "....XXXXXXXXoo",  // 3C
    "..............",
    "bbbbXXXXXXXXoo",  // 3D
    "..............",
    "..XXXXXXXXXXoo",  // 3E
    "..............",
    "XXXXXXXXXXXXoo",  // 3F
    "..............",
    "............bb",  // 40
    "..............",
    "bbbb........bb",  // 41
    "..............",
    "..oooo......bb",  // 42
    "..............",
    "XXXXoo......bb",  // 43
    "..............",
    "....bbbb....bb",  // 44
    "..............",
    "bbbbbbbb....bb",  // 45
    "..............",
    "..XXXXbb....bb",  // 46
    "..............",
    "XXXXXXbb....bb",  // 47
    "..............",
    "......oooo..bb",  // 48
    "..............",
    "bbbb..oooo..bb",  // 49
    "..............",
    "..oooooooo..bb",  // 4A
    "..............",
    "XXXXoooooo..bb",  // 4B
    "..............",
    "....XXXXoo..bb",  // 4C
    "..............",
    "bbbbXXXXoo..bb",  // 4D
    "..............",
    "..XXXXXXoo..bb",  // 4E
    "..............",
    "XXXXXXXXoo..bb",  // 4F
    "..............",
    "........bbbbbb",  // 50
    "..............",
    "bbbb....bbbbbb",  // 51
    "..............",
    "..oooo..bbbbbb",  // 52
    "..............",
    "XXXXoo..bbbbbb",  // 53
    "..............",
    "....bbbbbbbbbb",  // 54
    "..............",
    "bbbbbbbbbbbbbb",  // 55
    "..............",
    "..XXXXbbbbbbbb",  // 56
    "..............",
    "XXXXXXbbbbbbbb",  // 57
    "..............",
    "......XXXXbbbb",  // 58
    "..............",
    "bbbb..XXXXbbbb",  // 59
    "..............",
    "..ooooXXXXbbbb",  // 5A
    "..............",
    "XXXXooXXXXbbbb",  // 5B
    "..............",
    "....XXXXXXbbbb",  // 5C
    "..............",
    "bbbbXXXXXXbbbb",  // 5D
    "..............",
    "..XXXXXXXXbbbb",  // 5E
    "..............",
    "XXXXXXXXXXbbbb",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "bbbb......XXXX",  // 61
    "..............",
    "..oooo....XXXX",  // 62
    "..............",
    "XXXXoo....XXXX",  // 63
    "..............",
    "....bbbb..XXXX",  // 64
    "..............",
    "bbbbbbbb..XXXX",  // 65
    "..............",
    "..XXXXbb..XXXX",  // 66
    "..............",
    "XXXXXXbb..XXXX",  // 67
    "..............",
    "......ooooXXXX",  // 68
    "..............",
    "bbbb..ooooXXXX",  // 69
    "..............",
    "..ooooooooXXXX",  // 6A
    "..............",
    "XXXXooooooXXXX",  // 6B
    "..............",
    "....XXXXooXXXX",  // 6C
    "..............",
    "bbbbXXXXooXXXX",  // 6D
    "..............",
    "..XXXXXXooXXXX",  // 6E
    "..............",
    "XXXXXXXXooXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "bbbb....XXXXXX",  // 71
    "..............",
    "..oooo..XXXXXX",  // 72
    "..............",
    "XXXXoo..XXXXXX",  // 73
    "..............",
    "....bbbbXXXXXX",  // 74
    "..............",
    "bbbbbbbbXXXXXX",  // 75
    "..............",
    "..XXXXbbXXXXXX",  // 76
    "..............",
    "XXXXXXbbXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "bbbb..XXXXXXXX",  // 79
    "..............",
    "..ooooXXXXXXXX",  // 7A
    "..............",
    "XXXXooXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "bbbbXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=0

    "oo............",  // 00
    "..............",
    "XXbb..........",  // 01
    "..............",
    "oooooo........",  // 02
    "..............",
    "XXXXoo........",  // 03
    "..............",
    "oo..bbbb......",  // 04
    "..............",
    "XXbbbbbb......",  // 05
    "..............",
    "ooXXXXbb......",  // 06
    "..............",
    "XXXXXXbb......",  // 07
    "..............",
    "oo....oooo....",  // 08
    "..............",
    "XXbb..oooo....",  // 09
    "..............",
    "oooooooooo....",  // 0A
    "..............",
    "XXXXoooooo....",  // 0B
    "..............",
    "oo..XXXXoo....",  // 0C
    "..............",
    "XXbbXXXXoo....",  // 0D
    "..............",
    "ooXXXXXXoo....",  // 0E
    "..............",
    "XXXXXXXXoo....",  // 0F
    "..............",
    "oo......bbbb..",  // 10
    "..............",
    "XXbb....bbbb..",  // 11
    "..............",
    "oooooo..bbbb..",  // 12
    "..............",
    "XXXXoo..bbbb..",  // 13
    "..............",
    "oo..bbbbbbbb..",  // 14
    "..............",
    "XXbbbbbbbbbb..",  // 15
    "..............",
    "ooXXXXbbbbbb..",  // 16
    "..............",
    "XXXXXXbbbbbb..",  // 17
    "..............",
    "oo....XXXXbb..",  // 18
    "..............",
    "XXbb..XXXXbb..",  // 19
    "..............",
    "ooooooXXXXbb..",  // 1A
    "..............",
    "XXXXooXXXXbb..",  // 1B
    "..............",
    "oo..XXXXXXbb..",  // 1C
    "..............",
    "XXbbXXXXXXbb..",  // 1D
    "..............",
    "ooXXXXXXXXbb..",  // 1E
    "..............",
    "XXXXXXXXXXbb..",  // 1F
    "..............",
    "oo........oooo",  // 20
    "..............",
    "XXbb......oooo",  // 21
    "..............",
    "oooooo....oooo",  // 22
    "..............",
    "XXXXoo....oooo",  // 23
    "..............",
    "oo..bbbb..oooo",  // 24
    "..............",
    "XXbbbbbb..oooo",  // 25
    "..............",
    "ooXXXXbb..oooo",  // 26
    "..............",
    "XXXXXXbb..oooo",  // 27
    "..............",
    "oo....oooooooo",  // 28
    "..............",
    "XXbb..oooooooo",  // 29
    "..............",
    "oooooooooooooo",  // 2A
    "..............",
    "XXXXoooooooooo",  // 2B
    "..............",
    "oo..XXXXoooooo",  // 2C
    "..............",
    "XXbbXXXXoooooo",  // 2D
    "..............",
    "ooXXXXXXoooooo",  // 2E
    "..............",
    "XXXXXXXXoooooo",  // 2F
    "..............",
    "oo......XXXXoo",  // 30
    "..............",
    "XXbb....XXXXoo",  // 31
    "..............",
    "oooooo..XXXXoo",  // 32
    "..............",
    "XXXXoo..XXXXoo",  // 33
    "..............",
    "oo..bbbbXXXXoo",  // 34
    "..............",
    "XXbbbbbbXXXXoo",  // 35
    "..............",
    "ooXXXXbbXXXXoo",  // 36
    "..............",
    "XXXXXXbbXXXXoo",  // 37
    "..............",
    "oo....XXXXXXoo",  // 38
    "..............",
    "XXbb..XXXXXXoo",  // 39
    "..............",
    "ooooooXXXXXXoo",  // 3A
    "..............",
    "XXXXooXXXXXXoo",  // 3B
    "..............",
    "oo..XXXXXXXXoo",  // 3C
    "..............",
    "XXbbXXXXXXXXoo",  // 3D
    "..............",
    "ooXXXXXXXXXXoo",  // 3E
    "..............",
    "XXXXXXXXXXXXoo",  // 3F
    "..............",
    "oo..........bb",  // 40
    "..............",
    "XXbb........bb",  // 41
    "..............",
    "oooooo......bb",  // 42
    "..............",
    "XXXXoo......bb",  // 43
    "..............",
    "oo..bbbb....bb",  // 44
    "..............",
    "XXbbbbbb....bb",  // 45
    "..............",
    "ooXXXXbb....bb",  // 46
    "..............",
    "XXXXXXbb....bb",  // 47
    "..............",
    "oo....oooo..bb",  // 48
    "..............",
    "XXbb..oooo..bb",  // 49
    "..............",
    "oooooooooo..bb",  // 4A
    "..............",
    "XXXXoooooo..bb",  // 4B
    "..............",
    "oo..XXXXoo..bb",  // 4C
    "..............",
    "XXbbXXXXoo..bb",  // 4D
    "..............",
    "ooXXXXXXoo..bb",  // 4E
    "..............",
    "XXXXXXXXoo..bb",  // 4F
    "..............",
    "oo......bbbbbb",  // 50
    "..............",
    "XXbb....bbbbbb",  // 51
    "..............",
    "oooooo..bbbbbb",  // 52
    "..............",
    "XXXXoo..bbbbbb",  // 53
    "..............",
    "oo..bbbbbbbbbb",  // 54
    "..............",
    "XXbbbbbbbbbbbb",  // 55
    "..............",
    "ooXXXXbbbbbbbb",  // 56
    "..............",
    "XXXXXXbbbbbbbb",  // 57
    "..............",
    "oo....XXXXbbbb",  // 58
    "..............",
    "XXbb..XXXXbbbb",  // 59
    "..............",
    "ooooooXXXXbbbb",  // 5A
    "..............",
    "XXXXooXXXXbbbb",  // 5B
    "..............",
    "oo..XXXXXXbbbb",  // 5C
    "..............",
    "XXbbXXXXXXbbbb",  // 5D
    "..............",
    "ooXXXXXXXXbbbb",  // 5E
    "..............",
    "XXXXXXXXXXbbbb",  // 5F
    "..............",
    "oo........XXXX",  // 60
    "..............",
    "XXbb......XXXX",  // 61
    "..............",
    "oooooo....XXXX",  // 62
    "..............",
    "XXXXoo....XXXX",  // 63
    "..............",
    "oo..bbbb..XXXX",  // 64
    "..............",
    "XXbbbbbb..XXXX",  // 65
    "..............",
    "ooXXXXbb..XXXX",  // 66
    "..............",
    "XXXXXXbb..XXXX",  // 67
    "..............",
    "oo....ooooXXXX",  // 68
    "..............",
    "XXbb..ooooXXXX",  // 69
    "..............",
    "ooooooooooXXXX",  // 6A
    "..............",
    "XXXXooooooXXXX",  // 6B
    "..............",
    "oo..XXXXooXXXX",  // 6C
    "..............",
    "XXbbXXXXooXXXX",  // 6D
    "..............",
    "ooXXXXXXooXXXX",  // 6E
    "..............",
    "XXXXXXXXooXXXX",  // 6F
    "..............",
    "oo......XXXXXX",  // 70
    "..............",
    "XXbb....XXXXXX",  // 71
    "..............",
    "oooooo..XXXXXX",  // 72
    "..............",
    "XXXXoo..XXXXXX",  // 73
    "..............",
    "oo..bbbbXXXXXX",  // 74
    "..............",
    "XXbbbbbbXXXXXX",  // 75
    "..............",
    "ooXXXXbbXXXXXX",  // 76
    "..............",
    "XXXXXXbbXXXXXX",  // 77
    "..............",
    "oo....XXXXXXXX",  // 78
    "..............",
    "XXbb..XXXXXXXX",  // 79
    "..............",
    "ooooooXXXXXXXX",  // 7A
    "..............",
    "XXXXooXXXXXXXX",  // 7B
    "..............",
    "oo..XXXXXXXXXX",  // 7C
    "..............",
    "XXbbXXXXXXXXXX",  // 7D
    "..............",
    "ooXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=0  and bit 0 of next byte=1

    "..............",  // 00
    "..............",
    "bbbb..........",  // 01
    "..............",
    "..oooo........",  // 02
    "..............",
    "XXXXoo........",  // 03
    "..............",
    "....bbbb......",  // 04
    "..............",
    "bbbbbbbb......",  // 05
    "..............",
    "..XXXXbb......",  // 06
    "..............",
    "XXXXXXbb......",  // 07
    "..............",
    "......oooo....",  // 08
    "..............",
    "bbbb..oooo....",  // 09
    "..............",
    "..oooooooo....",  // 0A
    "..............",
    "XXXXoooooo....",  // 0B
    "..............",
    "....XXXXoo....",  // 0C
    "..............",
    "bbbbXXXXoo....",  // 0D
    "..............",
    "..XXXXXXoo....",  // 0E
    "..............",
    "XXXXXXXXoo....",  // 0F
    "..............",
    "........bbbb..",  // 10
    "..............",
    "bbbb....bbbb..",  // 11
    "..............",
    "..oooo..bbbb..",  // 12
    "..............",
    "XXXXoo..bbbb..",  // 13
    "..............",
    "....bbbbbbbb..",  // 14
    "..............",
    "bbbbbbbbbbbb..",  // 15
    "..............",
    "..XXXXbbbbbb..",  // 16
    "..............",
    "XXXXXXbbbbbb..",  // 17
    "..............",
    "......XXXXbb..",  // 18
    "..............",
    "bbbb..XXXXbb..",  // 19
    "..............",
    "..ooooXXXXbb..",  // 1A
    "..............",
    "XXXXooXXXXbb..",  // 1B
    "..............",
    "....XXXXXXbb..",  // 1C
    "..............",
    "bbbbXXXXXXbb..",  // 1D
    "..............",
    "..XXXXXXXXbb..",  // 1E
    "..............",
    "XXXXXXXXXXbb..",  // 1F
    "..............",
    "..........oooo",  // 20
    "..............",
    "bbbb......oooo",  // 21
    "..............",
    "..oooo....oooo",  // 22
    "..............",
    "XXXXoo....oooo",  // 23
    "..............",
    "....bbbb..oooo",  // 24
    "..............",
    "bbbbbbbb..oooo",  // 25
    "..............",
    "..XXXXbb..oooo",  // 26
    "..............",
    "XXXXXXbb..oooo",  // 27
    "..............",
    "......oooooooo",  // 28
    "..............",
    "bbbb..oooooooo",  // 29
    "..............",
    "..oooooooooooo",  // 2A
    "..............",
    "XXXXoooooooooo",  // 2B
    "..............",
    "....XXXXoooooo",  // 2C
    "..............",
    "bbbbXXXXoooooo",  // 2D
    "..............",
    "..XXXXXXoooooo",  // 2E
    "..............",
    "XXXXXXXXoooooo",  // 2F
    "..............",
    "........XXXXoo",  // 30
    "..............",
    "bbbb....XXXXoo",  // 31
    "..............",
    "..oooo..XXXXoo",  // 32
    "..............",
    "XXXXoo..XXXXoo",  // 33
    "..............",
    "....bbbbXXXXoo",  // 34
    "..............",
    "bbbbbbbbXXXXoo",  // 35
    "..............",
    "..XXXXbbXXXXoo",  // 36
    "..............",
    "XXXXXXbbXXXXoo",  // 37
    "..............",
    "......XXXXXXoo",  // 38
    "..............",
    "bbbb..XXXXXXoo",  // 39
    "..............",
    "..ooooXXXXXXoo",  // 3A
    "..............",
    "XXXXooXXXXXXoo",  // 3B
    "..............",
    "....XXXXXXXXoo",  // 3C
    "..............",
    "bbbbXXXXXXXXoo",  // 3D
    "..............",
    "..XXXXXXXXXXoo",  // 3E
    "..............",
    "XXXXXXXXXXXXoo",  // 3F
    "..............",
    "............XX",  // 40
    "..............",
    "bbbb........XX",  // 41
    "..............",
    "..oooo......XX",  // 42
    "..............",
    "XXXXoo......XX",  // 43
    "..............",
    "....bbbb....XX",  // 44
    "..............",
    "bbbbbbbb....XX",  // 45
    "..............",
    "..XXXXbb....XX",  // 46
    "..............",
    "XXXXXXbb....XX",  // 47
    "..............",
    "......oooo..XX",  // 48
    "..............",
    "bbbb..oooo..XX",  // 49
    "..............",
    "..oooooooo..XX",  // 4A
    "..............",
    "XXXXoooooo..XX",  // 4B
    "..............",
    "....XXXXoo..XX",  // 4C
    "..............",
    "bbbbXXXXoo..XX",  // 4D
    "..............",
    "..XXXXXXoo..XX",  // 4E
    "..............",
    "XXXXXXXXoo..XX",  // 4F
    "..............",
    "........bbbbXX",  // 50
    "..............",
    "bbbb....bbbbXX",  // 51
    "..............",
    "..oooo..bbbbXX",  // 52
    "..............",
    "XXXXoo..bbbbXX",  // 53
    "..............",
    "....bbbbbbbbXX",  // 54
    "..............",
    "bbbbbbbbbbbbXX",  // 55
    "..............",
    "..XXXXbbbbbbXX",  // 56
    "..............",
    "XXXXXXbbbbbbXX",  // 57
    "..............",
    "......XXXXbbXX",  // 58
    "..............",
    "bbbb..XXXXbbXX",  // 59
    "..............",
    "..ooooXXXXbbXX",  // 5A
    "..............",
    "XXXXooXXXXbbXX",  // 5B
    "..............",
    "....XXXXXXbbXX",  // 5C
    "..............",
    "bbbbXXXXXXbbXX",  // 5D
    "..............",
    "..XXXXXXXXbbXX",  // 5E
    "..............",
    "XXXXXXXXXXbbXX",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "bbbb......XXXX",  // 61
    "..............",
    "..oooo....XXXX",  // 62
    "..............",
    "XXXXoo....XXXX",  // 63
    "..............",
    "....bbbb..XXXX",  // 64
    "..............",
    "bbbbbbbb..XXXX",  // 65
    "..............",
    "..XXXXbb..XXXX",  // 66
    "..............",
    "XXXXXXbb..XXXX",  // 67
    "..............",
    "......ooooXXXX",  // 68
    "..............",
    "bbbb..ooooXXXX",  // 69
    "..............",
    "..ooooooooXXXX",  // 6A
    "..............",
    "XXXXooooooXXXX",  // 6B
    "..............",
    "....XXXXooXXXX",  // 6C
    "..............",
    "bbbbXXXXooXXXX",  // 6D
    "..............",
    "..XXXXXXooXXXX",  // 6E
    "..............",
    "XXXXXXXXooXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "bbbb....XXXXXX",  // 71
    "..............",
    "..oooo..XXXXXX",  // 72
    "..............",
    "XXXXoo..XXXXXX",  // 73
    "..............",
    "....bbbbXXXXXX",  // 74
    "..............",
    "bbbbbbbbXXXXXX",  // 75
    "..............",
    "..XXXXbbXXXXXX",  // 76
    "..............",
    "XXXXXXbbXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "bbbb..XXXXXXXX",  // 79
    "..............",
    "..ooooXXXXXXXX",  // 7A
    "..............",
    "XXXXooXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "bbbbXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=1

    "oo............",  // 00
    "..............",
    "XXbb..........",  // 01
    "..............",
    "oooooo........",  // 02
    "..............",
    "XXXXoo........",  // 03
    "..............",
    "oo..bbbb......",  // 04
    "..............",
    "XXbbbbbb......",  // 05
    "..............",
    "ooXXXXbb......",  // 06
    "..............",
    "XXXXXXbb......",  // 07
    "..............",
    "oo....oooo....",  // 08
    "..............",
    "XXbb..oooo....",  // 09
    "..............",
    "oooooooooo....",  // 0A
    "..............",
    "XXXXoooooo....",  // 0B
    "..............",
    "oo..XXXXoo....",  // 0C
    "..............",
    "XXbbXXXXoo....",  // 0D
    "..............",
    "ooXXXXXXoo....",  // 0E
    "..............",
    "XXXXXXXXoo....",  // 0F
    "..............",
    "oo......bbbb..",  // 10
    "..............",
    "XXbb....bbbb..",  // 11
    "..............",
    "oooooo..bbbb..",  // 12
    "..............",
    "XXXXoo..bbbb..",  // 13
    "..............",
    "oo..bbbbbbbb..",  // 14
    "..............",
    "XXbbbbbbbbbb..",  // 15
    "..............",
    "ooXXXXbbbbbb..",  // 16
    "..............",
    "XXXXXXbbbbbb..",  // 17
    "..............",
    "oo....XXXXbb..",  // 18
    "..............",
    "XXbb..XXXXbb..",  // 19
    "..............",
    "ooooooXXXXbb..",  // 1A
    "..............",
    "XXXXooXXXXbb..",  // 1B
    "..............",
    "oo..XXXXXXbb..",  // 1C
    "..............",
    "XXbbXXXXXXbb..",  // 1D
    "..............",
    "ooXXXXXXXXbb..",  // 1E
    "..............",
    "XXXXXXXXXXbb..",  // 1F
    "..............",
    "oo........oooo",  // 20
    "..............",
    "XXbb......oooo",  // 21
    "..............",
    "oooooo....oooo",  // 22
    "..............",
    "XXXXoo....oooo",  // 23
    "..............",
    "oo..bbbb..oooo",  // 24
    "..............",
    "XXbbbbbb..oooo",  // 25
    "..............",
    "ooXXXXbb..oooo",  // 26
    "..............",
    "XXXXXXbb..oooo",  // 27
    "..............",
    "oo....oooooooo",  // 28
    "..............",
    "XXbb..oooooooo",  // 29
    "..............",
    "oooooooooooooo",  // 2A
    "..............",
    "XXXXoooooooooo",  // 2B
    "..............",
    "oo..XXXXoooooo",  // 2C
    "..............",
    "XXbbXXXXoooooo",  // 2D
    "..............",
    "ooXXXXXXoooooo",  // 2E
    "..............",
    "XXXXXXXXoooooo",  // 2F
    "..............",
    "oo......XXXXoo",  // 30
    "..............",
    "XXbb....XXXXoo",  // 31
    "..............",
    "oooooo..XXXXoo",  // 32
    "..............",
    "XXXXoo..XXXXoo",  // 33
    "..............",
    "oo..bbbbXXXXoo",  // 34
    "..............",
    "XXbbbbbbXXXXoo",  // 35
    "..............",
    "ooXXXXbbXXXXoo",  // 36
    "..............",
    "XXXXXXbbXXXXoo",  // 37
    "..............",
    "oo....XXXXXXoo",  // 38
    "..............",
    "XXbb..XXXXXXoo",  // 39
    "..............",
    "ooooooXXXXXXoo",  // 3A
    "..............",
    "XXXXooXXXXXXoo",  // 3B
    "..............",
    "oo..XXXXXXXXoo",  // 3C
    "..............",
    "XXbbXXXXXXXXoo",  // 3D
    "..............",
    "ooXXXXXXXXXXoo",  // 3E
    "..............",
    "XXXXXXXXXXXXoo",  // 3F
    "..............",
    "oo..........XX",  // 40
    "..............",
    "XXbb........XX",  // 41
    "..............",
    "oooooo......XX",  // 42
    "..............",
    "XXXXoo......XX",  // 43
    "..............",
    "oo..bbbb....XX",  // 44
    "..............",
    "XXbbbbbb....XX",  // 45
    "..............",
    "ooXXXXbb....XX",  // 46
    "..............",
    "XXXXXXbb....XX",  // 47
    "..............",
    "oo....oooo..XX",  // 48
    "..............",
    "XXbb..oooo..XX",  // 49
    "..............",
    "oooooooooo..XX",  // 4A
    "..............",
    "XXXXoooooo..XX",  // 4B
    "..............",
    "oo..XXXXoo..XX",  // 4C
    "..............",
    "XXbbXXXXoo..XX",  // 4D
    "..............",
    "ooXXXXXXoo..XX",  // 4E
    "..............",
    "XXXXXXXXoo..XX",  // 4F
    "..............",
    "oo......bbbbXX",  // 50
    "..............",
    "XXbb....bbbbXX",  // 51
    "..............",
    "oooooo..bbbbXX",  // 52
    "..............",
    "XXXXoo..bbbbXX",  // 53
    "..............",
    "oo..bbbbbbbbXX",  // 54
    "..............",
    "XXbbbbbbbbbbXX",  // 55
    "..............",
    "ooXXXXbbbbbbXX",  // 56
    "..............",
    "XXXXXXbbbbbbXX",  // 57
    "..............",
    "oo....XXXXbbXX",  // 58
    "..............",
    "XXbb..XXXXbbXX",  // 59
    "..............",
    "ooooooXXXXbbXX",  // 5A
    "..............",
    "XXXXooXXXXbbXX",  // 5B
    "..............",
    "oo..XXXXXXbbXX",  // 5C
    "..............",
    "XXbbXXXXXXbbXX",  // 5D
    "..............",
    "ooXXXXXXXXbbXX",  // 5E
    "..............",
    "XXXXXXXXXXbbXX",  // 5F
    "..............",
    "oo........XXXX",  // 60
    "..............",
    "XXbb......XXXX",  // 61
    "..............",
    "oooooo....XXXX",  // 62
    "..............",
    "XXXXoo....XXXX",  // 63
    "..............",
    "oo..bbbb..XXXX",  // 64
    "..............",
    "XXbbbbbb..XXXX",  // 65
    "..............",
    "ooXXXXbb..XXXX",  // 66
    "..............",
    "XXXXXXbb..XXXX",  // 67
    "..............",
    "oo....ooooXXXX",  // 68
    "..............",
    "XXbb..ooooXXXX",  // 69
    "..............",
    "ooooooooooXXXX",  // 6A
    "..............",
    "XXXXooooooXXXX",  // 6B
    "..............",
    "oo..XXXXooXXXX",  // 6C
    "..............",
    "XXbbXXXXooXXXX",  // 6D
    "..............",
    "ooXXXXXXooXXXX",  // 6E
    "..............",
    "XXXXXXXXooXXXX",  // 6F
    "..............",
    "oo......XXXXXX",  // 70
    "..............",
    "XXbb....XXXXXX",  // 71
    "..............",
    "oooooo..XXXXXX",  // 72
    "..............",
    "XXXXoo..XXXXXX",  // 73
    "..............",
    "oo..bbbbXXXXXX",  // 74
    "..............",
    "XXbbbbbbXXXXXX",  // 75
    "..............",
    "ooXXXXbbXXXXXX",  // 76
    "..............",
    "XXXXXXbbXXXXXX",  // 77
    "..............",
    "oo....XXXXXXXX",  // 78
    "..............",
    "XXbb..XXXXXXXX",  // 79
    "..............",
    "ooooooXXXXXXXX",  // 7A
    "..............",
    "XXXXooXXXXXXXX",  // 7B
    "..............",
    "oo..XXXXXXXXXX",  // 7C
    "..............",
    "XXbbXXXXXXXXXX",  // 7D
    "..............",
    "ooXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",


// ************ for bytes on an odd column ***********

// bit 6 of previous byte=0  and bit 0 of next byte=0

    "..............",  // 00
    "..............",
    "oooo..........",  // 01
    "..............",
    "..bbbb........",  // 02
    "..............",
    "XXXXbb........",  // 03
    "..............",
    "....oooo......",  // 04
    "..............",
    "oooooooo......",  // 05
    "..............",
    "..XXXXoo......",  // 06
    "..............",
    "XXXXXXoo......",  // 07
    "..............",
    "......bbbb....",  // 08
    "..............",
    "oooo..bbbb....",  // 09
    "..............",
    "..bbbbbbbb....",  // 0A
    "..............",
    "XXXXbbbbbb....",  // 0B
    "..............",
    "....XXXXbb....",  // 0C
    "..............",
    "ooooXXXXbb....",  // 0D
    "..............",
    "..XXXXXXbb....",  // 0E
    "..............",
    "XXXXXXXXbb....",  // 0F
    "..............",
    "........oooo..",  // 10
    "..............",
    "oooo....oooo..",  // 11
    "..............",
    "..bbbb..oooo..",  // 12
    "..............",
    "XXXXbb..oooo..",  // 13
    "..............",
    "....oooooooo..",  // 14
    "..............",
    "oooooooooooo..",  // 15
    "..............",
    "..XXXXoooooo..",  // 16
    "..............",
    "XXXXXXoooooo..",  // 17
    "..............",
    "......XXXXoo..",  // 18
    "..............",
    "oooo..XXXXoo..",  // 19
    "..............",
    "..bbbbXXXXoo..",  // 1A
    "..............",
    "XXXXbbXXXXoo..",  // 1B
    "..............",
    "....XXXXXXoo..",  // 1C
    "..............",
    "ooooXXXXXXoo..",  // 1D
    "..............",
    "..XXXXXXXXoo..",  // 1E
    "..............",
    "XXXXXXXXXXoo..",  // 1F
    "..............",
    "..........bbbb",  // 20
    "..............",
    "oooo......bbbb",  // 21
    "..............",
    "..bbbb....bbbb",  // 22
    "..............",
    "XXXXbb....bbbb",  // 23
    "..............",
    "....oooo..bbbb",  // 24
    "..............",
    "oooooooo..bbbb",  // 25
    "..............",
    "..XXXXoo..bbbb",  // 26
    "..............",
    "XXXXXXoo..bbbb",  // 27
    "..............",
    "......bbbbbbbb",  // 28
    "..............",
    "oooo..bbbbbbbb",  // 29
    "..............",
    "..bbbbbbbbbbbb",  // 2A
    "..............",
    "XXXXbbbbbbbbbb",  // 2B
    "..............",
    "....XXXXbbbbbb",  // 2C
    "..............",
    "ooooXXXXbbbbbb",  // 2D
    "..............",
    "..XXXXXXbbbbbb",  // 2E
    "..............",
    "XXXXXXXXbbbbbb",  // 2F
    "..............",
    "........XXXXbb",  // 30
    "..............",
    "oooo....XXXXbb",  // 31
    "..............",
    "..bbbb..XXXXbb",  // 32
    "..............",
    "XXXXbb..XXXXbb",  // 33
    "..............",
    "....ooooXXXXbb",  // 34
    "..............",
    "ooooooooXXXXbb",  // 35
    "..............",
    "..XXXXooXXXXbb",  // 36
    "..............",
    "XXXXXXooXXXXbb",  // 37
    "..............",
    "......XXXXXXbb",  // 38
    "..............",
    "oooo..XXXXXXbb",  // 39
    "..............",
    "..bbbbXXXXXXbb",  // 3A
    "..............",
    "XXXXbbXXXXXXbb",  // 3B
    "..............",
    "....XXXXXXXXbb",  // 3C
    "..............",
    "ooooXXXXXXXXbb",  // 3D
    "..............",
    "..XXXXXXXXXXbb",  // 3E
    "..............",
    "XXXXXXXXXXXXbb",  // 3F
    "..............",
    "............oo",  // 40
    "..............",
    "oooo........oo",  // 41
    "..............",
    "..bbbb......oo",  // 42
    "..............",
    "XXXXbb......oo",  // 43
    "..............",
    "....oooo....oo",  // 44
    "..............",
    "oooooooo....oo",  // 45
    "..............",
    "..XXXXoo....oo",  // 46
    "..............",
    "XXXXXXoo....oo",  // 47
    "..............",
    "......bbbb..oo",  // 48
    "..............",
    "oooo..bbbb..oo",  // 49
    "..............",
    "..bbbbbbbb..oo",  // 4A
    "..............",
    "XXXXbbbbbb..oo",  // 4B
    "..............",
    "....XXXXbb..oo",  // 4C
    "..............",
    "ooooXXXXbb..oo",  // 4D
    "..............",
    "..XXXXXXbb..oo",  // 4E
    "..............",
    "XXXXXXXXbb..oo",  // 4F
    "..............",
    "........oooooo",  // 50
    "..............",
    "oooo....oooooo",  // 51
    "..............",
    "..bbbb..oooooo",  // 52
    "..............",
    "XXXXbb..oooooo",  // 53
    "..............",
    "....oooooooooo",  // 54
    "..............",
    "oooooooooooooo",  // 55
    "..............",
    "..XXXXoooooooo",  // 56
    "..............",
    "XXXXXXoooooooo",  // 57
    "..............",
    "......XXXXoooo",  // 58
    "..............",
    "oooo..XXXXoooo",  // 59
    "..............",
    "..bbbbXXXXoooo",  // 5A
    "..............",
    "XXXXbbXXXXoooo",  // 5B
    "..............",
    "....XXXXXXoooo",  // 5C
    "..............",
    "ooooXXXXXXoooo",  // 5D
    "..............",
    "..XXXXXXXXoooo",  // 5E
    "..............",
    "XXXXXXXXXXoooo",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "oooo......XXXX",  // 61
    "..............",
    "..bbbb....XXXX",  // 62
    "..............",
    "XXXXbb....XXXX",  // 63
    "..............",
    "....oooo..XXXX",  // 64
    "..............",
    "oooooooo..XXXX",  // 65
    "..............",
    "..XXXXoo..XXXX",  // 66
    "..............",
    "XXXXXXoo..XXXX",  // 67
    "..............",
    "......bbbbXXXX",  // 68
    "..............",
    "oooo..bbbbXXXX",  // 69
    "..............",
    "..bbbbbbbbXXXX",  // 6A
    "..............",
    "XXXXbbbbbbXXXX",  // 6B
    "..............",
    "....XXXXbbXXXX",  // 6C
    "..............",
    "ooooXXXXbbXXXX",  // 6D
    "..............",
    "..XXXXXXbbXXXX",  // 6E
    "..............",
    "XXXXXXXXbbXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "oooo....XXXXXX",  // 71
    "..............",
    "..bbbb..XXXXXX",  // 72
    "..............",
    "XXXXbb..XXXXXX",  // 73
    "..............",
    "....ooooXXXXXX",  // 74
    "..............",
    "ooooooooXXXXXX",  // 75
    "..............",
    "..XXXXooXXXXXX",  // 76
    "..............",
    "XXXXXXooXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "oooo..XXXXXXXX",  // 79
    "..............",
    "..bbbbXXXXXXXX",  // 7A
    "..............",
    "XXXXbbXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "ooooXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=0

    "bb............",  // 00
    "..............",
    "XXoo..........",  // 01
    "..............",
    "bbbbbb........",  // 02
    "..............",
    "XXXXbb........",  // 03
    "..............",
    "bb..oooo......",  // 04
    "..............",
    "XXoooooo......",  // 05
    "..............",
    "bbXXXXoo......",  // 06
    "..............",
    "XXXXXXoo......",  // 07
    "..............",
    "bb....bbbb....",  // 08
    "..............",
    "XXoo..bbbb....",  // 09
    "..............",
    "bbbbbbbbbb....",  // 0A
    "..............",
    "XXXXbbbbbb....",  // 0B
    "..............",
    "bb..XXXXbb....",  // 0C
    "..............",
    "XXooXXXXbb....",  // 0D
    "..............",
    "bbXXXXXXbb....",  // 0E
    "..............",
    "XXXXXXXXbb....",  // 0F
    "..............",
    "bb......oooo..",  // 10
    "..............",
    "XXoo....oooo..",  // 11
    "..............",
    "bbbbbb..oooo..",  // 12
    "..............",
    "XXXXbb..oooo..",  // 13
    "..............",
    "bb..oooooooo..",  // 14
    "..............",
    "XXoooooooooo..",  // 15
    "..............",
    "bbXXXXoooooo..",  // 16
    "..............",
    "XXXXXXoooooo..",  // 17
    "..............",
    "bb....XXXXoo..",  // 18
    "..............",
    "XXoo..XXXXoo..",  // 19
    "..............",
    "bbbbbbXXXXoo..",  // 1A
    "..............",
    "XXXXbbXXXXoo..",  // 1B
    "..............",
    "bb..XXXXXXoo..",  // 1C
    "..............",
    "XXooXXXXXXoo..",  // 1D
    "..............",
    "bbXXXXXXXXoo..",  // 1E
    "..............",
    "XXXXXXXXXXoo..",  // 1F
    "..............",
    "bb........bbbb",  // 20
    "..............",
    "XXoo......bbbb",  // 21
    "..............",
    "bbbbbb....bbbb",  // 22
    "..............",
    "XXXXbb....bbbb",  // 23
    "..............",
    "bb..oooo..bbbb",  // 24
    "..............",
    "XXoooooo..bbbb",  // 25
    "..............",
    "bbXXXXoo..bbbb",  // 26
    "..............",
    "XXXXXXoo..bbbb",  // 27
    "..............",
    "bb....bbbbbbbb",  // 28
    "..............",
    "XXoo..bbbbbbbb",  // 29
    "..............",
    "bbbbbbbbbbbbbb",  // 2A
    "..............",
    "XXXXbbbbbbbbbb",  // 2B
    "..............",
    "bb..XXXXbbbbbb",  // 2C
    "..............",
    "XXooXXXXbbbbbb",  // 2D
    "..............",
    "bbXXXXXXbbbbbb",  // 2E
    "..............",
    "XXXXXXXXbbbbbb",  // 2F
    "..............",
    "bb......XXXXbb",  // 30
    "..............",
    "XXoo....XXXXbb",  // 31
    "..............",
    "bbbbbb..XXXXbb",  // 32
    "..............",
    "XXXXbb..XXXXbb",  // 33
    "..............",
    "bb..ooooXXXXbb",  // 34
    "..............",
    "XXooooooXXXXbb",  // 35
    "..............",
    "bbXXXXooXXXXbb",  // 36
    "..............",
    "XXXXXXooXXXXbb",  // 37
    "..............",
    "bb....XXXXXXbb",  // 38
    "..............",
    "XXoo..XXXXXXbb",  // 39
    "..............",
    "bbbbbbXXXXXXbb",  // 3A
    "..............",
    "XXXXbbXXXXXXbb",  // 3B
    "..............",
    "bb..XXXXXXXXbb",  // 3C
    "..............",
    "XXooXXXXXXXXbb",  // 3D
    "..............",
    "bbXXXXXXXXXXbb",  // 3E
    "..............",
    "XXXXXXXXXXXXbb",  // 3F
    "..............",
    "bb..........oo",  // 40
    "..............",
    "XXoo........oo",  // 41
    "..............",
    "bbbbbb......oo",  // 42
    "..............",
    "XXXXbb......oo",  // 43
    "..............",
    "bb..oooo....oo",  // 44
    "..............",
    "XXoooooo....oo",  // 45
    "..............",
    "bbXXXXoo....oo",  // 46
    "..............",
    "XXXXXXoo....oo",  // 47
    "..............",
    "bb....bbbb..oo",  // 48
    "..............",
    "XXoo..bbbb..oo",  // 49
    "..............",
    "bbbbbbbbbb..oo",  // 4A
    "..............",
    "XXXXbbbbbb..oo",  // 4B
    "..............",
    "bb..XXXXbb..oo",  // 4C
    "..............",
    "XXooXXXXbb..oo",  // 4D
    "..............",
    "bbXXXXXXbb..oo",  // 4E
    "..............",
    "XXXXXXXXbb..oo",  // 4F
    "..............",
    "bb......oooooo",  // 50
    "..............",
    "XXoo....oooooo",  // 51
    "..............",
    "bbbbbb..oooooo",  // 52
    "..............",
    "XXXXbb..oooooo",  // 53
    "..............",
    "bb..oooooooooo",  // 54
    "..............",
    "XXoooooooooooo",  // 55
    "..............",
    "bbXXXXoooooooo",  // 56
    "..............",
    "XXXXXXoooooooo",  // 57
    "..............",
    "bb....XXXXoooo",  // 58
    "..............",
    "XXoo..XXXXoooo",  // 59
    "..............",
    "bbbbbbXXXXoooo",  // 5A
    "..............",
    "XXXXbbXXXXoooo",  // 5B
    "..............",
    "bb..XXXXXXoooo",  // 5C
    "..............",
    "XXooXXXXXXoooo",  // 5D
    "..............",
    "bbXXXXXXXXoooo",  // 5E
    "..............",
    "XXXXXXXXXXoooo",  // 5F
    "..............",
    "bb........XXXX",  // 60
    "..............",
    "XXoo......XXXX",  // 61
    "..............",
    "bbbbbb....XXXX",  // 62
    "..............",
    "XXXXbb....XXXX",  // 63
    "..............",
    "bb..oooo..XXXX",  // 64
    "..............",
    "XXoooooo..XXXX",  // 65
    "..............",
    "bbXXXXoo..XXXX",  // 66
    "..............",
    "XXXXXXoo..XXXX",  // 67
    "..............",
    "bb....bbbbXXXX",  // 68
    "..............",
    "XXoo..bbbbXXXX",  // 69
    "..............",
    "bbbbbbbbbbXXXX",  // 6A
    "..............",
    "XXXXbbbbbbXXXX",  // 6B
    "..............",
    "bb..XXXXbbXXXX",  // 6C
    "..............",
    "XXooXXXXbbXXXX",  // 6D
    "..............",
    "bbXXXXXXbbXXXX",  // 6E
    "..............",
    "XXXXXXXXbbXXXX",  // 6F
    "..............",
    "bb......XXXXXX",  // 70
    "..............",
    "XXoo....XXXXXX",  // 71
    "..............",
    "bbbbbb..XXXXXX",  // 72
    "..............",
    "XXXXbb..XXXXXX",  // 73
    "..............",
    "bb..ooooXXXXXX",  // 74
    "..............",
    "XXooooooXXXXXX",  // 75
    "..............",
    "bbXXXXooXXXXXX",  // 76
    "..............",
    "XXXXXXooXXXXXX",  // 77
    "..............",
    "bb....XXXXXXXX",  // 78
    "..............",
    "XXoo..XXXXXXXX",  // 79
    "..............",
    "bbbbbbXXXXXXXX",  // 7A
    "..............",
    "XXXXbbXXXXXXXX",  // 7B
    "..............",
    "bb..XXXXXXXXXX",  // 7C
    "..............",
    "XXooXXXXXXXXXX",  // 7D
    "..............",
    "bbXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=0  and bit 0 of next byte=1

    "..............",  // 00
    "..............",
    "oooo..........",  // 01
    "..............",
    "..bbbb........",  // 02
    "..............",
    "XXXXbb........",  // 03
    "..............",
    "....oooo......",  // 04
    "..............",
    "oooooooo......",  // 05
    "..............",
    "..XXXXoo......",  // 06
    "..............",
    "XXXXXXoo......",  // 07
    "..............",
    "......bbbb....",  // 08
    "..............",
    "oooo..bbbb....",  // 09
    "..............",
    "..bbbbbbbb....",  // 0A
    "..............",
    "XXXXbbbbbb....",  // 0B
    "..............",
    "....XXXXbb....",  // 0C
    "..............",
    "ooooXXXXbb....",  // 0D
    "..............",
    "..XXXXXXbb....",  // 0E
    "..............",
    "XXXXXXXXbb....",  // 0F
    "..............",
    "........oooo..",  // 10
    "..............",
    "oooo....oooo..",  // 11
    "..............",
    "..bbbb..oooo..",  // 12
    "..............",
    "XXXXbb..oooo..",  // 13
    "..............",
    "....oooooooo..",  // 14
    "..............",
    "oooooooooooo..",  // 15
    "..............",
    "..XXXXoooooo..",  // 16
    "..............",
    "XXXXXXoooooo..",  // 17
    "..............",
    "......XXXXoo..",  // 18
    "..............",
    "oooo..XXXXoo..",  // 19
    "..............",
    "..bbbbXXXXoo..",  // 1A
    "..............",
    "XXXXbbXXXXoo..",  // 1B
    "..............",
    "....XXXXXXoo..",  // 1C
    "..............",
    "ooooXXXXXXoo..",  // 1D
    "..............",
    "..XXXXXXXXoo..",  // 1E
    "..............",
    "XXXXXXXXXXoo..",  // 1F
    "..............",
    "..........bbbb",  // 20
    "..............",
    "oooo......bbbb",  // 21
    "..............",
    "..bbbb....bbbb",  // 22
    "..............",
    "XXXXbb....bbbb",  // 23
    "..............",
    "....oooo..bbbb",  // 24
    "..............",
    "oooooooo..bbbb",  // 25
    "..............",
    "..XXXXoo..bbbb",  // 26
    "..............",
    "XXXXXXoo..bbbb",  // 27
    "..............",
    "......bbbbbbbb",  // 28
    "..............",
    "oooo..bbbbbbbb",  // 29
    "..............",
    "..bbbbbbbbbbbb",  // 2A
    "..............",
    "XXXXbbbbbbbbbb",  // 2B
    "..............",
    "....XXXXbbbbbb",  // 2C
    "..............",
    "ooooXXXXbbbbbb",  // 2D
    "..............",
    "..XXXXXXbbbbbb",  // 2E
    "..............",
    "XXXXXXXXbbbbbb",  // 2F
    "..............",
    "........XXXXbb",  // 30
    "..............",
    "oooo....XXXXbb",  // 31
    "..............",
    "..bbbb..XXXXbb",  // 32
    "..............",
    "XXXXbb..XXXXbb",  // 33
    "..............",
    "....ooooXXXXbb",  // 34
    "..............",
    "ooooooooXXXXbb",  // 35
    "..............",
    "..XXXXooXXXXbb",  // 36
    "..............",
    "XXXXXXooXXXXbb",  // 37
    "..............",
    "......XXXXXXbb",  // 38
    "..............",
    "oooo..XXXXXXbb",  // 39
    "..............",
    "..bbbbXXXXXXbb",  // 3A
    "..............",
    "XXXXbbXXXXXXbb",  // 3B
    "..............",
    "....XXXXXXXXbb",  // 3C
    "..............",
    "ooooXXXXXXXXbb",  // 3D
    "..............",
    "..XXXXXXXXXXbb",  // 3E
    "..............",
    "XXXXXXXXXXXXbb",  // 3F
    "..............",
    "............XX",  // 40
    "..............",
    "oooo........XX",  // 41
    "..............",
    "..bbbb......XX",  // 42
    "..............",
    "XXXXbb......XX",  // 43
    "..............",
    "....oooo....XX",  // 44
    "..............",
    "oooooooo....XX",  // 45
    "..............",
    "..XXXXoo....XX",  // 46
    "..............",
    "XXXXXXoo....XX",  // 47
    "..............",
    "......bbbb..XX",  // 48
    "..............",
    "oooo..bbbb..XX",  // 49
    "..............",
    "..bbbbbbbb..XX",  // 4A
    "..............",
    "XXXXbbbbbb..XX",  // 4B
    "..............",
    "....XXXXbb..XX",  // 4C
    "..............",
    "ooooXXXXbb..XX",  // 4D
    "..............",
    "..XXXXXXbb..XX",  // 4E
    "..............",
    "XXXXXXXXbb..XX",  // 4F
    "..............",
    "........ooooXX",  // 50
    "..............",
    "oooo....ooooXX",  // 51
    "..............",
    "..bbbb..ooooXX",  // 52
    "..............",
    "XXXXbb..ooooXX",  // 53
    "..............",
    "....ooooooooXX",  // 54
    "..............",
    "ooooooooooooXX",  // 55
    "..............",
    "..XXXXooooooXX",  // 56
    "..............",
    "XXXXXXooooooXX",  // 57
    "..............",
    "......XXXXooXX",  // 58
    "..............",
    "oooo..XXXXooXX",  // 59
    "..............",
    "..bbbbXXXXooXX",  // 5A
    "..............",
    "XXXXbbXXXXooXX",  // 5B
    "..............",
    "....XXXXXXooXX",  // 5C
    "..............",
    "ooooXXXXXXooXX",  // 5D
    "..............",
    "..XXXXXXXXooXX",  // 5E
    "..............",
    "XXXXXXXXXXooXX",  // 5F
    "..............",
    "..........XXXX",  // 60
    "..............",
    "oooo......XXXX",  // 61
    "..............",
    "..bbbb....XXXX",  // 62
    "..............",
    "XXXXbb....XXXX",  // 63
    "..............",
    "....oooo..XXXX",  // 64
    "..............",
    "oooooooo..XXXX",  // 65
    "..............",
    "..XXXXoo..XXXX",  // 66
    "..............",
    "XXXXXXoo..XXXX",  // 67
    "..............",
    "......bbbbXXXX",  // 68
    "..............",
    "oooo..bbbbXXXX",  // 69
    "..............",
    "..bbbbbbbbXXXX",  // 6A
    "..............",
    "XXXXbbbbbbXXXX",  // 6B
    "..............",
    "....XXXXbbXXXX",  // 6C
    "..............",
    "ooooXXXXbbXXXX",  // 6D
    "..............",
    "..XXXXXXbbXXXX",  // 6E
    "..............",
    "XXXXXXXXbbXXXX",  // 6F
    "..............",
    "........XXXXXX",  // 70
    "..............",
    "oooo....XXXXXX",  // 71
    "..............",
    "..bbbb..XXXXXX",  // 72
    "..............",
    "XXXXbb..XXXXXX",  // 73
    "..............",
    "....ooooXXXXXX",  // 74
    "..............",
    "ooooooooXXXXXX",  // 75
    "..............",
    "..XXXXooXXXXXX",  // 76
    "..............",
    "XXXXXXooXXXXXX",  // 77
    "..............",
    "......XXXXXXXX",  // 78
    "..............",
    "oooo..XXXXXXXX",  // 79
    "..............",
    "..bbbbXXXXXXXX",  // 7A
    "..............",
    "XXXXbbXXXXXXXX",  // 7B
    "..............",
    "....XXXXXXXXXX",  // 7C
    "..............",
    "ooooXXXXXXXXXX",  // 7D
    "..............",
    "..XXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    "..............",

// bit 6 of previous byte=1  and bit 0 of next byte=1

    "bb............",  // 00
    "..............",
    "XXoo..........",  // 01
    "..............",
    "bbbbbb........",  // 02
    "..............",
    "XXXXbb........",  // 03
    "..............",
    "bb..oooo......",  // 04
    "..............",
    "XXoooooo......",  // 05
    "..............",
    "bbXXXXoo......",  // 06
    "..............",
    "XXXXXXoo......",  // 07
    "..............",
    "bb....bbbb....",  // 08
    "..............",
    "XXoo..bbbb....",  // 09
    "..............",
    "bbbbbbbbbb....",  // 0A
    "..............",
    "XXXXbbbbbb....",  // 0B
    "..............",
    "bb..XXXXbb....",  // 0C
    "..............",
    "XXooXXXXbb....",  // 0D
    "..............",
    "bbXXXXXXbb....",  // 0E
    "..............",
    "XXXXXXXXbb....",  // 0F
    "..............",
    "bb......oooo..",  // 10
    "..............",
    "XXoo....oooo..",  // 11
    "..............",
    "bbbbbb..oooo..",  // 12
    "..............",
    "XXXXbb..oooo..",  // 13
    "..............",
    "bb..oooooooo..",  // 14
    "..............",
    "XXoooooooooo..",  // 15
    "..............",
    "bbXXXXoooooo..",  // 16
    "..............",
    "XXXXXXoooooo..",  // 17
    "..............",
    "bb....XXXXoo..",  // 18
    "..............",
    "XXoo..XXXXoo..",  // 19
    "..............",
    "bbbbbbXXXXoo..",  // 1A
    "..............",
    "XXXXbbXXXXoo..",  // 1B
    "..............",
    "bb..XXXXXXoo..",  // 1C
    "..............",
    "XXooXXXXXXoo..",  // 1D
    "..............",
    "bbXXXXXXXXoo..",  // 1E
    "..............",
    "XXXXXXXXXXoo..",  // 1F
    "..............",
    "bb........bbbb",  // 20
    "..............",
    "XXoo......bbbb",  // 21
    "..............",
    "bbbbbb....bbbb",  // 22
    "..............",
    "XXXXbb....bbbb",  // 23
    "..............",
    "bb..oooo..bbbb",  // 24
    "..............",
    "XXoooooo..bbbb",  // 25
    "..............",
    "bbXXXXoo..bbbb",  // 26
    "..............",
    "XXXXXXoo..bbbb",  // 27
    "..............",
    "bb....bbbbbbbb",  // 28
    "..............",
    "XXoo..bbbbbbbb",  // 29
    "..............",
    "bbbbbbbbbbbbbb",  // 2A
    "..............",
    "XXXXbbbbbbbbbb",  // 2B
    "..............",
    "bb..XXXXbbbbbb",  // 2C
    "..............",
    "XXooXXXXbbbbbb",  // 2D
    "..............",
    "bbXXXXXXbbbbbb",  // 2E
    "..............",
    "XXXXXXXXbbbbbb",  // 2F
    "..............",
    "bb......XXXXbb",  // 30
    "..............",
    "XXoo....XXXXbb",  // 31
    "..............",
    "bbbbbb..XXXXbb",  // 32
    "..............",
    "XXXXbb..XXXXbb",  // 33
    "..............",
    "bb..ooooXXXXbb",  // 34
    "..............",
    "XXooooooXXXXbb",  // 35
    "..............",
    "bbXXXXooXXXXbb",  // 36
    "..............",
    "XXXXXXooXXXXbb",  // 37
    "..............",
    "bb....XXXXXXbb",  // 38
    "..............",
    "XXoo..XXXXXXbb",  // 39
    "..............",
    "bbbbbbXXXXXXbb",  // 3A
    "..............",
    "XXXXbbXXXXXXbb",  // 3B
    "..............",
    "bb..XXXXXXXXbb",  // 3C
    "..............",
    "XXooXXXXXXXXbb",  // 3D
    "..............",
    "bbXXXXXXXXXXbb",  // 3E
    "..............",
    "XXXXXXXXXXXXbb",  // 3F
    "..............",
    "bb..........XX",  // 40
    "..............",
    "XXoo........XX",  // 41
    "..............",
    "bbbbbb......XX",  // 42
    "..............",
    "XXXXbb......XX",  // 43
    "..............",
    "bb..oooo....XX",  // 44
    "..............",
    "XXoooooo....XX",  // 45
    "..............",
    "bbXXXXoo....XX",  // 46
    "..............",
    "XXXXXXoo....XX",  // 47
    "..............",
    "bb....bbbb..XX",  // 48
    "..............",
    "XXoo..bbbb..XX",  // 49
    "..............",
    "bbbbbbbbbb..XX",  // 4A
    "..............",
    "XXXXbbbbbb..XX",  // 4B
    "..............",
    "bb..XXXXbb..XX",  // 4C
    "..............",
    "XXooXXXXbb..XX",  // 4D
    "..............",
    "bbXXXXXXbb..XX",  // 4E
    "..............",
    "XXXXXXXXbb..XX",  // 4F
    "..............",
    "bb......ooooXX",  // 50
    "..............",
    "XXoo....ooooXX",  // 51
    "..............",
    "bbbbbb..ooooXX",  // 52
    "..............",
    "XXXXbb..ooooXX",  // 53
    "..............",
    "bb..ooooooooXX",  // 54
    "..............",
    "XXooooooooooXX",  // 55
    "..............",
    "bbXXXXooooooXX",  // 56
    "..............",
    "XXXXXXooooooXX",  // 57
    "..............",
    "bb....XXXXooXX",  // 58
    "..............",
    "XXoo..XXXXooXX",  // 59
    "..............",
    "bbbbbbXXXXooXX",  // 5A
    "..............",
    "XXXXbbXXXXooXX",  // 5B
    "..............",
    "bb..XXXXXXooXX",  // 5C
    "..............",
    "XXooXXXXXXooXX",  // 5D
    "..............",
    "bbXXXXXXXXooXX",  // 5E
    "..............",
    "XXXXXXXXXXooXX",  // 5F
    "..............",
    "bb........XXXX",  // 60
    "..............",
    "XXoo......XXXX",  // 61
    "..............",
    "bbbbbb....XXXX",  // 62
    "..............",
    "XXXXbb....XXXX",  // 63
    "..............",
    "bb..oooo..XXXX",  // 64
    "..............",
    "XXoooooo..XXXX",  // 65
    "..............",
    "bbXXXXoo..XXXX",  // 66
    "..............",
    "XXXXXXoo..XXXX",  // 67
    "..............",
    "bb....bbbbXXXX",  // 68
    "..............",
    "XXoo..bbbbXXXX",  // 69
    "..............",
    "bbbbbbbbbbXXXX",  // 6A
    "..............",
    "XXXXbbbbbbXXXX",  // 6B
    "..............",
    "bb..XXXXbbXXXX",  // 6C
    "..............",
    "XXooXXXXbbXXXX",  // 6D
    "..............",
    "bbXXXXXXbbXXXX",  // 6E
    "..............",
    "XXXXXXXXbbXXXX",  // 6F
    "..............",
    "bb......XXXXXX",  // 70
    "..............",
    "XXoo....XXXXXX",  // 71
    "..............",
    "bbbbbb..XXXXXX",  // 72
    "..............",
    "XXXXbb..XXXXXX",  // 73
    "..............",
    "bb..ooooXXXXXX",  // 74
    "..............",
    "XXooooooXXXXXX",  // 75
    "..............",
    "bbXXXXooXXXXXX",  // 76
    "..............",
    "XXXXXXooXXXXXX",  // 77
    "..............",
    "bb....XXXXXXXX",  // 78
    "..............",
    "XXoo..XXXXXXXX",  // 79
    "..............",
    "bbbbbbXXXXXXXX",  // 7A
    "..............",
    "XXXXbbXXXXXXXX",  // 7B
    "..............",
    "bb..XXXXXXXXXX",  // 7C
    "..............",
    "XXooXXXXXXXXXX",  // 7D
    "..............",
    "bbXXXXXXXXXXXX",  // 7E
    "..............",
    "XXXXXXXXXXXXXX",  // 7F
    ".............."
} ;

