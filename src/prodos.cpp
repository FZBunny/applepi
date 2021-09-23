/*****************************************************************************

    File: "prodos.cpp"

    Copyright (C) 2003,2019 Bruce Ward

    This file is part of ApplePi, the Apple2 emulator for the Raspberry Pi

    ApplePi is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ApplePi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ApplePi; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Or perhaps visit: http://www.gnu.org/licenses/licenses.html

*****************************************************************************/

//
//  List directory of a ProDos disk image.
//
//    ./prodos [-h] [-l dirname [-r]] [-x pathname ] filename
//


using namespace std ;

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#ifndef __WXMSW__
  #include <unistd.h>
  #include <getopt.h>
#endif
#include <fcntl.h>
#include <errno.h>

#include <list>
#include <iterator>
#include <string>

#include "prodos.h"


uint8*  ProDos::m_diskImage ;       // This points to the entire disk image
char     ProDos::m_volumeName[18] ;  // Contains the ProDOS volume name



// =========================== Class "Node" ==================================
//
//  A node object represents one entry in a ProDOS subdirectory.
//  Nodes are arranged in an n-ary tree.
//  Each subdirectory is a doubly-linked of 512-byte disk blocks.
//  Note that 'Node' uses recursive calls for both creating a tree,
//  and then traversing the tree.
//  The first entry of a subdirectory is the "firstBorn" node which
//  puts all remaining entries in that subdir in an STL vector called
//  "siblings".
//  


// The Node constructor creates the entire n-ary tree of disk directories.
// The pointer "m_treeRoot" within the single ProDos object points to the
// root of this tree.

Node::Node (Node* parent, bool firstBorn, dirHeader* hdrPtr, dirEntry* entryPtr)
{
    int len    = entryPtr->type_and_nameLength & 0x0f ;
    
    char tmp[16] ;
    memcpy (tmp, entryPtr->fileName, len) ;
    tmp[len] = 0 ;
    m_name = tmp ;

    m_parent   = parent ;

    m_hdrPtr   = hdrPtr ;
    m_entryPtr = entryPtr ;
    m_child    = NULL ;
    m_fileType = m_entryPtr->fileType ;

    if (firstBorn) {                   // A "firstBorn" node must create a vector of siblings;
        m_sibling.push_back (this) ;
        addSiblings(hdrPtr) ;  // these siblings are the rest of the entries in that subdirectory.
    }
    
    if (m_fileType == DIRTYPE) {       // If this entry is a directory, create a firstBorn child node
        uint16 block = m_entryPtr->keyPointer ;
        dirHeader* hdr = (dirHeader*)((uint8*)ProDos::m_diskImage + block*BLOCKSIZE) ;
        dirEntry* entry = (dirEntry*)((uint8*)hdr + sizeof(dirHeader)) ;
        m_child = new Node (this, FIRSTBORN, hdr, entry) ;
    }
}


void Node::addSiblings (dirHeader* hdrPtr)
{
    uint16 nextBlock = hdrPtr->nextDirBlock ;
    dirEntry* p = (dirEntry*)((uint8*)(hdrPtr) + sizeof(dirHeader)) ;

    while (p->fileName[0]) {
        Node* n = new Node (this, SIBLING, hdrPtr, p) ;
        m_sibling.push_back (n) ;
        p++ ;
        uint64 offset = (uint64)p-(uint64)ProDos::m_diskImage ;
        if ((offset & 0x1ff) > 0x1f0) {
            dirHeader* h = (dirHeader*)((uint8*)ProDos::m_diskImage + (nextBlock * 0x200)) ;
            nextBlock = h->nextDirBlock ;
            p = (dirEntry*)((uint8*)h + 4) ;
        }
    }
}


char* Node::getFileName (uint8* p)
{
    static char name[16] ;
    
    for (int i=0; i<15; i++) name[i] = 0x20 ;
    name[15] = 0 ;

    for (int i=0; i<15; i++) {
        if (p[i] == 0) break ;
        name[i] = p[i] ;
    }
    
    return name ;
}


const char* Node::getFileType (uint8 t)
{
    static char buffer[8] ;
    for (int i=0; i<22; i++) {
        if (t == m_typeCode[i]) return m_typeName[i] ;
    }
    sprintf (buffer, "$%2.2X", t) ;
    return buffer ;
}


