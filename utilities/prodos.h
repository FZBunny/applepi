/*****************************************************************************

    File: "prodos.h"

    Copyright (C) 2021, Bruce Ward

    This file is part of applepi, an apple2 emulator for Linux,
    especially aimed at the Raspberry Pi operating system.

    applepi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    applepi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with applepi; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Or perhaps visit: http://www.gnu.org/licenses/licenses.html

*****************************************************************************/


using namespace std ;

#include <vector>
#include <string>

typedef unsigned char      uint8 ;
typedef unsigned short     uint16 ;
typedef unsigned int       uint32 ;
typedef unsigned long long uint64 ;


const int  BLOCKSIZE = 512 ;
const int  FIVE_INCH_FLOPPY_SIZE = 143360 ;


typedef struct __attribute__((packed)) dirHeader {
    uint16 previousDirBlock ;    //  $00
    uint16 nextDirBlock ;        //  $02
    uint8  nameLen ;             //  $04  (len is lower nibble; upper nibble must be 0xF if this is a proDos disk.)
    uint8  directoryName[15] ;   //  $05
    uint8  unused[8] ;           //  $14
    uint8  creation[4] ;         //  $1C
    uint8  version ;             //  $20
    uint8  minVersion ;          //  $21
    uint8  access ;              //  $22
    uint8  entryLength ;         //  $23
    uint8  entriesPerBlock ;     //  $24
    uint16 fileCount ;           //  $25
    uint16 bitmapPointer ;       //  $27
    uint16 totalBlocks ;         //  $29
} dirHeader  ;                   // Length = $2B bytes

 
typedef struct __attribute__((packed)) dirEntry {
    uint8  type_and_nameLength ; //  $00
    uint8  fileName[15] ;        //  $01
    uint8  fileType ;            //  $10
    uint16 keyPointer ;          //  $11
    uint16 blocksUsed ;          //  $13
    uint8  eof[3] ;              //  $15
    uint8  creation[4] ;         //  $18
    uint8  version ;             //  $1C
    uint8  minVersion ;          //  $1D
    uint8  access ;              //  $1E
    uint16 auxType ;             //  $1F
    uint8  lastMod[4] ;          //  $21
    uint16 headerPointer ;       //  $25
} dirEntry  ;                    // Length = $27 bytes

/****
typedef struct dirListEntry {
    dirEntry* dirPtr ;
    dirListEntry* next ;
} dirListEntry ;


typedef struct dirHeaderEntry {
    dirHeader* hdrPtr ;
    dirHeader* next ;
    dirEntry*        dirPtr ;
} dirHeaderEntry ;
***/

// ============================================================================


class Node
{ 
public:

    Node (Node* parent, bool firstChild, dirHeader* dirHdrPtr, dirEntry* entryPtr) ;
    void addSiblings (dirHeader* hdrPtr) ;
    void addChild (void) ;
    void inorderTraverse (string path, Node* node) ;
    void inorderTraverse (Node* node, void* visitFunction) ;
    void visit (Node* node) ;
    
    char* getDate (uint8* packedDate) ;
    char* getFileName (uint8* p) ;
    const char* getFileType (uint8 t) ;

private:
    static const int    DIRTYPE   = 0x0f ;
    static const bool   FIRSTBORN = true ;
    static const bool   SIBLING   = false ;

    bool          m_firstBorn ;
    string        m_name ;
    uint8         m_fileType ;
    Node*         m_parent ;
    Node*         m_child ;
    dirHeader*    m_hdrPtr ;
    dirEntry*     m_entryPtr ;
    vector<Node*> m_sibling ;

    string m_unknownType ;
    const uint8 m_typeCode[22] = {
        0x00, 0x01, 0x04, 0x06, 0x0f, 0x19, 0x1a, 0x1b,
        0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6,
        0xf7, 0xf8, 0xfc, 0xfd, 0xfe, 0xff
    } ;

    const char* m_typeName[22] = {
        "000", "BAD", "TXT", "BIN", "DIR", "ADB", "AWP", "ASP",
        "PAS", "CMD", "UD1", "UD2", "UD3", "UD4", "UD5", "UD6",
        "UD7", "UD8", "BAS", "VAR", "REL", "SYS"
    } ;
} ;


// ============================================================================


class Tree
{
public:
    Tree (Tree* root) ;
private:

} ;


// ============================================================================


class ProDos
{
public:

    ProDos (char* fileName) ;
    ~ProDos (void) {} ;

    void makeDirectoryTree (void) ;
    void listDirectories (char* dirName, bool recurse) ;
 //   void listOneDirectory (dirEntry* p) ;
//    void createDirectoryTree (void) ;

    int findOffset (void) ;
//    uint8* getBlockPtr (int blockNumber) ;
    struct dirEntry* nextEntry (struct dirEntry* p) ;
    char* getDate (uint8* packedDate) ;
//    char* getFileName (uint8* p) ;
//    string getFileType (uint8 t) ;
    void volumeInfo (void) ;
    void extract (string path) ;
    uint32 DiskII_LogicalToPhysicalSector (void) ;

    Node*    m_treeRoot ;

    static uint8*  m_diskImage ;
    static char     m_volumeName[18] ;

private:

    char*  m_imageFileName ;
    int    m_file ;
    bool   m_diskII ;         // True if image is that of a 5-1/4" Apple disk II

} ;