char* Node::getDate (uint8* p)  // 'p' points to a 4-byte packed time & date
{
    static const char* month[12] = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    } ;
    static char buffer[82] ;
    uint8 zero = p[0] | p[0] | p[2] | p[3] ;
    if (zero == 0) {
        strcpy (buffer, "<no date>      ") ;
    } else {
        int yy = p[1] >> 1 ;
        int mm = (p[1]&1)<<3 | (p[0]>>5)  ;
        if (mm > 12) mm = 1 ;
        int dd = p[0] & 0x1f ;
        int HH = p[3] & 0x1f ;
        int MM = p[2] & 0x3f ;
        sprintf (buffer, "%2i-%s-%2.2i %2i:%2.2i", dd, month[mm-1], yy, HH, MM) ;
//printf ("%s\n", buffer) ;
    }
    return buffer ;
}


// Tree traverse for directory lists

void Node::inorderTraverse (string path, Node* node)
{
    union {
        uint32 eof32 ;
        uint8  eof8[4] ;
    } eof ;

    char buffer[82] ;

    path += "/" + m_name ;
printf ("*** m_name = '%s'\n", m_name.c_str()) ;
    printf ("%s\n", path.c_str()) ;
    printf ("NAME           TYPE   BLOCKS  MODIFIED         CREATED          ENDFILE SUBTYPE\n") ;
    printf ("----           ----   ------  ---------------  ---------------  ------- -------\n") ;
    
    char* name ;
    int nSiblings = node->m_sibling.size() ;
    for (int i=0; i<nSiblings; i++ ) {
        memset (buffer, ' ', 82) ;
        dirEntry* p = node->m_sibling[i]->m_entryPtr ;
        name = getFileName(p->fileName) ;

        sprintf (buffer,    "%s  ",  name) ;                      // name
        sprintf (buffer+16, "%s   ", getFileType(p->fileType)) ;  // file type
        sprintf (buffer+22, "%6i  ", p->blocksUsed) ;             // blocks used
        sprintf (buffer+30, "%s   ", getDate(p->lastMod)) ;       // date & time modified
        sprintf (buffer+47, "%s   ", getDate(p->creation)) ;      // date & time created

        for (int i=0; i<3; i++) eof.eof8[i] = p->eof[i] ;
        eof.eof8[3] = 0 ;
        sprintf (buffer+64, "%7i  ", eof.eof32) ;                 // endfile

        if (p->auxType) {                                         // "aux/sub type"
            if (p->fileType == 4) {                               // if type=="TXT", print record length
                sprintf (buffer+72, "R=%5i", p->auxType) ;
            }
            if (p->fileType == 6) {                               // if type=="BIN", print load address
                sprintf (buffer+72, "A=$%4.4X", p->auxType) ;
            }
        }

        puts (buffer) ;
    }
    printf ("\n") ;

    for (int i=0; i<nSiblings; i++ ) {
        Node* n = this->m_sibling[i]->m_child ;
        if (n) { 
            inorderTraverse (path, n) ;
        }
    }
}


void Node::visit (Node*)
{


}



// =========================== Class "ProDos" =================================



ProDos::ProDos (char* imageFileName)
{
    m_imageFileName = imageFileName ;

    m_file = open (imageFileName, O_RDONLY) ;
    if (m_file < 0) {
        printf ("\n*** Error: Can't open file %s; %s\n\n", imageFileName, strerror(errno)) ;
        exit(1) ;
    }
    
    struct stat st;
    stat (imageFileName, &st);
    int length = st.st_size;

    if (length < FIVE_INCH_FLOPPY_SIZE) {
        printf ("\n*** Error: \"%s\" is too small to be an Apple ProDos disk image. Quitting.\n\n", imageFileName) ;
        exit(1) ;
    }
    if (length < 819200) m_diskII = true ;
    else                 m_diskII = false ;

    int offset = findOffset() ;

    m_diskImage = (uint8*)calloc (length, 1) ;
    lseek (m_file, offset, SEEK_SET) ;
    read (m_file, (char*)m_diskImage, length-offset) ;

}


//  An annoying kludge...
//
//  Find the offset, in bytes, to the beginning of the actual disk data
//  contained in the disk image.
//
//  For reasons I cannot comprehend, many 800K 3.5" disk images contain
//  mystery junk(?) before the disk boot blocks.  The length of this
//  junk seems to be unpredictable.
//  So:  we search for the 4 bytes we expect to see as the 1st 4 bytes
//  the disk:
//
//  01      DB   #$01
//  38      SEC
//  b0 03   BCS  $0807
//

int ProDos::findOffset (void)
{
    uint8 buffer[BLOCKSIZE] ;

    lseek (m_file, 0, SEEK_SET) ;
    int n = read (m_file, buffer, BLOCKSIZE) ;
    if (n != BLOCKSIZE) {
        printf ("** Error reading %s: %s\n", m_imageFileName, strerror(errno)) ;
        exit(1) ;
    }

    for (n=0; n<BLOCKSIZE-3; n++) {
        if (buffer[n]==0x01 && buffer[n+1]==0x38 && buffer[n+2]==0xb0 && buffer[n+3]==0x03) break ;
    }
//    if (n > BLOCKSIZE-4) {
//        printf ("\n*** %s has a lot of junk in the 1st block; may not a ProDos disk image.\n\n", m_imageFileName) ;
//    }
    return n ;
}


char* ProDos::getDate (uint8* p)  // 'p' points to a 4-byte packed time & date
{
    const char* month[12] = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    } ;
    static char buffer[18] ;
    uint8 zero = p[0] | p[0] | p[2] | p[3] ;
    if (zero == 0) {
        sprintf (buffer, "<no date>      ") ;
    } else {
        int yy = p[1] >> 1 ;
        int mm = (p[1]&1)<<3 | (p[0]>>5)  ;
        if (mm > 12) mm = 1 ;
        int dd = p[0] & 0x1f ;
        int HH = p[3] & 0x1f ;
        int MM = p[2] & 0x3f ;
        sprintf (buffer, "%2i-%s-%2.2i %2i:%2.2i", dd, month[mm-1], yy, HH, MM) ;
    }
    return buffer ;
}


void ProDos::volumeInfo (void)
{
    dirHeader* header = (dirHeader*)(m_diskImage + BLOCKSIZE*2) ;

    int len = header->nameLen ;
    memcpy (m_volumeName, header->directoryName, 15) ;
    m_volumeName[len] = 0 ;

    printf ("\n") ;
    printf ("Volume  %s   in file \"%s\"\n\n", m_volumeName, m_imageFileName) ;
    printf ("Creation Date: %s\n", getDate(header->creation)) ;
    printf ("Version: %i\n", header->version) ;
    printf ("Min.Version: %i\n", header->minVersion) ;
    printf ("Access: 0x%2.2X\n", header->access) ;
    printf ("Total Blocks: %i\n", header->totalBlocks) ;
    printf ("\n") ;
}


void ProDos::extract (string)
{
  //  uint8* volDirHeader = buffer + 2*BLOCKSIZE + offset(buffer) ;

}

// ----------------------------------------------------------------------------


void usage(void)
{
    printf ("\nList directories or extractPath files from an Apple ProDos disk image.\n") ;

    printf ("\nUsage:\n") ;
    printf ("   prodos [-l dirname] [-r] [-x pathname] <name of image file>\n") ;
    printf ("Where:\n") ;
    printf ("  -h Print this 'help' message and exit\n") ;
    printf ("  -l List the named directory; use \"/\" for topmost directory\n") ;
    printf ("  -r Do a recurive directory list\n") ;
    printf ("  -x Extract a file from the image; use a full path name, with \"/\" delimiters\n\n") ;
    printf ("* If only the name of the image file is specified (with no options),\n") ;
    printf ("  the uppermost directory will be listed.\n") ;
    printf ("* If both \"-l\" and \"-x\" are used, only the directory list will be performed.\n\n") ;
}


int main (int argc, char *argv[])
{
    bool   badOption = false ;
 //   bool   recurse  = false;
 //   char   defaultDirName[2] {'/', 0} ;
 //   char*  dirName = defaultDirName ;
    char*  extractPath = NULL ;

    while (true) {
        int opt = getopt (argc, argv, "hl:rx:") ;
        if (opt == -1) break ;
        switch (opt) {
            case 'h':
               usage() ;
               exit(0) ;
               break ;
            case 'l':
 //              dirName = optarg ;
               break ;
            case 'r':
 //              recurse = true ;
               break ;
            case 'x':
               extractPath = optarg ;
               break ;
            default:
               badOption = true ;
               break ;
        }
    }

    if (badOption) {
        usage() ;
        exit(1) ;
    }

    char* fileName = argv[optind] ;
    if (fileName == NULL) {
        printf ("\n** missing filename\n") ;
        usage() ;
        exit(1) ;
    }

// ---------------------------------

    ProDos pDos (fileName) ;

    if (extractPath) {
        pDos.extract (extractPath) ;
    } else {
        pDos.volumeInfo() ;

        // 1... Create a directory tree
        dirHeader* hdr  = (dirHeader*)((uint64)(pDos.m_diskImage) + 0x400) ;
        dirEntry* entry = (dirEntry*)((uint64)(pDos.m_diskImage) + 0x400 + sizeof(dirHeader)) ;
        string path = "/" ;
        path.append (ProDos::m_volumeName) ;
        pDos.m_treeRoot = new Node (NULL, true, hdr, entry) ;

        // 2... Do an inorder traverse of the tree
        pDos.m_treeRoot->inorderTraverse (string(""), pDos.m_treeRoot) ; 
    }
}


